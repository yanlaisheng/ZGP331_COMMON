/**
 ******************************************************************************
 * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
 * @author  MCD Application Team
 * @version V3.5.0
 * @date    08-April-2011
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "com1_232.h"
#include "com2_232.h"
#include "GlobalConst.h"
#include "GlobalV_Extern.h" // ȫ�ֱ�������

#include "sx12xxEiger.h" //ZCL 2018.11.13
/** @addtogroup STM32F10x_StdPeriph_Template
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern u8 SoftClock[9]; // ����ʱ��

extern u8 Txd1Buffer[];
extern u8 Rcv1Buffer[];
extern u16 Cw_Txd1;
extern u16 Cw_Rcv1;
extern u16 C_NoRcv1;
extern u16 Cw_Txd1Max;

extern u8 Txd2Buffer[];
extern u8 Rcv2Buffer[];
extern u16 Cw_Txd2;
extern u16 Cw_Rcv2;
extern u16 C_NoRcv2;
extern u16 Cw_Txd2Max;

extern u8 Txd3Buffer[];
extern u8 Rcv3Buffer[];
extern u16 Cw_Txd3;
extern u16 Cw_Rcv3;
extern u16 C_NoRcv3;
extern u16 Cw_Txd3Max;

extern u8 Txd4Buffer[];
extern u8 Rcv4Buffer[];
extern u16 Cw_Txd4;
extern u16 Cw_Rcv4;
extern u16 C_NoRcv4;
extern u16 Cw_Txd4Max;

extern u16 C_PowenOnDelay;
extern volatile u32 sysTickValue;
extern u32 w32_SecCouter; // 2014.4.5

// ZCL 2018.12.11
extern uint C_M35Step;
extern uint C_M35PowerOnOff;
extern uint C_M35SendSum;
extern uint C_DtuInquireSend; // DTU��ѯָ�������
extern uint C_DtuSetSend;     // DTU����ָ�������
extern uint C_GprsDataSend;   // Gprs���ݴ��������
extern uint C_ReadNewSms;     // ���µĶ��ż�����
extern uint C_SendSms;        // ���Ͷ��ż���������bΪ�ֽ���
extern uint C_SmsFull;        // ������������
// extern	uint	 C_ReLink;							//�����Ӽ�����
extern uint C_DelayRunGprsModule; // ����͸��������
extern uint C_ReceiveChar;
extern uint C_ReceiveChar_Com1;

extern uint C_HeartSendNow[];
extern uint C_HeartDelay[];
extern uint C_LinkDelay[];
extern uint C_LinkNow[];
extern uint C_LinkNow_DoName[];
extern uint C_SmsRunStop;

extern uint C_LedLight;              // LED��˸��ʱ������
extern uint C_BetweenDtuInquireSend; // ����ǰ��ʱ3MS���ղ���>��־��ʱ300MS�ط�
extern uint C_BetweenDtuSetSend;     // ����ǰ��ʱ3MS���ղ���>��־��ʱ300MS�ط�
extern uint C_BetweenSendDataReturn; // ����ǰ��ʱ3MS���ղ���>��־��ʱ300MS�ط�
extern uint C_BetweenHeartSendNow;   // ����ǰ��ʱ3MS���ղ���>��־��ʱ300MS�ط�
extern uint C_BetweenSendSms;        // ����ǰ��ʱ3MS���ղ���>��־��ʱ300MS�ط�
extern uint C_NetLedLightCSQ;        // Net LED��˸CSQָʾ������ ZCL 2017.5.25

extern u8 S_Display;             // ������ʾ����		ZCL 2019.4.26
extern uint Cw_BakRcv3;          // ���ռ�����//
extern u32 C_SendHeartbeatTime0; // ��������ʱ����ʱ����
extern u32 C_SendHeartbeatTime;  // ��������ʱ����ʱ����
/* Private function prototypes -----------------------------------------------*/
void IWDG_Feed(void); // �������Ź�ι��	 2017.6.19

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
  //  static uint16_t tim_cnt = 0;       // ��ʱ������
  //  static uint8_t heart_beat_cnt = 0; // ��������
  uchar i;

  SClk1Ms++; // ע��˾���Ҫ�Ƶ�ѭ���⣬����++�� 2012.9.10
  sysTickValue++;
  sysTickMsValue++;
  // ZCL 2018.12.11

  C_ReceiveChar++;
  C_ReceiveChar_Com1++;
  C_LedLight++;
  C_BetweenDtuInquireSend++; // ����ǰ��ʱ3MS���ղ���>��־��ʱ300MS�ط�
  C_BetweenDtuSetSend++;     // ����ǰ��ʱ3MS���ղ���>��־��ʱ300MS�ط�
  C_BetweenSendDataReturn++; // ����ǰ��ʱ3MS���ղ���>��־��ʱ300MS�ط�
  C_BetweenHeartSendNow++;   // ����ǰ��ʱ3MS���ղ���>��־��ʱ300MS�ط�
  C_BetweenSendSms++;        // ����ǰ��ʱ3MS���ղ���>��־��ʱ300MS�ط�
  C_NetLedLightCSQ++;        // Net LED��˸CSQָʾ������ ZCL 2017.5.25

  if (SClk1Ms >= 10) // 10Ms
  {
    SClk1Ms = 0;
    SClk10Ms++;

    if (S_Display == 0) // ZCL 2019.4.26  =1������ʾ����ҳ���ˣ�����ι��
      IWDG_Feed();      // =1 ȥ���ж��е�ι��	����DTU�����Ͳ������� ZCL

    C_M35Step++;
    C_M35PowerOnOff++;
    C_SmsRunStop++;
  }

  if (SClk10Ms >= 100) // 1S
  {
    SClk10Ms = 0;
    SClkSecond++;
    //
    w32_SecCouter++;
    // ZCL 2018.12.11
    C_M35SendSum++;     // ÿ����һ�μ�������1
    C_DtuInquireSend++; // DTU��ѯָ�������
    C_DtuSetSend++;     // DTU����ָ�������
    C_GprsDataSend++;   // Gprs���ݴ��������
    C_ReadNewSms++;     // ���µĶ��ż�����
    C_SendSms++;        // ���Ͷ���
    C_SmsFull++;        // ��������ɾ������
    // C_ReLink++;								//�����Ӽ�����
    C_DelayRunGprsModule++;
    //
    for (i = 0; i < 4; i++)
    {
      C_HeartSendNow[i]++;
      C_HeartDelay[i]++;
      C_LinkDelay[i]++;
      C_LinkNow[i]++;
      C_LinkNow_DoName[i]++;
    }
  }

  if (C_PowenOnDelay < 10000) // MS  1200 ZCL 2019.4.25
    C_PowenOnDelay++;

  // ZCL 2018.11.13
  TickCounter++;

  //  /* ���ڽ��մ��� */
  //  if (usart3_rev_flag) // ��� usart3 �������ݱ�־Ϊ1
  //  {
  //    tim_cnt = 0;
  //    heart_beat_cnt = 0; // �����������ݹ���ʱ��������������
  //    C_SendHeartbeatTime0 = 0;
  //    C_SendHeartbeatTime = 0;
  //    usart3_rev_cnt++;         // usart3 ���ռ���
  //    if (usart3_rev_cnt >= 20) // ������ 3 ms δ���յ����ݣ�����Ϊ���ݽ�����ɡ�
  //    {
  //      usart3_rev_finish = 1;
  //      usart3_rev_flag = 0;
  //      usart3_rev_cnt = 0;

  //      Rcv3Buffer[Cw_Rcv3] = 0; // 2013.5.9 �ܳ��� ���յ������ݣ�ĩβ��0��
  //      //				printf("Receive Data��\r\n %s\r\n",Rcv3Buffer);		//YLS 2022.12.29 ���յ���GPRS����ͨ������1����ȥ����ʾ����
  //      Cw_BakRcv3 = Cw_Rcv3; // �� Cw_Rcv3 ����
  //      Cw_Rcv3 = 0;          // ׼���´ν��յ����濪ʼ
  //    }
  //  }

  //  /* lte �������� */
  //  if (!usart3_rev_flag && lte_connect_flag) // ������û�н��յ�����ʱ
  //  {
  //    tim_cnt++;           // ��ʱ������
  //    if (tim_cnt >= 1000) // ��ʱ�ﵽ 1s
  //    {
  //      tim_cnt = 0;
  //      heart_beat_cnt++;         // ��������
  //      if (heart_beat_cnt >= 30) // �����ﵽ 30s ��������־�� 1�����Ը���ʵ�������޸ĸ�ֵ��
  //      {
  //        heart_beat_cnt = 0;
  //        heart_beat_flag = 1;
  //      }
  //    }
  //  }
}

/**
 * @brief  This function handles USART1 Handler.
 * @param  None
 * @retval None
 */
void USART1_IRQHandler(void) // ͨѶ��0ʹ��USART1
{
  // ZCL 2018.11.13
  if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET) //
  {
    USART_ClearFlag(USART1, USART_FLAG_ORE); // �ȶ�SR
    USART_ReceiveData(USART1);               // ���DR ȡ�����ӵ�
  }

  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    Rcv1Buffer[Cw_Rcv1++] = USART_ReceiveData(USART1);

    /* Clear the USART1 Receive interrupt */
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);

    if (Cw_Rcv1 >= RCV1_MAX + 2)
    {
      Cw_Rcv1--;
    }

    C_NoRcv1 = 0; // û�н��ռ��������㣬�ܳ��� 2008.12.4
  }

  if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)
  {
    /* Clear the USART1 transmit interrupt */
    USART_ClearITPendingBit(USART1, USART_IT_TC); // 2010.7.4 �ܳ��� �ĳ�	USART_IT_TC

    if (Cw_Txd1 >= Cw_Txd1Max)
    {
      Cw_Txd1 = 0;    // �ܳ��� 2013.1.21
      Cw_Txd1Max = 0; // �ܳ��� 2013.1.21

      if (w_ZhouShanProtocol_bit9) // ����1������ѯ���Ʊ�־λ
        B_Com1Con = 0;

      /* Disable the UART3 Transmit interrupt */
      USART_ITConfig(USART1, USART_IT_TC, DISABLE);
      // ������ɣ�������Ϳ��ƶˣ��������״̬
      // RS485_CON=0;
      w_Com1TXCounter++; // ZCL 2019.10.20  ������ɣ����ͼ�����+1
    }
    else
    {
      /* Write one byte to the transmit data register */
      USART_SendData(USART1, Txd1Buffer[Cw_Txd1++]);
    }
  }
}

/**
 * @brief  This function handles USART2 Handler.
 * @param  None
 * @retval None
 */
void USART2_IRQHandler(void) // USART2
{
  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    Rcv2Buffer[Cw_Rcv2++] = USART_ReceiveData(USART2);

    /* Clear the USART2 Receive interrupt */
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);

    if (Cw_Rcv2 >= RCV2_MAX + 2)
    {
      Cw_Rcv2--;
    }

    C_NoRcv2 = 0; // û�н��ռ��������㣬�ܳ��� 2008.12.4
  }

  if (USART_GetITStatus(USART2, USART_IT_TC) != RESET)
  {
    /* Clear the USART2 transmit interrupt */
    USART_ClearITPendingBit(USART2, USART_IT_TC); // 2010.7.4 �ܳ��� �ĳ�	USART_IT_TC

    if (Cw_Txd2 >= Cw_Txd2Max)
    {
      Cw_Txd2 = 0;    // �ܳ��� 2013.1.21
      Cw_Txd2Max = 0; // �ܳ��� 2013.1.21

      /* Disable the UART3 Transmit interrupt */
      USART_ITConfig(USART2, USART_IT_TC, DISABLE);
      // ������ɣ�������Ϳ��ƶˣ��������״̬
      // RS485_CON=0;
    }
    else
    {
      /* Write one byte to the transmit data register */
      USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]);
    }
  }
}

/**
 * @brief  This function handles USART3 Handler.
 * @param  None
 * @retval None
 */
void USART3_IRQHandler(void) // USART3
{
  if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET) //
  {
    USART_ClearFlag(USART3, USART_FLAG_ORE); // �ȶ�SR
    USART_ReceiveData(USART3);               // ���DR ȡ�����ӵ�
  }

  if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) // ���շǿճ���=1
  {
    /* Read one byte from the receive data register */
    Rcv3Buffer[Cw_Rcv3++] = USART_ReceiveData(USART3);

    /* Clear the USART3 Receive interrupt */
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);

    if (Cw_Rcv3 >= RCV3_MAX)
    {
      Cw_Rcv3--;
    }

    usart3_rev_flag = 1; // ����3 ���ձ�־
    usart3_rev_cnt = 0;  // ����3���ռ�������

    C_NoRcv3 = 0; // û�н��ռ��������㣬�ܳ��� 2008.12.4
  }

  if (USART_GetITStatus(USART3, USART_IT_TC) != RESET)
  {
    /* Clear the USART3 transmit interrupt */     // USART_IT_TXE:�������ݼĴ������ж�
    USART_ClearITPendingBit(USART3, USART_IT_TC); // 2010.7.4 �ܳ��� �ĳ�	USART_IT_TC

    if (Cw_Txd3 >= Cw_Txd3Max)
    {
      Cw_Txd3 = 0;    // �ܳ��� 2013.1.21
      Cw_Txd3Max = 0; // �ܳ��� 2013.1.21

      /* Disable the USART3 Transmit interrupt */
      USART_ITConfig(USART3, USART_IT_TC, DISABLE);
      // ������ɣ�������Ϳ��ƶˣ��������״̬
      // GPIO_ResetBits(GPIOA, GPIO_Pin_0);
    }
    else
    {
      /* Write one byte to the transmit data register */
      USART_SendData(USART3, Txd3Buffer[Cw_Txd3++]);
    }
  }
}

void UART4_IRQHandler(void) // UART4
{
  if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    Rcv4Buffer[Cw_Rcv4++] = USART_ReceiveData(UART4);
    F_Reset_GPS = 0; // �������4�յ�GPS���������ݣ������־λ��������
    C_Reset_GPS = 0; // ������������
    /* Clear the UART4 Receive interrupt */
    USART_ClearITPendingBit(UART4, USART_IT_RXNE);

    if (Cw_Rcv4 >= RCV4_MAX + 2)
    {
      Cw_Rcv4--;
    }

    C_NoRcv4 = 0; // û�н��ռ��������㣬�ܳ��� 2008.12.4
  }

  if (USART_GetITStatus(UART4, USART_IT_TC) != RESET)
  {
    /* Clear the UART4 transmit interrupt */
    USART_ClearITPendingBit(UART4, USART_IT_TC); // 2010.7.4 �ܳ��� �ĳ�	USART_IT_TC

    if (Cw_Txd4 >= Cw_Txd4Max)
    {
      Cw_Txd4 = 0;    // �ܳ��� 2013.1.21
      Cw_Txd4Max = 0; // �ܳ��� 2013.1.21

      /* Disable the UART3 Transmit interrupt */
      USART_ITConfig(UART4, USART_IT_TC, DISABLE);
      // ������ɣ�������Ϳ��ƶˣ��������״̬
      // RS485_CON=0;
    }
    else
    {
      /* Write one byte to the transmit data register */
      USART_SendData(UART4, Txd4Buffer[Cw_Txd4++]);
    }
  }
}

// ZCL 2019.2.10
void EXTI2_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line2) != RESET) // 2019.2.10
  {
    LED0_TOGGLE;

    /* Clear the EXTI line 2 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line2);
  }
}

void EXTI9_5_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line6) != RESET) // 2019.2.10
  {
    LED1_TOGGLE;

    /* Clear the EXTI line 2 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line6);
  }

  if (EXTI_GetITStatus(EXTI_Line7) != RESET)
  {
    ;
  }
}

void TIM2_IRQHandler(void)
{
  //  tim2_interrupt_process();	  // ��ʱ���жϴ���
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
{
}*/

/**
 * @}
 */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
