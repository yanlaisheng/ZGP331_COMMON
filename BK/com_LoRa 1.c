/**
  ******************************************************************************
  * @file    com_LoRa.c
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.20
  * @date    2018-12-14
  * @brief   LoRa USE SPI3
	******************************************************************************
	* @note	2015.04.13

	*/

/* Includes ------------------------------------------------------------------*/
#include "com_LoRa.h"
#include "GlobalV_Extern.h" // ȫ�ֱ�������
#include "GlobalConst.h"
#include <stdio.h> //���ϴ˾������printf

#include "stm32f10x.h"
#include "com1_232.h"
#include "com2_232.h"
#include "com3_232.h" //ZCL 2018.10.18
#include "com4_232.h" //ZCL 2018.10.18
#include "DoWith.h"
#include "LCD.h"
#include "Key_Menu.h"

#include "LcdGraphic.H"

// #include "sys_config.h"			//ZCL 2018.11.12
#include "spi.h" //ZCL 2018.11.12		sx12xxEiger�ļ�����

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
// ZCL 2018.11.12
#define BUFFER_SIZE 100 // Define the payload size here

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// ZCL 2018.11.12
tRadioDriver *Radio = NULL;
const uint8_t MY_TEST_Msg[] = "LoRa_SX1278_SANLI";
static uint16_t BufferSize = BUFFER_SIZE; // RF buffer size

uint8_t LoRaTxBuf[BUFFER_SIZE]; // ZCL 2018.11.12
uint8_t LoRaRxBuf[BUFFER_SIZE]; // RF buffer

u8 B_LoRaSendTest;
u8 T_LoRaSendTest;
u16 C_LoRaSendTest;

u8 T_LoRaRcv; // ZCL 2018.11.13  LoRaRcv
u16 C_LoRaRcv;
u8 B_LoRaRcv;

u16 Cw_LoRaRx;	   // ���ռ�����//
u16 Cw_LoRaTx;	   // ���ͼ�����//
u16 Cw_BakLoRaRx;  // ���ռ�����//
u16 Cw_LoRaTxMax;  // �ж��ٸ��ַ���Ҫ����//
u16 Lw_LoRaChkSum; // ����У��ͣ�lo,hi ��λ//
// u16		Lw_LoRaRegAddr;						// ����1�Ĵ�����ַ
//
u8 B_LoRaCmd03;
u8 B_LoRaCmd16;
u8 B_LoRaCmd01;
u8 B_LoRaCmd06;
u8 B_LoRaCmd73;

//------------------------2016.4.2
u8 S_LoRaMSend; // ����

u8 T_LoRaCountNormalNext;	  // ��ʱ��
u16 C_LoRaCountNormalNext;	  // ������
u8 T_LoRaCount1OvertimeNext;  // ��ʱ��
u16 C_LoRaCount1OvertimeNext; // ������
u8 T_LoRaCount1Err;			  // ��ʱ��
u16 C_LoRaCount1Err;		  // ������
u8 T_LoRaCount2Err;			  // ��ʱ��
u16 C_LoRaCount2Err;		  // ������
u8 T_LoRaCount3Err;			  // ��ʱ��
u16 C_LoRaCount3Err;		  // ������
u8 T_LoRaCount4Err;			  // ��ʱ��
u16 C_LoRaCount4Err;		  // ������

u8 B_LoRaErrWithModule1; // ��־ ����ģ��1ͨѶʧ��
u8 B_LoRaErrWithModule2; // ��־ ����ģ��2ͨѶʧ��
u8 B_LoRaErrWithModule3; // ��־ ����ģ��1ͨѶʧ��
u8 B_LoRaErrWithModule4; // ��־ ����ģ��2ͨѶʧ��

u8 F_LoRaRCVCommandOk;
u8 F_LoRaSendNext;

u8 T_LoRaNoRcvMasterCMD;  // ��ʱ��
u16 C_LoRaNoRcvMasterCMD; // ������
u8 F_LoRaNoRcvMasterCMD;

u8 T_LoRaSendOverTime; // ZCL 2019.1.22 Lora���ͳ�ʱ
u16 C_LoRaSendOverTime;

/* Private variables extern --------------------------------------------------*/
extern u16 C_ForceSavPar; // ǿ�Ʊ������������

extern uchar Txd1Buffer[]; // UART���ͻ�����
extern uint Cw_Txd1;	   // ���ͼ�����//
extern uint Cw_Txd1Max;	   // �ж��ٸ��ַ���Ҫ����//

extern uchar Txd2Buffer[]; // UART���ͻ�����
extern uint Cw_Txd2;	   // ���ͼ�����//
extern uint Cw_Txd2Max;	   // �ж��ٸ��ַ���Ҫ����//
extern u16 Cw_BakRcv2;	   // ���ռ���������

extern uchar Txd3Buffer[]; // UART���ͻ�����
extern uint Cw_Txd3;	   // ���ͼ�����//
extern uint Cw_Txd3Max;	   // �ж��ٸ��ַ���Ҫ����//

extern uchar Txd4Buffer[]; // UART���ͻ�����
extern uint Cw_Txd4;	   // ���ͼ�����//
extern uint Cw_Txd4Max;	   // �ж��ٸ��ַ���Ҫ����//

/* Private function prototypes -----------------------------------------------*/
void GPIO_LoRa_Configuration(void); // GPIO����
void LoRa_config(void);
u16 CRC16(u8 *pCrcData, u8 CrcDataLen);
void Delay_MS(vu16 nCount);
u16 Address(u16 *p, u16 Area); // ���Ե�ַ

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
 * Function Name  : LoRa_Init
 * Description    : Initializes the peripherals used by the Usart driver.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void LoRa_Init(void)
{
	B_LoRaSendTest = 0;
	B_LoRaRcv = 0;

	// ZCL 2018.11.12 ����
	LoRaTxBuf[0] = 'Z';
	LoRaTxBuf[1] = 'C';
	LoRaTxBuf[2] = 'L';
	LoRaTxBuf[3] = 'T';
	LoRaTxBuf[4] = 'E';
	LoRaTxBuf[5] = 'S';
	LoRaTxBuf[6] = 'T';
	LoRaTxBuf[7] = '2';
	LoRaTxBuf[8] = '0';
	LoRaTxBuf[9] = '1';
	LoRaTxBuf[10] = '8';
	LoRaTxBuf[11] = '1';
	LoRaTxBuf[12] = '1';
	LoRaTxBuf[13] = '1';
	LoRaTxBuf[14] = '2';
	LoRaTxBuf[15] = 0;
	LoRaTxBuf[16] = 1;
	LoRaTxBuf[17] = 2;
	LoRaTxBuf[18] = 3;
	LoRaTxBuf[19] = 4;
	LoRaTxBuf[20] = 5;
	LoRaTxBuf[21] = 6;
	LoRaTxBuf[22] = 7;
	LoRaTxBuf[23] = 8;
	LoRaTxBuf[24] = 9;
	LoRaTxBuf[25] = 10;
	LoRaTxBuf[26] = 11;
	LoRaTxBuf[27] = 12;
	LoRaTxBuf[28] = 13;
	LoRaTxBuf[29] = 14;

	F_LoRaSendNext = 1;
}

// ZCL 2019.1.24 ��ע
/* 		F_LoRaSendNext=1;//����
		F_LoRaSendNext=2;//�ȴ��������
		F_LoRaSendNext=3;//�������״̬ */

// ���մ������ У�����
void LoRa_RcvProcess(void)
{
	u8 k, s, i = 0; // ��ʱ����
	u16 j;
	u16 *p_wRead;

	// 1. LORA�������ݴ洢  LORA���� ���� LORA�ӻ�
	if (B_LoRaRcv == 0 && B_LoRaSendTest == 1) // F_LoRaSendNext==3
	{
		while (Radio->Process() == RF_RX_DONE)
		{
			Radio->GetRxPacket(LoRaRxBuf, (uint16_t *)&BufferSize);
			// ZCL 2018.11.9 RX��
			// if( strncmp( ( const char* )LoRaRxBuf, ( const char* )MY_TEST_Msg, 18 ) == 0 )
			//{			//ZCL 2018.11.9 RX��

			// ZCL 2018.12.14 Pw_LoRaSet .1λ =1   LORA�����COM2 (͸��ʹ��)
			if (Pw_LoRaSet & 0x0002)
			{
				// ��LoRa���յ������ݷ��͵�����2		ZCL 2018.11.13 OK
				for (i = 0; i < BufferSize; i++)
				{
					Txd2Buffer[i] = LoRaRxBuf[i];
				}
				Cw_Txd2Max = BufferSize;
				Cw_Txd2 = 0;
				// RS485_CON=1;
				// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
				USART_ITConfig(USART2, USART_IT_TC, ENABLE); // ��ʼ����.
				USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]);
			}

			// else
			if (1)
			{
				// ��LoRa���յ������ݴ浽LoRaRxBuf�����д���
				for (i = 0; i < BufferSize; i++)
				{
					LoRaRxBuf[i] = LoRaRxBuf[i];
				}
				Cw_LoRaRx = BufferSize;
				Cw_BakLoRaRx = Cw_LoRaRx; // �� Cw_LoRaRx ����

				// ZCL 2018.12.14 B_LoRaRcv=1,���д���
				if (Cw_BakLoRaRx > 0)
					B_LoRaRcv = 1;
			}

			LED0_TOGGLE;

			/*
							//�͵�����1
							for(i=0;i < BufferSize;i++)
							{
								USART_putchar(USART1,LoRaRxBuf[i]);
							}
							printf("\n");
							LED0_TOGGLE;
							//���LoRaRxBuf
							for(i=0;i<BUFFER_SIZE;i++)
								LoRaRxBuf[i] = 0;

						}			//ZCL 2018.11.9 RX��

						*/

			// Soft_delay_ms(35);		//ZCL 2018.11.10  2,5,7����   8,20,15��1�Σ�25ż�����Σ�35��30,50,200����
			// LCD_DLY_ms(35);				//ZCL 2018.11.12
		}
	}

	// 2. LORA���մ洢���ݵĴ���  LORA���� ���� LORA�ӻ�
	if (B_LoRaRcv == 1) // ���մ������
	{
		B_LoRaRcv = 2;

		// LORA�ӻ� ���յ����� ���ӵ�ַ��⣭���յ�����λ����ѯָ�
		// ZCLע����ʱ��LORA�ǵ����ӻ��������ı�����λ����ѯ�� =2�����ӻ�����3.16.1.6ָ���
		if (LoRaRxBuf[0] == Pw_LoRaEquipmentNo)
		{
			j = CRC16(LoRaRxBuf, Cw_BakLoRaRx - 2); // CRC У��
			k = j >> 8;
			s = j;
			if (k == LoRaRxBuf[Cw_BakLoRaRx - 2] && s == LoRaRxBuf[Cw_BakLoRaRx - 1])
			{						   // CRCУ����ȷ
				if (LoRaRxBuf[1] == 3) // 03��ȡ���ּĴ���
				{
					B_LoRaCmd03 = 1;
				}
				else if (LoRaRxBuf[1] == 16) // 16Ԥ�ö�Ĵ���
				{
					C_ForceSavPar = 0; // ǿ�Ʊ������������=0
					B_LoRaCmd16 = 1;
				}
				else if (LoRaRxBuf[1] == 1) // 01��ȡ��Ȧ״̬
				{
					B_LoRaCmd01 = 1;
				}
				else if (LoRaRxBuf[1] == 6) // 06Ԥ�õ��Ĵ���
				{
					C_ForceSavPar = 0; // ǿ�Ʊ������������=0
					B_LoRaCmd06 = 1;
				}
				// 2016.4.5
				//  ����Ϊ��ģ����ӻ� (��ģ��)�ĳ��򣺣���ģ�鷢�ͣ�73��������ָ�����������������
				else if (LoRaRxBuf[1] == 73) // 73��������ָ��
				{
					B_LoRaCmd73 = 1;
					//
					T_LoRaNoRcvMasterCMD = 100;
					C_LoRaNoRcvMasterCMD = 0;
					F_LoRaNoRcvMasterCMD = 0;
				}
				else
					i = 1;

				// ZCL 2018.12.14   �ӻ��ķ��أ����յ���������ķ��أ�
				LoRa_SlaveSend();
			}
			else
				i = 2;
		}

		// LORA���� ���յ����� ��Ϊ��ģ��(�������յ���) 2018.12.14
		// LORA��ģ��1����-->������������			=3.4.5.6 ��չ�ӻ���ֻ��73ָ�
		else if (LoRaTxBuf[0] == Pw_LoRaModule1Add) // �ӵ�ַ���
		{
			j = CRC16(LoRaTxBuf, Cw_BakLoRaRx - 2); // CRC У��
			k = j >> 8;								// ZCL ע��
			s = j;
			if (k == LoRaTxBuf[Cw_BakLoRaRx - 2] && s == LoRaTxBuf[Cw_BakLoRaRx - 1])
			{							// CRCУ����ȷ
				if (LoRaTxBuf[1] == 73) // 73 ����ģ��ͨѶ
				{
					/*ZCL 2017.6.18 �л�ҳ��ʱ���������������淢��ǰ��һҳ�����ݷ���������ʱ�����͵���һҳ�ϣ�
							��������������ĳɷ��ص����ݣ�����ǰ��Txd1Buffer�ķ��͵�ַ����ֵ�����ں��ˣ� OK*/

					// 1. ת����ַ ZCL 2018.12.14
					j = LoRaTxBuf[2];
					Lw_Reg = (j << 8) + LoRaTxBuf[3];

					p_wRead = AddressChange();

					// 2. ���յ����ݴ洢 ZCL 2018.12.14
					for (i = 0; i < LoRaRxBuf[2] / 2; i++)
					{
						j = LoRaRxBuf[3 + i * 2];
						*(p_wRead + i) = (j << 8) + LoRaRxBuf[4 + i * 2];
					}
					//
					F_LoRaRCVCommandOk = 1;
					T_LoRaCount1Err = 100;
					C_LoRaCount1Err = 0;
					B_LoRaErrWithModule1 = 0;
				}
			}
		}

		//  LORA���� ��Ϊ��ģ��(�������յ���) 2018.12.14
		// ��ģ��2����-->������������
		else if (LoRaTxBuf[0] == Pw_LoRaModule2Add) // �ӵ�ַ���
		{
			j = CRC16(LoRaTxBuf, Cw_BakLoRaRx - 2); // CRC У��
			k = j >> 8;								// ZCL ע��
			s = j;
			if (k == LoRaTxBuf[Cw_BakLoRaRx - 2] && s == LoRaTxBuf[Cw_BakLoRaRx - 1])
			{							// CRCУ����ȷ
				if (LoRaTxBuf[1] == 73) // 73 ����ģ��ͨѶ
				{
					// 1. ת����ַ ZCL 2018.12.14
					j = LoRaTxBuf[2];
					Lw_Reg = (j << 8) + LoRaTxBuf[3];

					p_wRead = AddressChange();

					// 2. ���յ����ݴ洢 ZCL 2018.12.14
					for (i = 0; i < LoRaRxBuf[2] / 2; i++)
					{
						j = LoRaRxBuf[3 + i * 2];
						*(p_wRead + i) = (j << 8) + LoRaRxBuf[4 + i * 2]; // ZCL123 ����ĵ�ַ����Ҫ�ĸģ�
					}

					F_LoRaRCVCommandOk = 1;
					//

					T_LoRaCount2Err = 100;
					C_LoRaCount2Err = 0;
					B_LoRaErrWithModule2 = 0;
				}
			}
		}

		//  LORA���� ��Ϊ��ģ��(�������յ���) 2018.12.14
		// ��ģ��3����-->������������
		else if (LoRaTxBuf[0] == Pw_LoRaModule3Add) // �ӵ�ַ���
		{
			j = CRC16(LoRaTxBuf, Cw_BakLoRaRx - 2); // CRC У��
			k = j >> 8;								// ZCL ע��
			s = j;
			if (k == LoRaTxBuf[Cw_BakLoRaRx - 2] && s == LoRaTxBuf[Cw_BakLoRaRx - 1])
			{							// CRCУ����ȷ
				if (LoRaTxBuf[1] == 73) // 73 ����ģ��ͨѶ
				{
					// 1. ת����ַ ZCL 2018.12.14
					j = LoRaTxBuf[2];
					Lw_Reg = (j << 8) + LoRaTxBuf[3];

					p_wRead = AddressChange();

					// 2. ���յ����ݴ洢 ZCL 2018.12.14
					for (i = 0; i < LoRaRxBuf[2] / 2; i++)
					{
						j = LoRaRxBuf[3 + i * 2];
						*(p_wRead + i) = (j << 8) + LoRaRxBuf[4 + i * 2]; // ZCL123 ����ĵ�ַ����Ҫ�ĸģ�
					}

					F_LoRaRCVCommandOk = 1;
					//

					T_LoRaCount3Err = 100;
					C_LoRaCount3Err = 0;
					B_LoRaErrWithModule3 = 0;
				}
			}
		}

		//  LORA���� ��Ϊ��ģ��(�������յ���) 2018.12.14
		// ��ģ��4����-->������������
		else if (LoRaTxBuf[0] == Pw_LoRaModule4Add) // �ӵ�ַ���
		{
			j = CRC16(LoRaTxBuf, Cw_BakLoRaRx - 2); // CRC У��
			k = j >> 8;								// ZCL ע��
			s = j;
			if (k == LoRaTxBuf[Cw_BakLoRaRx - 2] && s == LoRaTxBuf[Cw_BakLoRaRx - 1])
			{							// CRCУ����ȷ
				if (LoRaTxBuf[1] == 73) // 73 ����ģ��ͨѶ
				{
					// 1. ת����ַ ZCL 2018.12.14
					j = LoRaTxBuf[2];
					Lw_Reg = (j << 8) + LoRaTxBuf[3];

					p_wRead = AddressChange();

					// 2. ���յ����ݴ洢 ZCL 2018.12.14
					for (i = 0; i < LoRaRxBuf[2] / 2; i++)
					{
						j = LoRaRxBuf[3 + i * 2];
						*(p_wRead + i) = (j << 8) + LoRaRxBuf[4 + i * 2]; // ZCL123 ����ĵ�ַ����Ҫ�ĸģ�
					}

					F_LoRaRCVCommandOk = 1;
					//

					T_LoRaCount4Err = 100;
					C_LoRaCount4Err = 0;
					B_LoRaErrWithModule4 = 0;
				}
			}
		}

		else
			i = 3;
	}

	// 3. ��ʱ���ٴν���
	if (B_LoRaRcv == 2)
	{
		if (T_LoRaRcv != SClk1Ms)
		{
			T_LoRaRcv = SClk1Ms; //
			C_LoRaRcv++;
			if (C_LoRaRcv > 35)
			{
				T_LoRaRcv = 100;
				C_LoRaRcv = 0;
				//
				B_LoRaRcv = 0;

				// ZCL 2018.12.14 ���մ�����ɣ���������
				for (j = 0; j < Cw_BakLoRaRx; j++)
				{
					LoRaRxBuf[j] = 0;
				}
				Radio->StartRx(); // RFLR_STATE_RX_INIT

				Cw_LoRaRx = 0;
				Cw_BakLoRaRx = 0;
			}
		}
	}
}

void LoRa_SlaveSend(void) // ����2�ӻ�����
{
	u16 m, n;
	u8 j = 0, k;
	u16 *p_wRead;
	u8 *p_bMove;
	u8 *p_bGen;
	u16 *p_wTarget; // ָ��Ŀ���ַ�����xdata zcl

	// ZCL 2018.12.15
	if (B_LoRaCmd03) // ��ȡ���ּĴ���
	{
		LoRaTxBuf[0] = LoRaRxBuf[0];	 // �豸�ӵ�ַPw_EquipmentNo
		LoRaTxBuf[1] = LoRaRxBuf[1];	 // ������
		LoRaTxBuf[2] = LoRaRxBuf[5] * 2; // LoRaRxBuf[5]=���� ��

		// ZCL 2018.12.14 ������е�ַ�ϳ�
		j = LoRaRxBuf[2];
		Lw_Reg = (j << 8) + LoRaRxBuf[3];
		// ZCL 2018.12.14 ��ַת��
		p_wRead = AddressChange();

		p_bMove = LoRaTxBuf;
		for (k = 0; k < LoRaRxBuf[5]; k++) // ����ѯ����
		{
			m = *(p_wRead + Lw_Reg + k);
			*(p_bMove + 3 + k * 2) = m >> 8;
			*(p_bMove + 3 + k * 2 + 1) = m;
		}
		Lw_LoRaChkSum = CRC16(LoRaTxBuf, LoRaTxBuf[2] + 3);
		LoRaTxBuf[LoRaTxBuf[2] + 3] = Lw_LoRaChkSum >> 8; // /256
		LoRaTxBuf[LoRaTxBuf[2] + 4] = Lw_LoRaChkSum;	  // ��λ�ֽ�

		Cw_LoRaTxMax = LoRaTxBuf[2] + 5;
		B_LoRaCmd03 = 0;
		Cw_LoRaTx = 0;
		// ZCL 2018.12.14 ����
		Radio->SetTxPacket(LoRaTxBuf, Cw_LoRaTxMax); // RFLR_STATE_TX_INIT
	}

	else if (B_LoRaCmd16 || B_LoRaCmd06) // 16Ԥ�ö�Ĵ���
	{
		j = 1;

		// ZCL123 2018.12.14   ���ݲ�ȫ���������������� --------------------------------

		// ZCL 2018.12.14 ������е�ַ�ϳ�
		j = LoRaRxBuf[2];
		Lw_Reg = (j << 8) + LoRaRxBuf[3];
		// ZCL 2018.12.14 ��ַת��
		p_wRead = AddressChange();

		B_LoRaCmd16 = 0;
		B_LoRaCmd06 = 0;
		Cw_LoRaTx = 0;
		// ZCL 2018.12.14 ����
		Radio->SetTxPacket(LoRaTxBuf, Cw_LoRaTxMax); // RFLR_STATE_TX_INIT
	}

	else if (B_LoRaCmd73) // 73
	{
		LoRaTxBuf[0] = LoRaRxBuf[0];	 // �豸�ӵ�ַPw_EquipmentNo
		LoRaTxBuf[1] = LoRaRxBuf[1];	 // ������
		LoRaTxBuf[2] = LoRaRxBuf[5] * 2; // LoRaRxBuf[5]=���� ��

		// ZCL 2018.12.14 ������е�ַ�ϳ�
		j = LoRaRxBuf[2];
		Lw_Reg = (j << 8) + LoRaRxBuf[3];
		// ZCL 2018.12.14 ��ַת��
		p_wRead = AddressChange();

		p_bMove = LoRaTxBuf;
		for (k = 0; k < LoRaRxBuf[5]; k++) // ����ѯ����
		{
			// m=*(p_wRead+Lw_Reg+k);			//ZCL 2019.1.23 ����
			m = *(p_wRead + k);
			*(p_bMove + 3 + k * 2) = m >> 8;
			*(p_bMove + 3 + k * 2 + 1) = m;
		}
		Lw_LoRaChkSum = CRC16(LoRaTxBuf, LoRaTxBuf[2] + 3);
		LoRaTxBuf[LoRaTxBuf[2] + 3] = Lw_LoRaChkSum >> 8; // /256
		LoRaTxBuf[LoRaTxBuf[2] + 4] = Lw_LoRaChkSum;	  // ��λ�ֽ�

		Cw_LoRaTxMax = LoRaTxBuf[2] + 5;
		B_LoRaCmd73 = 0;
		Cw_LoRaTx = 0;
		// ZCL 2018.12.14 ����
		Radio->SetTxPacket(LoRaTxBuf, Cw_LoRaTxMax); // RFLR_STATE_TX_INIT
	}
}

// ��Ϊ��ģ��ʱ���� 2016.4.1
void LoRa_MasterSend(void) // ����2�����ͳ���
{
	u8 i = 0, j = 0, m = 0;

	if (Pw_LoRaModule1Add > 0)
		i++; // ͳ�Ʒ�������
	if (Pw_LoRaModule2Add > 0)
		i++;
	if (Pw_LoRaModule3Add > 0)
		i++;
	if (Pw_LoRaModule4Add > 0)
		i++;
	//
	if (i > 10) // ��������ģ����߱�Ƶ���������Ʋ��ܽ��뷢�ͳ���
	{
		if (F_LoRaSendNext == 1)
		{
			// 1. ���Ʒ��Ͳ���
			S_LoRaMSend++;
			if (S_LoRaMSend > 4)
				S_LoRaMSend = 1; // COM_UNIT_NUM

			// 2.����˳����
			if (Pw_LoRaModule1Add > 0 && S_LoRaMSend == 1)
			{
				LoRaTxBuf[0] = Pw_LoRaModule1Add; // �豸�ӵ�ַ
				j = 1;
			}
			else if (Pw_LoRaModule2Add > 0 && S_LoRaMSend == 2)
			{
				LoRaTxBuf[0] = Pw_LoRaModule2Add; // �豸�ӵ�ַ
				j = 2;
			}
			else if (Pw_LoRaModule3Add > 0 && S_LoRaMSend == 3)
			{
				LoRaTxBuf[0] = Pw_LoRaModule3Add; // �豸�ӵ�ַ
				j = 3;
			}
			else if (Pw_LoRaModule4Add > 0 && S_LoRaMSend == 4)
			{
				LoRaTxBuf[0] = Pw_LoRaModule4Add; // �豸�ӵ�ַ
				j = 4;
			}

			if (j > 0) // LORA ������ģ��
			{
				// LoRaTxBuf[0]=Pw_EquipmentNo+S_LoRaMSend;
				LoRaTxBuf[1] = 73;	 // ������ 73=0x49
				LoRaTxBuf[2] = 0x75; // ��ʼ��ַ����λ��  30016: w_dspParLst[16]	//.2 A�����
				LoRaTxBuf[3] = 0x40; // ��ʼ��ַ����λ��
				LoRaTxBuf[4] = 0x00; // ��ȡ��������λ��
				LoRaTxBuf[5] = 0x0A; // ��ȡ��������λ�� 10���������޸�����

				Lw_LoRaChkSum = CRC16(LoRaTxBuf, 6);
				LoRaTxBuf[6] = Lw_LoRaChkSum >> 8; // /256
				LoRaTxBuf[7] = Lw_LoRaChkSum;	   // ��λ�ֽ�
				Cw_LoRaTxMax = 8;
				Cw_LoRaTx = 0;

				F_LoRaSendNext = 2; // �����־

				// ZCL 2018.12.15 ����
				Radio->SetTxPacket(LoRaTxBuf, Cw_LoRaTxMax); // RFLR_STATE_TX_INIT
			}
		}

		// 3.��ʱ����		// ���ͽ���
		// �жϽ�����������ʱ������һ�� 6MS
		// ZCL 2018.12.15 �����ٶ�������ʱ�� 100MS
		if (F_LoRaRCVCommandOk) // ����������ʱ�󣬷�����һ��
		{
			if (T_LoRaCountNormalNext != SClk10Ms)
			{
				T_LoRaCountNormalNext = SClk10Ms; //
				C_LoRaCountNormalNext++;
				if (C_LoRaCountNormalNext > 10) //
				{
					F_LoRaRCVCommandOk = 0;
					T_LoRaCountNormalNext = 100;
					C_LoRaCountNormalNext = 0;
					T_LoRaCount1OvertimeNext = 100;
					C_LoRaCount1OvertimeNext = 0;
					F_LoRaSendNext = 1; //
				}
			}
		}

		// �����굫���Ͳ���ȷ��û��F_LoRaRCVCommandOk��־����ʱ������һ��

		// ZCL 2019.1.24 ��ע
		/* 		F_LoRaSendNext=1;//����
				F_LoRaSendNext=2;//�ȴ��������
				F_LoRaSendNext=3;//�������״̬ */
		// 2.2Sû�н��յ����ݣ����뷢��״̬��

		// 4.1 LORA���������꣬�������״̬
		// 4.2 LORA���ͣ���û�м�⵽RF_TX_DONE����ʱ�˳���F_LoRaSendNext=3;
		if (F_LoRaSendNext == 2)
		{
			// 4.1 LORA�����꣬�������״̬
			while (Radio->Process() == RF_TX_DONE)
			{
				F_LoRaSendNext = 3;

				LCD_DLY_ms(1); // ZCL 2018.11.12
				// ZCL 2018.11.13 ���������������
				Radio->StartRx(); // RFLR_STATE_RX_INIT
				// LCD_DLY_ms(100);				//ZCL123 2018.11.12

				T_LoRaSendOverTime = 100;
				C_LoRaSendOverTime = 0;
			}

			// 4.2 LORA����(F_LoRaSendNext==2)����û�м�⵽RF_TX_DONE����ʱ�˳���F_LoRaSendNext=3;
			// û��while(Radio->Process( ) == RF_TX_DONE )	��������ʱ
			if (T_LoRaSendOverTime != SClk10Ms)
			{
				T_LoRaSendOverTime = SClk10Ms; //
				C_LoRaSendOverTime++;
				if (C_LoRaSendOverTime > 400) //
				{
					T_LoRaSendOverTime = 100;
					C_LoRaSendOverTime = 0;

					F_LoRaSendNext = 3; // ������ʱ�� F_LoRaSendNext=3���˳���״̬����������

					LCD_DLY_ms(1); // ZCL 2018.11.12
					// ZCL 2018.11.13 ���������������
					Radio->StartRx(); // RFLR_STATE_RX_INIT
				}
			}
		}

		// ZCL 2019.1.24 ��ע
		/* 		F_LoRaSendNext=1;//����
				F_LoRaSendNext=2;//�ȴ��������
				F_LoRaSendNext=3;//�������״̬ */
		// 2.2Sû�н��յ����ݣ����뷢��״̬��
		// 4.3������ɣ���ʱ�����ٴη��͡�
		if (F_LoRaSendNext == 3)
		{
			if (T_LoRaCount1OvertimeNext != SClk10Ms)
			{
				T_LoRaCount1OvertimeNext = SClk10Ms; //
				C_LoRaCount1OvertimeNext++;
				if (C_LoRaCount1OvertimeNext > 220) //
				{
					T_LoRaCount1OvertimeNext = 100;
					C_LoRaCount1OvertimeNext = 0;
					F_LoRaSendNext = 1; // =1����
				}
			}
		}

		// B_LoRaSendEnd=S_LoRaMSend;
		//}

		//----------------------------------------------------------------
		// 6. �жϳ�ʱ
		// 6.1�жϳ�ʱ����ʱͨѶ���� 4000MS   ����ģ��1ͨѶʧ�� // 6000
		if (Pw_LoRaModule1Add > 0 && !B_LoRaErrWithModule1) // ��������м�ʱ��ʱ�䵽����
		{
			if (T_LoRaCount1Err != SClk10Ms)
			{
				T_LoRaCount1Err = SClk10Ms; //
				C_LoRaCount1Err++;
				if (C_LoRaCount1Err > 1000) //
				{
					T_LoRaCount1Err = 100;
					C_LoRaCount1Err = 0;
					B_LoRaErrWithModule1 = 1; //
				}
			}
		}
		else if (Pw_LoRaModule1Add == 0)
		{
			B_LoRaErrWithModule1 = 0; //
			C_LoRaCount1Err = 0;
		}

		//----------------------------------------------------------------
		// 6.2�жϳ�ʱ����ʱͨѶ���� 4000MS	����ģ��2ͨѶʧ��
		if (Pw_LoRaModule2Add > 0 && !B_LoRaErrWithModule2) // ��������м�ʱ��ʱ�䵽����
		{
			if (T_LoRaCount2Err != SClk10Ms)
			{
				T_LoRaCount2Err = SClk10Ms; //
				C_LoRaCount2Err++;
				if (C_LoRaCount2Err > 1000) //
				{
					T_LoRaCount2Err = 100;
					C_LoRaCount2Err = 0;
					B_LoRaErrWithModule2 = 1; //
				}
			}
		}
		else if (Pw_LoRaModule2Add == 0)
		{
			B_LoRaErrWithModule2 = 0; //
			C_LoRaCount2Err = 0;
		}

		//----------------------------------------------------------------
		// 6.3�жϳ�ʱ����ʱͨѶ���� 4000MS	����ģ��3ͨѶʧ��  ZCL 2018.12.15
		if (Pw_LoRaModule3Add > 0 && !B_LoRaErrWithModule3) // ��������м�ʱ��ʱ�䵽����
		{
			if (T_LoRaCount3Err != SClk10Ms)
			{
				T_LoRaCount3Err = SClk10Ms; //
				C_LoRaCount3Err++;
				if (C_LoRaCount3Err > 1000) //
				{
					T_LoRaCount3Err = 100;
					C_LoRaCount3Err = 0;
					B_LoRaErrWithModule3 = 1; //
				}
			}
		}
		else if (Pw_LoRaModule3Add == 0)
		{
			B_LoRaErrWithModule3 = 0; //
			C_LoRaCount3Err = 0;
		}

		//----------------------------------------------------------------
		// 6.4�жϳ�ʱ����ʱͨѶ���� 4000MS	����ģ��4ͨѶʧ��   ZCL 2018.12.15
		if (Pw_LoRaModule4Add > 0 && !B_LoRaErrWithModule4) // ��������м�ʱ��ʱ�䵽����
		{
			if (T_LoRaCount4Err != SClk10Ms)
			{
				T_LoRaCount4Err = SClk10Ms; //
				C_LoRaCount4Err++;
				if (C_LoRaCount4Err > 1000) //
				{
					T_LoRaCount4Err = 100;
					C_LoRaCount4Err = 0;
					B_LoRaErrWithModule4 = 1; //
				}
			}
		}
		else if (Pw_LoRaModule4Add == 0)
		{
			B_LoRaErrWithModule4 = 0; //
			C_LoRaCount4Err = 0;
		}
	}

	// ZCL123 2019.1.24 ���Դ��ڷ��͵�LORA
	if (B_LoRaSendTest == 2)
	{
		B_LoRaSendTest = 3;
		// ZCL 2018.11.12
		// Radio->SetTxPacket( MY_TEST_Msg, strlen(MY_TEST_Msg) );   //RFLR_STATE_TX_INIT
		Radio->SetTxPacket(LoRaTxBuf, 10); // RFLR_STATE_TX_INIT
	}

	// printf("������RF_LoRa_TX_OK�� \n");
	if (B_LoRaSendTest == 3)
	{
		while (Radio->Process() == RF_TX_DONE)
		{
			printf("RF_LoRa_TX_OK! ZCL1! CAN DEL \n");
			LED0_TOGGLE;
			// Soft_delay_ms(1000);	//ZCL 2018.11.12
			// LCD_DLY_ms(1000);				//ZCL 2018.11.12

			// ZCL 2018.11.13 B_LoRaSendTest=1,��ʱ����
			B_LoRaSendTest = 1;

			LCD_DLY_ms(1); // ZCL 2018.11.12 LCD_DLY_ms(10);
			// ZCL 2018.11.13 ���������������
			Radio->StartRx(); // RFLR_STATE_RX_INIT
			B_LoRaRcv = 1;
		}
	}
}

/* ZCL ˵���� 2018.12.20
	Pw_ScrLoRaTest1==10 ���Է���ģʽ
	Pw_ScrLoRaTest1==11 ���Խ���ģʽ
	Pw_ScrLoRaTest1==1	͸��ģʽ�� LoRa �� COM2
	COM2�и�ֵ�������յ����ݣ���B_LoRaSend=2��Ȼ���������LoRa����
	ע�ⷢ���꣬��Ҫ����ʱ�������ٴη��ͣ� ��Ϊ������Ҫ��ʱ�䣬����û�з����ꣿ
*/
void LoRaSendTest(void) // LoRa ���Ͳ���
{
	// u8	i;

	// ZCL 2018.11.12 LORA SX1278 ����
	// SX1278 TX	����
	if (Pw_ScrLoRaTest1 == 10) //=10 ���Է���ģʽ
	{
		if (B_LoRaSendTest == 0)
		{
			Radio->SetTxPacket(MY_TEST_Msg, 18);

			B_LoRaSendTest = 2; //=2,��������
		}

		if (B_LoRaSendTest == 1 && T_LoRaSendTest != SClk1Ms)
		{
			T_LoRaSendTest = SClk1Ms; //
			C_LoRaSendTest++;
			if (C_LoRaSendTest > 1000)
			{
				T_LoRaSendTest = 100;
				C_LoRaSendTest = 0;
				//
				B_LoRaSendTest = 2;
			}
		}

		if (B_LoRaSendTest == 2)
		{
			B_LoRaSendTest = 3;
			// ZCL 2018.11.12
			// Radio->SetTxPacket( MY_TEST_Msg, strlen(MY_TEST_Msg) );   //RFLR_STATE_TX_INIT
			Radio->SetTxPacket(LoRaTxBuf, 30); // RFLR_STATE_TX_INIT
		}

		// printf("������RF_LoRa_TX_OK�� \n");
		while (Radio->Process() == RF_TX_DONE && B_LoRaSendTest == 3)
		{
			printf("RF_LoRa_TX_OK! ZCL1! CAN DEL \n");
			LED0_TOGGLE;
			// Soft_delay_ms(1000);	//ZCL 2018.11.12
			// LCD_DLY_ms(1000);				//ZCL 2018.11.12

			// ZCL 2018.11.13 B_LoRaSendTest=1,��ʱ����
			B_LoRaSendTest = 1;
		}
	}

	//=11 ���ղ���
	else if (Pw_ScrLoRaTest1 == 11)
	{
		// 2. SX1278 RX����
		LoRaRcvTest();
	}

	//=1 ͸��ģʽ
	else if (Pw_ScrLoRaTest1 == 1)
	{
		// 1. SX1278 TX����		COM2�и�ֵ�������յ����ݣ���B_LoRaSend=2��Ȼ���������LoRa����
		if (B_LoRaSendTest == 2)
		{
			B_LoRaSendTest = 3;
			// ZCL 2018.11.12
			// Radio->SetTxPacket( MY_TEST_Msg, strlen(MY_TEST_Msg) );   //RFLR_STATE_TX_INIT
			Radio->SetTxPacket(LoRaTxBuf, Cw_BakRcv2); // RFLR_STATE_TX_INIT
		}

		// printf("������RF_LoRa_TX_OK�� \n");
		if (B_LoRaSendTest == 3)
		{
			while (Radio->Process() == RF_TX_DONE)
			{
				printf("RF_LoRa_TX_OK! ZCL1! CAN DEL \n");
				LED0_TOGGLE;
				// Soft_delay_ms(1000);	//ZCL 2018.11.12
				// LCD_DLY_ms(1000);				//ZCL 2018.11.12

				// ZCL 2018.11.13 B_LoRaSendTest=1,��ʱ����
				B_LoRaSendTest = 1;

				LCD_DLY_ms(1); // ZCL 2018.11.12 LCD_DLY_ms(10);
				// ZCL 2018.11.13 ���������������
				Radio->StartRx(); // RFLR_STATE_RX_INIT
				B_LoRaRcv = 1;
			}
		}

		else
			// 2. SX1278 RX����
			LoRaRcvTest();
	}
}

void LoRaRcvTest(void) // LoRa ���ղ���
{
	u8 i;
	// 2. SX1278 RX����
	// 2.1 ��ʱ
	if (B_LoRaRcv == 1 && T_LoRaRcv != SClk1Ms)
	{
		T_LoRaRcv = SClk1Ms; //
		C_LoRaRcv++;
		if (C_LoRaRcv > 1) // ZCL123 35
		{
			T_LoRaRcv = 100;
			C_LoRaRcv = 0;
			//
			B_LoRaRcv = 2;
		}
	}

	// 2.2 �������״̬
	if (B_LoRaRcv == 2)
	{
		B_LoRaRcv = 0;
		// Radio->StartRx( );
	}

	// 2.3 �жϽ����Ƿ����
	if (B_LoRaRcv == 0)
	{
		while (Radio->Process() == RF_RX_DONE)
		{
			Radio->GetRxPacket(LoRaRxBuf, (uint16_t *)&BufferSize);
			// if( strncmp( ( const char* )LoRaRxBuf, ( const char* )MY_TEST_Msg, 18 ) == 0 )		//ZCL 2018.11.9 RX��
			//{			//ZCL 2018.11.9 RX��

			// ��LoRa���յ������ݷ��͵�����2		ZCL 2018.11.13 OK
			for (i = 0; i < BufferSize; i++)
			{
				Txd2Buffer[i] = LoRaRxBuf[i];
			}
			// Txd2Buffer[BufferSize]=0x0D;    //ZCL 2019.1.24 ע�͵�
			// Txd2Buffer[BufferSize+1]=0x0A;
			Cw_Txd2Max = BufferSize + 2;
			Cw_Txd2 = 0;
			// RS485_CON=1;
			// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
			USART_ITConfig(USART2, USART_IT_TC, ENABLE); // ��ʼ����.
			USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]);

			LED0_TOGGLE;
			// ��ջ���
			for (i = 0; i < BUFFER_SIZE; i++)
				LoRaRxBuf[i] = 0;

			/* 				for(i=0;i < BufferSize;i++)
							{
								USART_putchar(USART1,LoRaRxBuf[i]);
							}
							printf("\n");
							LED0_TOGGLE;
							for(i=0;i<BUFFER_SIZE;i++)
								LoRaRxBuf[i] = 0;
						}			//ZCL 2018.11.9 RX��

						Soft_delay_ms(35);		//ZCL 2018.11.10  2,5,7����   8,20,15��1�Σ�25ż�����Σ�35��30,50,200����
						LCD_DLY_ms(35);				//ZCL 2018.11.12

						*/

			// ZCL 2018.11.13 B_LoRaSendTest=1,��ʱ�����
			B_LoRaRcv = 1;
		}
	}
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
