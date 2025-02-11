/**
  ******************************************************************************
  * @file    LCD.h
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.28
  * @date    2014-01-03
  * @brief   Code  for UG-2864KSWLG01 WiseChip ���ӽӿڣ�IC:SSD1306
  * 				 Hardware	: STM32F103(101) C8T6,CBT6
	* 				 Platform	: IAR6.40,Keil-MDK5.01,LIB:V3.5.0(11/03/2011),JLINK V6-V7	
	******************************************************************************
	*/
	
#ifndef __LCD_H__
#define __LCD_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LCD_DLY_ms(u32 time);
void LCD_WrDat(unsigned char dat);																//LCDд����
void LCD_WrCmd(unsigned char cmd);																//LCDд����
void LCD_Set_Pos(unsigned char x, unsigned char y);								//LCD ��������
void LCD_Fill(unsigned char bmp_dat);															//LCDȫ��
void LCD_CLS(void);																								//LCD��λ
void LCD_Init(void);																							//LCD��ʼ��
void LCD_P6x8Str(u8 x, u8 y, u8 ch[]);					//������������ʾ6*8һ���׼ASCII�ַ���	��ʾ�����꣨x,y����yΪҳ��Χ0��7
void LCD_P8x16Str(u8 x, u8 y, u8 ch[]);				//������������ʾ8*16һ���׼ASCII�ַ���	 ��ʾ�����꣨x,y����yΪҳ��Χ0��7
void LCD_P16x16Ch(u8 x, u8 y, u8 N);													//������������ʾ16*16����  ��ʾ�����꣨x,y����yΪҳ��Χ0��7
void Draw_BMP(u8 x0, u8 y0, u8 x1, u8 y1, uc8 BMP[]);	//������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7

void Oled_display(uc8 picture[],u8 startpage,u8 endpage);		//��ָ����ʼҳ������ҳ
void Clear_screen(void);            //����Ļ 128*64
void Clear_page(u8 page); 	  			//��ָ��ҳ����
void Zhenkongdu_SelfCheck(void);		//��նȼ���ʱ+��ֵ��ʾ  Ч��ͼ���ļ�
void Show_cat(uc8 cat[]);     		  //��cat_offon����
void Cat_offon(void);								//��ʾè�أ�è��(����ͼ��è��)
void Setcontrast(void);							//���öԱȶ� ����
void Setcontrast2(void);						//���öԱȶ� ����

//p:��*ҳ  ��ָ��ҳ.ָ����дָ������ 16*16���� 32�ֽ�
void HZ_1616(u8 p,u8 h,u8 l,u8 a);  //16*16����
//p:��*ҳ  ��ָ��ҳ.ָ����дָ�����ȵ����飨����16������Ҫд2ҳ��ÿҳ�ĳ���Ϊ���鳤��/2��
void HZ_16x(u8 p,u8 h,u8 l,uc8 *pic,u16 Arraylength);   //write ���ⳤ�ȵ��ַ�ͼ�Σ���ָ��ҳ��λ�ã�
//���ƶ�ҳд.ָ����д 24*24���� 24*24���� 72�ֽ�
void HZ_2424(u8 p,u8 h,u8 l,u8 a);    //24*24����    �Ѿ����� p����<=5
//���ƶ�ҳд.ָ����д 24*24���� 24*24���� 72�ֽ�  ����ɨ����д
void HZ_2424Laser(u8 p,u8 h,u8 l,u8 a);    //24*24����    δ���� p����<=5
//p:��*ҳ  ��ָ��ҳ.ָ����дָ������ 16*7���� 14�ֽ�
void Num_1607(u8 p,u8 h,u8 l,u8 a);   //write ����0123456789>+ 16*7���� 14�ֽ�
//p:��*ҳ  ��ָ��ҳ.ָ����дָ����ĸ 16*8���� 16�ֽ�
void Num_1608(u8 p,u8 h,u8 l,u8 a);   //write ��ĸKPa     16*8���� 16�ֽ�
/*write word p-page h-column high l-column lower a-word*/
//w_word p:��*ҳ  ��ָ��ҳ.ָ����дָ�������� 16*32���� 64�ֽ�
void Num_1632(u8 p,u8 h,u8 l,u8 a);   


//LCD12864Һ����������
//LCD_RS	 --	  PB11
//LCD_RW	 --	  PB12
//LCD_En	 --	  PB13
//LCD_PSB	 --	  PA12
//LCD_RST	 --	  PA13

#define  LCD12864_RS_H			GPIO_SetBits(GPIOB, GPIO_Pin_9)		//����ָ����� RS  GPIO_Pin_11  ZCL 2018.10.11
#define  LCD12864_RS_L			GPIO_ResetBits(GPIOB, GPIO_Pin_9)		//

#define  LCD12864_RW_H			GPIO_SetBits(GPIOC, GPIO_Pin_4)		//��д���� RW  ZCL 2018.12.3
#define  LCD12864_RW_L			GPIO_ResetBits(GPIOC, GPIO_Pin_4)		//

#define  LCD12864_EN_H			GPIO_SetBits(GPIOA, GPIO_Pin_7)		//ʹ������ EN ZCL 2018.11.30
#define  LCD12864_EN_L			GPIO_ResetBits(GPIOA, GPIO_Pin_7)		//

/* 2015.3.28 LCD_RST�ĳ�PA15��PA13�ó�SWDIO �ܳ��� */
// #ifdef SWD
#define  LCD12864_RST_H			GPIO_SetBits(GPIOC, GPIO_Pin_2)		//��λ���� RST	GPIO_Pin_15  ZCL 2018.10.11
#define  LCD12864_RST_L			GPIO_ResetBits(GPIOC, GPIO_Pin_2)		//
// #else
// #define  LCD12864_RST_H			GPIO_SetBits(GPIOA, GPIO_Pin_13)		//��λ���� RST
// #define  LCD12864_RST_L			GPIO_ResetBits(GPIOA, GPIO_Pin_13)		//	
// #endif

#define  LCD12864_PSB_H			GPIO_SetBits(GPIOA, GPIO_Pin_12)		//���в���ѡ�� PSB
#define  LCD12864_PSB_L			GPIO_ResetBits(GPIOA, GPIO_Pin_12)		//

#define  LCD12864_DATA0_H		GPIO_SetBits(GPIOB, GPIO_Pin_14)		//
#define  LCD12864_DATA0_L		GPIO_ResetBits(GPIOB, GPIO_Pin_14)		//

#define  LCD12864_DATA1_H		GPIO_SetBits(GPIOB, GPIO_Pin_15)		//
#define  LCD12864_DATA1_L		GPIO_ResetBits(GPIOB, GPIO_Pin_15)		//

#define  LCD12864_DATA2_H		GPIO_SetBits(GPIOC, GPIO_Pin_6)			//
#define  LCD12864_DATA2_L		GPIO_ResetBits(GPIOC, GPIO_Pin_6)		//

#define  LCD12864_DATA3_H		GPIO_SetBits(GPIOC, GPIO_Pin_7)			//
#define  LCD12864_DATA3_L		GPIO_ResetBits(GPIOC, GPIO_Pin_7)		//

#define  LCD12864_DATA4_H		GPIO_SetBits(GPIOC, GPIO_Pin_8)			//
#define  LCD12864_DATA4_L		GPIO_ResetBits(GPIOC, GPIO_Pin_8)		//

#define  LCD12864_DATA5_H		GPIO_SetBits(GPIOC, GPIO_Pin_9)			//
#define  LCD12864_DATA5_L		GPIO_ResetBits(GPIOC, GPIO_Pin_9)		//

#define  LCD12864_DATA6_H		GPIO_SetBits(GPIOA, GPIO_Pin_8)			//
#define  LCD12864_DATA6_L 		GPIO_ResetBits(GPIOA, GPIO_Pin_8)		//

#define  LCD12864_DATA7_H		GPIO_SetBits(GPIOA, GPIO_Pin_15)		//ZCL 2018.11.30
#define  LCD12864_DATA7_L		GPIO_ResetBits(GPIOA, GPIO_Pin_15)		//



//�����߶�ȡ����
//LCD_DATA0	 --	  PB14
//LCD_DATA1	 --	  PB15
//LCD_DATA2	 --	  PC6
//LCD_DATA3	 --	  PC7
//LCD_DATA4	 --	  PC8
//LCD_DATA5	 --	  PC9
//LCD_DATA6	 --	  PA8
//LCD_DATA7	 --	  PA11-->PA15 //ZCL 2018.11.30

#define  LCD12864_RDATA0		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)	//
#define  LCD12864_RDATA1		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)	//
#define  LCD12864_RDATA2		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)	//
#define  LCD12864_RDATA3		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)	//
#define  LCD12864_RDATA4		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)	//
#define  LCD12864_RDATA5		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)	//
#define  LCD12864_RDATA6		GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)	//
#define  LCD12864_RDATA7		GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)	//ZCL 2018.11.30




void LCD12864_DelayUS(unsigned int Dly);											 //

void LCD12864_WriteOneByte(unsigned char Data);										 //
unsigned char LCD12864_ReadOneByte(void);											 //

unsigned char LCD12864_CheckBusy(void);												 //

void LCD12864_WriteCommand(unsigned char myCMD);									 //
void LCD12864_WriteData(unsigned char myData);										 //

unsigned char LCD12864_ReadData(void);												 //
void LCD12864_Clear(void);															 //

void LCD12864_SetPixelIndex(unsigned char x, unsigned char y, unsigned char Color);	 //
unsigned char LCD12864_GetPixelIndex(unsigned char x, unsigned char y);				 //

void LCD12864_Init(void);															 //

void LCD12864_Cursor(unsigned char myDIR);											 //

void LCD12864_TextMode(void);														 //
void LCD12864_GraphMode(void);														 //
void LCD12864_ClrText(void);														 //

void LCD12864_Char(unsigned int Add, char CH);										 //
void LCD12864_String(unsigned int Address, const char *Str);							 //
void LCD12864_Num(unsigned int Address, long Num);									 //
void LCD12864_Num2(unsigned int Address, char myLen, long Num);
//void LCD12864_Num3(unsigned int Address, char myLen, u8 XiaoShuBits, long Num); //ZCL
void LCD12864_Num3(unsigned int Address, char myLen, u8 XiaoShuBits, long Num, u8 mod);
#endif

/* This is the end line of LCD.h */
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
