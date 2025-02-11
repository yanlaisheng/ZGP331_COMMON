/**
  ******************************************************************************
  * @file    com_LoRa.h
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.10
  * @date    2015-04-13
  * @brief   Com1 USE USART2
	******************************************************************************
	*/

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __com_LoRa_H
#define __com_LoRa_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LoRa_Init(void);							//LoRa����Init
void LoRa(void);
void LoRaRcv(void);		// LoRa ����


#endif /* __COM2_485_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/


