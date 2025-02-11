/**
  ******************************************************************************
  * @file    com2_485.h
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.31
  * @date    2014-04-08
  * @brief   COM1 USE USART3
	******************************************************************************
	*/
	
#ifndef __COM2_232_H
#define __COM2_232_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Com2_Init(void);
void Com2_RcvProcess(void);
void Com2_SlaveSend(void);			// ����2�ӻ�����
void Com2_MasterSend(void);			// ����2�����ͳ���

#endif /* __COM1_232_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/


