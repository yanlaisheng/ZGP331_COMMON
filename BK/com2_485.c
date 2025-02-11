/**
  ******************************************************************************
  * @file    com2_485.c
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.31
  * @date    2014-04-08
  * @brief   COM1 USE USART2
	******************************************************************************
	*/
	
/* Includes ------------------------------------------------------------------*/
#include "com2_485.h"
#include "GlobalV_Extern.h"			// ȫ�ֱ�������
#include "GlobalConst.h"
//#include "CRCdata.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8  Txd2Buffer[TXD2_MAX];	// ���ͻ�����
u8  Rcv2Buffer[RCV2_MAX];	// ���ջ�����
u16   Rcv2Counter; 					// ���ռ�����//
u16	 Txd2Counter;						// ���ͼ�����//
u16   BakRcv2Count; 					// ���ռ�����//
u16   Txd2Max;								// �ж��ٸ��ַ���Ҫ����//
u16   w_Txd2ChkSum; 					// ����У��ͣ�lo,hi ��λ//
u16	 w_Com2RegAddr;					// ����2�Ĵ�����ַ
u8  T_NoRcv2Count;					// û�н��ռ�����
u16	 C_NoRcv2Count;
//
u8  B_Com2Cmd03;
u8  B_Com2Cmd16;
u8  B_Com2Cmd01;
u8  B_Com2Cmd06;

/* Private variables extern --------------------------------------------------*/
extern	u16	C_ForceSavPar;		// ǿ�Ʊ������������
extern	u8 B_ForceSavPar;					//ǿ�Ʊ��������־
extern	u8 F_ModTime;

/* Private function prototypes -----------------------------------------------*/
void GPIO_Com2_Configuration(void);							//GPIO����
void Com2_config(void);
u16 CRC16(u8 * pCrcData,u8 CrcDataLen);

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : GPIO_Com2_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Com2_Configuration(void)							//GPIO���� 
{
  GPIO_InitTypeDef GPIO_InitStructure;					//����GPIO��ʼ���ṹ��
	
  /* Configure Com1(ʹ��Ӳ��:USART1) Rx (PA.3) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Configure Com1(ʹ��Ӳ��:USART1) Tx (PA.2) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure 485 ���ƶ� PC.15 */  
	/* PC13��PC14��PC15����ͨ����Դ���ؽ��й��磬���������������Ϊ�������ʱ���������ƣ�
	��Ϊ�����ʱֻ�ܹ�����2MHzģʽ��,�����������Ϊ30pF, ͬһʱ�䣬����������ֻ��һ������
	����Ϊ������š�	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);	 */ 
  // Com2���ƶ�
  // RS485_CON=0;   	
}

/* USART2  configuration ------------------------------------------------------*/
/* USART  configured as follow:
        - BaudRate = 19200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
				RTS/CTSЭ��(Request To Send/Clear To Send)��������/������Э��				
*/
void Com2_config(void)
{
	USART_InitTypeDef USART_InitStructure;
	Pw_BaudRate2=9600;
	USART_InitStructure.USART_BaudRate = Pw_BaudRate2;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Configure Com2-USART2 */
	USART_Init(USART2, &USART_InitStructure);  
  /* Enable USART2 Receive and Transmit interrupts */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);			//�����ж�
	
  /* Enable the USART2 */
  USART_Cmd(USART2, ENABLE);		

}

/*******************************************************************************
* Function Name  : Com2_Init
* Description    : Initializes the peripherals used by the Usart driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Com2_Init(void)
{
  GPIO_Com2_Configuration();
  Com2_config();
	//
	Txd2Counter=0;
	Rcv2Counter=0;
	BakRcv2Count=0;
	// RS485_CON=0;
}

//���մ������ У�����
void Com2_RcvProcess(void)
{
	u8 k,s,i=0;       						// ��ʱ����
  u16 j;
	//��Ϊ����,ָ������ʱ�䵽��,�Ϳ��Դ�����յ����ַ�����	
	// ��û�յ������ַ���ʱ�䳬���趨ʱ�����ԶԽ��ջ�����д�����
	// **********************************rcv_counter<>0,�յ��ַ����ܴ���
	if ( Rcv2Counter>0 &&  T_NoRcv2Count!=SClk1Ms )
	{								// ���մ������
		// RS485_CON=0;
		T_NoRcv2Count=SClk1Ms;				// 
		C_NoRcv2Count++;
		if ( C_NoRcv2Count>NORCVMAXMS )				// 	
		{
			//	
			USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);		// ��ֹUART1�����ж�,��ֹ�ڴ����ڼ����					
			BakRcv2Count=Rcv2Counter;		// �� Rcv2Counter ����
			C_NoRcv2Count=0;				// ��û�н��ռ�����
			//
			if(BakRcv2Count<=RCV2_MAX)		// ���ճ�����ȷ,��������.
			{
				if( Rcv2Buffer[0]==2 )		// �ӵ�ַ��⣭���յ�����λ����ѯָ��
				{	
					j=CRC16( Rcv2Buffer, BakRcv2Count-2);		// CRC У��
					k=j>>8;
					s=j;
					if ( k==Rcv2Buffer[BakRcv2Count-2] 
						&& s==Rcv2Buffer[BakRcv2Count-1] )
					{											// CRCУ����ȷ
						if ( Rcv2Buffer[1]==3 )		// 03��ȡ���ּĴ���
						{
							B_Com2Cmd03=1;
							j=Rcv2Buffer[2];
							w_Com2RegAddr=(j<<8)+Rcv2Buffer[3];
						}
						else if ( Rcv2Buffer[1]==16 )	// 16Ԥ�ö�Ĵ���
						{
							C_ForceSavPar=0;		// ǿ�Ʊ������������=0							
							B_Com2Cmd16=1;
							j=Rcv2Buffer[2];
							w_Com2RegAddr=(j<<8)+Rcv2Buffer[3];
						}
						else if ( Rcv2Buffer[1]==1 )		// 01��ȡ��Ȧ״̬
						{
							B_Com2Cmd01=1;
						}
						else if ( Rcv2Buffer[1]==6 )		// 06Ԥ�õ��Ĵ���
						{
							C_ForceSavPar=0;		// ǿ�Ʊ������������=0							
							B_Com2Cmd06=1;
							j=Rcv2Buffer[2];
							w_Com2RegAddr=(j<<8)+Rcv2Buffer[3];
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
			USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
			Rcv2Counter=0;					// ׼���´ν��յ����濪ʼ
		}
	}
	if(i>0)
	{
		for(j=0;j<20;j++)
		{
			Rcv2Buffer[j]=0;
		}
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		
	}
}


void Com2_SlaveSend(void)			// ����2�ӻ�����  
{
	u16	m,n; 
	u8	j=0,k;
	u16	 *p_wRead;
	u8	 *p_bMove;
	u8	 *p_bGen;
	u16	 *p_wTarget;			// ָ��Ŀ���ַ�����xdata zcl

	//
	if ( B_Com2Cmd03 )		// ��ȡ���ּĴ���
	{
		Txd2Buffer[0]=2;				// �豸�ӵ�ַPw_EquipmentNo
		Txd2Buffer[1]=Rcv2Buffer[1];	// ������
		Txd2Buffer[2]=Rcv2Buffer[5]*2;	// Rcv2Buffer[5]=���� ��
		//
		p_wRead=w_ParLst;			// PAR��
		p_bMove=Txd2Buffer;
		for ( k=0;k<Rcv2Buffer[5] ;k++ )	// ����ѯ����
		{
			m=*(p_wRead+w_Com2RegAddr+k);	
			*(p_bMove+3+k*2)=m>>8;
			*(p_bMove+3+k*2+1)=m;
		}
		w_Txd2ChkSum=CRC16( Txd2Buffer, Txd2Buffer[2]+3 );
		Txd2Buffer[Txd2Buffer[2]+3]=w_Txd2ChkSum>>8;			// /256
		Txd2Buffer[Txd2Buffer[2]+4]=w_Txd2ChkSum;				// ��λ�ֽ�
		Txd2Max=Txd2Buffer[2]+5;
		//
		//
		B_Com2Cmd03=0;
		Txd2Counter=0;
		// RS485_CON=1;
		// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����		
		//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);				// ��ʼ����.
		USART_ITConfig(USART2, USART_IT_TC, ENABLE);				// ��ʼ����.			
		USART_SendData(USART2,Txd2Buffer[Txd2Counter++]); 				
	}
	//
	else if ( B_Com2Cmd16 || B_Com2Cmd06 )					// 16Ԥ�ö�Ĵ���
	{
		// ��� ����ֱ���޸ĵĲ���
		if ( w_Com2RegAddr==4 )  // 
			j=1;

		// ��Ҫ����ſ����޸ĵĲ���(��Ҫ�Ȱ�Pw_ModPar �޸ĳɹ涨ֵ�������޸ĵĲ���)
		else //if ( Pw_ModPar==10000 )	
			j=1;

		// �޸Ĳ�����Ԫ
		if ( j )					
		{
			if ( w_Com2RegAddr>=10 && w_Com2RegAddr<=16 )			// �޸�ʱ��
			{
				w_ModTimeNo=w_Com2RegAddr-10;
				F_ModTime=1;
			}				
			if ( B_Com2Cmd06 )		// Ԥ�õ���
			{
				m=Rcv2Buffer[4];
				w_ParLst[w_Com2RegAddr]=(m<<8)+Rcv2Buffer[5];
			}
			else if ( B_Com2Cmd16 )	// Ԥ�ö��
			{
				if ( Rcv2Buffer[5]<90 )
				{
					p_bGen=Rcv2Buffer;
					p_wTarget=w_ParLst;
					for ( k=0;k<Rcv2Buffer[5] ;k++ )		// Rcv2Buffer[5]=����
					{	
						m = *(p_bGen+7+k*2);
						n = *(p_bGen+7+k*2+1);
						*(p_wTarget+w_Com2RegAddr+k)= (m<<8) + n;	
					}	
				}
			}
		}


		// -------------------------
		// ��������
		Txd2Buffer[0]=2;				// �豸�ӵ�ַ
		Txd2Buffer[1]=Rcv2Buffer[1];	// ������
		Txd2Buffer[2]=Rcv2Buffer[2];	// ��ʼ��ַ��λ�ֽ�
		Txd2Buffer[3]=Rcv2Buffer[3];	// ��ʼ��ַ��λ�ֽ�
		Txd2Buffer[4]=Rcv2Buffer[4];	// �Ĵ���������λ
		Txd2Buffer[5]=Rcv2Buffer[5];	// �Ĵ���������λ	
		if ( j==0 )							// ������ܱ�����Ԥ�ã�����FFFF zcl
		{
			Txd2Buffer[4]=0xff;				// �Ĵ���������λ��Ԥ������
			Txd2Buffer[5]=0xff;				// �Ĵ���������λ��Ԥ������
		}
		w_Txd2ChkSum=CRC16( Txd2Buffer, 6);
		Txd2Buffer[6]=w_Txd2ChkSum>>8;		// /256
		Txd2Buffer[7]=w_Txd2ChkSum;				// ��λ�ֽ�
		Txd2Max=8;
		B_Com2Cmd16=0;
		B_Com2Cmd06=0;
		Txd2Counter=0;
		// RS485_CON=1;		
		// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����		
		//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);				// ��ʼ����.
		USART_ITConfig(USART2, USART_IT_TC, ENABLE);				// ��ʼ����.
		USART_SendData(USART2,Txd2Buffer[Txd2Counter++]); 		

	}// 06��16Ԥ�üĴ��� ����
}

// ��Ϊ��ģ��ʱ����
void Com2_MasterSend(void)		// ����2�����ͳ���
{

}



/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
