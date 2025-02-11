/**
  ******************************************************************************
  * @file    com1_485.c
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.27
  * @date    2014-01-03
  * @brief   COM1 USE USART3
	******************************************************************************
	*/
	
/* Includes ------------------------------------------------------------------*/
#include "com1_485.h"
#include "GlobalV_Extern.h"			// ȫ�ֱ�������
#include "GlobalConst.h"
#include "CRCdata.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8  Txd1Buffer[TXD1_MAX];	// ���ͻ�����
u8  Rcv1Buffer[RCV1_MAX];	// ���ջ�����
u16   Rcv1Counter; 					// ���ռ�����//
u16	 Txd1Counter;						// ���ͼ�����//
u16   BakRcv1Count; 					// ���ռ�����//
u16   Txd1Max;								// �ж��ٸ��ַ���Ҫ����//
u16   w_Txd1ChkSum; 					// ����У��ͣ�lo,hi ��λ//
u16	 w_Com1RegAddr;					// ����2�Ĵ�����ַ
u8  T_NoRcv1Count;					// û�н��ռ�����
u16	 C_NoRcv1Count;
u8	 B_PcSetGprs;						// PC����GPRS��־ 2013.6.5
//
u8  B_Com1Cmd03;
u8  B_Com1Cmd16;
u8  B_Com1Cmd01;
u8  B_Com1Cmd06;

/* Private variables extern --------------------------------------------------*/
extern	u16	C_ForceSavPar;		// ǿ�Ʊ������������
extern	u8 B_ForceSavPar;					//ǿ�Ʊ��������־
extern	u8 F_ModTime;

/* Private function prototypes -----------------------------------------------*/
void GPIO_Com1_Configuration(void);							//GPIO����
void Com1_config(void);
u16 CRC16(u8 * pCrcData,u8 CrcDataLen);

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : GPIO_Com1_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Com1_Configuration(void)							//GPIO���� 
{
  GPIO_InitTypeDef GPIO_InitStructure;					//����GPIO��ʼ���ṹ��
	
  /* Configure 232  */  		// RS232û�п��ƶ�
  /* Configure Com0(ʹ��Ӳ��:USART1) Tx (PA.9) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure Com0(ʹ��Ӳ��:USART1) Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
  /* Configure 485 ���ƶ� PC.15 */  
	/* PC13��PC14��PC15����ͨ����Դ���ؽ��й��磬���������������Ϊ�������ʱ���������ƣ�
	��Ϊ�����ʱֻ�ܹ�����2MHzģʽ��,�����������Ϊ30pF, ͬһʱ�䣬����������ֻ��һ������
	����Ϊ������š� */ 	
/*   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
  // Com1���ƶ�
  RS485_CON=0;   	 */
}

/* USART1  configuration ------------------------------------------------------*/
/* USART  configured as follow:
        - BaudRate = 19200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
				RTS/CTSЭ��(Request To Send/Clear To Send)��������/������Э��				
*/
void Com1_config(void)
{
	USART_InitTypeDef USART_InitStructure;
	Pw_BaudRate1=9600;
	USART_InitStructure.USART_BaudRate = Pw_BaudRate1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Configure Com1-USART1 */
	USART_Init(USART1, &USART_InitStructure);  
  /* Enable USART1 Receive and Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);			//�����ж�
	
  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);		

}

/*******************************************************************************
* Function Name  : Com1_Init
* Description    : Initializes the peripherals used by the Usart driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Com1_Init(void)
{
  GPIO_Com1_Configuration();
  Com1_config();
	//
	Txd1Counter=0;
	Rcv1Counter=0;
	BakRcv1Count=0;
	RS485_CON=0;
}

//���մ������ У�����
void Com1_RcvProcess(void)
{
	u8 k,s,i=0;       						// ��ʱ����
  u16 j;
	//��Ϊ����,ָ������ʱ�䵽��,�Ϳ��Դ�����յ����ַ�����	
	// ��û�յ������ַ���ʱ�䳬���趨ʱ�����ԶԽ��ջ�����д�����
	// **********************************rcv_counter<>0,�յ��ַ����ܴ���
	if ( Rcv1Counter>0 &&  T_NoRcv1Count!=SClk1Ms )
	{								// ���մ������
		RS485_CON=0;
		T_NoRcv1Count=SClk1Ms;				// 
		C_NoRcv1Count++;
		if ( C_NoRcv1Count>NORCVMAXMS )				// 	
		{
			//	
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);		// ��ֹUART1�����ж�,��ֹ�ڴ����ڼ����					
			BakRcv1Count=Rcv1Counter;		// �� Rcv1Counter ����
			C_NoRcv1Count=0;				// ��û�н��ռ�����
			//
			if(BakRcv1Count<=RCV1_MAX)		// ���ճ�����ȷ,��������.
			{
				if( Rcv1Buffer[0]==2 )		// �ӵ�ַ��⣭���յ�����λ����ѯָ��
				{	
					j=CRC16( Rcv1Buffer, BakRcv1Count-2);		// CRC У��
					k=j>>8;
					s=j;
					if ( k==Rcv1Buffer[BakRcv1Count-2] 
						&& s==Rcv1Buffer[BakRcv1Count-1] )
					{											// CRCУ����ȷ
						if ( Rcv1Buffer[1]==3 )		// 03��ȡ���ּĴ���
						{
							B_Com1Cmd03=1;
							j=Rcv1Buffer[2];
							w_Com1RegAddr=(j<<8)+Rcv1Buffer[3];
						}
						else if ( Rcv1Buffer[1]==16 )	// 16Ԥ�ö�Ĵ���
						{
							C_ForceSavPar=0;		// ǿ�Ʊ������������=0							
							B_Com1Cmd16=1;
							j=Rcv1Buffer[2];
							w_Com1RegAddr=(j<<8)+Rcv1Buffer[3];
						}
						else if ( Rcv1Buffer[1]==1 )		// 01��ȡ��Ȧ״̬
						{
							B_Com1Cmd01=1;
						}
						else if ( Rcv1Buffer[1]==6 )		// 06Ԥ�õ��Ĵ���
						{
							C_ForceSavPar=0;		// ǿ�Ʊ������������=0							
							B_Com1Cmd06=1;
							j=Rcv1Buffer[2];
							w_Com1RegAddr=(j<<8)+Rcv1Buffer[3];
						}
						else
							i=1;
					}
					else
						i=2;
				}
				else
					i=3;

			}
			else
				i=4;
			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
			Rcv1Counter=0;					// ׼���´ν��յ����濪ʼ
		}
	}
	if(i>0)
	{
		for(j=0;j<20;j++)
		{
			Rcv1Buffer[j]=0;
		}
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		
	}
}


void Com1_SlaveSend(void)			// ����2�ӻ�����  
{
	u16	m,n; 
	u8	j=0,k;
	u16	 *p_wRead;
	u8	 *p_bMove;
	u8	 *p_bGen;
	u16	 *p_wTarget;			// ָ��Ŀ���ַ�����xdata zcl

	//
	if ( B_Com1Cmd03 )		// ��ȡ���ּĴ���
	{
		Txd1Buffer[0]=2;				// �豸�ӵ�ַPw_EquipmentNo
		Txd1Buffer[1]=Rcv1Buffer[1];	// ������
		Txd1Buffer[2]=Rcv1Buffer[5]*2;	// Rcv1Buffer[5]=���� ��
		//
		p_wRead=w_ParLst;			// PAR��
		p_bMove=Txd1Buffer;
		for ( k=0;k<Rcv1Buffer[5] ;k++ )	// ����ѯ����
		{
			m=*(p_wRead+w_Com1RegAddr+k);	
			*(p_bMove+3+k*2)=m>>8;
			*(p_bMove+3+k*2+1)=m;
		}
		w_Txd1ChkSum=CRC16( Txd1Buffer, Txd1Buffer[2]+3 );
		Txd1Buffer[Txd1Buffer[2]+3]=w_Txd1ChkSum>>8;			// /256
		Txd1Buffer[Txd1Buffer[2]+4]=w_Txd1ChkSum;				// ��λ�ֽ�
		Txd1Max=Txd1Buffer[2]+5;
		//
		//
		B_Com1Cmd03=0;
		Txd1Counter=0;
		RS485_CON=1;
		// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����		
		//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);				// ��ʼ����.
		USART_ITConfig(USART1, USART_IT_TC, ENABLE);				// ��ʼ����.			
		USART_SendData(USART1,Txd1Buffer[Txd1Counter++]); 				
	}
	//
	else if ( B_Com1Cmd16 || B_Com1Cmd06 )					// 16Ԥ�ö�Ĵ���
	{
		// ��� ����ֱ���޸ĵĲ���
		if ( w_Com1RegAddr==4 )  // 
			j=1;

		// ��Ҫ����ſ����޸ĵĲ���(��Ҫ�Ȱ�Pw_ModPar �޸ĳɹ涨ֵ�������޸ĵĲ���)
		else //if ( Pw_ModPar==10000 )	
			j=1;

		// �޸Ĳ�����Ԫ
		if ( j )					
		{
			if ( w_Com1RegAddr>=10 && w_Com1RegAddr<=16 )			// �޸�ʱ��
			{
				w_ModTimeNo=w_Com1RegAddr-10;
				F_ModTime=1;
			}				
			if ( B_Com1Cmd06 )		// Ԥ�õ���
			{
				m=Rcv1Buffer[4];
				w_ParLst[w_Com1RegAddr]=(m<<8)+Rcv1Buffer[5];
			}
			else if ( B_Com1Cmd16 )	// Ԥ�ö��
			{
				if ( Rcv1Buffer[5]<90 )
				{
					p_bGen=Rcv1Buffer;
					p_wTarget=w_ParLst;
					for ( k=0;k<Rcv1Buffer[5] ;k++ )		// Rcv1Buffer[5]=����
					{	
						m = *(p_bGen+7+k*2);
						n = *(p_bGen+7+k*2+1);
						*(p_wTarget+w_Com1RegAddr+k)= (m<<8) + n;	
					}	
				}
			}
		}


		// -------------------------
		// ��������
		Txd1Buffer[0]=2;				// �豸�ӵ�ַ
		Txd1Buffer[1]=Rcv1Buffer[1];	// ������
		Txd1Buffer[2]=Rcv1Buffer[2];	// ��ʼ��ַ��λ�ֽ�
		Txd1Buffer[3]=Rcv1Buffer[3];	// ��ʼ��ַ��λ�ֽ�
		Txd1Buffer[4]=Rcv1Buffer[4];	// �Ĵ���������λ
		Txd1Buffer[5]=Rcv1Buffer[5];	// �Ĵ���������λ	
		if ( j==0 )							// ������ܱ�����Ԥ�ã�����FFFF zcl
		{
			Txd1Buffer[4]=0xff;				// �Ĵ���������λ��Ԥ������
			Txd1Buffer[5]=0xff;				// �Ĵ���������λ��Ԥ������
		}
		w_Txd1ChkSum=CRC16( Txd1Buffer, 6);
		Txd1Buffer[6]=w_Txd1ChkSum>>8;		// /256
		Txd1Buffer[7]=w_Txd1ChkSum;				// ��λ�ֽ�
		Txd1Max=8;
		B_Com1Cmd16=0;
		B_Com1Cmd06=0;
		Txd1Counter=0;
		RS485_CON=1;		
		// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����		
		//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);				// ��ʼ����.
		USART_ITConfig(USART1, USART_IT_TC, ENABLE);				// ��ʼ����.
		USART_SendData(USART1,Txd1Buffer[Txd1Counter++]); 		

	}// 06��16Ԥ�üĴ��� ����
}

// ��Ϊ��ģ��ʱ����
void Com1_MasterSend(void)		// ����2�����ͳ���
{

}

u16 CRC16(u8 * pCrcData,u8 CrcDataLen)
{
	u8 CRC16Hi=0xff;                   /* ��CRC�ֽڳ�ʼ�� */
  u8 CRC16Lo=0xff;                   /* ��CRC�ֽڳ�ʼ��*/
	u8 Index=0;
	u16  w_CRC16=0;
	while(CrcDataLen--)
	{
		Index = CRC16Hi ^* pCrcData++;
		CRC16Hi = CRC16Lo ^ CRC_H[Index];
		CRC16Lo = CRC_L[Index];
	}
	w_CRC16 = (CRC16Hi << 8) | CRC16Lo; 
	return (w_CRC16);
}

/*******************************************************************************
* Function Name  : fputc
* Description    : Retargets the C library printf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
	RS485_CON=1;
  USART_SendData(USART1, (u8) ch);

  /* Loop until the end of transmission */
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {
  }
	RS485_CON=0;
  return ch;
}


/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
