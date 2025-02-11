///*******************************************************************************
//* @file    			tim.c
//* @function     ��ʱ������					  				
//* @brief 				��3.5.0�汾�⽨�Ĺ���ģ��						  	
//* @attention		�����̽���Ϊ�ο�������ʵ�ʿ����������ʵ���������	
//* @version 			V1.0		
//* @company  		�����з�˼�����ӿƼ����޹�˾
//* @website  		https://freestrong.taobao.com
//* @Author       WisBarry
//* @date    			2018-11-10	
//********************************************************************************/

//#include "tim.h"
////#include "lte_uart2.h"
////#include "lte_at.h"

//uint8_t heart_beat_flag = 0;        // ������־
//extern uint8_t usart3_rev_flag,usart3_rev_cnt,usart3_rev_finish,lte_connect_flag;

///**
//  * @brief  ��ʱ��2��������    
//  * @param  ��
//  * @retval ��
//	* @note   1ms�ж�һ��
//  */
//void tim2_parameter_init(void)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);              // ʹ�ܶ�ʱ��2ʱ��
//	
//	/*��ʱ����������*/
//	TIM_TimeBaseStructure.TIM_Period = 999;					                // ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
//	TIM_TimeBaseStructure.TIM_Prescaler = 71;					              // ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ(72KHz�ļ���Ƶ��)
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		        // ����ʱ�ӷָ�:���ָѡ��Ĭ��TDTS = TIM_CKD_DIV1
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	      // TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE );           // ������ʱ��2�ж�
//	TIM_Cmd(TIM2, ENABLE);  									            // ������ʱ��2	
//}	

///**
//  * @brief  ��ʱ��2�ж�������ʼ������   
//  * @param  ��
//  * @retval ��
//  */
//void tim2_nvic_config(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;                        // ����NVIC��ʼ���ṹ��
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);             // �����ж����ȼ���Ϊ2����ռ���ȼ��ʹ����ȼ�����λ(����0��3)

//	/*��ʱ��2�ж���������*/
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				      // TIM2�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	// ������ռ���ȼ�1��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			    // ������Ӧ���ȼ�0��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				      // ʹ��IRQͨ��
//	NVIC_Init(&NVIC_InitStructure); 	
//}

///**
//  * @brief  ��ʱ��2��ʼ��  
//  * @param  ��
//  * @retval ��
//  */
//void tim2_init(void)
//{
//  tim2_parameter_init();
//  tim2_nvic_config();
//}

///**
//  * @brief  ��ʱ��2�жϴ��� 
//  * @param  ��
//  * @retval ��
//  */
//void tim2_interrupt_process(void)
//{
//	static uint16_t tim_cnt = 0;                         // ��ʱ������
//	static uint8_t heart_beat_cnt = 0;                   // ��������
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)   // ���TIM2�����жϷ������
//	{	
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );      // ���TIM2�����жϱ�־
//		/* ���ڽ��մ��� */
//		if(usart3_rev_flag)                                // ��� usart2 �������ݱ�־Ϊ1
//	  {
//			tim_cnt = 0;
//      heart_beat_cnt = 0;					                     // �����������ݹ���ʱ��������������
//		  usart3_rev_cnt++;                                // usart2 ���ռ���	
//		  if(usart3_rev_cnt >= 3)                          // ������ 3 ms δ���յ����ݣ�����Ϊ���ݽ�����ɡ�
//		  {
//			  usart3_rev_finish = 1;
//				usart3_rev_flag = 0;
//				usart3_rev_cnt = 0;
//		  }
//	  }
//		
//		/* lte �������� */
//		if(!usart3_rev_flag && lte_connect_flag)         // ������û�н��յ�����ʱ
//		{
//			tim_cnt++;                                      // ��ʱ������
//			if(tim_cnt >= 1000)                             // ��ʱ�ﵽ 1s
//			{
//				tim_cnt = 0;
//			  heart_beat_cnt++;                             // ��������
//			  if(heart_beat_cnt >= 30)                      // �����ﵽ 30s ��������־�� 1�����Ը���ʵ�������޸ĸ�ֵ��
//			  {
//				  heart_beat_cnt = 0;
//				  heart_beat_flag = 1;
//			  }
//		  }			
//		}
//	}
//}
