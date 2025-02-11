/**
  ******************************************************************************
  * @file    com4_485.c
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.10
  * @date    2015-04-13
  * @brief   Com1 USE UART4
	******************************************************************************
	* @note	2015.04.13
		COM4: GPS ATGM336H
	*/

/* Includes ------------------------------------------------------------------*/
#include "com4_232.h"
#include "GlobalV_Extern.h" // ȫ�ֱ�������
#include "GlobalConst.h"
#include <stdio.h>	//���ϴ˾������printf
#include "string.h" //strstr�������ҳ�str2�ַ�����str1�ַ����е�һ�γ��ֵ�λ��

#include "spi_flash.h" //ZCL 2020.3.26
#include "com1_232.h"  //ZCL 2020.3.26

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
char Txd4Buffer[TXD4_MAX]; // ���ͻ�����
char Rcv4Buffer[RCV4_MAX]; // ���ջ�����
u16 Cw_Rcv4;			   // ���ռ�����//
u16 Cw_Txd4;			   // ���ͼ�����//
u16 Cw_BakRcv4;			   // ���ռ�����//
u16 Cw_Txd4Max;			   // �ж��ٸ��ַ���Ҫ����//
u16 Lw_Txd4ChkSum;		   // ����У��ͣ�lo,hi ��λ//
u16 Lw_Com4RegAddr;		   // ����1�Ĵ�����ַ
//
u8 B_Com4Cmd03;
u8 B_Com4Cmd16;
u8 B_Com4Cmd01;
u8 B_Com4Cmd06;
u8 B_Com4Cmd73;
u8 T_NoRcv4; // û�н��ռ�����
u16 C_NoRcv4;

u8 B_SaveLatLon; // ZCL 2019.9.11  ��ʱ���棬��γ��

u8 *Gps_ptr, *Gps_Gps_ptr2, *Gps_Gps_ptr3; // ָ��
u8 GPS_StringBuffer[12];				   // BCD��ת���ַ������档

/* Private variables extern --------------------------------------------------*/
extern u16 C_ForceSavPar; // ǿ�Ʊ������������
extern u8 F_ModDspPar;	  // �޸�DSP����

extern u8 Txd2Buffer[TXD1_MAX]; // ���ͻ�����
extern u8 Rcv2Buffer[RCV1_MAX]; // ���ջ�����
extern u16 Cw_Rcv2;				// ���ռ�����//
extern u16 Cw_Txd2;				// ���ͼ�����//
extern u16 Cw_BakRcv2;			// ���ռ�����//
extern u16 Cw_Txd2Max;			// �ж��ٸ��ַ���Ҫ����//

/* Private function prototypes -----------------------------------------------*/
void GPIO_Com4_Configuration(void); // GPIO����
void Com4_config(void);
u16 CRC16(u8 *pCrcData, u8 CrcDataLen);
void Delay_MS(vu16 nCount);

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
 * Function Name  : GPIO_Com4_Configuration
 * Description    : Configures the different GPIO ports.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void GPIO_Com4_Configuration(void) // GPIO����
{
	GPIO_InitTypeDef GPIO_InitStructure; // ����GPIO��ʼ���ṹ��

	// ����˵����2�����ڷֱ���Usart1.Usart3(USART���±��1��ʼ,��STM32F103 ARM�Ĵ�������)
	// Com4 ʵ��Ϊ��һ������
	/* Configure 232  */ // RS232û�п��ƶ�
	/* Configure Com4(ʹ��Ӳ��:UART4) Rx (PC.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure Com4(ʹ��Ӳ��:UART4) Tx (PC.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// GPIO_ResetBits(GPIOC, GPIO_Pin_10);
}

/* UART4  configuration ------------------------------------------------------*/
/* USART  configured as follow:
		- BaudRate = 9600 baud
		- Word Length = 8 Bits
		- One Stop Bit
		- No parity
		- Hardware flow control disabled (RTS and CTS signals)
		- Receive and transmit enabled
				RTS/CTSЭ��(Request To Send/Clear To Send)��������/������Э��
*/
void Com4_config(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_DeInit(UART4);					// ��λ Usart1
	USART_StructInit(&USART_InitStructure); // ��USART_InitStruct�е�ÿһ��������ȱʡֵ����

	USART_InitStructure.USART_BaudRate = 9600; // 2.25M/BPS   RS232оƬ:230.4kbps
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure ����1-UART4 */
	USART_Init(UART4, &USART_InitStructure);
	/* Enable UART4 Receive and Transmit interrupts */
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	/* Enable the UART4 */
	USART_Cmd(UART4, ENABLE);
}

/*******************************************************************************
 * Function Name  : Com4_Init
 * Description    : Initializes the peripherals used by the Usart driver.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Com4_Init(void)
{
	GPIO_Com4_Configuration();
	Com4_config();
	//
	Cw_Txd4 = 0;
	Cw_Rcv4 = 0;
}

// ���մ������ У�����
void Com4_RcvProcess(void)
{
	u8 i = 0, k, m, n; // ��ʱ���� k,
	u8 u = 0;
	//	u8 v=0,w=0;
	u16 j;
	float nf_Du; // ZCL 2018.10.28

	// ��Ϊ����,ָ������ʱ�䵽��,�Ϳ��Դ�����յ����ַ�����
	//  ��û�յ������ַ���ʱ�䳬���趨ʱ�����ԶԽ��ջ�����д�����
	//  **********************************rcv_counter<>0,�յ��ַ����ܴ���
	if (Cw_Rcv4 > 0 && T_NoRcv4 != SClk1Ms)
	{ // ���մ������
		// RS485_CON=0;
		T_NoRcv4 = SClk1Ms; //
		C_NoRcv4++;
		// 1.ʱ��У��
		if (C_NoRcv4 > NORCVMAXMS) //
		{
			//
			USART_ITConfig(UART4, USART_IT_RXNE, DISABLE); // ��ֹUART1�����ж�,��ֹ�ڴ����ڼ����
			Cw_BakRcv4 = Cw_Rcv4;						   // �� Cw_Rcv4 ����
			C_NoRcv4 = 0;								   // ��û�н��ռ�����
			//
			// if(Cw_BakRcv4<=RCV4_MAX)		// ���ճ�����ȷ,��������.  ZCL123 2018.10.26
			// 2. ��������У��
			if (Cw_BakRcv4 > 10)
			{
				// 3. ��ʼ��'$'  α������'*' �����滹�����У����2�ֽں� 0x0d  0x0a��
				if (Rcv4Buffer[0] == '$' && Rcv4Buffer[Cw_BakRcv4 - 5] == '*' && Rcv4Buffer[Cw_BakRcv4 - 2] == 0x0d && Rcv4Buffer[Cw_BakRcv4 - 1] == 0x0a)
				{
					j = CalcCheck((u8 *)Rcv4Buffer, Cw_BakRcv4 - 5); // CRC У��
					m = (j % 256) / 16;
					if (m >= 10) // A-F(A:0x41)
						m = m - 10 + 0x41;
					else // 0-9
						m = m + 0x30;

					n = (j % 16);
					if (n >= 10) // A-F(A:0x41)
						n = n - 10 + 0x41;
					else // 0-9
						n = n + 0x30;
					// 4. �����У��
					if (m == Rcv4Buffer[Cw_BakRcv4 - 4] && n == Rcv4Buffer[Cw_BakRcv4 - 3])
					{
						// ���Ѳ����ǵ�ʱ�򣬷���,,,,,��
						if (strstr((char *)Rcv4Buffer, ",,,") == NULL) // �������ݵ�ʱ���Ѳ���,,,������ֵΪ0
						{
							// ʾ�� $GPGGA,235316.000,2959.9925,S,12000.0090,E,1,06,1.21,62.77,M,0.00,M,,*7B
							// 5.1 γ�ȼ������		ZCL123  ע����֤ �ȣ���һλΪ0�Ƿ�Ҳ���ͳ�����
							// Lat ddmm.mmmm γ�ȣ�ǰ 2 �ַ���ʾ�ȣ�������ַ���ʾ��
							// w_ScrGpsLat				//.0 γ�� ��-�֣�	ddmm.mmmm֮ddmm

							// ZCL 2020.11.24 ��γ�Ⱥ�4λ��5λС��������
							if (strstr(Rcv4Buffer, ",N,") != NULL)
							{
								Gps_ptr = (u8 *)strstr(Rcv4Buffer, ",N,"); // ZCL 2020.11.24 ԭ��4λС����
								if (*(Gps_ptr - 5) == '.')				   // 0x2E:'.'
								{
									w_ScrGpsLatDu = (*(Gps_ptr - 9) - 0x30) * 10 + (*(Gps_ptr - 8) - 0x30);
									w_ScrGpsLatFen1 = (*(Gps_ptr - 7) - 0x30) * 10 + (*(Gps_ptr - 6) - 0x30);
									w_ScrGpsLatFen2 = (*(Gps_ptr - 4) - 0x30) * 1000 + (*(Gps_ptr - 3) - 0x30) * 100 + (*(Gps_ptr - 2) - 0x30) * 10 + (*(Gps_ptr - 1) - 0x30);
								}
								else if (*(Gps_ptr - 6) == '.') // 0x2E:'.'				//ZCL 2020.11.24 ����5λС����
								{
									w_ScrGpsLatDu = (*(Gps_ptr - 10) - 0x30) * 10 + (*(Gps_ptr - 9) - 0x30);
									w_ScrGpsLatFen1 = (*(Gps_ptr - 8) - 0x30) * 10 + (*(Gps_ptr - 7) - 0x30);
									w_ScrGpsLatFen2 = (*(Gps_ptr - 5) - 0x30) * 1000 + (*(Gps_ptr - 4) - 0x30) * 100 + (*(Gps_ptr - 3) - 0x30) * 10 + (*(Gps_ptr - 2) - 0x30);
								}
							}
							else
							{
								w_ScrGpsLatDu = 0;
								w_ScrGpsLatFen1 = 0;
								w_ScrGpsLatFen2 = 0;
							}

							// 5.2 ���ȼ������
							// Lon dddmm.mmmm ���ȣ�ǰ 3 �ַ���ʾ�ȣ�������ַ���ʾ��

							// ZCL 2020.11.24 ��γ�Ⱥ�4λ��5λС��������
							if (strstr(Rcv4Buffer, ",E,") != NULL)
							{
								Gps_ptr = (u8 *)strstr((char *)Rcv4Buffer, ",E,"); // ZCL 2020.11.24 ԭ��4λС����
								if (*(Gps_ptr - 5) == '.')						   // 0x2E:'.'
								{
									w_ScrGpsLonDu = (*(Gps_ptr - 10) - 0x30) * 100 + (*(Gps_ptr - 9) - 0x30) * 10 + (*(Gps_ptr - 8) - 0x30);
									w_ScrGpsLonFen1 = (*(Gps_ptr - 7) - 0x30) * 10 + (*(Gps_ptr - 6) - 0x30);
									w_ScrGpsLonFen2 = (*(Gps_ptr - 4) - 0x30) * 1000 + (*(Gps_ptr - 3) - 0x30) * 100 + (*(Gps_ptr - 2) - 0x30) * 10 + (*(Gps_ptr - 1) - 0x30);
								}
								else if (*(Gps_ptr - 6) == '.') // 0x2E:'.'					//ZCL 2020.11.24 ����5λС����
								{
									w_ScrGpsLonDu = (*(Gps_ptr - 11) - 0x30) * 100 + (*(Gps_ptr - 10) - 0x30) * 10 + (*(Gps_ptr - 9) - 0x30);
									w_ScrGpsLonFen1 = (*(Gps_ptr - 8) - 0x30) * 10 + (*(Gps_ptr - 7) - 0x30);
									w_ScrGpsLonFen2 = (*(Gps_ptr - 5) - 0x30) * 1000 + (*(Gps_ptr - 4) - 0x30) * 100 + (*(Gps_ptr - 3) - 0x30) * 10 + (*(Gps_ptr - 2) - 0x30);
								}
							}
							else
							{
								w_ScrGpsLonDu = 0;
								w_ScrGpsLonFen1 = 0;
								w_ScrGpsLonFen2 = 0;
							}
							i = 1;

							// 6. γ�� ����  ��ʽת��
							// ZCL 2019.9.12 һ���ε�GPS���ߣ�����ֵ,,,�������߲������
							// γ��
							u = 0;
							//							v=0;
							//							w=0;
							nf_Du = (float)w_ScrGpsLatDu + (float)w_ScrGpsLatFen1 / 60 + (float)w_ScrGpsLatFen2 / 600000;

							if (nf_Du > 0.1) // ZCL 2019.9.11
							{
								w_ScrGpsLatAllDu1 = (int)(nf_Du * 100);								  // 35.44
								w_ScrGpsLatAllDu2 = (int)((nf_Du * 100 - w_ScrGpsLatAllDu1) * 10000); // 00.00****
								// ZCL 2019.9.11 ���γ�Ȼ������䣬���ñ���
								if (w_ScrGpsLatAllDu1 == Pw_ScrGpsLatAllDu1)
								{
									if ((w_ScrGpsLatAllDu2 / 100) == (Pw_ScrGpsLatAllDu2 / 100)) // ZCL 2019.9.11 ���10���ݱ仯�ٵ���
									{
										// if(B_SaveLatLon>0)
										// B_SaveLatLon--;
										//										v=1; //v>0����Ҫ���棬�۲����������ɾ������Ҫ��u������ZCL 2019.9.12
									}
									else
										u = 1;
								}
								else
									u = 2;
							}
							else // ZCL 2019.9.11 ʹ�ñ����ֵ
							{
								w_ScrGpsLatAllDu1 = Pw_ScrGpsLatAllDu1;
								w_ScrGpsLatAllDu2 = Pw_ScrGpsLatAllDu2;
								// if(B_SaveLatLon>0)
								// B_SaveLatLon--;
								//								v=2; //v>0����Ҫ���棬�۲����������ɾ������Ҫ��u������ZCL 2019.9.12
							}

							// ����
							nf_Du = (float)w_ScrGpsLonDu + (float)w_ScrGpsLonFen1 / 60 + (float)w_ScrGpsLonFen2 / 600000;
							if (nf_Du > 0.1) // ZCL 2019.9.11
							{
								w_ScrGpsLonAllDu1 = (int)(nf_Du * 100);
								w_ScrGpsLonAllDu2 = (int)((nf_Du * 100 - w_ScrGpsLonAllDu1) * 10000); // 00.00****
								// ZCL 2019.9.11 ������Ȼ������䣬���ñ���
								if (w_ScrGpsLonAllDu1 == Pw_ScrGpsLonAllDu1)
								{
									if ((w_ScrGpsLonAllDu2 / 100) == (Pw_ScrGpsLonAllDu2 / 100)) // ZCL 2019.9.11 ���10���ݱ仯�ٵ���
									{
										// if(B_SaveLatLon>0)
										// B_SaveLatLon--;
										//										v=3; //v>0����Ҫ���棬�۲����������ɾ������Ҫ��u������ZCL 2019.9.12
									}
									else
										u = 3;
								}
								else
									u = 4;
							}
							else // ZCL 2019.9.11 ʹ�ñ����ֵ
							{
								w_ScrGpsLonAllDu1 = Pw_ScrGpsLonAllDu1;
								w_ScrGpsLonAllDu1 = Pw_ScrGpsLonAllDu1;
								// if(B_SaveLatLon>0)
								// B_SaveLatLon--;
								//								v=4; //v>0����Ҫ���棬�۲����������ɾ������Ҫ��u������ZCL 2019.9.12
							}

							// ZCL 2019.9.11
							// 7. ��γ�ȱ仯���������档ԭ�ȼƻ�6���Ӻ󱣴�
							// 7.1
							if (u > 0) // u>0 ��γ�ȱ仯���ȸ��±���ֵ
							{
								Pw_ScrGpsLatAllDu1 = w_ScrGpsLatAllDu1;
								Pw_ScrGpsLatAllDu2 = w_ScrGpsLatAllDu2;

								Pw_ScrGpsLonAllDu1 = w_ScrGpsLonAllDu1;
								Pw_ScrGpsLonAllDu2 = w_ScrGpsLonAllDu2;

								//
								SPI_FLASH_CS_LOW();
								// ZCL 2019.9.11  ����GPS γ�Ⱥ;��Ȳ��� �� FM25L16

								// #define	Pw_ScrGpsLatAllDu1				w_GprsParLst[10]  ZCL 2019.9.11 10*2������
								// SPI_FMRAM_BufferWrite((u8 *)(&Pw_ScrGpsLatAllDu1), 10*2, 4*2);	//���Ժ��ã�

								// ZCL 2019.9.11 ������治���ã������  ZCL����1	���Ժ��ã�
								SPI_FMRAM_BufferWrite((u8 *)(&Pw_ScrGpsLatAllDu1), Position(w_GprsParLst, &Pw_ScrGpsLatAllDu1) * 2, 4 * 2);

								// ZCL 2019.9.11 ������治���ã������  ZCL����2
								// SPI_FMRAM_BufferWrite((u8 *)(&w_GprsParLst), 0, 14*2);
							}
							// ZCL 2022.5.22  ��γ�� ��䵽 ���ܱ�1������
							w_DNBScrGpsLatAllDu1 = w_ScrGpsLatAllDu1;
							w_DNBScrGpsLatAllDu2 = w_ScrGpsLatAllDu2;

							w_DNBScrGpsLonAllDu1 = w_ScrGpsLonAllDu1;
							w_DNBScrGpsLonAllDu2 = w_ScrGpsLonAllDu2;
						}
						else
							i = 2;
					}
					else
						i = 3;
				}
				else
					i = 4;
			}
			else
				i = 5;

			// ZCL 2018.12.6
			if (i > 0)
			{
				if (F_GpsToCom) // GPS ͸�����������COM2  .4λ ֵ��16
				{
					for (k = 0; k < Cw_BakRcv4; k++)
					{
						Txd2Buffer[k] = Rcv4Buffer[k]; // GPS���ݷ��͵�����2
					}

					// ZCL 2018.12.6 i==2�ǻ�Ҫ������л�����
					if (i == 2)
					{
						for (k = 0; k < RCV4_MAX; k++)
						{
							Rcv4Buffer[k] = 0; // GPS���ݷ��͵�����2
						}
					}

					// ׼������
					Cw_Txd2Max = Cw_BakRcv4;
					Cw_Txd2 = 0;

					// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
					// USART_ITConfig(USART2, USART_IT_TXE, ENABLE);				// ��ʼ����.
					USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]);
					USART_ITConfig(USART2, USART_IT_TC, ENABLE); // ��ʼ����.
				}
				else
				{
					// ZCL 2018.12.6 i==2�ǻ�Ҫ������л�����
					if (i == 2)
					{
						for (k = 0; k < RCV4_MAX; k++)
						{
							Rcv4Buffer[k] = 0; // GPS���ݷ��͵�����2
						}
						i = 0; // ZCL 2019.6.25
					}
				}
			}

			USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
			Cw_Rcv4 = 0; // ׼���´ν��յ����濪ʼ
		}
	}

	// ZCL 2018.10.27 ������ջ���
	if (i > 0)
	{
		for (j = 0; j < 20; j++)
		{
			Rcv4Buffer[j] = 0;
		}
		USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);

		Cw_Rcv4 = 0; // ׼���´ν��յ����濪ʼ  2019.6.25
	}
}

void Com4_SlaveSend(void) // ����2�ӻ�����
{
	u16 m, n;
	u8 j = 0, k;
	u16 *p_wRead;
	u8 *p_bMove;
	u8 *p_bGen;
	u16 *p_wTarget; // ָ��Ŀ���ַ�����xdata zcl

	if (Pw_EquipmentType == 0) // ˫����
	{
		//
		if (B_Com4Cmd03) // ��ȡ���ּĴ���
		{
			Txd4Buffer[0] = Pw_LoRaEquipmentNo; // �豸�ӵ�ַPw_EquipmentNo
			Txd4Buffer[1] = Rcv4Buffer[1];		// ������
			Txd4Buffer[2] = Rcv4Buffer[5] * 2;	// Rcv4Buffer[5]=���� ��

			// ת����ַ ZCL 2015.7.11

			p_wRead = AddressChange_2(Lw_Com4RegAddr);

			p_bMove = (u8 *)Txd4Buffer;
			for (k = 0; k < Rcv4Buffer[5]; k++) // ����ѯ����
			{
				// m = *(p_wRead + Lw_Com4RegAddr + k);	//2023.12.16 YLS
				m = *(p_wRead + k); // 2023.12.16 YLS ��AddressChange_2(Lw_Com4RegAddr)�������Ѿ�������Lw_Com4RegAddr���ƫ����
				*(p_bMove + 3 + k * 2) = m >> 8;
				*(p_bMove + 3 + k * 2 + 1) = m;
			}
			Lw_Txd4ChkSum = CRC16((u8 *)Txd4Buffer, Txd4Buffer[2] + 3);
			Txd4Buffer[Txd4Buffer[2] + 3] = Lw_Txd4ChkSum >> 8; // /256
			Txd4Buffer[Txd4Buffer[2] + 4] = Lw_Txd4ChkSum;		// ��λ�ֽ�
			Cw_Txd4Max = Txd4Buffer[2] + 5;
			//
			//
			B_Com4Cmd03 = 0;
			Cw_Txd4 = 0;
			// RS485_CON=1;
			// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
			// USART_ITConfig(UART4, USART_IT_TXE, ENABLE);				// ��ʼ����.
			USART_SendData(UART4, Txd4Buffer[Cw_Txd4++]);
			USART_ITConfig(UART4, USART_IT_TC, ENABLE); // ��ʼ����.
		}
		//
		else if (B_Com4Cmd16 || B_Com4Cmd06) // 16Ԥ�ö�Ĵ���
		{

			j = 1;

			// �޸Ĳ�����Ԫ
			if (j)
			{
				// ת����ַ ZCL 2015.7.11
				if (Lw_Com4RegAddr < STM32_RPAR_ADDRESS)
				{
					p_wTarget = Pw_ParLst;						   // �趨STM32 PAR��
					Lw_Com4RegAddr = Lw_Com4RegAddr - DNB_ADDRESS; //  2023.12.16 YLS
				}
				else if (Lw_Com4RegAddr < DSP_WRPAR_ADDRESS)
				{
					p_wTarget = w_ParLst; // ��STM32 PAR��
					Lw_Com4RegAddr = Lw_Com4RegAddr - STM32_RPAR_ADDRESS;
				}
				else if (Lw_Com4RegAddr < DSP_RPAR_ADDRESS) // ԭ�����޸Ĳ�����־�����ٱ���
				{
					p_wTarget = Pw_dspParLst; // �趨DSP PAR��
					Lw_Com4RegAddr = Lw_Com4RegAddr - DSP_WRPAR_ADDRESS;
					// ��ʾҪ�޸�DSP��������Ҫ��COM1��ת�����ָ��  ZCL 2015.7.11
					if (!F_ModDspPar)
						F_ModDspPar = 1;
					else // ԭ�����޸Ĳ�����־��������˴ε��޸�ָ�
					{
						B_Com4Cmd16 = 0;
						B_Com4Cmd06 = 0;
					}
				}
				/* 			else
							{
								p_wTarget=w_dspParLst;			// ��DSP PAR��
								Lw_Com4RegAddr=Lw_Com4RegAddr-DSP_RPAR_ADDRESS;
							}				 */

				if (B_Com4Cmd06) // Ԥ�õ���
				{
					m = Rcv4Buffer[4];
					m = (m << 8) + Rcv4Buffer[5];
					*(p_wTarget + Lw_Com4RegAddr) = m;

					// ��ΪҪת����DSP���ѵ�ַ��ֵ���������� ZCL  2015.7.11
					if (F_ModDspPar)
					{
						w_ModDspParAdd = Lw_Com4RegAddr;
						w_ModDspParValue = m;
					}
				}

				else if (B_Com4Cmd16) // Ԥ�ö��
				{
					if (Rcv4Buffer[5] < 90)
					{
						p_bGen = (u8 *)Rcv4Buffer;
						for (k = 0; k < Rcv4Buffer[5]; k++) // Rcv4Buffer[5]=����
						{
							m = *(p_bGen + 7 + k * 2);
							n = *(p_bGen + 7 + k * 2 + 1);
							m = (m << 8) + n;
							*(p_wTarget + Lw_Com4RegAddr + k) = m;
						}

						// ��ΪҪת����DSP���ѵ�ַ��ֵ���������� ZCL  2015.7.11
						// ������ʱֻ�ʺ�Ԥ��һ��DSP����  ZCL  2015.7.11
						if (F_ModDspPar)
						{
							w_ModDspParAdd = Lw_Com4RegAddr;
							w_ModDspParValue = m;
						}
					}
				}
			}

			// -------------------------
			// ��������
			// 2015.7.11 �����жϣ���Ϊ�趨DSP����ʱ�����ԭ���Ѿ����趨�����У�������
			if (B_Com4Cmd16 || B_Com4Cmd06)
			{
				Txd4Buffer[0] = 2;			   // �豸�ӵ�ַ
				Txd4Buffer[1] = Rcv4Buffer[1]; // ������
				Txd4Buffer[2] = Rcv4Buffer[2]; // ��ʼ��ַ��λ�ֽ�
				Txd4Buffer[3] = Rcv4Buffer[3]; // ��ʼ��ַ��λ�ֽ�
				Txd4Buffer[4] = Rcv4Buffer[4]; // �Ĵ���������λ
				Txd4Buffer[5] = Rcv4Buffer[5]; // �Ĵ���������λ
				if (j == 0)					   // ������ܱ�����Ԥ�ã�����FFFF zcl
				{
					Txd4Buffer[4] = 0xff; // �Ĵ���������λ��Ԥ������
					Txd4Buffer[5] = 0xff; // �Ĵ���������λ��Ԥ������
				}
				Lw_Txd4ChkSum = CRC16((u8 *)Txd4Buffer, 6);
				Txd4Buffer[6] = Lw_Txd4ChkSum >> 8; // /256
				Txd4Buffer[7] = Lw_Txd4ChkSum;		// ��λ�ֽ�
				Cw_Txd4Max = 8;
				B_Com4Cmd16 = 0;
				B_Com4Cmd06 = 0;
				Cw_Txd4 = 0;
				// RS485_CON=1;
				// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
				// USART_ITConfig(UART4, USART_IT_TXE, ENABLE);				// ��ʼ����.
				USART_SendData(UART4, Txd4Buffer[Cw_Txd4++]);
				USART_ITConfig(UART4, USART_IT_TC, ENABLE); // ��ʼ����.
			}
		} // 06��16Ԥ�üĴ��� ����
	}
	else // ��Ƶ���
	{
		//
		if (B_Com4Cmd03) // ��ȡ���ּĴ���
		{
			Txd4Buffer[0] = 2;				   // �豸�ӵ�ַPw_EquipmentNo
			Txd4Buffer[1] = Rcv4Buffer[1];	   // ������
			Txd4Buffer[2] = Rcv4Buffer[5] * 2; // Rcv4Buffer[5]=���� ��

			// ת����ַ ZCL 2015.7.11
			if (Lw_Com4RegAddr < STM32_RPAR_ADDRESS)
			{
				p_wRead = Pw_ParLst; // �趨STM32 PAR��
				Lw_Com4RegAddr = Lw_Com4RegAddr - STM32_WRPAR_ADDRESS_BPDJ;
			}
			else if (Lw_Com4RegAddr < DSP_WRPAR_ADDRESS)
			{
				p_wRead = w_ParLst; // ��STM32 PAR��
				Lw_Com4RegAddr = Lw_Com4RegAddr - STM32_RPAR_ADDRESS;
			}
			else if (Lw_Com4RegAddr < DSP_RPAR_ADDRESS)
			{
				p_wRead = Pw_dspParLst; // �趨DSP PAR��
				Lw_Com4RegAddr = Lw_Com4RegAddr - DSP_WRPAR_ADDRESS;
			}
			else
			{
				p_wRead = w_dspParLst; // ��DSP PAR��
				Lw_Com4RegAddr = Lw_Com4RegAddr - DSP_RPAR_ADDRESS;
			}

			p_bMove = (u8 *)Txd4Buffer;
			for (k = 0; k < Rcv4Buffer[5]; k++) // ����ѯ����
			{
				m = *(p_wRead + Lw_Com4RegAddr + k);
				*(p_bMove + 3 + k * 2) = m >> 8;
				*(p_bMove + 3 + k * 2 + 1) = m;
			}
			Lw_Txd4ChkSum = CRC16((u8 *)Txd4Buffer, Txd4Buffer[2] + 3);
			Txd4Buffer[Txd4Buffer[2] + 3] = Lw_Txd4ChkSum >> 8; // /256
			Txd4Buffer[Txd4Buffer[2] + 4] = Lw_Txd4ChkSum;		// ��λ�ֽ�
			Cw_Txd4Max = Txd4Buffer[2] + 5;
			//
			//
			B_Com4Cmd03 = 0;
			Cw_Txd4 = 0;
			// RS485_CON=1;
			// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
			// USART_ITConfig(UART4, USART_IT_TXE, ENABLE);				// ��ʼ����.
			USART_SendData(UART4, Txd4Buffer[Cw_Txd4++]);
			USART_ITConfig(UART4, USART_IT_TC, ENABLE); // ��ʼ����.
		}
		//
		else if (B_Com4Cmd16 || B_Com4Cmd06) // 16Ԥ�ö�Ĵ���
		{

			j = 1;

			// �޸Ĳ�����Ԫ
			if (j)
			{
				// ת����ַ ZCL 2015.7.11
				if (Lw_Com4RegAddr < STM32_RPAR_ADDRESS)
				{
					p_wTarget = Pw_ParLst; // �趨STM32 PAR��
					Lw_Com4RegAddr = Lw_Com4RegAddr - STM32_WRPAR_ADDRESS_BPDJ;
				}
				else if (Lw_Com4RegAddr < DSP_WRPAR_ADDRESS)
				{
					p_wTarget = w_ParLst; // ��STM32 PAR��
					Lw_Com4RegAddr = Lw_Com4RegAddr - STM32_RPAR_ADDRESS;
				}
				else if (Lw_Com4RegAddr < DSP_RPAR_ADDRESS) // ԭ�����޸Ĳ�����־�����ٱ���
				{
					p_wTarget = Pw_dspParLst; // �趨DSP PAR��
					Lw_Com4RegAddr = Lw_Com4RegAddr - DSP_WRPAR_ADDRESS;
					// ��ʾҪ�޸�DSP��������Ҫ��COM1��ת�����ָ��  ZCL 2015.7.11
					if (!F_ModDspPar)
						F_ModDspPar = 1;
					else // ԭ�����޸Ĳ�����־��������˴ε��޸�ָ�
					{
						B_Com4Cmd16 = 0;
						B_Com4Cmd06 = 0;
					}
				}
				/* 			else
							{
								p_wTarget=w_dspParLst;			// ��DSP PAR��
								Lw_Com4RegAddr=Lw_Com4RegAddr-DSP_RPAR_ADDRESS;
							}				 */

				if (B_Com4Cmd06) // Ԥ�õ���
				{
					m = Rcv4Buffer[4];
					m = (m << 8) + Rcv4Buffer[5];
					*(p_wTarget + Lw_Com4RegAddr) = m;

					// ��ΪҪת����DSP���ѵ�ַ��ֵ���������� ZCL  2015.7.11
					if (F_ModDspPar)
					{
						w_ModDspParAdd = Lw_Com4RegAddr;
						w_ModDspParValue = m;
					}
				}

				else if (B_Com4Cmd16) // Ԥ�ö��
				{
					if (Rcv4Buffer[5] < 90)
					{
						p_bGen = (u8 *)Rcv4Buffer;
						for (k = 0; k < Rcv4Buffer[5]; k++) // Rcv4Buffer[5]=����
						{
							m = *(p_bGen + 7 + k * 2);
							n = *(p_bGen + 7 + k * 2 + 1);
							m = (m << 8) + n;
							*(p_wTarget + Lw_Com4RegAddr + k) = m;
						}

						// ��ΪҪת����DSP���ѵ�ַ��ֵ���������� ZCL  2015.7.11
						// ������ʱֻ�ʺ�Ԥ��һ��DSP����  ZCL  2015.7.11
						if (F_ModDspPar)
						{
							w_ModDspParAdd = Lw_Com4RegAddr;
							w_ModDspParValue = m;
						}
					}
				}
			}

			// -------------------------
			// ��������
			// 2015.7.11 �����жϣ���Ϊ�趨DSP����ʱ�����ԭ���Ѿ����趨�����У�������
			if (B_Com4Cmd16 || B_Com4Cmd06)
			{
				Txd4Buffer[0] = 2;			   // �豸�ӵ�ַ
				Txd4Buffer[1] = Rcv4Buffer[1]; // ������
				Txd4Buffer[2] = Rcv4Buffer[2]; // ��ʼ��ַ��λ�ֽ�
				Txd4Buffer[3] = Rcv4Buffer[3]; // ��ʼ��ַ��λ�ֽ�
				Txd4Buffer[4] = Rcv4Buffer[4]; // �Ĵ���������λ
				Txd4Buffer[5] = Rcv4Buffer[5]; // �Ĵ���������λ
				if (j == 0)					   // ������ܱ�����Ԥ�ã�����FFFF zcl
				{
					Txd4Buffer[4] = 0xff; // �Ĵ���������λ��Ԥ������
					Txd4Buffer[5] = 0xff; // �Ĵ���������λ��Ԥ������
				}
				Lw_Txd4ChkSum = CRC16((u8 *)Txd4Buffer, 6);
				Txd4Buffer[6] = Lw_Txd4ChkSum >> 8; // /256
				Txd4Buffer[7] = Lw_Txd4ChkSum;		// ��λ�ֽ�
				Cw_Txd4Max = 8;
				B_Com4Cmd16 = 0;
				B_Com4Cmd06 = 0;
				Cw_Txd4 = 0;
				// RS485_CON=1;
				// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
				// USART_ITConfig(UART4, USART_IT_TXE, ENABLE);				// ��ʼ����.
				USART_SendData(UART4, Txd4Buffer[Cw_Txd4++]);
				USART_ITConfig(UART4, USART_IT_TC, ENABLE); // ��ʼ����.
			}
		} // 06��16Ԥ�üĴ��� ����
	}
}

// ��Ϊ��ģ��ʱ����
void Com4_MasterSend(void) // ����2�����ͳ���
{
}

// ������
u16 CalcCheck(u8 *Bytes, u16 len)
{
	u16 i, result;
	for (result = Bytes[1], i = 2; i < len; i++)
	{
		result ^= Bytes[i];
	}
	return result;
}

/* BK:
							if( strstr(Rcv4Buffer,",S,")!=NULL )
							{
								Gps_ptr=strstr(Rcv4Buffer,",S,");
								w_ScrGpsLat= (*(Gps_ptr-9)-0x30)*1000 +  (*(Gps_ptr-8)-0x30)*100 + (*(Gps_ptr-7)-0x30)*10 + (*(Gps_ptr-6)-0x30);
								w_ScrGpsLatFen= (*(Gps_ptr-4)-0x30)*1000 +  (*(Gps_ptr-3)-0x30)*100 + (*(Gps_ptr-2)-0x30)*10 + (*(Gps_ptr-1)-0x30);
							}
							else
							{
								w_ScrGpsLat=0;
								w_ScrGpsLatFen=0;
							}
							//5.2 ���ȼ������
							//Lon dddmm.mmmm ���ȣ�ǰ 3 �ַ���ʾ�ȣ�������ַ���ʾ��
							if( strstr(Rcv4Buffer,",E,")!=NULL )
							{
								Gps_ptr=strstr(Rcv4Buffer,",E,");
								w_ScrGpsLon= (*(Gps_ptr-10)-0x30)*10000 +  (*(Gps_ptr-9)-0x30)*1000 + (*(Gps_ptr-8)-0x30)*100 + (*(Gps_ptr-7)-0x30)*10 + (*(Gps_ptr-6)-0x30);
								w_ScrGpsLonFen= (*(Gps_ptr-4)-0x30)*1000 +  (*(Gps_ptr-3)-0x30)*100 + (*(Gps_ptr-2)-0x30)*10 + (*(Gps_ptr-1)-0x30);
							}
							else
							{
								w_ScrGpsLon=0;
								w_ScrGpsLonFen=0;
							}
							i=1; */

void Judge_Reset_GPS(void)
{
	// ǿ�ƶ�ʱ����
	if (F_Reset_GPS == 0 && T_Reset_GPS != SClkSecond)
	{
		T_Reset_GPS = SClkSecond;
		C_Reset_GPS++;
		if (C_Reset_GPS >= 30) // ����4����60��û�յ�GPS���������ݣ�����������־λ
		{
			C_Reset_GPS = 0;
			F_Reset_GPS = 1; // ��������־λ
			C_PowerOnOfft_GPS = 0;
		}
	}
}

// ����GPSģ��
void PowerOnOff_GPS(void)
{
	if (F_Reset_GPS)
	{
		if (T_PowerOnOff_GPS != SClkSecond)
		{
			T_PowerOnOff_GPS = SClkSecond;
			C_PowerOnOfft_GPS++;
			if (C_PowerOnOfft_GPS < 3) // ����4����60��û�յ�GPS���������ݣ�����������־λ
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_1); // ZCL 2019.6.25 �ر�GPS
			}
			else
			{
				C_PowerOnOfft_GPS = 0;
				F_Reset_GPS = 0;				 // ��������־λ
				GPIO_SetBits(GPIOC, GPIO_Pin_1); // ZCL 2018.10.19 ��GPS
			}
		}
	}
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
