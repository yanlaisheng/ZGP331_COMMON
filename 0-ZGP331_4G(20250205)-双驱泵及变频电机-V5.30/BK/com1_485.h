/**
  ******************************************************************************
  * @file    com1_485.h
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.27
  * @date    2014-01-03
  * @brief   COM1 USE USART1
	******************************************************************************
	*/
	
/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __COM1_485_H
#define __COM1_485_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Com1_Init(void);
void Com1_RcvProcess(void);
void Com1_SlaveSend(void);			// ����1�ӻ�����
void Com1_MasterSend(void);			// ����1�����ͳ���
void Com1_BaudRateSet(void);		// ����1���������ó���2013.1.17

#endif /* __COM1_232_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/


