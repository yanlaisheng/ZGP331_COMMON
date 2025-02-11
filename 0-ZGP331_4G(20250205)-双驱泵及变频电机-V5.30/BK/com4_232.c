/**
  ******************************************************************************
  * @file    com0_232.c
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.27
  * @date    2014-01-03
  * @brief   COM0 USE UART4
	******************************************************************************
	* @note	2014.12.3

		
	*/

/* Includes ------------------------------------------------------------------*/
#include "com4_232.h"
//#include "Gprs_at.h" ZCL123
#include "GlobalV_Extern.h"			// ȫ�ֱ�������
#include "GlobalConst.h"
#include "string.h"							//strstr�������ҳ�str2�ַ�����str1�ַ����е�һ�γ��ֵ�λ��
#include <stdio.h>							//���ϴ˾������printf

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uchar	Txd4Buffer[TXD4_MAX];		// ���ͻ�����
char	Rcv4Buffer[RCV4_MAX];		// ���ջ�����
uint	Rcv4Counter; 						// ���ռ�����//
uint	Txd4Counter;						// ���ͼ�����//
uint	BakRcv4Count; 					// ���ռ�����//
uint  Txd4Max;								// �ж��ٸ��ַ���Ҫ����//
uint	w_Txd4ChkSum; 					// ����У��ͣ�lo,hi ��λ//
uint	w_Uart4RegAddr;					// ����1�Ĵ�����ַ
//
uchar B_Uart4Cmd03;
uchar B_Uart4Cmd16;
uchar B_Uart4Cmd01;
uchar B_Uart4Cmd06;
uchar B_Uart4Cmd73;
uchar T_NoRcv4Count;					// û�н��ռ�����
uint	C_NoRcv4Count;



/* Private variables extern --------------------------------------------------*/



/* Private function prototypes -----------------------------------------------*/
void GPIO_Com4_Configuration(void);							//GPIO����
void Com4_config(void);
uint CRC16(uchar * pCrcData,uchar CrcDataLen);
void Delay_MS(vu16 nCount);
void Enable_Usart3_NVIC(void);
void Disable_Usart3_NVIC(void);

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : GPIO_Com4_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Com4_Configuration(void)							//GPIO���� 
{
  GPIO_InitTypeDef GPIO_InitStructure;					//����GPIO��ʼ���ṹ��
  
	//����˵����2�����ڷֱ���Usart1.Usart3(USART���±��1��ʼ,��STM32F103 ARM�Ĵ�������)
  //Com4 ʵ��Ϊ��һ������
  /* Configure 232  */  		// RS232û�п��ƶ�
  /* Configure Com4(ʹ��Ӳ��:UART4) Rx (PC.11) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  /* Configure Com4(ʹ��Ӳ��:UART4) Tx (PC.10) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_10);
}

/* UART4  configuration ------------------------------------------------------*/
/* USART  configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
				RTS/CTSЭ��(Request To Send/Clear To Send)��������/������Э��				
*/
void Com4_config(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_DeInit(UART4); //��λ Usart1
	USART_StructInit(&USART_InitStructure);	//��USART_InitStruct�е�ÿһ��������ȱʡֵ����	
	
	USART_InitStructure.USART_BaudRate = 9600;			//2.25M/BPS   RS232оƬ:230.4kbps
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Configure ����1-UART4 */
	USART_Init(UART4, &USART_InitStructure);  
  /* Enable UART4 Receive and Transmit interrupts */
	//USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
  /* Enable the UART4 */
  USART_Cmd(UART4, ENABLE);	

}

/*******************************************************************************
* Function Name  : Com4_Init
* Description    : Initializes the peripherals used by the Usart driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Com4_Init(void)
{
  GPIO_Com4_Configuration();
  Com4_config();
	//
	Txd4Counter=0;
	Rcv4Counter=0;
	//
	//Delay_MS(35);			ZCL123			//2013.6.22 ����VBAT�ϵ絽�ܽ�PWRKEY����֮���ʱ����Ϊ30MS	
}





/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
