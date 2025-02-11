/**
  ******************************************************************************
  * @file    LCD.c
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.28
  * @date    2014-01-03
  * @brief   Code  for UG-2864KSWLG01 WiseChip ���ӽӿڣ�IC:SSD1306
  * 				 Hardware	: STM32F103(101) C8T6,CBT6
	* 				 Platform	: IAR6.40,Keil-MDK5.01,LIB:V3.5.0(11/03/2011),JLINK V6-V7
	******************************************************************************
  * @attention	2014.1.3
	******************************************************************************
  * @note	2014.4.7
	�ܳ��� 2011.5.9
	1. Һ��RAM����LSB���ϣ�MSB����
	2. �����Ǿ�OLEDҺ��1.3��(UG-2864KSWLG01  WiseChip ���ӽӿ�)��ʾ�����ĵײ㺯��
	3. LCD12864_Num3()����ʾ��ֵ����ͬ������ʾ���治һ��������������ܲ�һ���� ZCL 2018.5.19
	4. ��ѹ����¶ȿ�����ʾ����		//ZCL 2019.10.18 ��ʾ������
	*/

/* Includes ------------------------------------------------------------------*/
#include "LCD.h"
#include "GlobalV_Extern.h" // ȫ�ֱ�������
#include "GlobalConst.h"
#include "DZ_Demo.h"	   //�ָ�����Һ����ʾ�����Դ�����   6x8 8X16 16x16 ͼƬ
#include "DZ_Cat_off.h"	   //è���۵���
#include "DZ_Cat_on.h"	   //è���۵���
#include "DZ_Number1607.h" //0123456789>+KPa .-   16*7����  16*8����
#include "DZ_Number1632.h" //0--9  16*32����
// #include <intrins.h>					//_crol_ ���Բ�����

#define LCD_DELAY_TIME 25

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
// �ܳ��� 2014.4.23 ѡ��ͬ��SEGBEGIN��������ͬ��Һ����
// #define	SEGBEGIN	0			//SSD1306  128*64
#define SEGBEGIN 2 // SH1106G  132*64
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile unsigned long sysTickValue;
u8 Remainder; // ����
u8 Quotient;  // ����

u8 B_DispZero;

/* Private variables extern --------------------------------------------------*/
extern u8 S_ModParStatus;
extern u16 Lw_PageNo; // ��ʾ�Ļ������
extern u8 F_ModPar1;  // �޸Ĳ�����־1	 ����ָ����ҳ���Ǹ�����Ҫ�޸�
/* Private function prototypes -----------------------------------------------*/
void IWDG_Feed(void); // �������Ź�ι��	 2017.6.19
/* Private functions ---------------------------------------------------------*/

/*********************LCD��ʼ��************************************/

/********************************************************************************
							   ��Ƶ���Һ����ʾDCM200_SCR��������
									   copyright @ 2014

���: ZCL  ZYX
����: 2014-09-25
*********************************************************************************/

/*******************************************************************************
��  ��:	void LCD12864_Init(void)
��  ��:	��ʼ��Һ��ģ��.
��  ��:	��

��  ��: ZCL  ZYX
��  ��: 2013-09-09
*********************************************************************************/
void LCD12864_Init(void)
{
	LCD12864_RST_L;			 // ��λ���͵�ƽ
	LCD12864_DelayUS(20000); // ��ʱ

	LCD12864_RST_H;			 // ������λ���ߵ�ƽ����λҺ��
	LCD12864_DelayUS(20000); // ��ʱ
	LCD12864_PSB_H;			 // LCD����ģʽ
	LCD12864_DelayUS(10000);

	LCD12864_WriteCommand(0x30); // ����ָ�
	LCD12864_DelayUS(1000);

	LCD12864_WriteCommand(0x01); // ������Ļ
	LCD12864_DelayUS(10000);

	LCD12864_WriteCommand(0x06); // �����ƶ�����
	LCD12864_DelayUS(1000);

	LCD12864_WriteCommand(0x0C); // ����ʾ,���α�
	LCD12864_DelayUS(1000);
}

/********************************************************************************
��  ��:	void LCD12864_DelayUS(unsigned int Dly)
��  ��:
��  ��:

��  ��: ZCL  ZYX
��  ��: 2013-09-09
********************************************************************************/
void LCD12864_DelayUS(unsigned int Dly)
{
	while (Dly--)
#ifdef GD32			 // ZCL 2021.12.22
		IWDG_Feed(); // ZCL 2021.10.25
#else
		;
#endif
	// KickDog();
	;
}

/********************************************************************************
��  ��:	void LCD12864_WriteOneByte(u8 Data)
��  ��:
��  ��:

��  ��: ZCL  ZYX
��  ��: 2013-09-09
*********************************************************************************/
// ZCL 2015.8.30 ��λд
void LCD12864_WriteOneByte(u8 Data)
{
	if (Data & 0x01)
		LCD12864_DATA0_H;
	else
		LCD12864_DATA0_L;

	if (Data & 0x02)
		LCD12864_DATA1_H;
	else
		LCD12864_DATA1_L;

	if (Data & 0x04)
		LCD12864_DATA2_H;
	else
		LCD12864_DATA2_L;

	if (Data & 0x08)
		LCD12864_DATA3_H;
	else
		LCD12864_DATA3_L;

	if (Data & 0x10)
		LCD12864_DATA4_H;
	else
		LCD12864_DATA4_L;

	if (Data & 0x20)
		LCD12864_DATA5_H;
	else
		LCD12864_DATA5_L;

	if (Data & 0x40)
		LCD12864_DATA6_H;
	else
		LCD12864_DATA6_L;

	if (Data & 0x80)
		LCD12864_DATA7_H;
	else
		LCD12864_DATA7_L;

	LCD12864_DelayUS(10);
}

/********************************************************************************
��  ��:	u8 LCD12864_ReadData(void)
��  ��:
��  ��:

��  ��: ZCL  ZYX
��  ��: 2013-09-09
*********************************************************************************/
u8 LCD12864_ReadOneByte(void)
{
	u8 RData;

	LCD12864_WriteOneByte(0xFF);
	LCD12864_DelayUS(10);

	RData = 0x00;

	if (LCD12864_RDATA0)
		RData |= 0x01;
	if (LCD12864_RDATA1)
		RData |= 0x02;
	if (LCD12864_RDATA2)
		RData |= 0x04;
	if (LCD12864_RDATA3)
		RData |= 0x08;
	if (LCD12864_RDATA4)
		RData |= 0x10;
	if (LCD12864_RDATA5)
		RData |= 0x20;
	if (LCD12864_RDATA6)
		RData |= 0x40;
	if (LCD12864_RDATA7)
		RData |= 0x80;

	return RData;
}

/********************************************************************************
��  ��:	u8 LCD12864_CheckBusy(void)
��  ��:	��ȡҺ��æµ״̬.
��  ��:	1--æµ;0--��æµ.

��  ��: ZCL  ZYX
��  ��: 2013-09-09
**********************************************************************************/
u8 LCD12864_CheckBusy(void)
{
	LCD12864_RS_L; // RS=0;	 L-CMD;H-Data.
	LCD12864_RW_H; // RW=1;	 L-Write;H-Read.
	LCD12864_DelayUS(10);

	LCD12864_EN_H;					  // EN=1;     L-Disable;H-Enable
	LCD12864_DelayUS(LCD_DELAY_TIME); // ��ʱһС��ʱ��,�ȴ����������ȶ�

	// ZCL 2015.8.30 ԭ�����������ش�����Ѿ��޸�
	//	while(LCD12864_RDATA7==1);

	LCD12864_EN_L;
	LCD12864_DelayUS(LCD_DELAY_TIME);

	return 1;
}

/********************************************************************************
��  ��:	void LCD12864_WriteCommand(u8 Data)
��  ��:	дһ���ֽڵ�"������"��Һ����.
��  ��:	��

��  ��: ZCL  ZYX
��  ��: 2013-09-09
*********************************************************************************/
void LCD12864_WriteCommand(u8 myCMD)
{
	LCD12864_CheckBusy(); // ���æµ״̬,���ҵȴ�

	LCD12864_RS_L;					  // RS=0;	 L-CMD;H-Data.
	LCD12864_RW_L;					  // RW=0;	 L-Write;H-Read.
	LCD12864_EN_L;					  // EN=0;     L-Disable;H-Enable
	LCD12864_DelayUS(LCD_DELAY_TIME); // ��ʱ

	LCD12864_WriteOneByte(myCMD);	  //
	LCD12864_DelayUS(LCD_DELAY_TIME); // ��ʱһС��ʱ��,�ȴ����������ȶ�

	LCD12864_EN_H;					  // EN=1;     L-Disable;H-Enable
	LCD12864_DelayUS(LCD_DELAY_TIME); // ��ʱһ��ʱ��,�ȴ����ݱ�Һ��������

	LCD12864_EN_L;					  // EN=1;     L-Disable;H-Enable
	LCD12864_DelayUS(LCD_DELAY_TIME); //

	LCD12864_DelayUS(10000); // ��ʱ
}

/********************************************************************************
��  ��:	void LCD12864_WriteData(u8 myData)
��  ��:	дһ���ֽڵ�"������"��Һ����.
��  ��:	��

��  ��: ZCL  ZYX
��  ��: 2013-09-09
*********************************************************************************/
void LCD12864_WriteData(u8 myData)
{
	LCD12864_CheckBusy(); // ���æµ״̬,���ҵȴ�

	LCD12864_RS_H;					  // RS=1;	 L-CMD;H-Data.  ������дָ�ͬ
	LCD12864_RW_L;					  // RW=0;	 L-Write;H-Read.
	LCD12864_EN_L;					  // EN=0;  L-Disable;H-Enable
	LCD12864_DelayUS(LCD_DELAY_TIME); //

	LCD12864_WriteOneByte(myData);	  //
	LCD12864_DelayUS(LCD_DELAY_TIME); // ��ʱһС��ʱ��,�ȴ����������ȶ�

	LCD12864_EN_H;					  // EN=1;     L-Disable;H-Enable
	LCD12864_DelayUS(LCD_DELAY_TIME); // ��ʱһ��ʱ��,�ȴ����ݱ�Һ��������

	LCD12864_EN_L;					  // EN=1;     L-Disable;H-Enable
	LCD12864_DelayUS(LCD_DELAY_TIME); //
}

/********************************************************************************
��  ��:	u8 LCD12864_ReadData(void)
��  ��:	��Һ����12864��ȡһ���ֽڵ�����.
��  ��:	��

��  ��: ZCL  ZYX
��  ��: 2013-09-09
*********************************************************************************/
u8 LCD12864_ReadData(void)
{
	u8 RData;

	// ZCL ���ܼӣ����ϳ���
	// LCD12864_CheckBusy();			 	//���æµ״̬,���ҵȴ�

	LCD12864_RS_H;					  // RS=1;	 L-CMD;H-Data.
	LCD12864_RW_H;					  // RW=1;	 L-Write;H-Read.		�ߵ�ƽ��
	LCD12864_EN_L;					  // EN=0;  L-Disable;H-Enable
	LCD12864_DelayUS(LCD_DELAY_TIME); //

	LCD12864_EN_H;					  // EN=1;     L-Disable;H-Enable		��ʹ�ܣ��ߵ�ƽ
	LCD12864_DelayUS(LCD_DELAY_TIME); // ��ʱһ��ʱ��,�ȴ������ȶ�

	RData = LCD12864_ReadOneByte(); // ��ȡ����

	LCD12864_EN_L;					  // EN=1;     L-Disable;H-Enable
	LCD12864_DelayUS(LCD_DELAY_TIME); //

	return RData;
}

/********************************************************************************
��  ��:	void LCD12864_Clear(void)
��  ��:	�ڻ�ͼģʽ�������Ļ. ָ��0x01���������ڻ�ͼģʽ,ֻ���������ı�ģʽ.
��  ��:	��

	DB7   DB6   DB5   DB4   DB3   DB2   DB1   DB0
		L     L     H     DL    x     RE    x     x

	DL --  1--8BIT���ƽӿ�; 0--4BIT���ƽӿ�
	RE --  1--����ָ�;   0--����ָ�

	��ͼ��ʾRAM �ṩ64x32���ֽڵļ���ռ�(������ָ���趨��ͼ RAM  ��ַ)��
�����Կ���256x64 ��Ķ�ά��ͼ����ռ䣬�ڸ��Ļ�ͼRAMʱ��������ָ��
�趨GDRAM ��ַ���费ֱ��ַ����ˮƽ��ַ(����д�������ֽڵ����������
��ֱ��ˮƽ�������ַ)����д������8λ�����ϵ���ͼRAM������ַ��������AC��
���Զ���һ,����д���ͼRAM �Ĳ������£�

������Ҫע�����,��ͼRAM(GDRAM)��д�������:
	1.�Ƚ���ֱ���ֽ�����(Y)д���ͼRAM��ַ.
	2.�ڽ�ˮƽ���ֽ�����(X)д���ͼRAM��ַ.
	3.��D15~D8д��RAM��(д���һ��Bytes).
	4.��D7~D0д��RAM��(д��ڶ���Bytes).


��  ��: ZCL  ZYX
��  ��: 2013-09-09
*********************************************************************************/
void LCD12864_Clear(void) // ZCL ? 2015.8.30
{
	u8 i, j;

	LCD12864_WriteCommand(0x36); // ��չָ�

	for (i = 0; i < 32; i++) //
	{
		LCD12864_WriteCommand(0x80 + i); // Y����
		LCD12864_WriteCommand(0x80);	 // X����

		for (j = 0; j < 16; j++) //
		{
			LCD12864_WriteData(0x00); // ��һ���ֽ�����
			LCD12864_WriteData(0x00); // �ڶ����ֽ�����
		}
	}
}

/*******************************************************************************
��  ��:	void LCD12864_Cursor(u8 myDIR)
��  ��: LCD12864�Ĺ����ƺ���,���ƹ�������ƶ�
��  ��:	myDIR   0--�������;1--�������
��  ��:	��

��  ��: ZCL  ZYX
��  ��: 2013-11-20
********************************************************************************/
void LCD12864_Cursor(u8 myDIR)
{
	if (myDIR == 0)
	{
		LCD12864_WriteCommand(0x10); // �������,AC�Զ�����1
	}
	else
	{
		LCD12864_WriteCommand(0x14); // �������,AC�Զ�����1
	}
}

/********************************************************************************
��  ��:	void LCD12864_SetPixelIndex(u8 x,u8 y,u8 Color)
��  ��:	��LCD12864�е��ض���������ʾһ����.
��  ��:	��

��  ��: ZCL  ZYX
��  ��: 2013-09-09
********************************************************************************/
void LCD12864_SetPixelIndex(u8 x, u8 y, u8 Color)
{
	u8 X_Byte;			 // X����
	unsigned int X_Bits; // ��ǵ�ǰ�㴦��x�����ֽ��е���һλ.��Ӧ��λ�ᱻ��1.
	u8 Y_Byte;			 // Y����
	unsigned int PixData;

	// 	LCD12864_WriteCommand(0x34);	    //8Bit���ƽӿ�; ����ָ�

	//--------------------------------------------------------------------------------
	// �������°�����
	if (y >= 32) //
	{
		Y_Byte = 0x80 + (y - 32); // ��Ч��:Y_Byte = 0x80+y%32
		X_Byte = 0x88 + x / 16;
		X_Bits = 0x8000 >> (x % 16); // �������ǰ��x���������ֽ��е���һλ
	}
	//--------------------------------------------------------------------------------
	// �������ϰ�����
	else //
	{
		Y_Byte = 0x80 + y; // ��Ч��:Y_Byte = 0x80+y%32
		X_Byte = 0x80 + x / 16;
		X_Bits = 0x8000 >> (x % 16); // �������ǰ��x���������ֽ��е���һλ
	}

	//--------------------------------------------------------------------------------
	// ��ȡ���ݲ�����,�����LCD��ʾ
	LCD12864_WriteCommand(Y_Byte); // ���ݸ�ʽ�涨��д��Y��ַ
	LCD12864_WriteCommand(X_Byte); // ��д��X��ַ

	LCD12864_ReadData();		   // ��һ�ζ�ȡ������Ч
	PixData = LCD12864_ReadData(); // �ڶ��ζ�ȡ��Ч.��λ.
	PixData <<= 8;
	PixData |= LCD12864_ReadData(); // �����ζ�ȡ.��λ.

	if (Color == 0) // ָ��Ϊ���ĳһλ
		PixData = X_Bits & (~PixData);
	else // ָ��Ϊ��ʾĳһλ
		PixData = X_Bits | PixData;

	LCD12864_WriteCommand(Y_Byte);
	LCD12864_WriteCommand(X_Byte);

	LCD12864_WriteData(PixData / 256);
	LCD12864_WriteData(PixData % 256);

	LCD12864_WriteCommand(0x36); // ������ͼ����
	LCD12864_DelayUS(50);		 // �����������ʱ
}

/********************************************************************************
��  ��:	u8 LCD12864_PixGet(u8 x,u8 y)
��  ��:	��ȡָ�������ɫֵ.
��  ��:	0--��ǰ��û����ʾ;1-��ǰ������ʾ.

��  ��: ZCL  ZYX
��  ��: 2013-09-09
********************************************************************************/
u8 LCD12864_GetPixelIndex(u8 x, u8 y)
{
	u8 X_Byte;			 // X����
	unsigned int X_Bits; // ��ǵ�ǰ�㴦��x�����ֽ��е���һλ.��Ӧ��λ�ᱻ��1.
	u8 Y_Byte;			 // Y����
	unsigned int PixData;

	// 	LCD12864_WriteCommand(0x34);	    //8Bit���ƽӿ�; ����ָ�

	//--------------------------------------------------------------------------------
	// �������°�����
	if (y >= 32) //
	{
		Y_Byte = 0x80 + (y - 32); // ��Ч��:Y_Byte = 0x80+y%32
		X_Byte = 0x88 + x / 16;
		X_Bits = 0x8000 >> (x % 16); // �������ǰ��x���������ֽ��е���һλ
	}
	//--------------------------------------------------------------------------------
	// �������ϰ�����
	else //
	{
		Y_Byte = 0x80 + y; // ��Ч��:Y_Byte = 0x80+y%32
		X_Byte = 0x80 + x / 16;
		X_Bits = 0x8000 >> (x % 16); // �������ǰ��x���������ֽ��е���һλ
	}

	//--------------------------------------------------------------------------------
	// ��ȡ���ݲ�����,�����LCD��ʾ
	LCD12864_WriteCommand(Y_Byte); // ���ݸ�ʽ�涨��д��Y��ַ
	LCD12864_WriteCommand(X_Byte); // ��д��X��ַ

	LCD12864_ReadData();		   // ��һ�ζ�ȡ������Ч
	PixData = LCD12864_ReadData(); // �ڶ��ζ�ȡ��Ч.��λ.
	PixData <<= 8;
	PixData |= LCD12864_ReadData(); // �����ζ�ȡ.��λ.

	if (PixData & X_Bits)
		return 1; //
	else
		return 0; //
}

/********************************************************************************
��  ��: void LCD12864_TextMode(void)
��  ��:
��  ��:
��  ��:	��


��  ��: ZCL  ZYX
��  ��: 2013-09-09
********************************************************************************/
void LCD12864_TextMode(void)
{
	LCD12864_WriteCommand(0x30); // ����ָ�
	LCD12864_DelayUS(1000);

	LCD12864_WriteCommand(0x06); // �����ƶ�����
	LCD12864_DelayUS(1000);

	LCD12864_WriteCommand(0x0C); // ����ʾ,�ع��,���λ�ò�������˸
	LCD12864_DelayUS(1000);
}

/********************************************************************************
��  ��: void LCD12864_GraphMode(void)
��  ��:
��  ��:
��  ��:	��


��  ��: ZCL  ZYX
��  ��: 2013-09-09
********************************************************************************/
void LCD12864_GraphMode(void)
{
	LCD12864_WriteCommand(0x30); // ����ָ�
	LCD12864_DelayUS(1000);

	LCD12864_WriteCommand(0x06); // �����ƶ�����
	LCD12864_DelayUS(1000);

	LCD12864_WriteCommand(0x0C); // ����ʾ,���α�
	LCD12864_DelayUS(1000);

	LCD12864_WriteCommand(0x34); // ��չָ�,8Bit���ƽӿ�
	LCD12864_DelayUS(1000);
}

/********************************************************************************
��  ��: void LCD12864_ClrText(void)
��  ��:
��  ��:
��  ��:	��


��  ��: ZCL  ZYX
��  ��: 2013-09-09
********************************************************************************/
void LCD12864_ClrText(void)
{
	LCD12864_WriteCommand(0x30); // ����ָ�
	LCD12864_DelayUS(1000);

	LCD12864_WriteCommand(0x01); // ������Ļ
	LCD_DLY_ms(3);
}

/********************************************************************************
��  ��:	void LCD12864_Char(unsigned int Add, char CH)
��  ��:
��  ��:	��
		��Һ���ƶ�λ����ʾһ���ַ�.(char����,���Ǻ���)

��  ��: ZCL  ZYX
��  ��: 2013-09-09
********************************************************************************/
void LCD12864_Char(unsigned int Add, char CH)
{
	LCD12864_WriteCommand(0x30); // ����ָ�

	LCD12864_WriteCommand(Add);
	LCD12864_WriteData(CH);
}

/********************************************************************************
��  ��: void LCD12864_String(unsigned int Address, const char *Str)
��  ��:	��Һ������ʾ�ַ����ĺ���.
��  ��:	Address  --�ַ�����ʾ��ͷ��ַ
				*Str     --�ַ����׵�ַ
��  ��:	��
		��Һ���ƶ�λ����ʾһ���ַ�.(char����,���Ǻ���)

��  ��: ZCL  ZYX
��  ��: 2013-09-09
********************************************************************************/
void LCD12864_String(unsigned int Address, const char *Str)
{
	u8 i;

	LCD12864_WriteCommand(0x30); // ����ָ�

	LCD12864_WriteCommand(Address);

	for (i = 0; Str[i] != '\0'; i++)
		LCD12864_WriteData(Str[i]);

	LCD12864_DelayUS(500);
}

/********************************************************************************
��  ��:	void LCD12864_Num(unsigned int Address, long Num)
��  ��: ��Һ�����ض���ַ�������.
��  ��: Address--Ҫ��ʾ�����ֵ�λ��.
				Num------Ҫ��ʾ������.
��  ��:	��

��  ��: ZCL  ZYX
��  ��: 2013-09-09
********************************************************************************/
// ZCL 2019.4.4 ����ʹ��ʱ��Ҫ�ӵ���ʱ��������ֹ������ص��� ��1MS��ʱ OK
void LCD12864_Num(unsigned int Address, long Num)
{
	int BitLength = 0; // Ҫ��ʾ�����ֵ�λ��
	int Wei[32];	   // Ҫ��ʾ������ÿһλ
	long NumTmp;
	int i;

	LCD12864_WriteCommand(0x30); // ����ָ�

	if (Num >= 0)
		NumTmp = Num; //
	else
		NumTmp = -Num;

	if (Num != 0) //
	{
		for (i = 0; i < 10; i++) // ���磺��һ���������зֽ⣺��5181,�ֳ�:5,1,8,1 BitLength=4
		{
			Wei[i] = NumTmp % 10;
			NumTmp /= 10;
			BitLength++;

			if (NumTmp == 0)
				break;
		}

		BitLength--; // BitLength ��һ����Ϊλ����0��ʼ��

		LCD12864_WriteCommand(Address);

		if (Num < 0) // ����Ǹ�������д���š�
		{
			LCD12864_WriteData('-');
		}
		// ZCL 2015.8.30 �����˼��ȡ���ֵ�ASCII�룬�ͳ���ʾ����OK
		for (i = BitLength; i >= 0; i--)
		{
			LCD12864_WriteData(Wei[i] + '0');
		}
	}
	// =0��ֱ����ָ���ַд'0'		ZCL
	else if (Num == 0)
	{
		LCD12864_WriteCommand(Address);
		LCD12864_WriteData('0');
	}
}

/********************************************************************************
��  ��:void LCD12864_Num2(unsigned int Address, char myLen, long Num)
��  ��: ��Һ�����ض���ַ�������.
��  ��: Address--Ҫ��ʾ�����ֵ�λ��.
				Num------Ҫ��ʾ������.
��  ��:	��

��  ��: ZCL  ZYX
��  ��: 2013-09-09
********************************************************************************/
// ������ú�������ʱ������ ������ʾ���ҡ� ZCL 2015.8.31
void LCD12864_Num2(unsigned int Address, char myLen, long Num)
{
	//	int BitLength=0;  			//Ҫ��ʾ�����ֵ�λ��
	int Wei[32]; // Ҫ��ʾ������ÿһλ
	long NumTmp;
	int i;

	if (myLen == 0)
		return; // ���ݳ�����0.

	for (i = 0; i < 32; i++)
		Wei[i] = 0; //

	LCD12864_WriteCommand(0x30); // ����ָ�

	if (Num >= 0)
		NumTmp = Num; //
	else
		NumTmp = -Num;

	if (Num != 0) //
	{
		for (i = 0; i < 10; i++)
		{
			Wei[i] = NumTmp % 10;
			NumTmp /= 10;

			if (NumTmp == 0)
				break;
		}

		LCD12864_WriteCommand(Address);

		if (Num < 0) // ����Ǹ�������д���š�
		{
			LCD12864_WriteData('-');
		}
		// ZCL 2015.8.30 �����˼��ȡ���ֵ�ASCII�룬�ͳ���ʾ����OK
		for (i = myLen - 1; i >= 0; i--)
		{
			LCD12864_WriteData(Wei[i] + '0');
		}
	}
	else if (Num == 0) // Ҫ��ʾ��������0.
	{
		LCD12864_WriteCommand(Address); // ��ַ

		for (i = 0; i < myLen; i++) // ����ָ����ʾ����0 ZCL
		{
			LCD12864_WriteData('0');
		}
	}
}

/********************************************************************************
��  ��:void LCD12864_Num3(unsigned int Address, char AllLen, u8 XiaoShuBits, long Num, u8 mod)
��  ��: ��Һ�����ض���ַ�������.
��  ��: Address--Ҫ��ʾ�����ֵ�λ��.
				Num------Ҫ��ʾ������.
��  ��:	��

��  ��: ZCL
��  ��: 2015
********************************************************************************/
// ������ú�������ʱ������ ������ʾ���ҡ� ZCL 2015.8.31
void LCD12864_Num3(unsigned int Address, char AllLen, u8 XiaoShuBits, long Num, u8 mod)
{
	//	int BitLength=0;  			//Ҫ��ʾ�����ֵ�λ��
	int Wei[32]; // Ҫ��ʾ������ÿһλ
	long NumTmp;
	int i;
	// char m;
	char HighBit = 0;

	u8 nB_Sign; // ZCL 2019.10.18 ���ű�־

	if (mod >= 10)
	{
		mod = mod - 10;
		nB_Sign = 1;
	}
	else
		nB_Sign = 0;
	// ZCL 2019.10.18 ���ű�־
	if (AllLen == 0)
		return; // ���ݳ�����0.

	for (i = 0; i < 32; i++)
		Wei[i] = 0; //

	LCD12864_WriteCommand(0x30); // ����ָ�

	if (Num >= 0)
		NumTmp = Num; //
	else
		NumTmp = -Num;

	// if(Num != 0) 							//
	if (1) //
	{
		for (i = 0; i < 10; i++)
		{
			Wei[i] = NumTmp % 10;
			NumTmp /= 10;

			if (NumTmp == 0)
				break;
		}

		/* 		if(S_ModParStatus	)
				{
					LCD12864_WriteCommand(0x0f);	//����ʾ,���α�
				} */

		if (Pw_EquipmentType == 0) // ˫����
		{

			// ZCL Lw_PageNo==5��ʱ����ʾ���棬������ʾ
			if (Lw_PageNo == TIMEPAGE || Lw_PageNo == DSP_TIMEPAGE) // ʱ��ҳ
			{
				LCD12864_WriteCommand(Address - 1);
				if (mod)
				{
					LCD12864_WriteData(' ');
					LCD12864_WriteData('>');
				}
				else
				{
					LCD12864_WriteData(' ');
					LCD12864_WriteData(' ');
				}
			}
			else if (Lw_PageNo >= FAULTPAGE1 && Lw_PageNo <= FAULTPAGE1 + 6) // ZCL 2018.9.21
			{
				if (Lw_PageNo == FAULTPAGE1 + 6 && S_ModParStatus > 0 && F_ModPar1)
				{
					LCD12864_WriteCommand(Address - 1);
					LCD12864_String(Address - 1, " >");
				}
				else if (Lw_PageNo == FAULTPAGE1 + 6 && F_ModPar1)
				{
					LCD12864_WriteCommand(Address - 1);
					LCD12864_String(Address - 1, "  ");
				}
				else
					LCD12864_WriteCommand(Address); // Address--Ҫ��ʾ�����ֵ�λ��.
			}

			// ZCL 2017.6.5 DCM100��Ƶģʽ DSP��Ƶ���ϵ�ҳ��
			else if (Lw_PageNo >= DSP_FAULTPAGE1 && Lw_PageNo <= DSP_FAULTPAGE1 + 6) // ZCL 2018.9.21
			{
				if (Lw_PageNo == DSP_FAULTPAGE1 + 6 && S_ModParStatus > 0 && F_ModPar1) // ZCL 2018.9.21
				{
					LCD12864_WriteCommand(Address - 1);
					LCD12864_String(Address - 1, " >");
				}
				else if (Lw_PageNo == DSP_FAULTPAGE1 + 6 && F_ModPar1) // ZCL 2018.9.21
				{
					LCD12864_WriteCommand(Address - 1);
					LCD12864_String(Address - 1, "  ");
				}
				else
					LCD12864_WriteCommand(Address);
			}

			// ZCL 2019.3.23 DCM220��Ƶģʽ DSP��Ƶ���ϵ�ҳ��
			else if (Lw_PageNo >= DSP_FAULTPAGE2 && Lw_PageNo <= DSP_FAULTPAGE2 + 6) // ZCL 2018.9.21
			{
				if (Lw_PageNo == DSP_FAULTPAGE2 + 6 && S_ModParStatus > 0 && F_ModPar1) // ZCL 2018.9.21
				{
					LCD12864_WriteCommand(Address - 1);
					LCD12864_String(Address - 1, " >");
				}
				else if (Lw_PageNo == DSP_FAULTPAGE2 + 6 && F_ModPar1) // ZCL 2018.9.21
				{
					LCD12864_WriteCommand(Address - 1);
					LCD12864_String(Address - 1, "  ");
				}
				else
					LCD12864_WriteCommand(Address);
			}

			// ZCL 2018.10.28  γ�Ⱥ;��Ȼ���		LatLonPage
			else if (Lw_PageNo == LatLonPage_SQB || Lw_PageNo == LatLonPage2_SQB) // ZCL 2018.10.28
			{
				LCD12864_WriteCommand(Address);
				// if(mod)
				// LCD12864_WriteData('>');
				// else
				// LCD12864_WriteData(':');
			}

			// ZCL 2018.8.21 �Ի�����ʾ���ݽ�������  ˫ˮԴ   һ������2������������ռһ��
			else if (Lw_PageNo >= TWO_WATER_SOURCE && Lw_PageNo <= TWO_WATER_SOURCE + 7)
			{
				LCD12864_WriteCommand(Address);
				if (mod)
					LCD12864_WriteData('>');
				else
					LCD12864_WriteData(' ');
			}

			// ZCL 2019.4.4 DTU ID ҳ��
			else if (Lw_PageNo == DTUIDPage_SQB)
			{
				LCD12864_WriteCommand(Address);
			}

			else
			{
				LCD12864_WriteCommand(Address);
				if (mod)
					LCD12864_WriteData('>');
				// ZCL 2019.10.18 ��ʾ������
				else if (nB_Sign == 1)
					LCD12864_WriteData('-');
				else
					LCD12864_WriteData(':');
			}

			if (Num < 0) // ����Ǹ�������д���š�
			{
				LCD12864_WriteData('-');
			}

			// ZCL 2015.8.30 �����˼��ȡ���ֵ�ASCII�룬�ͳ���ʾ����OK
			// ��������
			if (AllLen < XiaoShuBits)
				AllLen = XiaoShuBits;

			// ��ʾ����==С��λ��ʱ����ʾ  �磺0.300MPa
			if (AllLen == XiaoShuBits)
			{
				LCD12864_WriteData('0');
				LCD12864_WriteData('.');
				for (i = AllLen - 1; i >= 0; i--)
				{
					LCD12864_WriteData(Wei[i] + '0');
				}
			}
			// ��ʾ����>С��λ��ʱ����ʾ	  �磺1.60MPa
			// �ַ����������XiaoShuBits==0  XiaoShuBits>0
			else if (XiaoShuBits == 0)
			{
				B_DispZero = 0;
				for (i = AllLen - 1; i >= 0; i--)
				{
					if (Wei[i] != 0 || i == 0)
						B_DispZero = 1;
					if (B_DispZero)
						LCD12864_WriteData(Wei[i] + '0');
					else
					{
						if (Lw_PageNo == DTUIDPage_SQB)
						{
							; // ZCL 2019.4.4 DTU ID��ʱ����ʾ0
						}
						else
							LCD12864_WriteData(' '); // ����ʾ��
					}
				}
			}

			else if (XiaoShuBits > 0)
			{
				if (AllLen - XiaoShuBits > 1)
				{
					for (i = AllLen - 1; i >= XiaoShuBits; i--)
					{
						if (Wei[i] == 0 && HighBit == 0 && i + 1 - XiaoShuBits > 1)
							LCD12864_WriteData(' '); // ����ʾ��
						else
						{
							HighBit = 1;
							LCD12864_WriteData(Wei[i] + '0');
						}
					}
				}
				else
					LCD12864_WriteData(Wei[AllLen - 1] + '0');

				LCD12864_WriteData('.');
				for (i = XiaoShuBits - 1; i >= 0; i--)
				{
					LCD12864_WriteData(Wei[i] + '0');
				}
			}
		}
		else // ��Ƶ���
		{

			// ZCL Lw_PageNo==5��ʱ����ʾ���棬������ʾ
			if (Lw_PageNo == TIMEPAGE || Lw_PageNo == DSP_TIMEPAGE) // ʱ��ҳ
			{
				LCD12864_WriteCommand(Address - 1);
				if (mod)
				{
					LCD12864_WriteData(' ');
					LCD12864_WriteData('>');
				}
				else
				{
					LCD12864_WriteData(' ');
					LCD12864_WriteData(' ');
				}
			}
			else if (Lw_PageNo >= FAULTPAGE1 && Lw_PageNo <= FAULTPAGE1 + 6) // ZCL 2018.9.21
			{
				if (Lw_PageNo == FAULTPAGE1 + 6 && S_ModParStatus > 0 && F_ModPar1)
				{
					LCD12864_WriteCommand(Address - 1);
					LCD12864_String(Address - 1, " >");
				}
				else if (Lw_PageNo == FAULTPAGE1 + 6 && F_ModPar1)
				{
					LCD12864_WriteCommand(Address - 1);
					LCD12864_String(Address - 1, "  ");
				}
				else
					LCD12864_WriteCommand(Address); // Address--Ҫ��ʾ�����ֵ�λ��.
			}

			// ZCL 2017.6.5 DCM100��Ƶģʽ DSP��Ƶ���ϵ�ҳ��
			else if (Lw_PageNo >= DSP_FAULTPAGE1 && Lw_PageNo <= DSP_FAULTPAGE1 + 6) // ZCL 2018.9.21
			{
				if (Lw_PageNo == DSP_FAULTPAGE1 + 6 && S_ModParStatus > 0 && F_ModPar1) // ZCL 2018.9.21
				{
					LCD12864_WriteCommand(Address - 1);
					LCD12864_String(Address - 1, " >");
				}
				else if (Lw_PageNo == DSP_FAULTPAGE1 + 6 && F_ModPar1) // ZCL 2018.9.21
				{
					LCD12864_WriteCommand(Address - 1);
					LCD12864_String(Address - 1, "  ");
				}
				else
					LCD12864_WriteCommand(Address);
			}

			// ZCL 2019.3.23 DCM220��Ƶģʽ DSP��Ƶ���ϵ�ҳ��
			else if (Lw_PageNo >= DSP_FAULTPAGE2 && Lw_PageNo <= DSP_FAULTPAGE2 + 6) // ZCL 2018.9.21
			{
				if (Lw_PageNo == DSP_FAULTPAGE2 + 6 && S_ModParStatus > 0 && F_ModPar1) // ZCL 2018.9.21
				{
					LCD12864_WriteCommand(Address - 1);
					LCD12864_String(Address - 1, " >");
				}
				else if (Lw_PageNo == DSP_FAULTPAGE2 + 6 && F_ModPar1) // ZCL 2018.9.21
				{
					LCD12864_WriteCommand(Address - 1);
					LCD12864_String(Address - 1, "  ");
				}
				else
					LCD12864_WriteCommand(Address);
			}

			// ZCL 2018.10.28  γ�Ⱥ;��Ȼ���		LatLonPage
			else if (Lw_PageNo == LatLonPage_BPDJ || Lw_PageNo == LatLonPage2_BPDJ) // ZCL 2018.10.28
			{
				LCD12864_WriteCommand(Address);
				// if(mod)
				// LCD12864_WriteData('>');
				// else
				// LCD12864_WriteData(':');
			}

			// ZCL 2018.8.21 �Ի�����ʾ���ݽ�������  ˫ˮԴ   һ������2������������ռһ��
			else if (Lw_PageNo >= TWO_WATER_SOURCE && Lw_PageNo <= TWO_WATER_SOURCE + 7)
			{
				LCD12864_WriteCommand(Address);
				if (mod)
					LCD12864_WriteData('>');
				else
					LCD12864_WriteData(' ');
			}

			// ZCL 2019.4.4 DTU ID ҳ��
			else if (Lw_PageNo == DTUIDPage_BPDJ)
			{
				LCD12864_WriteCommand(Address);
			}

			else
			{
				LCD12864_WriteCommand(Address);
				if (mod)
					LCD12864_WriteData('>');
				// ZCL 2019.10.18 ��ʾ������
				else if (nB_Sign == 1)
					LCD12864_WriteData('-');
				else
					LCD12864_WriteData(':');
			}

			if (Num < 0) // ����Ǹ�������д���š�
			{
				LCD12864_WriteData('-');
			}

			// ZCL 2015.8.30 �����˼��ȡ���ֵ�ASCII�룬�ͳ���ʾ����OK
			// ��������
			if (AllLen < XiaoShuBits)
				AllLen = XiaoShuBits;

			// ��ʾ����==С��λ��ʱ����ʾ  �磺0.300MPa
			if (AllLen == XiaoShuBits)
			{
				LCD12864_WriteData('0');
				LCD12864_WriteData('.');
				for (i = AllLen - 1; i >= 0; i--)
				{
					LCD12864_WriteData(Wei[i] + '0');
				}
			}
			// ��ʾ����>С��λ��ʱ����ʾ	  �磺1.60MPa
			// �ַ����������XiaoShuBits==0  XiaoShuBits>0
			else if (XiaoShuBits == 0)
			{
				B_DispZero = 0;
				for (i = AllLen - 1; i >= 0; i--)
				{
					if (Wei[i] != 0 || i == 0)
						B_DispZero = 1;
					if (B_DispZero)
						LCD12864_WriteData(Wei[i] + '0');
					else
					{
						if (Lw_PageNo == DTUIDPage_BPDJ)
						{
							; // ZCL 2019.4.4 DTU ID��ʱ����ʾ0
						}
						else
							LCD12864_WriteData(' '); // ����ʾ��
					}
				}
			}

			else if (XiaoShuBits > 0)
			{
				if (AllLen - XiaoShuBits > 1)
				{
					for (i = AllLen - 1; i >= XiaoShuBits; i--)
					{
						if (Wei[i] == 0 && HighBit == 0 && i + 1 - XiaoShuBits > 1)
							LCD12864_WriteData(' '); // ����ʾ��
						else
						{
							HighBit = 1;
							LCD12864_WriteData(Wei[i] + '0');
						}
					}
				}
				else
					LCD12864_WriteData(Wei[AllLen - 1] + '0');

				LCD12864_WriteData('.');
				for (i = XiaoShuBits - 1; i >= 0; i--)
				{
					LCD12864_WriteData(Wei[i] + '0');
				}
			}
		}
	}
}

void LCD_DLY_ms(u32 Time)
{
	u32 tick;
	tick = sysTickValue;
	/* Wait for specified Time */
	while ((u32)(sysTickValue - tick) < Time)
		;
}

/***************************************************************************/
