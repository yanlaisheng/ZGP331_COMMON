/**
  ******************************************************************************
  * @file    Key_Menu.h
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.28
  * @date    2014-01-03
  * @brief   Code  for UG-2864KSWLG01 WiseChip ���ӽӿڣ�IC:SSD1306
  * 				 Hardware	: STM32F103(101) C8T6,CBT6
	* 				 Platform	: IAR6.40,Keil-MDK5.01,LIB:V3.5.0(11/03/2011),JLINK V6-V7	
	******************************************************************************
  * @attention	2014.1.3	
	******************************************************************************	
  * @note	2014.4.8
	�ܳ��� 2011.5.9
	1. Һ��RAM����LSB���ϣ�MSB����
	2. �����Ǿ�OLEDҺ��1.3��(UG-2864KSWLG01  WiseChip ���ӽӿ�)��ʾ�����ĵײ㺯��
	*/

#ifndef __Key_Menu_H__
#define __Key_Menu_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Menu_Demo(void) ;
void Menu_Welcome(void); 
void Menu_Dynamic_TXT(void); 
void Menu_Feature(void) ;
void Menu_LaserTxt(void); 
void Menu_ReadPara1(void);			//���� ������1
void Menu_ReadPara2(void);			//���� ������2
void Menu_ReadPara3(void);			//���� ������3
void Menu_SetPara1(void);				//���� �趨����1
void Menu_SetPara2(void);				//���� �趨����2
void Menu_SetPara3(void);				//���� �趨����3
void Menu_SetPara4(void);				//���� �趨����4
void Menu_SetPara5(void);				//���� �趨����5
void Menu_SetPara6(void);				//���� �趨����6
void Menu_SetPara7(void);				//���� �趨����7
void Menu_SetPara8(void);				//���� �趨����8
void GetKey(void);							//ZCL �õ������ļ�ֵ 2013.12.07
void Menu_Change(void);					//����˵��ı�
void ModParNum(u8 num);			//ָ��:�޸Ĳ���������
void FaultDisplay(u16 FaultValue , u8 Addr);		//������ʾ		����ֵ����ʾ��ַ
void DspFaultDisplay(u16 FaultValue , u8 Addr);		//ZCL 2018.5.18
void Menu_Delay(void);				//������ʱ

//p:ҳ  x:��������(h l ��������)(���λ)  w:Ҫ��ʾ�Ĳ���  length:������ʾ����
//void DispMod_Par(u8 p,u8 x,u16 *Address,u8 length,u8 f_modpar);
//void DispMod_Par(u8 x_pos,u8 length,u16 *Address, u8 ParType, u8 f_modpar);
void DispMod_Par(u8 x_pos,u8 length, u8 XiaoShuBits, u16 *Address, u8 ParType, u8 nb_modpar);

void Menu_Dsp1ReadPar1(void);			//���� DSP1������1		//ZCL 2019.3.8
void Menu_Dsp1ReadPar2(void);			//���� DSP1������2
void Menu_Dsp1ReadPar3(void);			//���� DSP1������3
void Menu_Dsp2ReadPar1(void);			//���� DSP2������1
void Menu_Dsp2ReadPar2(void);			//���� DSP2������2
void Menu_Dsp2ReadPar3(void);			//���� DSP2������3
void Menu_Dsp3ReadPar1(void);			//���� DSP3������1
void Menu_Dsp3ReadPar2(void);			//���� DSP3������2
void Menu_Dsp3ReadPar3(void);			//���� DSP3������3
void Menu_Dsp4ReadPar1(void);			//���� DSP4������1
void Menu_Dsp4ReadPar2(void);			//���� DSP4������2
void Menu_Dsp4ReadPar3(void);			//���� DSP4������3
void Menu_LoRaMasterSetPar1(void);	//���� LoRa���� DSP�趨����1 
void Menu_GpsReadPar1(void);		

void ReadBPDJPar(void);		//ZCL 2019.3.7 ����Ƶ������������
void ReadBPDJPar2(void);	//ZCL 2019.4.3
void DispMod_Par2_String(unsigned int Address, const char *Str);
//ZCL 2019.4.5
void DispMod_Par3_Num(unsigned int Address, long Num);

void DispMod_Par2(u8 x_pos,u8 length, u8 XiaoShuBits, u32 l_value, u8 ParType, u8 nb_modpar);		//ZCL 2022.4.23
#endif

/* This is the end line of LCD.h */
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
