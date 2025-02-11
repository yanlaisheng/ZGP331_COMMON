/**
 ******************************************************************************
 * @file    GlobalConst.h
 * @author  ChengLei Zhou  - �ܳ���
 * @version V1.27
 * @date    2014-01-03
 * @brief   ȫ�ֱ�������
 ******************************************************************************
 */

#ifndef __GLOBALCONST_H
#define __GLOBALCONST_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private function prototypes -----------------------------------------------*/
void GPIO_PinReverse(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/* ע��
	1. ���еĲ�������STM32 �� DSP �ģ� DCM200_SCRҲ�Ѿ����Լ��ķ������������ZCL 2015.9.3 ����SCRû�У�

	2. ��ʱע���������������������������ӵı������󣬲��޸���Ӧ�� ARM��DSP����������������
	u16  	Pw_ParLst[500];				// �������б��� �趨
	u16  	w_ParLst[500];				// �������б��� ֻ��

	u16  	Pw_dspParLst[100];		// DSP�������б��� �趨 2015.7.1
	u16  	w_dspParLst[150];			// DSP�������б��� ֻ�� 2015.7.1	ZCL 2018.9.21 ����100̫��
	u16  	w_ScrParLst[30];			// ZCL 2018.8.3 SCR�������  			ZCL 2018.9.21 ����10̫��

	2. ZCL 2019.4.3 ��ע
	DCM200_SCR_B �³����Ѿ������Լ��Ĳ�������w_ScrParLst

	3. ZCL 2019.4.4 ��ע
	DCM200_SCR_E(��LORA GPS GPRS) �³����Ѿ������Լ���FM25L16���������������������

	*/

/*
//ZCL 2015.06.09   �Ѿ��� stm32f10x.h�ж����ˡ�
typedef unsigned int 								u32;
typedef unsigned short int 					u16;
typedef unsigned char  							u8;

typedef const unsigned int 					uc32;
typedef const unsigned short int 		uc16;
typedef const unsigned char 					uc8;

typedef volatile unsigned short int vu16;
	*/

#define TRUE 1
#define FALSE 0
#define NULL 0
#define SPACE 0x20
#define TAB 0x09
#define QKEY 0x71

// #define	CRYSTAL	32

#define CS_FilterNum 32 //

#define FAULTPAGE1 10		 // ע����ϻ���ļ�ֵ����
#define TIMEPAGE 17			 // ע��ʱ�仭��ļ�ֵ����
#define DSP_FAULTPAGE1 213	 // ע����ϻ���ļ�ֵ���� DCM100 ��Ƶģʽ
#define DSP_FAULTPAGE2 255	 // ע����ϻ���ļ�ֵ���� DCM220 ��Ƶģʽ
#define DSP_TIMEPAGE 220	 // ע��ʱ�仭��ļ�ֵ����
#define TWO_WATER_SOURCE 126 // ע��˫ˮԴ����ļ�ֵ����		ZCL 2018.8.21

#define LatLonPage_SQB 2000 // ZCL 2018.10.28 ��γ��ҳ��  218  2023.2.2
#define LatLonPage2_SQB 500 // ZCL 2018.10.28 ��γ��ҳ��2
#define DTUIDPage_SQB 551	// ZCL 2019.4.4 DTU ID ҳ��   519  2023.2.2

#define LatLonPage_BPDJ 218	 // ZCL 2018.10.28 ��γ��ҳ��
#define LatLonPage2_BPDJ 500 // ZCL 2018.10.28 ��γ��ҳ��2
#define DTUIDPage_BPDJ 531	 // ZCL 2019.4.4 DTU ID ҳ�� 519

#define FLASH_PAR_SIZE 500 // NV��������

// С�ܶȣ����ܶ��ͺ�FLASHÿҳ 1K�ֽڣ����ܶ�FLASHÿҳ 2K�ֽ�
/*С������Ʒ FLASH<=32K��			ÿ���洢�黮��Ϊ32��1K�ֽڵ�ҳ��  0x0800 0000-0x0800 7FFF
	��������Ʒ 64K<=FLASH<=128K��ÿ���洢�黮��Ϊ128��1K�ֽڵ�ҳ��0x0800 0000-0x0801 FFFF
	��������Ʒ 256K<=FLASH��		ÿ���洢�黮��Ϊ256��2K�ֽڵ�ҳ�� 0x0800 0000-0x0807 FFFF
	�����Ͳ�Ʒ���Ϊ32K��64λ��ÿ���洢�黮��Ϊ128��2K�ֽڵ�ҳ��0x0800 0000-0x0803 FFFF */

/* //������STM32F10xCBT6 128K FLASH��Ʒ			2014.4.4
#define FLASH_PAR_ADDR		((u32)0x0801F000)		// NV������ַ -0x0801F3FF		124K��λ��
#define LOCKCODE_ADDR			((u32)0x0801F400)		// LOCKCODE������ַ 	125K��λ��
				//0801F400:LOCKCODE��ʼ��ַ		0801F410:������ʼ��ַ */

/* 		//������STM32F10xC8T6 64K FLASH��Ʒ			2014.4.4
#define FLASH_PAR_ADDR		((u32)0x0800F000)		// NV������ַ -0x0800F3FF		60K��λ��
#define LOCKCODE_ADDR			((u32)0x0800F400)		// LOCKCODE������ַ 	61K��λ��
				//0800F400:LOCKCODE��ʼ��ַ		0800F410:������ʼ��ַ
#define SECCOUNT_ADDR					((u32)0x0800F800)		// ���������ַ 	62K��λ�� */

#define LOCKCODE_ADDR ((u32)0x0803C000) // LOCKCODE������ַ 	240K��λ�� -243K
// 0803C000:LOCKCODE��ʼ��ַ		0803C010:������ʼ��ַ

#define FLASH_PAR_ADDR ((u32)0x0803D000) // ZCL 2018.12.10  SCR_E��û��ʹ��
#define DTU_PAR_ADDR ((u32)0x0803D800)	 // ZCL 2018.12.10  SCR_E��û��ʹ��

// ZCL 2018.8.3 ����FLASH��������������ַ
/*��������Ʒ 256K<=FLASH��		ÿ���洢�黮��Ϊ256��2K�ֽڵ�ҳ�� 0x0800 0000-0x0807 FFFF 	*/
// /* 0x0803F000 - 0x0803 FFFF		4K�ռ䣨252K-255K λ�ã������� SCR���趨���� */
// #define SCR_PAR_ADDR						((u32)0x0803F000)				// ��ַ-����ģʽ
// #define SCR_MENUMODE_ADDR			((u32)0x0803F002)				// ��ַ-�˵�ģʽ
// #define SCR_CURRENTBITS_ADDR		((u32)0x0803F004)				// ��ַ-����С��

#define FORCESAVESECS 120 // ǿ�Ʊ���ʱ��
#define NORCVMAXMS 5	  // 20 2007.7.5

#define TXD1_MAX 1536 // ���������
#define RCV1_MAX 2048 // ���ջ��������� //256*8.5

#define TXD2_MAX 1536 // ���������
#define RCV2_MAX 2048 // ���ջ��������� //256*8.5	�����ܴ󣬽��մ���Xmodem���ò����ã�ZCL 2019.3.22
#define TXD3_MAX 512  // ���������
#define RCV3_MAX 1536 // ���ջ��������� //256*8.5	�����ܴ󣬽���GPRS���������ã�ZCL 2019.3.14

#define TXD4_MAX 200 // ���������
#define RCV4_MAX 200 // ���ջ��������� //256*8.5

// ZCL 2018.11.12
// #define BUFFER_SIZE 60 // Define the payload size here

// �ѡ�λ����ַ��λ��š�ת��������ַ��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
// �Ѹõ�ַת����һ��ָ��
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))
// IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr (GPIOA_BASE + 12) // 0x4001080C
#define GPIOB_ODR_Addr (GPIOB_BASE + 12) // 0x40010C0C
#define GPIOC_ODR_Addr (GPIOC_BASE + 12) // 0x4001100C
#define GPIOD_ODR_Addr (GPIOD_BASE + 12) // 0x4001140C
/* #define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C     */

#define GPIOA_IDR_Addr (GPIOA_BASE + 8) // 0x40010808
#define GPIOB_IDR_Addr (GPIOB_BASE + 8) // 0x40010C08
#define GPIOC_IDR_Addr (GPIOC_BASE + 8) // 0x40011008
#define GPIOD_IDR_Addr (GPIOD_BASE + 8) // 0x40011408
/* #define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08  */

// IO�ڲ���,ֻ�Ե�һ��IO��!
// ȷ��n��ֵС��16!
#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr, n) // ���
#define PAin(n) BIT_ADDR(GPIOA_IDR_Addr, n)	 // ����

#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr, n) // ���
#define PBin(n) BIT_ADDR(GPIOB_IDR_Addr, n)	 // ����

#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr, n) // ���
#define PCin(n) BIT_ADDR(GPIOC_IDR_Addr, n)	 // ����

#define PDout(n) BIT_ADDR(GPIOD_ODR_Addr, n) // ���
#define PDin(n) BIT_ADDR(GPIOD_IDR_Addr, n)	 // ����

/* #define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //���
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //���
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //���
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //���� */

#define RS485_CON PDout(3) // RS485���ƶ�		//ZCL 2018.12.13  ������ʱ��

// ZCL 2020.3.26 ����GPRS�ܽŵ�ע��
// 1. ���ƽ�
#define PWR_KEY PBout(12)								// ģ�鿪�����߹ػ��ţ�=1���������ܳ����ͣ�ģ�鿪��������ֵ500ms��=0��ģ��ػ�
#define RESET_4G PBout(13)								// ��λ�ţ�=1���������ܳ����ͣ�ģ�鸴λ������ֵ200ms��Ȼ����=0
#define RESET_4G_ON GPIO_SetBits(GPIOB, GPIO_Pin_13)	// ��ʼ��λ
#define RESET_4G_OFF GPIO_ResetBits(GPIOB, GPIO_Pin_13) // ֹͣ��λ
// 2. ����š����ֿ��Բ�ʹ��
// DTE ׼������(��DTR�ܽ����Ϳ��Ի���ģ��.DTR�ܽ�����20ms��,���ڱ�����)
// #define		DTR						PBout(3)		//ZCL 2018.12.3 ������ʱ��		//ZCL 2020.3.26 ����Ž�����ȡ��
// #define		RING					PBin(4)			//ZCL 2018.12.3 ������ʱ��		//ZCL 2020.3.26 ����Ž�����ȡ��
#define STATUS_4G PBin(8) // ״̬����� =1,����=0,��			//ZCL 2020.3.26 ����ų���ʹ���ˣ�

// 3. �����3��ָʾ�ƣ���Ϊ�Ʋ�����ȥ����NET�ƣ�ZCL 2020.3.26
#define PWR_LIGHT PCout(15) // ZCL 2018.12.3
#define COM2_DATA PCout(12) //=1,��=0,��	���ǣ�Data ָʾ�� ������//ZCL 2018.12.3  ��COM2�ڵ�USART2�� ���Ѿ������ϵ�������û����ڽ������ݽ�����
// NET  ������һ�������ڵĹܽŴ����� //ZCL 2020.3.26

// 4. ����4Gģ��ĵ�Դ
#define POWERON_4G GPIO_ResetBits(GPIOC, GPIO_Pin_0) // 4G�ϵ�
#define POWEROFF_4G GPIO_SetBits(GPIOC, GPIO_Pin_0)	 // 4G�ϵ�

#define PWR_STA PCin(15)	  // ZCL 2013.9.2 ��������ã�������������ʹ�ã�����//ZCL 2018.12.3
#define COM2_DATASTA PCin(12) // ZCL 2013.9.2 ��������ã�������������ʹ�ã�����//ZCL 2018.12.3

// ZCL 2018.12.10  ָʾ�Ƶķ�ת
#define PWR_TOGGLE GPIO_PinReverse(GPIOC, GPIO_Pin_15)		 // ZCL 2018.12.10
#define COM2_DATA_TOGGLE GPIO_PinReverse(GPIOC, GPIO_Pin_12) // ZCL 2018.12.10

// ZCL 2018.12.13 ��Ϊָʾ�Ʋ����ã�����NET��PA11�����PD7�������á� H2
// ʵ��NET����ָʾLORA -->LED0_TOGGLE PA11
#define NET PAout(11)								   // PAout(11)		//=1,��=0,��		//ZCL 2018.12.3
#define NET_STA PAin(11)							   // PAin(11)		//ZCL 2013.9.2 ��������ã�������������ʹ�ã�����//ZCL 2018.12.3
#define NET_TOGGLE GPIO_PinReverse(GPIOA, GPIO_Pin_11) // GPIO_PinReverse(GPIOA, GPIO_Pin_11)			//ZCL 2018.12.10

// 4. ���ڵ�3�������õ�ָʾ��  ZCL 2020.3.26
// 1. NETLIGHTָʾ�ƣ�û������CPU�ܽţ�M35������ţ�M35��NETLIGHT�ܽţ�
// 2. STATUS_4Gָʾ�ƣ�����CPU�ܽţ�M35�������
// 3. COM3_DATAָʾ�ƣ�����CPU�ܽţ���CPU��������������Ϊû���ˣ����˸������ڵĹܽţ������á�
#define COM3_DATA PDout(5)									//=1,��=0,��		//ZCL 2018.12.3  ������ʱ�� ��COM3�ڵ�USART3�� ��CPU���ڸ�M35����ͨѶ�������������M35��
#define COM3_DATA_TOGGLE GPIO_PinReverse(GPIOD, GPIO_Pin_5) // ZCL 2020.3.26

// #define		COM0_TX				PAout(9)		//�������ԣ�COM1_LED���ģʽ�����״̬���ܶ�ȡ
// #define		COM0_RX				PAin(10)		//״̬�����
// #define		COM1_TX				PBout(10)		//�������ԣ�COM1_LED���ģʽ�����״̬���ܶ�ȡ
// #define		COM1_RX				PAin(3)			//״̬�����		//ZCL 2018.12.3

#define Qb_LcdBacklight PBout(0) //=1����

/* //DO �̵������
#define		Q1				PAout(15)			//=1���պ�

//�̵�����������
#define		Q1_in				PAin(15)		// */

// DI ����������

// GPS�� PPS������ GP_1PPS
#define GP_1PPS PCin(0) // GP_1PPS

// ZCL 2018.7.19  8����������������˳��
#define KEY1 PAin(4) // STOPKEY
#define KEY2 PAin(5) // RUNKEY
// ZCL 2019.4.16 ��ע��DCM200_SCR_E��KEY3��KEY4��KEY5 ��ռ�ã�GetKey()ʹ��ʱҪע�͵���
#define KEY3 PAin(6)   // LOCKKEY
#define KEY4 PAin(7)   // CHECKKEY
#define KEY5 PCin(4)   // SETKEY
#define KEY6 PCin(5)   // OKKEY
#define KEY7 PBin(1)   // RIGHTKEY
#define KEY8 PCin(13)  // DOWNKEY
#define KEY9 PCin(3)   // PBin(8)	//PBin(10) 		//LEFTKEY			ZCL 2018.10.18
#define KEY10 PCin(14) // PCin(12) 		//ESCKEY	ZCL 2018.12.05
#define KEY11 PBin(7)  // PCin(3)	//PCin(11) 		//UPKEY			ZCL 2018.10.18

#define STOPKEY 1
#define RUNKEY 2
#define LOCKKEY 3
#define CHECKKEY 4
#define SETKEY 5
#define OKKEY 6
#define RIGHTKEY 7
#define DOWNKEY 8
#define LEFTKEY 9
#define ESCKEY 10
#define UPKEY 11

/*
//7������
#define		DOWNKEY			11	//8  ZCL 2018.3.31  DOWNKEY�� UPKEY�� ʹ�õ�IO�� ��Ƶ����Ĳ�һ���������޸�
#define		LEFTKEY			5		//9  ZCL 2018.3.31  SETKEY������LEFTKEY���ã���Ϊû��LEFTKEY����û��RIGHTKEY������
#define		UPKEY				8		//11 ZCL 2018.3.31  UPKEY�� DOWNKEY�� ʹ�õ�IO�� ��Ƶ����Ĳ�һ���������޸�
		//��Ƶ��� ����PC13���½�PC11����Ƶ������PC11���½�PC13

//8�����������¼�����
#define		DOWNKEY			11			//8		ZCL 2018.5.21  �°�8�������¼�����
#define		UPKEY				8				//11	ZCL 2018.5.21  �°�8�������¼�����
	*/

/*
��Һ�������� 2013.12.4
#define		LCD_SCL				PBout(3)		//ʱ�� D0��SCLK��
#define		LCD_SDA				PBout(0)		//D1��MOSI�� ����
#define		LCD_RST				PAout(7)		//��λ
#define		LCD_DC				PBout(1)		//����/�������

#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel	    ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xCF
#define X_WIDTH 128
#define Y_WIDTH 64 */

// ZCL 2018.11.12 LORA SX1278-----------
// USART_BUAD_CONFIG
#define BAUD 115200
// USART_PRINTF_CONFIG
#define USED_COM_NUMBER USART1

// GPIO CONFIG			//ZCL 2018.11.9
#define RCC_LED_GPIOx RCC_APB2Periph_GPIOA
#define LED0_PINx GPIO_Pin_11 // ZCL 2018.12.13
#define LED0_GPIOx GPIOA
#define LED0_ON GPIO_SetBits(LED0_GPIOx, LED0_PINx)	   // ZCL 2018.11.9
#define LED0_OFF GPIO_ResetBits(LED0_GPIOx, LED0_PINx) // ZCL 2018.11.9
#define LED0_TOGGLE GPIO_PinReverse(LED0_GPIOx, LED0_PINx)

// ZCL 2018.11.9  �ټ�1·LED
#define RCC_LED1_GPIOx RCC_APB2Periph_GPIOD
#define LED1_PINx GPIO_Pin_6 // ZCL 2018.12.13 �����õ�
#define LED1_GPIOx GPIOD
#define LED1_ON GPIO_SetBits(LED1_GPIOx, LED1_PINx)	   // ZCL 2018.11.9
#define LED1_OFF GPIO_ResetBits(LED1_GPIOx, LED1_PINx) // ZCL 2018.11.9
#define LED1_TOGGLE GPIO_PinReverse(LED1_GPIOx, LED1_PINx)
// ZCL 2018.11.12 LORA SX1278-----------

//---------------------------------2015.8.29 STM32 DSP����
#define READSTM32DATA 1
#define WRITESTM32DATA 101
#define WRITESTM32MULDATA 201

#define MODPAR1 1
#define MODPAR2 2
#define MODPAR3 3
#define MODPAR4 4

#define DNB_ADDRESS 0			 // ��д������ַ��w_DNBParLst������ʱƫ��0000����  ZCL 2023.2.9
#define STM32_RPAR_ADDRESS 10000 // ֻ��������ַ��w_ParLst������ʱƫ��10000����
#define DSP_WRPAR_ADDRESS 20000	 // ��д������ַ��Pw_dspParLst������ʱƫ��20000����
#define DSP_RPAR_ADDRESS 30000	 // ֻ��������ַ��w_dspParLst������ʱƫ��30000����
#define SCR_WRPAR_ADDRESS 40000	 // ��д������ַ��w_ScrParLst������ʱƫ��40000����
// ZCL 2023.2.9
#define STM32_WRPAR_ADDRESS_SQB 50000 // ��д������ַ��Pw_ParLst������ʱƫ��60000����
#define STM32_WRPAR_ADDRESS_BPDJ 0	  // ��д������ַ��Pw_ParLst������ʱƫ��60000����

// ��ע������ARM DCM100��FM25L16�ķ���
//-------���Ᵽ�����ݵ�ַ����96���ֽڣ���������32��
#define FMADD_FLASH_REC_NO 640	// FLASH��¼���
#define FMADD_FLASH_REC_NUM 642 // FLASH��¼����
#define FMADD_FAULT_REC_NUM 644 // ���ϼ�¼����	������ǰ��Ϊ�˸�SM510��Ӧ����
#define FMADD_FAULT_REC_NO 646	// ���ϼ�¼���
#define FMADD_PUMP_RUN_TIME 648 //(15����,30���ֽ�)	 ˮ������ʱ��
//-------
#define FMADD_STM32 0 // STM32�趨���������ַ 		����FMRAM  0000-0639=640
// #define FMADD_FLASH_REC_NO	640			// FLASH��¼���  ������
#define FMADD_DSP 736	 // DSP�趨���������ַ 			����FMRAM  0736-1407=672
#define FMADD_FAULT 1408 // �������ݼ�¼��ַ ����FMRAM  1408-1919=512
#define FMADD_CRC 1920	 // CRC���ݼ�¼��ַ 	����FMRAM  1920-2039=60(1���ֱ���32���ֽڵ�CRC)

// ZCL 2019.4.4 FM25L16������������ռ����  2K�ֽ� 0-1023
// DCM200_SCR_E��FM25L16����
// 1. 0-99 �ֽڣ�50���֣�����SCR����
// 2. 256 �ֽڿ�ʼ��288���ֽڣ�DTU_PAR_SIZE�� ������ GPRS����

// ���ʱ��
#define SClk1Ms SoftClock[0]	// ���ʱ�� 1ms
#define SClk10Ms SoftClock[1]	// ���ʱ�� 10ms
#define SClkSecond SoftClock[2] // ���ʱ��  s
#define SClkMinute SoftClock[3] // ���ʱ��  m
#define SClkHour SoftClock[4]	// ���ʱ��  h
#define SClkDay SoftClock[5]	// ���ʱ��  d
#define SClkMonth SoftClock[6]	// ���ʱ��  m
#define SClkYear SoftClock[7]	// ���ʱ��  y
#define SClk0r5Ms SoftClock[8]	// ���ʱ��  0.5MS	2010.8.6 ��ʱ�ж��вɼ�����ֵ

/* #define STM32_WRPAR_ADDRESS			0 			// ��д������ַ��Pw_ParLst������ʱƫ��1000����
#define STM32_RPAR_ADDRESS			10000 	// ֻ��������ַ��w_ParLst������ʱƫ��1000����
#define DSP_WRPAR_ADDRESS				20000 	// ֻ��������ַ��Pw_dspParLst������ʱƫ��1000����
#define DSP_RPAR_ADDRESS				30000 	// ֻ��������ַ��w_dspParLst������ʱƫ��1000���� */
// void	Define2_Stm32RWPar(void) {}
//------1.�趨�����Ķ���---------------------------------------------------------------------
//  �����趨����
#define Pw_OutPDecMax Pw_ParLst[0]		  //.3 ��ˮ�����̡���ʼ��ֵ1660����Ӧ1.66MP		(С��λ��.3)
#define Pw_InPDecMax Pw_ParLst[1]		  //.3 ��ˮ�����̡���ʼ��ֵ1000����Ӧ1.00MP
#define Pw_SetP Pw_ParLst[2]			  //.3 �趨��ѹ��
#define Pw_HaveWaterRunP Pw_ParLst[3]	  //.3 ��ˮ����ѹ��(100��Ϊ1����)
#define Pw_NoWaterStopP Pw_ParLst[4]	  //.3 ��ˮͣ��ѹ������ʼ��ֵ3����Ӧ0.003MP
#define Pw_HaveWaterRunDelay Pw_ParLst[5] //.0 ��ˮ������ʱ����ʼ��ֵ30��
#define Pw_NoWaterStopDelay Pw_ParLst[6]  //.0 ��ˮͣ����ʱ
#define Pw_VfToGpDelay Pw_ParLst[7]		  //.0 ��Ƶת��Ƶ��ʱʱ��
#define Pw_GpExitDelay Pw_ParLst[8]		  //.0 ��Ƶ�˳���ʱʱ��

#define Pw_MinRunFreqDec Pw_ParLst[10]	  //.1 Ƶ�������½�����ֵ(Լ15HZ)
#define Pw_EquipmentNo Pw_ParLst[11]	  //.0 �豸ͨѶ��ַ
#define Pw_PumpNum Pw_ParLst[12]		  //.0 ˮ������
#define Pw_TimeChangeMin Pw_ParLst[13]	  //.0 ��ʱ����������
#define Pw_VvvfResetMax Pw_ParLst[14]	  //.0 ��Ƶ����λ����������ʼ��ֵ5��
#define Pw_CheckCycleMinute Pw_ParLst[15] //.0 Ѳ������ʱ�䣨��������
#define Pw_CheckRunFreq Pw_ParLst[16]	  //.1 Ѳ������Ƶ�ʣ���ʼ��ֵ200����ӦԼ20.0Hz
#define Pw_CheckRunSec Pw_ParLst[17]	  //.0 Ѳ�������ʱ�䣺��ʼ��ֵ30��
#define Pw_EnableCheck Pw_ParLst[18]	  //.0 ʹ��Ѳ�칦��
#define Pw_EnablePlayV Pw_ParLst[19]	  //.0 �������
#define Pw_BetweenPlaySec Pw_ParLst[20]	  //.0 �������ʱ��
#define Pw_VvvfComAddr Pw_ParLst[21]	  //.0 ��Ƶ��ͨѶ��ַ
#define Pw_LLJComAddr Pw_ParLst[22]		  //.0 ������ͨѶ��ַ

#define Pw_OverPThanSetPEk Pw_ParLst[24] //.3 ��ѹ�趨ƫ���ʼ��ֵ200����Ӧ0.200MP
//.0
#define Pw_VoltageMAX Pw_ParLst[25]		 //.1 ϵͳ��ѹ���ֵ
#define Pw_1PumpCurrentMAX Pw_ParLst[26] //.1 1#�õ������ֵ
#define Pw_2PumpCurrentMAX Pw_ParLst[27] //.1 2#�õ������ֵ
#define Pw_3PumpCurrentMAX Pw_ParLst[28] //.1 3#�õ������ֵ
#define Pw_4PumpCurrentMAX Pw_ParLst[29] //.1 4#�õ������ֵ
#define Pw_5PumpCurrentMAX Pw_ParLst[30] //.1 5#�õ������ֵ
#define Pw_YeWeiSensorMAX Pw_ParLst[31]	 //.2 Һλ����������
#define Pw_YeWeiSensorZero Pw_ParLst[32] //.0 Һλ��������ʼֵ��4MA��Ӧ��ֵ��
#define Pw_FluxMAX Pw_ParLst[33]		 //.1 ˲ʱ�������ֵ
//
#define Pw_EnableSmall Pw_ParLst[34]		 //.0 С������ѹ����ʹ��
#define Pw_SmallHoldP Pw_ParLst[35]			 //.3 С������ѹֵ
#define Pw_InSmallFreqMinValue Pw_ParLst[36] //.1 С�����������СƵ��ֵ (50)
#define Pw_SmallRiseSec Pw_ParLst[37]		 //.0 С��������ʱ��(30)
#define Pw_BetweenSmallSec Pw_ParLst[38]	 //.0 С�������ʱ�� �� (60��)
#define Pw_ExitSmallEk Pw_ParLst[39]		 //.3 С�����˳�ƫ���ʼ��ֵ3����Ӧ0.03MP
#define Pw_SmallStablePEk Pw_ParLst[40]		 //.3 С������ѹƫ��  Invariable:�㶨��
#define Pw_SmallJudgeDelay Pw_ParLst[41]	 //.0 С�����ж���ʱ
// 42.43
// 44 ������С����
// 45--51 ���ʱ������ ����		//ZCL 2018.4.21��ע���Ƶ� w_SetSecond		w_ParLst[245]--[251]
#define Pw_VoiceValue Pw_ParLst[44] //.0

//
#define Pw_PID_Kc Pw_ParLst[52] //.3 ����PID����ϵ��
#define Pw_PID_Ts Pw_ParLst[53] //.3 ����PID����ʱ��
#define Pw_PID_Ti Pw_ParLst[54] //.3 ����PID����ʱ��
#define Pw_PID_Td Pw_ParLst[55] //.3 ����PID΢��ʱ��

#define Pw_Module1Add Pw_ParLst[57] //.0 SM510ģ��1��ַ       //��ģ������

#define Pw_Module2Add Pw_ParLst[59]		//.0 SM510ģ��2��ַ		//.0����MD304L����
#define Pw_InPBigSetDelay Pw_ParLst[60] //.0 ����ˮѹ������ʱ
#define Pw_InPNormalDelay Pw_ParLst[61] //.0 ����ˮѹ��������ʱ
#define Pw_InPNormalEk Pw_ParLst[62]	//.3 ����ˮѹ������ƫ��
#define Pw_InPType Pw_ParLst[63]		//.0 ��ˮ��ѹ������������
#define Pw_OutPType Pw_ParLst[64]		//.0 ��ˮ��ѹ������������
#define Pw_SoftOverPEn Pw_ParLst[65]	//.0 �����ѹ����ʹ��
//
#define Pw_EnableTime Pw_ParLst[66]			   //.0 ʹ�ܶ�ʱ����ֹͣ����
#define Pw_Time1Run Pw_ParLst[67]			   //.0 ��ʱ1����
#define Pw_Time1Stop Pw_ParLst[68]			   //.0 ��ʱ1ֹͣ
#define Pw_Time1SetP Pw_ParLst[69]			   //.3 ��ʱ1�趨ѹ��
#define Pw_Time2Run Pw_ParLst[70]			   //.0 ��ʱ2����
#define Pw_Time2Stop Pw_ParLst[71]			   //.0 ��ʱ2ֹͣ
#define Pw_Time2SetP Pw_ParLst[72]			   //.3 ��ʱ2�趨ѹ��
#define Pw_Time3Run Pw_ParLst[73]			   //.0 ��ʱ3����
#define Pw_Time3Stop Pw_ParLst[74]			   //.0 ��ʱ3ֹͣ
#define Pw_Time3SetP Pw_ParLst[75]			   //.3 ��ʱ3�趨ѹ��
#define Pw_Time4Run Pw_ParLst[76]			   //.0 ��ʱ4����
#define Pw_Time4Stop Pw_ParLst[77]			   //.0 ��ʱ4ֹͣ
#define Pw_Time4SetP Pw_ParLst[78]			   //.3 ��ʱ4�趨ѹ��
#define Pw_Time5Run Pw_ParLst[79]			   //.0 ��ʱ5����
#define Pw_Time5Stop Pw_ParLst[80]			   //.0 ��ʱ5ֹͣ
#define Pw_Time5SetP Pw_ParLst[81]			   //.3 ��ʱ5�趨ѹ��
#define Pw_Time6Run Pw_ParLst[82]			   //.0 ��ʱ6����
#define Pw_Time6Stop Pw_ParLst[83]			   //.0 ��ʱ6ֹͣ
#define Pw_Time6SetP Pw_ParLst[84]			   //.3 ��ʱ6�趨ѹ��
#define Pw_TimePwdStopPwd Pw_ParLst[85]		   //.0 ��ʱ����ͣ������
#define Pw_TimePwdStopMD Pw_ParLst[86]		   //.0 ��ʱ����ͣ������
#define Pw_InPSensorZero Pw_ParLst[87]		   //.0 ��ˮ��ѹ����������ֵ
#define Pw_OutPSensorZero Pw_ParLst[88]		   //.0 ��ˮ��ѹ����������ֵ
#define Pw_SmallNoStableSubOneEn Pw_ParLst[89] //.0 С��������ѹ��1ʹ��

#define Pw_InPPidEkXiShu Pw_ParLst[90] //.0 ZCL 2017.8.17 �ĳɽ�ˮѹPIDƫ��ϵ��������ƫ��ӿ�PID

#define Pw_NoWaterHz Pw_ParLst[91]			 //.1 ��ˮͣ������Ƶ�� ��2017.8.4
#define Pw_NoWaterHzDownDelay1 Pw_ParLst[92] //.0��ˮ��Ƶ��ʱ  ��
#define Pw_NoWaterHzDownDelay2 Pw_ParLst[93] //.0 ������ˮͣ������ʱ

#define Pw_InPBigOutDelay Pw_ParLst[94] //.0 ��ˮ�ڴ��ڳ�ˮ��ѹ����ʱ
#define Pw_MaxSupplyDelay Pw_ParLst[95] //.0 ���ˮ������ʱ
#define Pw_InPBigSetEn Pw_ParLst[96]	//.0 ��ˮ�ڴ����趨ѹ��ʹ��
#define Pw_InPBigOutEn Pw_ParLst[97]	//.0 ��ˮ�ڴ��ڳ�ˮ��ѹ��ʹ��
#define Pw_MaxSupplyEn Pw_ParLst[98]	//.0 ���ˮ����ʹ��
#define Pw_TimeChangeEn Pw_ParLst[99]	//.0 ��ʱ����ʹ��
//
#define Pw_MaxFreqHex Pw_ParLst[100]	 //.0 ���Ƶ����ʾHexֵ��У��Ƶ��
#define Pw_VADelay Pw_ParLst[101]		 //.0 ��ѹ������ʾ��ʱMS��ÿ����ʱ�任��һ�� 2007.11.1
#define Pw_MaxPIDHex Pw_ParLst[102]		 //.0 ���PID����Hexֵ�����Ƶ�ѹ���ֵ������
#define Pw_MaxAIHex Pw_ParLst[103]		 //.0 ���ģ������Hexֵ��У��ģ�����룬һ�㲻�õ���
#define Pw_AIFilterSetNum Pw_ParLst[104] //.0 ģ�������˲�����Ŀ
#define Pw_DIStableSetNum Pw_ParLst[105] //.0 �������ȶ���Ŀ

// 106-110 Modem ɾ��

#define Pw_AlarmMask1 Pw_ParLst[111]	  //.0 ��������1
#define Pw_AlarmMask2 Pw_ParLst[112]	  //.0 ��������2
#define Pw_AlarmMask3 Pw_ParLst[113]	  //.0 ��������3
#define Pw_PowErrSwitchSel Pw_ParLst[114] //.0 ��Դ���Ͽ���ѡ��  0=���㣬1=�յ�
#define Pw_EquipType Pw_ParLst[115]		  //.0 �豸����ѡ��

#define Pw_VvvfResetSec Pw_ParLst[116]	  //.0 ��Ƶ��������λʱ��
#define Pw_ClrVvvfResetSec Pw_ParLst[117] //.0 �����Ƶ���Ѹ�λ����ʱ��
#define Pw_MaxSupplyGpSum Pw_ParLst[118]  //.0 ���ˮ������Ƶ����
#define Pw_MaxSupplyVfFreq Pw_ParLst[119] //.1 ���ˮ������ƵƵ��

#define Pw_TimeWriteRecSec Pw_ParLst[120]	  //.0 ��ʱд��¼ʱ��
#define Pw_AheadDaysVoiceCount Pw_ParLst[121] //.0 ��ǰ���������Ŵ���
#define Pw_MasterSlaveSel Pw_ParLst[122]	  //.0 �����ӻ�ѡ��
#define Pw_FanMotorOffDelay Pw_ParLst[123]	  //.0 ������ȶϿ���ʱ
#define Pw_DownPID_Kc Pw_ParLst[124]		  //.3 �����½�PID����ϵ��
#define Pw_DownPID_Ts Pw_ParLst[125]		  //.3 �����½�PID����ʱ��
#define Pw_DownPID_Ti Pw_ParLst[126]		  //.3 �����½�PID����ʱ��
#define Pw_Time1ChangeHM Pw_ParLst[127]		  //.0 ��ʱ1���� Сʱ����
#define Pw_Time2ChangeHM Pw_ParLst[128]		  //.0 ��ʱ2���� Сʱ����
#define Pw_Time3ChangeHM Pw_ParLst[129]		  //.0 ��ʱ3���� Сʱ����
//
#define Pw_VfToGpEk Pw_ParLst[130]			  //.3 ��Ƶת��Ƶ ƫ��
#define Pw_GpExitEk Pw_ParLst[131]			  //.3 ��Ƶ�˳�  ƫ��
#define Pw_FluxBase Pw_ParLst[132]			  //.2 ������ֵ
#define Pw_MinFlux Pw_ParLst[133]			  //.2 ��С����
#define Pw_MaxFlux Pw_ParLst[134]			  //.2 �������
#define Pw_MinP Pw_ParLst[135]				  //.3 ��Сѹ��
#define Pw_MaxP Pw_ParLst[136]				  //.3 ���ѹ��
#define Pw_BLBYEn Pw_ParLst[137]			  //.0 ������ѹʹ��
#define Pw_VfToGpEn Pw_ParLst[138]			  //.0 ��Ƶת��Ƶʹ��
#define Pw_CpuHotOffect Pw_ParLst[139]		  //.1 Cpu����ЧӦ�¶�
#define Pw_PSensorProtectDelay Pw_ParLst[140] //.0 ѹ��������������ʱ ��
#define Pw_BaudRate2 Pw_ParLst[141]			  //.0 ���ڲ�����
#define Pw_BaudRate0 Pw_ParLst[142]			  //.0 ��0������
#define Pw_BaudRate1 Pw_ParLst[143]			  //.0 ��1������
#define Pw_SmallBadClrSec Pw_ParLst[144]	  //.0 С�����ƻ������
//
#define Pw_SendToGprsEn Pw_ParLst[145]		  //.0 �������͸�GPRS��������ʹ��
#define Pw_SendToGprsDataLen Pw_ParLst[146]	  //.0 �������͸�GPRS�������ݳ��� ��
#define Pw_SendToGprsSec Pw_ParLst[147]		  //.0 �������͸�GPRS�������ݼ��ʱ�� ��
#define Pw_Com0BufType Pw_ParLst[148]		  //.0 ͨѶ��������(ͨѶЭ������) ��ͨ=1,����=2
#define Pw_ASensorType Pw_ParLst[149]		  //.0 ��������������
#define Pw_ASensorZero Pw_ParLst[150]		  //.0 ������������ֵ
#define Pw_VASumFilter Pw_ParLst[151]		  //.0 ���˲�
#define Pw_DelayCheckVvvfAlarm Pw_ParLst[152] //.0 ��ʱ����Ƶ������ ��
#define Pw_BCQErrStopEN Pw_ParLst[153]		  //.0 ������Ԥ��ͣ��ʹ��  �غ��� 2017.6.3

#define Pw_ScrFastKeyBetweenMS Pw_ParLst[155] //.0��������MS  ����SCR����� ZCL 2015.9.6
#define Pw_ZDRunMode Pw_ParLst[156]			  //.0 ��Ƶ����ģʽ
#define Pw_ZDRunHz Pw_ParLst[157]			  //.1 ��ƵƵ��
#define Pw_SystemRunMode Pw_ParLst[158]		  //.0 ϵͳ����ģʽ: =1��Ƶ��; =0������(Ĭ��)
#define Pw_VfAlarmSubOneSecs Pw_ParLst[159]	  //.0 ��Ƶ��������1�� �� //ZCL 2018.6.13

// ZCL 2019.8.31 ���ӣ���ˮ��Ƶģʽ
#define Pw_ISwitch1 Pw_ParLst[162] //.0 I_Switch1=0 ������ģʽ��I_Switch1=1 ��Ƶ��ģʽ

#define Pw_SoftOverPDelay Pw_ParLst[165] //.0�����ѹ��ʱ

#define Pw_InP_CTLEn Pw_ParLst[166]		   //.0��ˮ����Ƶ��ʹ�� 2010.8.3 qhd
#define Pw_VfFreqDownSircle Pw_ParLst[167] //.0ÿ��ݼ�Ƶ��ֵ
#define Pw_VfFreqDownMin Pw_ParLst[168]	   //.1��С����Ƶ��
#define Pw_SetInP Pw_ParLst[169]		   //.3��ˮ�趨ѹ��

#define Pw_PumpSoftStopEn Pw_ParLst[170]   //.0�豸��ͣʹ��
#define Pw_SoftStopDownDec Pw_ParLst[172]  //.1��ͣÿ��ݼ�Ƶ��ֵ
#define Pw_FastKeyBetweenMS Pw_ParLst[173] //.0��������MS  ����SCR����� ZCL 2015.9.6
// �¼ӵ��ܱ����  �غ���2016.9.20
#define Pw_UnitSlect2 Pw_ParLst[174]  // ��λѡ��ʹ�ܣ�1�� ����kwh����1���壻2�������������1kWH��
#define Pw_PluseToDNB2 Pw_ParLst[175] // ����KWH����1����
#define Pw_Clear2 Pw_ParLst[176]	  // �����ۻ�������ֵ
#define Pw_PluseToDNB Pw_ParLst[177]  // �����������1kwh
#define Pw_SumDNBjz Pw_ParLst[178]	  // Ԥ���������
#define Pw_SumDNBjzH Pw_ParLst[179]	  // Ԥ���������

#define Pw_NoWaterReAlarmEn Pw_ParLst[182]		//.0��ˮͣ��Ԥ������ʹ��
#define Pw_NoWaterReAlarmSetP Pw_ParLst[183]	//.3��ˮͣ��Ԥ���趨ѹ��
#define Pw_NoWaterReAlarmDelay Pw_ParLst[184]	//.0��ˮͣ��Ԥ����ʱ
#define Pw_NoWaterNoReAlarmSetP Pw_ParLst[185]	//.3��ˮͣ��Ԥ������趨ѹ��
#define Pw_NoWaterNoReAlarmDelay Pw_ParLst[186] //.0��ˮͣ��Ԥ�������ʱ
#define Pw_PBoDongContlVfEn Pw_ParLst[187]		//.0ѹ�����������趨ѹ��ʹ��
#define Pw_OutPDownEK Pw_ParLst[188]			//.3��ˮѹ���趨ѹ��
#define Pw_SetPDownEK Pw_ParLst[189]			//.3 �趨ѹ���½�ѹ��
#define Pw_PBoDongDelay Pw_ParLst[190]			//.0�����趨ѹ����ʱ
#define Pw_PBoDongOKDelay Pw_ParLst[191]		//.0�趨ѹ���ָ���ʱ

#define Pw_PresssureSteadyVfDownEn Pw_ParLst[192]	 //.0��ѹ��Ƶʹ��	0
#define Pw_VfDownStartTime Pw_ParLst[193]			 //.0��ʼʱ��		00:30
#define Pw_VfDownStopTime Pw_ParLst[194]			 //.0����ʱ��		04:30
#define Pw_PresssureSteadyVfDownPEk Pw_ParLst[195]	 //.3��ѹ��Ƶѹ��  0.020MPa
#define Pw_PresssureSteadyJudgeDelay Pw_ParLst[196]	 //.0��ѹ��ʱ		120s
#define Pw_PresssureUnstablePEk Pw_ParLst[197]		 //.3����ѹѹ��
#define Pw_PresssureUnstableDelay Pw_ParLst[198]	 //.0����ѹ��ʱ
#define Pw_PressureSteadyVfDownMin Pw_ParLst[199]	 //.1��ѹ��СƵ��
#define Pw_PressureSteadyVfDownSircle Pw_ParLst[200] //.1��ѹÿ�뽵Ƶ��ֵ
#define Pw_LowRunFreqDec Pw_ParLst[201]				 //.1�������Ƶ��ֵ
#define Pw_SmallVfUpValue Pw_ParLst[202]			 //.1С��������Ƶ������ֵ  ��2013.4.22

// ����Ϊ�¼Ӳ���  �غ���2016.1.7
#define Pw_SlaveStartDelay Pw_ParLst[203] //.0�ӻ�������ʱ
// ����Ϊ�¼Ӳ���  �غ���2016.1.15
#define Pw_InOutPEqualEn Pw_ParLst[204] //.0����ˮѹ��ͬ��ʹ��  �� 16.1.15
// ����Ϊ�����Ʋ��� ��2016.1.29

#define Pw_InstanFDecMax Pw_ParLst[205]		//.0����������
#define Pw_InstanFSensorZero Pw_ParLst[206] //.0�����Ƴ�ֵ
#define Pw_UnitSlect Pw_ParLst[207]			//.0��λѡ��ʹ�ܣ�1�� ���������״���1���壻2�������������1������
#define Pw_PluseToFlux2 Pw_ParLst[208]		//.0���������״���1����
#define Pw_Clear Pw_ParLst[209]				//.0�����ۻ�������ֵ
#define Pw_PluseToFlux Pw_ParLst[210]		//.0�����������1����
#define Pw_SumFluxjz Pw_ParLst[211]			//.0Ԥ���������
#define Pw_SumFluxjzH Pw_ParLst[212]		//.0Ԥ���������
#define Pw_InstanFSensorType Pw_ParLst[213] //.0��������������(4-20mA;0-5V)
// ���²���Ϊ��ˮ��ѹ��Ƶ���� �¼ӵ�
#define Pw_SlaveHaveWaterRunP Pw_ParLst[214]	 //.3 �ӻ���ˮ����ѹ��(100��Ϊ1����)
#define Pw_SlaveNoWaterStopP Pw_ParLst[215]		 //.3 �ӻ���ˮͣ��ѹ������ʼ��ֵ50����Ӧ0.050MP
#define Pw_SlaveHaveWaterRunDelay Pw_ParLst[216] //.0 �ӻ���ˮ������ʱ����ʼ��ֵ5��
#define Pw_SlaveNoWaterStopDelay Pw_ParLst[217]	 //.0 �ӻ���ˮͣ����ʱ����ʼ��ֵ5��

#define Pw_HengYaLongDelay Pw_ParLst[218]	 //.0 ��ѹ����ʱ
#define Pw_JiaYaShortDelay Pw_ParLst[219]	 //.0 ��ѹ����ʱ
#define Pw_InPChangeDelay Pw_ParLst[220]	 //.0 ��ˮѹ��ѹ ��ʱ	//ZCL 2017.8.16 ��������Ѿ�ȡ��
#define Pw_InPChangeEk Pw_ParLst[221]		 //.3 PID ƫ��  ��ˮѹ��ѹƫ��		��ˮѹPIDƫ��	ZCL 2017.8.14
#define Pw_InPChangeOutPXiShu Pw_ParLst[222] //.0 ��ѹϵ�� ��ˮѹ�ı��ˮѹϵ��(����)
											 // #define	Pw_InPPidEkXiShu			Pw_ParLst[90]		//.0 ��ˮPIDƫ��� ZCL 2017.8.17 �ĳɽ�ˮѹPIDƫ��ϵ��������ƫ��ӿ�PID
// ����Ϊ�¼Ӳ��� �� 16.4.18		2018.8.21
#define Pw_ChangeWaterSourceHour Pw_ParLst[223]	 // ����ˮ�л���ˮ���л�ˮԴ��ʱСʱ
#define Pw_DDFOpenDelay Pw_ParLst[224]			 // �綯���������ʱ
#define Pw_DDFCloseDelay Pw_ParLst[225]			 // �綯���ر������ʱ
#define Pw_HaveWaterSel Pw_ParLst[226]			 // ˮ����ˮͣ���ź�ѡ��
#define Pw_ChangeWaterSourceHour2 Pw_ParLst[227] // ˮ���л�������ˮ�л�ˮԴ��ʱСʱ
#define Pw_SelWaterToWater Pw_ParLst[228]		 // ��ʱ���㽻��ѡ��

#define Pw_Time1HM Pw_ParLst[229] // ��������ˮ�л���ˮ��1
#define Pw_Time2HM Pw_ParLst[230] // ��������ˮ�л���ˮ��2
#define Pw_Time3HM Pw_ParLst[231] // ����ˮ���л�������ˮ1
#define Pw_Time4HM Pw_ParLst[232] // ����ˮ���л�������ˮ2
#define Pw_Test Pw_ParLst[233]	  // �������ѡ��  =1�������
#define Pw_Time5HM Pw_ParLst[234] // ��������ˮ�л���ˮ��3
#define Pw_Time6HM Pw_ParLst[235] // ����ˮ���л�������ˮ3

// zcs2018.01.14
#define Pw_DDFManuAutoCtlEn Pw_ParLst[236] // �綯���ֶ�����ʹ��

// ע�⣺Ŀǰ��ˮ����256���ֲ�������������࣬��Ҫ���������ֵ��һ������һ������32���ֽڣ�16���֣�
//			DoWith.c ��ParLst_Init()		SavePar_Prompt()

/* // ����ģ�����������ò������ӵ�ַ240��ʼ����ַ254����15��
 */
/* #define STM32_WRPAR_ADDRESS			0 			// ��д������ַ��Pw_ParLst������ʱƫ��1000����
#define STM32_RPAR_ADDRESS			10000 	// ֻ��������ַ��w_ParLst������ʱƫ��1000����
#define DSP_WRPAR_ADDRESS				20000 	// ֻ��������ַ��Pw_dspParLst������ʱƫ��1000����
#define DSP_RPAR_ADDRESS				30000 	// ֻ��������ַ��w_dspParLst������ʱƫ��1000���� */
// void	Define3_Stm32RPar(void) {}
//------2.ֻ�������Ķ���------------------------------------------------------------------------
//  ����ֻ���������ӵ�ַ0��ʼ�����Ƿ���ʱ�ڲ���1000������ �ܳ��� 2015.6.28

#define w_Uk1 w_ParLst[0]			  //.0 Ƶ�����Hexֵ
#define w_InPSensorValue w_ParLst[1]  //.0 ��ˮ��ѹ����������ֵ
#define w_OutPSensorValue w_ParLst[2] //.0 ��ˮ��ѹ����������ֵ
#define w_FaultRecNum w_ParLst[3]	  //.0 ���ϼ�¼���� ZCL ������ǰ��Ϊ�˸�SM510��Ӧ����
#define w_FaultRecNo w_ParLst[4]	  //.0 ���ϼ�¼��  ZCL

#define w_DelVvvfAlarmed w_ParLst[5] //.0 ȡ�� ����Ƶ���� ״̬  ZCL		//ZCL 2022.10.3 =1 ɾ��

#define w_ModPar w_ParLst[9]	  //.0 �޸Ĳ���  ZCL 2015.9.9
#define w_ParInitial w_ParLst[10] //.0 ������ʼ��   ZCL 2015.9.9

#define w_FlashWrRdLock w_ParLst[11] //.0 Flashд������

//.0#define	w_FlashRecNoNumPointerNo	w_ParLst[12]	//.0 FLASH��¼���������ָ�����
#define w_SelFaultNo w_ParLst[13] //.0 ѡ����Ϻ�
#define w_SelRecNo w_ParLst[14]	  //.0 ѡ���¼��
#define w_PreFaultNo w_ParLst[15] //.0 �ϴι��Ϻ�
//
#define w_TestItemSel w_ParLst[16]		  //.0 ������ѡ���趨���� (�����޸ģ������Ա���)
#define w_VvvfAlmNum w_ParLst[17]		  //.0 ��Ƶ��������
#define w_BetweenSmall w_ParLst[18]		  //.0 С�������״̬
#define w_SmallStableRunSec w_ParLst[19]  //.0 С������ѹ��ʱ
#define w_SoftVer w_ParLst[20]			  //.2 ����汾
#define w_TimePwdStopST w_ParLst[21]	  //.0 ��ʱ����ͣ��״̬
#define w_EquipOperateStatus w_ParLst[22] //.0 �豸����״̬
#define w_EquipOperateNum w_ParLst[23]	  //.0 �豸����״̬����
#define w_EquipAlarmStatus w_ParLst[24]	  //.0 �豸ͣ��ԭ�򣨱�����
#define w_EquipStopNum w_ParLst[25]		  //.0 �豸ͣ��������������
//
#define w_EquipAlarmLast6 w_ParLst[26] //.0 �豸����ͣ��ԭ����6��
#define w_EquipAlarm6YM w_ParLst[27]   //.0 �豸����ͣ��6ʱ�䣭����
#define w_EquipAlarm6DH w_ParLst[28]   //.0 �豸����ͣ��6ʱ�䣭��ʱ
#define w_EquipAlarm6MS w_ParLst[29]   //.0 �豸����ͣ��6ʱ�䣭����
#define w_EquipAlarmLast5 w_ParLst[30] //.0 �豸����ͣ��ԭ����5��
#define w_EquipAlarm5YM w_ParLst[31]   //.0 �豸����ͣ��5ʱ�䣭����
#define w_EquipAlarm5DH w_ParLst[32]   //.0 �豸����ͣ��5ʱ�䣭��ʱ
#define w_EquipAlarm5MS w_ParLst[33]   //.0 �豸����ͣ��5ʱ�䣭����
#define w_EquipAlarmLast4 w_ParLst[34] //.0 �豸����ͣ��ԭ����4��
#define w_EquipAlarm4YM w_ParLst[35]   //.0 �豸����ͣ��4ʱ�䣭����
#define w_EquipAlarm4DH w_ParLst[36]   //.0 �豸����ͣ��4ʱ�䣭��ʱ
#define w_EquipAlarm4MS w_ParLst[37]   //.0 �豸����ͣ��4ʱ�䣭����
#define w_EquipAlarmLast3 w_ParLst[38] //.0 �豸����ͣ��ԭ����3��
#define w_EquipAlarm3YM w_ParLst[39]   //.0 �豸����ͣ��3ʱ�䣭����
#define w_EquipAlarm3DH w_ParLst[40]   //.0 �豸����ͣ��3ʱ�䣭��ʱ
#define w_EquipAlarm3MS w_ParLst[41]   //.0 �豸����ͣ��3ʱ�䣭����
#define w_EquipAlarmLast2 w_ParLst[42] //.0 �豸����ͣ��ԭ����2��
#define w_EquipAlarm2YM w_ParLst[43]   //.0 �豸����ͣ��2ʱ�䣭����
#define w_EquipAlarm2DH w_ParLst[44]   //.0 �豸����ͣ��2ʱ�䣭��ʱ
#define w_EquipAlarm2MS w_ParLst[45]   //.0 �豸����ͣ��2ʱ�䣭����
#define w_EquipAlarmLast1 w_ParLst[46] //.0 �豸����ͣ��ԭ����1��
#define w_EquipAlarm1YM w_ParLst[47]   //.0 �豸����ͣ��1ʱ�䣭����
#define w_EquipAlarm1DH w_ParLst[48]   //.0 �豸����ͣ��1ʱ�䣭��ʱ
#define w_EquipAlarm1MS w_ParLst[49]   //.0 �豸����ͣ��1ʱ�䣭����
//
#define w_SelEquipAlarm w_ParLst[50]	//.0 ѡ����豸����ͣ��ԭ��
#define w_SelEquipAlarm1YM w_ParLst[51] //.0 ѡ����豸����ͣ��ʱ�䣭����
#define w_SelEquipAlarm1DH w_ParLst[52] //.0 ѡ����豸����ͣ��ʱ�䣭��ʱ
#define w_SelEquipAlarm1MS w_ParLst[53] //.0 ѡ����豸����ͣ��ʱ�䣭����

#define w_FlashRecNo w_ParLst[54]	   //.0 FLASH��¼��
#define w_FlashRecNum w_ParLst[55]	   //.0 FLASH��¼����
#define w_TimePwdStopDays w_ParLst[56] //.0 ��ʱ����ͣ������
#define w_RemoteStop w_ParLst[57]	   //.0 Զ��ң�ر�Ƶֹͣ =1ͣ��

#define w_RemoteVfRstRelayOn w_ParLst[59] //.0 ң�ر�Ƶ����λ�̵���ON
#define w_TouchAutoManu w_ParLst[60]	  //.0 ���� �Զ�/�ֶ�
#define w_TouchRunStop w_ParLst[61]		  //.0 ���� ����/ֹͣ
#define w_TouchDDFOnOFF w_ParLst[62]	  // ���� ���ص綯�� zcs 2018.01.14

//.0#define	w_RunTimeSumNo				w_ParLst[67]	//.0 ˮ���ۼ�����ָ�����
#define w_SaveRecNo w_ParLst[68]		 //.0 ����Flash��¼��
#define w_SaveRecNum w_ParLst[69]		 //.0 ����Flash��¼����
#define w_CpuTemperatureHex w_ParLst[70] //.0 Cpu�¶�Hex
#define w_CpuWendu w_ParLst[71]			 //.1 Cpu�¶Ȼ���ֵ
#define w_SelPar w_ParLst[72]			 //.0 ѡ�����
#define w_SelParValue w_ParLst[73]		 //.0 ѡ�������ֵ

// �غ����¼Ӳ��� 2016.3.5
#define w_FluxPluseOnL w_ParLst[74] //.0�ۼ��������
#define w_FluxPluseOnH w_ParLst[75] //.0�ۼ��������

// �غ����¼Ӳ��� 2016.12.20
#define w_DNBPluseOnL w_ParLst[76] // ���������
#define w_DNBPluseOnH w_ParLst[77] // �ۼ��������

// Զ�̼�ؼ��в�ѯ���ò��� 32��
#define w_ProcessNo w_ParLst[80] //.0 �������
// Զ�̼�ؼ��в�ѯ���ò��� 31����
#define w_Pump12Status w_ParLst[81] //.0 ��12״̬
#define w_Pump34Status w_ParLst[82] //.0 ��34״̬			w_dspSysErrorCode1
#define w_Pump56Status w_ParLst[83] //.0 ��56״̬			w_dspOutVoltage
#define w_Flag1Unit w_ParLst[84]	//.0 ��־1��Ԫ
#define w_Flag2Unit w_ParLst[85]	//.0 ��־2��Ԫ
#define w_Flag3Unit w_ParLst[86]	//.0 ��־3��Ԫ
#define w_ResidualCL w_ParLst[87]	//.0 ���� /��־4��Ԫ
#define w_PIDCalcP w_ParLst[88]		//.3 PID����ѹ��
#define w_VvvfFreq w_ParLst[89]		//.1 ��Ƶ��Ƶ��
#define w_InPDec w_ParLst[90]		//.2 ��ˮ��ѹ��
#define w_OutPDec w_ParLst[91]		//.2 ��ˮ��ѹ��
#define w_InstanFlux w_ParLst[92]	//.2 ˲ʱ����		//ZCL 2007.6.15
#define w_Pump1Current w_ParLst[93] //.1 1�űõ���
#define w_Pump2Current w_ParLst[94] //.1 2�űõ���
#define w_Pump3Current w_ParLst[95] //.1 3�űõ���
#define w_Pump4Current w_ParLst[96] //.1 4�űõ���
#define w_Pump5Current w_ParLst[97] //.1 5�űõ���	w_dspNowTemperature
#define w_YeWeiDeep w_ParLst[98]	//.2 Һλ���
#define w_SysVoltage w_ParLst[99]	//.1 ϵͳ��ѹ		//ZCL 2007.6.15  w_dspBusBarVoltage

#define w_SumFluxL w_ParLst[100]  //.0 �ۼ���������(С��ģʽ������ǵ���2011.7.25)
#define w_SumFluxH w_ParLst[101]  //.0 �ۼ���������(С��ģʽ������Ǹ���2011.7.25)
#define w_DDFStatus w_ParLst[102] // 1#�綯��״̬ ��16.4.18
// #define	w_DDF2OpenValue				w_ParLst[103]		// 2#�綯������
// #define	w_DDF3OpenValue				w_ParLst[104]		// 3#�綯������
#define w_SumDNBL w_ParLst[103]		  // �ۼƵ�������(С��ģʽ������ǵ���2011.7.25)
#define w_SumDNBH w_ParLst[104]		  // �ۼƵ�������(С��ģʽ������Ǹ���2011.7.25)
#define w_DDF4OpenValue w_ParLst[105] //.0 4#�綯������
#define w_DDF5OpenValue w_ParLst[106] //.0 5#�綯������		w_dspOutVoltage
#define w_PHValue w_ParLst[107]		  //.0 PHֵ
#define w_ZhuoDuValue w_ParLst[108]	  //.0 �Ƕ�
// 3 ������ʱ���� 368 369 370
#define w_NowYM w_ParLst[109] //.0 ����
#define w_NowDH w_ParLst[110] //.0 ��ʱ
#define w_NowMS w_ParLst[111] //.0 ����

// ��¼��ʾ����  20060812 �ܳ���
#define w_RecProcessNo w_ParLst[120]	//.0 �������  0
#define w_RecPump12Status w_ParLst[121] //.0 ��12״̬
#define w_RecPump34Status w_ParLst[122] //.0 ��34״̬
#define w_RecPump56Status w_ParLst[123] //.0 ��56״̬
#define w_RecFlag1Unit w_ParLst[124]	//.0 ��־1��Ԫ
#define w_RecFlag2Unit w_ParLst[125]	//.0 ��־2��Ԫ
#define w_RecFlag3Unit w_ParLst[126]	//.0 ��־3��Ԫ
#define w_RecResidualCL w_ParLst[127]	//.0 ���� /��־4��Ԫ
#define w_RecPIDCalcP w_ParLst[128]		//.3 PID����ѹ�� 8
#define w_RecVvvfFreq w_ParLst[129]		//.1 ��Ƶ��Ƶ��
#define w_RecInPDec w_ParLst[130]		//.2 ��ˮ��ѹ��
#define w_RecOutPDec w_ParLst[131]		//.2 ��ˮ��ѹ��
#define w_RecSysVoltage w_ParLst[132]	//.1 ϵͳ��ѹ    //ZCL 2007.6.15
#define w_RecInstanFlux w_ParLst[133]	//.2 ˲ʱ����
#define w_RecPump1Current w_ParLst[134] //.1 1�űõ���
#define w_RecPump2Current w_ParLst[135] //.1 2�űõ���
#define w_RecPump3Current w_ParLst[136] //.1 3�űõ���
#define w_RecPump4Current w_ParLst[137] //.1 4�űõ���
#define w_RecPump5Current w_ParLst[138] //.1 5�űõ���
#define w_RecYeWeiDeep w_ParLst[139]	//.2 Һλ���    18����

#define w_RecSumFluxL w_ParLst[141] //.0 �ۼ���������
#define w_RecSumFluxH w_ParLst[142] //.0 �ۼ��������� 21
// 7  143 144 145 146 147 148 149  22-28  1#�綯������-5#�綯������  PHֵ �Ƕ�
// 3 ������ʱ���� 150 151 152      29-31
//
#define w_RecPump1ManuAuto w_ParLst[153] //.0 ��1 �ֶ�/�Զ�
#define w_RecPump1GpVf w_ParLst[154]	 //.0 ��1��Ƶ����Ƶ�����ϣ�ֹͣ
#define w_RecPump2ManuAuto w_ParLst[155] //.0 ��2 �ֶ�/�Զ�
#define w_RecPump2GpVf w_ParLst[156]	 //.0 ��2��Ƶ����Ƶ�����ϣ�ֹͣ
#define w_RecPump3ManuAuto w_ParLst[157] //.0 ��3 �ֶ�/�Զ�
#define w_RecPump3GpVf w_ParLst[158]	 //.0 ��3��Ƶ����Ƶ�����ϣ�ֹͣ
#define w_RecPump4ManuAuto w_ParLst[159] //.0 ��4 �ֶ�/�Զ�
#define w_RecPump4GpVf w_ParLst[160]	 //.0 ��4��Ƶ����Ƶ�����ϣ�ֹͣ
#define w_RecPump5ManuAuto w_ParLst[161] //.0 ��5 �ֶ�/�Զ�
#define w_RecPump5GpVf w_ParLst[162]	 //.0 ��5��Ƶ����Ƶ�����ϣ�ֹͣ
//
#define w_Pump1ManuAuto w_ParLst[163] //.0 ��1 �ֶ�/�Զ�
#define w_Pump1GpVf w_ParLst[164]	  //.0 ��1��Ƶ����Ƶ�����ϣ�ֹͣ
#define w_Pump2ManuAuto w_ParLst[165] //.0 ��2 �ֶ�/�Զ�
#define w_Pump2GpVf w_ParLst[166]	  //.0 ��2��Ƶ����Ƶ�����ϣ�ֹͣ
#define w_Pump3ManuAuto w_ParLst[167] //.0 ��3 �ֶ�/�Զ�
#define w_Pump3GpVf w_ParLst[168]	  //.0 ��3��Ƶ����Ƶ�����ϣ�ֹͣ
#define w_Pump4ManuAuto w_ParLst[169] //.0 ��4 �ֶ�/�Զ�
#define w_Pump4GpVf w_ParLst[170]	  //.0 ��4��Ƶ����Ƶ�����ϣ�ֹͣ
#define w_Pump5ManuAuto w_ParLst[171] //.0 ��5 �ֶ�/�Զ�
#define w_Pump5GpVf w_ParLst[172]	  //.0 ��5��Ƶ����Ƶ�����ϣ�ֹͣ
// 173-187 ԭ�ȱ�1-��5 �ۼ�����ʱ��
#define w_Pump1RunSecond w_ParLst[173] //.0 ��1������
#define w_Pump1RunHour w_ParLst[174]   //.0 ��1����Сʱ
// 187

#define w_TSensorValue w_ParLst[188] //.0 �¶ȴ��������ֵ

#define w_KglInNum w_ParLst[189]	 //.0
#define w_KglOutNum w_ParLst[190]	 //.0
#define w_KglInStatus w_ParLst[191]	 //.0
#define w_KglOutStatus w_ParLst[192] //.0
#define w_WriteDate w_ParLst[193]	 //.0�����д����
#define w_Writetime w_ParLst[194]	 //.0�����дʱ��

#define w_AQ1 w_ParLst[202] //.0 ģ�������ֵ1
#define w_AQ2 w_ParLst[203] //.0 ģ�������ֵ2
#define w_AI1 w_ParLst[204] //.0 ģ��������ֵ1
#define w_AI2 w_ParLst[205] //.0
#define w_AI3 w_ParLst[206] //.0
#define w_AI4 w_ParLst[207] //.0
#define w_AI5 w_ParLst[208] //.0
#define w_AI6 w_ParLst[209] //.0
#define w_AI7 w_ParLst[210] //.0
#define w_AI8 w_ParLst[211] //.0 ģ��������ֵ8

#define w_ReadDspOKCount w_ParLst[212]		//.0 Com1 03ָ�������ȷ���ش���
#define w_WriteDspOKCount w_ParLst[213]		//.0 Com1 06ָ�������ȷ���ش���
#define w_ReadDspErrCount w_ParLst[214]		//.0 ��DSP�������
#define w_WriteDspErrCount w_ParLst[215]	//.0 дDSP�������
#define w_RdDspNo w_ParLst[216]				//.0 ��Dsp ���
#define w_WrDspNo w_ParLst[217]				//.0 дDsp ���
#define w_ResetedDspCount w_ParLst[218]		//.0 �Ѿ���λDSP�Ĵ���
#define w_ResetDspDelayCount w_ParLst[219]	//.0 ��λDSP��ʱ����������λǰ����ʱ��
#define w_ResetDspDelayCount2 w_ParLst[220] //.0 ��λDSP��ʱ������2����λǰ����ʱ��

#define w_ManMadedspFault w_ParLst[221] //.0 ��Ϊ������� ������ ZCL 2018.6.5
#define w_DspStopNum w_ParLst[222]		//.0 DSPͣ������ 2018.8.27
#define w_DspStopStatus w_ParLst[223]	//.0 DSPͣ��ԭ�� 2018.8.27

#define w_SaveBaudRate0 w_ParLst[230] //.0 �����ʱ���ֵ0
#define w_SaveBaudRate1 w_ParLst[231] //.0 �����ʱ���ֵ1
#define w_SaveBaudRate2 w_ParLst[232] //.0 �����ʱ���ֵ2
#define w_SaveBaudRate3 w_ParLst[233] //.0 �����ʱ���ֵ3

//.0#define	w_SystemRunMode  					w_ParLst[234]	  // ��Ƶ��ģʽ	-- �ߵ�ƽ

#define w_ModDspParAdd w_ParLst[236]   //.0 �޸�DSP�����ĵ�ַ
#define w_ModDspParValue w_ParLst[237] //.0 �޸�DSP������ֵ

#define w_DspOperateStatus w_ParLst[238] //.0 DSP����״̬
#define w_DspOperateNum w_ParLst[239]	 //.0 DSP����״̬����
#define w_DspAlarmStatus w_ParLst[240]	 //.0 DSPͣ��ԭ�򣨱�����
#define w_DspAlarmNum w_ParLst[241]		 //.0 DSPͣ��������������

#define w_FmramErrL w_ParLst[242] //.0 Fmram ��д������� L
#define w_FmramErrH w_ParLst[243] //.0 Fmram ��д������� H

#define w_SetSecond w_ParLst[245] //.0 ������
#define w_SetMinute w_ParLst[246] //.0 ���÷�
#define w_SetHour w_ParLst[247]	  //.0 ����ʱ
#define w_SetDay w_ParLst[248]	  //.0 ������
#define w_SetMonth w_ParLst[249]  //.0 ������
#define w_SetYear w_ParLst[250]	  //.0 ������
#define w_SetWeek w_ParLst[251]	  //.0 ��������

#define w_AutoAssignTime w_ParLst[252] //.0�Զ�����ʱ��
#define w_SmallVfUpValue w_ParLst[253] //.0С��������ʱƵ��ÿ������ֵ  �� 2013.4.22
// �¼� ��16.4.18
#define w_ChangeWaterRunningHour w_ParLst[254]	// �л�ˮԴ����ˮ�����е�ʱ��
#define w_ChangeWaterRunningHour2 w_ParLst[255] // �л�ˮԴˮ�������е�ʱ��

// #define	w_TextWriteYear				w_ParLst[253]		//�����д����		ZCL 2016.10.06
// #define	w_TextWriteDate				w_ParLst[254]		//�����дʱ��

// ZCL 2019.10.19  YW310����
//(��YW310�еĶ��壬�ĳ���DCM220_SCR_E�ж��壬��ַ����)
//  #define	Pw_FWenDecMax						w_ParLst[1]		// ���¶����̡���ʼ��ֵ50����Ӧ50��			ZCL 2017.11.17
//  #define	Pw_ZWenDecMax						w_ParLst[2]		// ���¶����̡���ʼ��ֵ100����Ӧ100��	ZCL 2017.11.17
//  #define	Pw_PressureWenDuSel			w_ParLst[3]		// ѹ���¶�ѡ��	ZCL 2017.11.17 =0ѹ����=1�¶�
//  #define	Pw_PressureWenDuXiShu		w_ParLst[4]		// ѹ���¶�ϵ��	ZCL 2017.12.16

// #define	Pw_ModPar								w_ParLst[9]		// �޸Ĳ���
// #define	Pw_ParInitial						w_ParLst[23]	// ������ʼ��
// #define	Pw_EquipmentNo					w_ParLst[11]	// �豸ͨѶ��ַ
// #define	w_SetEquipmentNoST			w_ParLst[156]	// �趨��ַ״̬ ZCL 2019.10.19  �ڴ�״̬�ˣ������豸��ַ��Ҳ����ͨѶ�ϡ�
// #define	w_Counter							  w_ParLst[157]	// ZCL 2019.10.19 ��������ͨѶʱ�۲��á�YW310 ��H1ͨѶָʾ��
// #define		w_SoftVer							w_ParLst[142]		// ����汾
// #define	Pw_MaxAIHex			 				w_ParLst[54]	// ���ģ������Hexֵ��У��ģ�����룬һ�㲻�õ���
// #define	Pw_AI1SensorZero				w_ParLst[60]		// AI1��������ֵ

#define Pw_YW310FWenDecMax w_ParLst[256]		 // ���¶����̡���ʼ��ֵ50����Ӧ50��			ZCL 2017.11.17
#define Pw_YW310ZWenDecMax w_ParLst[257]		 // ���¶����̡���ʼ��ֵ100����Ӧ100��	ZCL 2017.11.17
#define Pw_YW310PressureWenDuSel w_ParLst[258]	 // ѹ���¶�ѡ��	ZCL 2017.11.17 =0ѹ����=1�¶�
#define Pw_YW310PressureWenDuXiShu w_ParLst[259] // ѹ���¶�ϵ��	ZCL 2017.12.16

#define Pw_YW310ModPar w_ParLst[260]		  // �޸Ĳ���
#define Pw_YW310ParInitial w_ParLst[261]	  // ������ʼ��
#define Pw_YW310EquipmentNo w_ParLst[262]	  // �豸ͨѶ��ַ
#define w_YW310SetEquipmentNoST w_ParLst[263] // �趨��ַ״̬ ZCL 2019.10.19  �ڴ�״̬�ˣ������豸��ַ��Ҳ����ͨѶ�ϡ�
#define w_YW310Counter w_ParLst[264]		  // ZCL 2019.10.19 ��������ͨѶʱ�۲��á�YW310 ��H1ͨѶָʾ��
#define w_YW310SoftVer w_ParLst[265]		  // ����汾

#define Pw_YW310MaxAIHex w_ParLst[266]		// ���ģ������Hexֵ��У��ģ�����룬һ�㲻�õ���
#define Pw_YW310AI1SensorZero w_ParLst[267] // AI1��������ֵ

// ��������ͬ���ĵ�ַ
#define Pw_ZM220Current1 w_ParLst[266]	// ��ѹ������� ZCL 2022.3.15
#define Pw_ZM220HotMinute w_ParLst[267] // ���ȷ����� ZCL 2022.3.15

// ZCL 2019.10.19  SZM220 LORA����
//  #define	Pw_ModPar										w_ParLst[9]		// �޸Ĳ���
//  #define	Pw_ParInitial								w_ParLst[23]	// ������ʼ��
//  #define	Pw_LoRaEquipmentNo					w_ParLst[212]		// LORA�豸ͨѶ��ַ ZCL 2018.12.13
//  #define	Pw_LoRaMasterSlaveSel				w_ParLst[213]		// �����ӻ�ѡ��
//  #define	Pw_ZM220LoRaSet							w_ParLst[218]		//.0 LoRa �趨  LORA SX1278
// LORA ������趨���� LoRaSettings  sx1276-LoRa.c
// #define	Pw_LoRaSetFreq							w_ParLst[226]	// ���� 4350000
// #define	Pw_LoRaSetPower							w_ParLst[227]	// ���� 20
// #define	Pw_LoRaSetSignalBW					w_ParLst[228]	// ���� 0-9
// #define	Pw_LoRaSetSpreadingFactor		w_ParLst[229]	// ���� 6-12
// #define	Pw_LoRaSetErrorCoding				w_ParLst[230]	// ���� 1-4
// #define	Pw_LoRaSetCrcOn							w_ParLst[231]	// 0-1
// #define	Pw_LoRaSetImplicitHeaderOn	w_ParLst[232]	// 0-1
// #define	Pw_LoRaSetRxSingleOn				w_ParLst[233]	// 0-1
// #define	Pw_LoRaSetFreqHopOn					w_ParLst[234]	// 0
// #define	Pw_LoRaSetHopPeriod					w_ParLst[235]	//
// #define	Pw_LoRaSetPayLoadLength			w_ParLst[236]	//
// #define	Pw_LoRaSetTxPacketTimeOut		w_ParLst[237]	//.0 //ZCL 2019.9.24 �����¼���
// #define	Pw_LoRaSetRxPacketTimeOut		w_ParLst[238]	//.0 //ZCL 2019.9.24 �����¼���
// #define	w_LoRaDateRate							w_ParLst[501]	//.2 ��������	 kbps
// #define	w_Counter										w_ParLst[641]		//.0 ������
// #define	w_SoftVer										w_ParLst[640]		//.2 ����汾
#define Pw_ZM220ModPar w_ParLst[260]	 // �޸Ĳ���
#define Pw_ZM220ParInitial w_ParLst[261] // ������ʼ��

#define Pw_ZM220LoRaEquipmentNo w_ParLst[268]		  // LORA�豸ͨѶ��ַ ZCL 2018.12.13
#define Pw_ZM220LoRaMasterSlaveSel w_ParLst[269]	  // �����ӻ�ѡ��
#define Pw_ZM220LoRaSet w_ParLst[270]				  //.0 LoRa �趨  LORA SX1278
													  // LORA ������趨���� LoRaSettings  sx1276-LoRa.c
#define Pw_ZM220LoRaSetFreq w_ParLst[271]			  // ���� 4350000
#define Pw_ZM220LoRaSetPower w_ParLst[272]			  // ���� 20
#define Pw_ZM220LoRaSetSignalBW w_ParLst[273]		  // ���� 0-9
#define Pw_ZM220LoRaSetSpreadingFactor w_ParLst[274]  // ���� 6-12
#define Pw_ZM220LoRaSetErrorCoding w_ParLst[275]	  // ���� 1-4
#define Pw_ZM220LoRaSetCrcOn w_ParLst[276]			  // 0-1
#define Pw_ZM220LoRaSetImplicitHeaderOn w_ParLst[277] // 0-1
#define Pw_ZM220LoRaSetRxSingleOn w_ParLst[278]		  // 0-1
#define Pw_ZM220LoRaSetFreqHopOn w_ParLst[279]		  // 0
#define Pw_ZM220LoRaSetHopPeriod w_ParLst[280]		  //
#define Pw_ZM220LoRaSetPayLoadLength w_ParLst[281]	  //
#define Pw_ZM220LoRaSetTxPacketTimeOut w_ParLst[282]  //.0 //ZCL 2019.9.24 �����¼���
#define Pw_ZM220LoRaSetRxPacketTimeOut w_ParLst[283]  //.0 //ZCL 2019.9.24 �����¼���
#define w_ZM220LoRaDateRate w_ParLst[286]			  //.2 ��������	 kbps
#define w_ZM220Counter w_ParLst[287]				  //.0 ������
#define w_ZM220SoftVer w_ParLst[288]				  //.2 ����汾

//
// ZCL 2019.3.20˵��
// w_ScrGpsLatAllDu1 �� w_ScrGpsLatAllDu2 ��������γ��
// w_ScrGpsLonAllDu1 �� w_ScrGpsLonAllDu2 �������Ǿ���
#define w_ScrGpsLatAllDu1 w_ParLst[300 + 0 * 24 + 0 - 4] //.2 γ�� ��1�� �磺35.44			2λС��
#define w_ScrGpsLatAllDu2 w_ParLst[300 + 0 * 24 + 0 - 3] //.0 γ�� ��2��	00.00****
														 //****���֣���ӵ�w_ScrGpsLatAllDu1���档 �磺****�� 2459�� ����γ���ǣ� 35.442459
#define w_ScrGpsLonAllDu1 w_ParLst[300 + 0 * 24 + 0 - 2] //.2 ���� ��1��	�磺127.39		2λС��
#define w_ScrGpsLonAllDu2 w_ParLst[300 + 0 * 24 + 0 - 1] //.0 ���� ��2�� 00.00****
														 //****���֣���ӵ�w_ScrGpsLonAllDu1���档�磺****�� 4628�� ���ܾ����ǣ� 127.394628

// ZCL 2019.10.15 ��ѹ����ɼ�
// ZCL 2019.9.28
// ���1
#define w_HighV1SoftVersion w_ParLst[300 + 0 * 24 + 0]	  // �汾
#define w_HighV1Counter w_ParLst[300 + 0 * 24 + 1]		  // ������
#define w_HighV1WenDu1DecValue w_ParLst[300 + 0 * 24 + 2] //.1 �¶�ֵ1
#define w_HighV1WenDu2DecValue w_ParLst[300 + 0 * 24 + 3] //.1 �¶�ֵ2
#define w_HighV1WenDu3DecValue w_ParLst[300 + 0 * 24 + 4] //.1 �¶�ֵ3
#define w_HighV1WenDu4DecValue w_ParLst[300 + 0 * 24 + 5] //.1 �¶�ֵ4
#define w_HighV1WenDu5DecValue w_ParLst[300 + 0 * 24 + 6] //.1 �¶�ֵ5
#define w_HighV1WenDu6DecValue w_ParLst[300 + 0 * 24 + 7] //.1 �¶�ֵ6

#define w_HighV1DianLi1 w_ParLst[300 + 0 * 24 + 8]	 //.x ��������1
#define w_HighV1DianLi2 w_ParLst[300 + 0 * 24 + 9]	 //.x ��������1
#define w_HighV1DianLi3 w_ParLst[300 + 0 * 24 + 10]	 //.x ��������1
#define w_HighV1DianLi4 w_ParLst[300 + 0 * 24 + 11]	 //.x ��������1
#define w_HighV1DianLi5 w_ParLst[300 + 0 * 24 + 12]	 //.x ��������1
#define w_HighV1DianLi6 w_ParLst[300 + 0 * 24 + 13]	 //.x ��������1
#define w_HighV1DianLi7 w_ParLst[300 + 0 * 24 + 14]	 //.x ��������1
#define w_HighV1DianLi8 w_ParLst[300 + 0 * 24 + 15]	 //.x ��������1
#define w_HighV1DianLi9 w_ParLst[300 + 0 * 24 + 16]	 //.x ��������1
#define w_HighV1DianLi10 w_ParLst[300 + 0 * 24 + 17] //.x ��������1
#define w_HighV1DianLi11 w_ParLst[300 + 0 * 24 + 18] //.x ��������1
#define w_HighV1DianLi12 w_ParLst[300 + 0 * 24 + 19] //.x ��������1
#define w_HighV1DianLi13 w_ParLst[300 + 0 * 24 + 20] //.x ��������1
#define w_HighV1DianLi14 w_ParLst[300 + 0 * 24 + 21] //.x ��������1
#define w_HighV1DianLi15 w_ParLst[300 + 0 * 24 + 22] //.x ��������1
#define w_HighV1DianLi16 w_ParLst[300 + 0 * 24 + 23] //.x ��������1

// w_HighV1DispDianLi1=(u16)(FtoU32(w_HighV1DianLi1,w_HighV1DianLi2)/10);		//�ߵ�ѹ1  ȥ��1λ ���2λС��  10110 ��� 10.11kV
// w_HighV1DispDianLi2=(u16)(FtoU32(w_HighV1DianLi3,w_HighV1DianLi4)/10);		//�ߵ�ѹ2  ȥ��1λ ���2λС��  10110 ��� 10.11kV
// w_HighV1DispDianLi3=(u16)(FtoU32(w_HighV1DianLi5,w_HighV1DianLi6)/10);		//�ߵ�ѹ3  ȥ��1λ ���2λС��  10110 ��� 10.11kV
// w_HighV1DispDianLi4=(u16)(FtoU32(w_HighV1DianLi7,w_HighV1DianLi8));			//�����1
// w_HighV1DispDianLi5=(u16)(FtoU32(w_HighV1DianLi9,w_HighV1DianLi10));			//�����2
// w_HighV1DispDianLi6=(u16)(FtoU32(w_HighV1DianLi11,w_HighV1DianLi12));		//�����3
// w_HighV1DispDianLi7=(u16)(FtoU32(w_HighV1DianLi13,w_HighV1DianLi14));		//���й�����
// w_HighV1DispDianLi8=(u16)(FtoU32(w_HighV1DianLi15,w_HighV1DianLi16));		//Ƶ��

// ���2
#define w_HighV2SoftVersion w_ParLst[300 + 1 * 24 + 0]	  // ������
#define w_HighV2Counter w_ParLst[300 + 1 * 24 + 1]		  // ������
#define w_HighV2WenDu1DecValue w_ParLst[300 + 1 * 24 + 2] //.1 �¶�ֵ1
#define w_HighV2WenDu2DecValue w_ParLst[300 + 1 * 24 + 3] //.1 �¶�ֵ2
#define w_HighV2WenDu3DecValue w_ParLst[300 + 1 * 24 + 4] //.1 �¶�ֵ3
#define w_HighV2WenDu4DecValue w_ParLst[300 + 1 * 24 + 5] //.1 �¶�ֵ4
#define w_HighV2WenDu5DecValue w_ParLst[300 + 1 * 24 + 6] //.1 �¶�ֵ5
#define w_HighV2WenDu6DecValue w_ParLst[300 + 1 * 24 + 7] //.1 �¶�ֵ6

#define w_HighV2DianLi1 w_ParLst[300 + 1 * 24 + 8]	 //.x ��������1
#define w_HighV2DianLi2 w_ParLst[300 + 1 * 24 + 9]	 //.x ��������1
#define w_HighV2DianLi3 w_ParLst[300 + 1 * 24 + 10]	 //.x ��������1
#define w_HighV2DianLi4 w_ParLst[300 + 1 * 24 + 11]	 //.x ��������1
#define w_HighV2DianLi5 w_ParLst[300 + 1 * 24 + 12]	 //.x ��������1
#define w_HighV2DianLi6 w_ParLst[300 + 1 * 24 + 13]	 //.x ��������1
#define w_HighV2DianLi7 w_ParLst[300 + 1 * 24 + 14]	 //.x ��������1
#define w_HighV2DianLi8 w_ParLst[300 + 1 * 24 + 15]	 //.x ��������1
#define w_HighV2DianLi9 w_ParLst[300 + 1 * 24 + 16]	 //.x ��������1
#define w_HighV2DianLi10 w_ParLst[300 + 1 * 24 + 17] //.x ��������1
#define w_HighV2DianLi11 w_ParLst[300 + 1 * 24 + 18] //.x ��������1
#define w_HighV2DianLi12 w_ParLst[300 + 1 * 24 + 19] //.x ��������1
#define w_HighV2DianLi13 w_ParLst[300 + 1 * 24 + 20] //.x ��������1
#define w_HighV2DianLi14 w_ParLst[300 + 1 * 24 + 21] //.x ��������1
#define w_HighV2DianLi15 w_ParLst[300 + 1 * 24 + 22] //.x ��������1
#define w_HighV2DianLi16 w_ParLst[300 + 1 * 24 + 23] //.x ��������1

// ���3
#define w_HighV3SoftVersion w_ParLst[300 + 2 * 24 + 0]	  // ������
#define w_HighV3Counter w_ParLst[300 + 2 * 24 + 1]		  // ������
#define w_HighV3WenDu1DecValue w_ParLst[300 + 2 * 24 + 2] //.1 �¶�ֵ1
#define w_HighV3WenDu2DecValue w_ParLst[300 + 2 * 24 + 3] //.1 �¶�ֵ2
#define w_HighV3WenDu3DecValue w_ParLst[300 + 2 * 24 + 4] //.1 �¶�ֵ3
#define w_HighV3WenDu4DecValue w_ParLst[300 + 2 * 24 + 5] //.1 �¶�ֵ4
#define w_HighV3WenDu5DecValue w_ParLst[300 + 2 * 24 + 6] //.1 �¶�ֵ5
#define w_HighV3WenDu6DecValue w_ParLst[300 + 2 * 24 + 7] //.1 �¶�ֵ6

#define w_HighV3DianLi1 w_ParLst[300 + 2 * 24 + 8]	 //.x ��������1
#define w_HighV3DianLi2 w_ParLst[300 + 2 * 24 + 9]	 //.x ��������1
#define w_HighV3DianLi3 w_ParLst[300 + 2 * 24 + 10]	 //.x ��������1
#define w_HighV3DianLi4 w_ParLst[300 + 2 * 24 + 11]	 //.x ��������1
#define w_HighV3DianLi5 w_ParLst[300 + 2 * 24 + 12]	 //.x ��������1
#define w_HighV3DianLi6 w_ParLst[300 + 2 * 24 + 13]	 //.x ��������1
#define w_HighV3DianLi7 w_ParLst[300 + 2 * 24 + 14]	 //.x ��������1
#define w_HighV3DianLi8 w_ParLst[300 + 2 * 24 + 15]	 //.x ��������1
#define w_HighV3DianLi9 w_ParLst[300 + 2 * 24 + 16]	 //.x ��������1
#define w_HighV3DianLi10 w_ParLst[300 + 2 * 24 + 17] //.x ��������1
#define w_HighV3DianLi11 w_ParLst[300 + 2 * 24 + 18] //.x ��������1
#define w_HighV3DianLi12 w_ParLst[300 + 2 * 24 + 19] //.x ��������1
#define w_HighV3DianLi13 w_ParLst[300 + 2 * 24 + 20] //.x ��������1
#define w_HighV3DianLi14 w_ParLst[300 + 2 * 24 + 21] //.x ��������1
#define w_HighV3DianLi15 w_ParLst[300 + 2 * 24 + 22] //.x ��������1
#define w_HighV3DianLi16 w_ParLst[300 + 2 * 24 + 23] //.x ��������1

// ���4
#define w_HighV4SoftVersion w_ParLst[300 + 3 * 24 + 0]	  // ������
#define w_HighV4Counter w_ParLst[300 + 3 * 24 + 1]		  // ������
#define w_HighV4WenDu1DecValue w_ParLst[300 + 3 * 24 + 2] //.1 �¶�ֵ1
#define w_HighV4WenDu2DecValue w_ParLst[300 + 3 * 24 + 3] //.1 �¶�ֵ2
#define w_HighV4WenDu3DecValue w_ParLst[300 + 3 * 24 + 4] //.1 �¶�ֵ3
#define w_HighV4WenDu4DecValue w_ParLst[300 + 3 * 24 + 5] //.1 �¶�ֵ4
#define w_HighV4WenDu5DecValue w_ParLst[300 + 3 * 24 + 6] //.1 �¶�ֵ5
#define w_HighV4WenDu6DecValue w_ParLst[300 + 3 * 24 + 7] //.1 �¶�ֵ6

#define w_HighV4DianLi1 w_ParLst[300 + 3 * 24 + 8]	 //.x ��������1
#define w_HighV4DianLi2 w_ParLst[300 + 3 * 24 + 9]	 //.x ��������1
#define w_HighV4DianLi3 w_ParLst[300 + 3 * 24 + 10]	 //.x ��������1
#define w_HighV4DianLi4 w_ParLst[300 + 3 * 24 + 11]	 //.x ��������1
#define w_HighV4DianLi5 w_ParLst[300 + 3 * 24 + 12]	 //.x ��������1
#define w_HighV4DianLi6 w_ParLst[300 + 3 * 24 + 13]	 //.x ��������1
#define w_HighV4DianLi7 w_ParLst[300 + 3 * 24 + 14]	 //.x ��������1
#define w_HighV4DianLi8 w_ParLst[300 + 3 * 24 + 15]	 //.x ��������1
#define w_HighV4DianLi9 w_ParLst[300 + 3 * 24 + 16]	 //.x ��������1
#define w_HighV4DianLi10 w_ParLst[300 + 3 * 24 + 17] //.x ��������1
#define w_HighV4DianLi11 w_ParLst[300 + 3 * 24 + 18] //.x ��������1
#define w_HighV4DianLi12 w_ParLst[300 + 3 * 24 + 19] //.x ��������1
#define w_HighV4DianLi13 w_ParLst[300 + 3 * 24 + 20] //.x ��������1
#define w_HighV4DianLi14 w_ParLst[300 + 3 * 24 + 21] //.x ��������1
#define w_HighV4DianLi15 w_ParLst[300 + 3 * 24 + 22] //.x ��������1
#define w_HighV4DianLi16 w_ParLst[300 + 3 * 24 + 23] //.x ��������1

// ���5
#define w_HighV5SoftVersion w_ParLst[300 + 4 * 24 + 0]	  // ������
#define w_HighV5Counter w_ParLst[300 + 4 * 24 + 1]		  // ������
#define w_HighV5WenDu1DecValue w_ParLst[300 + 4 * 24 + 2] //.1 �¶�ֵ1
#define w_HighV5WenDu2DecValue w_ParLst[300 + 4 * 24 + 3] //.1 �¶�ֵ2
#define w_HighV5WenDu3DecValue w_ParLst[300 + 4 * 24 + 4] //.1 �¶�ֵ3
#define w_HighV5WenDu4DecValue w_ParLst[300 + 4 * 24 + 5] //.1 �¶�ֵ4
#define w_HighV5WenDu5DecValue w_ParLst[300 + 4 * 24 + 6] //.1 �¶�ֵ5
#define w_HighV5WenDu6DecValue w_ParLst[300 + 4 * 24 + 7] //.1 �¶�ֵ6

#define w_HighV5DianLi1 w_ParLst[300 + 4 * 24 + 8]	 //.x ��������1
#define w_HighV5DianLi2 w_ParLst[300 + 4 * 24 + 9]	 //.x ��������1
#define w_HighV5DianLi3 w_ParLst[300 + 4 * 24 + 10]	 //.x ��������1
#define w_HighV5DianLi4 w_ParLst[300 + 4 * 24 + 11]	 //.x ��������1
#define w_HighV5DianLi5 w_ParLst[300 + 4 * 24 + 12]	 //.x ��������1
#define w_HighV5DianLi6 w_ParLst[300 + 4 * 24 + 13]	 //.x ��������1
#define w_HighV5DianLi7 w_ParLst[300 + 4 * 24 + 14]	 //.x ��������1
#define w_HighV5DianLi8 w_ParLst[300 + 4 * 24 + 15]	 //.x ��������1
#define w_HighV5DianLi9 w_ParLst[300 + 4 * 24 + 16]	 //.x ��������1
#define w_HighV5DianLi10 w_ParLst[300 + 4 * 24 + 17] //.x ��������1
#define w_HighV5DianLi11 w_ParLst[300 + 4 * 24 + 18] //.x ��������1
#define w_HighV5DianLi12 w_ParLst[300 + 4 * 24 + 19] //.x ��������1
#define w_HighV5DianLi13 w_ParLst[300 + 4 * 24 + 20] //.x ��������1
#define w_HighV5DianLi14 w_ParLst[300 + 4 * 24 + 21] //.x ��������1
#define w_HighV5DianLi15 w_ParLst[300 + 4 * 24 + 22] //.x ��������1
#define w_HighV5DianLi16 w_ParLst[300 + 4 * 24 + 23] //.x ��������1

// ���6
#define w_HighV6SoftVersion w_ParLst[300 + 5 * 24 + 0]	  // ������
#define w_HighV6Counter w_ParLst[300 + 5 * 24 + 1]		  // ������
#define w_HighV6WenDu1DecValue w_ParLst[300 + 5 * 24 + 2] //.1 �¶�ֵ1
#define w_HighV6WenDu2DecValue w_ParLst[300 + 5 * 24 + 3] //.1 �¶�ֵ2
#define w_HighV6WenDu3DecValue w_ParLst[300 + 5 * 24 + 4] //.1 �¶�ֵ3
#define w_HighV6WenDu4DecValue w_ParLst[300 + 5 * 24 + 5] //.1 �¶�ֵ4
#define w_HighV6WenDu5DecValue w_ParLst[300 + 5 * 24 + 6] //.1 �¶�ֵ5
#define w_HighV6WenDu6DecValue w_ParLst[300 + 5 * 24 + 7] //.1 �¶�ֵ6

#define w_HighV6DianLi1 w_ParLst[300 + 5 * 24 + 8]	 //.x ��������1
#define w_HighV6DianLi2 w_ParLst[300 + 5 * 24 + 9]	 //.x ��������1
#define w_HighV6DianLi3 w_ParLst[300 + 5 * 24 + 10]	 //.x ��������1
#define w_HighV6DianLi4 w_ParLst[300 + 5 * 24 + 11]	 //.x ��������1
#define w_HighV6DianLi5 w_ParLst[300 + 5 * 24 + 12]	 //.x ��������1
#define w_HighV6DianLi6 w_ParLst[300 + 5 * 24 + 13]	 //.x ��������1
#define w_HighV6DianLi7 w_ParLst[300 + 5 * 24 + 14]	 //.x ��������1
#define w_HighV6DianLi8 w_ParLst[300 + 5 * 24 + 15]	 //.x ��������1
#define w_HighV6DianLi9 w_ParLst[300 + 5 * 24 + 16]	 //.x ��������1
#define w_HighV6DianLi10 w_ParLst[300 + 5 * 24 + 17] //.x ��������1
#define w_HighV6DianLi11 w_ParLst[300 + 5 * 24 + 18] //.x ��������1
#define w_HighV6DianLi12 w_ParLst[300 + 5 * 24 + 19] //.x ��������1
#define w_HighV6DianLi13 w_ParLst[300 + 5 * 24 + 20] //.x ��������1
#define w_HighV6DianLi14 w_ParLst[300 + 5 * 24 + 21] //.x ��������1
#define w_HighV6DianLi15 w_ParLst[300 + 5 * 24 + 22] //.x ��������1
#define w_HighV6DianLi16 w_ParLst[300 + 5 * 24 + 23] //.x ��������1

// ���7
#define w_HighV7SoftVersion w_ParLst[300 + 6 * 24 + 0]	  // ������
#define w_HighV7Counter w_ParLst[300 + 6 * 24 + 1]		  // ������
#define w_HighV7WenDu1DecValue w_ParLst[300 + 6 * 24 + 2] //.1 �¶�ֵ1
#define w_HighV7WenDu2DecValue w_ParLst[300 + 6 * 24 + 3] //.1 �¶�ֵ2
#define w_HighV7WenDu3DecValue w_ParLst[300 + 6 * 24 + 4] //.1 �¶�ֵ3
#define w_HighV7WenDu4DecValue w_ParLst[300 + 6 * 24 + 5] //.1 �¶�ֵ4
#define w_HighV7WenDu5DecValue w_ParLst[300 + 6 * 24 + 6] //.1 �¶�ֵ5
#define w_HighV7WenDu6DecValue w_ParLst[300 + 6 * 24 + 7] //.1 �¶�ֵ6

#define w_HighV7DianLi1 w_ParLst[300 + 6 * 24 + 8]	 //.x ��������1
#define w_HighV7DianLi2 w_ParLst[300 + 6 * 24 + 9]	 //.x ��������1
#define w_HighV7DianLi3 w_ParLst[300 + 6 * 24 + 10]	 //.x ��������1
#define w_HighV7DianLi4 w_ParLst[300 + 6 * 24 + 11]	 //.x ��������1
#define w_HighV7DianLi5 w_ParLst[300 + 6 * 24 + 12]	 //.x ��������1
#define w_HighV7DianLi6 w_ParLst[300 + 6 * 24 + 13]	 //.x ��������1
#define w_HighV7DianLi7 w_ParLst[300 + 6 * 24 + 14]	 //.x ��������1
#define w_HighV7DianLi8 w_ParLst[300 + 6 * 24 + 15]	 //.x ��������1
#define w_HighV7DianLi9 w_ParLst[300 + 6 * 24 + 16]	 //.x ��������1
#define w_HighV7DianLi10 w_ParLst[300 + 6 * 24 + 17] //.x ��������1
#define w_HighV7DianLi11 w_ParLst[300 + 6 * 24 + 18] //.x ��������1
#define w_HighV7DianLi12 w_ParLst[300 + 6 * 24 + 19] //.x ��������1
#define w_HighV7DianLi13 w_ParLst[300 + 6 * 24 + 20] //.x ��������1
#define w_HighV7DianLi14 w_ParLst[300 + 6 * 24 + 21] //.x ��������1
#define w_HighV7DianLi15 w_ParLst[300 + 6 * 24 + 22] //.x ��������1
#define w_HighV7DianLi16 w_ParLst[300 + 6 * 24 + 23] //.x ��������1

// ���8
#define w_HighV8SoftVersion w_ParLst[300 + 7 * 24 + 0]	  // ������
#define w_HighV8Counter w_ParLst[300 + 7 * 24 + 1]		  // ������
#define w_HighV8WenDu1DecValue w_ParLst[300 + 7 * 24 + 2] //.1 �¶�ֵ1
#define w_HighV8WenDu2DecValue w_ParLst[300 + 7 * 24 + 3] //.1 �¶�ֵ2
#define w_HighV8WenDu3DecValue w_ParLst[300 + 7 * 24 + 4] //.1 �¶�ֵ3
#define w_HighV8WenDu4DecValue w_ParLst[300 + 7 * 24 + 5] //.1 �¶�ֵ4
#define w_HighV8WenDu5DecValue w_ParLst[300 + 7 * 24 + 6] //.1 �¶�ֵ5
#define w_HighV8WenDu6DecValue w_ParLst[300 + 7 * 24 + 7] //.1 �¶�ֵ6

#define w_HighV8DianLi1 w_ParLst[300 + 7 * 24 + 8]	 //.x ��������1
#define w_HighV8DianLi2 w_ParLst[300 + 7 * 24 + 9]	 //.x ��������1
#define w_HighV8DianLi3 w_ParLst[300 + 7 * 24 + 10]	 //.x ��������1
#define w_HighV8DianLi4 w_ParLst[300 + 7 * 24 + 11]	 //.x ��������1
#define w_HighV8DianLi5 w_ParLst[300 + 7 * 24 + 12]	 //.x ��������1
#define w_HighV8DianLi6 w_ParLst[300 + 7 * 24 + 13]	 //.x ��������1
#define w_HighV8DianLi7 w_ParLst[300 + 7 * 24 + 14]	 //.x ��������1
#define w_HighV8DianLi8 w_ParLst[300 + 7 * 24 + 15]	 //.x ��������1
#define w_HighV8DianLi9 w_ParLst[300 + 7 * 24 + 16]	 //.x ��������1
#define w_HighV8DianLi10 w_ParLst[300 + 7 * 24 + 17] //.x ��������1
#define w_HighV8DianLi11 w_ParLst[300 + 7 * 24 + 18] //.x ��������1
#define w_HighV8DianLi12 w_ParLst[300 + 7 * 24 + 19] //.x ��������1
#define w_HighV8DianLi13 w_ParLst[300 + 7 * 24 + 20] //.x ��������1
#define w_HighV8DianLi14 w_ParLst[300 + 7 * 24 + 21] //.x ��������1
#define w_HighV8DianLi15 w_ParLst[300 + 7 * 24 + 22] //.x ��������1
#define w_HighV8DianLi16 w_ParLst[300 + 7 * 24 + 23] //.x ��������1

// ZCL 2019.10.15 ��ѹ����ɼ�
// ZCL 2019.9.28
// ���1
// ZCL 2019.10.18  Һ������ʾֵ
#define w_HighV1DispSoftVersion w_ParLst[300 + 8 * 24 + 0]	  // �汾
#define w_HighV1DispCounter w_ParLst[300 + 8 * 24 + 1]		  // ������
#define w_HighV1DispWenDu1DecValue w_ParLst[300 + 8 * 24 + 2] //.1 �¶�ֵ1
#define w_HighV1DispWenDu2DecValue w_ParLst[300 + 8 * 24 + 3] //.1 �¶�ֵ2
#define w_HighV1DispWenDu3DecValue w_ParLst[300 + 8 * 24 + 4] //.1 �¶�ֵ3
#define w_HighV1DispWenDu4DecValue w_ParLst[300 + 8 * 24 + 5] //.1 �¶�ֵ4
#define w_HighV1DispWenDu5DecValue w_ParLst[300 + 8 * 24 + 6] //.1 �¶�ֵ5
#define w_HighV1DispWenDu6DecValue w_ParLst[300 + 8 * 24 + 7] //.1 �¶�ֵ6

#define w_HighV1DispDianLi1 w_ParLst[300 + 8 * 24 + 8]	 //.x ��������1
#define w_HighV1DispDianLi2 w_ParLst[300 + 8 * 24 + 9]	 //.x ��������1
#define w_HighV1DispDianLi3 w_ParLst[300 + 8 * 24 + 10]	 //.x ��������1
#define w_HighV1DispDianLi4 w_ParLst[300 + 8 * 24 + 11]	 //.x ��������1
#define w_HighV1DispDianLi5 w_ParLst[300 + 8 * 24 + 12]	 //.x ��������1
#define w_HighV1DispDianLi6 w_ParLst[300 + 8 * 24 + 13]	 //.x ��������1
#define w_HighV1DispDianLi7 w_ParLst[300 + 8 * 24 + 14]	 //.x ��������1
#define w_HighV1DispDianLi8 w_ParLst[300 + 8 * 24 + 15]	 //.x ��������1
#define w_HighV1DispDianLi9 w_ParLst[300 + 8 * 24 + 16]	 //.x ��������1
#define w_HighV1DispDianLi10 w_ParLst[300 + 8 * 24 + 17] //.x ��������1
														 // w_HighV1DispDianLi1=(u16)(FtoU32(w_HighV1DianLi1,w_HighV1DianLi2)/10);		//�ߵ�ѹ1  ȥ��1λ ���2λС��  10110 ��� 10.11kV
														 // w_HighV1DispDianLi2=(u16)(FtoU32(w_HighV1DianLi3,w_HighV1DianLi4)/10);		//�ߵ�ѹ2  ȥ��1λ ���2λС��  10110 ��� 10.11kV
														 // w_HighV1DispDianLi3=(u16)(FtoU32(w_HighV1DianLi5,w_HighV1DianLi6)/10);		//�ߵ�ѹ3  ȥ��1λ ���2λС��  10110 ��� 10.11kV
														 // w_HighV1DispDianLi4=(u16)(FtoU32(w_HighV1DianLi7,w_HighV1DianLi8));		//�����1
														 // w_HighV1DispDianLi5=(u16)(FtoU32(w_HighV1DianLi9,w_HighV1DianLi10));		//�����2
														 // w_HighV1DispDianLi6=(u16)(FtoU32(w_HighV1DianLi11,w_HighV1DianLi12));		//�����3
														 // w_HighV1DispDianLi7=(u16)(FtoU32(w_HighV1DianLi13,w_HighV1DianLi14));		//���й�����
														 // w_HighV1DispDianLi8=(u16)(FtoU32(w_HighV1DianLi15,w_HighV1DianLi16));		//Ƶ��

// ���2
#define w_HighV2DispSoftVersion w_ParLst[300 + 9 * 24 + 0]	  // ������
#define w_HighV2DispCounter w_ParLst[300 + 9 * 24 + 1]		  // ������
#define w_HighV2DispWenDu1DecValue w_ParLst[300 + 9 * 24 + 2] //.1 �¶�ֵ1
#define w_HighV2DispWenDu2DecValue w_ParLst[300 + 9 * 24 + 3] //.1 �¶�ֵ2
#define w_HighV2DispWenDu3DecValue w_ParLst[300 + 9 * 24 + 4] //.1 �¶�ֵ3
#define w_HighV2DispWenDu4DecValue w_ParLst[300 + 9 * 24 + 5] //.1 �¶�ֵ4
#define w_HighV2DispWenDu5DecValue w_ParLst[300 + 9 * 24 + 6] //.1 �¶�ֵ5
#define w_HighV2DispWenDu6DecValue w_ParLst[300 + 9 * 24 + 7] //.1 �¶�ֵ6

#define w_HighV2DispDianLi1 w_ParLst[300 + 9 * 24 + 8]	 //.x ��������1
#define w_HighV2DispDianLi2 w_ParLst[300 + 9 * 24 + 9]	 //.x ��������1
#define w_HighV2DispDianLi3 w_ParLst[300 + 9 * 24 + 10]	 //.x ��������1
#define w_HighV2DispDianLi4 w_ParLst[300 + 9 * 24 + 11]	 //.x ��������1
#define w_HighV2DispDianLi5 w_ParLst[300 + 9 * 24 + 12]	 //.x ��������1
#define w_HighV2DispDianLi6 w_ParLst[300 + 9 * 24 + 13]	 //.x ��������1
#define w_HighV2DispDianLi7 w_ParLst[300 + 9 * 24 + 14]	 //.x ��������1
#define w_HighV2DispDianLi8 w_ParLst[300 + 9 * 24 + 15]	 //.x ��������1
#define w_HighV2DispDianLi9 w_ParLst[300 + 9 * 24 + 16]	 //.x ��������1
#define w_HighV2DispDianLi10 w_ParLst[300 + 9 * 24 + 17] //.x ��������1

// ���3
#define w_HighV3DispSoftVersion w_ParLst[300 + 10 * 24 + 0]	   // ������
#define w_HighV3DispCounter w_ParLst[300 + 10 * 24 + 1]		   // ������
#define w_HighV3DispWenDu1DecValue w_ParLst[300 + 10 * 24 + 2] //.1 �¶�ֵ1
#define w_HighV3DispWenDu2DecValue w_ParLst[300 + 10 * 24 + 3] //.1 �¶�ֵ2
#define w_HighV3DispWenDu3DecValue w_ParLst[300 + 10 * 24 + 4] //.1 �¶�ֵ3
#define w_HighV3DispWenDu4DecValue w_ParLst[300 + 10 * 24 + 5] //.1 �¶�ֵ4
#define w_HighV3DispWenDu5DecValue w_ParLst[300 + 10 * 24 + 6] //.1 �¶�ֵ5
#define w_HighV3DispWenDu6DecValue w_ParLst[300 + 10 * 24 + 7] //.1 �¶�ֵ6

#define w_HighV3DispDianLi1 w_ParLst[300 + 10 * 24 + 8]	  //.x ��������1
#define w_HighV3DispDianLi2 w_ParLst[300 + 10 * 24 + 9]	  //.x ��������1
#define w_HighV3DispDianLi3 w_ParLst[300 + 10 * 24 + 10]  //.x ��������1
#define w_HighV3DispDianLi4 w_ParLst[300 + 10 * 24 + 11]  //.x ��������1
#define w_HighV3DispDianLi5 w_ParLst[300 + 10 * 24 + 12]  //.x ��������1
#define w_HighV3DispDianLi6 w_ParLst[300 + 10 * 24 + 13]  //.x ��������1
#define w_HighV3DispDianLi7 w_ParLst[300 + 10 * 24 + 14]  //.x ��������1
#define w_HighV3DispDianLi8 w_ParLst[300 + 10 * 24 + 15]  //.x ��������1
#define w_HighV3DispDianLi9 w_ParLst[300 + 10 * 24 + 16]  //.x ��������1
#define w_HighV3DispDianLi10 w_ParLst[300 + 10 * 24 + 17] //.x ��������1

// ���4
#define w_HighV4DispSoftVersion w_ParLst[300 + 11 * 24 + 0]	   // ������
#define w_HighV4DispCounter w_ParLst[300 + 11 * 24 + 1]		   // ������
#define w_HighV4DispWenDu1DecValue w_ParLst[300 + 11 * 24 + 2] //.1 �¶�ֵ1
#define w_HighV4DispWenDu2DecValue w_ParLst[300 + 11 * 24 + 3] //.1 �¶�ֵ2
#define w_HighV4DispWenDu3DecValue w_ParLst[300 + 11 * 24 + 4] //.1 �¶�ֵ3
#define w_HighV4DispWenDu4DecValue w_ParLst[300 + 11 * 24 + 5] //.1 �¶�ֵ4
#define w_HighV4DispWenDu5DecValue w_ParLst[300 + 11 * 24 + 6] //.1 �¶�ֵ5
#define w_HighV4DispWenDu6DecValue w_ParLst[300 + 11 * 24 + 7] //.1 �¶�ֵ6

#define w_HighV4DispDianLi1 w_ParLst[300 + 11 * 24 + 8]	  //.x ��������1
#define w_HighV4DispDianLi2 w_ParLst[300 + 11 * 24 + 9]	  //.x ��������1
#define w_HighV4DispDianLi3 w_ParLst[300 + 11 * 24 + 10]  //.x ��������1
#define w_HighV4DispDianLi4 w_ParLst[300 + 11 * 24 + 11]  //.x ��������1
#define w_HighV4DispDianLi5 w_ParLst[300 + 11 * 24 + 12]  //.x ��������1
#define w_HighV4DispDianLi6 w_ParLst[300 + 11 * 24 + 13]  //.x ��������1
#define w_HighV4DispDianLi7 w_ParLst[300 + 11 * 24 + 14]  //.x ��������1
#define w_HighV4DispDianLi8 w_ParLst[300 + 11 * 24 + 15]  //.x ��������1
#define w_HighV4DispDianLi9 w_ParLst[300 + 11 * 24 + 16]  //.x ��������1
#define w_HighV4DispDianLi10 w_ParLst[300 + 11 * 24 + 17] //.x ��������1

// ���5
#define w_HighV5DispSoftVersion w_ParLst[300 + 12 * 24 + 0]	   // ������
#define w_HighV5DispCounter w_ParLst[300 + 12 * 24 + 1]		   // ������
#define w_HighV5DispWenDu1DecValue w_ParLst[300 + 12 * 24 + 2] //.1 �¶�ֵ1
#define w_HighV5DispWenDu2DecValue w_ParLst[300 + 12 * 24 + 3] //.1 �¶�ֵ2
#define w_HighV5DispWenDu3DecValue w_ParLst[300 + 12 * 24 + 4] //.1 �¶�ֵ3
#define w_HighV5DispWenDu4DecValue w_ParLst[300 + 12 * 24 + 5] //.1 �¶�ֵ4
#define w_HighV5DispWenDu5DecValue w_ParLst[300 + 12 * 24 + 6] //.1 �¶�ֵ5
#define w_HighV5DispWenDu6DecValue w_ParLst[300 + 12 * 24 + 7] //.1 �¶�ֵ6

#define w_HighV5DispDianLi1 w_ParLst[300 + 12 * 24 + 8]	  //.x ��������1
#define w_HighV5DispDianLi2 w_ParLst[300 + 12 * 24 + 9]	  //.x ��������1
#define w_HighV5DispDianLi3 w_ParLst[300 + 12 * 24 + 10]  //.x ��������1
#define w_HighV5DispDianLi4 w_ParLst[300 + 12 * 24 + 11]  //.x ��������1
#define w_HighV5DispDianLi5 w_ParLst[300 + 12 * 24 + 12]  //.x ��������1
#define w_HighV5DispDianLi6 w_ParLst[300 + 12 * 24 + 13]  //.x ��������1
#define w_HighV5DispDianLi7 w_ParLst[300 + 12 * 24 + 14]  //.x ��������1
#define w_HighV5DispDianLi8 w_ParLst[300 + 12 * 24 + 15]  //.x ��������1
#define w_HighV5DispDianLi9 w_ParLst[300 + 12 * 24 + 16]  //.x ��������1
#define w_HighV5DispDianLi10 w_ParLst[300 + 12 * 24 + 17] //.x ��������1

// ���6
#define w_HighV6DispSoftVersion w_ParLst[300 + 13 * 24 + 0]	   // ������
#define w_HighV6DispCounter w_ParLst[300 + 13 * 24 + 1]		   // ������
#define w_HighV6DispWenDu1DecValue w_ParLst[300 + 13 * 24 + 2] //.1 �¶�ֵ1
#define w_HighV6DispWenDu2DecValue w_ParLst[300 + 13 * 24 + 3] //.1 �¶�ֵ2
#define w_HighV6DispWenDu3DecValue w_ParLst[300 + 13 * 24 + 4] //.1 �¶�ֵ3
#define w_HighV6DispWenDu4DecValue w_ParLst[300 + 13 * 24 + 5] //.1 �¶�ֵ4
#define w_HighV6DispWenDu5DecValue w_ParLst[300 + 13 * 24 + 6] //.1 �¶�ֵ5
#define w_HighV6DispWenDu6DecValue w_ParLst[300 + 13 * 24 + 7] //.1 �¶�ֵ6

#define w_HighV6DispDianLi1 w_ParLst[300 + 13 * 24 + 8]	  //.x ��������1
#define w_HighV6DispDianLi2 w_ParLst[300 + 13 * 24 + 9]	  //.x ��������1
#define w_HighV6DispDianLi3 w_ParLst[300 + 13 * 24 + 10]  //.x ��������1
#define w_HighV6DispDianLi4 w_ParLst[300 + 13 * 24 + 11]  //.x ��������1
#define w_HighV6DispDianLi5 w_ParLst[300 + 13 * 24 + 12]  //.x ��������1
#define w_HighV6DispDianLi6 w_ParLst[300 + 13 * 24 + 13]  //.x ��������1
#define w_HighV6DispDianLi7 w_ParLst[300 + 13 * 24 + 14]  //.x ��������1
#define w_HighV6DispDianLi8 w_ParLst[300 + 13 * 24 + 15]  //.x ��������1
#define w_HighV6DispDianLi9 w_ParLst[300 + 13 * 24 + 16]  //.x ��������1
#define w_HighV6DispDianLi10 w_ParLst[300 + 13 * 24 + 17] //.x ��������1

// ���7
#define w_HighV7DispSoftVersion w_ParLst[300 + 14 * 24 + 0]	   // ������
#define w_HighV7DispCounter w_ParLst[300 + 14 * 24 + 1]		   // ������
#define w_HighV7DispWenDu1DecValue w_ParLst[300 + 14 * 24 + 2] //.1 �¶�ֵ1
#define w_HighV7DispWenDu2DecValue w_ParLst[300 + 14 * 24 + 3] //.1 �¶�ֵ2
#define w_HighV7DispWenDu3DecValue w_ParLst[300 + 14 * 24 + 4] //.1 �¶�ֵ3
#define w_HighV7DispWenDu4DecValue w_ParLst[300 + 14 * 24 + 5] //.1 �¶�ֵ4
#define w_HighV7DispWenDu5DecValue w_ParLst[300 + 14 * 24 + 6] //.1 �¶�ֵ5
#define w_HighV7DispWenDu6DecValue w_ParLst[300 + 14 * 24 + 7] //.1 �¶�ֵ6

#define w_HighV7DispDianLi1 w_ParLst[300 + 14 * 24 + 8]	  //.x ��������1
#define w_HighV7DispDianLi2 w_ParLst[300 + 14 * 24 + 9]	  //.x ��������1
#define w_HighV7DispDianLi3 w_ParLst[300 + 14 * 24 + 10]  //.x ��������1
#define w_HighV7DispDianLi4 w_ParLst[300 + 14 * 24 + 11]  //.x ��������1
#define w_HighV7DispDianLi5 w_ParLst[300 + 14 * 24 + 12]  //.x ��������1
#define w_HighV7DispDianLi6 w_ParLst[300 + 14 * 24 + 13]  //.x ��������1
#define w_HighV7DispDianLi7 w_ParLst[300 + 14 * 24 + 14]  //.x ��������1
#define w_HighV7DispDianLi8 w_ParLst[300 + 14 * 24 + 15]  //.x ��������1
#define w_HighV7DispDianLi9 w_ParLst[300 + 14 * 24 + 16]  //.x ��������1
#define w_HighV7DispDianLi10 w_ParLst[300 + 14 * 24 + 17] //.x ��������1

// ���8
#define w_HighV8DispSoftVersion w_ParLst[300 + 15 * 24 + 0]	   // ������
#define w_HighV8DispCounter w_ParLst[300 + 15 * 24 + 1]		   // ������
#define w_HighV8DispWenDu1DecValue w_ParLst[300 + 15 * 24 + 2] //.1 �¶�ֵ1
#define w_HighV8DispWenDu2DecValue w_ParLst[300 + 15 * 24 + 3] //.1 �¶�ֵ2
#define w_HighV8DispWenDu3DecValue w_ParLst[300 + 15 * 24 + 4] //.1 �¶�ֵ3
#define w_HighV8DispWenDu4DecValue w_ParLst[300 + 15 * 24 + 5] //.1 �¶�ֵ4
#define w_HighV8DispWenDu5DecValue w_ParLst[300 + 15 * 24 + 6] //.1 �¶�ֵ5
#define w_HighV8DispWenDu6DecValue w_ParLst[300 + 15 * 24 + 7] //.1 �¶�ֵ6

#define w_HighV8DispDianLi1 w_ParLst[300 + 15 * 24 + 8]	  //.x ��������1
#define w_HighV8DispDianLi2 w_ParLst[300 + 15 * 24 + 9]	  //.x ��������1
#define w_HighV8DispDianLi3 w_ParLst[300 + 15 * 24 + 10]  //.x ��������1
#define w_HighV8DispDianLi4 w_ParLst[300 + 15 * 24 + 11]  //.x ��������1
#define w_HighV8DispDianLi5 w_ParLst[300 + 15 * 24 + 12]  //.x ��������1
#define w_HighV8DispDianLi6 w_ParLst[300 + 15 * 24 + 13]  //.x ��������1
#define w_HighV8DispDianLi7 w_ParLst[300 + 15 * 24 + 14]  //.x ��������1
#define w_HighV8DispDianLi8 w_ParLst[300 + 15 * 24 + 15]  //.x ��������1
#define w_HighV8DispDianLi9 w_ParLst[300 + 15 * 24 + 16]  //.x ��������1
#define w_HighV8DispDianLi10 w_ParLst[300 + 15 * 24 + 17] //.x ��������1

// ��ע�� w_ParLst[300+8*24+0] ----- w_ParLst[300+15*24+23]  ���䱻 Һ����ʹ�ã�ֻ������������䡣//ZCL 2022.3.21

// ZCL 2022.3.21 ����
#define w_HighV1RunSecond w_ParLst[684 + 0]	   //.0 ��		//ZCL 2022.3.10
#define w_HighV1RunMinute w_ParLst[684 + 1]	   //.0 ����
#define w_HighV1RunHour w_ParLst[684 + 2]	   //.0 Сʱ
#define w_HighV1SpeedValue w_ParLst[684 + 3]   //.0 ת��	//ZCL 2022.3.21
#define w_HighV1PulseCounter w_ParLst[684 + 4] //.0 ���������
#define w_HighV1NoUse_1 w_ParLst[684 + 5]	   //.0 Ԥ��	//ZCL 2022.3.21
#define w_HighV1NoUse_2 w_ParLst[684 + 6]	   //.0 Ԥ��
#define w_HighV1NoUse_3 w_ParLst[684 + 7]	   //.0 Ԥ��	//ZCL 2022.3.21
#define w_HighV1NoUse_4 w_ParLst[684 + 8]	   //.0 Ԥ��
#define w_HighV1NoUse_5 w_ParLst[684 + 9]	   //.0 Ԥ��	//ZCL 2022.3.21

#define w_HighV2RunSecond w_ParLst[694 + 0]	   //.0 ��		//ZCL 2022.3.10
#define w_HighV2RunMinute w_ParLst[694 + 1]	   //.0 ����
#define w_HighV2RunHour w_ParLst[694 + 2]	   //.0 Сʱ
#define w_HighV2SpeedValue w_ParLst[694 + 3]   //.0 ת��	//ZCL 2022.3.21
#define w_HighV2PulseCounter w_ParLst[694 + 4] //.0 ���������
#define w_HighV2NoUse_1 w_ParLst[694 + 5]	   //.0 Ԥ��	//ZCL 2022.3.21
#define w_HighV2NoUse_2 w_ParLst[694 + 6]	   //.0 Ԥ��
#define w_HighV2NoUse_3 w_ParLst[694 + 7]	   //.0 Ԥ��	//ZCL 2022.3.21
#define w_HighV2NoUse_4 w_ParLst[694 + 8]	   //.0 Ԥ��
#define w_HighV2NoUse_5 w_ParLst[694 + 9]	   //.0 Ԥ��	//ZCL 2022.3.21

#define w_HighV3RunSecond w_ParLst[704 + 0]	   //.0 ��		//ZCL 2022.3.10
#define w_HighV3RunMinute w_ParLst[704 + 1]	   //.0 ����
#define w_HighV3RunHour w_ParLst[704 + 2]	   //.0 Сʱ
#define w_HighV3SpeedValue w_ParLst[704 + 3]   //.0 ת��	//ZCL 2022.3.21
#define w_HighV3PulseCounter w_ParLst[704 + 4] //.0 ���������
#define w_HighV3NoUse_1 w_ParLst[704 + 5]	   //.0 Ԥ��	//ZCL 2022.3.21
#define w_HighV3NoUse_2 w_ParLst[704 + 6]	   //.0 Ԥ��
#define w_HighV3NoUse_3 w_ParLst[704 + 7]	   //.0 Ԥ��	//ZCL 2022.3.21
#define w_HighV3NoUse_4 w_ParLst[704 + 8]	   //.0 Ԥ��
#define w_HighV3NoUse_5 w_ParLst[704 + 9]	   //.0 Ԥ��	//ZCL 2022.3.21

#define w_HighV4RunSecond w_ParLst[714 + 0]	   //.0 ��		//ZCL 2022.3.10
#define w_HighV4RunMinute w_ParLst[714 + 1]	   //.0 ����
#define w_HighV4RunHour w_ParLst[714 + 2]	   //.0 Сʱ
#define w_HighV4SpeedValue w_ParLst[714 + 3]   //.0 ת��	//ZCL 2022.3.21
#define w_HighV4PulseCounter w_ParLst[714 + 4] //.0 ���������
#define w_HighV4NoUse_1 w_ParLst[714 + 5]	   //.0 Ԥ��	//ZCL 2022.3.21
#define w_HighV4NoUse_2 w_ParLst[714 + 6]	   //.0 Ԥ��
#define w_HighV4NoUse_3 w_ParLst[714 + 7]	   //.0 Ԥ��	//ZCL 2022.3.21
#define w_HighV4NoUse_4 w_ParLst[714 + 8]	   //.0 Ԥ��
#define w_HighV4NoUse_5 w_ParLst[714 + 9]	   //.0 Ԥ��	//ZCL 2022.3.21

#define w_HighV5RunSecond w_ParLst[724 + 0]	   //.0 ��		//ZCL 2022.3.10
#define w_HighV5RunMinute w_ParLst[724 + 1]	   //.0 ����
#define w_HighV5RunHour w_ParLst[724 + 2]	   //.0 Сʱ
#define w_HighV5SpeedValue w_ParLst[724 + 3]   //.0 ת��	//ZCL 2022.3.21
#define w_HighV5PulseCounter w_ParLst[724 + 4] //.0 ���������
#define w_HighV5NoUse_1 w_ParLst[724 + 5]	   //.0 Ԥ��	//ZCL 2022.3.21
#define w_HighV5NoUse_2 w_ParLst[724 + 6]	   //.0 Ԥ��
#define w_HighV5NoUse_3 w_ParLst[724 + 7]	   //.0 Ԥ��	//ZCL 2022.3.21
#define w_HighV5NoUse_4 w_ParLst[724 + 8]	   //.0 Ԥ��
#define w_HighV5NoUse_5 w_ParLst[724 + 9]	   //.0 Ԥ��	//ZCL 2022.3.21

#define w_HighV6RunSecond w_ParLst[734 + 0]	   //.0 ��		//ZCL 2022.3.10
#define w_HighV6RunMinute w_ParLst[734 + 1]	   //.0 ����
#define w_HighV6RunHour w_ParLst[734 + 2]	   //.0 Сʱ
#define w_HighV6SpeedValue w_ParLst[734 + 3]   //.0 ת��	//ZCL 2022.3.21
#define w_HighV6PulseCounter w_ParLst[734 + 4] //.0 ���������
#define w_HighV6NoUse_1 w_ParLst[734 + 5]	   //.0 Ԥ��	//ZCL 2022.3.21
#define w_HighV6NoUse_2 w_ParLst[734 + 6]	   //.0 Ԥ��
#define w_HighV6NoUse_3 w_ParLst[734 + 7]	   //.0 Ԥ��	//ZCL 2022.3.21
#define w_HighV6NoUse_4 w_ParLst[734 + 8]	   //.0 Ԥ��
#define w_HighV6NoUse_5 w_ParLst[734 + 9]	   //.0 Ԥ��	//ZCL 2022.3.21

#define w_HighV7RunSecond w_ParLst[744 + 0]	   //.0 ��		//ZCL 2022.3.10
#define w_HighV7RunMinute w_ParLst[744 + 1]	   //.0 ����
#define w_HighV7RunHour w_ParLst[744 + 2]	   //.0 Сʱ
#define w_HighV7SpeedValue w_ParLst[744 + 3]   //.0 ת��	//ZCL 2022.3.21
#define w_HighV7PulseCounter w_ParLst[744 + 4] //.0 ���������
#define w_HighV7NoUse_1 w_ParLst[744 + 5]	   //.0 Ԥ��	//ZCL 2022.3.21
#define w_HighV7NoUse_2 w_ParLst[744 + 6]	   //.0 Ԥ��
#define w_HighV7NoUse_3 w_ParLst[744 + 7]	   //.0 Ԥ��	//ZCL 2022.3.21
#define w_HighV7NoUse_4 w_ParLst[744 + 8]	   //.0 Ԥ��
#define w_HighV7NoUse_5 w_ParLst[744 + 9]	   //.0 Ԥ��	//ZCL 2022.3.21

#define w_HighV8RunSecond w_ParLst[754 + 0]	   //.0 ��		//ZCL 2022.3.10
#define w_HighV8RunMinute w_ParLst[754 + 1]	   //.0 ����
#define w_HighV8RunHour w_ParLst[754 + 2]	   //.0 Сʱ
#define w_HighV8SpeedValue w_ParLst[754 + 3]   //.0 ת��	//ZCL 2022.3.21
#define w_HighV8PulseCounter w_ParLst[754 + 4] //.0 ���������
#define w_HighV8NoUse_1 w_ParLst[754 + 5]	   //.0 Ԥ��	//ZCL 2022.3.21
#define w_HighV8NoUse_2 w_ParLst[754 + 6]	   //.0 Ԥ��
#define w_HighV8NoUse_3 w_ParLst[754 + 7]	   //.0 Ԥ��	//ZCL 2022.3.21
#define w_HighV8NoUse_4 w_ParLst[754 + 8]	   //.0 Ԥ��
#define w_HighV8NoUse_5 w_ParLst[754 + 9]	   //.0 Ԥ��	//ZCL 2022.3.21
/* #define STM32_WRPAR_ADDRESS			0 			// ��д������ַ��Pw_ParLst������ʱƫ��1000����
#define STM32_RPAR_ADDRESS			10000 	// ֻ��������ַ��w_ParLst������ʱƫ��1000����
#define DSP_WRPAR_ADDRESS				20000 	// ֻ��������ַ��Pw_dspParLst������ʱƫ��1000����
#define DSP_RPAR_ADDRESS				30000 	// ֻ��������ַ��w_dspParLst������ʱƫ��1000���� */
// void	Define4_DspRWPar(void) {}
//  3. DSP �趨����--------------------------------------------------------------------------
#define Pw_dspAddSpeedTime Pw_dspParLst[0]	 //.0 ����ʱ��	ʵʱ��Ч
#define Pw_dspMotorDirection Pw_dspParLst[1] //.0 ���ת��	0/1 ������Ч
#define Pw_dspTorquePromote Pw_dspParLst[2]	 //.0 ת������	0~30 ������Ч
#define Pw_dspVFCurve Pw_dspParLst[3]		 //.0 VF����	0~10	������Ч
#define Pw_dspDeadSupplement Pw_dspParLst[4] //.0 ��������	0~100	������Ч
#define Pw_dspSubSpeedTime Pw_dspParLst[5]	 //.0 ����ʱ��	0~6500S	ʵʱ��Ч

#define Pw_dspStartHz Pw_dspParLst[6]			 //.1 ����Ƶ��	0~500	ʵʱ��Ч
#define Pw_dspCarrierHz Pw_dspParLst[7]			 //.0 �ز�Ƶ��	2~10	������Ч 	DCM220��.1	DCM100��.0
#define Pw_dspUpperLimitHz Pw_dspParLst[8]		 //.1 ����Ƶ��	0~500	ʵʱ��Ч
#define Pw_dspLowerLimitHz Pw_dspParLst[9]		 //.1 ����Ƶ��	0~500	ʵʱ��Ч
#define Pw_dspMAXFREQ Pw_dspParLst[11]			 //.0 ���Ƶ�� 50-100Hz ZCL 2017.5.5 �趨��Ӱ���ѹ //BASE_FREQ
#define Pw_dspStartHzStopMode Pw_dspParLst[12]	 //.0 ����Ƶ��ͣ��ģʽ =0������ͣ����=1����ͣ������ͣ��Ĭ��0
#define Pw_dspFanRunTemperature Pw_dspParLst[13] //.1 ������ת�趨�¶�	ZCL 2018.4.20

#define Pw_dspWenDuSel Pw_dspParLst[14]		  //.0 �¶�ѡ��  ZCL 2018.7.16 =0,15-65DU; =1 105DU;
#define Pw_dspOCPCurrent Pw_dspParLst[15]	  //.0 ������������	ʵʱ��Ч	OverCurrentProtect
#define Pw_dspOVPVoltage Pw_dspParLst[16]	  //.0 ��ѹ������ѹ	ʵʱ��Ч  OverVoltage
#define Pw_dspUVPVoltage Pw_dspParLst[17]	  //.0 Ƿѹ������ѹ	ʵʱ��Ч  UnderVoltage
#define Pw_dspOHPTemperature Pw_dspParLst[18] //.0 ���ȱ����¶�	������Ч  Overheat
#define Pw_dspFanRunMode Pw_dspParLst[19]	  //.0 �������з�ʽ	0/1	0-����;1-�¿�
#define Pw_dspHuoErType Pw_dspParLst[20]	  //.0 �������� ��λ���������� 1��˶��  2�����  3������  0���������ͣ�Ԥ����
											  //   ����3λ  ��λ---��λ  �����������λ�����Դ������⺬�� ZCL 2017.4.29

#define Pw_dspKeyStopMode Pw_dspParLst[21]		 //.0 ����ͣ��ģʽ =0������ͣ����=1����ͣ������ͣ��
#define Pw_dspDIStopMode Pw_dspParLst[22]		 //.0 DIͣ��ģʽ =0������ͣ����=1����ͣ������ͣ��
#define Pw_dspFaultAutoResetEn Pw_dspParLst[23]	 //.0 �����Զ���λʹ�ܣ�=1����Ĭ������
#define Pw_dspFaultAutoResetSec Pw_dspParLst[24] //.0 �����Զ���λʱ�䣬��λ�룬Ĭ��300�� 20��

// 1-��ǰƵ�ʣ�2-�����ѹ��3-������4-�¶ȣ�5-ĸ�ߵ�ѹ��6-���������;7-MODBUS��������8-Ŀ��Ƶ��
#define Pw_dspAO1OutSource Pw_dspParLst[25] //.0 ģ�������1����Դ	0~10
#define Pw_dspAO2OutSource Pw_dspParLst[26] //.0 ģ�������2����Դ	0~10

// 0-�������1-ST���ã�2-���ϱ���;3-ϵͳPWM���״̬.
#define Pw_dspDO1OutSource Pw_dspParLst[27] //.0 �̵���1�������Դ	0~10
#define Pw_dspDO2OutSource Pw_dspParLst[28] //.0 �̵���2�������Դ	0~10

// ZCL 2018.3.29 ��Щ��DCM220��ʹ��
#define Pw_dspAddress Pw_dspParLst[29]	 //.0 ��Ƶ����ַ  2016.6.16
#define Pw_dspZDRunMode Pw_dspParLst[30] //.0 ָ��Ƶ������ģʽ
#define Pw_dspRunHz Pw_dspParLst[31]	 //.1 ָ������Ƶ��

#define Pw_dspAngleCompRef Pw_dspParLst[32] //.0 �ǶȲ���
#define Pw_dspAngleCompSet Pw_dspParLst[33] //.0 �ǶȲ���2

#define Pw_dspICurQsRef Pw_dspParLst[34]  //.0 ���ص����ο�
#define Pw_dspVoltCompUp Pw_dspParLst[35] //.0 ����
#define Pw_dspVoltCompUd Pw_dspParLst[36] //.0 ΢��

#define Pw_dspSelABCShunXu Pw_dspParLst[37] //.0 ѡ��ABC����˳��

#define Pw_dspnCnt Pw_dspParLst[38]		   //.0 ZCL 2018.3.29 ��ʱ����
#define Pw_dspFrqCompUp Pw_dspParLst[39]   //.0 ZCL 2018.3.29 ��ʱ����
#define Pw_dspFrqCompLimt Pw_dspParLst[40] //.0 ZCL 2018.3.29 ��ʱ����
#define Pw_dspFrqCompUd Pw_dspParLst[41]   //.0 ZCL 2018.3.29 ��ʱ����

#define Pw_dspCompDirection Pw_dspParLst[42] //.0 ��������
#define Pw_dspHzAISource Pw_dspParLst[43]	 //.0 DSP Ƶ��AI����Դ  ZCL 2018.3.26 DCM220ʹ��
#define Pw_dspAutoResetMax Pw_dspParLst[44]	 //.0 DSP �����Զ���λ���ֵ  ZCL 2018.3.24
// #define	Pw_dspFanRunTemperature		Pw_dspParLst[45]	//.1 ������ת�趨�¶�	0~1000	*0.1 ZCL 2018.3.30 �Ƶ����� DCM220

#define Pw_dspBusBarVoltageKi Pw_dspParLst[46]	//.2 ZCL 2019.12.24 ��ѹ��������
#define Pw_dspFastOverVoltage Pw_dspParLst[47]	//.0 ZCL 2019.12.24 ���ٹ�ѹ����ֵֵ
#define Pw_dspFastUnderVoltage Pw_dspParLst[48] //.0 ZCL 2019.12.24 ����Ƿѹ��ѹֵ

/* #define STM32_WRPAR_ADDRESS			0 			// ��д������ַ��Pw_ParLst������ʱƫ��0����
#define STM32_RPAR_ADDRESS			10000 	// ֻ��������ַ��w_ParLst������ʱƫ��10000����
#define DSP_WRPAR_ADDRESS				20000 	// ֻ��������ַ��Pw_dspParLst������ʱƫ��20000����
#define DSP_RPAR_ADDRESS				30000 	// ֻ��������ַ��w_dspParLst������ʱƫ��30000���� */
// void	Define5_DspRPar(void) {}

// 4.DSPֻ�������Ķ���--------------------------------------------------------------------------
#define w_dspSoftVersion w_dspParLst[0] //.2 DSP���ϵͳ�汾��	0~65535
#define w_NoUse1 w_dspParLst[1]			//.0
// 2��. ͨѶ
#define w_dspTemp1 w_dspParLst[2] //.0 ZCL 2018.4.19 ɾ�� A��ADCУ�����	0~65535
#define w_dspTemp2 w_dspParLst[3] //.0 ZCL 2018.4.19 ɾ�� B��ADCУ�����	0~65535
#define w_dspTemp3 w_dspParLst[4] //.0 ZCL 2018.4.19 ɾ�� C��ADCУ�����	0~65535
#define w_dspTemp4 w_dspParLst[5] //.0 ZCL 2018.4.19 ɾ�� A��ADCУ������	0~65535
#define w_dspTemp5 w_dspParLst[6] //.0 ZCL 2018.4.19 ɾ�� B��ADCУ������	0~65535
#define w_dspTemp6 w_dspParLst[7] //.0 ZCL 2018.4.19 ɾ�� C��ADCУ������	0~65535
// #define	w_dspAPhaseCalibrationZero  	w_dspParLst[2]	//.0 A��ADCУ�����	0~65535
// #define	w_dspBPhaseCalibrationZero  	w_dspParLst[3]	//.0 B��ADCУ�����	0~65535
// #define	w_dspCPhaseCalibrationZero  	w_dspParLst[4]	//.0 C��ADCУ�����	0~65535
// #define	w_dspAPhaseCalibrationGain  	w_dspParLst[5]	//.0 A��ADCУ������	0~65535
// #define	w_dspBPhaseCalibrationGain  	w_dspParLst[6]	//.0 B��ADCУ������	0~65535
// #define	w_dspCPhaseCalibrationGain  	w_dspParLst[7]	//.0 C��ADCУ������	0~65535

#define w_dspAPhaseDLHZero w_dspParLst[8]			//.0 A������������	0~65535
#define w_dspBPhaseDLHZero w_dspParLst[9]			//.0 B������������	0~65535
#define w_dspCPhaseDLHZero w_dspParLst[10]			//.0 C������������	0~65535
#define w_dspAPhaseCurrentADCValue w_dspParLst[11]	//.0 A�����ADCֵ
#define w_dspBPhaseCurrentADCValue w_dspParLst[12]	//.0 B�����ADCֵ
#define w_dspCPhaseCurrentADCValue w_dspParLst[13]	//.0 C�����ADCֵ
#define w_dspBusBarVoltageADCValue w_dspParLst[14]	//.0 ĸ�ߵ�ѹADCֵ
#define w_dspNowTemperatureADCValue w_dspParLst[15] //.0 ��ǰ�¶�ADCֵ		0~1000	x10

// 1��. ͨѶ
#define w_dspAPhaseCurrent w_dspParLst[16]	  //.2 A����� 0~65535	x100
#define w_dspBPhaseCurrent w_dspParLst[17]	  //.2 B����� 0~65535	x100
#define w_dspCPhaseCurrent w_dspParLst[18]	  //.2 C�����0~65535	x100
#define w_dsp3PhaseCurrentSum w_dspParLst[19] //.0 �������֮��
// 2018.3.23 ��ʱĿ��Ƶ�ʣ������м���
#define w_dspTmpTargetHz w_dspParLst[20]	//.1 ��ʱĿ��Ƶ�ʣ������м���	0~500	x10
#define w_dspNowHz w_dspParLst[21]			//.1 ��ǰƵ��	0~500	x10
#define w_dspNowRotateSpeed w_dspParLst[22] //.0 ��ǰת��	0~60000	x10
#define w_dspNowTemperature w_dspParLst[23] //.1 ��ǰ�¶�	0~1000	x10
#define w_dspOutVoltage w_dspParLst[24]		//.0 �����ѹ	0~500	x1
#define w_dspBusBarVoltage w_dspParLst[25]	//.0 ĸ�ߵ�ѹ	0~800	x1

#define w_dspCounter w_dspParLst[26]		//.0 DCM220����Ҫ��DCM100��ѭ��������
#define w_dspCommErrCounter w_dspParLst[27] //.0 DCM220����Ҫ��DCM100ͨѶ��������������ڹ۲�ͨѶ����
											// w_dspCommErrCounter ZCL 2018.9.13  �������Ᵽ��Pw_ScrKeyMode
#define w_dspCounter2 w_dspParLst[28]		//.0 DCM220����Ҫ��DCM100�ϵ��Pw����������
											// w_dspCounter2 ZCL 2018.9.13  �������Ᵽ��Pw_ScrKeyMode
#define w_dspCounter3 w_dspParLst[29]		//.0 DCM220 DCM100: dsp����������

#define w_dspSysErrorCode1 w_dspParLst[30] //.0 ϵͳ������E0	0~65535	 ʹ��λ
#define w_dspSysErrorCode2 w_dspParLst[31] //.0 ϵͳ������E2	0~65535	 No Use ZCL 2018.3.21
#define w_dspSysRunStatus1 w_dspParLst[32] //.0 ϵͳ����״̬��1		ʹ��λ
#define w_dspSysRunStatus2 w_dspParLst[33] //.0 ϵͳ����״̬��2		No Use ZCL 2018.4.19

#define w_dspCounter4 w_dspParLst[35] //.0 DCM220����Ҫ��DCM100��ModbusDataValidCount
#define w_dspCounter5 w_dspParLst[36] //.0 DCM220 DCM100: my_PDPINTA_ISR�ж�  F_dspIpmFault ������

// ZCL 2015.9.16 ���뱣����ʱ�������ۿ�����
#define w_dspOverVPDelayCount w_dspParLst[41]	  //.0 ��ѹ������ʱ������
#define w_dspOverCPDelayCount w_dspParLst[42]	  //.0 ����������ʱ������
#define w_dspLosePhasePDelayCount w_dspParLst[43] //.0 ȱ�ౣ����ʱ������
#define w_dspUnderVPDelayCount w_dspParLst[44]	  //.0 Ƿѹ������ʱ������
#define w_dspOverHotPDelayCount w_dspParLst[45]	  //.0 ���ȱ�����ʱ������
#define w_dspOverHotCancelCount w_dspParLst[46]	  //.0 ����ȡ��������ʱ������

#define w_dspModPar w_dspParLst[48]		//.0 �޸Ĳ���   ZCL 2015.9.9 DCM220��
#define w_dspParInitial w_dspParLst[49] //.0 ������ʼ�� ZCL 2015.9.9 DCM220��

/*---DSP���յ���STM32�����ÿ����֣�����-----------------------*/
// 3. ��ʱдĿ��Ƶ�ʡ�ģ�������1��ģ�������2 ��DSP	��MAXTimeWriteNum==3 ���ƶ�д������
#define w_dspTargetHz w_dspParLst[50] //.1 Ŀ��Ƶ��	0~500	x10
#define w_dspAO1 w_dspParLst[51]	  //.0 DSPģ������ǰ�����1	0~4095	0~5V���
#define w_dspAO2 w_dspParLst[52]	  //.0 DSPģ������ǰ�����2	0~4095	0~5V���

// ZCL 2018.3.24 DCM220ʹ��
#define w_dspAI1ADCValue w_dspParLst[53] //.0 AI1 ADC�ɼ�ֵ ZCL 2016.6.21
#define w_dspAI2ADCValue w_dspParLst[54] //.0 AI2 ADC�ɼ�ֵ

#define w_dspWriteYear w_dspParLst[57] //.0�����д��		ZCL 2013.3.22
#define w_dspWriteDate w_dspParLst[58] //.0�����д����

#define w_dspFaultRecNum w_dspParLst[59] //.0 ���ϼ�¼���� ZCL ������ǰ��Ϊ�˸�SM510��Ӧ����
#define w_dspFaultRecNo w_dspParLst[60]	 //.0 ���ϼ�¼��  ZCL
#define w_dspFaultNum w_dspParLst[61]	 //.0 DSP����������������  2016.8.23
#define w_dspFaultStatus w_dspParLst[62] //.0 DSP����ԭ�򣨱����� 2016.8.23

#define w_dspStopNum w_dspParLst[63]	//.0 DSPͣ������ 2018.4.24
#define w_dspStopStatus w_dspParLst[64] //.0 DSPͣ��ԭ�� 2018.4.24

#define w_dspSelFaultNo w_dspParLst[69]	  //.0 ѡ����Ϻ�
#define w_dspSelRecNo w_dspParLst[70]	  //.0 ѡ���¼��
#define w_dspPreFaultNo w_dspParLst[71]	  //.0 �ϴι��Ϻ�
#define w_dspTouchRunStop w_dspParLst[72] //.0 ���� ����/ֹͣ 2016.12.5
#define w_dspDIResetCount w_dspParLst[73] //.0 DI��λ������ DCM220�� 2018.3.24
#define w_dspAutoResetNum w_dspParLst[74] //.0 DSP �����Զ���λ���� ���� �ۿ��� 2018.3.24
#define w_dspManMadeFault w_dspParLst[75] //.0 ��Ϊ������� ������ 2018.3.24

#define w_dspEquipAlarmLast6 w_dspParLst[82]   //.0 �豸ͣ��ԭ����6��
#define w_dspEquipAlarm6YM w_dspParLst[83]	   //.0 �豸ͣ��6ʱ�䣭����
#define w_dspEquipAlarm6DH w_dspParLst[84]	   //.0 �豸ͣ��6ʱ�䣭��ʱ
#define w_dspEquipAlarm6MS w_dspParLst[85]	   //.0 �豸ͣ��6ʱ�䣭����
#define w_dspEquipAlarmLast5 w_dspParLst[86]   //.0 �豸ͣ��ԭ����5��
#define w_dspEquipAlarm5YM w_dspParLst[87]	   //.0 �豸ͣ��5ʱ�䣭����
#define w_dspEquipAlarm5DH w_dspParLst[88]	   //.0 �豸ͣ��5ʱ�䣭��ʱ
#define w_dspEquipAlarm5MS w_dspParLst[89]	   //.0 �豸ͣ��5ʱ�䣭����
#define w_dspEquipAlarmLast4 w_dspParLst[90]   //.0 �豸ͣ��ԭ����4��
#define w_dspEquipAlarm4YM w_dspParLst[91]	   //.0 �豸ͣ��4ʱ�䣭����
#define w_dspEquipAlarm4DH w_dspParLst[92]	   //.0 �豸ͣ��4ʱ�䣭��ʱ
#define w_dspEquipAlarm4MS w_dspParLst[93]	   //.0 �豸ͣ��4ʱ�䣭����
#define w_dspEquipAlarmLast3 w_dspParLst[94]   //.0 �豸ͣ��ԭ����3��
#define w_dspEquipAlarm3YM w_dspParLst[95]	   //.0 �豸ͣ��3ʱ�䣭����
#define w_dspEquipAlarm3DH w_dspParLst[96]	   //.0 �豸ͣ��3ʱ�䣭��ʱ
#define w_dspEquipAlarm3MS w_dspParLst[97]	   //.0 �豸ͣ��3ʱ�䣭����
#define w_dspEquipAlarmLast2 w_dspParLst[98]   //.0 �豸ͣ��ԭ����2��
#define w_dspEquipAlarm2YM w_dspParLst[99]	   //.0 �豸ͣ��2ʱ�䣭����
#define w_dspEquipAlarm2DH w_dspParLst[100]	   //.0 �豸ͣ��2ʱ�䣭��ʱ
#define w_dspEquipAlarm2MS w_dspParLst[101]	   //.0 �豸ͣ��2ʱ�䣭����
#define w_dspEquipAlarmLast1 w_dspParLst[102]  //.0 �豸ͣ��ԭ����1��
#define w_dspEquipAlarm1YM w_dspParLst[103]	   //.0 �豸ͣ��1ʱ�䣭����
#define w_dspEquipAlarm1DH w_dspParLst[104]	   //.0 �豸ͣ��1ʱ�䣭��ʱ
#define w_dspEquipAlarm1MS w_dspParLst[105]	   //.0 �豸ͣ��1ʱ�䣭����
#define w_dspSelEquipAlarm w_dspParLst[106]	   //.0 ѡ����豸ͣ��ԭ��
#define w_dspSelEquipAlarm1YM w_dspParLst[107] //.0 ѡ����豸ͣ��ʱ�䣭����
#define w_dspSelEquipAlarm1DH w_dspParLst[108] //.0 ѡ����豸ͣ��ʱ�䣭��ʱ
#define w_dspSelEquipAlarm1MS w_dspParLst[109] //.0 ѡ����豸ͣ��ʱ�䣭����
#define w_dspFlashRecNo w_dspParLst[110]	   //.0 FLASH��¼��
#define w_dspFlashRecNum w_dspParLst[111]	   //.0 FLASH��¼����

#define w_dspdeadComp1Ds w_dspParLst[120]	   //.0 w_dspAPhaseCalibrationZero   2017.2.4
#define w_dspdeadComp1Qs w_dspParLst[121]	   //.0 w_dspBPhaseCalibrationZero
#define w_dspFrqComp w_dspParLst[122]		   //.0 w_dspAPhaseCalibrationZero   2017.2.4
#define w_dspFrqCompAfterLimt w_dspParLst[123] //.0 w_dspBPhaseCalibrationZero

// ��ʱע���������������������������ӵı������󣬲��޸���Ӧ�� ARM��DSP���������������� ZCL 2018.9.21

/* #define	w_dspSysErrorCode1  		w_dspParLst[30]	  // ϵͳ������E0	0~65535	 */
#define F_dspOverCurrent MEM_ADDR(BITBAND((u32) & w_dspParLst[30], 0))			 // ��������	0/1
#define F_dspOverVoltage MEM_ADDR(BITBAND((u32) & w_dspParLst[30], 1))			 // ��ѹ����	0/1
#define F_dspUnderVoltage MEM_ADDR(BITBAND((u32) & w_dspParLst[30], 2))			 // Ƿѹ����	0/1
#define F_dspLosePhase MEM_ADDR(BITBAND((u32) & w_dspParLst[30], 3))			 // ȱ�ౣ��	0/1
#define F_dspIpmFault MEM_ADDR(BITBAND((u32) & w_dspParLst[30], 4))				 // IPM����	0/1
#define F_dspDLHFault MEM_ADDR(BITBAND((u32) & w_dspParLst[30], 5))				 // ������ ����������	0/1
#define F_dspIpmHot MEM_ADDR(BITBAND((u32) & w_dspParLst[30], 6))				 // IPM����	0/1
#define F_dspMoterHot MEM_ADDR(BITBAND((u32) & w_dspParLst[30], 7))				 // �������	0/1
#define F_dspFanFault MEM_ADDR(BITBAND((u32) & w_dspParLst[30], 8))				 // ���ȹ���	0/1
#define F_dspEepromFault MEM_ADDR(BITBAND((u32) & w_dspParLst[30], 9))			 // EEPROM����	0/1
#define F_dspDspFault MEM_ADDR(BITBAND((u32) & w_dspParLst[30], 10))			 // DSP����������	0/1
#define F_dspStm32Fault MEM_ADDR(BITBAND((u32) & w_dspParLst[30], 11))			 // STM����������	0/1
#define F_dspGroundProtect MEM_ADDR(BITBAND((u32) & w_dspParLst[30], 12))		 // �ӵر���	0/1
#define F_dspAddSpeedUnderVoltage MEM_ADDR(BITBAND((u32) & w_dspParLst[30], 13)) // ������Ƿ��ѹ	0/1
#define F_dspSubSpeedOverVoltage MEM_ADDR(BITBAND((u32) & w_dspParLst[30], 14))	 // �����й���ѹ	0/1

/* #define	w_dspSysRunStatus1  		w_dspParLst[32]	  // ϵͳ����״̬��1 */
#define F_dspFan1Run MEM_ADDR(BITBAND((u32) & w_dspParLst[32], 0))		// ����1������־λ	0/1
#define F_dspFan2Run MEM_ADDR(BITBAND((u32) & w_dspParLst[32], 1))		// ����2������־λ	0/1
#define F_dspQ1Status MEM_ADDR(BITBAND((u32) & w_dspParLst[32], 2))		// �̵���1��ǰ���״̬	0/1
#define F_dspQ2Status MEM_ADDR(BITBAND((u32) & w_dspParLst[32], 3))		// �̵���2��ǰ���״̬	0/1
#define F_dspDspRun MEM_ADDR(BITBAND((u32) & w_dspParLst[32], 4))		// DSP���б�־	0/1
#define F_dspDspPwmOut MEM_ADDR(BITBAND((u32) & w_dspParLst[32], 5))	// DSP���PWM��־	0/1
#define F_dspDspStartLock MEM_ADDR(BITBAND((u32) & w_dspParLst[32], 6)) // DSP��������	0/1
#define F_dspMainJDQ MEM_ADDR(BITBAND((u32) & w_dspParLst[32], 7))		// DSP����·�̵���״̬	0/1

// 5. SCR Һ��������--------------------------------------------------------------------------

// ��ʱע���������������������������ӵı������󣬲��޸���Ӧ�� ARM��DSP���������������� ZCL 2018.9.21

//------------- GPRS ZCL 2018.12.10
#define SCR_PAR_SIZE 50	 // NV ��������	��
#define DTU_PAR_SIZE 288 // DTU��������	�ֽ�

// ZCL 2018.12.10
//  ����GPRS�趨������RAM�еĵ�ַ 2012.7.23
#define MobileServerBase 0			  // 8+1 �������:01
#define AccessPointBase 9			  // 24+1 Access Point Name ���������:05
#define DtuNoBase 34				  // 11+1 DTU���ʶ����:06
#define MaxSendLengthBase 46		  // 2 ��������:07
#define LastPacketIdleIntervalBase 48 // 2 ��������ʱ����:08 600���� QITCFG=3,2,512,1 200
#define DataReturnBase 50			  // 1 ֧�����ݻ�ִ:09
#define AutoAnswerBase 51			  // 1 �Զ�Ӧ��:0A
#define RedialIntervalBase 52		  // 2 �ز���ʱ����:0B
#define ConsoleInfoTypeBase 54		  // 1 ����̨��Ϣ����:0C
#define BaudRateBase 55				  // 8 ���ڹ�����ʽ:0D
#define PacketCompartIDBase 63		  // 1 ���ݰ��ָ���ʶ:0E
#define EnTDPBase 64				  // 1 ����TDPЭ��:0F
#define TerminalTypeBase 65			  // 1 �ն�����:0x10
#define CallTypeBase 66				  // 1 ���з�ʽ:0x11
#define CallIntervalBase 67			  // 2 ����ʱ����:0x12
#define OfflineIntervalBase 69		  // 2 ��������ʱ����:0x13
#define OfflineModeBase 71			  // 1 ��������ģʽ:0x14

#define Ip0Base 72					  // 4 IP��ַ:0x20------
#define Ip1Base 76					  // 4 IP��ַ:0x30
#define Ip2Base 80					  // 4 IP��ַ:0x40
#define Ip3Base 84					  // 4 IP��ַ:0x50
#define Port0Base 88				  // 2	�˿ں�:0x22------
#define Port1Base 90				  // 2	�˿ں�:0x32
#define Port2Base 92				  // 2	�˿ں�:0x42
#define Port3Base 94				  // 2	�˿ں�:0x52
#define LoclaPort0Base 96			  // 2 ���ض˿ں�:0x23------
#define LoclaPort1Base 98			  // 2 ���ض˿ں�:0x33
#define LoclaPort2Base 100			  // 2 ���ض˿ں�:0x43
#define LoclaPort3Base 102			  // 2 ���ض˿ں�:0x53
#define LinkTCPUDP0Base 104			  // 1 TCP=1.UDP=0 ����:0x24------
#define LinkTCPUDP1Base 105			  // 1 TCP.UDP ����:0x34
#define LinkTCPUDP2Base 106			  // 1 TCP.UDP ����:0x44
#define LinkTCPUDP3Base 107			  // 1 TCP.UDP ����:0x54
#define EnDDP0Base 108				  // 1 DDPЭ��:0x25  =1DDPЭ�飻=0͸��Э��------
#define EnDDP1Base 109				  // 1 DDPЭ��:0x35
#define EnDDP2Base 110				  // 1 DDPЭ��:0x45
#define EnDDP3Base 111				  // 1 DDPЭ��:0x55
#define OnlineReportInterval0Base 112 // 2 ���߱���ʱ����:0x26 ����ʱ�� 40��------
#define OnlineReportInterval1Base 114 // 2 ���߱���ʱ����:0x36 ����ʱ�� 40��
#define OnlineReportInterval2Base 116 // 2 ���߱���ʱ����:0x46 ����ʱ�� 40��
#define OnlineReportInterval3Base 118 // 2 ���߱���ʱ����:0x56 ����ʱ�� 40��
#define NetCheckInterval0Base 120	  // 2 ������ʱ����:0x27------
#define NetCheckInterval1Base 122	  // 2 ������ʱ����:0x37
#define NetCheckInterval2Base 124	  // 2 ������ʱ����:0x47
#define NetCheckInterval3Base 126	  // 2 ������ʱ����:0x57
#define MaxLoginDSCNum0Base 128		  // 1 ����½DSC����:0x28------
#define MaxLoginDSCNum1Base 129		  // 1 ����½DSC����:0x38
#define MaxLoginDSCNum2Base 130		  // 1 ����½DSC����:0x48
#define MaxLoginDSCNum3Base 131		  // 1 ����½DSC����:0x58
#define MaxNetCheckNum0Base 132		  // 1 ������������:0x29------
#define MaxNetCheckNum1Base 133		  // 1 ������������:0x39
#define MaxNetCheckNum2Base 134		  // 1 ������������:0x49
#define MaxNetCheckNum3Base 135		  // 1 ������������:0x59
#define ControlLevel0Base 136		  // 1 ���Ƽ���:0x2B------
#define ControlLevel1Base 137		  // 1 ���Ƽ���:0x3B
#define ControlLevel2Base 138		  // 1 ���Ƽ���:0x4B
#define ControlLevel3Base 139		  // 1 ���Ƽ���:0x5B
#define ReConnect0Base 140			  // 2 ������ʱ����:0x2C------
#define ReConnect1Base 142			  // 2 ������ʱ����:0x3C
#define ReConnect2Base 144			  // 2 ������ʱ����:0x4C
#define ReConnect3Base 146			  // 2 ������ʱ����:0x5C

#define LinkModeBase 148	 // 1 ͨ������ģʽ:0x60 "------"
#define TcpKeepaliveBase 149 // 2 TCP����ʱ����:0x61
#define DNSAddressBase 151	 // 4 DNS��ַ:0x62
#define PPPKeepaliveBase 155 // 2 PPP����ʱ����:0x63

#define DomainName0Base 157 // 30+1	����:0x21------
#define DomainName1Base 188 // 30+1	����:0x31
#define DomainName2Base 219 // 30+1	����:0x41
#define DomainName3Base 250 // 30+1	����:0x51

// volatile uint	xdata	w_GprsParLst[50];					// �������б���
//  Gprs LoRa Scr ���趨����������w_GprsParLst[]���飬���浽Һ��������ARM Flash��
//  #define	Pw_GprsOutPDecMax					w_GprsParLst[0]		//.2 ��ˮ�����̡���ʼ��ֵ1660����Ӧ1.66MP
//  #define	Pw_GprsInPDecMax					w_GprsParLst[1]		//.2 ��ˮ�����̡���ʼ��ֵ1000����Ӧ1.00MP
#define Pw_RebootInterval w_GprsParLst[0]	  //.0 �������ʱ�� ����
#define Pw_ConsoleInfo w_GprsParLst[1]		  //.0 =1��������ʾ����̨��Ϣ
#define Pw_LoRaEquipmentNo w_GprsParLst[2]	  //.0 LORA�豸ͨѶ��ַ ZCL 2018.12.13
#define Pw_LoRaMasterSlaveSel w_GprsParLst[3] //.0 �����ӻ�ѡ��
#define Pw_LoRaModule1Add w_GprsParLst[4]	  //.0 LORAģ��1��ַ
#define Pw_LoRaModule2Add w_GprsParLst[5]	  //.0 LORAģ��2��ַ
#define Pw_LoRaModule3Add w_GprsParLst[6]	  //.0 LORAģ��3��ַ
#define Pw_LoRaModule4Add w_GprsParLst[7]	  //.0 LORAģ��4��ַ

// #define Pw_GprsFaultRestart w_GprsParLst[8] //.0 ������������ 2012.10.18
#define Pw_EquipmentType w_GprsParLst[8] //.0 �豸���ͣ�=0��˫���ã�=1����Ƶ���

#define Pw_ScrEquipPower w_GprsParLst[9] //.1 �豸���� 2019.9.11

// ZCL 2019.9.11 ����γ�Ⱥ;��ȣ���ֹ���ǵ�ʱ�����㣬�������
#define Pw_ScrGpsLatAllDu1 w_GprsParLst[10] //.2 γ�� ��1�� �磺35.44			2λС��
#define Pw_ScrGpsLatAllDu2 w_GprsParLst[11] //.0 γ�� ��2��	00.00****			****���֣���ӵ�w_ScrGpsLatAllDu1���档 �磺****�� 2459�� ����γ���ǣ� 35.442459
#define Pw_ScrGpsLonAllDu1 w_GprsParLst[12] //.2 ���� ��1��	�磺127.39		2λС��
#define Pw_ScrGpsLonAllDu2 w_GprsParLst[13] //.0 ���� ��2�� 00.00****			****���֣���ӵ�w_ScrGpsLonAllDu1���档�磺****�� 4628�� ���ܾ����ǣ� 127.394628

#define Pw_LoRaModule5Add w_GprsParLst[14] //.0 LORAģ��5��ַ  ZCL 2019.10.18
#define Pw_LoRaModule6Add w_GprsParLst[15] //.0 LORAģ��6��ַ  ZCL 2019.10.18
// 11-15

#define Pw_LoRaSet w_GprsParLst[16]		   //.0 LoRa �趨  LORA SX1278
										   // ��LoRa Gprs �趨�����ֿ���������һ�����ı������趨��
#define Pw_GprsSet w_GprsParLst[17]		   //.0 Gprs �趨
#define Pw_ScrLoRaTest1 w_GprsParLst[18]   //.0 �������2  LORA SX1278
#define Pw_ScrLoRaSend2 w_GprsParLst[19]   //.0 �������3  LORA SX1278
#define Pw_ScrKeyMode w_GprsParLst[20]	   //.0 Һ��������ģʽ
										   //=0��8������������ģʽ; =1��7������;	=2��8�����������¼�����
#define Pw_ScrMenuMode w_GprsParLst[21]	   //.0 �˵�ģʽ 	=0��������ģʽ; =1����Ƶ��ģʽ��
#define Pw_ScrCurrentBits w_GprsParLst[22] //.0 ����С�� 	=2������ģʽ; =1�����ʱ�Ƶ��ģʽ��

// 23 24
#define Pw_LoRaModule7Add w_GprsParLst[23] //.0 LORAģ��7��ַ  ZCL 2019.10.18
#define Pw_LoRaModule8Add w_GprsParLst[24] //.0 LORAģ��8��ַ  ZCL 2019.10.18

#define Pw_GprsSetSecond w_GprsParLst[25]	  //.0 ������	// ע��:��ַ�������Ķ�,������ʹ��  ZCL
#define Pw_GprsSetMinute w_GprsParLst[26]	  //.0 ���÷�
#define Pw_GprsSetHour w_GprsParLst[27]		  //.0 ����ʱ
#define Pw_GprsSetDay w_GprsParLst[28]		  //.0 ������
#define Pw_GprsSetTest w_GprsParLst[29]		  //.0 ����			ZCL 2019.3.22
#define Pw_GprsBaudRate3 w_GprsParLst[30]	  //.0 ����3�����ʣ��̶���57600���������û�ã�
#define Pw_GprsBaudRate2 w_GprsParLst[31]	  //.0 ����2������
#define Pw_GprsDataBitLen2 w_GprsParLst[32]	  //.0 ����2 ����λ����
#define Pw_GprsStopBitLen2 w_GprsParLst[33]	  //.0 ����2 ֹͣλ����
#define Pw_GprsParityBitLen2 w_GprsParLst[34] //.0 ����2 У��λ����
#define Pw_GprsFlowControl2 w_GprsParLst[35]  //.0 ����2 ��������

// LORA ������趨���� LoRaSettings  sx1276-LoRa.c
#define Pw_LoRaSetFreq w_GprsParLst[36]				//.0 ���� 4350000
#define Pw_LoRaSetPower w_GprsParLst[37]			//.0 ���� 20
#define Pw_LoRaSetSignalBW w_GprsParLst[38]			//.0 ���� 0-9
#define Pw_LoRaSetSpreadingFactor w_GprsParLst[39]	//.0 ���� 6-12
#define Pw_LoRaSetErrorCoding w_GprsParLst[40]		//.0 ���� 1-4
#define Pw_LoRaSetCrcOn w_GprsParLst[41]			//.0 0-1
#define Pw_LoRaSetImplicitHeaderOn w_GprsParLst[42] //.0 0-1
#define Pw_LoRaSetRxSingleOn w_GprsParLst[43]		//.0 0-1
#define Pw_LoRaSetFreqHopOn w_GprsParLst[44]		//.0 0
#define Pw_LoRaSetHopPeriod w_GprsParLst[45]		//.0
													// ZCL 2019.3.22 �ռ䲻����
#define Pw_LoRaSetPayLoadLength w_GprsParLst[46]	//
#define Pw_LoRaSetTxPacketTimeOut w_GprsParLst[47]	//.0 //ZCL 2019.9.24 �����¼���
#define Pw_LoRaSetRxPacketTimeOut w_GprsParLst[48]	//.0 //ZCL 2019.9.24 �����¼���

// ����Э�� YLS 2023.11.28
#define w_ZhouShanProtocol w_GprsParLst[49]										 // ����Э��
#define w_ZhouShanProtocol_bit0 MEM_ADDR(BITBAND((u32) & w_ZhouShanProtocol, 0)) // �����1·ͨ����=0��ͨ��Э�飻=1��ɽЭ��
#define w_ZhouShanProtocol_bit1 MEM_ADDR(BITBAND((u32) & w_ZhouShanProtocol, 1)) // �����2·ͨ����=0��ͨ��Э�飻=1��ɽЭ��
#define w_ZhouShanProtocol_bit2 MEM_ADDR(BITBAND((u32) & w_ZhouShanProtocol, 2)) // �����3·ͨ����=0��ͨ��Э�飻=1��ɽЭ��
#define w_ZhouShanProtocol_bit3 MEM_ADDR(BITBAND((u32) & w_ZhouShanProtocol, 3)) // �����4·ͨ����=0��ͨ��Э�飻=1��ɽЭ��

#define w_ZhouShanProtocol_bit4 MEM_ADDR(BITBAND((u32) & w_ZhouShanProtocol, 4)) // 4-6λ�����������ͼ��ʱ��
#define w_ZhouShanProtocol_bit5 MEM_ADDR(BITBAND((u32) & w_ZhouShanProtocol, 5)) // 000:20���ӣ�001��10���ӣ�010:5���ӣ�011��2����
#define w_ZhouShanProtocol_bit6 MEM_ADDR(BITBAND((u32) & w_ZhouShanProtocol, 6)) // 100��1���ӣ�101:30�룻110:20�룻111��10��
#define w_ZhouShanProtocol_bit7 MEM_ADDR(BITBAND((u32) & w_ZhouShanProtocol, 7)) // =0������32���֣�=1������40����

#define w_ZhouShanProtocol_bit8 MEM_ADDR(BITBAND((u32) & w_ZhouShanProtocol, 8))   // ����2����Э�飬=0��ͨ��Э�飻=1����ɽЭ��
#define w_ZhouShanProtocol_bit9 MEM_ADDR(BITBAND((u32) & w_ZhouShanProtocol, 9))   // ����1�������ͣ�=1���������ͣ�=0������������
#define w_ZhouShanProtocol_bit10 MEM_ADDR(BITBAND((u32) & w_ZhouShanProtocol, 10)) // ����1����LoRa��������շ���=1����أ�=0�������
#define w_ZhouShanProtocol_bit11 MEM_ADDR(BITBAND((u32) & w_ZhouShanProtocol, 11)) // ����

#define w_ZhouShanProtocol_bit12 MEM_ADDR(BITBAND((u32) & w_ZhouShanProtocol, 12)) // ����
#define w_ZhouShanProtocol_bit13 MEM_ADDR(BITBAND((u32) & w_ZhouShanProtocol, 13)) // ����
#define w_ZhouShanProtocol_bit14 MEM_ADDR(BITBAND((u32) & w_ZhouShanProtocol, 14)) // ����
#define w_ZhouShanProtocol_bit15 MEM_ADDR(BITBAND((u32) & w_ZhouShanProtocol, 15)) // ����

// ����ۿ�����
#define w_GprsSaveBaudRate3 w_GprsParLst[50]	 //.0 ���洮��3������
#define w_GprsSaveBaudRate2 w_GprsParLst[51]	 //.0 ���洮��2������
#define w_GprsSaveDataBitLen2 w_GprsParLst[52]	 //.0 ���洮��2 ����λ����
#define w_GprsSaveStopBitLen2 w_GprsParLst[53]	 //.0 ���洮��2 ֹͣλ����
#define w_GprsSaveParityBitLen2 w_GprsParLst[54] //.0 ���洮��2 У��λ����
#define w_GprsSaveFlowControl2 w_GprsParLst[55]	 //.0 ���洮��2 ��������
#define w_GprsModPar w_GprsParLst[56]			 //.0 �޸Ĳ���		//ZCL 2019.9.11 ��w_ModPar=2000Ҳ�������޸�SCR����
#define w_GprsParInitial w_GprsParLst[57]		 //.0 ������ʼ�� //ZCL 2019.9.11 ֻ����w_GprsParInitial����ʼ��SCR����

#define w_GprsXmodemFTItem w_GprsParLst[61] //.0 Xmodem�ļ����� ��  ZCL 2019.4.5
#define w_GprsXmodemRcvLen w_GprsParLst[62] //.0 Xmodem���ճ���
#define w_GprsTemp2 w_GprsParLst[63]		//.0
#define w_GprsCSQ w_GprsParLst[64]			//.0 �ź�ǿ��  2017.5.25
#define w_GprsSoftVer w_GprsParLst[65]		//.2 ����汾
// ZCL 2019.9.24
#define w_LoRaDateRate w_GprsParLst[66] //.2 ��������	 kbps

#define w_LoRaLeiDaModuleAdd w_GprsParLst[67] //.0 ZCL 2020.9.30 ר�������״���� SZM220��ַ
#define w_LoRaLeiDaDiValue w_GprsParLst[68]	  //.0 ZCL 2020.9.30 �״���� SZM220 DIֵ
// 11 ���¹��̲���
#define w_ScrSaveKeyMode w_GprsParLst[71]	  //.0 ����Һ��������ģʽ
#define w_ScrVERSION w_GprsParLst[72]		  //.0 Һ��������汾
#define w_ScrWriteYear w_GprsParLst[73]		  //.0 ZCL 2018.4.21  2018.8.4
#define w_ScrWriteDate w_GprsParLst[74]		  //.0 ZCL 2018.4.21  2018.8.4
#define w_ScrSaveMenuMode w_GprsParLst[75]	  //.0 ����˵�ģʽ 	=0��BPDJģʽ; =1����Ƶ��ģʽ��
#define w_ScrSaveCurrentBits w_GprsParLst[76] //.0 �������С�� 	=2������ģʽ; =1�����ʱ�Ƶ��ģʽ��
#define w_ScrTemp1 w_GprsParLst[77]			  //.0 2018.10.19

#define w_ScrGpsLatDu w_GprsParLst[78]	 //.0 γ�� �ȣ�	ddmm.mmmm֮dd
#define w_ScrGpsLatFen1 w_GprsParLst[79] //.0 γ�� �֣�		ddmm.mmmm֮mm
#define w_ScrGpsLatFen2 w_GprsParLst[80] //.0 γ�� �֣�		ddmm.mmmm֮mmmm
#define w_ScrGpsLonDu w_GprsParLst[81]	 //.0 ���� �ȣ�	dddmm.mmmm֮ddd
#define w_ScrGpsLonFen1 w_GprsParLst[82] //.0 ���� �֣�		dddmm.mmmm֮mm
#define w_ScrGpsLonFen2 w_GprsParLst[83] //.0 ���� �֣�		dddmm.mmmm֮mmmm

// ZCL 2019.3.20˵��
//  w_ScrGpsLatAllDu1 �� w_ScrGpsLatAllDu2 ��������γ��
//  w_ScrGpsLonAllDu1 �� w_ScrGpsLonAllDu2 �������Ǿ���

// #define	w_ScrGpsLatAllDu1				w_GprsParLst[84]		//.2 γ�� ��1�� �磺35.44			2λС��
// #define	w_ScrGpsLatAllDu2				w_GprsParLst[85]		//.0 γ�� ��2��	00.00****			****���֣���ӵ�w_ScrGpsLatAllDu1���档 �磺****�� 2459�� ����γ���ǣ� 35.442459
// #define	w_ScrGpsLonAllDu1				w_GprsParLst[86]		//.2 ���� ��1��	�磺127.39		2λС��
// #define	w_ScrGpsLonAllDu2				w_GprsParLst[87]		//.0 ���� ��2�� 00.00****			****���֣���ӵ�w_ScrGpsLonAllDu1���档�磺****�� 4628�� ���ܾ����ǣ� 127.394628

// ZCL 2019.3.11
// ��Ƶ���1

// ZCL 2019.3.11
// ��Ƶ���1
#define w_dsp1SoftVersion w_GprsParLst[88]	  //.2 DSP���ϵͳ�汾��	0~65535
#define w_dsp1NowHz w_GprsParLst[89]		  //.1 ��ǰƵ��	0~500	x10
#define w_dsp1APhaseCurrent w_GprsParLst[90]  //.2 A����� 0~65535	x100
#define w_dsp1OutVoltage w_GprsParLst[91]	  //.0 �����ѹ	0~500	x1
#define w_dsp1NowTemperature w_GprsParLst[92] //.1 ��ǰ�¶�	0~1000	x10
#define w_dsp1SysErrorCode1 w_GprsParLst[93]  //.0 ϵͳ������E0	0~65535	 ʹ��λ
#define w_dsp1SysRunStatus1 w_GprsParLst[94]  //.0 ϵͳ����״̬��1		ʹ��λ
#define w_dsp1AI1ADCValue w_GprsParLst[95]	  //.0 AI1 ADC�ɼ�ֵ ZCL 2016.6.21
#define w_dsp1AI2ADCValue w_GprsParLst[96]	  //.0 AI2 ADC�ɼ�ֵ
#define w_dsp1Counter3 w_GprsParLst[97]		  //.0 DCM220 DCM100: dsp����������

// #define	w_dsp1BusBarVoltage			w_GprsParLst[93]		//.0 ĸ�ߵ�ѹ	0~800	x1
// #define	w_dsp1TargetHz					w_GprsParLst[99]	//.1 Ŀ��Ƶ��	0~500	x10		����

// #define	w_dsp1Tmp1							w_GprsParLst[13]
// #define	w_dsp1Tmp2							w_GprsParLst[14]
// #define	w_dsp1Tmp3							w_GprsParLst[15]

// ��Ƶ���2
#define w_dsp2SoftVersion w_GprsParLst[98]	   //.2 DSP���ϵͳ�汾��	0~65535
#define w_dsp2NowHz w_GprsParLst[99]		   //.1 ��ǰƵ��	0~500	x10
#define w_dsp2APhaseCurrent w_GprsParLst[100]  //.2 A����� 0~65535	x100
#define w_dsp2OutVoltage w_GprsParLst[101]	   //.0 �����ѹ	0~500	x1
#define w_dsp2NowTemperature w_GprsParLst[102] //.1 ��ǰ�¶�	0~1000	x10
#define w_dsp2SysErrorCode1 w_GprsParLst[103]  //.0 ϵͳ������E0	0~65535	 ʹ��λ
#define w_dsp2SysRunStatus1 w_GprsParLst[104]  //.0 ϵͳ����״̬��1		ʹ��λ
#define w_dsp2AI1ADCValue w_GprsParLst[105]	   //.0 AI1 ADC�ɼ�ֵ ZCL 2016.6.21
#define w_dsp2AI2ADCValue w_GprsParLst[106]	   //.0 AI2 ADC�ɼ�ֵ
#define w_dsp2Counter3 w_GprsParLst[107]	   //.0 DCM220 DCM100: dsp����������

// ��Ƶ���3
#define w_dsp3SoftVersion w_GprsParLst[108]	   //.2 DSP���ϵͳ�汾��	0~65535
#define w_dsp3NowHz w_GprsParLst[109]		   //.1 ��ǰƵ��	0~500	x10
#define w_dsp3APhaseCurrent w_GprsParLst[110]  //.2 A����� 0~65535	x100
#define w_dsp3OutVoltage w_GprsParLst[111]	   //.0 �����ѹ	0~500	x1
#define w_dsp3NowTemperature w_GprsParLst[112] //.1 ��ǰ�¶�	0~1000	x10
#define w_dsp3SysErrorCode1 w_GprsParLst[113]  //.0 ϵͳ������E0	0~65535	 ʹ��λ
#define w_dsp3SysRunStatus1 w_GprsParLst[114]  //.0 ϵͳ����״̬��1		ʹ��λ
#define w_dsp3AI1ADCValue w_GprsParLst[115]	   //.0 AI1 ADC�ɼ�ֵ ZCL 2016.6.21
#define w_dsp3AI2ADCValue w_GprsParLst[116]	   //.0 AI2 ADC�ɼ�ֵ
#define w_dsp3Counter3 w_GprsParLst[117]	   //.0 DCM220 DCM100: dsp����������

// ��Ƶ���4
#define w_dsp4SoftVersion w_GprsParLst[118]	   //.2 DSP���ϵͳ�汾��	0~65535
#define w_dsp4NowHz w_GprsParLst[119]		   //.1 ��ǰƵ��	0~500	x10
#define w_dsp4APhaseCurrent w_GprsParLst[120]  //.2 A����� 0~65535	x100
#define w_dsp4OutVoltage w_GprsParLst[121]	   //.0 �����ѹ	0~500	x1
#define w_dsp4NowTemperature w_GprsParLst[122] //.1 ��ǰ�¶�	0~1000	x10
#define w_dsp4SysErrorCode1 w_GprsParLst[123]  //.0 ϵͳ������E0	0~65535	 ʹ��λ
#define w_dsp4SysRunStatus1 w_GprsParLst[124]  //.0 ϵͳ����״̬��1		ʹ��λ
#define w_dsp4AI1ADCValue w_GprsParLst[125]	   //.0 AI1 ADC�ɼ�ֵ ZCL 2016.6.21
#define w_dsp4AI2ADCValue w_GprsParLst[126]	   //.0 AI2 ADC�ɼ�ֵ
#define w_dsp4Counter3 w_GprsParLst[127]	   //.0 DCM220 DCM100: dsp����������

// ZCL 2019.9.11  ��Ƶ���1
#define w_1InPDec w_GprsParLst[128]				//.3 ��ˮѹ��
#define w_1OutPDec w_GprsParLst[129]			//.3 ��ˮѹ��
#define w_1PIDCalcP w_GprsParLst[130]			//.3 �趨ѹ��
#define w_1EquipOperateStatus w_GprsParLst[131] //.0 ��ˮ״̬
#define w_1EquipAlarmStatus w_GprsParLst[132]	//.0 ͣ��ԭ��
#define w_1ScrEquipPower w_GprsParLst[133]		//.0 �豸����
#define w_1NoUse1 w_GprsParLst[134]				//.0 Ԥ��1
#define w_1NoUse2 w_GprsParLst[135]				//.0 Ԥ��2
#define w_1NoUse3 w_GprsParLst[136]				//.0 Ԥ��3
#define w_1NoUse4 w_GprsParLst[137]				//.0 Ԥ��4

// ZCL 2019.9.11  ��Ƶ���2
#define w_2InPDec w_GprsParLst[138]				//.3 ��ˮѹ��
#define w_2OutPDec w_GprsParLst[139]			//.3 ��ˮѹ��
#define w_2PIDCalcP w_GprsParLst[140]			//.3 �趨ѹ��
#define w_2EquipOperateStatus w_GprsParLst[141] //.0 ��ˮ״̬
#define w_2EquipAlarmStatus w_GprsParLst[142]	//.0 ͣ��ԭ��
#define w_2ScrEquipPower w_GprsParLst[143]		//.0 �豸����
#define w_2NoUse1 w_GprsParLst[144]				//.0 Ԥ��1
#define w_2NoUse2 w_GprsParLst[145]				//.0 Ԥ��2
#define w_2NoUse3 w_GprsParLst[146]				//.0 Ԥ��3
#define w_2NoUse4 w_GprsParLst[147]				//.0 Ԥ��4

// ZCL 2019.9.11  ��Ƶ���3
#define w_3InPDec w_GprsParLst[148]				//.3 ��ˮѹ��
#define w_3OutPDec w_GprsParLst[149]			//.3 ��ˮѹ��
#define w_3PIDCalcP w_GprsParLst[150]			//.3 �趨ѹ��
#define w_3EquipOperateStatus w_GprsParLst[151] //.0 ��ˮ״̬
#define w_3EquipAlarmStatus w_GprsParLst[152]	//.0 ͣ��ԭ��
#define w_3ScrEquipPower w_GprsParLst[153]		//.0 �豸����
#define w_3NoUse1 w_GprsParLst[154]				//.0 Ԥ��1
#define w_3NoUse2 w_GprsParLst[155]				//.0 Ԥ��2
#define w_3NoUse3 w_GprsParLst[156]				//.0 Ԥ��3
#define w_3NoUse4 w_GprsParLst[157]				//.0 Ԥ��4

// ZCL 2019.9.11  ��Ƶ���4
#define w_4InPDec w_GprsParLst[158]				//.3 ��ˮѹ��
#define w_4OutPDec w_GprsParLst[159]			//.3 ��ˮѹ��
#define w_4PIDCalcP w_GprsParLst[160]			//.3 �趨ѹ��
#define w_4EquipOperateStatus w_GprsParLst[161] //.0 ��ˮ״̬
#define w_4EquipAlarmStatus w_GprsParLst[162]	//.0 ͣ��ԭ��
#define w_4ScrEquipPower w_GprsParLst[163]		//.0 �豸����
#define w_4NoUse1 w_GprsParLst[164]				//.0 Ԥ��1
#define w_4NoUse2 w_GprsParLst[165]				//.0 Ԥ��2
#define w_4NoUse3 w_GprsParLst[166]				//.0 Ԥ��3
#define w_4NoUse4 w_GprsParLst[167]				//.0 Ԥ��4

#define w_Com1TXCounter w_GprsParLst[168] // ZCL 2019.10.19 ��������ͨѶʱ�۲���
#define w_Com1RXCounter w_GprsParLst[169] // ZCL 2019.10.19 ��������ͨѶʱ�۲���

#define w_LoraCounter1 w_GprsParLst[170] // LORA	������
#define w_LoraCounter2 w_GprsParLst[171] // LORA	������
#define w_LoraCounter3 w_GprsParLst[172] // LORA	������
#define w_LoraCounter4 w_GprsParLst[173] // LORA	������
#define w_LoraCounter5 w_GprsParLst[174] // LORA	������
#define w_LoraCounter6 w_GprsParLst[174] // LORA	������
#define w_LoraCounter7 w_GprsParLst[174] // LORA	������
#define w_LoraCounter8 w_GprsParLst[174] // LORA	������

// u16  	w_GprsParLst[180];  //ZCL 2019.10.19 ע�ⳤ��

// ZCL 2019.2.18
/* 	(0). Pw_LoRaSet .0λ =1   LORA����	F_LoRaEn
		(1). Pw_LoRaSet .1λ =1   LORA�����COM2 (͸��ʹ��)	F_LoRaToCom

		(0). Pw_GprsSet .0λ =1   GPRS����	F_GprsEn
		(1). Pw_GprsSet .1λ =1   GPRS͸��ģʽ�������COM2����ģʽ��=GP311���ܣ�	F_GprsTransToCom

		(2). Pw_GprsSet .2λ =1   ����GPRSģʽ		F_GprsMasterNotToCom					//ZCL �¼�  2019.3.11
		(3). Pw_GprsSet .3λ =1   ����GPRSģʽ�µĽ������������COM2�����������	F_GprsMasterToCom //ZCL �¼�  2019.3.11

		(4). Pw_GprsSet .4λ =1  ����0x0010 	//GPS�� ͸�����������COM2  .4λ ֵ��16		F_GpsToCom

	 */

#define F_LoRaEn MEM_ADDR(BITBAND((u32) & Pw_LoRaSet, 0))	 // LORA����	0/1
#define F_LoRaToCom MEM_ADDR(BITBAND((u32) & Pw_LoRaSet, 1)) // LORA�����COM2 (͸��ʹ��)		0/1

#define F_GprsEn MEM_ADDR(BITBAND((u32) & Pw_GprsSet, 0))		  // GPRS����	0/1
#define F_GprsTransToCom MEM_ADDR(BITBAND((u32) & Pw_GprsSet, 1)) // GPRS͸��ģʽ�������COM2		0/1

// ZCL 2019.3.11 ����GPRSģʽ�£��������Ϣ�����ڣ�Ĭ�ϣ�	0/1
#define F_GprsMasterNotToCom MEM_ADDR(BITBAND((u32) & Pw_GprsSet, 2)) // MasterNotToCom

// ZCL 2019.3.11 ����GPRSģʽ���趨�����Ϣ������	 2019.3.11
#define F_GprsMasterToCom MEM_ADDR(BITBAND((u32) & Pw_GprsSet, 3)) // MasterToCom

#define F_GpsToCom MEM_ADDR(BITBAND((u32) & Pw_GprsSet, 4)) // GPS�� ͸�����������COM2	0/1

// ZCL 2019.3.11
// ZCL 2019.9.28  21��  25��  ˮ��
// ������Щ�����Ӽ�ر�Ƶ����ĳ�������ֲ������ԭ������w_dsp1ParLst[88]-[114]�е�
#define w_WaterMeterID1 w_GprsParLst[188]		   //.1
#define w_WaterMeterID2 w_GprsParLst[189]		   //.1
#define w_WaterMeterSum1 w_GprsParLst[190]		   //.1
#define w_WaterMeterSum2 w_GprsParLst[191]		   //.1
#define w_WaterMeterInstantFlow1 w_GprsParLst[192] //.1
#define w_WaterMeterInstantFlow2 w_GprsParLst[193] //.1
#define w_WaterMeterInSum1 w_GprsParLst[194]	   //.x
#define w_WaterMeterInSum2 w_GprsParLst[195]	   //.x

#define w_WaterMeterOutSum1 w_GprsParLst[196]	   //.x
#define w_WaterMeterOutSum2 w_GprsParLst[197]	   //.x
#define w_WaterMeterRemainMoney1 w_GprsParLst[198] //.x
#define w_WaterMeterRemainMoney2 w_GprsParLst[199] //.x
#define w_WaterMeterOverdraft1 w_GprsParLst[200]   //.x
#define w_WaterMeterOverdraft2 w_GprsParLst[201]   //.x
#define w_WaterMeterStatus1 w_GprsParLst[202]	   //.x
#define w_WaterMeterStatus2 w_GprsParLst[203]	   //.x

#define w_WaterMeterNoUse1 w_GprsParLst[204]  //.x
#define w_WaterMeterNoUse2 w_GprsParLst[205]  //.x
#define w_WaterMeterTime1 w_GprsParLst[206]	  //.x
#define w_WaterMeterTime2 w_GprsParLst[207]	  //.x
#define w_WaterMeterTime3 w_GprsParLst[208]	  //.x
#define w_WaterMeterSoftVer w_GprsParLst[209] //.2 ����汾
#define w_WaterMeterNoUse3 w_GprsParLst[210]  //.x
#define w_WaterMeterNoUse4 w_GprsParLst[211]  //.x
#define w_WaterMeterNoUse5 w_GprsParLst[212]  //.x
#define w_WaterMeterNoUse6 w_GprsParLst[213]  //.x
#define w_WaterMeterNoUse7 w_GprsParLst[214]  //.x

// ZCL 2019.11.21 Pw_LoRaSet��Pw_GprsSet�趨�Ͳ���˵����
// ��1��ͨ��LORA�����趨��
//  Pw_LoRaSet=1��F_LoRaEn=1��F_LoRaToCom=0����
//  Pw_GprsSet=9��F_GprsEn=1��F_GprsTransToCom=0��F_GprsMasterNotToCom=0��F_GprsMasterToCom=1��

// ��2����ʹ�ô���2����LORA����ʱ���趨˵����
//  Pw_LoRaSet=1��F_LoRaEn=1��F_LoRaToCom=0����
//  Pw_GprsSet=0��F_GprsEn=0��F_GprsTransToCom=0��F_GprsMasterNotToCom=0��F_GprsMasterToCom=0�� GPRS�ر�
//  Pw_GprsSet=1 GPRS�򿪣�ָ��͵�COM���������Ϻ�� gprsdemo.exe����demo��ָ�û�з��أ�
//  Pw_GprsSet=5 GPRS�򿪣�ָ��͵�COM����ΪF_GprsMasterNotToCom=1���������Ϻ�� gprsdemo.exe����demo��ָ����Է��أ�

//----------LoRa ���������� �����ӻ��� ����������
// u16  	w_dsp1ParLst[50];				// �������б��� ��д ZCL 2019.3.7
// u16  	w_dsp2ParLst[50];				// �������б��� ��д ZCL 2019.3.7
// u16  	w_dsp3ParLst[50];				// �������б��� ��д ZCL 2019.3.7
// u16  	w_dsp4ParLst[50];				// �������б��� ��д ZCL 2019.3.7
// ZCL 2019.3.11 ɾ�����ŵ�w_GprsParLst[]

// ZCL 2022.6.1 220��LORAԶ���������ġ� ��ע��ZCL 2022.9.13  ͨ��485�ɼ����ӻ��ĵ������ݣ��͵�������Ϳ���
//  ���ܱ�1ֱ���Ǳ�������䵽��� ���ܱ�2,3,4 ͨѶ����䵽���
// ���ܱ�1
#define w_Lora1PtL w_DNBParLst[0] // �����й�����
#define w_Lora1PtH w_DNBParLst[1] // �����й�����

#define w_Lora1SetVfFreq w_DNBParLst[2]	 // �趨�ı�ƵƵ�� ZCL 2023.3.20
#define w_Lora1OutVoltage w_DNBParLst[3] // �����ѹ  ZCL 2023.3.20

#define w_Lora1EptL w_DNBParLst[4] // �����й����ܣ������ö������㣩
#define w_Lora1EptH w_DNBParLst[5] // �����й����ܣ������ö������㣩

#define w_Lora1VfErrCode w_DNBParLst[6]	  // ��Ƶ�����ϴ���  ZCL 2023.2.10
#define w_Lora1SoftVersion w_DNBParLst[7] // ����汾
#define w_Lora1XiaoLv w_DNBParLst[8]	  // Ч��
#define w_Lora1StopNum w_DNBParLst[9]	  // ͣ������ ZCL 2023.2.10		//ѹ��2

#define w_Lora1ShunShiFlowL w_DNBParLst[10] // ˲ʱ����  ZCL 2022.5.17
#define w_Lora1ShunShiFlowH w_DNBParLst[11] // ˲ʱ����

#define w_Lora1SumFlowL w_DNBParLst[12] // �ۼ�����   ZCLע���ı���ʾ������ ������ǰ �����ں�
#define w_Lora1SumFlowH w_DNBParLst[13] // �ۼ�����

#define w_Lora1UabRms w_DNBParLst[14] // Ab�ߵ�ѹ��Чֵ

#define w_Lora1TouchRunStop w_DNBParLst[15] // ���� ����/ֹͣ  =1 ���У�=0 ͣ��

#define w_Lora1Speed w_DNBParLst[16] // ���ת��

#define w_Lora1Pft w_DNBParLst[17] // ���๦������

#define w_Lora1Counter1_BPQ w_DNBParLst[18] // ���ܱ�/��Ƶ��������
#define w_Lora1Counter2 w_DNBParLst[19]		// SZM220������

#define w_Lora1IaRms w_DNBParLst[20] // A�������Чֵ

#define w_Lora1YeWei1 w_DNBParLst[21] // Һλ1  ZCL 2023.8.8
#define w_Lora1YeWei2 w_DNBParLst[22] // Һλ2  ZCL 2023.8.8

#define w_Lora1PumpRunSecond w_DNBParLst[23] //	����ۼ�����ʱ����
#define w_Lora1PumpRunMinute w_DNBParLst[24] //	����ۼ�����ʱ�����
#define w_Lora1PumpRunHourL w_DNBParLst[25]	 //	����ۼ�����ʱ��Сʱ����
#define w_Lora1PumpRunHourH w_DNBParLst[26]	 //	����ۼ�����ʱ��Сʱ����

#define w_Lora1VvvfFreq w_DNBParLst[27] // ��Ƶ��Ƶ��  ZCL 2022.8.19

#define w_Lora1InP w_DNBParLst[28]			 // ��ˮѹ��
#define w_Lora1OutP w_DNBParLst[29]			 // ��ˮѹ��
#define w_Lora1WenDu1 w_DNBParLst[30]		 // �¶�1
#define w_Lora1WenDu2 w_DNBParLst[31]		 // �¶�2
#define w_Lora1SetP w_DNBParLst[32]			 // �趨ѹ��
#define w_Lora1Flag1Unit w_DNBParLst[33]	 // ��־1��Ԫ  �豸״̬
#define w_Lora1Flag2Unit w_DNBParLst[34]	 // ��־2��Ԫ
#define w_Lora1Flag3Unit w_DNBParLst[35]	 // ��־3��Ԫ
#define w_Lora1Pump12Status w_DNBParLst[36]	 // ��״̬
#define w_Lora1RemoteZDRunHz w_DNBParLst[37] // ң�ض�Ƶ  ZCL 2024.1.5  ����10.0HZ��������

#define F_LoRa1_Comm_Fault MEM_ADDR(BITBAND((u32) & w_Lora1Flag3Unit, 0))	  // 1#��LORAͨѶ�쳣	0/1
#define F_LoRa1_VVVFComm_Fault MEM_ADDR(BITBAND((u32) & w_Lora1Flag3Unit, 1)) // 1#�ñ�Ƶ��ͨѶ�쳣	0/1

// ���ܱ�2
#define w_Lora2PtL w_DNBParLst[0 + 40] // �����й�����
#define w_Lora2PtH w_DNBParLst[1 + 40] // �����й�����

#define w_Lora2StL w_DNBParLst[2 + 40] // �������ڹ���
#define w_Lora2StH w_DNBParLst[3 + 40] // �������ڹ���

#define w_Lora2EptL w_DNBParLst[4 + 40] // �����й����ܣ������ö������㣩
#define w_Lora2EptH w_DNBParLst[5 + 40] // �����й����ܣ������ö������㣩

#define w_Lora2YaLi w_DNBParLst[6 + 40]		   // ѹ��  δʹ�ã�Ԥ��
#define w_Lora2SoftVersion w_DNBParLst[7 + 40] // ����汾
#define w_Lora2XiaoLv w_DNBParLst[8 + 40]	   // Ч��
#define w_Lora2YaLi2 w_DNBParLst[9 + 40]	   // ѹ��2  δʹ�ã�Ԥ��

#define w_Lora2ShunShiFlowL w_DNBParLst[10 + 40] // ˲ʱ����  ZCL 2022.5.17
#define w_Lora2ShunShiFlowH w_DNBParLst[11 + 40] // ˲ʱ����

#define w_Lora2SumFlowL w_DNBParLst[12 + 40] // �ۼ�����   ZCLע���ı���ʾ������ ������ǰ �����ں�
#define w_Lora2SumFlowH w_DNBParLst[13 + 40] // �ۼ�����

#define w_Lora2UabRms w_DNBParLst[14 + 40] // Ab�ߵ�ѹ��Чֵ

#define w_Lora2TouchRunStop w_DNBParLst[15 + 40] // ���� ����/ֹͣ  =1 ���У�=0 ͣ��

#define w_Lora2Speed w_DNBParLst[16 + 40] // ���ת��

#define w_Lora2Pft w_DNBParLst[17 + 40] // ���๦������

#define w_Lora2Counter1_BPQ w_DNBParLst[18 + 40] // ���ܱ������
#define w_Lora2Counter2 w_DNBParLst[19 + 40]	 // SZM220������

#define w_Lora2IaRms w_DNBParLst[20 + 40] // A�������Чֵ

#define w_Lora2QtL w_DNBParLst[21 + 40] // �����޹�����
#define w_Lora2QtH w_DNBParLst[22 + 40] // �����޹�����

#define w_Lora2PumpRunSecond w_DNBParLst[23 + 40] //	����ۼ�����ʱ����
#define w_Lora2PumpRunMinute w_DNBParLst[24 + 40] //	����ۼ�����ʱ�����
#define w_Lora2PumpRunHourL w_DNBParLst[25 + 40]  //	����ۼ�����ʱ��Сʱ����
#define w_Lora2PumpRunHourH w_DNBParLst[26 + 40]  //	����ۼ�����ʱ��Сʱ����

#define w_Lora2VvvfFreq w_DNBParLst[27 + 40] // ��Ƶ��Ƶ��  ZCL 2022.8.19

#define w_Lora2InP w_DNBParLst[28 + 40]			 // ��ˮѹ��
#define w_Lora2OutP w_DNBParLst[29 + 40]		 // ��ˮѹ��
#define w_Lora2WenDu1 w_DNBParLst[30 + 40]		 // �¶�1
#define w_Lora2WenDu2 w_DNBParLst[31 + 40]		 // �¶�2
#define w_Lora2SetP w_DNBParLst[32 + 40]		 // �趨ѹ��
#define w_Lora2Flag1Unit w_DNBParLst[33 + 40]	 // ��־1��Ԫ  �豸״̬
#define w_Lora2Flag2Unit w_DNBParLst[34 + 40]	 // ��־2��Ԫ
#define w_Lora2Flag3Unit w_DNBParLst[35 + 40]	 // ��־3��Ԫ
#define w_Lora2Pump12Status w_DNBParLst[36 + 40] // ��״̬

#define F_LoRa2_Comm_Fault MEM_ADDR(BITBAND((u32) & w_Lora2Flag3Unit, 0))	  // 2#��LORAͨѶ�쳣	0/1
#define F_LoRa2_VVVFComm_Fault MEM_ADDR(BITBAND((u32) & w_Lora2Flag3Unit, 1)) // 2#�ñ�Ƶ��ͨѶ�쳣	0/1

// ���ܱ�3  ZCL  2022.9.13
#define w_Lora3PtL w_DNBParLst[0 + 80] // �����й�����
#define w_Lora3PtH w_DNBParLst[1 + 80] // �����й�����

#define w_Lora3StL w_DNBParLst[2 + 80] // �������ڹ���
#define w_Lora3StH w_DNBParLst[3 + 80] // �������ڹ���

#define w_Lora3EptL w_DNBParLst[4 + 80] // �����й����ܣ������ö������㣩
#define w_Lora3EptH w_DNBParLst[5 + 80] // �����й����ܣ������ö������㣩

#define w_Lora3YaLi w_DNBParLst[6 + 80]		   // ѹ��  δʹ�ã�Ԥ��
#define w_Lora3SoftVersion w_DNBParLst[7 + 80] // ����汾
#define w_Lora3XiaoLv w_DNBParLst[8 + 80]	   // Ч��
#define w_Lora3YaLi2 w_DNBParLst[9 + 80]	   // ѹ��2  δʹ�ã�Ԥ��

#define w_Lora3ShunShiFlowL w_DNBParLst[10 + 80] // ˲ʱ����  ZCL 2022.5.17
#define w_Lora3ShunShiFlowH w_DNBParLst[11 + 80] // ˲ʱ����

#define w_Lora3SumFlowL w_DNBParLst[12 + 80] // �ۼ�����   ZCLע���ı���ʾ������ ������ǰ �����ں�
#define w_Lora3SumFlowH w_DNBParLst[13 + 80] // �ۼ�����

#define w_Lora3UabRms w_DNBParLst[14 + 80] // Ab�ߵ�ѹ��Чֵ

#define w_Lora3TouchRunStop w_DNBParLst[15 + 80] // ���� ����/ֹͣ  =1 ���У�=0 ͣ��

#define w_Lora3Speed w_DNBParLst[16 + 80] // ���ת��

#define w_Lora3Pft w_DNBParLst[17 + 80] // ���๦������

#define w_Lora3Counter1_BPQ w_DNBParLst[18 + 80] // ���ܱ������
#define w_Lora3Counter2 w_DNBParLst[19 + 80]	 // SZM220������

#define w_Lora3IaRms w_DNBParLst[20 + 80] // A�������Чֵ

#define w_Lora3QtL w_DNBParLst[21 + 80] // �����޹�����
#define w_Lora3QtH w_DNBParLst[22 + 80] // �����޹�����

#define w_Lora3PumpRunSecond w_DNBParLst[23 + 80] //	����ۼ�����ʱ����
#define w_Lora3PumpRunMinute w_DNBParLst[24 + 80] //	����ۼ�����ʱ�����
#define w_Lora3PumpRunHourL w_DNBParLst[25 + 80]  //	����ۼ�����ʱ��Сʱ����
#define w_Lora3PumpRunHourH w_DNBParLst[26 + 80]  //	����ۼ�����ʱ��Сʱ����

#define w_Lora3VvvfFreq w_DNBParLst[27 + 80] // ��Ƶ��Ƶ��  ZCL 2022.8.19

#define w_Lora3InP w_DNBParLst[28 + 80]			 // ��ˮѹ��
#define w_Lora3OutP w_DNBParLst[29 + 80]		 // ��ˮѹ��
#define w_Lora3WenDu1 w_DNBParLst[30 + 80]		 // �¶�1
#define w_Lora3WenDu2 w_DNBParLst[31 + 80]		 // �¶�2
#define w_Lora3SetP w_DNBParLst[32 + 80]		 // �趨ѹ��
#define w_Lora3Flag1Unit w_DNBParLst[33 + 80]	 // ��־1��Ԫ  �豸״̬
#define w_Lora3Flag2Unit w_DNBParLst[34 + 80]	 // ��־2��Ԫ
#define w_Lora3Flag3Unit w_DNBParLst[35 + 80]	 // ��־3��Ԫ
#define w_Lora3Pump12Status w_DNBParLst[36 + 80] // ��״̬

#define F_LoRa3_Comm_Fault MEM_ADDR(BITBAND((u32) & w_Lora3Flag3Unit, 0))	  // 3#��LORAͨѶ�쳣	0/1
#define F_LoRa3_VVVFComm_Fault MEM_ADDR(BITBAND((u32) & w_Lora3Flag3Unit, 1)) // 3#�ñ�Ƶ��ͨѶ�쳣	0/1

// ���ܱ�4  ZCL  2022.9.13
#define w_Lora4PtL w_DNBParLst[0 + 120] // �����й�����
#define w_Lora4PtH w_DNBParLst[1 + 120] // �����й�����

#define w_Lora4StL w_DNBParLst[2 + 120] // �������ڹ���
#define w_Lora4StH w_DNBParLst[3 + 120] // �������ڹ���

#define w_Lora4EptL w_DNBParLst[4 + 120] // �����й����ܣ������ö������㣩
#define w_Lora4EptH w_DNBParLst[5 + 120] // �����й����ܣ������ö������㣩

#define w_Lora4YaLi w_DNBParLst[6 + 120]		// ѹ��  δʹ�ã�Ԥ��
#define w_Lora4SoftVersion w_DNBParLst[7 + 120] // ����汾
#define w_Lora4XiaoLv w_DNBParLst[8 + 120]		// Ч��
#define w_Lora4YaLi2 w_DNBParLst[9 + 120]		// ѹ��2  δʹ�ã�Ԥ��

#define w_Lora4ShunShiFlowL w_DNBParLst[10 + 120] // ˲ʱ����  ZCL 2022.5.17
#define w_Lora4ShunShiFlowH w_DNBParLst[11 + 120] // ˲ʱ����

#define w_Lora4SumFlowL w_DNBParLst[12 + 120] // �ۼ�����   ZCLע���ı���ʾ������ ������ǰ �����ں�
#define w_Lora4SumFlowH w_DNBParLst[13 + 120] // �ۼ�����

#define w_Lora4UabRms w_DNBParLst[14 + 120] // Ab�ߵ�ѹ��Чֵ

#define w_Lora4TouchRunStop w_DNBParLst[15 + 120] // ���� ����/ֹͣ  =1 ���У�=0 ͣ��

#define w_Lora4Speed w_DNBParLst[16 + 120] // ���ת��

#define w_Lora4Pft w_DNBParLst[17 + 120] // ���๦������

#define w_Lora4Counter1_BPQ w_DNBParLst[18 + 120] // ���ܱ������
#define w_Lora4Counter2 w_DNBParLst[19 + 120]	  // SZM220������

#define w_Lora4IaRms w_DNBParLst[20 + 120] // A�������Чֵ

#define w_Lora4QtL w_DNBParLst[21 + 120] // �����޹�����
#define w_Lora4QtH w_DNBParLst[22 + 120] // �����޹�����

#define w_Lora4PumpRunSecond w_DNBParLst[23 + 120] //	����ۼ�����ʱ����
#define w_Lora4PumpRunMinute w_DNBParLst[24 + 120] //	����ۼ�����ʱ�����
#define w_Lora4PumpRunHourL w_DNBParLst[25 + 120]  //	����ۼ�����ʱ��Сʱ����
#define w_Lora4PumpRunHourH w_DNBParLst[26 + 120]  //	����ۼ�����ʱ��Сʱ����

#define w_Lora4VvvfFreq w_DNBParLst[27 + 120] // ��Ƶ��Ƶ��  ZCL 2022.8.19

#define w_Lora4InP w_DNBParLst[28 + 120]		  // ��ˮѹ��
#define w_Lora4OutP w_DNBParLst[29 + 120]		  // ��ˮѹ��
#define w_Lora4WenDu1 w_DNBParLst[30 + 120]		  // �¶�1
#define w_Lora4WenDu2 w_DNBParLst[31 + 120]		  // �¶�2
#define w_Lora4SetP w_DNBParLst[32 + 120]		  // �趨ѹ��
#define w_Lora4Flag1Unit w_DNBParLst[33 + 120]	  // ��־1��Ԫ  �豸״̬
#define w_Lora4Flag2Unit w_DNBParLst[34 + 120]	  // ��־2��Ԫ
#define w_Lora4Flag3Unit w_DNBParLst[35 + 120]	  // ��־3��Ԫ
#define w_Lora4Pump12Status w_DNBParLst[36 + 120] // ��״̬

#define F_LoRa4_Comm_Fault MEM_ADDR(BITBAND((u32) & w_Lora4Flag3Unit, 0))	  // 4#��LORAͨѶ�쳣	0/1
#define F_LoRa4_VVVFComm_Fault MEM_ADDR(BITBAND((u32) & w_Lora4Flag3Unit, 1)) // 4#�ñ�Ƶ��ͨѶ�쳣	0/1

// ���ܱ�5
#define w_Lora5PtL w_DNBParLst[0 + 160] // �����й�����
#define w_Lora5PtH w_DNBParLst[1 + 160] // �����й�����
#define w_Lora5StL w_DNBParLst[2 + 160] // �������ڹ���
#define w_Lora5StH w_DNBParLst[3 + 160] // �������ڹ���

#define w_Lora5EptL w_DNBParLst[4 + 160] // �����й����ܣ������ö������㣩
#define w_Lora5EptH w_DNBParLst[5 + 160] // �����й����ܣ������ö������㣩

#define w_Lora5YaLi w_DNBParLst[6 + 160]		// ѹ��  δʹ�ã�Ԥ��
#define w_Lora5SoftVersion w_DNBParLst[7 + 160] // ����汾
#define w_Lora5XiaoLv w_DNBParLst[8 + 160]		// Ч��
#define w_Lora5YaLi2 w_DNBParLst[9 + 160]		// ѹ��2  δʹ�ã�Ԥ��

#define w_Lora5ShunShiFlowL w_DNBParLst[10 + 160] // ˲ʱ����  ZCL 2022.5.17
#define w_Lora5ShunShiFlowH w_DNBParLst[11 + 160] // ˲ʱ����

#define w_Lora5SumFlowL w_DNBParLst[12 + 160] // �ۼ�����   ZCLע���ı���ʾ������ ������ǰ �����ں�
#define w_Lora5SumFlowH w_DNBParLst[13 + 160] // �ۼ�����

#define w_Lora5UabRms w_DNBParLst[14 + 160] // Ab�ߵ�ѹ��Чֵ

#define w_Lora5TouchRunStop w_DNBParLst[15 + 160] // ���� ����/ֹͣ  =1 ���У�=0 ͣ��

#define w_Lora5Speed w_DNBParLst[16 + 160] // ���ת��

#define w_Lora5Pft w_DNBParLst[17 + 160] // ���๦������

#define w_Lora5Counter1_BPQ w_DNBParLst[18 + 160] // ���ܱ������
#define w_Lora5Counter2 w_DNBParLst[19 + 160]	  // SZM220������

#define w_Lora5IaRms w_DNBParLst[20 + 160] // A�������Чֵ

#define w_Lora5QtL w_DNBParLst[21 + 160] // �����޹�����
#define w_Lora5QtH w_DNBParLst[22 + 160] // �����޹�����

#define w_Lora5PumpRunSecond w_DNBParLst[23 + 160] //	����ۼ�����ʱ����
#define w_Lora5PumpRunMinute w_DNBParLst[24 + 160] //	����ۼ�����ʱ�����
#define w_Lora5PumpRunHourL w_DNBParLst[25 + 160]  //	����ۼ�����ʱ��Сʱ����
#define w_Lora5PumpRunHourH w_DNBParLst[26 + 160]  //	����ۼ�����ʱ��Сʱ����

#define w_Lora5VvvfFreq w_DNBParLst[27 + 160] // ��Ƶ��Ƶ��  ZCL 2022.8.19

#define w_Lora5InP w_DNBParLst[28 + 160]		  // ��ˮѹ��
#define w_Lora5OutP w_DNBParLst[29 + 160]		  // ��ˮѹ��
#define w_Lora5WenDu1 w_DNBParLst[30 + 160]		  // �¶�1
#define w_Lora5WenDu2 w_DNBParLst[31 + 160]		  // �¶�2
#define w_Lora5SetP w_DNBParLst[32 + 160]		  // �趨ѹ��
#define w_Lora5Flag1Unit w_DNBParLst[33 + 160]	  // ��־1��Ԫ  �豸״̬
#define w_Lora5Flag2Unit w_DNBParLst[34 + 160]	  // ��־2��Ԫ
#define w_Lora5Flag3Unit w_DNBParLst[35 + 160]	  // ��־3��Ԫ
#define w_Lora5Pump12Status w_DNBParLst[36 + 160] // ��״̬

#define F_LoRa5_Comm_Fault MEM_ADDR(BITBAND((u32) & w_Lora5Flag3Unit, 0))	  // 5#��LORAͨѶ�쳣	0/1
#define F_LoRa5_VVVFComm_Fault MEM_ADDR(BITBAND((u32) & w_Lora5Flag3Unit, 1)) // 5#�ñ�Ƶ��ͨѶ�쳣	0/1

// ���ܱ�6
#define w_Lora6Flag3Unit w_DNBParLst[35 + 200]								  // ��־3��Ԫ
#define F_LoRa6_Comm_Fault MEM_ADDR(BITBAND((u32) & w_Lora6Flag3Unit, 0))	  // 6#��LORAͨѶ�쳣	0/1
#define F_LoRa6_VVVFComm_Fault MEM_ADDR(BITBAND((u32) & w_Lora6Flag3Unit, 1)) // 6#�ñ�Ƶ��ͨѶ�쳣	0/1

// ���ܱ�7
#define w_Lora7Flag3Unit w_DNBParLst[35 + 240]								  // ��־3��Ԫ
#define F_LoRa7_Comm_Fault MEM_ADDR(BITBAND((u32) & w_Lora7Flag3Unit, 0))	  // 7#��LORAͨѶ�쳣	0/1
#define F_LoRa7_VVVFComm_Fault MEM_ADDR(BITBAND((u32) & w_Lora7Flag3Unit, 1)) // 7#�ñ�Ƶ��ͨѶ�쳣	0/1

// ���ܱ�8
#define w_Lora8Flag3Unit w_DNBParLst[35 + 280]								  // ��־3��Ԫ
#define F_LoRa8_Comm_Fault MEM_ADDR(BITBAND((u32) & w_Lora8Flag3Unit, 0))	  // 8#��LORAͨѶ�쳣	0/1
#define F_LoRa8_VVVFComm_Fault MEM_ADDR(BITBAND((u32) & w_Lora8Flag3Unit, 1)) // 8#�ñ�Ƶ��ͨѶ�쳣	0/1

// ��ͣ��ַ���壬����ͨ��Һ�����޸�ֱ�ӿ�����ͣ
#define Addr_Lora1TouchRunStop 15  // ���� ����/ֹͣ  =1 ���У�=0 ͣ��
#define Addr_Lora2TouchRunStop 55  // ���� ����/ֹͣ  =1 ���У�=0 ͣ��
#define Addr_Lora3TouchRunStop 95  // ���� ����/ֹͣ  =1 ���У�=0 ͣ��
#define Addr_Lora4TouchRunStop 135 // ���� ����/ֹͣ  =1 ���У�=0 ͣ��
#define Addr_Lora5TouchRunStop 175 // ���� ����/ֹͣ  =1 ���У�=0 ͣ��

// ZCL 2022.5.19   2019.3.20˵��
//  w_ScrGpsLatAllDu1 �� w_ScrGpsLatAllDu2 ��������γ��
//  w_ScrGpsLonAllDu1 �� w_ScrGpsLonAllDu2 �������Ǿ���
#define w_DNBScrGpsLatAllDu1 w_DNBParLst[160 + 500] //.2 γ�� ��1�� �磺35.44			2λС��
#define w_DNBScrGpsLatAllDu2 w_DNBParLst[161 + 500] //.0 γ�� ��2��	00.00****
													//****���֣���ӵ�w_ScrGpsLatAllDu1���档 �磺****�� 2459�� ����γ���ǣ� 35.442459
#define w_DNBScrGpsLonAllDu1 w_DNBParLst[162 + 500] //.2 ���� ��1��	�磺127.39		2λС��
#define w_DNBScrGpsLonAllDu2 w_DNBParLst[163 + 500] //.0 ���� ��2�� 00.00****
													//****���֣���ӵ�w_ScrGpsLonAllDu1���档�磺****�� 4628�� ���ܾ����ǣ� 127.394628

#define w_Pump1DispWenDu1DecValue w_DNBParLst[170 + 500] //.1 �¶�ֵ1
#define w_Pump1DispWenDu2DecValue w_DNBParLst[171 + 500] //.1 �¶�ֵ2
#define w_Pump2DispWenDu1DecValue w_DNBParLst[172 + 500] //.1 �¶�ֵ1
#define w_Pump2DispWenDu2DecValue w_DNBParLst[173 + 500] //.1 �¶�ֵ2
#define w_Pump3DispWenDu1DecValue w_DNBParLst[174 + 500] //.1 �¶�ֵ1
#define w_Pump3DispWenDu2DecValue w_DNBParLst[175 + 500] //.1 �¶�ֵ2
#define w_Pump4DispWenDu1DecValue w_DNBParLst[176 + 500] //.1 �¶�ֵ1
#define w_Pump4DispWenDu2DecValue w_DNBParLst[177 + 500] //.1 �¶�ֵ2
#define w_Pump5DispWenDu1DecValue w_DNBParLst[178 + 500] //.1 �¶�ֵ1
#define w_Pump5DispWenDu2DecValue w_DNBParLst[179 + 500] //.1 �¶�ֵ2

// ZCL 2023.2.10
#define w_Pump1RemoteStop w_DNBParLst[181 + 500]	 //.0 ��1ң����ͣ			//ZCL 2023.2.10
#define w_Pump2RemoteStop w_DNBParLst[182 + 500]	 //.0 ��2ң����ͣ
#define w_Pump3RemoteStop w_DNBParLst[183 + 500]	 //.0 ��3ң����ͣ
#define w_Pump4RemoteStop w_DNBParLst[184 + 500]	 //.0 ��4ң����ͣ
#define w_Pump5RemoteStop w_DNBParLst[185 + 500]	 //.0 ��5ң����ͣ
#define w_SavePump1RemoteStop w_DNBParLst[186 + 500] //.0 ��1ң����ͣ			//ZCL 2023.2.10
#define w_SavePump2RemoteStop w_DNBParLst[187 + 500] //.0 ��2ң����ͣ
#define w_SavePump3RemoteStop w_DNBParLst[188 + 500] //.0 ��3ң����ͣ
#define w_SavePump4RemoteStop w_DNBParLst[189 + 500] //.0 ��4ң����ͣ
#define w_SavePump5RemoteStop w_DNBParLst[190 + 500] //.0 ��5ң����ͣ

// ZCL 2023.2.10
#define w_ZhuanFaAdd w_DNBParLst[191 + 500]	 //.0 Ԥ��
#define w_ZhuanFaData w_DNBParLst[192 + 500] //.0 Ԥ��	//ZCL 2022.3.21

// #define w_Com1RcVOkCounter w_DNBParLst[220 + 500] // ������ �������� +1  ZCL 2023.3.2
// #define w_Com2RcVOkCounter w_DNBParLst[221 + 500] // ������ �������� +1
// #define w_Com3RcVOkCounter w_DNBParLst[222 + 500] // ������ �������� +1
// SIM7600�õĲ���

// ��һ·ͨ����IP��ַ
#define Pw_Link1IP1 Pw_ParLst_GPRS[0]
#define Pw_Link1IP2 Pw_ParLst_GPRS[1]
#define Pw_Link1IP3 Pw_ParLst_GPRS[2]
#define Pw_Link1IP4 Pw_ParLst_GPRS[3]

// �ڶ�·ͨ����IP��ַ
#define Pw_Link2IP1 Pw_ParLst_GPRS[4]
#define Pw_Link2IP2 Pw_ParLst_GPRS[5]
#define Pw_Link2IP3 Pw_ParLst_GPRS[6]
#define Pw_Link2IP4 Pw_ParLst_GPRS[7]

// ����·ͨ����IP��ַ
#define Pw_Link3IP1 Pw_ParLst_GPRS[8]
#define Pw_Link3IP2 Pw_ParLst_GPRS[9]
#define Pw_Link3IP3 Pw_ParLst_GPRS[10]
#define Pw_Link3IP4 Pw_ParLst_GPRS[11]

// ����·ͨ����IP��ַ
#define Pw_Link4IP1 Pw_ParLst_GPRS[12]
#define Pw_Link4IP2 Pw_ParLst_GPRS[13]
#define Pw_Link4IP3 Pw_ParLst_GPRS[14]
#define Pw_Link4IP4 Pw_ParLst_GPRS[15]

#define Pw_Link1localPort Pw_ParLst_GPRS[16] // ����1�˿�
#define Pw_Link2localPort Pw_ParLst_GPRS[17] // ����2�˿�
#define Pw_Link3localPort Pw_ParLst_GPRS[18] // ����3�˿�
#define Pw_Link4localPort Pw_ParLst_GPRS[19] // ����4�˿�

#define Pw_Link1Port Pw_ParLst_GPRS[20] // ��һ·����Զ�˶˿�
#define Pw_Link2Port Pw_ParLst_GPRS[21] // �ڶ�·����Զ�˶˿�
#define Pw_Link3Port Pw_ParLst_GPRS[22] // ����·����Զ�˶˿�
#define Pw_Link4Port Pw_ParLst_GPRS[23] // ����·����Զ�˶˿�

#define Pw_Link1TCP_UDPType Pw_ParLst_GPRS[24] // 0:UDP,1:TCP
#define Pw_Link2TCP_UDPType Pw_ParLst_GPRS[25]
#define Pw_Link3TCP_UDPType Pw_ParLst_GPRS[26]
#define Pw_Link4TCP_UDPType Pw_ParLst_GPRS[27]

#define Pw_Link1NetWorkCheckTime Pw_ParLst_GPRS[28] // ��������ʱ��
#define Pw_Link2NetWorkCheckTime Pw_ParLst_GPRS[29]
#define Pw_Link3NetWorkCheckTime Pw_ParLst_GPRS[30]
#define Pw_Link4NetWorkCheckTime Pw_ParLst_GPRS[31]

#define Pw_RFModularAddress Pw_ParLst_GPRS[32] // ����ģ��ĵ�ַ
#define Pw_DTUIDNUM1 Pw_ParLst_GPRS[33]		   // DTU����
#define Pw_DTUIDNUM2 Pw_ParLst_GPRS[34]
#define Pw_DTUIDNUM3 Pw_ParLst_GPRS[35]

#define Pw_link1OnlineReportTime Pw_ParLst_GPRS[36] // ����1 ���߱���ʱ����(����)(10����)=5000
#define Pw_link2OnlineReportTime Pw_ParLst_GPRS[37]
#define Pw_link3OnlineReportTime Pw_ParLst_GPRS[38]
#define Pw_link4OnlineReportTime Pw_ParLst_GPRS[39]

#define Pw_link1ReconnectTimeBetwen Pw_ParLst_GPRS[40] // ���ݷ��ʹ���Ӧ��10��,��������8������û����ȷ���ͳɹ�,��200MS�������ӣ�Ĭ��ȡ20(*10ms)
#define Pw_GPRSRTModeSelect Pw_ParLst_GPRS[41]
#define Pw_LastPacketIdleTime Pw_ParLst_GPRS[42] // ��������ʱ����
#define Pw_SupportDataReceipt Pw_ParLst_GPRS[43] //=1��֧��������ʾ��=0������ʾ

#endif /* __GLOBALCONST_H */
