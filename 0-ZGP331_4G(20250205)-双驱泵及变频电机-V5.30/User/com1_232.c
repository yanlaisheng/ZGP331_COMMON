/**
  ******************************************************************************
  * @file    com1_232.c
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.10
  * @date    2015-04-13
  * @brief   Com1 USE USART1
	******************************************************************************
	* @note	2015.04.13
		COM1: Һ������ƵͨѶ
	*/

/* Includes ------------------------------------------------------------------*/
#include "com1_232.h"
#include "GlobalV_Extern.h" // ȫ�ֱ�������
#include "GlobalConst.h"
#include <stdio.h> //���ϴ˾������printf
#include "CRCdata.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 Txd1Buffer[TXD1_MAX];	  // ���ͻ�����
u8 Rcv1Buffer[RCV1_MAX];	  // ���ջ�����
u8 Txd1Buffer_TEMP[TXD1_MAX]; // ���ͻ�����
u16 Cw_Rcv1;				  // ���ռ�����//
u16 Cw_Txd1;				  // ���ͼ�����//
u16 Cw_BakRcv1;				  // ���ռ�����//
u16 Cw_Txd1Max;				  // �ж��ٸ��ַ���Ҫ����//
u16 Lw_Txd1ChkSum;			  // ����У��ͣ�lo,hi ��λ//
u16 Lw_Com1RegAddr;			  // ����1�Ĵ�����ַ
u16 w_Txd1ChkSum;			  // ����У��ͣ�lo,hi ��λ//
u16 w_Com1RegAddr;			  // ����1�Ĵ�����ַ
u16 Txd1Max;				  // �ж��ٸ��ַ���Ҫ����//
u16 Txd1Counter;			  // ���ͼ�����//
u16 BakRcv1Count;			  // ���ռ�����//
//
u8 B_Com1Cmd03;
u8 B_Com1Cmd16;
u8 B_Com1Cmd01;
u8 B_Com1Cmd06;
u8 B_Com1Cmd73;
u8 T_NoRcv1; // û�н��ռ�����
u16 C_NoRcv1;

u8 S_Com1SendNoNum; // Com1���ʹ���
u8 S_Com1Send;		// Com1���Ͳ���
u8 F_Com1Send;		// Com1���ͱ�־
u8 T_Com1Send;
u16 C_Com1Send;

u8 T_Com1SendDelay;
u16 C_Com1SendDelay;
u8 S_RdWrNo; // ��д���

u8 B_ModYW310SZM220; // ZCL 2019.10.19
u8 B_Com1Con;		 // ZCL 2019.9.22
u8 T_Com1Err1;
u16 C_Com1Err1;
u8 T_Com1Err2;
u16 C_Com1Err2;
u8 T_Com1NormalNext;
u16 C_Com1NormalNext;
u8 T_Com1OvertimeNext;
u16 C_Com1OvertimeNext;
u8 B_Com1Send1;
u8 B_Com1Send2;
u8 B_Com1Sd1End;
u8 B_Com1Sd2End;
u8 S_Com1MSend;
u8 F_Com1SendNext;
u8 F_Com1RCVCommandOk;

u16 Lw_Com1EquipmentNo1; // ZCL 2022.9.13
u16 Lw_Com1EquipmentNo2; // ZCL 2022.9.13
/* Private variables extern --------------------------------------------------*/
extern u16 C_ForceSavPar; // ǿ�Ʊ������������
extern u8 F_ModStm32Par;  // �޸�STM32����

// ZCL 2018.5.14
extern u8 S_ModParStatus;	   // �޸Ĳ���ȷ�Ϻ�ָ����һ������
extern u8 ModParNo;			   // �޸Ĳ������++
extern u8 ModParBitPos;		   // �޸Ĳ���λλ��=0
extern u8 S_OldModParStatus;   // �ɵ��޸Ĳ���״̬������Ա��ã�ZCL 2018.5.16
extern u8 B_PressedStopRunKey; // ����STOP����RUN�� 2016.12.6
extern u8 B_ForceSavPar;	   // ǿ�Ʊ��������־
/* Private function prototypes -----------------------------------------------*/
void GPIO_Com1_Configuration(void); // GPIO����
void Com1_config(void);
u16 CRC16(u8 *pCrcData, u8 CrcDataLen);
void Delay_MS(vu16 nCount);

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
 * Function Name  : GPIO_Com1_Configuration
 * Description    : Configures the different GPIO ports.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void GPIO_Com1_Configuration(void) // GPIO����
{
	GPIO_InitTypeDef GPIO_InitStructure; // ����GPIO��ʼ���ṹ��

	// ����˵����USART1(USART���±��1��ʼ,��STM32F103 ARM�Ĵ�������)
	// Com1 ʵ��Ϊ��һ������
	/* Configure 485  */ // RS485û�п��ƶ�
	/* Configure Com1(ʹ��Ӳ��:USART1) Tx (PA.9) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure Com1(ʹ��Ӳ��:USART1) Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/* USART1  configuration ------------------------------------------------------*/
/* USART  configured as follow:
		- BaudRate = 9600 baud
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
	USART_DeInit(USART1);					// ��λ Usart
	USART_StructInit(&USART_InitStructure); // ��USART_InitStruct�е�ÿһ��������ȱʡֵ����

	USART_InitStructure.USART_BaudRate = Pw_GprsBaudRate2; // 57600��9600;

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure ����1-USART1 */
	USART_Init(USART1, &USART_InitStructure);
	/* Enable USART1 Receive and Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
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
	Cw_Txd1 = 0;
	Cw_Rcv1 = 0;
}

// ���մ������ У�����
void Com1_RcvProcess(void)
{
	u8 k, s, i = 0; // ��ʱ����
					//	u8 err_num = 0;
	u16 j;
	u16 *p_wRead;

	// ��Ϊ����,ָ������ʱ�䵽��,�Ϳ��Դ�����յ����ַ�����
	//  ��û�յ������ַ���ʱ�䳬���趨ʱ�����ԶԽ��ջ�����д�����
	//  **********************************rcv_counter<>0,�յ��ַ����ܴ���
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Cw_Rcv1 > 0 && T_NoRcv1 != SClk1Ms)
		{ // ���մ������
			// RS485_CON=0;
			T_NoRcv1 = SClk1Ms; //
			C_NoRcv1++;
			if (C_NoRcv1 > NORCVMAXMS) //
			{
				//
				USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); // ��ֹUART1�����ж�,��ֹ�ڴ����ڼ����
				Cw_BakRcv1 = Cw_Rcv1;							// �� Cw_Rcv1 ����
				C_NoRcv1 = 0;									// ��û�н��ռ�����
				//
				if (Cw_BakRcv1 <= RCV1_MAX) // ���ճ�����ȷ,��������.
				{
					if (Rcv1Buffer[0] == Pw_LoRaEquipmentNo // ZCL 2019.10.20  ���մӻ��ķ������ݣ�
						|| B_ModYW310SZM220 == 1			// ZCL 2019.10.20  ��д YW310��SZM220�еĲ��������Բ���У���ַ
					)
					{
						j = CRC16(Rcv1Buffer, Cw_BakRcv1 - 2); // CRC У��
						k = j >> 8;
						s = j;
						if (k == Rcv1Buffer[Cw_BakRcv1 - 2] && s == Rcv1Buffer[Cw_BakRcv1 - 1])
						{							// CRCУ����ȷ
							if (Rcv1Buffer[1] == 3) // 03��ȡ���ּĴ���
							{

								// Lw_Reg ��Read_Stm32Data( Address(&Pw_SoftOverPEn,0), 1 ) �и�ֵ��
								// ת����ַ ZCL 2015.9.1 �������������ӻ��ķ�������

								/*ZCL 2017.6.18 �л�ҳ��ʱ���������������淢��ǰ��һҳ�����ݷ���������ʱ�����͵���һҳ�ϣ�
										��������������ĳɷ��ص����ݣ�����ǰ��Txd1Buffer�ķ��͵�ַ����ֵ�����ں��ˣ� OK*/
								j = Txd1Buffer[2];
								Lw_Reg = (j << 8) + Txd1Buffer[3];

								// ZCL 2019.10.19 Address()���е�ַ�ı任�� ע�⣺Com1_RcvProcess����ʱ�����е�ַ�ķ��任
								if (B_ModYW310SZM220 == 1) // ZCL 2019.10.19 ��д YW310��SZM220�еĲ�������ַҪ�任��
								{
									p_wRead = w_ParLst; // ��STM32 PAR��

									// ZCL 2019.10.19  ��ַ���
									// YW310
									if (Lw_Reg == 9)
										Lw_Reg = 260;
									else if (Lw_Reg == 23)
										Lw_Reg = 261;
									else if (Lw_Reg == 11)
										Lw_Reg = 262;
									else if (Lw_Reg == 156)
										Lw_Reg = 263;
									else if (Lw_Reg == 157)
										Lw_Reg = 264;
									else if (Lw_Reg == 142)
										Lw_Reg = 265; // ZCL 2019.10.20

									else if (Lw_Reg == 1)
										Lw_Reg = 256;
									else if (Lw_Reg == 2)
										Lw_Reg = 257;
									else if (Lw_Reg == 3)
										Lw_Reg = 258;
									else if (Lw_Reg == 4)
										Lw_Reg = 259; // ZCL 2019.10.21
									else if (Lw_Reg == 54)
										Lw_Reg = 266;
									else if (Lw_Reg == 60)
										Lw_Reg = 267; // ZCL 2019.10.21

									// SZM220
									else if (Lw_Reg == 212)
										Lw_Reg = 268;
									else if (Lw_Reg == 213)
										Lw_Reg = 269;
									else if (Lw_Reg == 218)
										Lw_Reg = 270;
									else if (Lw_Reg == 226)
										Lw_Reg = 271;
									else if (Lw_Reg == 227)
										Lw_Reg = 272;
									else if (Lw_Reg == 228)
										Lw_Reg = 273;
									else if (Lw_Reg == 229)
										Lw_Reg = 274;
									else if (Lw_Reg == 230)
										Lw_Reg = 275;
									else if (Lw_Reg == 231)
										Lw_Reg = 276;
									else if (Lw_Reg == 232)
										Lw_Reg = 277;
									else if (Lw_Reg == 233)
										Lw_Reg = 278;
									else if (Lw_Reg == 234)
										Lw_Reg = 279;
									else if (Lw_Reg == 235)
										Lw_Reg = 280;
									else if (Lw_Reg == 236)
										Lw_Reg = 281;
									else if (Lw_Reg == 237)
										Lw_Reg = 282;
									else if (Lw_Reg == 238)
										Lw_Reg = 283;
									else if (Lw_Reg == 501)
										Lw_Reg = 286;
									else if (Lw_Reg == 641)
										Lw_Reg = 287;
									else if (Lw_Reg == 640)
										Lw_Reg = 288; // ZCL 2019.10.20

									p_wRead += Lw_Reg;
									B_ModYW310SZM220 = 0;
								}
								else if (w_ZhouShanProtocol_bit9) // ������ѯ
								{
									F_Com1RCVCommandOk = 1;
									T_Com1NormalNext = 100;
									C_Com1NormalNext = 0;
									T_Com1OvertimeNext = 100;
									C_Com1OvertimeNext = 0;
									T_Com1Err1 = 100;
									C_Com1Err1 = 0;
									B_Com1ErrWithModule1 = 0; //
									B_Com1Send1 = 0;
									B_Com1Sd1End = 0;
									// w_Com1RcVOkCounter++; // ����  2023.3.2
								}
								else
								{
									B_Com1Cmd03 = 1;
									j = Rcv1Buffer[2];
									Lw_Com1RegAddr = (j << 8) + Rcv1Buffer[3];
								}
							}
							else if (Rcv1Buffer[1] == 6) // 06Ԥ�õ��Ĵ���
							{
								// F_Com1Send = 11;
								C_ForceSavPar = 0; // ǿ�Ʊ������������=0
								B_Com1Cmd06 = 1;
								j = Rcv1Buffer[2];
								Lw_Com1RegAddr = (j << 8) + Rcv1Buffer[3];
							}
							else if (Rcv1Buffer[1] == 16) // 16Ԥ�ö�Ĵ���
							{
								// F_Com1Send = 12;
								C_ForceSavPar = 0; // ǿ�Ʊ������������=0
								B_Com1Cmd16 = 1;
								j = Rcv1Buffer[2];
								Lw_Com1RegAddr = (j << 8) + Rcv1Buffer[3];
							}
							else
								i = 1;

							// ZCL 2019.10.20 ���յ���ȷָ����ռ�����+1
							//						w_Com1RXCounter++;
						}
						else
							i = 2;
					}
					else
						i = 3;
				}
				else
					i = 4;
				USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
				Cw_Rcv1 = 0; // ׼���´ν��յ����濪ʼ
			}
		}
	}
	else // ��Ƶ���
	{
		if (Cw_Rcv1 > 0 && T_NoRcv1 != SClk1Ms)
		{ // ���մ������
			// RS485_CON=0;
			T_NoRcv1 = SClk1Ms; //
			C_NoRcv1++;
			if (C_NoRcv1 > NORCVMAXMS) //
			{
				//
				USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); // ��ֹUART1�����ж�,��ֹ�ڴ����ڼ����
				Cw_BakRcv1 = Cw_Rcv1;							// �� Cw_Rcv1 ����
				C_NoRcv1 = 0;									// ��û�н��ռ�����
				//
				if (Cw_BakRcv1 <= RCV1_MAX) // ���ճ�����ȷ,��������.
				{
					if (Rcv1Buffer[0] == 2) // �ӵ�ַ��⣭���յ�����λ����ѯָ��
					{
						j = CRC16(Rcv1Buffer, Cw_BakRcv1 - 2); // CRC У��
						k = j >> 8;
						s = j;
						if (k == Rcv1Buffer[Cw_BakRcv1 - 2] && s == Rcv1Buffer[Cw_BakRcv1 - 1])
						{							// CRCУ����ȷ
							if (Rcv1Buffer[1] == 3) // 03��ȡ���ּĴ���
							{

								// Lw_Reg ��Read_Stm32Data( Address(&Pw_SoftOverPEn,0), 1 ) �и�ֵ��
								// ת����ַ ZCL 2015.9.1 �������������ӻ��ķ�������

								/*ZCL 2017.6.18 �л�ҳ��ʱ���������������淢��ǰ��һҳ�����ݷ���������ʱ�����͵���һҳ�ϣ�
										��������������ĳɷ��ص����ݣ�����ǰ��Txd1Buffer�ķ��͵�ַ����ֵ�����ں��ˣ� OK*/
								j = Txd1Buffer[2];
								Lw_Reg = (j << 8) + Txd1Buffer[3];

								if (Lw_Reg < STM32_RPAR_ADDRESS)
								{
									p_wRead = Pw_ParLst; // �趨STM32 PAR��
									p_wRead += Lw_Reg - STM32_WRPAR_ADDRESS_BPDJ;
								}
								else if (Lw_Reg >= STM32_RPAR_ADDRESS && Lw_Reg < DSP_WRPAR_ADDRESS)
								{
									p_wRead = w_ParLst; // ��STM32 PAR��
									p_wRead += Lw_Reg - STM32_RPAR_ADDRESS;
								}
								else if (Lw_Reg >= DSP_WRPAR_ADDRESS && Lw_Reg < DSP_RPAR_ADDRESS)
								{
									p_wRead = Pw_dspParLst; // �趨DSP PAR��
									p_wRead += Lw_Reg - DSP_WRPAR_ADDRESS;
								}

								// else if(Lw_Reg>=DSP_RPAR_ADDRESS )  //ZCL 2019.11.22 �ĳ�����
								else if (Lw_Reg >= DSP_RPAR_ADDRESS && Lw_Reg < SCR_WRPAR_ADDRESS)
								{
									p_wRead = w_dspParLst; // ��DSP PAR��
									p_wRead += Lw_Reg - DSP_RPAR_ADDRESS;
								}

								// ZCL 2019.11.22 ���� w_GprsParLst PAR��
								else if (Lw_Reg >= SCR_WRPAR_ADDRESS)
								{
									p_wRead = w_GprsParLst; // w_GprsParLst PAR��
									p_wRead += Lw_Reg - SCR_WRPAR_ADDRESS;
								}

								//
								for (i = 0; i < Rcv1Buffer[2] / 2; i++)
								{
									j = Rcv1Buffer[3 + i * 2];
									*(p_wRead + i) = (j << 8) + Rcv1Buffer[4 + i * 2];
								}

								F_Com1Send = 10;
							}
							else if (Rcv1Buffer[1] == 6) // 06Ԥ�õ��Ĵ���
							{
								F_Com1Send = 11;
							}
							else if (Rcv1Buffer[1] == 16) // 16Ԥ�ö�Ĵ���
							{
								F_Com1Send = 12;
							}
							else if (Rcv1Buffer[1] == 1) // 01��ȡ��Ȧ״̬
							{
								B_Com1Cmd01 = 1;
							}

							else
								i = 1;

							// ZCL 2019.10.20 ���յ���ȷָ����ռ�����+1
							//						w_Com1RXCounter++;
						}
						else
							i = 2;
					}
					else
						i = 3;
				}
				else
					i = 4;
				USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
				Cw_Rcv1 = 0; // ׼���´ν��յ����濪ʼ
			}
		}
	}

	if (i > 0)
	{
		for (j = 0; j < 20; j++)
		{
			Rcv1Buffer[j] = 0;
		}
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	}
}

void Com1_SlaveSend(void) // ����1�ӻ�����
{
	u16 m, n, j;
	u8 k;
	u16 *p_wRead;
	u8 *p_bMove;
	u8 *p_bGen;
	u16 *p_wTarget;
	u16 Lw_Com1Addr; // ����3�Ĵ�����ַ

	if (Pw_EquipmentType == 0) // ˫����
	{
		if (B_Com1Cmd03) // ��ȡ���ּĴ���
		{
			Txd1Buffer[0] = Pw_LoRaEquipmentNo; // �豸�ӵ�ַPw_EquipmentNo
			Txd1Buffer[1] = Rcv1Buffer[1];		// ������
			Txd1Buffer[2] = Rcv1Buffer[5] * 2;	// Rcv1Buffer[5]=���� ��
			j = Rcv1Buffer[2];
			Lw_Com1Addr = (j << 8) + Rcv1Buffer[3];

			// ת����ַ ZCL 2015.7.11
			p_wRead = AddressConvert(Lw_Com1Addr); // 2023.12.16 YLS
			p_bMove = Txd1Buffer;
			for (k = 0; k < Rcv1Buffer[5]; k++) // ����ѯ����
			{
				// m = *(p_wRead + Lw_Com1RegAddr + k); //2023.12.16 YLS
				m = *(p_wRead + k); // 2023.12.16 YLS  ����AddressConvert���Ѿ�������Lw_Com1RegAddr���ƫ����
				*(p_bMove + 3 + k * 2) = m >> 8;
				*(p_bMove + 3 + k * 2 + 1) = m;
			}
			Lw_Txd1ChkSum = CRC16((u8 *)Txd1Buffer, Txd1Buffer[2] + 3);
			Txd1Buffer[Txd1Buffer[2] + 3] = Lw_Txd1ChkSum >> 8; // /256
			Txd1Buffer[Txd1Buffer[2] + 4] = Lw_Txd1ChkSum;		// ��λ�ֽ�
			Cw_Txd1Max = Txd1Buffer[2] + 5;
			//
			B_Com1Cmd03 = 0;
			Cw_Txd1 = 0;
			// RS185_CON=1;
			USART_SendData(USART1, Txd1Buffer[Cw_Txd1++]);
			USART_ITConfig(USART1, USART_IT_TC, ENABLE); // ��ʼ����.
		}
		//
		else if (B_Com1Cmd16 || B_Com1Cmd06) // 16Ԥ�ö�Ĵ���
		{
			if (B_Com1Cmd06) // Ԥ�õ���
			{
				// m = Rcv1Buffer[2];
				// m = (m << 8) + Rcv1Buffer[3];
				j = Rcv1Buffer[2];
				Lw_Com1Addr = (j << 8) + Rcv1Buffer[3];
				// ����޸�ZGP331�����������
				if (Lw_Com1Addr >= STM32_RPAR_ADDRESS) // �����ַ��10000���ϣ����޸�ZGP331����Ĳ���
				{
					// ����Ԥ�ñ����� �趨������
					p_wTarget = AddressConvert(Lw_Com1Addr);
					m = Rcv1Buffer[4];
					// w_ParLst[Lw_Com3RegAddr] = (m << 8) + Txd2Buffer[5];
					*p_wTarget = (m << 8) + Rcv1Buffer[5];
					B_ForceSavPar = 1; // �������
				}
				else
				{
					w_ZhuanFaAdd = Lw_Com1Addr;
					m = Rcv1Buffer[4];
					m = (m << 8) + Rcv1Buffer[5];
					//				*(p_wTarget + Lw_Com2RegAddr) = m;
					w_ZhuanFaData = m;
					F_ModeParLora = 1;
				}
			}

			else if (B_Com1Cmd16) // Ԥ�ö��
			{
				if (Rcv1Buffer[6] == 2)
				{
					m = Rcv1Buffer[2];
					m = (m << 8) + Rcv1Buffer[3];
					w_ZhuanFaAdd = m;

					m = Rcv1Buffer[7];
					m = (m << 8) + Rcv1Buffer[8];
					w_ZhuanFaData = m;

					F_ModeParLora = 1;
				}
				else
					F_ModeParLora = 0;
			}

			// -------------------------
			// ��������
			// 2015.7.11 �����жϣ���Ϊ�趨DSP����ʱ�����ԭ���Ѿ����趨�����У�������
			if (B_Com1Cmd16 || B_Com1Cmd06)
			{
				Txd1Buffer[0] = Pw_LoRaEquipmentNo; // �豸�ӵ�ַ
				Txd1Buffer[1] = Rcv1Buffer[1];		// ������
				Txd1Buffer[2] = Rcv1Buffer[2];		// ��ʼ��ַ��λ�ֽ�
				Txd1Buffer[3] = Rcv1Buffer[3];		// ��ʼ��ַ��λ�ֽ�
				Txd1Buffer[4] = Rcv1Buffer[4];		// �Ĵ���������λ
				Txd1Buffer[5] = Rcv1Buffer[5];		// �Ĵ���������λ
				// if (j == 0)					   // ������ܱ�����Ԥ�ã�����FFFF zcl
				// {
				// 	Txd1Buffer[4] = 0xff; // �Ĵ���������λ��Ԥ������
				// 	Txd1Buffer[5] = 0xff; // �Ĵ���������λ��Ԥ������
				// }
				Lw_Txd1ChkSum = CRC16((u8 *)Txd1Buffer, 6);
				Txd1Buffer[6] = Lw_Txd1ChkSum >> 8; // /256
				Txd1Buffer[7] = Lw_Txd1ChkSum;		// ��λ�ֽ�
				Cw_Txd1Max = 8;
				B_Com1Cmd16 = 0;
				B_Com1Cmd06 = 0;
				Cw_Txd1 = 0;
				// RS185_CON=1;
				USART_SendData(USART1, Txd1Buffer[Cw_Txd1++]);
				USART_ITConfig(USART1, USART_IT_TC, ENABLE); // ��ʼ����.
			}
		} // 06��16Ԥ�üĴ��� ����
	}
	else // ��Ƶ���
	{
		//
		if (B_Com1Cmd03) // ��ȡ���ּĴ���
		{
			Txd1Buffer[0] = 2;				   // �豸�ӵ�ַPw_EquipmentNo
			Txd1Buffer[1] = Rcv1Buffer[1];	   // ������
			Txd1Buffer[2] = Rcv1Buffer[5] * 2; // Rcv1Buffer[5]=���� ��

			// ת����ַ ZCL 2015.7.11
			if (Lw_Com1RegAddr < STM32_RPAR_ADDRESS)
			{
				p_wRead = Pw_ParLst; // �趨STM32 PAR��
				Lw_Com1RegAddr = Lw_Com1RegAddr - STM32_WRPAR_ADDRESS_BPDJ;
			}
			else if (Lw_Com1RegAddr < DSP_WRPAR_ADDRESS)
			{
				p_wRead = w_ParLst; // ��STM32 PAR��
				Lw_Com1RegAddr = Lw_Com1RegAddr - STM32_RPAR_ADDRESS;
			}
			else if (Lw_Com1RegAddr < DSP_RPAR_ADDRESS)
			{
				p_wRead = Pw_dspParLst; // �趨DSP PAR��
				Lw_Com1RegAddr = Lw_Com1RegAddr - DSP_WRPAR_ADDRESS;
			}
			else if (Lw_Com1RegAddr < SCR_WRPAR_ADDRESS)
			{
				p_wRead = w_dspParLst; // ��DSP PAR��
				Lw_Com1RegAddr = Lw_Com1RegAddr - DSP_RPAR_ADDRESS;
			}
			// ZCL 2019.11.22 ���� w_GprsParLst PAR��
			else
			{
				p_wRead = w_GprsParLst; // w_GprsParLst PAR��
				Lw_Com1RegAddr = Lw_Com1RegAddr - SCR_WRPAR_ADDRESS;
			}

			p_bMove = Txd1Buffer;
			for (k = 0; k < Rcv1Buffer[5]; k++) // ����ѯ����
			{
				m = *(p_wRead + Lw_Com1RegAddr + k);
				*(p_bMove + 3 + k * 2) = m >> 8;
				*(p_bMove + 3 + k * 2 + 1) = m;
			}
			Lw_Txd1ChkSum = CRC16((u8 *)Txd1Buffer, Txd1Buffer[2] + 3);
			Txd1Buffer[Txd1Buffer[2] + 3] = Lw_Txd1ChkSum >> 8; // /256
			Txd1Buffer[Txd1Buffer[2] + 4] = Lw_Txd1ChkSum;		// ��λ�ֽ�
			Cw_Txd1Max = Txd1Buffer[2] + 5;
			//
			//
			B_Com1Cmd03 = 0;
			Cw_Txd1 = 0;
			// RS185_CON=1;
			USART_SendData(USART1, Txd1Buffer[Cw_Txd1++]);
			USART_ITConfig(USART1, USART_IT_TC, ENABLE); // ��ʼ����.
		}
		//
		else if (B_Com1Cmd16 || B_Com1Cmd06) // 16Ԥ�ö�Ĵ���
		{

			j = 1;

			// �޸Ĳ�����Ԫ
			if (j)
			{
				// ת����ַ ZCL 2015.7.11
				if (Lw_Com1RegAddr < STM32_RPAR_ADDRESS)
				{
					p_wTarget = Pw_ParLst; // �趨STM32 PAR��
					Lw_Com1RegAddr = Lw_Com1RegAddr - STM32_WRPAR_ADDRESS_BPDJ;
				}
				else if (Lw_Com1RegAddr < DSP_WRPAR_ADDRESS)
				{
					p_wTarget = w_ParLst; // ��STM32 PAR��
					Lw_Com1RegAddr = Lw_Com1RegAddr - STM32_RPAR_ADDRESS;
				}
				else if (Lw_Com1RegAddr < DSP_RPAR_ADDRESS) // ԭ�����޸Ĳ�����־�����ٱ���
				{
					p_wTarget = Pw_dspParLst; // �趨DSP PAR��
					Lw_Com1RegAddr = Lw_Com1RegAddr - DSP_WRPAR_ADDRESS;
				}
				else if (Lw_Com1RegAddr < SCR_WRPAR_ADDRESS)
				{
					// p_wTarget=w_dspParLst;			// ��DSP PAR��
					// Lw_Com1RegAddr=Lw_Com1RegAddr-DSP_RPAR_ADDRESS;
					;
				}

				// ZCL 2019.11.22 ���� w_GprsParLst PAR��
				else
				{
					p_wTarget = w_GprsParLst; // w_GprsParLst PAR��
					Lw_Com1RegAddr = Lw_Com1RegAddr - SCR_WRPAR_ADDRESS;
				}

				if (B_Com1Cmd06) // Ԥ�õ���
				{
					m = Rcv1Buffer[4];
					m = (m << 8) + Rcv1Buffer[5];
					*(p_wTarget + Lw_Com1RegAddr) = m;

					// ��ΪҪת����DSP���ѵ�ַ��ֵ���������� ZCL  2015.7.11
					//				if(F_ModDspPar)
					//				{
					//					w_ModDspParAdd=Lw_Com1RegAddr;
					//					w_ModDspParValue=m;
					//				}
				}

				else if (B_Com1Cmd16) // Ԥ�ö��
				{
					if (Rcv1Buffer[5] < 90)
					{
						p_bGen = Rcv1Buffer;
						for (k = 0; k < Rcv1Buffer[5]; k++) // Rcv1Buffer[5]=����
						{
							m = *(p_bGen + 7 + k * 2);
							n = *(p_bGen + 7 + k * 2 + 1);
							m = (m << 8) + n;
							*(p_wTarget + Lw_Com1RegAddr + k) = m;
						}

						// ��ΪҪת����DSP���ѵ�ַ��ֵ���������� ZCL  2015.7.11
						// ������ʱֻ�ʺ�Ԥ��һ��DSP����  ZCL  2015.7.11
						//					if(F_ModDspPar)
						//					{
						//						w_ModDspParAdd=Lw_Com1RegAddr;
						//						w_ModDspParValue=m;
						//					}
					}
				}
			}

			// -------------------------
			// ��������
			// 2015.7.11 �����жϣ���Ϊ�趨DSP����ʱ�����ԭ���Ѿ����趨�����У�������
			if (B_Com1Cmd16 || B_Com1Cmd06)
			{
				Txd1Buffer[0] = 2;			   // �豸�ӵ�ַ
				Txd1Buffer[1] = Rcv1Buffer[1]; // ������
				Txd1Buffer[2] = Rcv1Buffer[2]; // ��ʼ��ַ��λ�ֽ�
				Txd1Buffer[3] = Rcv1Buffer[3]; // ��ʼ��ַ��λ�ֽ�
				Txd1Buffer[1] = Rcv1Buffer[1]; // �Ĵ���������λ
				Txd1Buffer[5] = Rcv1Buffer[5]; // �Ĵ���������λ
				if (j == 0)					   // ������ܱ�����Ԥ�ã�����FFFF zcl
				{
					Txd1Buffer[1] = 0xff; // �Ĵ���������λ��Ԥ������
					Txd1Buffer[5] = 0xff; // �Ĵ���������λ��Ԥ������
				}
				Lw_Txd1ChkSum = CRC16((u8 *)Txd1Buffer, 6);
				Txd1Buffer[6] = Lw_Txd1ChkSum >> 8; // /256
				Txd1Buffer[7] = Lw_Txd1ChkSum;		// ��λ�ֽ�
				Cw_Txd1Max = 8;
				B_Com1Cmd16 = 0;
				B_Com1Cmd06 = 0;
				Cw_Txd1 = 0;
				// RS185_CON=1;
				// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
				USART_SendData(USART1, Txd1Buffer[Cw_Txd1++]);
				USART_ITConfig(USART1, USART_IT_TC, ENABLE); // ��ʼ����.
			}
		} // 06��16Ԥ�üĴ��� ����
	}
}

// ��Ϊ��ģ��ʱ����
void Com1_MasterSend(void) // ����1�����ͳ���
{
	u8 i = 0, j = 0;

	Lw_Com1EquipmentNo1 = 2;
	Lw_Com1EquipmentNo2 = 3;

	if (Lw_Com1EquipmentNo1 > 0)
		i++; // ͳ�Ʒ�������
	if (Lw_Com1EquipmentNo2 > 0)
		i++;

	// if ( Pw_VvvfComAddr>0 )	i++;
	// if ( Pw_LLJComAddr>0 )	i++;
	//
	if (i > 0) // ��������ģ����߱�Ƶ���������Ʋ��ܽ��뷢�ͳ���
	{
		if (F_Com1SendNext) // 1. ���Ʒ��Ͳ���
		{
			S_Com1MSend++;
			if (S_Com1MSend > 2)
				S_Com1MSend = 1; // COM_UNIT_NUM
			//
			// 2.����˳����
			if (Lw_Com1EquipmentNo1 > 0 && S_Com1MSend == 1)
			{
				F_Com1SendNext = 0; // �����־
				B_Com1Send1 = 1;
				Txd1Buffer[0] = Lw_Com1EquipmentNo1; // �豸�ӵ�ַ
				j = 1;
			}
			else if (Lw_Com1EquipmentNo2 > 0 && S_Com1MSend == 2)
			{
				F_Com1SendNext = 0; // �����־
				B_Com1Send2 = 1;
				Txd1Buffer[0] = Lw_Com1EquipmentNo2; // �豸�ӵ�ַ
				j = 2;
			}

			// #define	w_DispWenDuDecValue		w_ParLst[148]		// ��ʾ�¶�DECֵ��������ı���ʾ���ã�ZCL 2017.11.21��
			// ZCL ����  03ָ��
			// ZCL 2022.8.24 =111�����û�� !!!
			if (j == 1) // ������ģ��
			{
				// Txd1Buffer[0]=Pw_EquipmentNo+S_Com1MSend;
				Txd1Buffer[1] = 0x03; // ������
				Txd1Buffer[2] = 0x01; // ��ʼ��ַ����λ��
				Txd1Buffer[3] = 0x53; // ��ʼ��ַ����λ��  �ӵ�ַ339��ʼ��
				Txd1Buffer[4] = 0;	  // ��ȡ��������λ��
				Txd1Buffer[5] = 0x20; // ��ȡ��������λ����32����
				w_Txd1ChkSum = CRC16(Txd1Buffer, 6);
				Txd1Buffer[6] = w_Txd1ChkSum >> 8; // /256
				Txd1Buffer[7] = w_Txd1ChkSum;	   // ��λ�ֽ�
												   //				Txd1Max = 8;
												   //				Txd1Counter = 0;
				Cw_Txd1Max = 8;
				Cw_Txd1 = 0;
				B_Com1Con = 1; // B_Com1Con=1,���Է���

				// 2010.7.4 �ܳ��� �ĳ�	USART_IT_TC
				USART_SendData(USART1, Txd1Buffer[Cw_Txd1++]);
				USART_ITConfig(USART1, USART_IT_TC, ENABLE); // ��ʼ����.
			}
		}

		// 3.��ʱ����
		if (!B_Com1Con) // =0 ���ͽ���
		{
			if (B_Com1Send1 == 1)
				B_Com1Sd1End = 1;
			if (B_Com1Send2 == 1)
				B_Com1Sd2End = 1;
			/* 			if ( B_Com1Send3 == 1 )
							B_Com1Sd3End = 1;
						if ( B_Com1Send1 == 1 )
							B_Com1Sd4End = 1; */
		}

		//
		// ���жϽ�����������ʱ������һ�� 6MS
		if (F_Com1RCVCommandOk) // ����������ʱ�󣬷�����һ��
		{
			if (T_Com1NormalNext != SClk1Ms)
			{
				T_Com1NormalNext = SClk1Ms; //
				C_Com1NormalNext++;
				if (C_Com1NormalNext > 8) //
				{
					F_Com1RCVCommandOk = 0;
					T_Com1NormalNext = 100;
					C_Com1NormalNext = 0;
					T_Com1OvertimeNext = 100;
					C_Com1OvertimeNext = 0;
					F_Com1SendNext = 1; //
				}
			}
		}

		// ���жϷ����곬ʱ����ʱ������һ�� 250MS   (2400bps ����32��) // 200
		if (!B_Com1Con && !F_Com1SendNext) // ��������м�ʱ��ʱ�䵽������һ��
		{
			if (T_Com1OvertimeNext != SClk1Ms)
			{
				T_Com1OvertimeNext = SClk1Ms; //
				C_Com1OvertimeNext++;
				if (C_Com1OvertimeNext > 250) // 250  ZCL123
				{
					T_Com1OvertimeNext = 100;
					C_Com1OvertimeNext = 0;
					F_Com1SendNext = 1; //
				}
			}
		}

		//----------------------------------------------------------------
		// �жϷ����곬ʱ����ʱͨѶ���� 4000MS   ����ģ��1ͨѶʧ�� // 6000
		if (B_Com1Sd1End == 1 && !B_Com1ErrWithModule1) // ��������м�ʱ��ʱ�䵽����
		{
			if (T_Com1Err1 != SClk1Ms)
			{
				T_Com1Err1 = SClk1Ms; //
				C_Com1Err1++;
				if (C_Com1Err1 > 4000) //
				{
					T_Com1Err1 = 100;
					C_Com1Err1 = 0;
					B_Com1ErrWithModule1 = 1; //
				}
			}
		}

		//----------------------------------------------------------------
		// �жϷ����곬ʱ����ʱͨѶ���� 4000MS	����ģ��2ͨѶʧ��
		if (B_Com1Sd2End == 1 && !B_Com1ErrWithModule2) // ��������м�ʱ��ʱ�䵽����
		{
			if (T_Com1Err2 != SClk1Ms)
			{
				T_Com1Err2 = SClk1Ms; //
				C_Com1Err2++;
				if (C_Com1Err2 > 4000) //
				{
					T_Com1Err2 = 100;
					C_Com1Err2 = 0;
					B_Com1ErrWithModule2 = 1; //
				}
			}
		}

		// �����������������͡��ܳ��� 20060924
		//...
	}

	// ZCL 2023.2.28
	if (Lw_Com1EquipmentNo1 == 0)
	{
		B_Com1Sd1End = 0;
		B_Com1ErrWithModule1 = 0; //
		C_Com1Err1 = 0;
	}

	if (Lw_Com1EquipmentNo2 == 0)
	{
		B_Com1Sd2End = 0;
		B_Com1ErrWithModule2 = 0; //
		C_Com1Err2 = 0;
	}
}

u16 CRC16(u8 *pCrcData, u8 CrcDataLen)
{
	u8 CRC16Hi = 0xff; /* ��CRC�ֽڳ�ʼ�� */
	u8 CRC16Lo = 0xff; /* ��CRC�ֽڳ�ʼ��*/
	u8 Index = 0;
	u16 w_CRC16 = 0;
	while (CrcDataLen--)
	{
		Index = CRC16Hi ^ *pCrcData++;
		CRC16Hi = CRC16Lo ^ CRC_H[Index];
		CRC16Lo = CRC_L[Index];
	}
	w_CRC16 = (CRC16Hi << 8) | CRC16Lo;
	return (w_CRC16);
}

/*************************************************************************************
��  ��: void Read_Stm32Data(u16 Reg, u16 Num)
��  ��:	ͨ��MODBUS��ȡSTM32��ָ����������.
��  ��:
		Reg   -- ��Ƕ�ȡSTM32�Ŀ�ʼ�Ĵ���
				Num		-- ��ȡ���ݸ���.
��  ��:	��

��  ��: ZCL
ʱ  ��: 2015-07-03
*************************************************************************************/
void Read_Stm32Data(u16 Reg, u16 Num)
{
	// ZCL 2017.6.19 �����ʱ����Ҫ����ֹǰ��Ľ���û����ɣ��Ϳ�ʼ��һ������
	if (S_Com1Send == 0 && T_Com1SendDelay != SClk10Ms) // ZCL 2016.6.16 ������ʱ
	{
		T_Com1SendDelay = SClk10Ms;
		C_Com1SendDelay++;
		if (C_Com1SendDelay > 10) // 100MS������ٴη���
		{
			S_Com1Send = READSTM32DATA;
			C_Com1SendDelay = 0;
		}
	}
	/* 	//ZCL 2017.6.19 ����û�м���ʱ���������ݽ��մ�λ�ã�������˸��
		if(S_Com1Send==0)
			S_Com1Send=READSTM32DATA; */

	if (S_Com1Send == READSTM32DATA)
	{
		S_Com1Send = READSTM32DATA + 1; // ZCL 2015.8.18 !!! ��ʱɾ��

		Txd1Buffer[0] = 0x02;		// �豸�ӵ�ַ STM32
		Txd1Buffer[1] = 0x03;		// ������
		Txd1Buffer[2] = (Reg >> 8); // ��ʼ��ַ����λ��
		Txd1Buffer[3] = Reg;		// ��ʼ��ַ����λ��  ��ַ��9-23 �趨������ѯ
		Txd1Buffer[4] = (Num >> 8); // ��ȡ��������λ��
		Txd1Buffer[5] = Num;		// ��ȡ��������λ��
		Lw_Txd1ChkSum = CRC16(Txd1Buffer, 6);
		Txd1Buffer[6] = Lw_Txd1ChkSum >> 8; // /256
		Txd1Buffer[7] = Lw_Txd1ChkSum;		// ��λ�ֽ�
		Cw_Txd1Max = 8;
		Cw_Txd1 = 0;
		F_Com1Send = 1;

		USART_ITConfig(USART1, USART_IT_TC, ENABLE); // ��ʼ����.
		USART_SendData(USART1, Txd1Buffer[Cw_Txd1++]);
	}
	else if (S_Com1Send == READSTM32DATA + 1)
	{
		if (F_Com1Send == 10) // ��ĵط����,�����ȡ������
		{
			F_Com1Send = 0;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			// ZCL 2015.9.1  2018.5.19
			if (B_PressedStopRunKey == 0)	   // ZCL 2016.12.6 ����STOP ���� RUN��
				S_RdWrNo++;					   // ����������һ��ָ��
			else if (B_PressedStopRunKey == 1) // ZCL 2016.12.6 ����STOP ���� RUN��
				B_ModPar = 100;				   // ZCL 2016.12.6 =100��ʾ���ٽ��в�����д����
		}
		else if (T_Com1Send != SClk10Ms)
		{
			T_Com1Send = SClk10Ms; //
			C_Com1Send++;
			if (C_Com1Send > 80) // 	300MS���ͳ�ʱ
			{
				C_Com1Send = 0;
				if (B_PressedStopRunKey == 0) // ZCL 2016.12.6 δ����STOP ���� RUN������������
				{
					S_Com1Send = READSTM32DATA; //=1���Լ�������
					S_Com1SendNoNum++;			// ���ͼ�������1
				}
				else // ZCL 2016.12.6 ����STOP ���� RUN��
				{
					S_Com1Send = 0;
					S_Com1SendNoNum = 0;
					B_ModPar = 100; // ZCL 2016.12.6 =100��ʾ���ٽ��в�����д����
				}
			}
		}
	}

	if (S_Com1SendNoNum >= 3) // ���ʹ���>3�Σ��ù��ϱ�־��
	{
		S_Com1Send = 0; //=0,�����ֹ����
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
		// ZCL 2015.9.1
		// S_RdWrNo++;				//����������һ��ָ�� ZCL 2017.6.19 ע�͵�
	}
}

/*************************************************************************************
��  ��: void Write_Stm32Data(u16 Reg, u16 Data)
��  ��:	ͨ��MODBUS��STM32д��ָ������.
��  ��:
		Reg   -- Ҫд��ļĴ�����ַ.
				Data  -- Ҫд������.
��  ��:	��

��  ��: ZCL
ʱ  ��: 2015-07-03
*************************************************************************************/
void Write_Stm32Data(u16 Reg, u16 Data)
{
	if (S_Com1Send == 0)
		S_Com1Send = WRITESTM32DATA;

	if (S_Com1Send == WRITESTM32DATA)
	{
		S_Com1Send = WRITESTM32DATA + 1;

		Txd1Buffer[0] = 0x02;		 // �豸�ӵ�ַ
		Txd1Buffer[1] = 0x06;		 // ������
		Txd1Buffer[2] = (Reg >> 8);	 // ��ʼ��ַ����λ��
		Txd1Buffer[3] = Reg;		 // ��ʼ��ַ����λ��  ��ַ��9-23 �趨������ѯ
		Txd1Buffer[4] = (Data >> 8); // Ԥ�����ݣ���λ��
		Txd1Buffer[5] = Data;		 // Ԥ�����ݣ���λ��
		Lw_Txd1ChkSum = CRC16(Txd1Buffer, 6);
		Txd1Buffer[6] = Lw_Txd1ChkSum >> 8; // /256
		Txd1Buffer[7] = Lw_Txd1ChkSum;		// ��λ�ֽ�
		Cw_Txd1Max = 8;
		Cw_Txd1 = 0;
		F_Com1Send = 1;
		// RS485_CON=1;
		// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
		// USART_ITConfig(USART1, USART_IT_TXE, ENABLE);				// ��ʼ����.
		USART_ITConfig(USART1, USART_IT_TC, ENABLE); // ��ʼ����.
		USART_SendData(USART1, Txd1Buffer[Cw_Txd1++]);
	}
	else if (S_Com1Send == WRITESTM32DATA + 1)
	{
		if (F_Com1Send == 11) // ��ĵط����,�����ȡ������
		{
			F_Com1Send = 0;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			// ZCL 2015.9.1
			// S_RdWrNo++;				//����������һ��ָ��

			// ZCL 2018.5.14 ԭ�����޸�״̬��ָ����һ��
			if (B_ModPar == 1 || B_ModPar == 2)
			{
				S_ModParStatus = 1; // �޸Ĳ���ȷ�Ϻ�ָ����һ������
				ModParNo++;			// �޸Ĳ������++
				ModParBitPos = 0;	// �޸Ĳ���λλ��=0
			}

			// ZCL 2018.5.19
			if (B_ModPar == 100)
				B_PressedStopRunKey = 0;
			// ZCL 2015.9.1
			// S_RdWrNo++;				//����������һ��ָ��
			B_ModPar = 0;
		}
		else if (T_Com1Send != SClk1Ms)
		{
			T_Com1Send = SClk1Ms; //
			C_Com1Send++;
			if (C_Com1Send > 800) // 	300MS���ͳ�ʱ
			{
				C_Com1Send = 0;
				S_Com1Send = WRITESTM32DATA; //=1���Լ�������
				S_Com1SendNoNum++;			 // ���ͼ�������1
			}
		}
	}

	if (S_Com1SendNoNum >= 3) // ���ʹ���>3�Σ��ù��ϱ�־��
	{
		S_Com1Send = 0; //=0,�����ֹ����
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;

		if (B_ModPar == 100)
			B_PressedStopRunKey = 0;
		// ZCL 2015.9.1
		// S_RdWrNo++;				//����������һ��ָ��
		B_ModPar = 0;

		// ZCL 2018.5.14
		// ZCL 2019.3.22 ע�͵��������//�������⣺��һ���Զ��˳��޸�״̬������Ϊû������ARM�壬����Write_Stm32Data()�г���3����S_ModParStatus  ����취��ע�͵�S_ModParStatus=0;  ��ע�͵���ModParNo=0; ModParBitPos=0;

		// ZCL 2019.4.9
		//  S_ModParStatus=0;		//�޸Ĳ���ȷ�Ϻ�ָ����һ������
		//  ModParNo=0;					//�޸Ĳ������++
		//  ModParBitPos=0;			//�޸Ĳ���λλ��=0
	}
}

/*************************************************************************************
��  ��: void Write_Stm32MulDatas(u16 Reg, u16 Num, u16 *Data)
��  ��:	ͨ��MODBUS��STM32д��ָ������.
��  ��:
		Reg   -- Ҫд��ļĴ�����ַ.
				Data  -- Ҫд������.
��  ��:	��

��  ��: ZCL
ʱ  ��: 2015-07-03
*************************************************************************************/
void Write_Stm32MulDatas(u16 Reg, u16 Num, u16 *Data)
{
	u16 i;

	if (S_Com1Send == 0)
		S_Com1Send = WRITESTM32MULDATA;

	if (S_Com1Send == WRITESTM32MULDATA)
	{
		S_Com1Send = WRITESTM32MULDATA + 1;

		Txd1Buffer[0] = 0x02;		// �豸�ӵ�ַ
		Txd1Buffer[1] = 0x10;		// ������
		Txd1Buffer[2] = (Reg >> 8); // ��ʼ��ַ����λ��
		Txd1Buffer[3] = Reg;		// ��ʼ��ַ����λ��  ��ַ��9-23 �趨������ѯ
		Txd1Buffer[4] = (Num >> 8); // Ԥ�ø�������λ��
		Txd1Buffer[5] = Num;		// Ԥ�ø�������λ��
		Txd1Buffer[6] = Num * 2;	// Ԥ���ֽ���
		for (i = 0; i < Num; i++)
		{
			Txd1Buffer[7 + i * 2] = (Data[i] >> 8);
			Txd1Buffer[7 + i * 2 + 1] = Data[i];
		}

		Lw_Txd1ChkSum = CRC16(Txd1Buffer, 7 + i * 2);
		Txd1Buffer[7 + i * 2] = Lw_Txd1ChkSum >> 8; // /256
		Txd1Buffer[7 + i * 2 + 1] = Lw_Txd1ChkSum;	// ��λ�ֽ�
		Cw_Txd1Max = 7 + i * 2 + 2;
		Cw_Txd1 = 0;
		F_Com1Send = 1;
		// RS485_CON=1;
		// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
		// USART_ITConfig(USART1, USART_IT_TXE, ENABLE);				// ��ʼ����.
		USART_ITConfig(USART1, USART_IT_TC, ENABLE); // ��ʼ����.
		USART_SendData(USART1, Txd1Buffer[Cw_Txd1++]);
	}
	else if (S_Com1Send == WRITESTM32MULDATA + 1)
	{
		if (F_Com1Send == 12) // ��ĵط����,�����ȡ������
		{
			F_Com1Send = 0;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			// ZCL 2015.9.1
			// S_RdWrNo++;				//����������һ��ָ��
			B_ModPar = 0;
		}
		else if (T_Com1Send != SClk1Ms)
		{
			T_Com1Send = SClk1Ms; //
			C_Com1Send++;
			if (C_Com1Send > 800) // 	1000MS���ͳ�ʱ
			{
				C_Com1Send = 0;
				S_Com1Send = WRITESTM32MULDATA; //=1���Լ�������
				S_Com1SendNoNum++;				// ���ͼ�������1
			}
		}
	}

	if (S_Com1SendNoNum >= 3) // ���ʹ���>3�Σ��ù��ϱ�־��
	{
		S_Com1Send = 0; //=0,�����ֹ����
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
		// ZCL 2015.9.1
		// S_RdWrNo++;				//����������һ��ָ��
		B_ModPar = 0;
	}
}

// ��Ե�ַ
u16 Position(u16 *p1, u16 *p2)
{
	return p2 - p1;
}

// ����ƫ������ ���Ե�ַ
u16 Address(u16 *p, u16 Area)
{
	B_ModYW310SZM220 = 0;	   // ZCL 2019.10.19
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Area == 0)
			Lw_Reg = Position(Pw_ParLst, p) + DNB_ADDRESS; //  2023.12.16 YLS STM32_WRPAR_ADDRESS���������Ϊ50000��
		else if (Area == 1)
			Lw_Reg = Position(w_ParLst, p) + STM32_RPAR_ADDRESS;
		else if (Area == 2)
			Lw_Reg = Position(Pw_dspParLst, p) + DSP_WRPAR_ADDRESS;
		else if (Area == 3)
			Lw_Reg = Position(w_dspParLst, p) + DSP_RPAR_ADDRESS;
		// ZCL 2018.8.3 SCR�������  û���ã�ֻ�ǲ���������
		else if (Area == 4)
			Lw_Reg = Position(w_GprsParLst, p) + SCR_WRPAR_ADDRESS; // ZCL 2018.9.13  +SCR_WRPAR_ADDRESS
		// ZCL 2019.10.19 ��дYW310��SZM220�Ĳ��������е�ַ�任�� ע�⣺Com1_RcvProcess����ʱ�����е�ַ�ķ��任
		else if (Area == 8)
		{
			Lw_Reg = Position(w_ParLst, p);
			B_ModYW310SZM220 = 1; // ZCL 2019.10.19

			// YW310
			if (Lw_Reg == 260)
				Lw_Reg = 9;
			else if (Lw_Reg == 261)
				Lw_Reg = 23;
			else if (Lw_Reg == 262)
				Lw_Reg = 11;
			else if (Lw_Reg == 263)
				Lw_Reg = 156;
			else if (Lw_Reg == 264)
				Lw_Reg = 157;
			else if (Lw_Reg == 265)
				Lw_Reg = 142;

			else if (Lw_Reg == 256)
				Lw_Reg = 1; // ZCL 2019.10.21
			else if (Lw_Reg == 257)
				Lw_Reg = 2;
			else if (Lw_Reg == 258)
				Lw_Reg = 3;
			else if (Lw_Reg == 259)
				Lw_Reg = 4;
			else if (Lw_Reg == 266)
				Lw_Reg = 54;
			else if (Lw_Reg == 267)
				Lw_Reg = 60; // ZCL 2019.10.21

			// SZM220
			else if (Lw_Reg == 268)
				Lw_Reg = 212;
			else if (Lw_Reg == 269)
				Lw_Reg = 213;
			else if (Lw_Reg == 270)
				Lw_Reg = 218;
			else if (Lw_Reg == 271)
				Lw_Reg = 226;
			else if (Lw_Reg == 272)
				Lw_Reg = 227;
			else if (Lw_Reg == 273)
				Lw_Reg = 228;
			else if (Lw_Reg == 274)
				Lw_Reg = 229;
			else if (Lw_Reg == 275)
				Lw_Reg = 230;
			else if (Lw_Reg == 276)
				Lw_Reg = 231;
			else if (Lw_Reg == 277)
				Lw_Reg = 232;
			else if (Lw_Reg == 278)
				Lw_Reg = 233;
			else if (Lw_Reg == 279)
				Lw_Reg = 234;
			else if (Lw_Reg == 280)
				Lw_Reg = 235;
			else if (Lw_Reg == 281)
				Lw_Reg = 236;
			else if (Lw_Reg == 282)
				Lw_Reg = 237;
			else if (Lw_Reg == 283)
				Lw_Reg = 238;
			else if (Lw_Reg == 286)
				Lw_Reg = 501;
			else if (Lw_Reg == 287)
				Lw_Reg = 641;
			else if (Lw_Reg == 288)
				Lw_Reg = 640;
		}
	}
	else // ��Ƶ���
	{
		if (Area == 0)
			Lw_Reg = Position(Pw_ParLst, p) + STM32_WRPAR_ADDRESS_BPDJ;
		else if (Area == 1)
			Lw_Reg = Position(w_ParLst, p) + STM32_RPAR_ADDRESS;
		else if (Area == 2)
			Lw_Reg = Position(Pw_dspParLst, p) + DSP_WRPAR_ADDRESS;
		else if (Area == 3)
			Lw_Reg = Position(w_dspParLst, p) + DSP_RPAR_ADDRESS;
		// ZCL 2018.8.3 SCR�������  û���ã�ֻ�ǲ���������
		else if (Area == 4)
			Lw_Reg = Position(w_GprsParLst, p) + SCR_WRPAR_ADDRESS; // ZCL 2018.9.13  +SCR_WRPAR_ADDRESS
	}

	return Lw_Reg;
}

// ��ȥƫ������ ���Ե�ַ
u16 *AddressChange(void)
{
	u16 *p;
	if (Lw_Reg < STM32_RPAR_ADDRESS)
	{
		p = Pw_ParLst;			   // �趨STM32 PAR��
		if (Pw_EquipmentType == 0) // ˫����
		{
			p += Lw_Reg - DNB_ADDRESS;
		}
		else // ��Ƶ���
		{
			p += Lw_Reg - STM32_WRPAR_ADDRESS_BPDJ;
		}
	}
	else if (Lw_Reg >= STM32_RPAR_ADDRESS && Lw_Reg < DSP_WRPAR_ADDRESS)
	{
		p = w_ParLst; // ��STM32 PAR��
		p += Lw_Reg - STM32_RPAR_ADDRESS;
	}
	else if (Lw_Reg >= DSP_WRPAR_ADDRESS && Lw_Reg < DSP_RPAR_ADDRESS)
	{
		p = Pw_dspParLst; // �趨DSP PAR��
		p += Lw_Reg - DSP_WRPAR_ADDRESS;
	}

	// else if(Lw_Reg>=DSP_RPAR_ADDRESS )  //ZCL 2019.11.22 �ĳ�����
	else if (Lw_Reg >= DSP_RPAR_ADDRESS && Lw_Reg < SCR_WRPAR_ADDRESS)
	{
		p = w_dspParLst; // ��DSP PAR��
		p += Lw_Reg - DSP_RPAR_ADDRESS;
	}

	// ZCL 2019.11.22 ���� w_GprsParLst PAR��
	else if (Lw_Reg >= SCR_WRPAR_ADDRESS)
	{
		p = w_GprsParLst; // w_GprsParLst PAR��
		p += Lw_Reg - SCR_WRPAR_ADDRESS;
	}
	return p;
}

// �������ĵ�ַת������
u16 *AddressChange_2(u16 L_Reg)
{
	u16 *p;
	if (L_Reg < STM32_RPAR_ADDRESS)
	{
		p = Pw_ParLst; // �趨STM32 PAR��
		p += L_Reg - DNB_ADDRESS;
	}
	else if (L_Reg >= STM32_RPAR_ADDRESS && L_Reg < DSP_WRPAR_ADDRESS)
	{
		p = w_ParLst; // ��STM32 PAR��
		p += L_Reg - STM32_RPAR_ADDRESS;
	}
	else if (L_Reg >= DSP_WRPAR_ADDRESS && L_Reg < DSP_RPAR_ADDRESS)
	{
		p = Pw_dspParLst; // �趨DSP PAR��
		p += L_Reg - DSP_WRPAR_ADDRESS;
	}

	// else if(L_Reg>=DSP_RPAR_ADDRESS )  //ZCL 2019.11.22 �ĳ�����
	else if (L_Reg >= DSP_RPAR_ADDRESS && L_Reg < SCR_WRPAR_ADDRESS)
	{
		p = w_dspParLst; // ��DSP PAR��
		p += L_Reg - DSP_RPAR_ADDRESS;
	}

	// ZCL 2019.11.22 ���� w_GprsParLst PAR��
	else if (L_Reg >= SCR_WRPAR_ADDRESS)
	{
		p = w_GprsParLst; // w_GprsParLst PAR��
		p += L_Reg - SCR_WRPAR_ADDRESS;
	}
	return p;
}

// ��ȥƫ������ ���Ե�ַ YLS 2023.12.15
// ֻ��˫���ó����õ��������
u16 *AddressConvert(u16 L_Reg)
{
	u16 *p;
	if (L_Reg < STM32_RPAR_ADDRESS)
	{
		p = w_DNBParLst; // w_DNBParLst PAR��
		p += L_Reg - DNB_ADDRESS;
	}
	else if (L_Reg >= STM32_RPAR_ADDRESS && L_Reg < DSP_WRPAR_ADDRESS)
	{
		p = w_ParLst; // ��STM32 PAR��
		p += L_Reg - STM32_RPAR_ADDRESS;
	}
	else if (L_Reg >= DSP_WRPAR_ADDRESS && L_Reg < DSP_RPAR_ADDRESS)
	{
		p = Pw_dspParLst; // �趨DSP PAR��
		p += L_Reg - DSP_WRPAR_ADDRESS;
	}

	// else if(L_Reg>=DSP_RPAR_ADDRESS )  //ZCL 2019.11.22 �ĳ�����
	else if (L_Reg >= DSP_RPAR_ADDRESS && L_Reg < SCR_WRPAR_ADDRESS)
	{
		p = w_dspParLst; // ��DSP PAR��
		p += L_Reg - DSP_RPAR_ADDRESS;
	}

	// ZCL 2019.11.22 ���� w_GprsParLst PAR��
	else if (L_Reg >= SCR_WRPAR_ADDRESS && L_Reg < STM32_WRPAR_ADDRESS_SQB)
	{
		p = w_GprsParLst; // w_GprsParLst PAR��
		p += L_Reg - SCR_WRPAR_ADDRESS;
	}

	// YLS 2023.12.15
	else if (L_Reg >= STM32_WRPAR_ADDRESS_SQB)
	{
		p = Pw_ParLst; // Pw_ParLst PAR��
		p += L_Reg - STM32_WRPAR_ADDRESS_SQB;
	}
	return p;
}

void USART_putchar(USART_TypeDef *USART_x, unsigned char ch)
{
	/* Write a character to the USART */
	USART_SendData(USART_x, (unsigned char)ch);
	while (USART_GetFlagStatus(USART_x, USART_FLAG_TXE) == RESET)
		;
	while (USART_GetFlagStatus(USART_x, USART_FLAG_TC) == RESET)
		;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
