/**
  ******************************************************************************
  * @file    com_LoRa.h
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.20
  * @date    2018-12-14
  * @brief   LoRa USE SPI3
  ******************************************************************************
  * @note	2015.04.13

  */

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __COM_LORA_H
#define __COM_LORA_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void LoRa_Init(void);
void LoRa_RcvProcess(void);
void LoRa_SlaveSend(void);  // ����0�ӻ�����
void LoRa_MasterSend(void); // ����0�����ͳ���

void LoRaSendTest(void);   // LoRa ���Ͳ���
void LoRaRcvTest(void);    // LoRa ���ղ���
void LoRaSendToDone(void); // LoRa ���ͣ�������Ƿ����

void Jude_VVVF_Comm(void); // ����Ƶ��ͨ�Ź���

#endif /* __COM_LORA_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
