/**
 ******************************************************************************
 * @file    main.c
 * @author  ChengLei Zhou  - �ܳ���
 * @version V1.00
 * @date    2014-02-12
 * @brief   Main program body
 * 				 Hardware	: STM32F103CBT6,LSM303DLHC
 * 				 Platform	: IAR6.40,Keil-MDK5.01,LIB:V3.5.0(11/03/2011),JLINK V6-V7
 ******************************************************************************
 * History Platform:
 *				2013.12.31 GP311 IAR640 MDK501 LIB3.5ģ�� �ܳ���
 * Change Log:
 ******************************************************************************/

/* �����ʹ��SWD���ԣ�2015.9.3 ZCL ������������
	 ע�ͺ�ʹ�ô��ڵ��ԡ�  */
#define SWD // ZCL 2019.9.11 ��ע��SCR_E�� SWD���Ѿ�ר�ã�����Ҳ�޸ġ�SWD�Ķ����Ѿ�û���ã�

/*ZCL  2019.9.11 ��ע
1. DCM200_SCR_E ��LORA,GPRS,GPS,FM25L16

2. SCR������������浽FM25L16�У���DCM100_SCR_B��һ����������浽ARM�ڲ�FLASH�У�
	//ZCL 2019.4.4 FM25L16������������ռ����  2K�ֽ� 0-1023
		//DCM200_SCR_E��FM25L16����
		//1. 0-99 �ֽڣ�50���֣�����SCR����
		//2. 256 �ֽڿ�ʼ��288���ֽڣ�DTU_PAR_SIZE�� ������ GPRS����

3. ����ʱ�ѿ��Ź�ע�͵�������ʹ��ʱ�ٴ򿪿��Ź�������Ӱ����档 ZCL 2019.9.12

4. ����ͨѶ�У���ѯ��ַ������Ҫ���м��飬��ֹ������������������� ZCL 2019.11.21

5. //ZCL 2019.11.21 Pw_LoRaSet��Pw_GprsSet�趨�Ͳ���˵����
	��1��ͨ��LORA�����趨��
	Pw_LoRaSet=1��F_LoRaEn=1��F_LoRaToCom=0����
	Pw_GprsSet=9��F_GprsEn=1��F_GprsTransToCom=0��F_GprsMasterNotToCom=0��F_GprsMasterToCom=1��

	��2����ʹ�ô���2����LORA����ʱ���趨˵����
		Pw_LoRaSet=1��F_LoRaEn=1��F_LoRaToCom=0����
		Pw_GprsSet=0��F_GprsEn=0��F_GprsTransToCom=0��F_GprsMasterNotToCom=0��F_GprsMasterToCom=0�� GPRS�ر�
		Pw_GprsSet=1 GPRS�򿪣�����ָ��͵�COM2���������Ϻ�� gprsdemo.exe����demo��ָ�û�з��أ�
		Pw_GprsSet=5 GPRS�򿪣�����ָ��͵�COM2����ΪF_GprsMasterNotToCom=1���������Ϻ�� gprsdemo.exe����demo��ָ����Է��أ�
		Pw_GprsSet=9 GPRS�򿪣�����ָ���͵�COM2����ΪF_GprsMasterToCom=1���������Ϻ�� gprsdemo.exe����demo��ָ����Է��أ���ע�����9Ҳ���ã���������5����������ָ��͵�COM2��

6���ô���2���Է��� w_ParLst��5��  ZCL 2019.11.21  2019.11.22 ���ȸĵĸ�ѹ����ĳ�����ֲ�����
	#define STM32_WRPAR_ADDRESS			0 			// ��д������ַ��Pw_ParLst������ʱƫ��0000����
	#define STM32_RPAR_ADDRESS			10000 		// ֻ��������ַ��w_ParLst������ʱƫ��10000����
	#define DSP_WRPAR_ADDRESS				20000 		// ��д������ַ��Pw_dspParLst������ʱƫ��20000����
	#define DSP_RPAR_ADDRESS				30000 		// ֻ��������ַ��w_dspParLst������ʱƫ��30000����
	#define SCR_WRPAR_ADDRESS				40000 		// ��д������ַ��w_ScrParLst������ʱƫ��40000����

7.  ZCL 2019.11.29 ��GPS����
		�����ã�1����F_GpsToCom=1; 		//������Pw_GprsSetΪ16��
				  ��2��Pw_LoRaSet=0;  ����  Pw_LoRaSet=1;

		20191129-V802-1 M J-LINK LORA ��λ�����ʣ�����2 OK������2���Է���w_ParLst��5���� OK ��GSP���� OK
		�� 2019.11.29 4���ط�����İ汾��
	��1��//ZCL 2019.11.29
			extern u8 Txd4Buffer[];
	��2��		 && F_GpsToCom==0 )			//ZCL 2019.11.29 �� && F_GpsToCom==0

	��3��			//ZCL 2019.11.29 Pw_GprsSet .4λ =1  F_GpsToCom  ����: COM2���յ�����ָ�ת����GPS��(COM4)
			else if( F_GpsToCom )

	��4��		//ZCL 2019.11.29 GPS���ã�����ֱ�Ӹ�ֵ�ڲ���ʱ��


  8. ZCL 2020.9.30
			һ��SZM220 DI�����״��⣨�̶���ַ99����ͨ��LORAת����LORA������LORA����ת��������SZM220 DO��� OK



*/

/* @note	2014.1.3 ----------------------------------------------------------
  �ܳ��ڣ�
	  Version_DCM200_SCR_E.C
  1. COM1���ӱ�Ƶ���ARM
  2. GPRS��USART3 ת���� USART2(COM2)
  3. LORA��SPI3 ת���� USART2(COM2)
  4. ע��void DoWith(void)	�� ��ֱ��Ӳ��IO�ķ�ת�������ӿ�ʱҪע�⣡
		  //COM2_DATA ��ת  2014.11.27�޸�
		  GPIO_WriteBit(GPIOC, GPIO_Pin_12,
					 (BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_12))));		//ZCL 2018.12.4
  5. �����������س���GPRS�������ᶪ����ʼ��DTU ID����ģ�

  6. ZCL 2019.4.16 ��ע��DCM200_SCR_E��KEY3��KEY4��KEY5 ��ռ�ã�ʹ��ʱҪע�͵���

  7. ZCL 2019.6.25 GPSż�����ݳ�0��ԭ��²⣺GPRS����������GPSû�и�λ�á�
  �޸ģ�GPIO_Configuration()�� �ر�GPS
		  ��ʱ������ѭ��ǰ����GPS.
		  GPIO_SetBits(GPIOC, GPIO_Pin_1); 			//ZCL 2019.6.25 ��GPS

  8. �ܳ��ڱ�ע�����ӻ���󣬵�����ת�ķ�������522��ʼ����ǰ�滻

		  �ɰ汾��500---522  999
		  �°汾��500---520  523---532  999

����Ҫ���Ӿɰ汾522��ʼ���� �滻���°汾��532��
			  �پɰ汾521������   �滻���°汾��53*��
			  ������
			  ������ǰ�������滻��

  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "GlobalV.h"
#include "GlobalConst.h"
#include "com1_232.h"
#include "com2_232.h"
#include "DoWith.h"
#include "LCD.h"
#include "Key_Menu.h"

#include "LcdGraphic.H"

#include "com3_232.h" //ZCL 2018.10.18
#include "com4_232.h" //ZCL 2018.10.18
#include "com_LoRa.h"

// #include "sys_config.h" //ZCL 2018.11.12
#include "spi.h" //ZCL 2018.11.12		sx12xxEiger�ļ�����

#include "spi_flash.h"

/** @addtogroup STM32F10x_StdPeriph_Template
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
EXTI_InitTypeDef EXTI_InitStructure; // �ⲿ�жϳ�ʼ���ṹ��
NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;

/* Private define ------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 S_Display; // ������ʾ����

/* Private variables extern --------------------------------------------------*/
extern u16 Lw_SavePageNo;
extern u16 Lw_PageNo;	// ��ʾ�Ļ������
extern u8 B_LockCodeOK; // ����������ȷ��־

extern tRadioDriver *Radio; // ZCL 2018.11.13
extern u8 B_LoRaRcv;		// ZCL 2019.01.29

extern uchar S_M35;
extern uchar AT_ComOk_Flag;
extern uchar AT_3EOk_Flag; //>���Ա�־��׼�����뷢�����ݣ�
extern uint C_M35Step;	   // M35ģ�鲽����� 2012.7.20
extern uchar B_PcSetGprs;  // PC����GPRS��־ 2013.6.6
extern uchar B_NextS35Cmd;
extern u8 B_LockCodeOK; // ����������ȷ��־
extern u8 B_LoRaRCVCommandOk;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
// void SysTick_Config(void);
void TIM_Configuration(void);
void EXTI_Configuration(void);	  // ZCL 2009.5.18
void IWDG_Init(u8 prer, u16 rlr); // �������Ź���ʼ�� 2013.7.3
void IWDG_Feed(void);			  // �������Ź�ι��	 2013.7.3
void PowerDelay(u16 nCount);

void XmodemFileTransfer(void);
void XmodemFileTransfer_Com1(void);
void GPIO_PinReverse(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Delay_US(vu16 nCount);
void Delay_MS(vu16 nCount);
/* Private functions ---------------------------------------------------------*/

// MDK5�ã�ͬʱ��MDK Options�������SEMIHOSTING�ĺ궨�壬�������á�use MicroLIB��
#ifdef SEMIHOSTING
#pragma import(__use_no_semihosting)
_sys_exit(int x)
{
	x = x;
}
struct __FILE
{
	int handle;
	/* Whatever you require here. If the only file you are using is */
	/* standard output using printf() for debugging, no file handling */
	/* is required. */
};
/* FILE is typedef�� d in stdio.h. */
FILE __stdout; // ZCL 2019.3.29
#endif

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */

int main(void)
{
	/*!< At this stage the microcontroller clock setting is already configured,
		 this is done through SystemInit() function which is called from startup
		 file (startup_stm32f10x_xx.s) before to branch to application main.
		 To reconfigure the default setting of SystemInit() function, refer to
		 system_stm32f10x.c file
	   */
	// u8 i;

#ifdef DEBUG
	debug();
#endif

	PowerDelay(700);

	/* System Clocks Configuration */
	RCC_Configuration();
	/* GPIO ports pins Con2figuration */
	GPIO_Configuration();

	// ZCL 2015.9.12 �ϵ���ʱ ������ DCM100 ARM����Ҫ
	PowerDelay(300);

	// ZCL  2015.9.12 ��RCC_Configuration  GPIO_Configuration �ϵ��ȶ�����ִ��һ��
	/* System Clocks Configuration */
	RCC_Configuration();
	/* GPIO ports pins Configuration */
	GPIO_Configuration();

	SysTick_Config(72000); // ʹ���ں˺���
	TIM_Configuration();
	//	tim2_init();                     // ��ʱ��2��ʼ��

	SPI_FLASH_Init(); // ��LORA�г�ʼ��
	SpiInit();		  //

	/* Initialize the SPI FLASH driver --------------*/
	Variable_Init(); // Initialize VARIABLE
	ParLst_Init();	 // RAM���趨�������г�ʼ��
	Boot_ParLst();	 // ��ʼ���趨����		ZCL 2018.12.22 ����

	EXTI_Configuration();
	// NVIC_Configuration();
	Com1_Init();
	Com2_Init();

	Com3_Init(); // ZCL 2018.10.18
	Com4_Init(); // ZCL 2018.10.18

	// Xmodem�����ﴫ��ʱ��57600���������������������(һ��9600)
	XmodemFileTransfer_Com1(); // XMODEM�ļ�����
	XmodemFileTransfer();	   // XMODEM�ļ�����

	/* NVIC Configuration */ // �ж��������
	NVIC_Configuration();

	// ZCL 2018.11.12  LORA SX1278 ��ʼ��
	SpiInit();
	LED0_ON;
	// LED1_ON;
	// ZCL 2018.11.21 tRadioDriver *Radio = NULL;  �Զ���ṹ��ָ��
	Radio = RadioDriverInit(); // ZCL radio.c��  tRadioDriver* RadioDriverInit( void )
	Radio->Init();
	Radio->StartRx(); // RFLR_STATE_RX_INIT

	// printf("RF_LoRa_TX_OK�� \n");
	LoRa_Init(); // ZCL 2021.10.25 TEST

	GetLockCode();	  // 2013.10.26 �õ���������
	B_LockCodeOK = 1; // 2013.10.26 ע�ͺ�����Ϊ�������뱣�����ܣ�����=1���൱��û�б���

	LCD_DLY_ms(800);
	LCD12864_Init(); // oled ��ʼ��

	ADC_DMA_Init();

	// 2015.9.6 ZCL 		������ʱ��ʱ���Ӵ��Ź�ʱ�䣬�������������IWDG_Init(4,938)
	// Ԥ��Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ1s;  Ԥ��Ƶ��Ϊ64,����ֵΪ938,���ʱ��Ϊ1.5s	IWDG_Init(A,B): 4*(2^A)*B/40

	// ZCL �����ʱ��ѿ��Ź�ע�͵����ڲ�Ʒ���е�ʱ��򿪡�
	// IWDG_Init(4,938);				//1.5S 	 //ZCL 2021.10.25 TEST
	// IWDG_Init(4,625);    	//1S
	//		IWDG_Init(5,1250); 		//4S

	Pw_ScrLoRaTest1 = 0; // ZCL 2018.11.13  =1 ͸��ģʽ =0��͸�� (=10���Է��ͣ�=11���Խ���ɾ��)

	Lw_SavePageNo = 1000;

	if (Pw_LoRaMasterSlaveSel == 1)
		Lw_PageNo = 500; // ZCL 2019.3.8 ��ʾLORA�ӻ��������滮4���ӻ���
	else if (Pw_LoRaMasterSlaveSel == 0)
	{
		if (Pw_ScrMenuMode == 1)
			Lw_PageNo = 200; // ZCL �ӱ�Ƶ�������濪ʼ��ʾ���޹�ˮ���档ע�͵���������ʾ���й�ˮ����
		else if (Pw_ScrMenuMode == 0)
			Lw_PageNo = 0; //
	}

	GPIO_SetBits(GPIOC, GPIO_Pin_1); // ZCL 2019.6.25 ��GPS
									 //	F_RunAgain = 1;
									 //	F_RunCheckModular = 1;

	B_LoRaRCVCommandOk = 1;
	/* Infinite loop */
	while (1)
	{
		// Delay_MS(4000);					//ZCL 2019.4.26 ���Կ��Ź���λ��
		Boot_ParLst();	  // ��ʼ���趨����
		ParLimit();		  // ��������
		DoWith();		  // һЩ���ݵĴ���
		SavePar_Prompt(); // �����趨������FLASH"+"״̬��ʾ
		ForceSavePar();	  // ǿ�Ʊ������  ZCL 2018.8.3 ��д��������Һ��������
		Time_Output();
		//
		if (w_ZhouShanProtocol_bit9) // ����1������ѯ���Ʊ�־λ
			Com1_MasterSend();		 // ����1�����ͳ���20060923	DB9����1

		Com1_RcvProcess(); // ����1���մ���
		Com1_SlaveSend();  // ����1�ӻ�����

		Com2_MasterSend(); // ����2�����ͳ���20060923	DB9����2
		Com2_RcvProcess(); // ����2���մ���
		Com2_SlaveSend();  // ����2�ӻ�����

		Com3_MasterSend(); // ����3�����ͳ���20181211 GPRS
		Com3_RcvProcess(); // ����3���մ���
		Com3_SlaveSend();  // ����3�ӻ�����

		Com4_MasterSend(); // ����4�����ͳ���20181211 GPS
		Com4_RcvProcess(); // ����4���մ���
		Com4_SlaveSend();  // ����4�ӻ�����
		Judge_Reset_GPS(); // �ж��Ƿ���Ҫ����GPS������4����60��û�յ�GPS����������
		PowerOnOff_GPS();  // ����GPS
		// ZCL 2019.1.19 �Ӳ������趨����  Pw_LoRaSet
		if (Pw_ScrLoRaTest1 == 0) // Pw_ScrLoRaTest1=0 ��͸��
		{
			if (F_LoRaEn) //.0 =1����LORA		//Pw_LoRaSet .0 =1����LORA
			{
				if (Pw_LoRaMasterSlaveSel == 1)//LoRa����
				{
					LoRa_MasterSend();		   // LoRa�����ͳ���
					if (Pw_EquipmentType == 0) // ˫���òŻ��жϱ�Ƶ��ͨѶ�Ƿ�����
						Jude_VVVF_Comm();	   // �жϱ�Ƶ��ͨѶ
				}

				LoRa_RcvProcess(); // LoRa���մ���
				LoRa_SlaveSend();  // LoRa�ӻ�����
			}
		}

		// ZCL 2019.1.24 ����LORA
		if (Pw_ScrLoRaTest1 == 1) // Pw_ScrLoRaTest1=1 ͸��
			LoRaRcvTest();

		LoRaSendToDone(); // LoRaSendTest

		IWDG_Feed(); // ι��	2013.7.3

		// AnologIn(); 						//ZCL 2018.11.13 �ݴ棬����ĳ�����ɾ��
		/* 		TIM_SetCompare2(TIM2, w_AI1Value); 		//PWM-DAC */
		// TIM2->CCR2=w_AI1Value; 		//Ҳ����ֱ�Ӳ����Ĵ���

		if (w_TestItemSel >= 1) // ����ѡ�� 2014.5.6
		{
			Test(); // ���Գ���
		}
		else if (B_LockCodeOK == 1) // 2013.10.26 ��Ҫ��������У����ȷ��ִ��
		{
			;
		}

		// ���Ի����� Ƭͷ�����ã�ֻ��ʾһ�Σ�
		if (S_Display == 0)
		{
			Menu_Welcome(); // ���� ��ӭ

			S_Display = 1; // ZCL 2019.4.26  ����������ж���Ҳʹ�ã�����ι��
		}

		if (S_Display == 1) // ZCL 2019.1.29
		{

			GetKey();
			Menu_Change();

			if (F_GprsEn) // Pw_GprsSet.0=1	GPRS�򿪣�����
			{
				// ִ��������1. <3  2.��OK �� > ��־ 3.��ʱʱ�䵽(C_M35Step 10MS������)
				if (S_M35 < 3 || S_M35 >= 0x1B)
					M35_LOOP(); // M35 ģ��ѭ��ִ�г���

				else if (AT_ComOk_Flag || AT_3EOk_Flag || B_NextS35Cmd || C_M35Step >= 100)
					M35_LOOP(); // M35 ģ��ѭ��ִ�г���

				if (B_PcSetGprs == 2)	 // PC�������������GPRS���� 2013.6.6
					SmsAndPC_SetupInq(); // ���ź�PC���úͲ�ѯGPRS����
			}
		}

	} // while end
}

/*******************************************************************************
 * Function Name  : RCC_Configuration
 * Description    : Configures the different system clocks.
 * Input          : None
 * Output         : None
 * Return     xia    : None
 *******************************************************************************/
void RCC_Configuration(void)
{
	// ErrorStatus HSEStartUpStatus;

	// /* RCC system reset(for debug purpose) */
	// RCC_DeInit();

	// /* Enable HSE */
	// RCC_HSEConfig(RCC_HSE_ON);

	// /* Wait till HSE is ready */
	// HSEStartUpStatus = RCC_WaitForHSEStartUp();

	// if(HSEStartUpStatus == SUCCESS)
	// {
	// /* HCLK = SYSCLK */
	// RCC_HCLKConfig(RCC_SYSCLK_Div1);

	// /* PCLK2 = HCLK */
	// RCC_PCLK2Config(RCC_HCLK_Div1);

	// /* PCLK1 = HCLK/2 */
	// RCC_PCLK1Config(RCC_HCLK_Div2);

	// /* ADCCLK = PCLK2/6 */
	// RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	// /* Flash 2 wait state */
	// FLASH_SetLatency(FLASH_Latency_2);

	// /* Enable Prefetch Buffer */
	// FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

	// /* PLLCLK = 8MHz * 4 = 32 MHz */
	// RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_4);   // 2013.1.17

	// /* Enable PLL */
	// RCC_PLLCmd(ENABLE);

	// /* Wait till PLL is ready */
	// while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	// {
	// }

	// /* Select PLL as system clock source */
	// RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	// /* Wait till PLL is used as system clock source */
	// while(RCC_GetSYSCLKSource() != 0x08)
	// {
	// }
	// }

	/* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE and AFIO clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // USART1��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // USART2��ʱ��

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); // USART3��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);  // UART4��ʱ��

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // DAC�� 2014.4.23
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // ��ʱUS,MS��

	// 2013.12.8 ADC�ɼ�
	/* Enable DMA clock */							   // !!! �¿��ļ���RCC_AHBPeriph_DMA1   RCC_AHBPeriph_DMA2 2008.11.26
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // ʹ��DMAʱ��  ADC��
	/* Enable ADC1                 /and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); // ʹ��ADC1ʱ��

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE); // ZCL 2018.11.9 ע��SPI3ʹ��APB1ʱ��
}

/*******************************************************************************
 * Function Name  : GPIO_Configuration
 * Description    : Configures the different GPIO ports.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

#ifdef SWD
	// 2015.9.3 �ܳ��� ����ʹ��JTAG��SWD���ܣ�������ԡ�
	// Ҫ��JTDI ������һ�� 2013.8.30
	// ���ͣ�GPIO_Remap_SWJ_JTAGDisable  ��JTAG-DP ʧ�� + SW-DPʹ�� ��
	// PB4��PB3��PA15�������Ų�����ͨIO������JTAG�ĸ��ù��ܣ��ֱ�ΪJNTRST��JTDI��JTDO��
	//
#else
	// �ص�JTAG����,�ͷ�PA13��PA14
	// GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); //SWJ ��ȫʧ�ܣ�JTAG+SW-DP��
#endif

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	//-------------------------1. LCD -- 12864Һ���ӿ�
	// LCD_RS	 --	  PB11-->PB9	ZCL 2018.10.18
	// LCD_RW	 --	  PB12-->PC4  ZCL 2018.12.4
	// LCD_En	 --	  PB13-->PA7	ZCL 2018.11.29

	// LCD_DATA0	 --	  PB14
	// LCD_DATA1	 --	  PB15

	// LCD_DATA2	 --	  PC6
	// LCD_DATA3	 --	  PC7
	// LCD_DATA4	 --	  PC8
	// LCD_DATA5	 --	  PC9

	// LCD_DATA6	 --	  PA8
	// LCD_DATA7	 --	  PA11-->PA15 ZCL 2018.11.29

	// LCD_PSB	 --	  PA12

	// LCD_RST	 --	  PA13-->PC2			ZCL 2018.10.18
	// 2015.3.28 LCD_RST�ĳ�PA15��PA13�ó�SWDIO �ܳ���
	// 2015.3.28 CPU_BUZZER������ȡ����PA14��SWCLK �ܳ���

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_15 | GPIO_Pin_12 | GPIO_Pin_7; // ZCL 2018.11.29
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// LCD_RST
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//-------------------------1.2 Һ������
	// LCD_BACKLIGHT  --  PB0

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		 //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	Qb_LcdBacklight = 1;

	// 2015.3.28 LCD_RST�ĳ�PA15��PA13�ó�SWDIO �ܳ���
	/* 	#ifdef SWD
		;
		#else
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13 ;
			GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		#endif */

	/* #ifdef SWD
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		//2015.3.28 LCD_RST�ĳ�PA15��PA13�ó�SWDIO �ܳ���
		GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	#endif */

	//-------------------------2. �����˿�
	// KEY1		 --	  	PA4
	// KEY2		 --	  	PA5
	// KEY3		 --	  	PA6			ZCL 2018.11.30  ɾ��
	// KEY4		 --	  	PA7			ZCL 2018.11.30  ɾ��

	// KEY5		 --	  	PC4			ZCL 2018.11.30  ɾ��
	// KEY6		 --	  	PC5

	// KEY7		 --	  	PB1
	// KEY8		 --	  	PC13
	// KEY9		 --	  	PB10-->PB8-->PC3				//ZCL 2018.10.18  //ZCL 2018.11.30
	// KEY10		 --	  	PC12-->PC14  						//ZCL 2018.12.5
	// KEY11		 --	  	PC11-->PC3-->PB7				//ZCL 2018.10.18  //ZCL 2018.11.30

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5; //| GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_7; // ZCL 2018.11.30  GPIO_Pin_8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; // ZCL 2018.12.3	 GPIO_Pin_4 |
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_14 | GPIO_Pin_13; // ZCL 2018.12.5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//-------------------------3. ����˿�
	// PA0			ZCL 2018.10.18 	�ĳɣ�LoRa_NSS
	// PA1			ZCL 2018.10.18 	�ĳɣ�LoRa_RESET
	// 3.1 LoRa
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// ����	DIO1:PD2  DIO2:PA6
	/* Configure 	as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// 3.2 GPRS
	// ���  PWR_KEY:PBout(12)��RESET_4G:PBout(13)
	/* Configure as Output push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// ��������ʼ����ֵΪ0���������Ҫ��ֵ
	// GPIO_ResetBits(GPIOB, GPIO_Pin_12);		//=0���������ܳ����ߣ�ģ�鲻����
	GPIO_SetBits(GPIOB, GPIO_Pin_12); //=1���������ܳ����ͣ�ģ�鿪��
	GPIO_ResetBits(GPIOB, GPIO_Pin_13);

	// GPRSָʾ�Ƴ�ʼ��
	// PC12: COM1_DATA����GPRSָʾ�ƣ�H1��
	// PC15: PWR����WORKָʾ�ƣ�H0��
	// PA11: NET����LoRaָʾ�ƣ�H2��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// ��������ʼ����ֵΪ0���������Ҫ��ֵ
	GPIO_SetBits(GPIOC, GPIO_Pin_15); // H0
	GPIO_SetBits(GPIOC, GPIO_Pin_12); // H1
	GPIO_SetBits(GPIOA, GPIO_Pin_11); // H2

	// ����GPRS	STATUS_4G:PB8
	/* Configure 	as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// 3.3 GPS  GP_1PPS:PC0 ����������		GP_ON/OFF(RESET):PC1  �͵�ƽ�ض�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOC, GPIO_Pin_0); // YLS 2023.12.25 ��4Gģ���Դ

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOC, GPIO_Pin_1); // ZCL 2019.6.25 �ر�GPS

	//	 GPIO_SetBits(GPIOC, GPIO_Pin_1); 			//ZCL 2018.10.19 ��GPS

	//-------------------------4. SPI ����洢��
	// SPI3_SCK	  --   PB3
	// SPI3_MISO	  --   PB4
	// SPI3_MOSI	  --   PB5
	// SPI3_NSS	  --   PB6		FM25L16 Chip select

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	  //
	GPIO_Init(GPIOB, &GPIO_InitStructure);			  //

	// Configure FM25L16 Chip select:  PB6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // �����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);			  //

	GPIO_SetBits(GPIOB, GPIO_Pin_6);
}

/*******************************************************************************
 * Function Name  : EXTI_Configuration
 * Description    : Configures the used EXTI lines.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
// 3   EXTI_Mode_Interrupt = 0x00,  		//�жϴ���
// 4   EXTI_Mode_Event = 0x04           //�¼�����

// 9   EXTI_Trigger_Rising = 0x08,       //�����ش���
// 10   EXTI_Trigger_Falling = 0x0C,        //�½��ش���
// 11   EXTI_Trigger_Rising_Falling = 0x10  //�ߵ͵�ƽ����
void EXTI_Configuration(void)
{
	// /* Connect EXTI Line2 to PD2 */
	// GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource2);

	// /* Configure EXTI Line0 to generate an interrupt on falling edge */
	// EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	// EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	// EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	//EXTI_Trigger_Falling;
	// EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	// EXTI_Init(&EXTI_InitStructure);

	// /* Connect EXTI Line6 to PA.6 */
	// GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);

	// /* Configure EXTI Line1 to generate an interrupt on falling edge */
	// EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	// EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	// EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	//EXTI_Trigger_Falling;
	// EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	// EXTI_Init(&EXTI_InitStructure);
}

/*******************************************************************************
 * Function Name  : NVIC_Configuration
 * Description    : Configures the NVIC and Vector Table base address.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void NVIC_Configuration(void)
{

#ifdef VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

	/* Configure the NVIC Preemption Priority Bits �������ȼ��� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* Enable the USART1 Interrupt */				  // ��V2.0.3�� USART3_IRQChannel
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; // 2014 ZCL FOR LIB-V3.5.0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the USART2 Interrupt */				  // ��V2.0.3�� USART3_IRQChannel
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; // 2014 ZCL FOR LIB-V3.5.0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the USART2 Interrupt */				  // ��V2.0.3�� USART3_IRQChannel
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; // 2014 ZCL FOR LIB-V3.5.0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the UART4 Interrupt */ // ZCL 2018.10.23
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn; // 2014 ZCL FOR LIB-V3.5.0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //
	NVIC_Init(&NVIC_InitStructure);

	// ZCL 2019.2.14
	//  /* Enable the EXTI2_IRQChannel Interrupt */
	// NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// NVIC_Init(&NVIC_InitStructure);

	// /* Enable the EXTI6_IRQChannel Interrupt */
	// NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
 * Function Name  : SysTick_Config
 * Description    : Configure a SysTick Base time to 10 ms.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
// void SysTick_Config(void)
// {
// /* Configure HCLK clock as SysTick clock source */
// SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

// /* SysTick interrupt each 1000 Hz with HCLK equal to 32MHz */		//equal:���
// SysTick_SetReload(32000);		// 32000=1000HZ  //320=100 000HZ�ٶ����Լӿ�

// /* Enable the SysTick Interrupt */
// SysTick_ITConfig(ENABLE);

// /* Enable the SysTick Counter */
// SysTick_CounterCmd(SysTick_Counter_Enable);
// }

/*�ڼ��Ĵ���(IWDG_KR)��д��0xCCCC����ʼ���ö������Ź���
��ʱ��������ʼ���临λֵ0xFFF�ݼ�������
��������������ĩβ0x000ʱ�������һ����λ�ź�(IWDG_RESET)��
���ۺ�ʱ��ֻҪ���Ĵ���IWDG_KR�б�д��0xAAAA�� IWDG_RLR�е�ֵ�ͻᱻ���¼��ص��������дӶ�����������Ź���λ ��*/
// ���Ź��������� //��ʼ���������Ź�
// prer����Ƶ����0~7��ֻ�е�3λ��Ч����
// Ԥ��Ƶ����=4*2^prer�����ֵΪ256
// rlr: ��װ�ؼĴ���ֵ: ��11 λ��Ч.
// ʱ�����(���):Tout=((4��2^prer) ��rlr)/40 (ms).
void IWDG_Init(u8 prer, u16 rlr)
{
	IWDG->KR = 0X5555; // ʹ�ܶ�IWDG->PR��IWDG->RLR��д
	IWDG->PR = prer;   // ���÷�Ƶϵ��
	IWDG->RLR = rlr;   // ��װ�ؼĴ��� IWDG->RLR
	IWDG->KR = 0XAAAA; // reload
	IWDG->KR = 0XCCCC; // ʹ�ܿ��Ź�
}

// ι�������Ź�
void IWDG_Feed(void)
{
	IWDG->KR = 0XAAAA; // reload
}

void TIM_Configuration(void)
{
	/* TIM2 Configuration ---------------------------------------------------
	 Generates PWM signals
	 TIM2CLK = 72 MHz, Prescaler = 0x0, TIM2 counter clock = 72 MHz
	 TIM2 frequency = TIM2 counter clock/(TIM2_Period + 1) = 17.57 KHz	72000000/4096
	   - TIM2 Channel1 & Channel1N duty cycle = TIM2->CCR1 / (TIM2_Period + 1) = 50%
	   - TIM2 Channel2 & Channel2N duty cycle = TIM2->CCR2 / (TIM2_Period + 1) = 37.5%
	   - TIM2 Channel3 & Channel3N duty cycle = TIM2->CCR3 / (TIM2_Period + 1) = 25%
	   - TIM2 Channel4 duty cycle = TIM2->CCR4 / (TIM2_Period + 1) = 12.5%
	   //�ܳ��� ���ھ���Ƶ��Ϊ36MHZ������PWMƵ�ʼ���
	   //��Ӧ�ܽŸ����������

	----------------------------------------------------------------------- */

	/* TIM2 Peripheral Configuration */
	// Resets the TIM peripheral registers to their default reset values
	// TIMx: where x can be 1 to 8 to select the TIM peripheral.
	TIM_DeInit(TIM2); // Ĭ�ϳ�ʼ��

	// ZCL 2021.5.4 GD32		������������� TIM ������ GD ������ֵ���� 0���� ST ����������ֵ������ֵ�� 0xFFFF;
	// ZCL ����ԭ�ȳ����������ֵ

	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Period = 0xFFF;  // 1.��������
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 2.����ʱ��Ƶ��=clock/(ʱ��Ԥ��Ƶ+1)
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Channel 1, 2,3 and 4 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; // TIM_OCMode_PWM2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	// TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;    N�������ʵ���趨 �ܳ���
	TIM_OCInitStructure.TIM_Pulse = 0x0;					 // ����ռ�ձ�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; // ���� ������ֵС��TIM_Pulseʱ�ĵ�ƽ
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	/* TIM2 counter enable */
	TIM_Cmd(TIM2, ENABLE);

	/* TIM1 Main Output Enable */ // �߼���ʱ�����д˹���
								  // Enables or disables the TIM peripheral��s main outputs.
	// TIM_CtrlPWMOutputs(TIM1,ENABLE);

	// TIM_OC1PreloadConfig(),TIM_ARRPreloadConfig();�������������Ƶ���ccr1��arr��Ԥװ��ʹ�ܣ�
	// ʹ�ܺ�ʧ�ܵ�������ǣ�ʹ�ܵ�ʱ���������Ĵ����Ķ�д��Ҫ�ȴ��и����¼�����ʱ���ܱ��ı�
	// ���������������Ǹ����¼�����ʧ�ܵ�ʱ�����ֱ�ӽ��ж�д��û���ӳ١�

	// ���������е�����Ҫ�ı�pwm��Ƶ�ʺ�ռ�ձȵ��ã�
	// TIM_SetAutoreload()
	// TIM_SetCompare1()�����������Ϳ����ˡ�

	// 2014.7.10 TIM3 ����(��ʱ)	 ZCL 2018.12.10
	TIM_TimeBaseStructure.TIM_Period = 1;		  // �Զ�װ��
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // �ܳ��ڣ����ݾ���72M��Ƶ�ʵ�1000KHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;

#ifdef GD32
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0; // ZCL 2021.5.4 GD32 �¼�
#else
	;
#endif

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; // ���¼���
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
}

void SPI2Init(void)
{
	/* 	GPIO_InitTypeDef           GPIO_InitStructure;						  //
		SPI_InitTypeDef            SPI_InitStructure;						  //

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);                 //ʹ�ܶ˿�Bʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 				  //
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);                  //ʹ��SPI2ʱ��ʱ��

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                       //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;					  //
		GPIO_Init(GPIOB, &GPIO_InitStructure);								  //

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                            //SPI2_NSS PB12
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                      //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;					  //
		GPIO_Init(GPIOB, &GPIO_InitStructure);								  //

		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;    //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                         //����SPI����ģʽ:����Ϊ��SPI
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                     //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                            //ѡ���˴���ʱ�ӵ���̬:ʱ�����յ�
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                          //���ݲ����ڵڶ���ʱ����
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                             //SPI_NSS_Soft;  //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;  //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                    //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
		SPI_InitStructure.SPI_CRCPolynomial = 7;                              //CRCֵ����Ķ���ʽ
		SPI_Init(SPI2, &SPI_InitStructure);                                   //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
		SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, DISABLE);                     //SPI2�жϿ��� ,�ر�
		SPI_Cmd(SPI2, ENABLE);                                                //ʹ��SPI����
	 */
}

void SPI3Init(void)
{
	/* 	GPIO_InitTypeDef           GPIO_InitStructure;						  //
		SPI_InitTypeDef            SPI_InitStructure;						  //

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);                 //ʹ�ܶ˿�Bʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 				  //
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);                  //ʹ��SPI3ʱ��ʱ��

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;		//����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                       //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;					  //
		GPIO_Init(GPIOB, &GPIO_InitStructure);								  //

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                             //SPI3_NSS PB6
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                      //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;					  //
		GPIO_Init(GPIOB, &GPIO_InitStructure);								  //

		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;    //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                         //����SPI����ģʽ:����Ϊ��SPI
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                     //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                            //ѡ���˴���ʱ�ӵ���̬:ʱ�����յ�
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                          //���ݲ����ڵڶ���ʱ����
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                             //SPI_NSS_Soft;  //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;  //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                    //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
		SPI_InitStructure.SPI_CRCPolynomial = 7;                              //CRCֵ����Ķ���ʽ
		SPI_Init(SPI3, &SPI_InitStructure);                                   //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
		SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_RXNE, DISABLE);                     //SPI2�жϿ��� ,�ر�
		SPI_Cmd(SPI3, ENABLE);                                                //ʹ��SPI����
	 */
}

/********************************************************
 * us��ʱ����                                         *
 ********************************************************/
void Delay_US(vu16 nCount)
{
	u16 TIMCounter = nCount;
	TIM_Cmd(TIM3, ENABLE);
	TIM_SetCounter(TIM3, TIMCounter);
	while (TIMCounter > 1)
	{
		TIMCounter = TIM_GetCounter(TIM3);
	}
	TIM_Cmd(TIM3, DISABLE);
}

/********************************************************
 * ms��ʱ����                                         *
 ********************************************************/
void Delay_MS(vu16 nCount)
{
	u16 i;
	for (i = 0; i < nCount; i++)
	{
		Delay_US(1000); // 1MS
	}
}

void delay_ms(vu16 nCount)
{
	u16 i;
	for (i = 0; i < nCount; i++)
	{
		Delay_US(1000); // 1MS
	}
}

/********************************************************
 * �ϵ���ʱ����  ԼMS  ZCL                               *
 ********************************************************/
void PowerDelay(u16 nCount) // 2015.9.12
{
	// ZCL 2015.9.12 ���� unsigned int = u32 ���Դ���65535������ARM STM32��˵
	unsigned int i = 36000;
	while (nCount--)
	{
		while (i > 0)
			i--;
		i = 36000;
	}
}

// ZCL 2018.11.13
void GPIO_PinReverse(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	GPIOx->ODR ^= GPIO_Pin;
}

#ifdef DEBUG
/*******************************************************************************
 * Function Name  : assert_failed
 * Description    : Reports the name of the source file and the source line number
 *                  where the assert_param error has occurred.
 * Input          : - file: pointer to the source file name
 *                  - line: assert_param error line source number
 * Output         : None
 * Return         : None
 *******************************************************************************/
void assert_failed(u8 *file, u32 line)
{
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
