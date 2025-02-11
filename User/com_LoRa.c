/**
  ******************************************************************************
  * @file    com_LoRa.c
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.20
  * @date    2018-12-14
  * @brief   LoRa USE SPI3
	******************************************************************************
	* @note	2015.04.13

	//ZCL 2019.1.24 ��ע
	SPI3

  8. ZCL 2020.9.30
			һ��SZM220 DI�����״��⣨�̶���ַ99����ͨ��LORAת����LORA������LORA����ת��������SZM220 DO��� OK
			//!=99, �����ĸ�ѹ���SZM220;   =99 �״�����SZM220

	9. ZCL 2022.3.10  LORA�ɼ� ��ѹ���220������������24���֣����ӵ�30���֡�
		//ZCL 2022.3.10 24������������3���ֵ�����ʱ�䡣�ĳ�30����Ԥ��3��
	*/

// test svn --yls 2025.02.10
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

// #include "sys_config.h" //ZCL 2018.11.12
#include "spi.h" //ZCL 2018.11.12		sx12xxEiger�ļ�����

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
// ZCL 2018.11.12  2022.5.19
#define BUFFER_SIZE 100 // Define the payload size here  ZCL 2022.3.17    60  80

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
// ZCL 2018.11.12
tRadioDriver *Radio = NULL;
// const uint8_t MY_TEST_Msg[] = "LoRa_SX1278_SANLI";
static uint16_t BufferSize = BUFFER_SIZE; // RF buffer size

uint8_t LoRaTxBuf[BUFFER_SIZE]; // ZCL 2018.11.12
uint8_t LoRaRxBuf[BUFFER_SIZE]; // RF buffer

uint8_t LoRaTxBuf2[BUFFER_SIZE]; // ZCL 2021.11.18
uint8_t LoRaRxBuf2[BUFFER_SIZE]; // RF buffer

u8 B_LoRaSendMaster;
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
u8 B_LoRaCmd03_Master;
u8 B_LoRaCmd06_Master;

//------------------------2016.4.2
u8 S_LoRaSendNoMaster; // ����

u8 T_LoRaCountNormalNext;  // ��ʱ��
u16 C_LoRaCountNormalNext; // ������
u8 T_LoRaDelaySend;		   // ��ʱ��
u16 C_LoRaDelaySend;	   // ������

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

u8 T_LoRaCount5Err;	 // ��ʱ��		2019.10.18
u16 C_LoRaCount5Err; // ������
u8 T_LoRaCount6Err;	 // ��ʱ��
u16 C_LoRaCount6Err; // ������
u8 T_LoRaCount7Err;	 // ��ʱ��
u16 C_LoRaCount7Err; // ������
u8 T_LoRaCount8Err;	 // ��ʱ��
u16 C_LoRaCount8Err; // ������		2019.10.18

u8 T_LoRaCount9Err;		 // ��ʱ��
u16 C_LoRaCount9Err;	 // ������		2019.10.18
u8 B_LoRaErrWithModule9; //

u8 B_LoRaErrWithModule1; // ��־ ����ģ��1ͨѶʧ��
u8 B_LoRaErrWithModule2; // ��־ ����ģ��2ͨѶʧ��
u8 B_LoRaErrWithModule3; // ��־ ����ģ��3ͨѶʧ��
u8 B_LoRaErrWithModule4; // ��־ ����ģ��4ͨѶʧ��
u8 B_LoRaErrWithModule5; // ��־ ����ģ��5ͨѶʧ��
u8 B_LoRaErrWithModule6; // ��־ ����ģ��6ͨѶʧ��
u8 B_LoRaErrWithModule7; // ��־ ����ģ��7ͨѶʧ��
u8 B_LoRaErrWithModule8; // ��־ ����ģ��8ͨѶʧ��

u8 B_LoRaRCVCommandOk;
u8 B_LoRaSendMaster;

u8 T_LoRaNoRcvMasterCMD;  // ��ʱ��
u16 C_LoRaNoRcvMasterCMD; // ������
u8 B_LoRaNoRcvMasterCMD;

u8 T_LoRaSendOverTime; // ZCL 2019.1.22 Lora���ͳ�ʱ
u16 C_LoRaSendOverTime;

u8 B_LoraSendWrite; // ZCL 2021.11.16 LORA д����ķ���
u8 B_LoraSendWriteLength;

u8 T_LoRaLeiDa; // ZCL 2022.3.23  //�״�̵���2S��Ͽ�
u16 C_LoRaLeiDa;

// u8	B_WaitTxDone;					//ZCL 2019.2.15 �ȴ��������

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
extern uchar S_M35;		   // s���裺M35����

extern uint Cw_Txd3Max;	   // �ж��ٸ��ַ���Ҫ����//
extern uint Lw_Txd3ChkSum; // ����У��ͣ�lo,hi ��λ//
extern uint Cw_Txd3TmpMax;
extern uchar B_GprsDataReturn; // GPRS ���ݷ��ء������ӣ��յ�����2���ݣ���ͨ��DTU���ͳ�ȥ
extern uchar Txd3TmpBuffer[];
/* Private function prototypes -----------------------------------------------*/
void GPIO_LoRa_Configuration(void); // GPIO����
void LoRa_config(void);
u16 CRC16(u8 *pCrcData, u8 CrcDataLen);
void Delay_MS(vu16 nCount);
u16 Address(u16 *p, u16 Area); // ���Ե�ַ
extern void Com2_printf(char *fmt, ...);
extern void Com1_sendData(uint8_t txd_buffer[], uint16_t send_num);
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
	B_LoRaSendMaster = 1;
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
}

// LoRa���մ������ У�����
void LoRa_RcvProcess(void)
{
	u8 k, s, i = 0; // ��ʱ����
	u16 j;
	u16 *p_wRead;

	// 1. LORA�������ݴ洢  LORA���� ���� LORA�ӻ�
	// �����꣬����=4����ʱ��������ͣ��ӻ�=1���ȴ�����
	if (B_LoRaSendMaster == 4 || B_LoRaSendMaster == 1)
	{
		// if( Radio->Process( ) == RF_RX_DONE )
		while (Radio->Process() == RF_RX_DONE)
		{
			Radio->GetRxPacket(LoRaRxBuf, (uint16_t *)&BufferSize);
			// if( strncmp( ( const char* )LoRaRxBuf, ( const char* )MY_TEST_Msg, 18 ) == 0 )
			Cw_LoRaRx = BufferSize;
			Cw_BakLoRaRx = BufferSize; // �� Cw_LoRaRx ����

			if (w_ZhouShanProtocol_bit10) // ����1����LoRa��������շ���=1����أ�=0�������
			{
				// if (S_M35 == 0x1B)
				Com1_sendData(LoRaRxBuf, Cw_BakLoRaRx); // ���յ�����ת��������1
			}

			// ZCL 2018.12.14 Pw_LoRaSet .1λ =1   LORA�����COM2 (͸��ʹ��)
			if (F_LoRaToCom)
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
				USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]); // ZCL 2022.9.19
				USART_ITConfig(USART2, USART_IT_TC, ENABLE);   // ��ʼ����.
			}
			// LED0_TOGGLE;		//ZCL 2019.10.15 �������ָʾ���ĳɷ������ָʾ��
			// ZCL 2019.3.9
			B_LoRaRcv = 1; // �ӻ���Ҫ��������

			// Soft_delay_ms(35);		//ZCL 2018.11.10  2,5,7����   8,20,15��1�Σ�25ż�����Σ�35��30,50,200����
			// LCD_DLY_ms(35);				//ZCL 2018.11.12
		}
	}

	// ˫����
	if (Pw_EquipmentType == 0)
	{
		// 2. LORA���մ洢���ݵĴ���  LORA���� ���� LORA�ӻ�
		if (B_LoRaRcv == 1) // ���մ������
		{
			B_LoRaRcv = 2;

			// LORA�ӻ� ���յ����� ���ӵ�ַ��⣭���յ�����λ����ѯָ�
			// ZCLע����ʱ��LORA�ǵ����ӻ��������ı�����λ����ѯ�� =2�����ӻ�����3.16.1.6ָ���
			if (Pw_LoRaMasterSlaveSel == 0) // ZCL 2022.10.4 Һ��������������������γ��������õġ�
			{
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
							B_LoRaNoRcvMasterCMD = 0;
						}
						else
							i = 1;
					}
					else
						i = 2;
				}
			}

			// LORA���� ���յ����� ��Ϊ��ģ��(�������յ���) 2018.12.14
			// LORA��ģ��1����-->������������			=3.4.5.6 ��չ�ӻ���ֻ��73ָ�
			else if (LoRaRxBuf[0] == Pw_LoRaModule1Add) // �ӵ�ַ���
			{
				j = CRC16(LoRaRxBuf, Cw_BakLoRaRx - 2); // CRC У��
				k = j >> 8;								// ZCL ע��
				s = j;
				if (k == LoRaRxBuf[Cw_BakLoRaRx - 2] && s == LoRaRxBuf[Cw_BakLoRaRx - 1]) // CRCУ��
				// if ( 0xD7==LoRaRxBuf[Cw_BakLoRaRx-2]
				// && 0x98==LoRaRxBuf[Cw_BakLoRaRx-1] )		//ZCL123 2019.10.17	����
				{							// CRCУ����ȷ
					if (LoRaRxBuf[1] == 73) // 73 ����ģ��ͨѶ
					{
						/*ZCL 2017.6.18 �л�ҳ��ʱ���������������淢��ǰ��һҳ�����ݷ���������ʱ�����͵���һҳ�ϣ�
								��������������ĳɷ��ص����ݣ�����ǰ��Txd1Buffer�ķ��͵�ַ����ֵ�����ں��ˣ� OK*/

						p_wRead = w_DNBParLst; // ZCL 2022.5.19

						// 2. ���յ����ݴ洢 ZCL 2018.12.14
						if (LoRaRxBuf[2] == 40 * 2) // ZCL 2019.9.11  2019.10.15
						{
							for (i = 0; i < 40; i++) // ZCL 2022.3.21  30
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + i) = (j << 8) + LoRaRxBuf[4 + i * 2]; // 256 Lw_Reg
							}

							// �����Ƶ��ͨѶ������
							Count_Save1_No1_VVVF_Comm = (LoRaRxBuf[3 + 18 * 2] << 8) + LoRaRxBuf[4 + 18 * 2];
							if (F_No1_VVVFComm_Fault == 1)
							{
								F_LoRa1_VVVFComm_Fault = 1; // ��ͨѶ���ϱ�־
							}
							else
							{
								F_LoRa1_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
							}
						}

						w_LoraCounter1++; // ����
					}
					else if (LoRaRxBuf[1] == 03) // 03 ����ģ��ͨѶ
					{
						B_LoRaCmd03_Master = 1;
						F_ModeParLora = 0; // YLS 2024.10.07  ���Ԥ�ñ�־
					}
					else if (LoRaRxBuf[1] == 06) // 06 ����ģ��ͨѶ
					{
						B_LoRaCmd06_Master = 1;
					}

					B_LoRaRCVCommandOk = 1;
					T_LoRaCount1Err = 100;
					C_LoRaCount1Err = 0;
					B_LoRaErrWithModule1 = 0;
					F_LoRa1_Comm_Fault = 0; // YLS 2025.01.27 ��ͨѶ���ϱ�־
				}
			}

			//  LORA���� ��Ϊ��ģ��(�������յ���) 2018.12.14
			// ��ģ��2����-->������������
			else if (LoRaRxBuf[0] == Pw_LoRaModule2Add) // �ӵ�ַ���
			{
				j = CRC16(LoRaRxBuf, Cw_BakLoRaRx - 2); // CRC У��
				k = j >> 8;								// ZCL ע��
				s = j;
				if (k == LoRaRxBuf[Cw_BakLoRaRx - 2] && s == LoRaRxBuf[Cw_BakLoRaRx - 1])
				{							// CRCУ����ȷ
					if (LoRaRxBuf[1] == 73) // 73 ����ģ��ͨѶ
					{
						p_wRead = &w_DNBParLst[40]; // ZCL 2022.5.19

						// 2. ���յ����ݴ洢 ZCL 2018.12.14
						if (LoRaRxBuf[2] == 40 * 2) // ZCL 2019.9.11  2019.10.15
						{
							for (i = 0; i < 40; i++) // ZCL 2022.3.21  30
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + i) = (j << 8) + LoRaRxBuf[4 + i * 2]; // 256 Lw_Regd
							}

							// �����Ƶ��ͨѶ������
							Count_Save1_No2_VVVF_Comm = (LoRaRxBuf[3 + 18 * 2] << 8) + LoRaRxBuf[4 + 18 * 2];
							if (F_No2_VVVFComm_Fault == 1)
							{
								F_LoRa2_VVVFComm_Fault = 1; // ��ͨѶ���ϱ�־
							}
							else
							{
								F_LoRa2_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
							}
						}

						w_LoraCounter2++; // ����
					}
					else if (LoRaRxBuf[1] == 03) // 03 ����ģ��ͨѶ
					{
						B_LoRaCmd03_Master = 1;
						F_ModeParLora = 0; // YLS 2024.10.07  ���Ԥ�ñ�־
					}
					else if (LoRaRxBuf[1] == 06) // 06 ����ģ��ͨѶ
					{
						B_LoRaCmd06_Master = 1;
					}
					B_LoRaRCVCommandOk = 1;
					T_LoRaCount2Err = 100;
					C_LoRaCount2Err = 0;
					B_LoRaErrWithModule2 = 0;
					F_LoRa2_Comm_Fault = 0; // YLS 2025.01.27 ��ͨѶ���ϱ�־
				}
			}

			//  LORA���� ��Ϊ��ģ��(�������յ���) 2018.12.14
			// ��ģ��3����-->������������
			else if (LoRaRxBuf[0] == Pw_LoRaModule3Add) // �ӵ�ַ���
			{
				j = CRC16(LoRaRxBuf, Cw_BakLoRaRx - 2); // CRC У��
				k = j >> 8;								// ZCL ע��
				s = j;
				if (k == LoRaRxBuf[Cw_BakLoRaRx - 2] && s == LoRaRxBuf[Cw_BakLoRaRx - 1])
				{							// CRCУ����ȷ
					if (LoRaRxBuf[1] == 73) // 73 ����ģ��ͨѶ
					{
						p_wRead = &w_DNBParLst[80]; // ZCL 2022.9.19

						// 2. ���յ����ݴ洢 ZCL 2018.12.14
						if (LoRaRxBuf[2] == 40 * 2) // ZCL 2019.9.11  2019.10.15
						{
							for (i = 0; i < 40; i++) // ZCL 2022.3.21  30
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + i) = (j << 8) + LoRaRxBuf[4 + i * 2]; // 256 Lw_Reg
							}

							// �����Ƶ��ͨѶ������
							Count_Save1_No3_VVVF_Comm = (LoRaRxBuf[3 + 18 * 2] << 8) + LoRaRxBuf[4 + 18 * 2];
							if (F_No3_VVVFComm_Fault == 1)
							{
								F_LoRa3_VVVFComm_Fault = 1; // ��ͨѶ���ϱ�־
							}
							else
							{
								F_LoRa3_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
							}
						}

						w_LoraCounter3++; // ����
					}
					else if (LoRaRxBuf[1] == 03) // 03 ����ģ��ͨѶ
					{
						B_LoRaCmd03_Master = 1;
						F_ModeParLora = 0; // YLS 2024.10.07  ���Ԥ�ñ�־
					}
					else if (LoRaRxBuf[1] == 06) // 06 ����ģ��ͨѶ
					{
						B_LoRaCmd06_Master = 1;
					}
					B_LoRaRCVCommandOk = 1;
					T_LoRaCount3Err = 100;
					C_LoRaCount3Err = 0;
					B_LoRaErrWithModule3 = 0;
					F_LoRa3_Comm_Fault = 0; // YLS 2025.01.27 ��ͨѶ���ϱ�־
				}
			}

			//  LORA���� ��Ϊ��ģ��(�������յ���) 2018.12.14
			// ��ģ��4����-->������������
			else if (LoRaRxBuf[0] == Pw_LoRaModule4Add) // �ӵ�ַ���
			{
				j = CRC16(LoRaRxBuf, Cw_BakLoRaRx - 2); // CRC У��
				k = j >> 8;								// ZCL ע��
				s = j;
				if (k == LoRaRxBuf[Cw_BakLoRaRx - 2] && s == LoRaRxBuf[Cw_BakLoRaRx - 1])
				{							// CRCУ����ȷ
					if (LoRaRxBuf[1] == 73) // 73 ����ģ��ͨѶ
					{
						p_wRead = &w_DNBParLst[120]; // ZCL 2022.9.19

						// 2. ���յ����ݴ洢 ZCL 2018.12.14
						if (LoRaRxBuf[2] == 40 * 2) // ZCL 2019.9.11  2019.10.15
						{
							for (i = 0; i < 40; i++) // ZCL 2022.3.21  30
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + i) = (j << 8) + LoRaRxBuf[4 + i * 2]; // 256 Lw_Reg
							}

							// �����Ƶ��ͨѶ������
							Count_Save1_No4_VVVF_Comm = (LoRaRxBuf[3 + 18 * 2] << 8) + LoRaRxBuf[4 + 18 * 2];
							if (F_No4_VVVFComm_Fault == 1)
							{
								F_LoRa4_VVVFComm_Fault = 1; // ��ͨѶ���ϱ�־
							}
							else
							{
								F_LoRa4_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
							}
						}

						w_LoraCounter4++; // ����
					}
					else if (LoRaRxBuf[1] == 03) // 03 ����ģ��ͨѶ
					{
						B_LoRaCmd03_Master = 1;
						F_ModeParLora = 0; // YLS 2024.10.07  ���Ԥ�ñ�־
					}
					else if (LoRaRxBuf[1] == 06) // 06 ����ģ��ͨѶ
					{
						B_LoRaCmd06_Master = 1;
					}
					B_LoRaRCVCommandOk = 1;
					T_LoRaCount4Err = 100;
					C_LoRaCount4Err = 0;
					B_LoRaErrWithModule4 = 0;
					F_LoRa4_Comm_Fault = 0; // YLS 2025.01.27 ��ͨѶ���ϱ�־
				}
			}

			//  LORA���� ��Ϊ��ģ��(�������յ���) 2018.12.15
			// ��ģ��6����-->������������--------------------------------------------------------------
			//   LORA���� ��Ϊ��ģ��(�������յ���) 2018.12.14
			//  ��ģ��5����-->������������
			else if (LoRaRxBuf[0] == Pw_LoRaModule5Add) // �ӵ�ַ���
			{
				j = CRC16(LoRaRxBuf, Cw_BakLoRaRx - 2); // CRC У��
				k = j >> 8;								// ZCL ע��
				s = j;
				if (k == LoRaRxBuf[Cw_BakLoRaRx - 2] && s == LoRaRxBuf[Cw_BakLoRaRx - 1])
				{							// CRCУ����ȷ
					if (LoRaRxBuf[1] == 73) // 73 ����ģ��ͨѶ
					{
						p_wRead = &w_DNBParLst[160]; // ZCL 2022.5.19

						// 2. ���յ����ݴ洢 ZCL 2018.12.14
						if (LoRaRxBuf[2] == 40 * 2) // ZCL 2019.9.11  2019.10.15
						{
							for (i = 0; i < 40; i++) // ZCL 2022.3.21  30
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + i) = (j << 8) + LoRaRxBuf[4 + i * 2]; // 256 Lw_Reg
							}

							// �����Ƶ��ͨѶ������
							Count_Save1_No5_VVVF_Comm = (LoRaRxBuf[3 + 18 * 2] << 8) + LoRaRxBuf[4 + 18 * 2];
							if (F_No5_VVVFComm_Fault == 1)
							{
								F_LoRa5_VVVFComm_Fault = 1; // ��ͨѶ���ϱ�־
							}
							else
							{
								F_LoRa5_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
							}
						}

						w_LoraCounter5++; // ����
					}
					else if (LoRaRxBuf[1] == 03) // 03 ����ģ��ͨѶ
					{
						B_LoRaCmd03_Master = 1;
						F_ModeParLora = 0; // YLS 2024.10.07  ���Ԥ�ñ�־
					}
					else if (LoRaRxBuf[1] == 06) // 06 ����ģ��ͨѶ
					{
						B_LoRaCmd06_Master = 1;
					}
					B_LoRaRCVCommandOk = 1;
					T_LoRaCount5Err = 100;
					C_LoRaCount5Err = 0;
					B_LoRaErrWithModule5 = 0;
					F_LoRa5_Comm_Fault = 0; // YLS 2025.01.27 ��ͨѶ���ϱ�־
				}
			}

			//  LORA���� ��Ϊ��ģ��(�������յ���) 2018.12.15
			// ��ģ��6����-->������������
			else if (LoRaRxBuf[0] == Pw_LoRaModule6Add) // �ӵ�ַ���
			{
				j = CRC16(LoRaRxBuf, Cw_BakLoRaRx - 2); // CRC У��
				k = j >> 8;								// ZCL ע��
				s = j;
				if (k == LoRaRxBuf[Cw_BakLoRaRx - 2] && s == LoRaRxBuf[Cw_BakLoRaRx - 1])
				{							// CRCУ����ȷ
					if (LoRaRxBuf[1] == 73) // 73 ����ģ��ͨѶ
					{
						p_wRead = &w_DNBParLst[200]; // ZCL 2022.5.19

						// 2. ���յ����ݴ洢 ZCL 2018.12.14
						if (LoRaRxBuf[2] == 40 * 2) // ZCL 2019.9.11  2019.10.15
						{
							for (i = 0; i < 45; i++) // ZCL 2022.3.21  30
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + 256 + 45 * 2 + i) = (j << 8) + LoRaRxBuf[4 + i * 2]; // 256 Lw_Reg
							}

							// �����Ƶ��ͨѶ������
							Count_Save1_No6_VVVF_Comm = (LoRaRxBuf[3 + 18 * 2] << 8) + LoRaRxBuf[4 + 18 * 2];
							if (F_No6_VVVFComm_Fault == 1)
							{
								F_LoRa6_VVVFComm_Fault = 1; // ��ͨѶ���ϱ�־
							}
							else
							{
								F_LoRa6_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
							}
						}
						w_LoraCounter6++; // ����
					}
					else if (LoRaRxBuf[1] == 03) // 03 ����ģ��ͨѶ
					{
						B_LoRaCmd03_Master = 1;
						F_ModeParLora = 0; // YLS 2024.10.07  ���Ԥ�ñ�־
					}
					else if (LoRaRxBuf[1] == 06) // 06 ����ģ��ͨѶ
					{
						B_LoRaCmd06_Master = 1;
					}
					B_LoRaRCVCommandOk = 1;
					T_LoRaCount6Err = 100;
					C_LoRaCount6Err = 0;
					B_LoRaErrWithModule6 = 0;
					F_LoRa6_Comm_Fault = 0; // YLS 2025.01.27 ��ͨѶ���ϱ�־
				}
			}

			//  LORA���� ��Ϊ��ģ��(�������յ���) 2018.12.14
			// ��ģ��7����-->������������
			else if (LoRaRxBuf[0] == Pw_LoRaModule7Add) // �ӵ�ַ���
			{
				j = CRC16(LoRaRxBuf, Cw_BakLoRaRx - 2); // CRC У��
				k = j >> 8;								// ZCL ע��
				s = j;
				if (k == LoRaRxBuf[Cw_BakLoRaRx - 2] && s == LoRaRxBuf[Cw_BakLoRaRx - 1])
				{							// CRCУ����ȷ
					if (LoRaRxBuf[1] == 73) // 73 ����ģ��ͨѶ
					{
						p_wRead = &w_DNBParLst[240]; // ZCL 2022.5.19

						// 2. ���յ����ݴ洢 ZCL 2018.12.14
						if (LoRaRxBuf[2] == 40 * 2) // ZCL 2019.9.11  2019.10.15
						{
							for (i = 0; i < 45; i++) // ZCL 2022.3.21  30
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + 256 + 45 * 3 + i) = (j << 8) + LoRaRxBuf[4 + i * 2]; // 256 Lw_Reg
							}

							// �����Ƶ��ͨѶ������
							Count_Save1_No7_VVVF_Comm = (LoRaRxBuf[3 + 18 * 2] << 8) + LoRaRxBuf[4 + 18 * 2];
							if (F_No7_VVVFComm_Fault == 1)
							{
								F_LoRa7_VVVFComm_Fault = 1; // ��ͨѶ���ϱ�־
							}
							else
							{
								F_LoRa7_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
							}
						}

						w_LoraCounter7++; // ����
					}
					else if (LoRaRxBuf[1] == 03) // 03 ����ģ��ͨѶ
					{
						B_LoRaCmd03_Master = 1;
						F_ModeParLora = 0; // YLS 2024.10.07  ���Ԥ�ñ�־
					}
					else if (LoRaRxBuf[1] == 06) // 06 ����ģ��ͨѶ
					{
						B_LoRaCmd06_Master = 1;
					}
					B_LoRaRCVCommandOk = 1;
					T_LoRaCount7Err = 100;
					C_LoRaCount7Err = 0;
					B_LoRaErrWithModule7 = 0;
					F_LoRa7_Comm_Fault = 0; // YLS 2025.01.27 ��ͨѶ���ϱ�־
				}
			}

			//  LORA���� ��Ϊ��ģ��(�������յ���) 2018.12.14
			// ��ģ��8����-->������������
			else if (LoRaRxBuf[0] == Pw_LoRaModule8Add) // �ӵ�ַ���
			{
				j = CRC16(LoRaRxBuf, Cw_BakLoRaRx - 2); // CRC У��
				k = j >> 8;								// ZCL ע��
				s = j;
				if (k == LoRaRxBuf[Cw_BakLoRaRx - 2] && s == LoRaRxBuf[Cw_BakLoRaRx - 1])
				{							// CRCУ����ȷ
					if (LoRaRxBuf[1] == 73) // 73 ����ģ��ͨѶ
					{
						p_wRead = &w_DNBParLst[280]; // ZCL 2022.5.19

						// 2. ���յ����ݴ洢 ZCL 2018.12.14
						if (LoRaRxBuf[2] == 40 * 2) // ZCL 2019.9.11  2019.10.15
						{
							for (i = 0; i < 45; i++) // ZCL 2022.3.21  30
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + 256 + 45 * 3 + i) = (j << 8) + LoRaRxBuf[4 + i * 2]; // 256 Lw_Reg
							}

							// �����Ƶ��ͨѶ������
							Count_Save1_No8_VVVF_Comm = (LoRaRxBuf[3 + 18 * 2] << 8) + LoRaRxBuf[4 + 18 * 2];
							if (F_No8_VVVFComm_Fault == 1)
							{
								F_LoRa8_VVVFComm_Fault = 1; // ��ͨѶ���ϱ�־
							}
							else
							{
								F_LoRa8_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
							}
						}
						w_LoraCounter8++; // ����
					}
					else if (LoRaRxBuf[1] == 03) // 03 ����ģ��ͨѶ
					{
						B_LoRaCmd03_Master = 1;
					}
					else if (LoRaRxBuf[1] == 06) // 06 ����ģ��ͨѶ
					{
						B_LoRaCmd06_Master = 1;
					}
					B_LoRaRCVCommandOk = 1;
					T_LoRaCount8Err = 100;
					C_LoRaCount8Err = 0;
					B_LoRaErrWithModule8 = 0;
					F_LoRa8_Comm_Fault = 0; // YLS 2025.01.27 ��ͨѶ���ϱ�־
				}
			}

			//  LORA���� ��Ϊ��ģ��(�������յ���) ZCL 2020.9.30
			// ��ģ�� 99 ����-->������������ (�״����SZM220 ���ص�����)
			else if (0) // �ӵ�ַ���		LoRaRxBuf[0]==w_LoRaLeiDaModuleAdd
			{
				j = CRC16(LoRaRxBuf, Cw_BakLoRaRx - 2); // CRC У��
				k = j >> 8;								// ZCL ע��
				s = j;
				if (k == LoRaRxBuf[Cw_BakLoRaRx - 2] && s == LoRaRxBuf[Cw_BakLoRaRx - 1])
				{							// CRCУ����ȷ
					if (LoRaRxBuf[1] == 73) // 73 ����ģ��ͨѶ
					{
						// ZCL 2020.9.30  �״�ר��SZM220�͹�����DIֵ
						j = LoRaRxBuf[3];
						w_LoRaLeiDaDiValue = (j << 8) + LoRaRxBuf[4];

						B_LoRaRCVCommandOk = 1;
						//

						T_LoRaCount9Err = 100; // ZCL 2022.3.11 �⼸��ԭ��д����8������������ T_LoRaCount8Err
						C_LoRaCount9Err = 0;
						B_LoRaErrWithModule9 = 0;
					}
				}
			}

			else
				i = 3;
		}
	}
	else // ��Ƶ���
	{
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
						B_LoRaNoRcvMasterCMD = 0;
					}
					else
						i = 1;
				}
				else
					i = 2;
			}

			// LORA���� ���յ����� ��Ϊ��ģ��(�������յ���) 2018.12.14
			// LORA��ģ��1����-->������������			=3.4.5.6 ��չ�ӻ���ֻ��73ָ�
			else if (LoRaRxBuf[0] == Pw_LoRaModule1Add) // �ӵ�ַ���
			{
				j = CRC16(LoRaRxBuf, Cw_BakLoRaRx - 2); // CRC У��
				k = j >> 8;								// ZCL ע��
				s = j;
				if (k == LoRaRxBuf[Cw_BakLoRaRx - 2] && s == LoRaRxBuf[Cw_BakLoRaRx - 1]) // ZCL 2019.10.17
				// if ( 0xD7==LoRaRxBuf[Cw_BakLoRaRx-2]
				// && 0x98==LoRaRxBuf[Cw_BakLoRaRx-1] )		//ZCL123 2019.10.17	����
				{							// CRCУ����ȷ
					if (LoRaRxBuf[1] == 73) // 73 ����ģ��ͨѶ
					{
						/*ZCL 2017.6.18 �л�ҳ��ʱ���������������淢��ǰ��һҳ�����ݷ���������ʱ�����͵���һҳ�ϣ�
								��������������ĳɷ��ص����ݣ�����ǰ��Txd1Buffer�ķ��͵�ַ����ֵ�����ں��ˣ� OK*/

						// 1. ת����ַ ZCL 2019.3.9
						j = LoRaTxBuf[2];
						Lw_Reg = (j << 8) + LoRaTxBuf[3];

						p_wRead = &w_dsp1SoftVersion;

						// 2. ���յ����ݴ洢 ZCL 2018.12.14
						if (LoRaRxBuf[2] == 25 * 2) // ZCL 2019.9.11  2021.11.10
						{
							for (i = 0; i < 25; i++) // ZCL 2019.9.11  LoRaRxBuf[2]/2
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + Lw_Reg + i) = (j << 8) + LoRaRxBuf[4 + i * 2];
							}
						}

						else if (LoRaRxBuf[2] == 16 * 2) // ZCL 2019.9.11
						{
							for (i = 0; i < 10; i++) // ZCL 2019.9.11  LoRaRxBuf[2]/2
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + Lw_Reg + i) = (j << 8) + LoRaRxBuf[4 + i * 2];
							}

							// ZCL 2019.9.11 �¼ӵģ���6��
							p_wRead = &w_1InPDec;
							for (i = 10; i < 16; i++)
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + Lw_Reg + i - 10) = (j << 8) + LoRaRxBuf[4 + i * 2]; // ZCL 2019.9.11  -10
							}
						}
						//
						B_LoRaRCVCommandOk = 1;
						// B_LoRaSendMaster=8;				//ZCL 2019.10.17
						//
						// Radio->StartRx( );   //RFLR_STATE_RX_INIT			ZCL 2019.10.17

						T_LoRaCount1Err = 100;
						C_LoRaCount1Err = 0;
						B_LoRaErrWithModule1 = 0;

						// ZCL 2022.10.4
						w_LoraCounter1++; // ����
					}
					// ZCL 2021.11.18  ���մӻ���Ӧ�� 16  06 ָ�������ʲôʵ�ʲ�����
					else if (LoRaRxBuf[1] == 16 || LoRaRxBuf[1] == 6) // 16  6
					{
						B_LoRaRCVCommandOk = 1;
						//

						T_LoRaCount1Err = 100;
						C_LoRaCount1Err = 0;
						B_LoRaErrWithModule1 = 0;
					}
				}
			}

			//  LORA���� ��Ϊ��ģ��(�������յ���) 2018.12.14
			// ��ģ��2����-->������������
			else if (LoRaRxBuf[0] == Pw_LoRaModule2Add) // �ӵ�ַ���
			{
				j = CRC16(LoRaRxBuf, Cw_BakLoRaRx - 2); // CRC У��
				k = j >> 8;								// ZCL ע��
				s = j;
				if (k == LoRaRxBuf[Cw_BakLoRaRx - 2] && s == LoRaRxBuf[Cw_BakLoRaRx - 1])
				{							// CRCУ����ȷ
					if (LoRaRxBuf[1] == 73) // 73 ����ģ��ͨѶ
					{
						// 1. ת����ַ ZCL 2019.3.9
						j = LoRaTxBuf[2];
						Lw_Reg = (j << 8) + LoRaTxBuf[3];

						// p_wRead=AddressChange();

						p_wRead = &w_dsp2SoftVersion;

						// 2. ���յ����ݴ洢 ZCL 2018.12.14
						if (LoRaRxBuf[2] == 25 * 2) // ZCL 2019.9.11  2021.11.10
						{
							for (i = 0; i < 25; i++) // ZCL 2019.9.11  LoRaRxBuf[2]/2
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + Lw_Reg + i) = (j << 8) + LoRaRxBuf[4 + i * 2];
							}
						}

						else if (LoRaRxBuf[2] == 16 * 2) // ZCL 2019.9.11
						{
							for (i = 0; i < 10; i++) // ZCL 2019.9.11  LoRaRxBuf[2]/2
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + Lw_Reg + i) = (j << 8) + LoRaRxBuf[4 + i * 2];
							}

							// ZCL 2019.9.11 �¼ӵģ���6��
							p_wRead = &w_2InPDec;
							for (i = 10; i < 16; i++)
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + Lw_Reg + i - 10) = (j << 8) + LoRaRxBuf[4 + i * 2]; // ZCL 2019.9.11  -10
							}
						}

						B_LoRaRCVCommandOk = 1;
						//

						T_LoRaCount2Err = 100;
						C_LoRaCount2Err = 0;
						B_LoRaErrWithModule2 = 0;
						// ZCL 2022.10.4
						w_LoraCounter1++; // ����
					}
				}
			}

			// ZCL 2022.9.10  �޸ļ��룺��ģ��3  ��ģ��4

			//  LORA���� ��Ϊ��ģ��(�������յ���) 2018.12.14
			// ��ģ��3����-->������������
			else if (LoRaRxBuf[0] == Pw_LoRaModule3Add) // �ӵ�ַ���
			{
				j = CRC16(LoRaRxBuf, Cw_BakLoRaRx - 2); // CRC У��
				k = j >> 8;								// ZCL ע��
				s = j;
				if (k == LoRaRxBuf[Cw_BakLoRaRx - 2] && s == LoRaRxBuf[Cw_BakLoRaRx - 1])
				{							// CRCУ����ȷ
					if (LoRaRxBuf[1] == 73) // 73 ����ģ��ͨѶ
					{
						// 1. ת����ַ ZCL 2019.3.9
						j = LoRaTxBuf[2];
						Lw_Reg = (j << 8) + LoRaTxBuf[3];

						// p_wRead=AddressChange();

						p_wRead = &w_dsp3SoftVersion;

						// 2. ���յ����ݴ洢 ZCL 2018.12.14
						if (LoRaRxBuf[2] == 25 * 2) // ZCL 2019.9.11  2021.11.10
						{
							for (i = 0; i < 25; i++) // ZCL 2019.9.11  LoRaRxBuf[2]/2
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + Lw_Reg + i) = (j << 8) + LoRaRxBuf[4 + i * 2];
							}
						}

						else if (LoRaRxBuf[2] == 16 * 2) // ZCL 2019.9.11
						{
							for (i = 0; i < 10; i++) // ZCL 2019.9.11  LoRaRxBuf[2]/2
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + Lw_Reg + i) = (j << 8) + LoRaRxBuf[4 + i * 2];
							}

							// ZCL 2019.9.11 �¼ӵģ���6��
							p_wRead = &w_3InPDec;
							for (i = 10; i < 16; i++)
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + Lw_Reg + i - 10) = (j << 8) + LoRaRxBuf[4 + i * 2]; // ZCL 2019.9.11  -10
							}
						}

						B_LoRaRCVCommandOk = 1;
						//

						T_LoRaCount3Err = 100;
						C_LoRaCount3Err = 0;
						B_LoRaErrWithModule3 = 0;
						// ZCL 2022.10.4
						w_LoraCounter1++; // ����
					}
				}
			}

			//  LORA���� ��Ϊ��ģ��(�������յ���) 2018.12.14
			// ��ģ��4����-->������������
			else if (LoRaRxBuf[0] == Pw_LoRaModule4Add) // �ӵ�ַ���
			{
				j = CRC16(LoRaRxBuf, Cw_BakLoRaRx - 2); // CRC У��
				k = j >> 8;								// ZCL ע��
				s = j;
				if (k == LoRaRxBuf[Cw_BakLoRaRx - 2] && s == LoRaRxBuf[Cw_BakLoRaRx - 1])
				{							// CRCУ����ȷ
					if (LoRaRxBuf[1] == 73) // 73 ����ģ��ͨѶ
					{

						// 1. ת����ַ ZCL 2019.3.9
						j = LoRaTxBuf[2];
						Lw_Reg = (j << 8) + LoRaTxBuf[3];

						// p_wRead=AddressChange();

						p_wRead = &w_dsp4SoftVersion;

						// 2. ���յ����ݴ洢 ZCL 2018.12.14
						if (LoRaRxBuf[2] == 25 * 2) // ZCL 2019.9.11  2021.11.10
						{
							for (i = 0; i < 25; i++) // ZCL 2019.9.11  LoRaRxBuf[2]/2
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + Lw_Reg + i) = (j << 8) + LoRaRxBuf[4 + i * 2];
							}
						}

						else if (LoRaRxBuf[2] == 16 * 2) // ZCL 2019.9.11
						{
							for (i = 0; i < 10; i++) // ZCL 2019.9.11  LoRaRxBuf[2]/2
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + Lw_Reg + i) = (j << 8) + LoRaRxBuf[4 + i * 2];
							}

							// ZCL 2019.9.11 �¼ӵģ���6��
							p_wRead = &w_4InPDec;
							for (i = 10; i < 16; i++)
							{
								j = LoRaRxBuf[3 + i * 2];
								//*(p_wRead+i)=(j<<8)+LoRaRxBuf[4+i*2];
								*(p_wRead + Lw_Reg + i - 10) = (j << 8) + LoRaRxBuf[4 + i * 2]; // ZCL 2019.9.11  -10
							}
						}

						B_LoRaRCVCommandOk = 1;
						//

						T_LoRaCount4Err = 100;
						C_LoRaCount4Err = 0;
						B_LoRaErrWithModule4 = 0;
						// ZCL 2022.10.4
						w_LoraCounter1++; // ����
					}
				}
			}

			else
				i = 3;
		}
	}
}

void LoRa_SlaveSend(void) // LoRa�ӻ�����
{
	u16 m; //,n;
	u8 j = 0, k;
	u16 *p_wRead;
	u8 *p_bMove;
	// u8	 *p_bGen;
	// u16	 *p_wTarget;			// ָ��Ŀ���ַ�����xdata zcl

	if (B_LoRaCmd03) // ��ȡ���ּĴ���
	{
		LoRaTxBuf[0] = LoRaRxBuf[0];	 // �豸�ӵ�ַPw_EquipmentNo
		LoRaTxBuf[1] = LoRaRxBuf[1];	 // ������
		LoRaTxBuf[2] = LoRaRxBuf[5] * 2; // LoRaRxBuf[5]=����

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
		// Radio->SetTxPacket(LoRaTxBuf, Cw_LoRaTxMax );   //RFLR_STATE_TX_INIT
		B_LoRaSendMaster = 22;
	}

	else if (B_LoRaCmd16 || B_LoRaCmd06) // 16Ԥ�ö�Ĵ���
	{
		j = LoRaRxBuf[2];
		Lw_Reg = (j << 8) + LoRaRxBuf[3];
		// ZCL 2018.12.14 ��ַת��
		p_wRead = AddressChange();

		B_LoRaCmd16 = 0;
		B_LoRaCmd06 = 0;
		Cw_LoRaTx = 0;
		// ZCL 2018.12.14 ����
		// Radio->SetTxPacket(LoRaTxBuf, Cw_LoRaTxMax );   //RFLR_STATE_TX_INIT
		B_LoRaSendMaster = 22;
	}

	// ZCL 2019.9.24 ��ѯ16���֣���֡ͷ֡β����37���ֽ�
	else if (B_LoRaCmd73) // 73
	{
		Delay_MS(5); // 500? ZCL123

		LoRaTxBuf[0] = LoRaRxBuf[0];	 // �豸�ӵ�ַPw_EquipmentNo
		LoRaTxBuf[1] = LoRaRxBuf[1];	 // ������
		LoRaTxBuf[2] = LoRaRxBuf[5] * 2; // LoRaRxBuf[5]=���� ��

		// ZCL 2018.12.14 ������е�ַ�ϳ�
		j = LoRaRxBuf[2];
		Lw_Reg = (j << 8) + LoRaRxBuf[3];
		// ZCL 2018.12.14 ��ַת��
		// p_wRead=AddressChange();
		p_wRead = &w_dsp1SoftVersion; // ZCL 2019.3.8

		p_bMove = LoRaTxBuf;

		if (LoRaRxBuf[5] == 10) // ZCL 2019.9.11
		{
			for (k = 0; k < LoRaRxBuf[5]; k++) // ����ѯ����
			{
				// m=*(p_wRead+Lw_Reg+k);			//ZCL 2019.1.23 ����
				// m=*(p_wRead+k);
				m = *(p_wRead + Lw_Reg + k); // ZCL 2019.3.7 ��Ϊ���������飬������ҪLw_Reg

				*(p_bMove + 3 + k * 2) = m >> 8;
				*(p_bMove + 3 + k * 2 + 1) = m;
			}
		}

		// ZCL 2019.9.11 �Ӵ�16����
		else if (LoRaRxBuf[5] == 16)
		{
			p_wRead = &w_dsp1SoftVersion; // ZCL 2019.3.8
			for (k = 0; k < 10; k++)	  // ����ѯ����
			{
				// m=*(p_wRead+Lw_Reg+k);			//ZCL 2019.1.23 ����
				// m=*(p_wRead+k);
				m = *(p_wRead + Lw_Reg + k); // ZCL 2019.3.7 ��Ϊ���������飬������ҪLw_Reg

				*(p_bMove + 3 + k * 2) = m >> 8;
				*(p_bMove + 3 + k * 2 + 1) = m;
			}

			p_wRead = &w_1InPDec;	  // ZCL 2019.9.11
			for (k = 10; k < 16; k++) // ����ѯ���ݣ�2019.9.11  ����6��
			{
				// m=*(p_wRead+Lw_Reg+k);			//ZCL 2019.1.23 ����
				// m=*(p_wRead+k);
				m = *(p_wRead + Lw_Reg + k - 10); // ZCL 2019.3.7 ��Ϊ���������飬������ҪLw_Reg		  ZCL 2019.9.11  -10

				*(p_bMove + 3 + k * 2) = m >> 8;
				*(p_bMove + 3 + k * 2 + 1) = m;
			}
			//
		}

		Lw_LoRaChkSum = CRC16(LoRaTxBuf, LoRaTxBuf[2] + 3);
		LoRaTxBuf[LoRaTxBuf[2] + 3] = Lw_LoRaChkSum >> 8; // /256
		LoRaTxBuf[LoRaTxBuf[2] + 4] = Lw_LoRaChkSum;	  // ��λ�ֽ�

		Cw_LoRaTxMax = LoRaTxBuf[2] + 5; // ZCL 2019.9.24 ��ѯ16���֣���֡ͷ֡β����37���ֽ�
		B_LoRaCmd73 = 0;
		Cw_LoRaTx = 0;
		// ZCL 2018.12.14 ����
		// Radio->SetTxPacket(LoRaTxBuf, Cw_LoRaTxMax );   //RFLR_STATE_TX_INIT
		B_LoRaSendMaster = 22;
	}

	else if (B_LoRaCmd03_Master) // ��ȡ�ӻ�����
	{
		Txd3TmpBuffer[0] = Pw_LoRaEquipmentNo; // ����LoRa��ַ

		// YLS 2024.10.06 Ϊ��������λ����������̨�ӻ����ص����ݣ���03��������иĽ���������λ������
		if (LoRaRxBuf[0] == Pw_LoRaModule1Add)
		{
			Txd3TmpBuffer[1] = LoRaRxBuf[1] + 0x10; // ������
		}
		else if (LoRaRxBuf[0] == Pw_LoRaModule2Add)
		{
			Txd3TmpBuffer[1] = LoRaRxBuf[1] + 0x20; // ������
		}
		else if (LoRaRxBuf[0] == Pw_LoRaModule3Add)
		{
			Txd3TmpBuffer[1] = LoRaRxBuf[1] + 0x30; // ������
		}
		else if (LoRaRxBuf[0] == Pw_LoRaModule4Add)
		{
			Txd3TmpBuffer[1] = LoRaRxBuf[1] + 0x40; // ������
		}
		else if (LoRaRxBuf[0] == Pw_LoRaModule5Add)
		{
			Txd3TmpBuffer[1] = LoRaRxBuf[1] + 0x50; // ������
		}

		Txd3TmpBuffer[2] = LoRaRxBuf[2]; // Rcv2Buffer[5]=���� ��

		for (k = 1; k <= LoRaRxBuf[2]; k++) // ����ѯ����
		{
			Txd3TmpBuffer[2 + k] = LoRaRxBuf[2 + k]; // Rcv2Buffer[5]=���� ��
		}
		Lw_Txd3ChkSum = CRC16(Txd3TmpBuffer, Txd3TmpBuffer[2] + 3);
		Txd3TmpBuffer[Txd3TmpBuffer[2] + 3] = Lw_Txd3ChkSum >> 8; // /256
		Txd3TmpBuffer[Txd3TmpBuffer[2] + 4] = Lw_Txd3ChkSum;	  // ��λ�ֽ�
		Cw_Txd3TmpMax = Txd3TmpBuffer[2] + 5;
		//
		B_LoRaCmd03_Master = 0;
		Cw_Txd3 = 0;
		B_GprsDataReturn = 1; // ģ��͸���У������յ����ݣ�ת����GPRS����
	}
	else if (B_LoRaCmd06_Master) // ��ȡ�ӻ�����
	{
		Txd3TmpBuffer[0] = Pw_LoRaEquipmentNo; // ����LoRa��ַ

		Txd3TmpBuffer[1] = LoRaRxBuf[1];
		Txd3TmpBuffer[2] = LoRaRxBuf[2];
		Txd3TmpBuffer[3] = LoRaRxBuf[3];
		Txd3TmpBuffer[4] = LoRaRxBuf[4];
		Txd3TmpBuffer[5] = LoRaRxBuf[5];

		Lw_Txd3ChkSum = CRC16(Txd3TmpBuffer, 6);
		Txd3TmpBuffer[6] = Lw_Txd3ChkSum >> 8; // /256
		Txd3TmpBuffer[7] = Lw_Txd3ChkSum;	   // ��λ�ֽ�
		Cw_Txd3TmpMax = 8;
		//
		B_LoRaCmd06_Master = 0;
		Cw_Txd3 = 0;
		B_GprsDataReturn = 1; // ģ��͸���У������յ����ݣ�ת����GPRS����
	}
}

// ��Ϊ��ģ��ʱ���� 2016.4.1
void LoRa_MasterSend(void) // LoRa�����ͳ���
{
	u8 i = 0, j = 0;

	w_LoRaLeiDaModuleAdd = 0;

	if (Pw_LoRaModule1Add > 0)
		i++; // ͳ�Ʒ�������
	if (Pw_LoRaModule2Add > 0)
		i++;
	if (Pw_LoRaModule3Add > 0)
		i++;
	if (Pw_LoRaModule4Add > 0)
		i++;
	if (Pw_LoRaModule5Add > 0)
		i++;
	if (Pw_LoRaModule6Add > 0)
		i++;
	if (Pw_LoRaModule7Add > 0)
		i++;
	if (Pw_LoRaModule8Add > 0)
		i++;

	// ˫����
	if (Pw_EquipmentType == 0)
	{
		if (i > 0) // ��������ģ����߱�Ƶ���������Ʋ��ܽ��뷢�ͳ���
		{
			if (B_LoRaSendMaster == 1) //&& B_LoRaRCVCommandOk
			{
				if (T_LoRaDelaySend != SClk10Ms)
				{
					T_LoRaDelaySend = SClk10Ms; //
					C_LoRaDelaySend++;
					if (C_LoRaDelaySend > 20) // 210���� ZCL 2019.10.17	��ʹ����������ģʽ��2�������ˣ� �����
					{
						T_LoRaDelaySend = 100;
						C_LoRaDelaySend = 0;

						if (F_ModeParLora == 0) // ��û���޸�����ͷ��Ͳ�ѯָ��
						{
							// 1. ���Ʒ��Ͳ���
							S_LoRaSendNoMaster++;

							// S_LoRaSendNoMaster=1;  //ZCL123		����ʱ�ã�ֻ����1����ַ�ģ�����ɾ�����ָ�++

							if (S_LoRaSendNoMaster > i) // ZCL 2020.9.30  8  ZCL 2023.2.9  4
								S_LoRaSendNoMaster = 1; // COM_UNIT_NUM

							// 2.����˳����
							if (Pw_LoRaModule1Add > 0 && S_LoRaSendNoMaster == 1)
							{
								LoRaTxBuf[0] = Pw_LoRaModule1Add; // �豸�ӵ�ַ
								j = 1;
							}
							else if (Pw_LoRaModule2Add > 0 && S_LoRaSendNoMaster == 2)
							{
								LoRaTxBuf[0] = Pw_LoRaModule2Add; // �豸�ӵ�ַ
								j = 2;
							}
							else if (Pw_LoRaModule3Add > 0 && S_LoRaSendNoMaster == 3)
							{
								LoRaTxBuf[0] = Pw_LoRaModule3Add; // �豸�ӵ�ַ
								j = 3;
							}
							else if (Pw_LoRaModule4Add > 0 && S_LoRaSendNoMaster == 4)
							{
								LoRaTxBuf[0] = Pw_LoRaModule4Add; // �豸�ӵ�ַ
								j = 4;
							}
							else if (Pw_LoRaModule5Add > 0 && S_LoRaSendNoMaster == 5)
							{
								LoRaTxBuf[0] = Pw_LoRaModule5Add; // �豸�ӵ�ַ
								j = 5;
							}
							else if (Pw_LoRaModule6Add > 0 && S_LoRaSendNoMaster == 6)
							{
								LoRaTxBuf[0] = Pw_LoRaModule6Add; // �豸�ӵ�ַ
								j = 6;
							}
							else if (Pw_LoRaModule7Add > 0 && S_LoRaSendNoMaster == 7)
							{
								LoRaTxBuf[0] = Pw_LoRaModule7Add; // �豸�ӵ�ַ
								j = 7;
							}
							else if (Pw_LoRaModule8Add > 0 && S_LoRaSendNoMaster == 8)
							{
								LoRaTxBuf[0] = Pw_LoRaModule8Add; // �豸�ӵ�ַ
								j = 8;
							}

							else
								j = 0;

							if (j > 0) // LORA ������ģ��
									   //|| B_LoRaSendMaster==2)			//ZCL 2019.1.24 =2�������ͣ�����2ֻ�ǲ��Ͻṹ��ʵ��û��ʹ��
							{
								// LoRaTxBuf[0]=Pw_EquipmentNo+S_LoRaSendNoMaster;
								LoRaTxBuf[1] = 73;	 // ������ 73=0x49
								LoRaTxBuf[2] = 0x00; // ��ʼ��ַ����λ�� 0x7540:30016: w_dspParLst[16]	//.2 A�����
								LoRaTxBuf[3] = 0x00; // ��ʼ��ַ����λ��
								// LoRaTxBuf[4]=0x00;						// ��ȡ��������λ��
								LoRaTxBuf[4] = 0;  // ��ȡ��������λ��  ZCL 2020.9.30  ����������ֽ�  ���͸�����ѹ����ӻ�
								LoRaTxBuf[5] = 40; // ��ȡ��������λ��  ZCL 2019.9.11 10�ĳ�16�����������������ֵ  24��
												   // ZCL 2022.3.10 24������������3���ֵ�����ʱ�䡣�ĳ�30����Ԥ��3��
								Lw_LoRaChkSum = CRC16(LoRaTxBuf, 6);
								LoRaTxBuf[6] = Lw_LoRaChkSum >> 8; // /256
								LoRaTxBuf[7] = Lw_LoRaChkSum;	   // ��λ�ֽ�
								Cw_LoRaTxMax = 8;
								Cw_LoRaTx = 0;

								B_LoRaSendMaster = 12; //=12 ��������
								B_LoRaRCVCommandOk = 0;
							}
						}

						// 06ָ�� �޸Ĳ���
						else if (F_ModeParLora == 1)
						{
							if (w_ZhuanFaAdd < 40)
							{
								LoRaTxBuf[0] = Pw_LoRaModule1Add; // ��1
								LoRaTxBuf[2] = w_ZhuanFaAdd >> 8; // ��ʼ��ַ����λ�� 		// /256;
								LoRaTxBuf[3] = w_ZhuanFaAdd;	  // ��ʼ��ַ����λ��
							}

							if (w_ZhuanFaAdd >= 40 && w_ZhuanFaAdd < 80)
							{
								LoRaTxBuf[0] = Pw_LoRaModule2Add;		 // ��2
								LoRaTxBuf[2] = (w_ZhuanFaAdd - 40) >> 8; // ��ʼ��ַ����λ��         // /256;
								LoRaTxBuf[3] = w_ZhuanFaAdd - 40;		 // ��ʼ��ַ����λ��
							}
							if (w_ZhuanFaAdd >= 80 && w_ZhuanFaAdd < 120)
							{
								LoRaTxBuf[0] = Pw_LoRaModule3Add;		 // ??3
								LoRaTxBuf[2] = (w_ZhuanFaAdd - 80) >> 8; // ��ʼ��ַ����λ��         // /256;
								LoRaTxBuf[3] = w_ZhuanFaAdd - 80;		 // ��ʼ��ַ����λ��
							}
							if (w_ZhuanFaAdd >= 120 && w_ZhuanFaAdd < 160)
							{
								LoRaTxBuf[0] = Pw_LoRaModule4Add;		  // ??4
								LoRaTxBuf[2] = (w_ZhuanFaAdd - 120) >> 8; // ��ʼ��ַ����λ��         // /256;
								LoRaTxBuf[3] = w_ZhuanFaAdd - 120;		  // ��ʼ��ַ����λ��
							}
							if (w_ZhuanFaAdd >= 160 && w_ZhuanFaAdd < 200)
							{
								LoRaTxBuf[0] = Pw_LoRaModule5Add;		  // ??5
								LoRaTxBuf[2] = (w_ZhuanFaAdd - 160) >> 8; // ��ʼ��ַ����λ��         // /256;
								LoRaTxBuf[3] = w_ZhuanFaAdd - 160;		  // ��ʼ��ַ����λ��
							}
							if (w_ZhuanFaAdd >= 200 && w_ZhuanFaAdd < 240)
							{
								LoRaTxBuf[0] = Pw_LoRaModule6Add;		  // ??6
								LoRaTxBuf[2] = (w_ZhuanFaAdd - 200) >> 8; // ��ʼ��ַ����λ��         // /256;
								LoRaTxBuf[3] = w_ZhuanFaAdd - 200;		  // ��ʼ��ַ����λ��
							}

							if (w_ZhuanFaAdd >= 240 && w_ZhuanFaAdd < 280)
							{
								LoRaTxBuf[0] = Pw_LoRaModule7Add;		  // ��7			YLS 2023.10.24�޸ĵ�ַ����
								LoRaTxBuf[2] = (w_ZhuanFaAdd - 240) >> 8; // ��ʼ��ַ����λ�� 		// /256;
								LoRaTxBuf[3] = (w_ZhuanFaAdd - 240);	  // ��ʼ��ַ����λ��
							}
							if (w_ZhuanFaAdd >= 280 && w_ZhuanFaAdd < 320)
							{
								LoRaTxBuf[0] = Pw_LoRaModule8Add;		  // ��8			YLS 2023.10.24�޸ĵ�ַ����
								LoRaTxBuf[2] = (w_ZhuanFaAdd - 280) >> 8; // ��ʼ��ַ����λ�� 		// /256;
								LoRaTxBuf[3] = (w_ZhuanFaAdd - 280);	  // ��ʼ��ַ����λ��
							}

							LoRaTxBuf[1] = 6; // ������ 73=0x49

							LoRaTxBuf[4] = w_ZhuanFaData >> 8; // ���ݣ���λ�� 		// /256;
							LoRaTxBuf[5] = w_ZhuanFaData;	   // ���ݣ���λ��

							Lw_LoRaChkSum = CRC16(LoRaTxBuf, 6);
							LoRaTxBuf[6] = Lw_LoRaChkSum >> 8; // /256
							LoRaTxBuf[7] = Lw_LoRaChkSum;	   // ��λ�ֽ�
							Cw_LoRaTxMax = 8;
							Cw_LoRaTx = 0;

							B_LoRaSendMaster = 12; //=12 ��������
							B_LoRaRCVCommandOk = 0;

							F_ModeParLora = 0; // ZCL 2023.2.10  ���Ԥ�ñ�־
						}
						// ��ĳһ̨�ӻ��Ĳ�����03ָ�
						// ��ַ[10000,19999)��1#��[20000,29999)��2#��[30000,39999)��3#��[40000,49999)��4#��[50000,59999)��5#
						else if (F_ModeParLora == 2)
						{
							if (w_ZhuanFaAdd >= 10000 && w_ZhuanFaAdd < 19999)
							{
								LoRaTxBuf[0] = Pw_LoRaModule1Add;			// ��1
								LoRaTxBuf[2] = (w_ZhuanFaAdd - 10000) >> 8; // ��ʼ��ַ����λ�� 		// /256;
								LoRaTxBuf[3] = (w_ZhuanFaAdd - 10000);		// ��ʼ��ַ����λ��
							}
							if (w_ZhuanFaAdd >= 20000 && w_ZhuanFaAdd < 29999)
							{
								LoRaTxBuf[0] = Pw_LoRaModule2Add;			// ��2
								LoRaTxBuf[2] = (w_ZhuanFaAdd - 20000) >> 8; // ��ʼ��ַ����λ��         // /256;
								LoRaTxBuf[3] = (w_ZhuanFaAdd - 20000);		// ��ʼ��ַ����λ��
							}
							if (w_ZhuanFaAdd >= 30000 && w_ZhuanFaAdd < 39999)
							{
								LoRaTxBuf[0] = Pw_LoRaModule3Add;			// ��3
								LoRaTxBuf[2] = (w_ZhuanFaAdd - 30000) >> 8; // ��ʼ��ַ����λ��         // /256;
								LoRaTxBuf[3] = (w_ZhuanFaAdd - 30000);		// ��ʼ��ַ����λ��
							}
							if (w_ZhuanFaAdd >= 40000 && w_ZhuanFaAdd < 49999)
							{
								LoRaTxBuf[0] = Pw_LoRaModule4Add;			// ��4
								LoRaTxBuf[2] = (w_ZhuanFaAdd - 40000) >> 8; // ��ʼ��ַ����λ��         // /256;
								LoRaTxBuf[3] = (w_ZhuanFaAdd - 40000);		// ��ʼ��ַ����λ��
							}
							if (w_ZhuanFaAdd >= 50000 && w_ZhuanFaAdd < 59999)
							{
								LoRaTxBuf[0] = Pw_LoRaModule5Add;			// ��5
								LoRaTxBuf[2] = (w_ZhuanFaAdd - 50000) >> 8; // ��ʼ��ַ����λ��         // /256;
								LoRaTxBuf[3] = (w_ZhuanFaAdd - 50000);		// ��ʼ��ַ����λ��
							}

							LoRaTxBuf[1] = 3;				   // ������ 03
															   //						LoRaTxBuf[2] = w_ZhuanFaAdd  >> 8; // ��ʼ��ַ����λ��         // /256;
															   //						LoRaTxBuf[3] = w_ZhuanFaAdd ;		// ��ʼ��ַ����λ��
							LoRaTxBuf[4] = w_ZhuanFaData >> 8; // ���ݣ���λ�� 		// /256;
							LoRaTxBuf[5] = w_ZhuanFaData;	   // ���ݣ���λ��

							Lw_LoRaChkSum = CRC16(LoRaTxBuf, 6);
							LoRaTxBuf[6] = Lw_LoRaChkSum >> 8; // /256
							LoRaTxBuf[7] = Lw_LoRaChkSum;	   // ��λ�ֽ�
							Cw_LoRaTxMax = 8;
							Cw_LoRaTx = 0;

							B_LoRaSendMaster = 12; //=12 ��������
							B_LoRaRCVCommandOk = 0;

							// F_ModeParLora = 0; // ZCL 2023.2.10  ���Ԥ�ñ�־
						}
						// дĳһ̨�ӻ��Ĳ�����06ָ�
						// ��ַ[10000,19999)��1#��[20000,29999)��2#��[30000,39999)��3#��[40000,49999)��4#��[50000,59999)��5#
						else if (F_ModeParLora == 3)
						{
							if (w_ZhuanFaAdd >= 10000 && w_ZhuanFaAdd < 19999)
							{
								LoRaTxBuf[0] = Pw_LoRaModule1Add; // ��1
							}
							if (w_ZhuanFaAdd >= 20000 && w_ZhuanFaAdd < 29999)
							{
								LoRaTxBuf[0] = Pw_LoRaModule2Add; // ��2
							}
							if (w_ZhuanFaAdd >= 30000 && w_ZhuanFaAdd < 39999)
							{
								LoRaTxBuf[0] = Pw_LoRaModule3Add; // ��3
							}
							if (w_ZhuanFaAdd >= 40000 && w_ZhuanFaAdd < 49999)
							{
								LoRaTxBuf[0] = Pw_LoRaModule4Add; // ��4
							}
							if (w_ZhuanFaAdd >= 50000 && w_ZhuanFaAdd < 59999)
							{
								LoRaTxBuf[0] = Pw_LoRaModule5Add; // ��5
							}

							LoRaTxBuf[1] = 6; // ������ 06

							LoRaTxBuf[2] = (w_ZhuanFaAdd) >> 8; // ��ʼ��ַ����λ�� 		// /256;
							LoRaTxBuf[3] = (w_ZhuanFaAdd);		// ��ʼ��ַ����λ��

							LoRaTxBuf[4] = w_ZhuanFaData >> 8; // ���ݣ���λ�� 		// /256;
							LoRaTxBuf[5] = w_ZhuanFaData;	   // ���ݣ���λ��

							Lw_LoRaChkSum = CRC16(LoRaTxBuf, 6);
							LoRaTxBuf[6] = Lw_LoRaChkSum >> 8; // /256
							LoRaTxBuf[7] = Lw_LoRaChkSum;	   // ��λ�ֽ�
							Cw_LoRaTxMax = 8;
							Cw_LoRaTx = 0;

							B_LoRaSendMaster = 12; //=12 ��������
							B_LoRaRCVCommandOk = 0;

							F_ModeParLora = 0; // ZCL 2023.2.10  ���Ԥ�ñ�־
						}
					}
				}
			}

			//----------------------------------------------------------------
			// 6. �жϳ�ʱ
			// 6.1�жϳ�ʱ����ʱͨѶ���� 4000MS   ����ģ��1ͨѶʧ�� // 6000
			if (Pw_LoRaModule1Add > 0 && (!B_LoRaErrWithModule1 || F_ModeParLora == 2)) // ��������м�ʱ��ʱ�䵽����
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
						F_LoRa1_Comm_Fault = 1;	  // YLS 2025.01.27 ��ͨѶ���ϱ�־

						F_ModeParLora = 0; // ��ʱ���ָ���73�������ѯ
					}
				}
			}
			else if (Pw_LoRaModule1Add == 0)
			{
				B_LoRaErrWithModule1 = 0; //
				F_LoRa1_Comm_Fault = 0;	  // YLS 2025.01.27 ��ͨѶ���ϱ�־

				C_LoRaCount1Err = 0;
			}

			//----------------------------------------------------------------
			// 6.2�жϳ�ʱ����ʱͨѶ���� 4000MS	����ģ��2ͨѶʧ��
			if (Pw_LoRaModule2Add > 0 && (!B_LoRaErrWithModule2 || F_ModeParLora == 2)) // ��������м�ʱ��ʱ�䵽����
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
						F_LoRa2_Comm_Fault = 1;	  // YLS 2025.01.27 ��ͨѶ���ϱ�־
						F_ModeParLora = 0;		  // ��ʱ���ָ���73�������ѯ
					}
				}
			}
			else if (Pw_LoRaModule2Add == 0)
			{
				B_LoRaErrWithModule2 = 0; //
				F_LoRa2_Comm_Fault = 0;	  // YLS 2025.01.27 ��ͨѶ���ϱ�־
				C_LoRaCount2Err = 0;
			}

			//----------------------------------------------------------------
			// 6.3�жϳ�ʱ����ʱͨѶ���� 4000MS	����ģ��3ͨѶʧ��  ZCL 2018.12.15
			if (Pw_LoRaModule3Add > 0 && (!B_LoRaErrWithModule3 || F_ModeParLora == 2)) // ��������м�ʱ��ʱ�䵽����
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
						F_LoRa3_Comm_Fault = 1;	  // YLS 2025.01.27 ��ͨѶ���ϱ�־

						F_ModeParLora = 0; // ��ʱ���ָ���73�������ѯ
					}
				}
			}
			else if (Pw_LoRaModule3Add == 0)
			{
				B_LoRaErrWithModule3 = 0; //
				F_LoRa3_Comm_Fault = 0;	  // YLS 2025.01.27 ��ͨѶ���ϱ�־
				C_LoRaCount3Err = 0;
			}

			//----------------------------------------------------------------
			// 6.4�жϳ�ʱ����ʱͨѶ���� 4000MS	����ģ��4ͨѶʧ��   ZCL 2018.12.15
			if (Pw_LoRaModule4Add > 0 && (!B_LoRaErrWithModule4 || F_ModeParLora == 2)) // ��������м�ʱ��ʱ�䵽����
			{
				if (T_LoRaCount4Err != SClk10Ms)
				{
					T_LoRaCount4Err = SClk10Ms; //
					C_LoRaCount4Err++;
					if (C_LoRaCount4Err > 1000) //
					{
						T_LoRaCount4Err = 100;
						C_LoRaCount4Err = 0;
						B_LoRaErrWithModule4 = 1;
						F_LoRa4_Comm_Fault = 1; // YLS 2025.01.27 ��ͨѶ���ϱ�־
						F_ModeParLora = 0;		// ��ʱ���ָ���73�������ѯ
					}
				}
			}
			else if (Pw_LoRaModule4Add == 0)
			{
				B_LoRaErrWithModule4 = 0; //
				F_LoRa4_Comm_Fault = 0;	  // YLS 2025.01.27 ��ͨѶ���ϱ�־
				C_LoRaCount4Err = 0;
			}

			//----------------------------------------------------------------
			// 6.5�жϳ�ʱ����ʱͨѶ���� 4000MS	����ģ��5ͨѶʧ��   ZCL 2018.12.15  2019.10.18
			if (Pw_LoRaModule5Add > 0 && (!B_LoRaErrWithModule5 || F_ModeParLora == 2)) // ��������м�ʱ��ʱ�䵽����
			{
				if (T_LoRaCount5Err != SClk10Ms)
				{
					T_LoRaCount5Err = SClk10Ms; //
					C_LoRaCount5Err++;
					if (C_LoRaCount5Err > 1000) //
					{
						T_LoRaCount5Err = 100;
						C_LoRaCount5Err = 0;
						B_LoRaErrWithModule5 = 1; //
						F_LoRa5_Comm_Fault = 1;	  // YLS 2025.01.27 ��ͨѶ���ϱ�־
						F_ModeParLora = 0;		  // ��ʱ���ָ���73�������ѯ
					}
				}
			}
			else if (Pw_LoRaModule5Add == 0)
			{
				B_LoRaErrWithModule5 = 0; //
				F_LoRa5_Comm_Fault = 0;	  // YLS 2025.01.27 ��ͨѶ���ϱ�־
				C_LoRaCount5Err = 0;
			}

			//----------------------------------------------------------------
			// 6.6�жϳ�ʱ����ʱͨѶ���� 4000MS	����ģ��6ͨѶʧ��   ZCL 2018.12.15  2019.10.18
			if (Pw_LoRaModule6Add > 0 && (!B_LoRaErrWithModule6 || F_ModeParLora == 2)) // ��������м�ʱ��ʱ�䵽����
			{
				if (T_LoRaCount6Err != SClk10Ms)
				{
					T_LoRaCount6Err = SClk10Ms; //
					C_LoRaCount6Err++;
					if (C_LoRaCount6Err > 1000) //
					{
						T_LoRaCount6Err = 100;
						C_LoRaCount6Err = 0;
						B_LoRaErrWithModule6 = 1; //
						F_LoRa6_Comm_Fault = 1;	  // YLS 2025.01.27 ��ͨѶ���ϱ�־
						F_ModeParLora = 0;		  // ��ʱ���ָ���73�������ѯ
					}
				}
			}
			else if (Pw_LoRaModule6Add == 0)
			{
				B_LoRaErrWithModule6 = 0; //
				F_LoRa6_Comm_Fault = 0;	  // YLS 2025.01.27 ��ͨѶ���ϱ�־
				C_LoRaCount6Err = 0;
			}

			//----------------------------------------------------------------
			// 6.7�жϳ�ʱ����ʱͨѶ���� 4000MS	����ģ��7ͨѶʧ��   ZCL 2018.12.15  2019.10.18
			if (Pw_LoRaModule7Add > 0 && (!B_LoRaErrWithModule7 || F_ModeParLora == 2)) // ��������м�ʱ��ʱ�䵽����
			{
				if (T_LoRaCount7Err != SClk10Ms)
				{
					T_LoRaCount7Err = SClk10Ms; //
					C_LoRaCount7Err++;
					if (C_LoRaCount7Err > 1000) //
					{
						T_LoRaCount7Err = 100;
						C_LoRaCount7Err = 0;
						B_LoRaErrWithModule7 = 1; //
						F_LoRa7_Comm_Fault = 1;	  // YLS 2025.01.27 ��ͨѶ���ϱ�־
						F_ModeParLora = 0;		  // ��ʱ���ָ���73�������ѯ
					}
				}
			}
			else if (Pw_LoRaModule7Add == 0)
			{
				B_LoRaErrWithModule7 = 0; //
				F_LoRa7_Comm_Fault = 0;	  // YLS 2025.01.27 ��ͨѶ���ϱ�־
				C_LoRaCount7Err = 0;
			}

			//----------------------------------------------------------------
			// 6.8�жϳ�ʱ����ʱͨѶ���� 4000MS	����ģ��8ͨѶʧ��   ZCL 2018.12.15  2019.10.18
			if (Pw_LoRaModule8Add > 0 && (!B_LoRaErrWithModule8 || F_ModeParLora == 2)) // ��������м�ʱ��ʱ�䵽����
			{
				if (T_LoRaCount8Err != SClk10Ms)
				{
					T_LoRaCount8Err = SClk10Ms; //
					C_LoRaCount8Err++;
					if (C_LoRaCount8Err > 1000) //
					{
						T_LoRaCount8Err = 100;
						C_LoRaCount8Err = 0;
						B_LoRaErrWithModule8 = 1; //
						F_LoRa8_Comm_Fault = 1;	  // YLS 2025.01.27 ��ͨѶ���ϱ�־
						F_ModeParLora = 0;		  // ��ʱ���ָ���73�������ѯ
					}
				}
			}
			else if (Pw_LoRaModule8Add == 0)
			{
				B_LoRaErrWithModule8 = 0; //
				F_LoRa8_Comm_Fault = 0;	  // YLS 2025.01.27 ��ͨѶ���ϱ�־
				C_LoRaCount8Err = 0;
			}

			//----------------------------------------------------------------
			// 6.9�жϳ�ʱ����ʱͨѶ���� 4000MS	����ģ��9ͨѶʧ��   ZCL 2022.3.11
			if (w_LoRaLeiDaModuleAdd > 0 && (!B_LoRaErrWithModule9 || F_ModeParLora == 2)) // ��������м�ʱ��ʱ�䵽����
			{
				if (T_LoRaCount9Err != SClk10Ms)
				{
					T_LoRaCount9Err = SClk10Ms; //
					C_LoRaCount9Err++;
					if (C_LoRaCount9Err > 1000) //
					{
						T_LoRaCount9Err = 100;
						C_LoRaCount9Err = 0;
						B_LoRaErrWithModule9 = 1; //
					}
				}
			}
			else if (w_LoRaLeiDaModuleAdd == 0)
			{
				B_LoRaErrWithModule9 = 0; //
				C_LoRaCount9Err = 0;
			}
		}
	}
	else // ��Ƶ���
	{
		if (i > 0) // ��������ģ����߱�Ƶ���������Ʋ��ܽ��뷢�ͳ���
		{
			if (B_LoRaSendMaster == 1) //&& B_LoRaRCVCommandOk
			{
				if (T_LoRaDelaySend != SClk10Ms)
				{
					T_LoRaDelaySend = SClk10Ms; //
					C_LoRaDelaySend++;
					if (C_LoRaDelaySend > 20) // 210���� ZCL 2019.10.17	��ʹ����������ģʽ��2�������ˣ� �����
					{
						T_LoRaDelaySend = 100;
						C_LoRaDelaySend = 0;

						if (B_LoraSendWrite == 0) // ZCL 2021.11.17
						{
							// 1. ���Ʒ��Ͳ���
							S_LoRaSendNoMaster++;

							// S_LoRaSendNoMaster=1;  //ZCL123		����ʱ�ã�ֻ����1����ַ�ģ�����ɾ�����ָ�++

							if (S_LoRaSendNoMaster > 4)
								S_LoRaSendNoMaster = 1; // COM_UNIT_NUM

							// 2.����˳����
							if (Pw_LoRaModule1Add > 0 && S_LoRaSendNoMaster == 1)
							{
								LoRaTxBuf[0] = Pw_LoRaModule1Add; // �豸�ӵ�ַ
								j = 1;
							}
							else if (Pw_LoRaModule2Add > 0 && S_LoRaSendNoMaster == 2)
							{
								LoRaTxBuf[0] = Pw_LoRaModule2Add; // �豸�ӵ�ַ
								j = 2;
							}
							else if (Pw_LoRaModule3Add > 0 && S_LoRaSendNoMaster == 3)
							{
								LoRaTxBuf[0] = Pw_LoRaModule3Add; // �豸�ӵ�ַ
								j = 3;
							}
							else if (Pw_LoRaModule4Add > 0 && S_LoRaSendNoMaster == 4)
							{
								LoRaTxBuf[0] = Pw_LoRaModule4Add; // �豸�ӵ�ַ
								j = 4;
							}

							else
								j = 0;

							if (j > 0) // LORA ������ģ��
									   //|| B_LoRaSendMaster==2)			//ZCL 2019.1.24 =2�������ͣ�����2ֻ�ǲ��Ͻṹ��ʵ��û��ʹ��
							{
								// LoRaTxBuf[0]=Pw_EquipmentNo+S_LoRaSendNoMaster;
								LoRaTxBuf[1] = 73;	 // ������ 73=0x49
								LoRaTxBuf[2] = 0x00; // ��ʼ��ַ����λ�� 0x7540:30016: w_dspParLst[16]	//.2 A�����
								LoRaTxBuf[3] = 0x00; // ��ʼ��ַ����λ��
													 // LoRaTxBuf[4]=0x00;						// ��ȡ��������λ��
								LoRaTxBuf[4] = 0x00; // ��ȡ��������λ��
								LoRaTxBuf[5] = 16;	 // ��ȡ��������λ��  ZCL 2019.9.11 10�ĳ�16�����������������ֵ  2021.11.10 25��
													 // ZCL 2022.3.10 24������������3���ֵ�����ʱ�䡣�ĳ�30����Ԥ��3��
								Lw_LoRaChkSum = CRC16(LoRaTxBuf, 6);
								LoRaTxBuf[6] = Lw_LoRaChkSum >> 8; // /256
								LoRaTxBuf[7] = Lw_LoRaChkSum;	   // ��λ�ֽ�
								Cw_LoRaTxMax = 8;
								Cw_LoRaTx = 0;

								B_LoRaSendMaster = 12; //=12 ��������
								B_LoRaRCVCommandOk = 0;

								// ZCL 2018.12.15 ����
								// Radio->SetTxPacket( LoRaTxBuf, Cw_LoRaTxMax );   //RFLR_STATE_TX_INIT
							}
						}

						else if (B_LoraSendWrite == 1) // ZCL 2021.11.17
						{
							// ZCL 2021.11.18 ȡ������ķ������ݣ��͵�LoRaTxBuf
							for (j = 0; j < B_LoraSendWriteLength; j++) // Rcv0Buffer[5]=����
							{
								LoRaTxBuf[j] = LoRaTxBuf2[j];
							}

							LoRaTxBuf[0] = Pw_LoRaModule1Add; // ZCL ��ʱ ֻ����1����ַ��

							if (LoRaTxBuf[5] <= 10) // ZCL 2021.11.17  ������
							{
								Lw_LoRaChkSum = CRC16(LoRaTxBuf, B_LoraSendWriteLength - 2);
								LoRaTxBuf[B_LoraSendWriteLength - 2] = Lw_LoRaChkSum >> 8; // /256
								LoRaTxBuf[B_LoraSendWriteLength - 1] = Lw_LoRaChkSum;	   // ��λ�ֽ�
								Cw_LoRaTxMax = B_LoraSendWriteLength;
								Cw_LoRaTx = 0;

								B_LoRaSendMaster = 12; //=12 ��������
							}
							else
								; // ZCL 2021.11.17 ������ȡ�� дָ��

							B_LoraSendWrite = 0;

							// ZCL 2018.12.15 ����
							// Radio->SetTxPacket( LoRaTxBuf, Cw_LoRaTxMax );   //RFLR_STATE_TX_INIT
						}
					}
				}
			}

			// B_LoRaSendEnd=S_LoRaSendNoMaster;
			//}

			// �����굫���Ͳ���ȷ��û��B_LoRaRCVCommandOk��־����ʱ������һ��

			// 5.��ʱ����		// ���ͽ���
			// �жϽ�����������ʱ������һ�� 6MS
			// ZCL 2018.12.15 �����ٶ�������ʱ�� 100MS
			if (B_LoRaRCVCommandOk) // ����������ʱ�󣬷�����һ��
			{
				if (T_LoRaCountNormalNext != SClk10Ms)
				{
					T_LoRaCountNormalNext = SClk10Ms; //
					C_LoRaCountNormalNext++;
					if (C_LoRaCountNormalNext > 50) //
					{
						B_LoRaRCVCommandOk = 0;

						T_LoRaCountNormalNext = 100;
						C_LoRaCountNormalNext = 0;

						T_LoRaCount1OvertimeNext = 100;
						C_LoRaCount1OvertimeNext = 0;

						B_LoRaSendMaster = 1; //
					}
				}
			}
			//		}

			//----------------------------------------------------------------
			// 6. �жϳ�ʱ
			// 6.1�жϳ�ʱ����ʱͨѶ���� 4000MS   ����ģ��1ͨѶʧ�� // 6000
			if (Pw_LoRaModule1Add > 0 && !B_LoRaErrWithModule1) // ��������м�ʱ��ʱ�䵽����
			{
				if (T_LoRaCount1Err != SClk10Ms)
				{
					T_LoRaCount1Err = SClk10Ms; //
					C_LoRaCount1Err++;
					if (C_LoRaCount1Err > 600) //
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
					if (C_LoRaCount2Err > 600) //
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
					if (C_LoRaCount3Err > 600) //
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
					if (C_LoRaCount4Err > 600) //
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
	}
}

void LoRaSendToDone(void) // LoRa ���ͣ�������Ƿ����
{
	// 4.0. SX1278 TX����		COM2�и�ֵ�������յ����ݣ���B_LoRaSendMaster=2��Ȼ���������LoRa����

	// ZCL 2019.2.16 ���Դ����յ��󣬷��͵�LORA
	if (B_LoRaSendMaster == 2 || B_LoRaSendMaster == 12 || B_LoRaSendMaster == 22) //	|| B_LoRaSendMaster==3
	{
		LCD_DLY_ms(10); // ZCL123 2018.11.12

		//=12 ��������ָ��
		if (B_LoRaSendMaster == 12)
		{
			Radio->SetTxPacket(LoRaTxBuf, Cw_LoRaTxMax); // RFLR_STATE_TX_INIT
		}

		//=22	�ӻ��յ������ͷ�������
		else if (B_LoRaSendMaster == 22)
		{
			Radio->SetTxPacket(LoRaTxBuf, Cw_LoRaTxMax); // RFLR_STATE_TX_INIT
		}

		//=2 ����ת��
		else if (B_LoRaSendMaster == 2)
		{
			// ZCL 2018.11.12
			Cw_LoRaTxMax = Cw_BakRcv2;
			Radio->SetTxPacket(LoRaTxBuf, Cw_LoRaTxMax); // RFLR_STATE_TX_INIT
		}

		// ���͵�����1�����м�� YLS 2023.03.23
		if (w_ZhouShanProtocol_bit10) // ����1����LoRa��������շ���=1����أ�=0�������
		{
			// if (S_M35 == 0x1B)
			Com1_sendData(LoRaTxBuf, Cw_LoRaTxMax);
		}

		LCD_DLY_ms(10); // ZCL123 2018.11.12
		B_LoRaSendMaster = 3;
		// ZCL 2019.2.16 ������ôд�������� Radio->Process( ) ���ж��Ƿ�����
		// Radio->StartRx( );   //RFLR_STATE_RX_INIT
	}

	// ZCL 2019.2.16
	//=3 ���뷢�ͺ󣬵ȴ�LORA������� RF_TX_DONE�� �����꣬����=4����ʱ��������ͣ��ӻ�=1���ȴ�����
	if (B_LoRaSendMaster == 3)
	{
		while (Radio->Process() == RF_TX_DONE)
		{
			Radio->StartRx(); // RFLR_STATE_RX_INIT

			// ZCL  2019.2.16 ���������꣬�ȴ����� B_LoRaSendMaster=4
			if (Pw_LoRaMasterSlaveSel == 1)
				B_LoRaSendMaster = 4;
			else
				B_LoRaSendMaster = 1;

			LED0_TOGGLE; // ZCL 2019.10.15 �������ָʾ���ĳɷ������ָʾ��
		}
	}

	// 4.1 LORA���������꣬�������״̬
	// 4.2 LORA���ͣ���û�м�⵽RF_TX_DONE����ʱ�˳���B_LoRaSendMaster=3;
	if (B_LoRaSendMaster == 53) //=53��ʱ��ִ�� ZCL 2019.2.18
	{
		// 4.1 LORA ����Ƿ����꣬������������״̬
		while (Radio->Process() == RF_TX_DONE)
		{
			printf("RF_LoRa_TX_OK! \n"); // ZCL123 2019.1.24 ����Ӧ��ʱɾ����

			LED0_TOGGLE;

			B_LoRaSendMaster = 4; // B_LoRaSendMaster=4��������ɡ���ʱ����������

			// ZCL 2018.11.13 ���������������
			Radio->StartRx(); // RFLR_STATE_RX_INIT
			LCD_DLY_ms(50);	  // ZCL 2018.11.12

			B_LoRaRcv = 0;

			T_LoRaSendOverTime = 100;
			C_LoRaSendOverTime = 0;
			C_LoRaCount1OvertimeNext = 0;
		}

		// 4.2 LORA����(B_LoRaSendMaster==2)����û�м�⵽RF_TX_DONE����ʱ�˳���B_LoRaSendMaster=3;
		// û�� while(Radio->Process( ) == RF_TX_DONE )	��������ʱ
		if (T_LoRaSendOverTime != SClk10Ms) //
		{
			T_LoRaSendOverTime = SClk10Ms; //
			C_LoRaSendOverTime++;
			if (C_LoRaSendOverTime > 400) //
			{
				T_LoRaSendOverTime = 100;
				C_LoRaSendOverTime = 0;
				C_LoRaCount1OvertimeNext = 0;

				// printf("RF_LoRa_TX_OverTime \n");
				printf(" TX OverTime \n"); // ZCL123 2019.1.24 ����Ӧ��ʱɾ����

				B_LoRaSendMaster = 4; // B_LoRaSendMaster=4��������ɡ���ʱ����������

				LCD_DLY_ms(10); // ZCL 2018.11.12
				// ZCL 2018.11.13 ���������������
				Radio->StartRx(); // RFLR_STATE_RX_INIT
			}
		}
	}

	// 4.3������ɣ���ʱ�����ٴη��͡�
	if (B_LoRaSendMaster == 54) //=54��ʱ��ִ�� ZCL 2019.2.18
	{
		if (Pw_ScrLoRaTest1 == 1)
		{
			T_LoRaCount1OvertimeNext = 100;
			C_LoRaCount1OvertimeNext = 0;
			B_LoRaSendMaster = 1; // =1����
		}

		else if (T_LoRaCount1OvertimeNext != SClk10Ms)
		{
			T_LoRaCount1OvertimeNext = SClk10Ms; //
			C_LoRaCount1OvertimeNext++;
			if (C_LoRaCount1OvertimeNext > 300) //
			{
				T_LoRaCount1OvertimeNext = 100;
				C_LoRaCount1OvertimeNext = 0;
				B_LoRaSendMaster = 1; // =1����
			}
		}
	}

	// 4.4������ɣ���ʱ�����ٴη��͡�  ZCL 2019.2.18 ��������
	if (B_LoRaSendMaster == 4 || B_LoRaSendMaster == 3)
	{
		if (T_LoRaCount1OvertimeNext != SClk10Ms)
		{
			T_LoRaCount1OvertimeNext = SClk10Ms; //
			C_LoRaCount1OvertimeNext++;
			if (C_LoRaCount1OvertimeNext > 60) // (1)2S���ã� 1.5S����̫���������ν���ģʽ ZCL 2019.10.17;  (2) �ĳ���������ģʽ��600MS���ܺã�
			{
				T_LoRaCount1OvertimeNext = 100;
				C_LoRaCount1OvertimeNext = 0;
				if (B_LoRaSendMaster == 3)
					Radio->StartRx(); // RFLR_STATE_RX_INIT
				B_LoRaSendMaster = 1; // =1����

				C_LoRaCountNormalNext = 0;
			}
		}
	}
}

void LoRaRcvTest(void) // LoRa ���ղ���
{
	u8 i;
	// 2. SX1278 RX����
	// 2.1 ��ʱ
	if (B_LoRaRcv == 1)
	{
		if (T_LoRaRcv != SClk1Ms)
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

			// ��LoRa���յ������ݷ��͵�����2		ZCL 2018.11.13 OK
			for (i = 0; i < BufferSize; i++)
			{
				Txd2Buffer[i] = LoRaRxBuf[i];
			}
			Cw_Txd2Max = BufferSize + 2;
			Cw_Txd2 = 0;
			// RS485_CON=1;
			// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
			USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]); // ZCL 2022.9.19
			USART_ITConfig(USART2, USART_IT_TC, ENABLE);   // ��ʼ����.

			LED0_TOGGLE;
			// ��ջ���
			for (i = 0; i < BUFFER_SIZE; i++)
				LoRaRxBuf[i] = 0;

			// Soft_delay_ms(35);		//ZCL 2018.11.10  2,5,7����   8,20,15��1�Σ�25ż�����Σ�35��30,50,200����
			// LCD_DLY_ms(35);				//ZCL 2018.11.12

			// ZCL 2018.11.13 B_LoRaSendMaster=1,��ʱ�����
			B_LoRaRcv = 1;
		}
	}
}

void Jude_VVVF_Comm(void) // ����Ƶ��ͨ�Ź���
{
	// �ж�1#��Ƶ��ͨѶ����
	if (Pw_LoRaModule1Add > 0 && !B_LoRaErrWithModule1) // �ӻ���ַ��Ϊ0������û��LoRaͨѶ����
	{
		if (T_Judge_No1_VVVF_Comm != SClk10Ms)
		{
			T_Judge_No1_VVVF_Comm = SClk10Ms; //
			C_Judge_No1_VVVF_Comm++;
			if (C_Judge_No1_VVVF_Comm > 1500) // 15s
			{
				T_Judge_No1_VVVF_Comm = 100;
				C_Judge_No1_VVVF_Comm = 0;
				if (Count_Save2_No1_VVVF_Comm == Count_Save1_No1_VVVF_Comm)
				{
					F_No1_VVVFComm_Fault = 1;	//
					F_LoRa1_VVVFComm_Fault = 1; // ��ͨѶ���ϱ�־
				}
				else
				{
					F_No1_VVVFComm_Fault = 0;	//
					F_LoRa1_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
					Count_Save2_No1_VVVF_Comm = Count_Save1_No1_VVVF_Comm;
				}
			}
		}
	}
	else
	{
		F_No1_VVVFComm_Fault = 0;	//
		F_LoRa1_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
		C_Judge_No1_VVVF_Comm = 0;
	}

	// �ж�2#��Ƶ��ͨѶ����
	if (Pw_LoRaModule2Add > 0 && !B_LoRaErrWithModule2) // �ӻ���ַ��Ϊ0������û��LoRaͨѶ����
	{
		if (T_Judge_No2_VVVF_Comm != SClk10Ms)
		{
			T_Judge_No2_VVVF_Comm = SClk10Ms; //
			C_Judge_No2_VVVF_Comm++;
			if (C_Judge_No2_VVVF_Comm > 1500) // 15s
			{
				T_Judge_No2_VVVF_Comm = 100;
				C_Judge_No2_VVVF_Comm = 0;
				if (Count_Save2_No2_VVVF_Comm == Count_Save1_No2_VVVF_Comm)
				{
					F_No2_VVVFComm_Fault = 1;	//
					F_LoRa2_VVVFComm_Fault = 1; // ��ͨѶ���ϱ�־
				}
				else
				{
					F_No2_VVVFComm_Fault = 0;	//
					F_LoRa2_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
					Count_Save2_No2_VVVF_Comm = Count_Save1_No2_VVVF_Comm;
				}
			}
		}
	}
	else
	{
		F_No2_VVVFComm_Fault = 0;	//
		F_LoRa2_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
		C_Judge_No2_VVVF_Comm = 0;
	}

	// �ж�3#��Ƶ��ͨѶ����
	if (Pw_LoRaModule3Add > 0 && !B_LoRaErrWithModule3) // �ӻ���ַ��Ϊ0������û��LoRaͨѶ����
	{
		if (T_Judge_No3_VVVF_Comm != SClk10Ms)
		{
			T_Judge_No3_VVVF_Comm = SClk10Ms; //
			C_Judge_No3_VVVF_Comm++;
			if (C_Judge_No3_VVVF_Comm > 1500) // 15s
			{
				T_Judge_No3_VVVF_Comm = 100;
				C_Judge_No3_VVVF_Comm = 0;
				if (Count_Save2_No3_VVVF_Comm == Count_Save1_No3_VVVF_Comm)
				{
					F_No3_VVVFComm_Fault = 1;	//
					F_LoRa3_VVVFComm_Fault = 1; // ��ͨѶ���ϱ�־
				}
				else
				{
					F_No3_VVVFComm_Fault = 0;	//
					F_LoRa3_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
					Count_Save2_No3_VVVF_Comm = Count_Save1_No3_VVVF_Comm;
				}
			}
		}
	}
	else
	{
		F_No3_VVVFComm_Fault = 0;	//
		F_LoRa3_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
		C_Judge_No3_VVVF_Comm = 0;
	}

	// �ж�4#��Ƶ��ͨѶ����
	if (Pw_LoRaModule4Add > 0 && !B_LoRaErrWithModule4) // �ӻ���ַ��Ϊ0������û��LoRaͨѶ����
	{
		if (T_Judge_No4_VVVF_Comm != SClk10Ms)
		{
			T_Judge_No4_VVVF_Comm = SClk10Ms; //
			C_Judge_No4_VVVF_Comm++;
			if (C_Judge_No4_VVVF_Comm > 1500) // 15s
			{
				T_Judge_No4_VVVF_Comm = 100;
				C_Judge_No4_VVVF_Comm = 0;
				if (Count_Save2_No4_VVVF_Comm == Count_Save1_No4_VVVF_Comm)
				{
					F_No4_VVVFComm_Fault = 1;	//
					F_LoRa4_VVVFComm_Fault = 1; // ��ͨѶ���ϱ�־
				}
				else
				{
					F_No4_VVVFComm_Fault = 0;	//
					F_LoRa4_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
					Count_Save2_No4_VVVF_Comm = Count_Save1_No4_VVVF_Comm;
				}
			}
		}
	}
	else
	{
		F_No4_VVVFComm_Fault = 0;	//
		F_LoRa4_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
		C_Judge_No4_VVVF_Comm = 0;
	}

	// �ж�5#��Ƶ��ͨѶ����
	if (Pw_LoRaModule5Add > 0 && !B_LoRaErrWithModule5) // �ӻ���ַ��Ϊ0������û��LoRaͨѶ����
	{
		if (T_Judge_No5_VVVF_Comm != SClk10Ms)
		{
			T_Judge_No5_VVVF_Comm = SClk10Ms; //
			C_Judge_No5_VVVF_Comm++;
			if (C_Judge_No5_VVVF_Comm > 1500) // 15s
			{
				T_Judge_No5_VVVF_Comm = 100;
				C_Judge_No5_VVVF_Comm = 0;
				if (Count_Save2_No5_VVVF_Comm == Count_Save1_No5_VVVF_Comm)
				{
					F_No5_VVVFComm_Fault = 1;	//
					F_LoRa5_VVVFComm_Fault = 1; // ��ͨѶ���ϱ�־
				}
				else
				{
					F_No5_VVVFComm_Fault = 0;	//
					F_LoRa5_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
					Count_Save2_No5_VVVF_Comm = Count_Save1_No5_VVVF_Comm;
				}
			}
		}
	}
	else
	{
		F_No5_VVVFComm_Fault = 0;	//
		F_LoRa5_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
		C_Judge_No5_VVVF_Comm = 0;
	}

	// �ж�6#��Ƶ��ͨѶ����
	if (Pw_LoRaModule6Add > 0 && !B_LoRaErrWithModule6) // �ӻ���ַ��Ϊ0������û��LoRaͨѶ����
	{
		if (T_Judge_No6_VVVF_Comm != SClk10Ms)
		{
			T_Judge_No6_VVVF_Comm = SClk10Ms; //
			C_Judge_No6_VVVF_Comm++;
			if (C_Judge_No6_VVVF_Comm > 1500) // 15s
			{
				T_Judge_No6_VVVF_Comm = 100;
				C_Judge_No6_VVVF_Comm = 0;
				if (Count_Save2_No6_VVVF_Comm == Count_Save1_No6_VVVF_Comm)
				{
					F_No6_VVVFComm_Fault = 1;	//
					F_LoRa6_VVVFComm_Fault = 1; // ��ͨѶ���ϱ�־
				}
				else
				{
					F_No6_VVVFComm_Fault = 0;	//
					F_LoRa6_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
					Count_Save2_No6_VVVF_Comm = Count_Save1_No6_VVVF_Comm;
				}
			}
		}
	}
	else
	{
		F_No6_VVVFComm_Fault = 0;	//
		F_LoRa6_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
		C_Judge_No6_VVVF_Comm = 0;
	}

	// �ж�7#��Ƶ��ͨѶ����
	if (Pw_LoRaModule7Add > 0 && !B_LoRaErrWithModule7) // �ӻ���ַ��Ϊ0������û��LoRaͨѶ����
	{
		if (T_Judge_No7_VVVF_Comm != SClk10Ms)
		{
			T_Judge_No7_VVVF_Comm = SClk10Ms; //
			C_Judge_No7_VVVF_Comm++;
			if (C_Judge_No7_VVVF_Comm > 1500) // 15s
			{
				T_Judge_No7_VVVF_Comm = 100;
				C_Judge_No7_VVVF_Comm = 0;
				if (Count_Save2_No7_VVVF_Comm == Count_Save1_No7_VVVF_Comm)
				{
					F_No7_VVVFComm_Fault = 1;	//
					F_LoRa7_VVVFComm_Fault = 1; // ��ͨѶ���ϱ�־
				}
				else
				{
					F_No7_VVVFComm_Fault = 0;	//
					F_LoRa7_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
					Count_Save2_No7_VVVF_Comm = Count_Save1_No7_VVVF_Comm;
				}
			}
		}
	}
	else
	{
		F_No7_VVVFComm_Fault = 0;	//
		F_LoRa7_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
		C_Judge_No7_VVVF_Comm = 0;
	}

	// �ж�8#��Ƶ��ͨѶ����
	if (Pw_LoRaModule8Add > 0 && !B_LoRaErrWithModule8) // �ӻ���ַ��Ϊ0������û��LoRaͨѶ����
	{
		if (T_Judge_No8_VVVF_Comm != SClk10Ms)
		{
			T_Judge_No8_VVVF_Comm = SClk10Ms; //
			C_Judge_No8_VVVF_Comm++;
			if (C_Judge_No8_VVVF_Comm > 1500) // 15s
			{
				T_Judge_No8_VVVF_Comm = 100;
				C_Judge_No8_VVVF_Comm = 0;
				if (Count_Save2_No8_VVVF_Comm == Count_Save1_No8_VVVF_Comm)
				{
					F_No8_VVVFComm_Fault = 1;	//
					F_LoRa8_VVVFComm_Fault = 1; // ��ͨѶ���ϱ�־
				}
				else
				{
					F_No8_VVVFComm_Fault = 0;	//
					F_LoRa8_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
					Count_Save2_No8_VVVF_Comm = Count_Save1_No8_VVVF_Comm;
				}
			}
		}
	}
	else
	{
		F_No8_VVVFComm_Fault = 0;	//
		F_LoRa8_VVVFComm_Fault = 0; // ��ͨѶ���ϱ�־
		C_Judge_No8_VVVF_Comm = 0;
	}
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
