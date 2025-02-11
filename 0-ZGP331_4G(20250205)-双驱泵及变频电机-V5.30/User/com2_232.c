/**
  ******************************************************************************
  * @file    com2_232.c
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.10
  * @date    2015-04-13
  * @brief   Com1 USE USART2
	******************************************************************************
	* @note	2015.04.13
			COM2: ͸��������ԣ�XmodemЭ�鴫���ļ�
	*/

/* Includes ------------------------------------------------------------------*/
#include "com1_232.h"
#include "com2_232.h"
#include "GlobalV_Extern.h" // ȫ�ֱ�������
#include "GlobalConst.h"
#include <stdio.h> //���ϴ˾������printf

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 Txd2Buffer[TXD2_MAX];	  // ���ͻ�����
u8 Rcv2Buffer[RCV2_MAX];	  // ���ջ�����
u8 Txd2Buffer_TEMP[TXD2_MAX]; // ���ͻ�����
u16 Cw_Rcv2;				  // ���ռ�����
u16 Cw_Txd2;				  // ���ͼ�����
u16 Cw_BakRcv2;				  // ���ռ���������
u16 Cw_Txd2Max;				  // �ж��ٸ��ַ���Ҫ����
u16 Lw_Txd2ChkSum;			  // ����У��ͣ�lo,hi ��λ
u16 Lw_Com2RegAddr;			  // ����1�Ĵ�����ַ
//
u8 B_Com2Cmd03;
u8 B_Com2Cmd16;
u8 B_Com2Cmd01;
u8 B_Com2Cmd06;
u8 B_Com2Cmd73;
u8 T_NoRcv2; // û�н��ռ�����
u16 C_NoRcv2;

u8 F_ModDspPar; // �޸�DSP����

// B_PcSetGprs��=1ʱ���ٽ��յ���������=2������Sms_SetupInq()��������
u8 B_PcSetGprs; // PC����GPRS��־ 2013.6.5

/* Private variables extern --------------------------------------------------*/
extern u16 C_ForceSavPar;	// ǿ�Ʊ������������
extern uint8_t LoRaTxBuf[]; // ZCL 2018.11.12
extern u8 B_LoRaSendMaster;

extern u8 Txd3Buffer[TXD3_MAX]; // ���ͻ�����
extern u8 Rcv3Buffer[RCV3_MAX]; // ���ջ�����
extern u16 Cw_Rcv3;				// ���ռ�����
extern u16 Cw_Txd3;				// ���ͼ�����
extern u16 Cw_BakRcv3;			// ���ռ���������
extern u16 Cw_Txd3Max;			// �ж��ٸ��ַ���Ҫ����
extern u16 Lw_Txd3ChkSum;		// ����У��ͣ�lo,hi ��λ
extern u16 Lw_Com3RegAddr;		// ����1�Ĵ�����ַ

extern u8 B_GprsDataReturn; // GPRS ���ݷ��ء������ӣ��յ�����2���ݣ���ͨ��DTU���ͳ�ȥ
extern u16 C_GprsDataSend;
extern u8 B_SmsRunStop; // ���ſ���ֹͣ���豸ͣ����
extern u8 S_M35;
extern u16 C_DelayRunGprsModule; // ��ʱ����GPRSģ���־
extern u8 CGD0_LinkConnectOK[];	 // ����0���ӱ�־
extern u8 B_ATComStatus;		 // AT����״̬ 2013.5.20
extern u8 B_ForceSavPar;		 // ǿ�Ʊ��������־
extern uchar B_RestartNo;		 // ZCL 2019.4.19  ������ţ������ӡ�۲�����

// ZCL 2019.11.29
extern u8 Txd4Buffer[];
extern u8 Rcv4Buffer[];
extern u16 Cw_Txd4;
extern u16 Cw_Rcv4;
extern u16 C_NoRcv4;
extern u16 Cw_Txd4Max;

/* Private function prototypes -----------------------------------------------*/
void GPIO_Com2_Configuration(void); // GPIO����
void Com2_config(void);
u16 CRC16(u8 *pCrcData, u8 CrcDataLen);
void Delay_MS(vu16 nCount);
extern void Com2_printf(char *fmt, ...);
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
 * Function Name  : GPIO_Com2_Configuration
 * Description    : Configures the different GPIO ports.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void GPIO_Com2_Configuration(void) // GPIO����
{
	GPIO_InitTypeDef GPIO_InitStructure; // ����GPIO��ʼ���ṹ��

	/* Configure Com2(ʹ��Ӳ��:USART2) Rx (PA.3) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure Com2(ʹ��Ӳ��:USART2) Tx (PA.2) as alternate function push-pull */
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
		- BaudRate = 9600 baud
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
	// 2014.1.5 �������ǳ���Ҫ����λ�Ĵ�����������MDK�мĴ���û�и�λ�������
	USART_DeInit(USART2);					// ��λ Usart3
	USART_StructInit(&USART_InitStructure); // ��USART_InitStruct�е�ÿһ��������ȱʡֵ����

	// 1. 00 00 25 80 ��������9600��25 80����ʮ����������
	USART_InitStructure.USART_BaudRate = Pw_GprsBaudRate2; // 9600;
	// 2. 03 ����λΪ8λ��00-5�� 01-6�� 02-7�� 03-8����������ֻ������03��
	// STM32: ֻ��USART_WordLength_8b ��USART_WordLength_9b ��û��5λ��6λ��7λ�ĳ���
	// STM32: �˴�������żУ��ʱ��Ҫ9������λ������żУ��ʱ8������λ��ͨ����żУ��λ����������λ����
	if (Pw_GprsParityBitLen2 == 0x00)
	{
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		Pw_GprsDataBitLen2 = 0x03;
	}
	else if (Pw_GprsParityBitLen2 == 0x08 || Pw_GprsParityBitLen2 == 0x18)
	{
		Pw_GprsDataBitLen2 = 0x03;
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	}
	// 3. 00 ֹͣλΪ1λ��00-1��04-2�����������������00��04��
	// STM32: USART_StopBits_1 ��USART_StopBits_0.5 ��USART_StopBits_2 ��USART_StopBits_1.5
	if (Pw_GprsStopBitLen2 == 0x00)
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
	else if (Pw_GprsStopBitLen2 == 0x04)
		USART_InitStructure.USART_StopBits = USART_StopBits_2;
	else
	{
		Pw_GprsStopBitLen2 = 0x00;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
	}
	// 4. 00 У��λ-�ޣ�00-�ޣ�08-�棬18-ż�����������������00��08��18��
	// STM32: USART_Parity_No  	USART_Parity_Odd ��ģʽ		USART_Parity_Even żģʽ
	if (Pw_GprsParityBitLen2 == 0x00)
		USART_InitStructure.USART_Parity = USART_Parity_No;
	else if (Pw_GprsParityBitLen2 == 0x08)
		USART_InitStructure.USART_Parity = USART_Parity_Odd;
	else if (Pw_GprsParityBitLen2 == 0x18)
		USART_InitStructure.USART_Parity = USART_Parity_Even;
	else
	{
		Pw_GprsParityBitLen2 = 0x00;
		USART_InitStructure.USART_Parity = USART_Parity_No;
	}
	// 5. 03 ����-�����أ�01-Xon/Xoff��02-Ӳ���أ�03-�����أ�04-��˫��485��05-ȫ˫��422��
	//				�������������03��02��
	// STM32: 		USART_HardwareFlowControl_None  		Ӳ��������ʧ��
	// USART_HardwareFlowControl_RTS  			�������� RTSʹ��
	// USART_HardwareFlowControl_CTS  			������� CTSʹ��
	// USART_HardwareFlowControl_RTS_CTS  	RTS�� CTSʹ��
	if (Pw_GprsFlowControl2 == 0x03)
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	else if (Pw_GprsFlowControl2 == 0x02)
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
	else
	{
		Pw_GprsFlowControl2 = 0x03;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	}
	// 6. ����ʹ�ܺͽ���ʹ�ܣ����ÿ���
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure Com1-USART2 */
	USART_Init(USART2, &USART_InitStructure);
	/* Enable USART2 Receive interrupts */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

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
	Cw_Txd2 = 0;
	Cw_Rcv2 = 0;
}

// ���մ������ У�����
void Com2_RcvProcess(void)
{
	u8 k, i = 0, s; // ��ʱ���� k,s,   ZCL 2019.11.21 s
	u16 j;
	// ��Ϊ����,ָ������ʱ�䵽��,�Ϳ��Դ�����յ����ַ�����
	//  ��û�յ������ַ���ʱ�䳬���趨ʱ�����ԶԽ��ջ�����д�����
	//  **********************************rcv_counter<>0,�յ��ַ����ܴ���
	if (Cw_Rcv2 > 0 && T_NoRcv2 != SClk1Ms)
	{ // ���մ������
		// RS485_CON=0;
		T_NoRcv2 = SClk1Ms; //
		C_NoRcv2++;
		if (C_NoRcv2 > NORCVMAXMS) //
		{
			//
			USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); // ��ֹUART1�����ж�,��ֹ�ڴ����ڼ����
			Cw_BakRcv2 = Cw_Rcv2;							// �� Cw_Rcv2 ����
			C_NoRcv2 = 0;									// ��û�н��ռ�����

			//
			if (Cw_BakRcv2 <= RCV2_MAX) // ���ճ�����ȷ,��������.
			{
				// ZCL 2019.11.21 ȡ�����if��ע�ͣ��ô���2���Է���
				if (F_LoRaToCom == 0 && F_GprsTransToCom == 0 && F_GpsToCom == 0) // ZCL 2019.11.29 �� && F_GpsToCom==0
				{
					// ZCL 2018.11.13 ע�͵����ĳɴ���2���յ���LoRa����
					if (Rcv2Buffer[0] == Pw_LoRaEquipmentNo) // �ӵ�ַ��⣭���յ�����λ����ѯָ��
					{
						j = CRC16(Rcv2Buffer, Cw_BakRcv2 - 2); // CRC У��
						k = j >> 8;
						s = j;
						if (k == Rcv2Buffer[Cw_BakRcv2 - 2] && s == Rcv2Buffer[Cw_BakRcv2 - 1])
						{							// CRCУ����ȷ
							if (Rcv2Buffer[1] == 3) // 03��ȡ���ּĴ���
							{
								B_Com2Cmd03 = 1;
								j = Rcv2Buffer[2];
								Lw_Com2RegAddr = (j << 8) + Rcv2Buffer[3];
							}
							else if (Rcv2Buffer[1] == 16) // 16Ԥ�ö�Ĵ���
							{
								C_ForceSavPar = 0; // ǿ�Ʊ������������=0
								B_Com2Cmd16 = 1;
								j = Rcv2Buffer[2];
								Lw_Com2RegAddr = (j << 8) + Rcv2Buffer[3];
							}
							else if (Rcv2Buffer[1] == 1) // 01��ȡ��Ȧ״̬
							{
								B_Com2Cmd01 = 1;
							}
							else if (Rcv2Buffer[1] == 6) // 06Ԥ�õ��Ĵ���
							{
								C_ForceSavPar = 0; // ǿ�Ʊ������������=0
								B_Com2Cmd06 = 1;
								j = Rcv2Buffer[2];
								Lw_Com2RegAddr = (j << 8) + Rcv2Buffer[3];
							}
							else
								i = 1;
						}
						else
							i = 2;
					}
					else
						i = 3;
				}

				// ZCL 2018.12.20 Pw_LoRaSet .1λ =1  ת����LORA COM2�����LORA (͸��ʹ��)
				else if (F_LoRaToCom)
				{
					for (j = 0; j < Cw_BakRcv2; j++)
					{
						LoRaTxBuf[j] = Rcv2Buffer[j];

						B_LoRaSendMaster = 2;
					}
				}

				// Pw_GprsSet .1λ =1   GPRS�����COM2
				else if (F_GprsTransToCom)
				{
					if (Cw_BakRcv2 <= RCV2_MAX) // ���ճ�����ȷ,��������.
					{
						// ZCL 2019.4.19 ���ԣ����Դ�ӡ�Ϳ��Ź������� 2019.4.25
						if (Rcv2Buffer[0] == '@' && Rcv2Buffer[1] == '9' && Rcv2Buffer[2] == '9' && Rcv2Buffer[3] == '#' && Cw_BakRcv2 == 4)
						{
							Com2_printf("Test RestartNo=%d\r\n", B_RestartNo); // ZCL 2019.4.19 ������ţ������ӡ�۲�����
							Delay_MS(3000);									   // ��ʱ����û��ι�����豸��λ�� 2019.4.11
						}

						// 1.1 �������Ӷ��ɣ�����ATָ��״̬
						if (Rcv2Buffer[0] == '@' && Rcv2Buffer[1] == '1' && Rcv2Buffer[2] == '1' && Rcv2Buffer[3] == '#' && Cw_BakRcv2 == 4)
						{
							B_ATComStatus = 1;
						}
						// 1.2 �������Ӷ��ɣ��˳�ATָ��״̬
						else if (Rcv2Buffer[0] == '@' && Rcv2Buffer[1] == '2' && Rcv2Buffer[2] == '2' && Rcv2Buffer[3] == '#' && Cw_BakRcv2 == 4)
						{
							B_ATComStatus = 2; // ����ֱ��Ϊ0,@22#����ط���PC,�����ں��淢�ͺ������
						}

						// 2.1 �������Ӷ��ɣ������趨GPRS����״̬ 2013.6.6
						if (Rcv2Buffer[0] == '@' && Rcv2Buffer[1] == '3' && Rcv2Buffer[2] == '3' && Rcv2Buffer[3] == '#' && Cw_BakRcv2 == 4)
						{
							B_PcSetGprs = 1;
							Com2_printf("\r\nEnter PC set gprs par mode\r\n");
						}
						// 2.2 �������Ӷ��ɣ��˳��趨GPRS����״̬ 2013.6.6
						else if (Rcv2Buffer[0] == '@' && Rcv2Buffer[1] == '4' && Rcv2Buffer[2] == '4' && Rcv2Buffer[3] == '#' && Cw_BakRcv2 == 4)
						{
							B_PcSetGprs = 0;
							B_ForceSavPar = 1; // �˳��趨��������� 2013.6.6
							// SavePar_Prompt();						//������� 2012.10.11
							Com2_printf("\r\nQuit PC set gprs par mode, Set end!\r\n");
						}

						// 3 B_PcSetGprs=1������PC�����趨GPRS����״̬�£���ʱ�ٽ��յ����ݣ�����B_PcSetGprs=2��
						// ����Sms_SetupInq()��������
						else if (B_PcSetGprs == 1) //(=1��PC�����趨GPRS����״̬��)		3�ŵ�2.1ǰ�������
						{
							B_PcSetGprs = 2; //=2��Ϊ��־����main����ת��Sms_SetupInq()�����趨GPRS����
											 // ���������������B_PcSetGprs=1��Ȼ���ٵȴ����մ�����һ����Ϣ
						}

						// 5. ������ʱ���Ͱѽ��յ����ݣ����͸�GPRSģ�飬��ֵB_GprsDataReturn
						else if (CGD0_LinkConnectOK[0] + CGD0_LinkConnectOK[1] + CGD0_LinkConnectOK[2] + CGD0_LinkConnectOK[3] != 0 && !B_ATComStatus)
						{
							// 2012.10.12 ���ſ����豸��ͣ�����������������Ϸ���
							if (B_SmsRunStop == 2 || B_SmsRunStop == 3)
							{
								if (Rcv2Buffer[2] == 0x01 && Rcv2Buffer[3] == 0x3D)
								{
									B_SmsRunStop = 3; // ���յ��豸���ͻ�������Ϣ�����Է�������
								}
							}
							else
							{
								B_GprsDataReturn = 1; // �˱�־��1����com0_232.c�л�������
								C_GprsDataSend = 0;
							}
						}

						// 6. �������Ӷ��ɣ���ATָ�ֱ�ӰѴ���2���յ������ݷ��͵�����3(GPRS)
						if (B_ATComStatus) // ע�⣺ȥ����else
						{
							// �Ѵ���2���յ������ݷ��͵�����3
							for (k = 0; k < Cw_BakRcv2; k++)
							{
								Txd3Buffer[k] = Rcv2Buffer[k];
							}
							Cw_Txd3Max = Cw_BakRcv2;
							Cw_Txd3 = 0;
							RS485_CON = 1;
							// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
							USART_ITConfig(USART3, USART_IT_TC, ENABLE); // ��ʼ����.
							USART_SendData(USART3, Txd3Buffer[Cw_Txd3++]);

							// ����������͵����ã��Ѵ���2���յ��ģ��ٻط�������2����PC�ۿ���
							// 2013.5.22 �ѽ��յ������ݻط�������2����Ϊ���Ż��ԣ��᷵������ָ�������
							for (k = 0; k < Cw_BakRcv2; k++)
							{
								Txd2Buffer[k] = Rcv2Buffer[k];
							}
							Cw_Txd2Max = Cw_BakRcv2;
							Cw_Txd2 = 0;
							RS485_CON = 1;
							USART_ITConfig(USART2, USART_IT_TC, ENABLE); // ��ʼ����.
							USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]);

							// 2013.5.23 �յ�@22#���˳�AT״̬�����������
							if (B_ATComStatus == 2)
								B_ATComStatus = 0;
						}
					}

					// 2014.12.9 ��������������ת��ʱ��ֹ���ա�
					// 2016.7.1 ZCL ��Ϊ��������ļ��룬��������ڸ�ÿ·���ӷ�����Ϻ󣬻��ô򿪽��ա�������
					if (B_GprsDataReturn == 0)
					{
						USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
						USART2->CR1 |= 0x0004; //.3λ  ���н���
					}
				}

				// ZCL 2019.11.29 Pw_GprsSet .4λ =1  F_GpsToCom  ����: COM2���յ�����ָ�ת����GPS��(COM4)
				else if (F_GpsToCom)
				{
					for (j = 0; j < Cw_BakRcv2; j++)
					{
						Txd4Buffer[j] = Rcv2Buffer[j];
					}

					// ׼������
					Cw_Txd4Max = Cw_BakRcv2;
					Cw_Txd4 = 0;

					// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
					USART_SendData(UART4, Txd4Buffer[Cw_Txd4++]);
					USART_ITConfig(UART4, USART_IT_TC, ENABLE); // ��ʼ����.
				}
			}
			else
				i = 4;
			USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
			Cw_Rcv2 = 0; // ׼���´ν��յ����濪ʼ
		}
	}

	if (i > 0)
	{
		for (j = 0; j < 20; j++)
		{
			Rcv2Buffer[j] = 0;
		}
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	}
}

void Com2_SlaveSend(void) // ����2�ӻ�����
{
	u16 m, n, j = 0;
	u8 k;
	u16 *p_wRead;
	u8 *p_bMove;
	u16 *p_wTarget;
	u16 Lw_Com2Addr;
	u8 *p_bGen;

	if (Pw_EquipmentType == 0) // ˫����
	{
		//
		if (B_Com2Cmd03) // ��ȡ���ּĴ���
		{
			Txd2Buffer[0] = Pw_LoRaEquipmentNo; // �豸�ӵ�ַPw_EquipmentNo
			Txd2Buffer[1] = Rcv2Buffer[1];		// ������
			Txd2Buffer[2] = Rcv2Buffer[5] * 2;	// Rcv2Buffer[5]=���� ��

			// ת����ַ
			p_wRead = AddressConvert(Lw_Com2RegAddr); // 2023.12.16 YLS

			p_bMove = Txd2Buffer;
			for (k = 0; k < Rcv2Buffer[5]; k++) // ����ѯ����
			{
				// m = *(p_wRead + Lw_Com2RegAddr + k);// 2023.12.16 YLS
				m = *(p_wRead + k); // 2023.12.16 YLS
				*(p_bMove + 3 + k * 2) = m >> 8;
				*(p_bMove + 3 + k * 2 + 1) = m;
			}
			Lw_Txd2ChkSum = CRC16(Txd2Buffer, Txd2Buffer[2] + 3);
			Txd2Buffer[Txd2Buffer[2] + 3] = Lw_Txd2ChkSum >> 8; // /256
			Txd2Buffer[Txd2Buffer[2] + 4] = Lw_Txd2ChkSum;		// ��λ�ֽ�
			Cw_Txd2Max = Txd2Buffer[2] + 5;
			//
			//
			B_Com2Cmd03 = 0;
			Cw_Txd2 = 0;
			// RS485_CON=1;
			// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
			// USART_ITConfig(USART2, USART_IT_TXE, ENABLE);				// ��ʼ����.
			USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]);
			USART_ITConfig(USART2, USART_IT_TC, ENABLE); // ��ʼ����.
		}
		//
		else if (B_Com2Cmd16 || B_Com2Cmd06) // 16Ԥ�ö�Ĵ���
		{

			if (B_Com2Cmd06) // Ԥ�õ���
			{
				j = Rcv2Buffer[2];
				Lw_Com2Addr = (j << 8) + Rcv2Buffer[3];
				// ����޸�ZGP331�����������
				if (Lw_Com2Addr >= STM32_RPAR_ADDRESS) // �����ַ��10000���ϣ����޸�ZGP331����Ĳ���
				{
					// ����Ԥ�ñ����� �趨������
					p_wTarget = AddressConvert(Lw_Com2Addr);
					m = Rcv2Buffer[4];
					// w_ParLst[Lw_Com3RegAddr] = (m << 8) + Txd2Buffer[5];
					*p_wTarget = (m << 8) + Rcv2Buffer[5];
					B_ForceSavPar = 1; // �������
				}
				else
				{
					// m = Rcv2Buffer[2];
					// m = (m << 8) + Rcv2Buffer[3];
					w_ZhuanFaAdd = Lw_Com2Addr;
					m = Rcv2Buffer[4];
					m = (m << 8) + Rcv2Buffer[5];
					//				*(p_wTarget + Lw_Com2RegAddr) = m;
					w_ZhuanFaData = m;
					F_ModeParLora = 1;
				}
			}

			else if (B_Com2Cmd16) // Ԥ�ö��
			{
				if (Rcv2Buffer[6] == 2)
				{
					m = Rcv2Buffer[2];
					m = (m << 8) + Rcv2Buffer[3];
					w_ZhuanFaAdd = m;

					m = Rcv2Buffer[7];
					m = (m << 8) + Rcv2Buffer[8];
					w_ZhuanFaData = m;

					F_ModeParLora = 1;
				}
				else
					F_ModeParLora = 0;
			}

			// -------------------------
			// ��������
			// 2015.7.11 �����жϣ���Ϊ�趨DSP����ʱ�����ԭ���Ѿ����趨�����У�������
			if (B_Com2Cmd16 || B_Com2Cmd06)
			{
				Txd2Buffer[0] = Pw_LoRaEquipmentNo; // �豸�ӵ�ַ
				Txd2Buffer[1] = Rcv2Buffer[1];		// ������
				Txd2Buffer[2] = Rcv2Buffer[2];		// ��ʼ��ַ��λ�ֽ�
				Txd2Buffer[3] = Rcv2Buffer[3];		// ��ʼ��ַ��λ�ֽ�
				Txd2Buffer[4] = Rcv2Buffer[4];		// �Ĵ���������λ
				Txd2Buffer[5] = Rcv2Buffer[5];		// �Ĵ���������λ
				/*
				if (j == 0)					   // ������ܱ�����Ԥ�ã�����FFFF zcl
				{
					Txd2Buffer[4] = 0xff; // �Ĵ���������λ��Ԥ������
					Txd2Buffer[5] = 0xff; // �Ĵ���������λ��Ԥ������
				}
				*/
				Lw_Txd2ChkSum = CRC16(Txd2Buffer, 6);
				Txd2Buffer[6] = Lw_Txd2ChkSum >> 8; // /256
				Txd2Buffer[7] = Lw_Txd2ChkSum;		// ��λ�ֽ�
				Cw_Txd2Max = 8;
				B_Com2Cmd16 = 0;
				B_Com2Cmd06 = 0;
				Cw_Txd2 = 0;
				// RS485_CON=1;
				// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
				// USART_ITConfig(USART2, USART_IT_TXE, ENABLE);				// ��ʼ����.
				USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]);
				USART_ITConfig(USART2, USART_IT_TC, ENABLE); // ��ʼ����.
			}
		} // 06��16Ԥ�üĴ��� ����
	}
	else // ��Ƶ���
	{
		//
		if (B_Com2Cmd03) // ��ȡ���ּĴ���
		{
			Txd2Buffer[0] = 2;				   // �豸�ӵ�ַPw_EquipmentNo
			Txd2Buffer[1] = Rcv2Buffer[1];	   // ������
			Txd2Buffer[2] = Rcv2Buffer[5] * 2; // Rcv2Buffer[5]=���� ��

			// ת����ַ ZCL 2015.7.11
			if (Lw_Com2RegAddr < STM32_RPAR_ADDRESS)
			{
				p_wRead = Pw_ParLst; // �趨STM32 PAR��
				Lw_Com2RegAddr = Lw_Com2RegAddr - STM32_WRPAR_ADDRESS_BPDJ;
			}
			else if (Lw_Com2RegAddr < DSP_WRPAR_ADDRESS)
			{
				p_wRead = w_ParLst;									  // ��STM32 PAR��
				Lw_Com2RegAddr = Lw_Com2RegAddr - STM32_RPAR_ADDRESS; // 10000
			}
			else if (Lw_Com2RegAddr < DSP_RPAR_ADDRESS)
			{
				p_wRead = Pw_dspParLst;								 // �趨DSP PAR��
				Lw_Com2RegAddr = Lw_Com2RegAddr - DSP_WRPAR_ADDRESS; // 20000
			}
			// else   //ZCL 2019.11.22
			else if (Lw_Com2RegAddr < SCR_WRPAR_ADDRESS)
			{
				p_wRead = w_dspParLst;								// ��DSP PAR��
				Lw_Com2RegAddr = Lw_Com2RegAddr - DSP_RPAR_ADDRESS; // 30000
			}

			// ZCL 2019.11.22 ���� w_GprsParLst PAR��
			else
			{
				p_wRead = w_GprsParLst; // w_GprsParLst PAR��
				Lw_Com2RegAddr = Lw_Com2RegAddr - SCR_WRPAR_ADDRESS;
			}

			p_bMove = Txd2Buffer;
			for (k = 0; k < Rcv2Buffer[5]; k++) // ����ѯ����
			{
				m = *(p_wRead + Lw_Com2RegAddr + k);
				*(p_bMove + 3 + k * 2) = m >> 8;
				*(p_bMove + 3 + k * 2 + 1) = m;
			}
			Lw_Txd2ChkSum = CRC16(Txd2Buffer, Txd2Buffer[2] + 3);
			Txd2Buffer[Txd2Buffer[2] + 3] = Lw_Txd2ChkSum >> 8; // /256
			Txd2Buffer[Txd2Buffer[2] + 4] = Lw_Txd2ChkSum;		// ��λ�ֽ�
			Cw_Txd2Max = Txd2Buffer[2] + 5;
			//
			//
			B_Com2Cmd03 = 0;
			Cw_Txd2 = 0;
			// RS485_CON=1;
			// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
			// USART_ITConfig(USART2, USART_IT_TXE, ENABLE);				// ��ʼ����.
			USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]);
			USART_ITConfig(USART2, USART_IT_TC, ENABLE); // ��ʼ����.
		}
		//
		else if (B_Com2Cmd16 || B_Com2Cmd06) // 16Ԥ�ö�Ĵ���
		{

			j = 1;

			// �޸Ĳ�����Ԫ
			if (j)
			{
				// ת����ַ ZCL 2015.7.11
				if (Lw_Com2RegAddr < STM32_RPAR_ADDRESS)
				{
					p_wTarget = Pw_ParLst; // �趨STM32 PAR��
					Lw_Com2RegAddr = Lw_Com2RegAddr - STM32_WRPAR_ADDRESS_BPDJ;
				}
				else if (Lw_Com2RegAddr < DSP_WRPAR_ADDRESS)
				{
					p_wTarget = w_ParLst; // ��STM32 PAR��
					Lw_Com2RegAddr = Lw_Com2RegAddr - STM32_RPAR_ADDRESS;
				}
				else if (Lw_Com2RegAddr < DSP_RPAR_ADDRESS) // ԭ�����޸Ĳ�����־�����ٱ���
				{
					p_wTarget = Pw_dspParLst; // �趨DSP PAR��
					Lw_Com2RegAddr = Lw_Com2RegAddr - DSP_WRPAR_ADDRESS;
					// ��ʾҪ�޸�DSP��������Ҫ��COM1��ת�����ָ��  ZCL 2015.7.11
					if (!F_ModDspPar)
						F_ModDspPar = 1;
					else // ԭ�����޸Ĳ�����־��������˴ε��޸�ָ�
					{
						B_Com2Cmd16 = 0;
						B_Com2Cmd06 = 0;
					}
				}

				/* 			else
							{
								p_wTarget=w_dspParLst;			// ��DSP PAR��
								Lw_Com2RegAddr=Lw_Com2RegAddr-DSP_RPAR_ADDRESS;
							}				 */

				// else   // 06 16 ָ�� дֻ����DSP PAR��������Ҫ��//ZCL 2019.11.22
				else if (Lw_Com2RegAddr < SCR_WRPAR_ADDRESS)
				{
					// p_wTarget=w_dspParLst;			// ��DSP PAR��
					// Lw_Com2RegAddr=Lw_Com2RegAddr-DSP_RPAR_ADDRESS;
					;
				}

				// ZCL 2019.11.22 ���� w_GprsParLst PAR��
				else
				{
					p_wTarget = w_GprsParLst; // w_GprsParLst PAR��
					Lw_Com2RegAddr = Lw_Com2RegAddr - SCR_WRPAR_ADDRESS;
				}

				if (B_Com2Cmd06) // Ԥ�õ���
				{
					m = Rcv2Buffer[4];
					m = (m << 8) + Rcv2Buffer[5];
					*(p_wTarget + Lw_Com2RegAddr) = m;

					// ��ΪҪת����DSP���ѵ�ַ��ֵ���������� ZCL  2015.7.11
					if (F_ModDspPar)
					{
						w_ModDspParAdd = Lw_Com2RegAddr;
						w_ModDspParValue = m;
					}
				}

				else if (B_Com2Cmd16) // Ԥ�ö��
				{
					if (Rcv2Buffer[5] < 90)
					{
						p_bGen = Rcv2Buffer;
						for (k = 0; k < Rcv2Buffer[5]; k++) // Rcv2Buffer[5]=����
						{
							m = *(p_bGen + 7 + k * 2);
							n = *(p_bGen + 7 + k * 2 + 1);
							m = (m << 8) + n;
							*(p_wTarget + Lw_Com2RegAddr + k) = m;
						}

						// ��ΪҪת����DSP���ѵ�ַ��ֵ���������� ZCL  2015.7.11
						// ������ʱֻ�ʺ�Ԥ��һ��DSP����  ZCL  2015.7.11
						if (F_ModDspPar)
						{
							w_ModDspParAdd = Lw_Com2RegAddr;
							w_ModDspParValue = m;
						}
					}
				}
			}

			// -------------------------
			// ��������
			// 2015.7.11 �����жϣ���Ϊ�趨DSP����ʱ�����ԭ���Ѿ����趨�����У�������
			if (B_Com2Cmd16 || B_Com2Cmd06)
			{
				Txd2Buffer[0] = 2;			   // �豸�ӵ�ַ
				Txd2Buffer[1] = Rcv2Buffer[1]; // ������
				Txd2Buffer[2] = Rcv2Buffer[2]; // ��ʼ��ַ��λ�ֽ�
				Txd2Buffer[3] = Rcv2Buffer[3]; // ��ʼ��ַ��λ�ֽ�
				Txd2Buffer[4] = Rcv2Buffer[4]; // �Ĵ���������λ
				Txd2Buffer[5] = Rcv2Buffer[5]; // �Ĵ���������λ
				if (j == 0)					   // ������ܱ�����Ԥ�ã�����FFFF zcl
				{
					Txd2Buffer[4] = 0xff; // �Ĵ���������λ��Ԥ������
					Txd2Buffer[5] = 0xff; // �Ĵ���������λ��Ԥ������
				}
				Lw_Txd2ChkSum = CRC16(Txd2Buffer, 6);
				Txd2Buffer[6] = Lw_Txd2ChkSum >> 8; // /256
				Txd2Buffer[7] = Lw_Txd2ChkSum;		// ��λ�ֽ�
				Cw_Txd2Max = 8;
				B_Com2Cmd16 = 0;
				B_Com2Cmd06 = 0;
				Cw_Txd2 = 0;
				// RS485_CON=1;
				USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]);
				USART_ITConfig(USART2, USART_IT_TC, ENABLE); // ��ʼ����.
			}
		} // 06��16Ԥ�üĴ��� ����
	}
}

// ��Ϊ��ģ��ʱ����
void Com2_MasterSend(void) // ����2�����ͳ���
{
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
	RS485_CON = 1;
	//  USART_SendData(USART1, (u8) ch);
	USART_SendData(USART2, (u8)ch);

	/* Loop until the end of transmission */
	//  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
	{
	}
	RS485_CON = 0;
	return ch;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
