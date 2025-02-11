/**
  ******************************************************************************
  * @file    DoWith.h
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.27
  * @date    2014-01-03
  * @brief   �����������⣬���������,ģ���������⣬ģ�������,������������
	******************************************************************************
	*/

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __DOWITH_H
#define __DOWITH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Variable_Init(void);			// ������ʼ��
void ParLst_Init (void);			// RAM�в������� ��ʼ�� (����)
void ParArrayWrite(u16 *p_Top,u16 *p_Base, uint w_WriteSize);
void ParArrayRead(uint *p_Top,uc16 *p_Base, uint w_ReadSize);
void GprsArrayRead(uchar *p_Top,uc8 *p_Base, uint w_ReadSize);		//ZCL 2019.4.4 uc16
void Boot_ParLst (void);			// ��ʼ���趨����
void SavePar_Prompt (void);		// �������+״̬��ʾ
void ForceSavePar(void);			// ǿ�Ʊ������
void DoWith(void);						// һЩ����,��¼�Ĵ���
void ParLimit(void);					// ��������
void DealPlayVoice(void);			// ���� ѭ������
void SPI_FLASH_Record (void);	// SPI-FLASH ��¼����
void Time_Output(void);				// ���ʱ�����	 2008.10.21
void GetLockCode(void);				// 2013.10.26 �õ���������
void Formula_12(u8 *D,u8 *Result);
void Formula_113(u8 *D,u8 *Result);

void Test(void); 		//��� 
void	ADC_DMA_Init(void); 

u32 FtoU32(u16 w1,u16 w2); 						//���ֽڵĸ�������ת��Ϊ����10�ĳ���������
#endif /* __DOWITH_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/


