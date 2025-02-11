// #include "sim7600ce.h"
// #include "GlobalConst.h"
// #include "GlobalV_Extern.h"
// #include <stdio.h>
//// #include <stdlib.h>
// #include <string.h>
// #include "malloc.h"
// #include "stdarg.h"

// u8 Scan_Wtime = 0;		  // ����ɨ����Ҫ��ʱ��
// u8 sim_ready = 0;		  // SIM������״̬=1������=0δ�ҵ�
// u8 F_RunAgain = 1;		  // ��������
// u8 C_simCheckNum = 0;	  // SIM��������
// u8 T_NetworkCheck = 0;	  // ��������ʱ
// u8 C_NetworkCheck = 0;	  // ������ʱ��
// u8 F_RunCheckModular = 0; // ���й����м��ģ���־

// u8 T_AgainLinkTime;	 // ��ʱ�������ݵ�ʱ��
// u16 C_AgainLinkTime; // ��ʱ�������ݵĴ���

// u8 T_CheckLinkTime;	 // ���GPRS��������ʱ��
// u16 C_CheckLinkTime; // ���GPRS�������Ӵ���

// u8 T_SendHeartbeatTime0;  // ��������ʱ��
// u32 C_SendHeartbeatTime0; // ��������ʱ����ʱ����

// u8 T_SendHeartbeatTime;	 // ��������ʱ��
// u32 C_SendHeartbeatTime; // ��������ʱ����ʱ����

// u8 T_SendATime;	 // ����ATʱ��
// u16 C_SendATime; // ����AT��ʱ����

// u8 T_GPRSLEDTime;  // GPRSָʾ��״̬�л���ʱ
// u16 C_GPRSLEDTime; // GPRSָʾ��״̬�л���ʱ

// u8 F_OneConnectStat = 0;	 // ��1·��Ҫ����IP��״̬
// u8 F_TWOConnectStat = 0;	 // ��2·��Ҫ����IP��״̬
// u8 F_ThreeConnectStat = 0;	 // ��3·��Ҫ����IP��״̬
// u8 F_FourConnectStat = 0;	 // ��4·��Ҫ����IP��״̬
// u8 ConnectIPNum = 0;		 // ��Ҫ����IP������
// u8 NowConnectIPNum = 0;		 // ��ǰ�����豸������
// u8 F_ONESendHeartbeat = 0;	 // ��������1
// u8 F_TwoSendHeartbeat = 0;	 // ��������2
// u8 F_ThreeSendHeartbeat = 0; // ��������3
// u8 F_FourSendHeartbeat = 0;	 // ��������4

// u8 F_ONESendHeartbeat0 = 0;	  // ��������1
// u8 F_TwoSendHeartbeat0 = 0;	  // ��������2
// u8 F_ThreeSendHeartbeat0 = 0; // ��������3
// u8 F_FourSendHeartbeat0 = 0;  // ��������4

// u8 Connect1Lose; // ��1·����ʧ��
// u8 Connect2Lose; // ��2·����ʧ��
// u8 Connect3Lose; // ��3·����ʧ��
// u8 Connect4Lose; // ��4·����ʧ��
// u8 Connect1OK;	 // ��1·���ӳɹ�
// u8 Connect2OK;	 // ��2·���ӳɹ�
// u8 Connect3OK;	 // ��3·���ӳɹ�
// u8 Connect4OK;	 // ��4·���ӳɹ�

// u8 NormalHeartbeat1_0; // ��1·��������
// u8 NormalHeartbeat2_0; // ��2·��������
// u8 NormalHeartbeat3_0; // ��3·��������
// u8 NormalHeartbeat4_0; // ��4·��������

// u8 NormalHeartbeat1; // ��1·��������
// u8 NormalHeartbeat2; // ��2·��������
// u8 NormalHeartbeat3; // ��3·��������
// u8 NormalHeartbeat4; // ��4·��������

// u8 Select_NetNum = 0;  // ����ѡ�����
// u8 S_check_status = 0; // ģ���ʼ��״̬���Բ����

// u8 domain_name[30] = {0};  //="";	//����1
// u8 domain_name1[30] = {0}; //="";	//����2
// u8 domain_name2[30] = {0}; //="";	//����3
// u8 domain_name3[30] = {0}; //="";	//����4

// u8 Net2G[] = {"AT+CNMP=13"};  //  GSM
// u8 Net4G[] = {"AT+CNMP=38"};  //  LTE
// u8 NetAuto[] = {"AT+CNMP=2"}; // AUTO

// u8 connectsta = 0;		  // 0,��������;1,���ӳɹ�;2,���ӹر�;
// u8 hbeaterrcnt0 = 0;	  // �������������,����8�������ź���Ӧ��,��������ϵͳ����������
// u8 SendDataErrorNum0 = 0; // �������ݴ��������,����8�η��������ź���Ӧ��,������ϵͳ����������

// u8 hbeaterrcnt = 0;		 // �������������,����8�������ź���Ӧ��,��������ϵͳ����������
// u8 SendDataErrorNum = 0; // �������ݴ��������,����8�η��������ź���Ӧ��,������ϵͳ����������

// u8 Link1SendOK0 = 0; // ��1·���ͳɹ�
// u8 Link2SendOK0 = 0; // ��2·���ͳɹ�
// u8 Link3SendOK0 = 0; // ��3·���ͳɹ�
// u8 Link4SendOK0 = 0; // ��4·���ͳɹ�

// u8 Link1SendOK = 0; // ��1·���ͳɹ�
// u8 Link2SendOK = 0; // ��2·���ͳɹ�
// u8 Link3SendOK = 0; // ��3·���ͳɹ�
// u8 Link4SendOK = 0; // ��4·���ͳɹ�

// u16 PowerOnOFFWaitNum = 0; // ���ػ�״̬����

// u8 ModularADDIP[4]; // ģ�鱾���IP��ַ

// u8 CallerNumberbuf[11]; // �յ����ŵĵ绰����
// u8 SMSbuf[50];			// �յ��������ݴ洢��

//// ���������ӱ���
////[15]:0,û�н��յ����ݣ�1���յ���һ������
////[14:0]:���յ������ݳ���
// u16 USART3_RX_STA = 0;		// ���ڽ��յ�����������
// extern u8 Rcv3Buffer[];		// COM3���ջ�����
// extern u8 Txd3Buffer[];		// COM3���ͻ�����
// extern u8 Txd2Buffer[];		// COM2���ͻ�����
// extern uint Cw_Rcv3;		// ���ռ�����//
// extern uint Cw_Txd3;		// ���ͼ�����//
// extern uint Cw_BakRcv3;		// ���ռ�����//
// extern uint Cw_Txd3Max;		// �ж��ٸ��ַ���Ҫ����//
// extern uint Lw_Txd3ChkSum;	// ����У��ͣ�lo,hi ��λ//
// extern uint Lw_Com3RegAddr; // ����3�Ĵ�����ַ

// extern uchar B_Com3Cmd03;
// extern uchar B_Com3Cmd16;
// extern uchar B_Com3Cmd01;
// extern uchar B_Com3Cmd06;
// extern uchar B_Com3Cmd73;
// extern u8 B_LoraSendWrite; // ZCL 2021.11.16 LORA д����ķ���
// extern u8 B_LoraSendWriteLength;

// extern uint8_t LoRaTxBuf2[]; // ZCL 2021.11.18
// extern uchar Txd3TmpBuffer[TXD3_MAX];
// extern uint Cw_Txd3TmpMax;
// extern uchar B_GprsDataReturn; // GPRS ���ݷ��ء������ӣ��յ�����2���ݣ���ͨ��DTU���ͳ�ȥ
// extern u16 Cw_Txd2Max;		   // �ж��ٸ��ַ���Ҫ����
// extern u8 Rcv2Buffer[];		   // ���ջ�����
// extern u16 Cw_Rcv2;			   // ���ռ�����
// extern u16 Cw_Txd2;			   // ���ͼ�����
// extern u16 Cw_BakRcv2;		   // ���ռ���������

// u8 BakRcv2Count;
// u8 S_HongDianAcesse;
// u8 F_DTUSendRegister;
// extern uchar B_RestartNo; // ZCL 2019.4.19  ������ţ������ӡ�۲�����
// extern uint C_Com3NoRcv;  // COM0 û�н����������ݼ����� ZCL 2017.5.30
// extern uc8 GPRS_ZhenTou[];
//// u8* DTURegisterNUM1;
//// u8* DTUEND;
// extern uchar *ptr, *ptr2, *ptr3; // ָ��
// extern uchar B_Com3Cmd8B;		 // ��ѯDTU�������� 2012.8.17
// extern uchar B_Com3Cmd8D;		 // ����DTU�������� 2012.8.24
// extern uchar B_Com3Cmd89;		 // ��������������ָ�� 2011.8.24
// extern uchar B_OtherCmd;		 // ��λ��ͨ��DTU������2������ָ��跢�͵�����2��
// extern uchar ZhuCeOkFLAG[];		 // ע��ɹ���־
// extern uchar ChannelNo;			 // Ӧ��ͨ��,�ĳ���ֵ
// extern char *tcp_ip;
// extern uint8_t lte_send_cmd(char *cmd, char *res, uint32_t timeOut, uint8_t retime);
// extern void Gprs_TX_Fill(uc8 *String, u8 Length);
// extern void Gprs_TX_Byte(u8 i);
// typedef void (*pFunction)(void); // ����һ���������͵Ĳ���

//// ��ʱ
//// u8 COM2_DATA;
// u8 LEDGPRS;
//// u8 PWR_LIGHT;

// void IWDG_Feed(void); // �������Ź�ι��
// void delay_ms(vu16 nCount);
// void SPI_FLASH_WriteEnable(void);
//// void SPI_FLASH_BufferWrite(void);
// void DTUIDInformation(void);
// void MakeInsertData(u8 *a, u16 aCount, u16 pos, u8 *b, u16 bCount);
// void SPI_FMRAM_BufferWrite(u8 *pBuffer, u16 WriteAddr, u16 NumByteToWrite);
// void UART1_Output_Datas(uint8_t S_Out[], uint8_t Counter);
// void UART2_Output_Datas(uint8_t S_Out[], uint8_t Counter);
// void UART3_Output_Datas(uint8_t S_Out[], uint16_t Counter);
// void usart3_send_string(unsigned char *str, unsigned short len);
// void lte_clear(void);
// void Fill_data(void);
// extern void LCD_DLY_ms(u32 Time);
// extern u16 CRC16(u8 *pCrcData, u8 CrcDataLen);
// extern void Com3_SlaveSend(void);
// void Com2_printf(char *fmt, ...)
//{
//	uint16_t txd_max;
//	va_list ap;
//	va_start(ap, fmt);
//	vsprintf((char *)Txd3Buffer, fmt, ap); // vsprintf   sprintf
//	va_end(ap);
//	// DMA��ʽ����
//	//	while(DMA1_Channel4->CNDTR!=0);	//�ȴ�ͨ��4�������
//	//	UART_DMA_Enable(DMA1_Channel4,strlen((const char*)Txd3Buffer)); //����DMA�������ݷ���

//	// �жϷ�ʽ����
//	//	Cw_Txd3 = 0;
//	//	Cw_Txd3Max=strlen((const char*)Txd3Buffer);
//	//	USART_SendData(USART3, Txd3Buffer[Cw_Txd3++]);
//	//	USART_ITConfig(USART3, USART_IT_TC, ENABLE); // ��ʼ����.

//	// ������ʽ����
//	txd_max = strlen((const char *)Txd3Buffer);
//	UART3_Output_Datas(Txd3Buffer, txd_max);
//}

// unsigned int atoi(unsigned char *s, unsigned char sz)
//{
//	unsigned int i;
//	unsigned int n = 0;
//	unsigned char *p;
//	p = s;
//	if (sz != 0) // ���Ȳ�����0
//	{
//		for (i = 0; i < sz; i++)
//		{
//			if (*(p + i) >= '0' && *(p + i) <= '9')
//			{
//				n = 10 * n + (*(p + i) - '0');
//			}
//		}
//		return n;
//	}
//	else // ���򷵻�Ϊ��
//		return NULL;
// }

// void reboot(void)
//{
//	pFunction Jump_To_Application;
//	u32 JumpAddress;
//	JumpAddress = *(vu32 *)(0x00000004);
//	Jump_To_Application = (pFunction)JumpAddress;
//	Jump_To_Application();
// }

// void HongDianProtocolRegister(void)
//{
//	u16 Writelink1OnlineReportTime, Writelink2OnlineReportTime, Writelink3OnlineReportTime, Writelink4OnlineReportTime; // �ϵ��¼������ʱ��
//	if (S_HongDianAcesse == 1)																							// ע��
//	{
//		F_PowerOnRegister = 0;								   // ��ʼע��
//		Writelink1OnlineReportTime = Pw_link1OnlineReportTime; // �ϵ��¼������ʱ��
//		Writelink2OnlineReportTime = Pw_link2OnlineReportTime; // �ϵ��¼������ʱ��
//		Writelink3OnlineReportTime = Pw_link3OnlineReportTime; // �ϵ��¼������ʱ��
//		Writelink4OnlineReportTime = Pw_link4OnlineReportTime; // �ϵ��¼������ʱ��
//		Pw_link1OnlineReportTime = 1;						   // �ϵ������ȷ���������������
//		Pw_link2OnlineReportTime = 1;						   // �ϵ������ȷ���������������
//		Pw_link3OnlineReportTime = 1;						   // �ϵ������ȷ���������������
//		Pw_link4OnlineReportTime = 1;						   // �ϵ������ȷ���������������
//		while (!F_PowerOnRegister)
//			sim7600ce_SendHeartbeat(); // ����ģ������
//		if (F_PowerOnRegister == 1)
//		{
//			Pw_link1OnlineReportTime = Writelink1OnlineReportTime;
//			Pw_link2OnlineReportTime = Writelink2OnlineReportTime;
//			Pw_link3OnlineReportTime = Writelink3OnlineReportTime;
//			Pw_link4OnlineReportTime = Writelink4OnlineReportTime;
//			S_HongDianAcesse = 0;
//		}
//	}
// }

// void DTUhbeaterrPackets(void) // DTU�豸����������������ͬʱҲ������ע�������
//{
//	u8 DTURegisterNUM1[4] = {0x7B, 0x01, 0x00, 0x16};
//	u8 DTUEND[1] = {0x7B};

//	DTUIDInformation();										 // ��ȡDTUID��������Ϣ
//	MakeInsertData(p_TmpBuf, 22, 0, DTURegisterNUM1, 4);	 // dtuע��ǰ����
//	MakeInsertData(p_TmpBuf, 22, 4, DTUIDBuf, 11);			 // dtuע��ID����
//	MakeInsertData(p_TmpBuf, 22, 15, (u8 *)ModularADDIP, 4); // dtuע�᱾��IP��ַ����
//	MakeInsertData(p_TmpBuf, 22, 19, local_port_Byte, 2);	 // dtuע�᱾�ض˿ڲ���
//	MakeInsertData(p_TmpBuf, 22, 21, DTUEND, 1);			 // dtuע��󲿷�
//}

// void MakeInsertData(u8 *a, u16 aCount, u16 pos, u8 *b, u16 bCount)
//{
//	u16 i, j = 0;
//	u8 *p_Tmpa;
//	u8 *p_Tmpb;
//	p_Tmpa = a;
//	p_Tmpb = b;
//	for (i = pos; i <= (bCount + pos - 1); i++)
//	{
//		*(p_Tmpa + i) = *(p_Tmpb + j);
//		j++;
//		if (i > aCount)
//			break;
//	}
//	if (j >= bCount)
//		j = 0;
// }

// void DTUIDInformation(void)
//{
//	//	u8 i;
//	//	u16 hex,n=1;
//	//	hex=Pw_DTUIDNUM1;//��һ��IDת��Ϊ�ַ�
//	//	for (i=0;i<=3;i++)
//	//	{
//	//		n=n*10;
//	//		DTUIDBuf[i]=sim7600ce_hex2chr(hex/(10000/n));
//	//		hex=hex%(10000/n);
//	//	}
//	//	n=1;
//	//	hex=Pw_DTUIDNUM2;//�ڶ���IDת��Ϊ�ַ�
//	//	for (i=4;i<=7;i++)
//	//	{
//	//		n=n*10;
//	//		DTUIDBuf[i]=sim7600ce_hex2chr(hex/(10000/n));
//	//		hex=hex%(10000/n);
//	//	}
//	//	n=1;
//	//	hex=Pw_DTUIDNUM3;//������IDת��Ϊ�ַ�
//	//	for (i=8;i<=10;i++)
//	//	{
//	//		n=n*10;
//	//		DTUIDBuf[i]=sim7600ce_hex2chr(hex/(1000/n));
//	//		hex=hex%(1000/n);
//	//	}

//	uchar m;
//	for (m = 0; m < 11; m++)
//		DTUIDBuf[m] = GprsPar[DtuNoBase + m];
//}

//// GPIO��ʼ��
////
// void SIM7600CE_GPIO_Init(void)
//{
//	//	GPIO_InitTypeDef GPIO_InitStructure;
//	//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE); // PORTBʱ��ʹ��

//	////	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;	// pb12 �ϵ����ţ�Pb13 ��λ����
//	//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 					// pb12�ߵ�ƽ>500ms���ϵ磻pb13�ߵ�ƽ>200ms����λ
//	//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	////	GPIO_ResetBits(GPIOB, GPIO_Pin_11);
//	//	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
//	//	GPIO_ResetBits(GPIOB, GPIO_Pin_13);

//	//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // Pa11 DTS
//	//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	//	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
//}

//// usmart֧�ֲ���
//// ���յ���ATָ��Ӧ�����ݷ��ظ����Դ���
//// mode:0,������USART3_RX_STA;
////      1,���� USART3_RX_STA;
// void sim_at_response(u8 mode)
//{
//	//	if (USART3_RX_STA & 0X8000) //���յ�һ��������
//	if (usart3_rev_finish) // YLS 2023.02.08
//	{
//		//		Rcv3Buffer[USART3_RX_STA & 0X7FFF] = 0; //��ӽ�����
//		printf("%s", Rcv3Buffer);
//		if (mode)
//		{
//			USART3_RX_STA = 0;
//			Cw_Rcv3 = 0; // YLS 2023.02.08
//			usart3_rev_finish = 0;
//		}
//	}
// }

//// sim7600ce���������,�����յ���Ӧ��
//// str:�ڴ���Ӧ����
//// ����ֵ:0,û�еõ��ڴ���Ӧ����
//// ����,�ڴ�Ӧ������λ��(str��λ��)
// u8 *sim7600ce_check_cmd(u8 *str)
//{
//	char *strx = 0;
//	//	if (USART3_RX_STA & 0X8000)
//	if (usart3_rev_finish) // YLS 2023.02.08
//	{
//		//		Rcv3Buffer[USART3_RX_STA & 0X7FFF] = 0; //��ӽ�����
//		Rcv3Buffer[Cw_BakRcv3] = 0; // ��ӽ�����
//		strx = strstr((const char *)Rcv3Buffer, (char *)str);
//	}
//	return (u8 *)strx;
// }
//// ��sim7600ce��������
////  cmd:���͵������ַ���
////  ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
////  waittime:�ȴ�ʱ��(��λ:10ms)
//// ����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
////        1,����ʧ��
//// u8 sim7600ce_send_cmd(u8 *cmd, u8 *ack, u16 waittime)
////{
////	u8 res = 0;
////	u8 C_Init_PwrLed_changeTime = 0; //��ʼ��ʱPWRLED�ƽ���ʱ��

////	USART3_RX_STA = 0;
////	C_Init_PwrLed_changeTime = 0; //��ʼ��ʱPWRLED�ƽ���ʱ���塰0��

//////	if ((u32)cmd <= 0XFF)
//////	{
//////		while (DMA1_Channel4->CNDTR != 0)
//////			; //�ȴ��������
//////		USART3->DR = (u32)cmd;
//////	}
//////	else
//////		Com2_printf("%s\r\n", cmd); //�������

//////	if (waittime == 1100) // 11s����ش�������(����ɨ��ģʽ)
//////	{
//////		Scan_Wtime = 11; //��Ҫ��ʱ��ʱ��
//////						 //		 TIM4_SetARR(9999);//����1S��ʱ�ж�
//////		TIM4->ARR = 9999;
//////	}
////
////	printf("%s",cmd);
////	Com2_printf("%s\r\n", cmd); //�������
////
////	if (ack && waittime) //��Ҫ�ȴ�Ӧ��
////	{
////		while (--waittime) //�ȴ�����ʱ
////		{
////			if (F_RunAgain == 1)
////			{
////				COM2_DATA = 0;
////				LEDGPRS = 0;
////				C_Init_PwrLed_changeTime++;
////				if (C_Init_PwrLed_changeTime >= 40)	//����
////				{
////					PWR_LED_TOGGLE; //��Դ����˸	����
////					C_Init_PwrLed_changeTime = 0;
////				}
////			}
////			else
////			{
////				C_Init_PwrLed_changeTime++;
////				if (C_Init_PwrLed_changeTime >= 100)	//����
////				{
//////					LEDGPRS = ~LEDGPRS; //��Դ����˸
////					PWR_LED_TOGGLE; //��Դ����˸	����
////					C_Init_PwrLed_changeTime = 0;
////					T_GPRSLEDTime = 100;
////					C_GPRSLEDTime = 0;
////				}
////			}
////			if (BakRcv2Count)
////				COM2_DATA = 0;			// COM�н������ݣ��ص�
////			delay_ms(10);				//ÿ�εȴ�����ʱ������ʱ10MS
////			IWDG_Feed();				//ι��
//////			if (USART3_RX_STA & 0X8000) //���յ��ڴ���Ӧ����
////			if(usart3_rev_finish)							                  // ������ڽ������
////			{
////				if (sim7600ce_check_cmd(ack))
////				{
////					printf("receive:%s",Rcv3Buffer);
////					lte_clear();										                // ��ջ���
////					return 0;
//////					break; //�õ���Ч����
////				}

////				USART3_RX_STA = 0;
////			}
////			lte_clear();										                // ��ջ���
////		}
////		if (waittime == 0) //��ʱι��
////		{
////			IWDG_Feed(); //ι��
////			res = 1;
////		}
////	}
////	return res;
////}

//// ��sim7600ce��������
////  cmd:���͵������ַ���
////  ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
////  waittime:�ȴ�ʱ��(��λ:10ms)
//// ����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
////        1,����ʧ��
// u8 sim7600ce_send_cmd(u8 *cmd, u8 *ack, u16 waittime)
//{
//	//	uint8_t i = 0;
//	uint32_t timeout;
//	u8 C_Init_PwrLed_changeTime = 0; // ��ʼ��ʱPWRLED�ƽ���ʱ��
//	//	u8 *cmd1;
//	u8 cmd1[50] = {0};
//	//	for(i=0;i<retime;i++)
//	//	{

//	lte_clear();
//	timeout = waittime;
//	sprintf((char *)cmd1, "%s\r\n", cmd);
//	printf("%s", cmd1);

//	usart3_send_string((unsigned char *)cmd1, strlen((const char *)cmd1));
//	while (timeout--)
//	{
//		if (F_RunAgain == 1)
//		{
//			COM2_DATA = 0;
//			LEDGPRS = 0;
//			C_Init_PwrLed_changeTime++;
//			if (C_Init_PwrLed_changeTime >= 40) // ����
//			{
//				PWR_LED_TOGGLE; // ��Դ����˸	����
//				C_Init_PwrLed_changeTime = 0;
//			}
//		}
//		else
//		{
//			C_Init_PwrLed_changeTime++;
//			if (C_Init_PwrLed_changeTime >= 100) // ����
//			{
//				//					LEDGPRS = ~LEDGPRS; //��Դ����˸
//				PWR_LED_TOGGLE; // ��Դ����˸	����
//				C_Init_PwrLed_changeTime = 0;
//				T_GPRSLEDTime = 100;
//				C_GPRSLEDTime = 0;
//			}
//		}
//		if (usart3_rev_finish) // ������ڽ������
//		{
//			if (strstr((const char *)Rcv3Buffer, (char *)ack) != NULL) // ����������ؼ���
//			{
//				printf("receive:%s", Rcv3Buffer);
//				//					lte_clear();										                // ��ջ���
//				usart3_rev_finish = 0;
//				return 0;
//			}
//		}
//		//			delay_ms(10);
//		LCD_DLY_ms(10);
//	}
//	//		lte_clear();
//	//  }
//	return 1;
//}

///**
// * @brief  lte��ջ���
// * @param  ��
// * @retval ��
// * @note   ��
// */
// void lte_clear(void)
//{
//	memset(Rcv3Buffer, 0, 200);
//	//	Cw_Rcv3 = 0;
//}

///**
// * @brief  ����2�����ַ���
// * @param  	str��Ҫ���͵�����
// *				    len�����ݳ���
// * @retval ��
// * @note   ��
// */
// void usart3_send_string(unsigned char *str, unsigned short len)
//{
//	uint8_t count = 0;
//	for (; count < len; count++)
//	{
//		USART_SendData(USART3, *str++); // ��������
//		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
//			; // �ȴ��������
//	}
//}

//// ����SIM7600ce�������ݣ���������ģʽ��ʹ�ã�
////  request:�ڴ����������ַ���
////  waittimg:�ȴ�ʱ��(��λ��10ms)
//// ����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
////        1,����ʧ��
// u8 sim7600ce_wait_request(u8 *request, u16 waittime)
//{
//	u8 res = 1;

//	if (request && waittime)
//	{
//		while (--waittime)
//		{
//			//			if (USART3_RX_STA & 0x8000) //���յ��ڴ���Ӧ����
//			if (usart3_rev_finish) // YLS 2023.02.08
//			{
//				if (sim7600ce_check_cmd(request))
//					break; // �õ���Ч����
//				USART3_RX_STA = 0;
//			}
//		}
//		if (waittime == 0)
//			res = 0;
//	}
//	return res;
//}

//// ��1���ַ�ת��Ϊ16��������
////  chr:�ַ�,0~9/A~F/a~F
//// ����ֵ:chr��Ӧ��16������ֵ
// u8 sim7600ce_chr2hex(u8 chr)
//{
//	if (chr >= '0' && chr <= '9')
//		return chr - '0';
//	if (chr >= 'A' && chr <= 'F')
//		return (chr - 'A' + 10);
//	if (chr >= 'a' && chr <= 'f')
//		return (chr - 'a' + 10);
//	return 0;
// }
//// ��1��16��������ת��Ϊ�ַ�
////  hex:16��������,0~15;
//// ����ֵ:�ַ�
// u8 sim7600ce_hex2chr(u8 hex)
//{
//	if (hex <= 9)
//		return hex + '0';
//	if (hex >= 10 && hex <= 15)
//		return (hex - 10 + 'A');
//	return '0';
// }

//// NTP����ͬ��ʱ��//����ͬ��ʱ��
// void ntp_update(void)
//{
//	sim7600ce_send_cmd("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", "OK", 200); // ���ó��س���1
//	sim7600ce_send_cmd("AT+SAPBR=3,1,\"APN\",\"CMNET\"", "OK", 200);
//	sim7600ce_send_cmd("AT+SAPBR=1,1", 0, 200); // ����һ��GPRS������
//	delay_ms(5);
//	sim7600ce_send_cmd("AT+CNTPCID=1", "OK", 200);				   // ����CNTPʹ�õ�CID
//	sim7600ce_send_cmd("AT+CNTP=\"202.120.2.101\",32", "OK", 200); // ����NTP�������ͱ���ʱ��(32ʱ�� ʱ����׼ȷ)
//	sim7600ce_send_cmd("AT+CNTP", "+CNTP: 1", 600);				   // ͬ������ʱ��
// }

//// Modular(ģ��)��Ϣ��ʾ(������,ģ������,��Ʒ���к�)
// void sim7600ce_Modular_Information(void)
//{
//	u8 *p, *p1;
//	p = (u8 *)mymalloc(50); // ����50���ڴ�
//	USART3_RX_STA = 0;
//	if (sim7600ce_send_cmd("AT+CGMI", "OK", 200) == 0) // ��ѯ����������
//	{
//		p1 = (u8 *)strstr((const char *)(Rcv3Buffer + 2), "\r\n");
//		p1[0] = 0; // ���������
//		sprintf((char *)p, "������:%s", Rcv3Buffer + 2);
//		printf("%s", p); //
//		USART3_RX_STA = 0;
//		lte_clear();
//	}
//	if (sim7600ce_send_cmd("AT+CGMM", "OK", 200) == 0) // ��ѯģ������
//	{
//		p1 = (u8 *)strstr((const char *)(Rcv3Buffer + 2), "\r\n");
//		p1[0] = 0; // ���������
//		sprintf((char *)p, "ģ���ͺ�:%s", Rcv3Buffer + 2);
//		printf("%s", p); //
//		USART3_RX_STA = 0;
//		lte_clear();
//	}
//	if (sim7600ce_send_cmd("AT+CGSN", "OK", 200) == 0) // ��ѯ��Ʒ���к�
//	{
//		p1 = (u8 *)strstr((const char *)(Rcv3Buffer + 2), "\r\n"); // ���һس�
//		p1[0] = 0;												   // ���������
//		sprintf((char *)p, "���к�:%s", Rcv3Buffer + 2);
//		printf("%s", p); //
//		USART3_RX_STA = 0;
//		lte_clear();
//	}
//	myfree(p);
// }
//// GSM��Ϣ��ʾ(��ѯSIM��,��Ӫ������,�ź�����,��ص���,����ʱ��)
//// ����ֵ:0,����
//// ����,�������
// u8 sim7600ce_GSM_Information(void)
//{
//	u8 *p, *p1, *p2;
//	u8 res = 0;
//	u8 i;
//	p = (u8 *)mymalloc(50); // ����50���ڴ�
//	USART3_RX_STA = 0;

//	if (sim7600ce_send_cmd("AT+COPS?", "OK", 200) == 0) // ��ѯ��Ӫ������
//	{
//		p1 = (u8 *)strstr((const char *)(Rcv3Buffer), "\"");
//		if (p1) // ����Ч����
//		{
//			p2 = (u8 *)strstr((const char *)(p1 + 1), "\"");
//			if (p2)
//				p2[0] = 0; // ���������
//						   //  sprintf((char*)p,"��Ӫ��:%s",p1+1);
//						   // printf("%s",p);//
//		}
//	}
//	else
//		res |= 1 << 1;

//	lte_clear();
//	if (res == 0)
//	{
//		if (sim7600ce_send_cmd("AT+CSQ", "+CSQ:", 200) == 0) // ��ѯ�ź�����
//		{
//			p1 = (u8 *)strstr((const char *)(Rcv3Buffer), ":");
//			p2 = (u8 *)strstr((const char *)(p1), ",");
//			if (p2)
//				p2[0] = 0; // ���������
//			//			sprintf((char *)CSQBuf, "%s", p1 + 2); //�����ź����� ��EDGE��
//			if (p1)
//			{
//				for (i = 0; i < 2; i++)
//				{
//					CSQBuf[i] = *(p1 + 2);
//					p1++;
//				}
//				w_GprsCSQ = atoi(CSQBuf, 2);
//				//			sprintf((char*)p,"�ź�����:%s",p1+2);
//				//				printf("%s",p);//
//				printf("�ź�����:%d", w_GprsCSQ);
//				USART3_RX_STA = 0;
//				lte_clear();
//			}
//			lte_clear();
//		}
//		else
//			res |= 1 << 2;
//	}
//	else
//	{
//		printf("%s", "�Ҳ����ƶ����磡����"); //
//	}
//	myfree(p);
//	lte_clear();
//	return res;
//}
//// ��Ϣ��ʾ(��ǰ���ӵ�����)
// void sim7600ce_Netlink_Information(void)
//{
//	u8 CheckNet[] = {"AT+CPSI?"}; // ��ѯģ���������硣
//	USART3_RX_STA = 0;

//	while (DMA1_Channel4->CNDTR != 0)
//		;						   // �ȴ�ͨ��4�������
//	Com2_printf("%s\r\n", CheckNet); // ��ѯģ����������
//	delay_ms(50);
//	//	if (USART3_RX_STA & 0X8000) //���յ�Ӧ����
//	if (usart3_rev_finish) // YLS 2023.02.08
//	{
//		if (sim7600ce_check_cmd("+CPSI: LTE")) // 4G
//		{

//			F_NowNetworkSystem = 4; // ѡ��4G��ʽ
//			if (Pw_SupportDataReceipt == 1)
//				printf("3.��ǰ����4G����\r\n"); // ��ǰ����4G����
//		}
//		else if (sim7600ce_check_cmd("+CPSI: GSM")) // 2G
//		{

//			F_NowNetworkSystem = 2; // ѡ��2G��ʽ
//			if (Pw_SupportDataReceipt == 1)
//				printf("3.��ǰ����2G����\r\n"); // ��ǰ����2G����
//		}
//		else if (sim7600ce_check_cmd("+CPSI: CDMA")) // 2G����
//		{

//			F_NowNetworkSystem = 2; // ѡ��2G��ʽ
//			if (Pw_SupportDataReceipt == 1)
//				printf("3.��ǰ���ڵ���2G����\r\n"); // ��ǰ����2G����
//		}
//		else if (sim7600ce_check_cmd("+CPSI: WCDMA")) // 3G
//		{

//			F_NowNetworkSystem = 3; // ѡ��3G��ʽ
//			if (Pw_SupportDataReceipt == 1)
//				printf("3.��ǰ����3G����\r\n"); // ��ǰ����3G����
//		}
//		else if (sim7600ce_check_cmd("+CPSI: CDMA2000")) // 3G����
//		{

//			F_NowNetworkSystem = 3; // ѡ��3G��ʽ����
//			if (Pw_SupportDataReceipt == 1)
//				printf("3.��ǰ���ڵ���3G����\r\n"); // ��ǰ����3G����	����
//		}
//		else if (sim7600ce_check_cmd("+CPSI: NO SERVICE")) // û�з���
//		{
//			F_NowNetworkSystem = 0; // û�з���
//			if (Pw_SupportDataReceipt == 1)
//				printf("3.û�в�ѯ�����磬������\r\n");
//		}
//		else
//		{
//			F_NowNetworkSystem = 1; // ��������
//		}
//		USART3_RX_STA = 0;
//	}
//}
//// �ر�����
// u8 close_net(void)
//{
//	// �ر�����
//	if (sim7600ce_send_cmd("AT+NETCLOSE", "+NETCLOSE:", 300) == 0)
//	{
//		if (Pw_SupportDataReceipt == 1)
//			printf("* �ر����� *\r\n");
//	}
//	else
//	{
//		// printf("�ر�����ʧ��\r\n");
//		return 0;
//	}
//	return 1;
// }
//// �˳�����ģʽ
// void exit_data_mode(void)
//{
//	unsigned char cmd[] = {"+++"}; // ���͡�+++�����˳�͸������ģʽ��

//	delay_ms(1000);
//	IWDG_Feed();			  // ι��
//	PWR_LED_TOGGLE;			  // ��Դ����˸
//	Com2_printf("%s\r\n", cmd); // �������com1
//	delay_ms(1000);
//	IWDG_Feed(); // ι��
//}

//// ѡ������
// u8 select_net(void)
//{
//	u8 i;
//	u8 DianXinAPN[] = {"CTNET"};				 // ���Ž����
//	unsigned char CheckNet[] = {"AT+CPSI?"};	 // ��ѯģ���������硣
//	unsigned char CheckNetSet[] = {"AT+CGATT?"}; // ��ѯģ���ǹ�ѡ��ɹ���

//	// �Զ�ѡ���л����粿��
//	if (sim7600ce_send_cmd(NetAuto, "OK", 200) == 0)
//	{
//		if (Pw_SupportDataReceipt == 1)
//			printf("2.�Զ�ѡ������ɹ�\r\n");
//	}
//	else
//	{
//		if (Pw_SupportDataReceipt == 1)
//			printf("2.�Զ�ѡ������ʧ��\r\n");
//		return 0;
//	}
//	for (i = 0; i <= 30; i++) // ѡ������󣬻�ע�����磬����ʱ��᳤һ��,�ɸ���ʵ������޸�15s
//	{
//		PWR_LED_TOGGLE; // ��Դ����˸
//		delay_ms(500);
//		IWDG_Feed(); // ι��
//	}
//	// ��������Ƿ�ѡ��ɹ�
//	for (i = 0; i <= 20; i++) // �鿴20��
//	{
//		if (sim7600ce_send_cmd(CheckNetSet, "+CGATT: 1", 200) == 0)
//		{
//			if (Pw_SupportDataReceipt == 1)
//				printf("2.GPRS��������ɹ�\r\n");
//			i = 100; // ֻҪ�ɹ�ѡ������������鿴��
//			Select_NetNum = 0;
//		}
//		else
//		{
//			if (Pw_SupportDataReceipt == 1)
//				printf("2.GPRS��������ʧ��\r\n");

//			PWR_LED_TOGGLE; // ��Դ����˸
//			delay_ms(500);
//			IWDG_Feed(); // ι��
//						 //	Select_NetNum++;
//			if (i >= 20)
//			{
//				Select_NetNum = 0;
//				return 0;
//			}
//		}
//	}

//	// ��ѯģ���������磨ѡ����Ϻ󣬼��ģ���Ƿ�������ѡ��������ϣ�
//	USART3_RX_STA = 0;
//	//	while (DMA1_Channel4->CNDTR != 0)
//	//		;						   //�ȴ�ͨ��4�������
//	Com2_printf("%s\r\n", CheckNet); // ��ѯģ����������

//	for (i = 0; i <= 16; i++) // �ȴ����ؽ��
//	{
//		PWR_LED_TOGGLE; // ��Դ����˸
//		delay_ms(500);
//		IWDG_Feed(); // ι��
//		//		if (USART3_RX_STA & 0X8000)
//		if (usart3_rev_finish) // YLS 2023.02.08
//			break;			   // ���յ�Ӧ����,���ڵȴ�����ѭ��
//	}
//	//	if (USART3_RX_STA & 0X8000) //���յ�Ӧ����
//	if (usart3_rev_finish) // YLS 2023.02.08
//	{
//		if (sim7600ce_check_cmd("+CPSI: LTE")) // 4G
//		{

//			F_NowNetworkSystem = 4; // ѡ��4G��ʽ
//			if (Pw_SupportDataReceipt == 1)
//				printf("3.��ǰ����4G����\r\n"); // ��ǰ����4G����
//		}
//		else if (sim7600ce_check_cmd("+CPSI: GSM")) // 2G
//		{

//			F_NowNetworkSystem = 2; // ѡ��2G��ʽ
//			if (Pw_SupportDataReceipt == 1)
//				printf("3.��ǰ����2G����\r\n"); // ��ǰ����2G����
//		}
//		else if (sim7600ce_check_cmd("+CPSI: CDMA")) // 2G����
//		{

//			F_NowNetworkSystem = 2; // ѡ��2G��ʽ
//			if (Pw_SupportDataReceipt == 1)
//				printf("3.��ǰ���ڵ���2G����\r\n"); // ��ǰ����2G����
//		}
//		else if (sim7600ce_check_cmd("+CPSI: WCDMA")) // 3G
//		{

//			F_NowNetworkSystem = 3; // ѡ��3G��ʽ
//			if (Pw_SupportDataReceipt == 1)
//				printf("3.��ǰ����3G����\r\n"); // ��ǰ����3G����
//		}
//		else if (sim7600ce_check_cmd("+CPSI: CDMA2000")) // 3G����
//		{
//			F_NowNetworkSystem = 3; // ѡ��3G��ʽ����
//			if (Pw_SupportDataReceipt == 1)
//				printf("3.��ǰ���ڵ���3G����\r\n"); // ��ǰ����3G����	����
//		}
//		else if (sim7600ce_check_cmd("+CPSI: NO SERVICE")) // û�з���
//		{
//			F_NowNetworkSystem = 0; // û�з���
//			if (Pw_SupportDataReceipt == 1)
//				printf("3.û�в�ѯ�����磬������\r\n");

//			USART3_RX_STA = 0;
//			Cw_Rcv3 = 0;
//			usart3_rev_finish = 0; // YLS 2023.02.08
//			return 0;
//		}
//		else
//		{
//			F_NowNetworkSystem = 1; // ��������
//		}
//		USART3_RX_STA = 0;
//		Cw_Rcv3 = 0;
//		usart3_rev_finish = 0; // YLS 2023.02.08
//	}

//	// ������ŵĽ����Ͳ�����ע���ˣ�ֱ������
//	if (strstr((const char *)DianXinAPN, (const char *)APN_Name) == 0) // ˵�����ǵ��ŵĽ����
//	{
//		// ��ѯ�Ƿ�ע��ɹ�    ��ע��ɹ���ʾ�Ѿ�������Ҫѡ������磩
//		if (sim7600ce_send_cmd("AT+CREG?", "+CREG: 0,1", 1000) == 0)
//		{
//			if (Pw_SupportDataReceipt == 1)
//				printf("4.����ע��ɹ�\r\n");
//		}
//		else
//		{
//			if (Pw_SupportDataReceipt == 1)
//				printf("4.����ע��ʧ��\r\n");

//			return 0;
//		}
//	}
//	return 1;
//}
//// ���ģ��IP������1Ϊ��ȡIP���󡣷���0Ϊ��ȡIP��ȷ
////  cmd���͵��ַ���ָ��
////  waittime�ȵ�ʱ��
//// ����� 2018.08.23 12��16
// u8 sim7600ce_send_Modular_IP(u8 *cmd, u16 waittime) // ���ģ��IP
//{
//	u8 i, res = 0, num = 0, num1 = 0;
//	u8 cmd1[50] = {0};
//	USART3_RX_STA = 0;

//	sprintf((char *)cmd1, "%s\r", cmd);
//	printf("%s", cmd1);
//	usart3_send_string((unsigned char *)cmd1, strlen((const char *)cmd1));

//	//	Com2_printf("%s\r\n", cmd); //
//	if (waittime) // ��Ҫ�ȴ�Ӧ��
//	{
//		while (--waittime) // �ȴ�����ʱ
//		{
//			delay_ms(10); // ÿ�εȴ�����ʱ������ʱ10MS
//			IWDG_Feed();  // ι��
//			//			if (USART3_RX_STA & 0X8000)
//			if (usart3_rev_finish) // YLS 2023.02.08
//			{
//				if (sim7600ce_check_cmd("ERROR"))
//				{
//					USART3_RX_STA = 0; // ��鷢�ַ��ص����ݳ��ִ���
//					Cw_Rcv3 = 0;
//					usart3_rev_finish = 0; // YLS 2023.02.08
//				}
//				else // �õ���Ч����
//				{
//					for (i = 2; i <= 25; i++) // �ѵõ�����ȷ���ݽ���ת������
//					{
//						if ((Rcv3Buffer[i] == 0x2E) || (Rcv3Buffer[i] == 0x0D))
//						{
//							ModularADDIP[num1] = (u8)atoi(&Rcv3Buffer[i - num], num); //	ת����Ӧ��IP��ַ
//							if (num1 == 3)
//								break;
//							num1++; // ���㵱ǰ�㣨��.�����ĸ���
//							num = 0;
//						}
//						if (Rcv3Buffer[i] != 0x2E)
//							num++;
//					}
//					break; // ת����������whileѭ��
//				}
//			}
//		}
//		if (waittime == 0)
//			res = 1; // �ȴ�ʱ�䵽���ش���
//	}
//	return res; // ���δ����ȴ�ʱ�䷵����ȷ
//}
//// SIM7600ce��ʼ��
////
// void SIM7600ce_Init(void)
//{
//	u8 Set_GPRS_APN[60];
//	u8 i = 0, j;

//	//	for (j = 0; j <= 14; j++)
//	//		APN_Name[j] = Pw_ParLst_GPRS[j + 8]; //�����
//	for (j = 0; j <= 24; j++)
//		APN_Name[j] = GprsPar[j + AccessPointBase]; // �����

//	//	for (j = 0; j <= 24; j++)
//	//		domain_name[j] = Pw_ParLst_GPRS[j + 107]; //����1 ����
//	//	for (j = 0; j <= 24; j++)
//	//		domain_name1[j] = Pw_ParLst_GPRS[j + 132]; //����2 ����
//	//	for (j = 0; j <= 24; j++)
//	//		domain_name2[j] = Pw_ParLst_GPRS[j + 157]; //����3 ����
//	//	for (j = 0; j <= 24; j++)
//	//		domain_name3[j] = Pw_ParLst_GPRS[j + 182]; //����4 ����

//	for (j = 0; j <= 30; j++)
//		domain_name[j] = GprsPar[j + DomainName0Base]; // ����1 ����
//	for (j = 0; j <= 30; j++)
//		domain_name1[j] = GprsPar[j + DomainName1Base]; // ����2 ����
//	for (j = 0; j <= 30; j++)
//		domain_name2[j] = GprsPar[j + DomainName2Base]; // ����3 ����
//	for (j = 0; j <= 30; j++)
//		domain_name3[j] = GprsPar[j + DomainName3Base]; // ����4 ����

//	while (F_RunAgain == 1) // ��������
//	{
//		//		GPIO_SetBits(GPIOB, GPIO_Pin_13); //PB13 ��λ���ţ���ʱ����2.5��
//		RESET_4G_ON;

//		COM2_DATA = 0;
//		LEDGPRS = 0;
//		delay_ms(400);
//		PWR_LED_TOGGLE; // ��Դ����˸
//		delay_ms(400);
//		PWR_LED_TOGGLE; // ��Դ����˸
//		delay_ms(400);
//		PWR_LED_TOGGLE; // ��Դ����˸
//		delay_ms(400);
//		PWR_LED_TOGGLE; // ��Դ����˸
//		delay_ms(400);
//		PWR_LED_TOGGLE; // ��Դ����˸
//		delay_ms(400);
//		PWR_LED_TOGGLE; // ��Դ����˸
//		delay_ms(400);
//		PWR_LED_TOGGLE; // ��Դ����˸
//		IWDG_Feed();	// ι��

//		//		GPIO_ResetBits(GPIOB, GPIO_Pin_13); //	REST
//		RESET_4G_OFF;
//		for (i = 0; i <= 10; i++) // ��ʱ�ȴ�ģ���ȶ�
//		{
//			PWR_LED_TOGGLE; // ��Դ����˸
//			delay_ms(400);
//			IWDG_Feed(); // ι��
//		}

//		exit_data_mode(); // �˳�͸��ģʽ

//		while (sim7600ce_send_cmd("AT", "OK", 30)) // ����Ƿ�Ӧ��ATָ��
//		{
//			PWR_LED_TOGGLE; // ��Դ����˸
//			delay_ms(200);	//"������������ģ��..."
//			IWDG_Feed();	// ι��
//			PowerOnOFFWaitNum++;
//			if (PowerOnOFFWaitNum >= 25) // 5��
//			{
//				//				reboot(); //ϵͳ����	YLS
//			}
//		}
//		PowerOnOFFWaitNum = 0;
//		close_net(); // �ر����磬���������Ϊ�����Ѿ����������´�����ʧ��
//		delay_ms(5);

//		if (Pw_SupportDataReceipt == 1) // ���԰汾��Ϣ
//		{
//			printf("ģ��оƬ�汾��Ϣ:\r\n");
//			sim7600ce_send_cmd("AT+SIMCOMATI", "OK", 200); // ��ѯģ��汾��Ϣ
//			sim_at_response(0);							   // ��ӡ��Ϣ
//			printf("\r\n");
//		}
//		/****************************************************************************
//		����������������ָ�
//		������ָ�����˼�Ǵ��й��ƶ���FDDƵ�Σ���Ϊ����֮ǰ�й��ƶ�û��FDD�����գ�
//		ֻ��TDD�����գ�������ָ�����ģ��������4GƵ�Ρ�
//		�����  2020.12.04
//		***************************************************************************/
//		if (Pw_LastPacketIdleTime == 400) // ����������ʱ�������ж��Ƿ�����4G��=400��ʾ����
//		{
//			sim7600ce_send_cmd("AT+CNBP=,0x000001E000000095", "OK", 200);											 // ����ʵ������������4G��ʹ�ø�ָ���������Ҹ�   2020.05.14 	# ����TDDƵ��
//			sim7600ce_send_cmd("AT+CNV=/nv/item_files/modem/lte/rrc/cap/diff_fdd_tdd_fgi_enable,0,01,1", "OK", 200); // ����ʵ�����������ӣ�ʹ�ø�ָ���������Ҹ�   2020.05.14
//		}
//		// 5G���Ӻ�������BAND41Ƶ�㣬����ģ��Ҫ����band41Ƶ��
//		//		sim7600ce_send_cmd("AT+CNV=/nv/item_files/modem/lte/rrc/skip_unsupported_b41_meas,0,01,1", "OK", 200);				  //                        ���Ҹ�   2022.04.06
//		//		sim7600ce_send_cmd("AT+CNV=/nv/item_files/modem/lte/rrc/skip_unsupported_band_meas,0,0000000000010000,1", "OK", 200); //  ���Ҹ�   2022.04.06
//		// ��������ָ���Ǽ����ƶ��²���5G����ʱ������band41Ƶ��,���ӵ�������.//2022.4.06

//		sim7600ce_send_cmd("ATE0", "OK", 200); // �رջ���

//		// ��ѯSIM���Ƿ���λ
//		if (S_check_status == 0)
//		{
//			if (sim7600ce_send_cmd("AT+CPIN?", "+CPIN: READY", 200) == 0) // ��ѯSIM���Ƿ���λ
//			{
//				if (Pw_SupportDataReceipt == 1)
//					printf("1.�����ɹ�\r\n");
//				S_check_status = 1;
//			}
//			else
//			{
//				if (Pw_SupportDataReceipt == 1)
//					printf("1.����ʧ��\r\n");
//				S_check_status = 0;
//			}
//		}

//		sprintf((char *)Set_GPRS_APN, "AT+CGDCONT=1,\"IP\",\"%s\"", APN_Name);
//		// ����APN

//		if (sim7600ce_send_cmd(Set_GPRS_APN, "OK", 200) == 0)
//		{
//			if (Pw_SupportDataReceipt == 1)
//				printf("����APN�ɹ�\r\n");
//		}
//		else
//		{
//			if (Pw_SupportDataReceipt == 1)
//				printf("APN���ó���\r\n");
//			S_check_status = 0;
//		}
//		// ѡ������
//		if (S_check_status == 1)
//		{
//			if (!select_net()) // ��һ��ѡ�����·�󣬿�ֱ��ȥ���ú���
//			{
//				// printf("��ʼ��������������\r\n");
//				S_check_status = 0;
//			}
//			else // ��ʼ������ͨ�������Խ��й��ܲ���
//			{
//				PWR_LIGHT = 1; // ��ʼ������ͨ�������Խ��й�����������
//				S_check_status = 2;
//			}
//		}
//		// �ò���ǰ���Ѿ���������ˣ����Բ�����Ҳ�У�������û����ɾ����
//		sprintf((char *)Set_GPRS_APN, "AT+CGDCONT=1,\"IP\",\"%s\"", APN_Name);
//		// ����APN
//		if (S_check_status == 2)
//		{
//			if (sim7600ce_send_cmd(Set_GPRS_APN, "OK", 200) == 0)
//			{
//				if (Pw_SupportDataReceipt == 1)
//					printf("5.����APN\r\n");
//				S_check_status = 3;
//			}
//			else
//			{
//				if (Pw_SupportDataReceipt == 1)
//					printf("5.APN���ó���\r\n");
//				S_check_status = 0;
//			}
//		}
//		// ������������
//		if (S_check_status == 3)
//		{
//			if (sim7600ce_send_cmd("AT+CSOCKSETPN=1", "OK", 200) == 0)
//			{
//				if (Pw_SupportDataReceipt == 1)
//					printf("6.�����ƶ�����\r\n");
//				S_check_status = 4;
//			}
//			else
//			{
//				if (Pw_SupportDataReceipt == 1)
//					printf("6.�����ƶ�����ʧ��\r\n");
//				S_check_status = 0;
//			}
//		}
//		// ����Ϊ��͸��ģʽ
//		if (S_check_status == 4)
//		{
//			if (sim7600ce_send_cmd("AT+CIPMODE?", "+CIPMODE: 0", 200) == 0) // ���ﲻ��ֱ������͸��ģʽ��������Ϊ�ظ����õ���ERROR
//			{
//				if (Pw_SupportDataReceipt == 1)
//					printf("7.����Ϊ��͸��ģʽ\r\n");
//				S_check_status = 5;
//			}
//			else
//			{
//				if (Pw_SupportDataReceipt == 1)
//					printf("7.����Ϊ��͸��ģʽ\r\n");

//				sim7600ce_send_cmd("AT+CIPMODE=0", "OK", 200);
//				S_check_status = 5;
//			}
//		}
//		// ������
//		if (S_check_status == 5)
//		{
//			if (sim7600ce_send_cmd("AT+NETOPEN", "+NETOPEN: 0", 5000) == 0) // �����ظ������������ERROR
//			{
//				if (Pw_SupportDataReceipt == 1)
//					printf("8.������\r\n");
//				S_check_status = 6;
//			}
//			else
//			{
//				if (Pw_SupportDataReceipt == 1)
//					printf("8.������ʧ��\r\n");
//				S_check_status = 0;
//			}
//		}
//		// ��ȡ����IP��ַ
//		if (S_check_status == 6)
//		{
//			if (sim7600ce_send_Modular_IP("AT+IPADDR\r\n", 500) == 0)
//			//			if (sim7600ce_send_cmd("AT+IPADDR", "+IPADDR:", 200) == 0)
//			{
//				if (Pw_SupportDataReceipt == 1)
//					printf("9.���IP��ַ\r\n");
//				// �ɹ���ȡ����IP��ַ
//				S_check_status = 7; // ��ʼ�����̺�
//			}
//			else
//			{
//				// ��ȡʧ�ܱ���IP��ַ
//				S_check_status = 0; // ��ʼ�����̺��塰0��
//			}
//		}
//		// ������������
//		if (S_check_status == 7)
//		{
//			sim7600ce_GSM_Information();
//			if (GPRSNetConfig(4) == 0) // ��ʼ����
//			{
//				F_RunAgain = 0;					  // ����������
//				F_RunCheckModular = 0;			  // ���й����м��ģ���־��0
//				S_check_status = 0;				  // ��ʼ�����̺��塰0��
//				S_HongDianAcesse = 1;			  // ����ע�����
//				hbeaterrcnt = 0;				  // ���¼������
//				SendDataErrorNum = 0;			  // ���¼�ⷢ������Ӧ���źŴ���
//				connectsta = 0;					  // ����״̬��0
//				PWR_LIGHT = 1;					  // ��
//				sim7600ce_connectstaModularNum(); // ���ģ����Ҫ���ӵ�IP����
//				HongDianProtocolRegister();		  // ���е���������ϵ��Ƚ���ע��
//			}
//			else
//			{
//				// ��������ʧ����������
//				S_check_status = 0; // ��ʼ�����̺��塰0��
//			}
//		}
//	}
//}

//// ������������
// const u8 *modetbl[2] = {"UDP", "TCP"}; // ����ģʽ//modetbl:1:TCP����;0,UDP����)
// u8 GPRSNetConfig(u8 link_num)
//{
//	u8 localPortBuf[5] = {0}; // ���ض˿ڻ���
//	u8 ipbuf[15];			  // IP����
//	u8 portBuf[5] = {0};	  // Զ�˶˿ڻ���
//	u8 *p2, *p3;
//	u8 i = 0, k = 0;

//	// if(sim7600ce_send_cmd("AT+CGATT=1","OK",500))return 3;					//����GPRSҵ��

//	if (sim7600ce_send_cmd("AT+CIPHEAD=1\r\n", "OK", 500))
//		return 4; // ���ý���������ʾIPͷ(�����ж�������Դ)

//	if (link_num == 0 || link_num >= 4)
//	{
//		// ��������
//		//	if (domain_name[0] + domain_name[1] + domain_name[2] + domain_name[3] != 0) //��һ·�����������ݲ���Ϊ0�����򲻽��н���
//		if (GprsPar[DomainName0Base] + GprsPar[DomainName0Base + 1] + GprsPar[DomainName0Base + 2] + GprsPar[DomainName0Base + 3] != 0) // ��һ·�����������ݲ���Ϊ0�����򲻽��н���
//		{
//			//		sprintf((char *)p_TmpBuf, "AT+CDNSGIP =\"%s\"", domain_name); //��������Ϣ������Ŀռ�p_TmpBuf
//			sprintf((char *)p_TmpBuf, "AT+CDNSGIP =\"%s\"", (char *)GprsPar[DomainName0Base]); // ��������Ϣ������Ŀռ�p_TmpBuf
//			if (sim7600ce_send_cmd(p_TmpBuf, "+CDNSGIP: 1", 500))
//				; // �ɹ�����DNS
//			//		p2 = (u8 *)strstr((const char *)Rcv3Buffer, (const char *)domain_name); //���ս�����IP��Ϣ
//			p2 = (u8 *)strstr((const char *)Rcv3Buffer, (const char *)GprsPar[DomainName0Base]); // ���ս�����IP��Ϣ
//			if (p2)																				 // ������ȷ
//			{
//				p3 = (u8 *)strstr((const char *)p2, ","); //
//				sprintf((char *)p_TmpBuf, "%s", p3 + 2);  // �ѽ�����IP��Ϣ��p_TmpBuf
//				for (i = 0; i <= 3; i++)				  // ת��Ϊ16������
//				{
//					if (p_TmpBuf[i] == 0x2E)
//					{
//						Pw_Link1IP1 = atoi(&p_TmpBuf[0], i); // ת��Ϊ16����
//						GprsPar[Ip0Base] = Pw_Link1IP1;		 // YLS 2022.12.27
//						k = i + 1;							 // IP��һλ��ֵ
//						break;								 //
//					}
//				}
//				for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
//				{
//					if (p_TmpBuf[i] == 0x2E)
//					{
//						Pw_Link1IP2 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
//						GprsPar[Ip0Base + 1] = Pw_Link1IP2;		 // YLS 2022.12.27
//						k = i + 1;								 // IP��һλ��ֵ
//						break;									 //
//					}
//				}
//				for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
//				{
//					if (p_TmpBuf[i] == 0x2E)
//					{
//						Pw_Link1IP3 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
//						GprsPar[Ip0Base + 2] = Pw_Link1IP3;		 // YLS 2022.12.27
//						k = i + 1;								 // IP��һλ��ֵ
//						break;									 //
//					}
//				}
//				for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
//				{
//					if (p_TmpBuf[i] == 0x22)
//					{
//						Pw_Link1IP4 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
//						GprsPar[Ip0Base + 3] = Pw_Link1IP4;		 // YLS 2022.12.27
//						k = 0;									 //
//						break;									 //
//					}
//				}
//			}
//		}

//		//		if ((Pw_Link1IP1 + Pw_Link1IP2 + Pw_Link1IP3 + Pw_Link1IP4) != 0) //ֻ�е�һ·����ʵ��͸������
//		if ((GprsPar[Ip0Base] + GprsPar[Ip0Base + 1] + GprsPar[Ip0Base + 2] + GprsPar[Ip0Base + 3]) != 0) // ֻ�е�һ·����ʵ��͸������
//		{
//			Pw_Link1localPort = (GprsPar[LoclaPort0Base] << 8) + GprsPar[LoclaPort0Base + 1]; // YLS 2022.12.27
//			sprintf((char *)localPortBuf, "%d", Pw_Link1localPort);							  // ��һ·���ӱ��ض˿���ת��Ϊascii��
//			//			sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link1IP1, Pw_Link1IP2, Pw_Link1IP3, Pw_Link1IP4); //���IP
//			sprintf((char *)ipbuf, "%d.%d.%d.%d", GprsPar[Ip0Base], GprsPar[Ip0Base + 1], GprsPar[Ip0Base + 2], GprsPar[Ip0Base + 3]); // ���IP
//			Pw_Link1Port = (GprsPar[Port0Base] << 8) + GprsPar[Port0Base + 1];														   // YLS 2022.12.27
//			sprintf((char *)portBuf, "%d", Pw_Link1Port);																			   // ��һ·����Զ�˶˿���ת��Ϊascii��
//			//		if (Pw_Link1TCP_UDPType == 0)															   //��һ·ΪUDP
//			if (GprsPar[LinkTCPUDP0Base] == 0) // ��һ·ΪUDP
//			{
//				sprintf((char *)p_TmpBuf, "AT+CIPOPEN=0,\"%s\",\"%s\",%s,%s", modetbl[(u8)GprsPar[LinkTCPUDP0Base]], ipbuf, portBuf, localPortBuf); // ��������Ϣ������Ŀռ�p_TmpBuf
//				// if(sim7600ce_send_cmd(p_TmpBuf,"CONNECT",500))	return 9;		//�����1·���ӣ�͸�������ʱ���ø������ã�
//				if (sim7600ce_send_cmd(p_TmpBuf, "+CIPOPEN: 0,0", 500))
//				{
//					CGD0_LinkConnectOK[0] = FALSE;
//					return 9; // �����1·���ӣ����ã�͸�������ʱ��ע�͵���
//				}
//				else
//				{
//					CGD0_LinkConnectOK[0] = TRUE;
//				}
//			}
//			else ////��һ·ΪTCP Client
//			{
//				sprintf((char *)p_TmpBuf, "AT+CIPOPEN=0,\"%s\",\"%s\",%s", modetbl[(u8)GprsPar[LinkTCPUDP0Base]], ipbuf, portBuf); // ��������Ϣ������Ŀռ�p_TmpBuf
//				//				printf("AT+CIPOPEN=0,\"%s\",\"%s\",%s", modetbl[(u8)GprsPar[LinkTCPUDP0Base]], ipbuf, portBuf); //��������Ϣ������Ŀռ�p_TmpBuf
//				// if(sim7600ce_send_cmd(p_TmpBuf,"CONNECT",500))	return 9;		//�����1·���ӣ�͸�������ʱ���ø������ã�
//				if (sim7600ce_send_cmd(p_TmpBuf, "+CIPOPEN: 0,0", 500))
//				{
//					CGD0_LinkConnectOK[0] = FALSE;
//					return 9; // �����1·���ӣ����ã�͸�������ʱ��ע�͵���
//				}
//				else
//				{
//					CGD0_LinkConnectOK[0] = TRUE;
//				}
//			}
//		}
//	}

//	if (link_num == 1 || link_num >= 4)
//	{
//		//	if (domain_name1[0] + domain_name1[1] + domain_name1[2] + domain_name1[3] != 0) //�ڶ�·�����������ݲ���Ϊ0�����򲻽��н���
//		if (GprsPar[DomainName1Base] + GprsPar[DomainName1Base + 1] + GprsPar[DomainName1Base + 2] + GprsPar[DomainName1Base + 3] != 0) // ��һ·�����������ݲ���Ϊ0�����򲻽��н���
//		{
//			//		sprintf((char *)p_TmpBuf, "AT+CDNSGIP =\"%s\"", domain_name1); //��������Ϣ������Ŀռ�p_TmpBuf
//			sprintf((char *)p_TmpBuf, "AT+CDNSGIP =\"%s\"", (char *)GprsPar[DomainName1Base]); // ��������Ϣ������Ŀռ�p_TmpBuf
//			if (sim7600ce_send_cmd(p_TmpBuf, "+CDNSGIP: 1", 500))
//				; // �ɹ�����DNS
//			//		p2 = (u8 *)strstr((const char *)Rcv3Buffer, (const char *)domain_name1); //���ս�����IP��Ϣ
//			p2 = (u8 *)strstr((const char *)Rcv3Buffer, (const char *)GprsPar[DomainName1Base]); // ���ս�����IP��Ϣ
//			if (p2)																				 // ������ȷ
//			{
//				p3 = (u8 *)strstr((const char *)p2, ","); //
//				sprintf((char *)p_TmpBuf, "%s", p3 + 2);  // �ѽ�����IP��Ϣ��p_TmpBuf
//				for (i = 0; i <= 3; i++)				  // ת��Ϊ16������
//				{
//					if (p_TmpBuf[i] == 0x2E)
//					{
//						Pw_Link2IP1 = atoi(&p_TmpBuf[0], i); // ת��Ϊ16����
//						GprsPar[Ip1Base] = Pw_Link2IP1;		 // YLS 2022.12.27
//						k = i + 1;							 // IP��һλ��ֵ
//						break;								 //
//					}
//				}
//				for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
//				{
//					if (p_TmpBuf[i] == 0x2E)
//					{
//						Pw_Link2IP2 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
//						GprsPar[Ip1Base + 1] = Pw_Link2IP2;		 // YLS 2022.12.27
//						k = i + 1;								 // IP��һλ��ֵ
//						break;									 //
//					}
//				}
//				for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
//				{
//					if (p_TmpBuf[i] == 0x2E)
//					{
//						Pw_Link2IP3 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
//						GprsPar[Ip1Base + 2] = Pw_Link2IP3;		 // YLS 2022.12.27
//						k = i + 1;								 // IP��һλ��ֵ
//						break;									 //
//					}
//				}
//				for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
//				{
//					if (p_TmpBuf[i] == 0x22)
//					{
//						Pw_Link2IP4 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
//						GprsPar[Ip1Base + 3] = Pw_Link2IP4;		 // YLS 2022.12.27
//						k = 0;									 //
//						break;									 //
//					}
//				}
//			}
//		}

//		//		if ((Pw_Link2IP1 + Pw_Link2IP2 + Pw_Link2IP3 + Pw_Link2IP4) != 0)
//		if (GprsPar[DomainName1Base] + GprsPar[DomainName1Base + 1] + GprsPar[DomainName1Base + 2] + GprsPar[DomainName1Base + 3] != 0)
//		{
//			Pw_Link2localPort = (GprsPar[LoclaPort1Base] << 8) + GprsPar[LoclaPort1Base + 1]; // YLS 2022.12.27
//			sprintf((char *)localPortBuf, "%d", Pw_Link2localPort);							  // �ڶ�·���ӱ��ض˿���ת��Ϊascii��
//			//			sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link2IP1, Pw_Link2IP2, Pw_Link2IP3, Pw_Link2IP4); //���IP
//			sprintf((char *)ipbuf, "%d.%d.%d.%d", GprsPar[Ip1Base], GprsPar[Ip1Base + 1], GprsPar[Ip1Base + 2], GprsPar[Ip1Base + 3]); // ���IP

//			Pw_Link2Port = (GprsPar[Port1Base] << 8) + GprsPar[Port1Base + 1]; // YLS 2022.12.27
//			sprintf((char *)portBuf, "%d", Pw_Link2Port);					   // ��һ·����Զ�˶˿���ת��Ϊascii��
//			//		if (Pw_Link2TCP_UDPType == 0)															   //�ڶ�·ΪUDP
//			if (GprsPar[LinkTCPUDP1Base] == 0) // �ڶ�·ΪUDP
//			{
//				sprintf((char *)p_TmpBuf, "AT+CIPOPEN=1,\"%s\",\"%s\",%s,%s", modetbl[(u8)GprsPar[LinkTCPUDP1Base]], ipbuf, portBuf, localPortBuf); // ��������Ϣ������Ŀռ�p_TmpBuf
//				if (sim7600ce_send_cmd(p_TmpBuf, "+CIPOPEN: 1,0", 500))
//				{
//					CGD0_LinkConnectOK[1] = FALSE;
//					return 10; // �����2·���ӣ����ã�
//				}
//				else
//				{
//					CGD0_LinkConnectOK[1] = TRUE;
//				}
//			}
//			else ////�ڶ�·ΪTCP
//			{
//				sprintf((char *)p_TmpBuf, "AT+CIPOPEN=1,\"%s\",\"%s\",%s", modetbl[(u8)GprsPar[LinkTCPUDP1Base]], ipbuf, portBuf); // ��������Ϣ������Ŀռ�p_TmpBuf
//				if (sim7600ce_send_cmd(p_TmpBuf, "+CIPOPEN: 1,0", 500))
//				{
//					CGD0_LinkConnectOK[1] = FALSE;
//					return 10; // �����2·���ӣ����ã�
//				}
//				else
//				{
//					CGD0_LinkConnectOK[1] = TRUE;
//				}
//			}
//		}
//	}

//	if (link_num == 2 || link_num >= 4)
//	{
//		//	if (domain_name2[0] + domain_name2[1] + domain_name2[2] + domain_name2[3] != 0) //����·�����������ݲ���Ϊ0�����򲻽��н���
//		if (GprsPar[DomainName2Base] + GprsPar[DomainName2Base + 1] + GprsPar[DomainName2Base + 2] + GprsPar[DomainName2Base + 3] != 0) // ����·�����������ݲ���Ϊ0�����򲻽��н���
//		{
//			//		sprintf((char *)p_TmpBuf, "AT+CDNSGIP =\"%s\"", domain_name2); //��������Ϣ������Ŀռ�p_TmpBuf
//			sprintf((char *)p_TmpBuf, "AT+CDNSGIP =\"%s\"", (char *)GprsPar[DomainName2Base]); // ��������Ϣ������Ŀռ�p_TmpBuf
//			if (sim7600ce_send_cmd(p_TmpBuf, "+CDNSGIP: 1", 500))
//				; // �ɹ�����DNS
//			//		p2 = (u8 *)strstr((const char *)Rcv3Buffer, (const char *)domain_name2); //���ս�����IP��Ϣ
//			p2 = (u8 *)strstr((const char *)Rcv3Buffer, (const char *)GprsPar[DomainName2Base]); // ���ս�����IP��Ϣ
//			if (p2)																				 // ������ȷ
//			{
//				p3 = (u8 *)strstr((const char *)p2, ","); //
//				sprintf((char *)p_TmpBuf, "%s", p3 + 2);  // �ѽ�����IP��Ϣ��p_TmpBuf
//				for (i = 0; i <= 3; i++)				  // ת��Ϊ16������
//				{
//					if (p_TmpBuf[i] == 0x2E)
//					{
//						Pw_Link3IP1 = atoi(&p_TmpBuf[0], i); // ת��Ϊ16����
//						GprsPar[Ip2Base] = Pw_Link3IP1;		 // YLS 2022.12.27
//						k = i + 1;							 // IP��һλ��ֵ
//						break;								 //
//					}
//				}
//				for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
//				{
//					if (p_TmpBuf[i] == 0x2E)
//					{
//						Pw_Link3IP2 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
//						GprsPar[Ip2Base + 1] = Pw_Link3IP2;		 // YLS 2022.12.27
//						k = i + 1;								 // IP��һλ��ֵ
//						break;									 //
//					}
//				}
//				for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
//				{
//					if (p_TmpBuf[i] == 0x2E)
//					{
//						Pw_Link3IP3 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
//						GprsPar[Ip2Base + 2] = Pw_Link3IP3;		 // YLS 2022.12.27
//						k = i + 1;								 // IP��һλ��ֵ
//						break;									 //
//					}
//				}
//				for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
//				{
//					if (p_TmpBuf[i] == 0x22)
//					{
//						Pw_Link3IP4 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
//						GprsPar[Ip2Base + 3] = Pw_Link3IP4;		 // YLS 2022.12.27
//						k = 0;									 //
//						break;									 //
//					}
//				}
//			}
//		}

//		//		if ((Pw_Link3IP1 + Pw_Link3IP2 + Pw_Link3IP3 + Pw_Link3IP4) != 0)
//		if (GprsPar[DomainName2Base] + GprsPar[DomainName2Base + 1] + GprsPar[DomainName2Base + 2] + GprsPar[DomainName2Base + 3] != 0)
//		{
//			Pw_Link3localPort = (GprsPar[LoclaPort2Base] << 8) + GprsPar[LoclaPort2Base + 1]; // YLS 2022.12.27
//			sprintf((char *)localPortBuf, "%d", Pw_Link3localPort);							  // ����·���ӱ��ض˿���ת��Ϊascii��
//			//			sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link3IP1, Pw_Link3IP2, Pw_Link3IP3, Pw_Link3IP4); //���IP
//			sprintf((char *)ipbuf, "%d.%d.%d.%d", GprsPar[Ip2Base], GprsPar[Ip2Base + 1], GprsPar[Ip2Base + 2], GprsPar[Ip2Base + 3]); // ���IP
//			Pw_Link3Port = (GprsPar[Port2Base] << 8) + GprsPar[Port2Base + 1];														   // YLS 2022.12.27
//			sprintf((char *)portBuf, "%d", Pw_Link3Port);																			   // ����·����Զ�˶˿���ת��Ϊascii��
//			if (Pw_Link3TCP_UDPType == 0)																							   // ����·ΪUDP
//			{
//				sprintf((char *)p_TmpBuf, "AT+CIPOPEN=2,\"%s\",\"%s\",%s,%s", modetbl[(u8)Pw_Link3TCP_UDPType], ipbuf, portBuf, localPortBuf); // ��������Ϣ������Ŀռ�p_TmpBuf
//				if (sim7600ce_send_cmd(p_TmpBuf, "+CIPOPEN: 2,0", 500))
//				{
//					CGD0_LinkConnectOK[2] = FALSE;
//					return 11; // �����3·���ӣ����ã�
//				}
//				else
//				{
//					CGD0_LinkConnectOK[2] = TRUE;
//				}
//			}
//			else ////����·ΪTCP
//			{
//				sprintf((char *)p_TmpBuf, "AT+CIPOPEN=2,\"%s\",\"%s\",%s", modetbl[(u8)Pw_Link3TCP_UDPType], ipbuf, portBuf); // ��������Ϣ������Ŀռ�p_TmpBuf
//				if (sim7600ce_send_cmd(p_TmpBuf, "+CIPOPEN: 2,0", 500))
//				{
//					CGD0_LinkConnectOK[2] = FALSE;
//					return 11; // �����3·���ӣ����ã�
//				}
//				else
//				{
//					CGD0_LinkConnectOK[2] = TRUE;
//				}
//			}
//		}
//	}

//	if (link_num == 3 || link_num >= 4)
//	{
//		//	if (domain_name3[0] + domain_name3[1] + domain_name3[2] + domain_name3[3] != 0) //����·�����������ݲ���Ϊ0�����򲻽��н���
//		if (GprsPar[DomainName3Base] + GprsPar[DomainName3Base + 1] + GprsPar[DomainName3Base + 2] + GprsPar[DomainName3Base + 3] != 0) // ��һ·�����������ݲ���Ϊ0�����򲻽��н���
//		{
//			//		sprintf((char *)p_TmpBuf, "AT+CDNSGIP =\"%s\"", domain_name3); //��������Ϣ������Ŀռ�p_TmpBuf
//			sprintf((char *)p_TmpBuf, "AT+CDNSGIP =\"%s\"", (char *)GprsPar[DomainName3Base]); // ��������Ϣ������Ŀռ�p_TmpBuf
//			if (sim7600ce_send_cmd(p_TmpBuf, "+CDNSGIP: 1", 500))
//				; // �ɹ�����DNS
//			//		p2 = (u8 *)strstr((const char *)Rcv3Buffer, (const char *)domain_name3); //���ս�����IP��Ϣ
//			p2 = (u8 *)strstr((const char *)Rcv3Buffer, (const char *)GprsPar[DomainName3Base]); // ���ս�����IP��Ϣ
//			if (p2)																				 // ������ȷ
//			{
//				p3 = (u8 *)strstr((const char *)p2, ","); //
//				sprintf((char *)p_TmpBuf, "%s", p3 + 2);  // �ѽ�����IP��Ϣ��p_TmpBuf
//				for (i = 0; i <= 3; i++)				  // ת��Ϊ16������
//				{
//					if (p_TmpBuf[i] == 0x2E)
//					{
//						Pw_Link4IP1 = atoi(&p_TmpBuf[0], i); // ת��Ϊ16����
//						GprsPar[Ip3Base] = Pw_Link4IP1;		 // YLS 2022.12.27
//						k = i + 1;							 // IP��һλ��ֵ
//						break;								 //
//					}
//				}
//				for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
//				{
//					if (p_TmpBuf[i] == 0x2E)
//					{
//						Pw_Link4IP2 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
//						GprsPar[Ip3Base + 1] = Pw_Link4IP2;		 // YLS 2022.12.27
//						k = i + 1;								 // IP��һλ��ֵ
//						break;									 //
//					}
//				}
//				for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
//				{
//					if (p_TmpBuf[i] == 0x2E)
//					{
//						Pw_Link4IP3 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
//						GprsPar[Ip3Base + 2] = Pw_Link4IP3;		 // YLS 2022.12.27
//						k = i + 1;								 // IP��һλ��ֵ
//						break;									 //
//					}
//				}
//				for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
//				{
//					if (p_TmpBuf[i] == 0x22)
//					{
//						Pw_Link4IP4 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
//						GprsPar[Ip3Base + 3] = Pw_Link4IP4;		 // YLS 2022.12.27
//						k = 0;									 //
//						break;									 //
//					}
//				}
//			}
//		}

//		//		if ((Pw_Link4IP1 + Pw_Link4IP2 + Pw_Link4IP3 + Pw_Link4IP4) != 0)
//		if (GprsPar[DomainName3Base] + GprsPar[DomainName3Base + 1] + GprsPar[DomainName3Base + 2] + GprsPar[DomainName3Base + 3] != 0)
//		{
//			Pw_Link4localPort = (GprsPar[LoclaPort3Base] << 8) + GprsPar[LoclaPort3Base + 1]; // YLS 2022.12.27
//			sprintf((char *)localPortBuf, "%d", Pw_Link4localPort);							  // ����·���ӱ��ض˿���ת��Ϊascii��
//			//			sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link4IP1, Pw_Link4IP2, Pw_Link4IP3, Pw_Link4IP4); //���IP
//			sprintf((char *)ipbuf, "%d.%d.%d.%d", GprsPar[Ip3Base], GprsPar[Ip3Base + 1], GprsPar[Ip3Base + 2], GprsPar[Ip3Base + 3]); // ���IP

//			Pw_Link4Port = (GprsPar[Port3Base] << 8) + GprsPar[Port3Base + 1]; // YLS 2022.12.27
//			sprintf((char *)portBuf, "%d", Pw_Link4Port);					   // ����·����Զ�˶˿���ת��Ϊascii��
//			if (Pw_Link4TCP_UDPType == 0)									   // ����·ΪUDP
//			{
//				sprintf((char *)p_TmpBuf, "AT+CIPOPEN=3,\"%s\",\"%s\",%s,%s", modetbl[(u8)Pw_Link4TCP_UDPType], ipbuf, portBuf, localPortBuf); // ��������Ϣ������Ŀռ�p_TmpBuf
//				if (sim7600ce_send_cmd(p_TmpBuf, "+CIPOPEN: 3,0", 500))
//				{
//					CGD0_LinkConnectOK[3] = FALSE;
//					return 12; // �����4·���ӣ����ã�
//				}
//				else
//				{
//					CGD0_LinkConnectOK[3] = TRUE;
//				}
//			}
//			else ////����·ΪTCP
//			{
//				sprintf((char *)p_TmpBuf, "AT+CIPOPEN=3,\"%s\",\"%s\",%s", modetbl[(u8)Pw_Link4TCP_UDPType], ipbuf, portBuf); // ��������Ϣ������Ŀռ�p_TmpBuf
//				if (sim7600ce_send_cmd(p_TmpBuf, "+CIPOPEN: 3,0", 500))
//				{
//					CGD0_LinkConnectOK[3] = FALSE;
//					return 12; // �����4·���ӣ����ã�
//				}
//				else
//				{
//					CGD0_LinkConnectOK[3] = TRUE;
//				}
//			}
//		}
//	}

//	return 0;
//	//
//}

//// SIM7600ce���й�����ÿ��һ��ʱ�������� ��������粻����������ģ��
////
// void sim7600ce_NetworkCheck(void)
//{
//	//	u8 res = 0;

//	//	if (F_RunAgain == 0 && T_NetworkCheck != SClkSecond) // ���й�����ÿ��һ��ʱ��������
//	//	{
//	//		T_NetworkCheck = SClkSecond; //
//	//		C_NetworkCheck++;
//	//		if (C_NetworkCheck > Pw_Link1NetWorkCheckTime) // ���й�����ÿ��20��������
//	//		{
//	//			T_NetworkCheck = 100;
//	//			C_NetworkCheck = 0;

//	//			if (sim7600ce_send_cmd("AT+CPIN?", "+CPIN: READY", 200))
//	//				res |= 1 << 0; // ��ѯSIM���Ƿ���λ

//	//			if (sim7600ce_send_cmd("AT+COPS?", "OK", 200))
//	//				res |= 1 << 1; // ��ѯ��Ӫ������
//	//			if (res != 0)
//	//			{
//	//				F_RunAgain = 1;		   // ��������ģ��
//	//				F_RunCheckModular = 1; // ���й����м��ģ���־
//	//			}
//	//		}
//	//	}
//	//	else if (F_RunAgain == 1 && F_RunCheckModular == 1)
//	//	{
//	//		SIM7600ce_Init(); // sim7600ce��ʼ��
//	//	}
//	if (F_RunAgain == 1 && F_RunCheckModular == 1)
//	{
//		SIM7600ce_Init(); // sim7600ce��ʼ��
//	}
//}
//// sim7600ce GPRS���ݷ��������
//// tcp/udp���ݷ��������
//// ����������,��ά������
//// SendDateBuf:���͵����ݵ�BUF���ַ���
//// SendBufLen:���͵����ݳ���
//// Star_OR_hex:���͵���16�����������ַ�����=1��ʮ����������=0���ַ���
//// �ر�˵���������ͣ������п��԰���0x03��0x1A��0x1B
//// ����� 2019.05.14  10��49
// void sim7600ceGPRSUpLoaded(u8 *SendDateBuf, u16 SendBufLen, u8 Star_OR_hex)
//{
//	u8 ipbuf[15];			   // IP����
//	u8 portBuf[5] = {0};	   // Զ�˶˿ڻ���
//	u8 p_SendTmpBuf[50] = {0}; // ����
//	u8 F_SendData = 0;		   // �������ݱ�־

//	if (hbeaterrcnt <= 8 && !F_GPRSSendFinish) // ����<=8��������ʱ����û�з�����ɱ�־
//	{
//		if (F_AcklinkNum == 1)
//		{
//			sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link1IP1, Pw_Link1IP2, Pw_Link1IP3, Pw_Link1IP4); // ���IP
//			sprintf((char *)portBuf, "%d", Pw_Link1Port);											   // ��һ·����Զ�˶˿���ת��Ϊascii��
//			if ((Pw_Link1TCP_UDPType == 0) && (F_UDP_OR_TCP == 0))									   // ��һ·ΪUDP
//			{
//				sprintf((char *)p_SendTmpBuf, "AT+CIPSEND=0,%d,\"%s\",%s", SendBufLen, ipbuf, portBuf); // ��������Ϣ������Ŀռ�p_LinkTmpBuf
//				if (sim7600ce_send_cmd(p_SendTmpBuf, ">", 200) == 0)
//					F_SendData = 1; // �����1·����
//			}
//			else if ((Pw_Link1TCP_UDPType == 1) && (F_UDP_OR_TCP == 1)) ////��һ·ΪTCP Client
//			{
//				sprintf((char *)p_SendTmpBuf, "AT+CIPSEND=0,%d", SendBufLen);
//				if (sim7600ce_send_cmd(p_SendTmpBuf, ">", 200) == 0)
//					F_SendData = 1; // �����1·����
//			}

//			if (F_SendData == 1) // ��������
//			{
//				if (!F_DTUSendRegister)
//					COM2_DATA = 1; // ���Ϳ�ʼ����ָʾ��(����ע���)
//				if (Star_OR_hex)   //=1��ʮ����������=0���ַ���
//				{
//					memcpy(Txd3Buffer, SendDateBuf, SendBufLen);
//					//					while(DMA1_Channel4->CNDTR!=0);	//�ȴ��������
//					//					UART_DMA_Enable(DMA1_Channel4,SendBufLen); //���ͳ�ȥ
//					UART3_Output_Datas(Txd3Buffer, SendBufLen); // YLS 2023.02.10
//				}
//				else
//					Com2_printf("%s\r\n", SendDateBuf); // ����
//				delay_ms(20);						  // ��ʱ��Ҫ��
//				F_SendData = 0;						  // ���ͱ�־��0
//				// �����ȷ��Ͳ���Ҫ0X1A,"OK"���ߡ�<Ctrl+Z>��
//				//	if(sim7600ce_send_cmd((u8*)0X1A,"OK",1000)==0)//����1·�Ƿ����ɹ���Ϊ��������ݷ��ͳɹ�!�������ݵ�Զ�˷���, CTRL+Z (0x1a)����
//				COM2_DATA = 0; // ���ͽ����ر�ָʾ��
//							   //	else //�������ݷ���ʧ��!
//				COM2_DATA = 0; // ���ͽ����ر�ָʾ��

//				Link1SendOK = 1;		  // ��1·���ͳɹ���
//				C_SendHeartbeatTime0 = 0; // ������������
//				C_SendHeartbeatTime = 0;  // ������������
//			}
//			else // ���ݷ���ʧ��
//			{
//				SendDataErrorNum++; // ����8�Σ�ϵͳ����
//				Link1SendOK = 0;	// ��1·����ʧ�ܡ�
//			}
//		}
//		if (F_AcklinkNum == 2)
//		{
//			delay_ms(10);																			   // ��ʱ��Ҫ��
//			sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link2IP1, Pw_Link2IP2, Pw_Link2IP3, Pw_Link2IP4); // ���IP
//			sprintf((char *)portBuf, "%d", Pw_Link2Port);											   // ��2·����Զ�˶˿���ת��Ϊascii��
//			if ((Pw_Link2TCP_UDPType == 0) && (F_UDP_OR_TCP == 0))									   // ��2·ΪUDP
//			{
//				sprintf((char *)p_SendTmpBuf, "AT+CIPSEND=1,%d,\"%s\",%s", SendBufLen, ipbuf, portBuf); // ��������Ϣ������Ŀռ�p_LinkTmpBuf
//				if (sim7600ce_send_cmd(p_SendTmpBuf, ">", 200) == 0)
//					F_SendData = 1; // �����2·����
//			}
//			else if ((Pw_Link2TCP_UDPType == 1) && (F_UDP_OR_TCP == 1)) // ��2·ΪTCP Client
//			{
//				sprintf((char *)p_SendTmpBuf, "AT+CIPSEND=1,%d", SendBufLen);
//				if (sim7600ce_send_cmd(p_SendTmpBuf, ">", 200) == 0)
//					F_SendData = 1; // �����2·����
//			}
//			if (F_SendData == 1) // ��������
//			{
//				COM2_DATA = 1; // ���Ϳ�ʼ����ָʾ��
//				if (Star_OR_hex)
//				{

//					memcpy(Txd3Buffer, SendDateBuf, SendBufLen);

//					//					while(DMA1_Channel4->CNDTR!=0);	//�ȴ��������
//					//					UART_DMA_Enable(DMA1_Channel4,SendBufLen); //���ͳ�ȥ
//					UART3_Output_Datas(Txd3Buffer, SendBufLen); // YLS 2023.02.10
//				}
//				else
//					Com2_printf("%s\r\n", SendDateBuf); // ��������

//				delay_ms(20);	// ��ʱ��Ҫ��
//				F_SendData = 0; // ���ͱ�־��0
//				// �����ȷ��Ͳ���Ҫ0X1A,"OK"
//				//	if(sim7600ce_send_cmd((u8*)0X1A,"OK",1000)==0)//����2·�Ƿ����ɹ���Ϊ��������ݷ��ͳɹ�!�������ݵ�Զ�˷���, CTRL+Z (0x1a)����
//				COM2_DATA = 0; // ���ͽ����ر�ָʾ��
//							   //	else //�������ݷ���ʧ��!
//				COM2_DATA = 0; // ���ͽ����ر�ָʾ��

//				Link2SendOK = 1;		  // ��2·���ͳɹ���
//				C_SendHeartbeatTime0 = 0; // ������������
//				C_SendHeartbeatTime = 0;  // ������������
//			}
//			else // ���ݷ���ʧ��
//			{
//				SendDataErrorNum++; // ����8�Σ�ϵͳ����
//				Link2SendOK = 0;	// ��2·����ʧ�ܡ�
//			}
//		}
//		if (F_AcklinkNum == 3)
//		{
//			delay_ms(10);																			   // ��ʱ��Ҫ��
//			sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link3IP1, Pw_Link3IP2, Pw_Link3IP3, Pw_Link3IP4); // ���IP
//			sprintf((char *)portBuf, "%d", Pw_Link3Port);											   // ��3·����Զ�˶˿���ת��Ϊascii��
//			if ((Pw_Link3TCP_UDPType == 0) && (F_UDP_OR_TCP == 0))									   // ��3·ΪUDP
//			{
//				sprintf((char *)p_SendTmpBuf, "AT+CIPSEND=2,%d,\"%s\",%s", SendBufLen, ipbuf, portBuf); // ��������Ϣ������Ŀռ�p_LinkTmpBuf
//				if (sim7600ce_send_cmd(p_SendTmpBuf, ">", 200) == 0)
//					F_SendData = 1; // �����3·����
//			}
//			else if ((Pw_Link3TCP_UDPType == 1) && (F_UDP_OR_TCP == 1)) // ��3·ΪTCP Client
//			{
//				sprintf((char *)p_SendTmpBuf, "AT+CIPSEND=2,%d", SendBufLen);
//				if (sim7600ce_send_cmd(p_SendTmpBuf, ">", 200) == 0)
//					F_SendData = 1; // �����3·����
//			}
//			if (F_SendData == 1) // ��������
//			{
//				COM2_DATA = 1; // ���Ϳ�ʼ����ָʾ��
//				if (Star_OR_hex)
//				{
//					memcpy(Txd3Buffer, SendDateBuf, SendBufLen);

//					//					while(DMA1_Channel4->CNDTR!=0);	//�ȴ��������
//					//					UART_DMA_Enable(DMA1_Channel4,SendBufLen); //���ͳ�ȥ
//					UART3_Output_Datas(Txd3Buffer, SendBufLen); // YLS 2023.02.10
//				}
//				else
//					Com2_printf("%s\r\n", SendDateBuf); // ����������

//				delay_ms(20);	// ��ʱ��Ҫ��
//				F_SendData = 0; // ���ͱ�־��0
//				// �����ȷ��Ͳ���Ҫ0X1A,"OK"
//				//  if(sim7600ce_send_cmd((u8*)0X1A,"OK",1000)==0)//����3·�Ƿ����ɹ���Ϊ��������ݷ��ͳɹ�!�������ݵ�Զ�˷���, CTRL+Z (0x1a)����
//				COM2_DATA = 0; // ���ͽ����ر�ָʾ��
//				// else //�������ݷ���ʧ��!
//				COM2_DATA = 0; // ���ͽ����ر�ָʾ��

//				Link3SendOK = 1;		  // ��3·���ͳɹ���
//				C_SendHeartbeatTime0 = 0; // ������������
//				C_SendHeartbeatTime = 0;  // ������������
//			}
//			else // ���ݷ���ʧ��
//			{
//				SendDataErrorNum++; // ����8�Σ�ϵͳ����
//				Link3SendOK = 0;	// ��3·����ʧ�ܡ�
//			}
//		}
//		if (F_AcklinkNum == 4)
//		{
//			delay_ms(10);																			   // ��ʱ��Ҫ��
//			sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link4IP1, Pw_Link4IP2, Pw_Link4IP3, Pw_Link4IP4); // ���IP
//			sprintf((char *)portBuf, "%d", Pw_Link4Port);											   // ��4·����Զ�˶˿���ת��Ϊascii��
//			if ((Pw_Link4TCP_UDPType == 0) && (F_UDP_OR_TCP == 0))									   // ��4·ΪUDP
//			{
//				sprintf((char *)p_SendTmpBuf, "AT+CIPSEND=3,%d,\"%s\",%s", SendBufLen, ipbuf, portBuf); // ��������Ϣ������Ŀռ�p_LinkTmpBuf
//				if (sim7600ce_send_cmd(p_SendTmpBuf, ">", 200) == 0)
//					F_SendData = 1; // �����4·����
//			}
//			else if ((Pw_Link4TCP_UDPType == 1) && (F_UDP_OR_TCP == 1)) // ��4·ΪTCP Client
//			{
//				sprintf((char *)p_SendTmpBuf, "AT+CIPSEND=3,%d", SendBufLen);
//				if (sim7600ce_send_cmd(p_SendTmpBuf, ">", 200) == 0)
//					F_SendData = 1; // �����4·����
//			}
//			if (F_SendData == 1) // ��������
//			{
//				COM2_DATA = 1; // ���Ϳ�ʼ����ָʾ��
//				if (Star_OR_hex)
//				{
//					memcpy(Txd3Buffer, SendDateBuf, SendBufLen);

//					//					while(DMA1_Channel4->CNDTR!=0);	//�ȴ��������
//					//					UART_DMA_Enable(DMA1_Channel4,SendBufLen); //���ͳ�ȥ
//					UART3_Output_Datas(Txd3Buffer, SendBufLen); // YLS 2023.02.10
//				}
//				else
//					Com2_printf("%s\r\n", SendDateBuf); // ��������

//				delay_ms(20);	// ��ʱ��Ҫ��
//				F_SendData = 0; // ���ͱ�־��0
//				// �����ȷ��Ͳ���Ҫ0X1A,"OK"
//				//  if(sim7600ce_send_cmd((u8*)0X1A,"OK",1000)==0)//����4·�Ƿ����ɹ���Ϊ��������ݷ��ͳɹ�!�������ݵ�Զ�˷���, CTRL+Z (0x1a)����
//				COM2_DATA = 0; // ���ͽ����ر�ָʾ��
//				// else //�������ݷ���ʧ��!
//				COM2_DATA = 0; // ���ͽ����ر�ָʾ��

//				Link4SendOK = 1;		  // ��4·���ͳɹ���
//				C_SendHeartbeatTime0 = 0; // ������������
//				C_SendHeartbeatTime = 0;  // ������������
//			}
//			else // ���ݷ���ʧ��
//			{
//				SendDataErrorNum++; // ����8�Σ�ϵͳ����
//				Link4SendOK = 0;	// ��4·����ʧ�ܡ�
//			}
//		}
//		if (Link1SendOK + Link2SendOK + Link3SendOK + Link4SendOK == ConnectIPNum) // ������Ҫ���ӵĶ����ͳɹ���
//		{
//			if (SendDataErrorNum != 0)
//				SendDataErrorNum--;
//		}
//		F_GPRSSendFinish = 1; // ������ɣ�׼���´η���
//	}
//}
//// GPRSģ������ʧ�ܺ���������
// void sim7600ce_ReconnectNet(void)
//{
//	if (((hbeaterrcnt > 4) || (SendDataErrorNum > 4)) && (T_AgainLinkTime != SClk10Ms)) // ���ݷ��ʹ���Ӧ��10��,��������8������û����ȷ���ͳɹ�,��200MS��������
//	{
//		T_AgainLinkTime = SClk10Ms; // ��ʱ�������ݵ�ʱ��
//		C_AgainLinkTime++;
//		if (C_AgainLinkTime > Pw_link1ReconnectTimeBetwen) // 2S
//		{
//			T_AgainLinkTime = 100;
//			C_AgainLinkTime = 0;

//			// �����ж���,��������8������û����ȷ���ͳɹ�,����������
//			F_RunAgain = 1;		   // ��������ģ��
//			F_RunCheckModular = 1; // ���й����м��ģ���־

//			connectsta = 0;	 // ����״̬��0
//			hbeaterrcnt = 0; // ����������0
//							 //			reboot();		 //ϵͳ����
//		}
//	}
//}

//// GPRSģ�鷢����������
// void sim7600ce_SendHeartbeat(void)
//{
//	u8 ipbuf[15];																					// IP����
//	u8 portBuf[5] = {0};																			// Զ�˶˿ڻ���
//	u8 p_LinkTmpBuf[50] = {0};																		// ����
//	u8 F_HeartData = 0;																				// ���������ݱ�־
//	u16 local_port;																					// ����ip�˿��ݴ�
//	u32 AllTotalTime;																				// ��ʱ��
//	u16 link1OnlineReportTime, link2OnlineReportTime, link3OnlineReportTime, link4OnlineReportTime; // ����ʱ��

//	if (!usart3_rev_flag && connectsta == 0 && T_SendHeartbeatTime != SClk10Ms) // ����������ʱ��,ÿ30�뷢��һ������
//	{
//		T_SendHeartbeatTime = SClk10Ms; // ��������ʱ��
//		C_SendHeartbeatTime++;
//		// 30S

//		// ��1·
//		if (F_OneConnectStat == 1 && (C_SendHeartbeatTime >= (Pw_link1OnlineReportTime + (NowConnectIPNum * 200))))
//		{
//			if (!F_ONESendHeartbeat)
//			{
//				sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link1IP1, Pw_Link1IP2, Pw_Link1IP3, Pw_Link1IP4); // ���IP
//				sprintf((char *)portBuf, "%d", Pw_Link1Port);											   // ��һ·����Զ�˶˿���ת��Ϊascii��

//				NowConnectIPNum++; // ��ǰҪ���͵���+1

//				if (Pw_Link1TCP_UDPType == 0) // ��һ·ΪUDP
//				{
//					sprintf((char *)p_LinkTmpBuf, "AT+CIPSEND=0,22,\"%s\",%s", ipbuf, portBuf); // ��������Ϣ������Ŀռ�p_LinkTmpBuf
//					if (sim7600ce_send_cmd(p_LinkTmpBuf, ">", 200) == 0)
//						F_HeartData = 1; // �����1·��������
//				}
//				else ////��һ·ΪTCP Client
//				{
//					sprintf((char *)p_LinkTmpBuf, "AT+CIPOPEN=0,\"TCP\",\"%s\",%s", ipbuf, portBuf); // ��������Ϣ������Ŀռ�p_TmpBuf
//					if (sim7600ce_send_cmd(p_LinkTmpBuf, "+CIPOPEN: 0,0", 30))
//						; // �����1·���ӣ����ã�͸�������ʱ��ע�͵���
//					// �������ݷ�ֹTCP�������˹رգ����ܹ�������
//					if (sim7600ce_send_cmd("AT+CIPSEND=0,22", ">", 100) == 0)
//						F_HeartData = 1; // �����1·��������
//				}
//				if (F_HeartData == 1) // ��������
//				{
//					local_port = Pw_Link1localPort;			  /*���屾��1�˿�*/
//					local_port_Byte[0] = local_port >> 8;	  /*���屾��1�˿��ֽ�*/
//					local_port_Byte[1] = local_port & 0x00FF; /*���屾��1�˿��ֽ�*/
//					DTUhbeaterrPackets();					  // ��ȡ����������
//					memcpy(Txd3Buffer, p_TmpBuf, 22);		  //
//															  //					while(DMA1_Channel4->CNDTR!=0);	//�������
//															  //					UART_DMA_Enable(DMA1_Channel4,22); //���ͳ�ȥ
//					UART3_Output_Datas(Txd3Buffer, 22);		  // YLS 2023.02.05
//															  //					UART1_Output_Datas(Txd3Buffer, 22);

//					F_HeartData = 0; // ����������־��0
//					delay_ms(40);	 // ����Ҫ����ʱ

//					NormalHeartbeat1 = 1; // ��1·��������
//					USART3_RX_STA = 0;
//					Cw_Rcv3 = 0;
//					usart3_rev_finish = 0; // YLS 2023.02.08
//					C_SendHeartbeatTime = 0;
//				}
//				else
//				{
//					NormalHeartbeat1 = 0; // ��1·����������
//					hbeaterrcnt++;
//				}
//				F_ONESendHeartbeat = 1;
//			}
//		}

//		// ��2·
//		if (F_TWOConnectStat == 1 && (C_SendHeartbeatTime >= (Pw_link2OnlineReportTime + (NowConnectIPNum * 200))))
//		{
//			if (!F_TwoSendHeartbeat)
//			{
//				sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link2IP1, Pw_Link2IP2, Pw_Link2IP3, Pw_Link2IP4); // ���IP
//				sprintf((char *)portBuf, "%d", Pw_Link2Port);											   // ��2·����Զ�˶˿���ת��Ϊascii��

//				NowConnectIPNum++; // ��ǰҪ���͵���+1

//				if (Pw_Link2TCP_UDPType == 0) // ��2·ΪUDP
//				{
//					sprintf((char *)p_LinkTmpBuf, "AT+CIPSEND=1,22,\"%s\",%s", ipbuf, portBuf); // ��������Ϣ������Ŀռ�p_LinkTmpBuf
//					if (sim7600ce_send_cmd(p_LinkTmpBuf, ">", 200) == 0)
//						F_HeartData = 1; // �����2·��������
//				}
//				else ////��2·ΪTCP Client
//				{
//					sprintf((char *)p_LinkTmpBuf, "AT+CIPOPEN=1,\"TCP\",\"%s\",%s", ipbuf, portBuf); // ��������Ϣ������Ŀռ�
//					if (sim7600ce_send_cmd(p_LinkTmpBuf, "+CIPOPEN: 1,0", 30))
//						; // �����2·���ӣ����ã�͸�������ʱ��ע�͵���
//					// �������ݷ�ֹTCP�������˹رգ����ܹ�������
//					if (sim7600ce_send_cmd("AT+CIPSEND=1,22", ">", 100) == 0)
//						F_HeartData = 1; // �����2·��������
//				}

//				if (F_HeartData == 1) // ��������
//				{
//					local_port = Pw_Link2localPort;			  /*���屾��2�˿�*/
//					local_port_Byte[0] = local_port >> 8;	  /*���屾��2�˿��ֽ�*/
//					local_port_Byte[1] = local_port & 0x00FF; /*���屾��2�˿��ֽ�*/
//					DTUhbeaterrPackets();					  // ��ȡ����������
//					memcpy(Txd3Buffer, p_TmpBuf, 22);
//					//					while(DMA1_Channel4->CNDTR!=0);	//�������
//					//					UART_DMA_Enable(DMA1_Channel4,22); //���ͳ�ȥ
//					UART3_Output_Datas(Txd3Buffer, 22); // YLS 2023.02.05

//					F_HeartData = 0; // ����������־��0
//					delay_ms(40);	 // ����Ҫ����ʱ

//					NormalHeartbeat2 = 1; // ��2·��������
//					USART3_RX_STA = 0;
//					Cw_Rcv3 = 0;
//					usart3_rev_finish = 0; // YLS 2023.02.08
//					C_SendHeartbeatTime = 0;
//				}
//				else
//				{
//					NormalHeartbeat2 = 0; // ��2·����������
//					hbeaterrcnt++;
//				}
//				F_TwoSendHeartbeat = 1;
//			}
//		}

//		// ��3·
//		if (F_ThreeConnectStat == 1 && (C_SendHeartbeatTime >= (Pw_link3OnlineReportTime + (NowConnectIPNum * 200))))
//		{
//			if (!F_ThreeSendHeartbeat)
//			{
//				sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link3IP1, Pw_Link3IP2, Pw_Link3IP3, Pw_Link3IP4); // ���IP
//				sprintf((char *)portBuf, "%d", Pw_Link3Port);											   // ��3·����Զ�˶˿���ת��Ϊascii��

//				NowConnectIPNum++; // ��ǰҪ���͵���+1

//				if (Pw_Link3TCP_UDPType == 0) // ��3·ΪUDP
//				{
//					sprintf((char *)p_LinkTmpBuf, "AT+CIPSEND=2,22,\"%s\",%s", ipbuf, portBuf); // ��������Ϣ������Ŀռ�p_LinkTmpBuf
//					if (sim7600ce_send_cmd(p_LinkTmpBuf, ">", 200) == 0)
//						F_HeartData = 1; // �����3·��������
//				}
//				else ////��3·ΪTCP Client
//				{
//					sprintf((char *)p_LinkTmpBuf, "AT+CIPOPEN=2,\"TCP\",\"%s\",%s", ipbuf, portBuf); // ��������Ϣ������Ŀռ�
//					if (sim7600ce_send_cmd(p_LinkTmpBuf, "+CIPOPEN: 2,0", 30))
//						; // �����3·���ӣ����ã�͸�������ʱ��ע�͵���
//					// �������ݷ�ֹTCP�������˹رգ����ܹ�������
//					if (sim7600ce_send_cmd("AT+CIPSEND=2,22", ">", 100) == 0)
//						F_HeartData = 1; // �����3·��������
//				}
//				if (F_HeartData == 1) // ��������
//				{
//					local_port = Pw_Link3localPort;			  /*���屾��3�˿�*/
//					local_port_Byte[0] = local_port >> 8;	  /*���屾��3�˿��ֽ�*/
//					local_port_Byte[1] = local_port & 0x00FF; /*���屾��3�˿��ֽ�*/
//					DTUhbeaterrPackets();					  // ��ȡ����������
//					memcpy(Txd3Buffer, p_TmpBuf, 22);
//					//					while(DMA1_Channel4->CNDTR!=0);	//�������
//					//					UART_DMA_Enable(DMA1_Channel4,22); //���ͳ�ȥ
//					UART3_Output_Datas(Txd3Buffer, 22); // YLS 2023.02.05

//					F_HeartData = 0; // ����������־��0
//					delay_ms(40);	 // ����Ҫ����ʱ

//					NormalHeartbeat3 = 1; // ��3·��������
//					USART3_RX_STA = 0;
//					Cw_Rcv3 = 0;
//					usart3_rev_finish = 0; // YLS 2023.02.08
//					C_SendHeartbeatTime = 0;
//				}
//				else
//				{
//					NormalHeartbeat3 = 0; // ��3·����������
//					hbeaterrcnt++;
//				}
//				F_ThreeSendHeartbeat = 1;
//			}
//		}

//		// ��4·
//		if (F_FourConnectStat == 1 && (C_SendHeartbeatTime >= (Pw_link4OnlineReportTime + (NowConnectIPNum * 200))))
//		{
//			if (!F_FourSendHeartbeat)
//			{
//				sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link4IP1, Pw_Link4IP2, Pw_Link4IP3, Pw_Link4IP4); // ���IP
//				sprintf((char *)portBuf, "%d", Pw_Link4Port);											   // ��4·����Զ�˶˿���ת��Ϊascii��

//				NowConnectIPNum++; // ��ǰҪ���͵���+1

//				if (Pw_Link4TCP_UDPType == 0) // ��4·ΪUDP
//				{
//					sprintf((char *)p_LinkTmpBuf, "AT+CIPSEND=3,22,\"%s\",%s", ipbuf, portBuf); // ��������Ϣ������Ŀռ�p_LinkTmpBuf
//					if (sim7600ce_send_cmd(p_LinkTmpBuf, ">", 200) == 0)
//						F_HeartData = 1; // �����3·��������
//				}
//				else ////��4·ΪTCP Client
//				{
//					sprintf((char *)p_LinkTmpBuf, "AT+CIPOPEN=3,\"TCP\",\"%s\",%s", ipbuf, portBuf); // ��������Ϣ������Ŀռ�
//					if (sim7600ce_send_cmd(p_LinkTmpBuf, "+CIPOPEN: 3,0", 30))
//						; // �����4·���ӣ����ã�͸�������ʱ��ע�͵���
//					// �������ݷ�ֹTCP�������˹رգ����ܹ�������
//					if (sim7600ce_send_cmd("AT+CIPSEND=3,22", ">", 100) == 0)
//						F_HeartData = 1; // �����4·��������
//				}
//				if (F_HeartData == 1) // ��������
//				{
//					local_port = Pw_Link4localPort;			  /*���屾��4�˿�*/
//					local_port_Byte[0] = local_port >> 8;	  /*���屾��4�˿��ֽ�*/
//					local_port_Byte[1] = local_port & 0x00FF; /*���屾��4�˿��ֽ�*/
//					DTUhbeaterrPackets();					  // ��ȡ����������
//					memcpy(Txd3Buffer, p_TmpBuf, 22);
//					//					while(DMA1_Channel4->CNDTR!=0);	//�������
//					//					UART_DMA_Enable(DMA1_Channel4,22); //���ͳ�ȥ
//					UART3_Output_Datas(Txd3Buffer, 22); // YLS 2023.02.05

//					F_HeartData = 0; // ����������־��0
//					delay_ms(40);	 // ����Ҫ����ʱ

//					NormalHeartbeat4 = 1; // ��4·��������
//					USART3_RX_STA = 0;
//					Cw_Rcv3 = 0;
//					usart3_rev_finish = 0; // YLS 2023.02.08
//					C_SendHeartbeatTime = 0;
//				}
//				else
//				{
//					NormalHeartbeat4 = 0; // ��4·����������
//					hbeaterrcnt++;
//				}
//				F_FourSendHeartbeat = 1;
//			}
//		}
//		if (NormalHeartbeat1 + NormalHeartbeat2 + NormalHeartbeat3 + NormalHeartbeat4 == ConnectIPNum) // ��Ҫ���ӵĶ���������
//		{
//			if (NowConnectIPNum == ConnectIPNum) // ˵���Ѿ����������
//			{
//				T_SendHeartbeatTime = 100; // ����������������0���·�������
//				C_SendHeartbeatTime = 0;
//				NowConnectIPNum = 0;
//				hbeaterrcnt = 0;
//				F_PowerOnRegister = 1; // �ϵ�ע�����
//				F_ONESendHeartbeat = 0;
//				F_TwoSendHeartbeat = 0;
//				F_ThreeSendHeartbeat = 0;
//				F_FourSendHeartbeat = 0;
//			}
//		}
//		// ������ʱ��

//		if (F_OneConnectStat == 1)
//			link1OnlineReportTime = Pw_link1OnlineReportTime;
//		else
//			link1OnlineReportTime = 0;
//		if (F_TWOConnectStat == 1)
//			link2OnlineReportTime = Pw_link2OnlineReportTime;
//		else
//			link2OnlineReportTime = 0;
//		if (F_ThreeConnectStat == 1)
//			link3OnlineReportTime = Pw_link3OnlineReportTime;
//		else
//			link3OnlineReportTime = 0;
//		if (F_FourConnectStat == 1)
//			link4OnlineReportTime = Pw_link4OnlineReportTime;
//		else
//			link4OnlineReportTime = 0;
//		AllTotalTime = link1OnlineReportTime + link2OnlineReportTime + link3OnlineReportTime + link4OnlineReportTime;

//		if (((NormalHeartbeat1 + NormalHeartbeat2 + NormalHeartbeat3 + NormalHeartbeat4) != ConnectIPNum) &&
//			(C_SendHeartbeatTime >= (AllTotalTime + (ConnectIPNum * 200))))
//		{
//			T_SendHeartbeatTime = 100; // ��������������ʱ���ʱ����Ȼ����������0���·�������
//			C_SendHeartbeatTime = 0;
//			NowConnectIPNum = 0;
//			F_PowerOnRegister = 1; // �ϵ�ע�����
//			F_ONESendHeartbeat = 0;
//			F_TwoSendHeartbeat = 0;
//			F_ThreeSendHeartbeat = 0;
//			F_FourSendHeartbeat = 0;
//		}
//	}
//}

//// GPRSģ�����ݽ��պ���
// void sim7600ce_ReceiveData(void)
//{
//	u8 RecNumBit, IP1NumBit, IP2NumBit, IP3NumBit, IP4NumBit, PortNumBit; // ���յ������ݳ����Ǽ�λ��
//	u16 j = 0, n, m, i;
//	u8 *p2, *p3;
//	// u8 *p1;
//	u8 ipbuf[15];			   // IP����
//	u8 portBuf[5] = {0};	   // Զ�˶˿ڻ���
//	u8 p_RECV_IPBuf[40] = {0}; // ���ջ���

//	//	delay_ms(1);
//	//	COM2_DATA = 0;				//����������ݹر�ָʾ��
//	//	if (USART3_RX_STA & 0X8000) //��������(���еĽ������ݴ���)
//	if (usart3_rev_finish) // YLS 2023.02.07
//	{
//		// USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
//		//		Rcv3Buffer[USART3_RX_STA & 0X7FFF] = 0; //��ӽ�����
//		if (Cw_BakRcv3 <= RCV3_MAX - 1)
//		{
//			Rcv3Buffer[Cw_BakRcv3] = 0; // ��ӽ�����
//		}
//		else
//		{
//			Rcv3Buffer[RCV3_MAX - 1] = 0; // ��ӽ�����
//		}

//		// p2 = (u8 *)strstr((const char *)Rcv3Buffer, "+CIPSEND:"); // ���յ�TCP/UDP����
//		// if (p2)													  // ���յ�TCP/UDP����
//		// {
//		// 	usart3_rev_finish = 0;
//		// 	return;
//		// }

//		//		if (Pw_SupportDataReceipt == 1)
//		//			printf("%s", Rcv3Buffer);			//����ط���������ʾ�ַ����ķ�ʽ������Ӧ�÷��Ͷ��������ݣ���Ϊ�����п��ܴ���0,�ͽض���
//		//			UART1_Output_Datas(Rcv3Buffer, Cw_BakRcv3);
//		//		delay_ms(5);

//		p2 = (u8 *)strstr((const char *)Rcv3Buffer, "RECV FROM:"); // ���յ�TCP/UDP����
//		if (p2)													   // ���յ�TCP/UDP����
//		{
//			COM2_DATA = 1; // ���յ����ݿ���ָʾ��

//			// ��1·
//			if ((Pw_Link1IP1 + Pw_Link1IP2 + Pw_Link1IP3 + Pw_Link1IP4) != 0) // ��һ·��IP��ַ
//			{
//				sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link1IP1, Pw_Link1IP2, Pw_Link1IP3, Pw_Link1IP4); // ���IP
//				sprintf((char *)portBuf, "%d", Pw_Link1Port);											   // ��һ·����Զ�˶˿���ת��Ϊascii��
//				sprintf((char *)p_RECV_IPBuf, "%s:%s", ipbuf, portBuf);									   // ��������Ϣ��p_RECV_IPBuf
//				if (strstr((const char *)p2, (const char *)p_RECV_IPBuf))								   // �յ��ĵ�1·����������
//				{
//					p3 = (u8 *)strstr((const char *)p2, "+IPD");
//					//					sprintf((char *)p1_TmpBuf, "%s", p3 + 4); //���յ��ĵ�1·���ݳ����ַ� ����������ָ��ָ����ַ��������鴫ֵ����֪���ܴ��������������׳��������
//					for (i = 0; i < 15; i++)
//					{
//						p1_TmpBuf[i] = *(p3 + 4);
//						p3++;
//					}
//					for (i = 0; i <= 5; i++) // �ѽ��յ��ĵ�1·���ݳ����ַ�ת��Ϊ16������
//					{
//						if (p1_TmpBuf[i] == 0x0D) // ˵�����ݳ����ַ�������û����
//						{
//							ReceiveGPRSBufOneDateLen = atoi(&p1_TmpBuf[0], i); // ת��Ϊ16����

//							ReceiveGPRSBufTwoDateLen = 0;
//							ReceiveGPRSBufThreeDateLen = 0;
//							ReceiveGPRSBufFourDateLen = 0;
//							break; //
//						}
//					}
//					for (i = 0; i <= 5; i++)
//						p1_TmpBuf[i] = 0; // �ѽ��յĵ�1·�ַ����泤������0��Ϊ��һ�μ���
//					//					for (i = 0; i <RCV3_MAX; i++)
//					//						ReceiveGPRSBuf[i] = 0; //�����ReceiveGPRSBuf��Ȼ���ٰѽ��յ������ݷŵ�����
//					memset(ReceiveGPRSBuf, 0, RCV3_MAX);
//					m = ReceiveGPRSBufOneDateLen;
//					for (RecNumBit = 0; m != 0; RecNumBit++)
//						m /= 10;
//					m = Pw_Link1IP1;
//					for (IP1NumBit = 0; m != 0; IP1NumBit++)
//						m /= 10;
//					m = Pw_Link1IP2;
//					for (IP2NumBit = 0; m != 0; IP2NumBit++)
//						m /= 10;
//					m = Pw_Link1IP3;
//					for (IP3NumBit = 0; m != 0; IP3NumBit++)
//						m /= 10;
//					m = Pw_Link1IP4;
//					for (IP4NumBit = 0; m != 0; IP4NumBit++)
//						m /= 10;
//					m = Pw_Link1Port;
//					for (PortNumBit = 0; m != 0; PortNumBit++)
//						m /= 10;
//					n = RecNumBit + IP1NumBit + IP2NumBit + IP3NumBit + IP4NumBit + PortNumBit;
//					for (i = 24 + n; i <= (ReceiveGPRSBufOneDateLen + 24 + n); i++)
//					{
//						ReceiveGPRSBuf[j] = Rcv3Buffer[i]; // ���յ��ĵ�1·���ݸ�ReceiveGPRSBuf
//						j++;
//					}
//					if (j >= ReceiveGPRSBufOneDateLen)
//						j = 0;
//					ChannelNo = 0; // ͨ���ţ��ڼ�·ͨ���յ�����
//					F_AcklinkNum = 1;
//				}
//			}

//			// ��2·
//			if ((Pw_Link2IP1 + Pw_Link2IP2 + Pw_Link2IP3 + Pw_Link2IP4) != 0)
//			{
//				sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link2IP1, Pw_Link2IP2, Pw_Link2IP3, Pw_Link2IP4); // ���IP
//				sprintf((char *)portBuf, "%d", Pw_Link2Port);											   // ��һ·����Զ�˶˿���ת��Ϊascii��
//				sprintf((char *)p_RECV_IPBuf, "%s:%s", ipbuf, portBuf);									   // ��������Ϣ��p_RECV_IPBuf
//				if (strstr((const char *)p2, (const char *)p_RECV_IPBuf))								   // �յ��ĵ�2·����������
//				{
//					p3 = (u8 *)strstr((const char *)p2, "+IPD");
//					//					sprintf((char *)p1_TmpBuf, "%s", p3 + 4); //���յ��ĵ�2·���ݳ����ַ�
//					for (i = 0; i < 15; i++)
//					{
//						p1_TmpBuf[i] = *(p3 + 4);
//						p3++;
//					}
//					for (i = 0; i <= 5; i++) // �ѽ��յ��ĵ�2·���ݳ����ַ�ת��Ϊ16������
//					{
//						if (p1_TmpBuf[i] == 0x0D) // ˵�����ݳ����ַ�������û����
//						{
//							ReceiveGPRSBufTwoDateLen = atoi(&p1_TmpBuf[0], i); // ת��Ϊ16����

//							ReceiveGPRSBufOneDateLen = 0;
//							ReceiveGPRSBufThreeDateLen = 0;
//							ReceiveGPRSBufFourDateLen = 0;
//							break; //
//						}
//					}
//					for (i = 0; i <= 5; i++)
//						p1_TmpBuf[i] = 0; // �ѽ��յĵ�2·�ַ����泤������0��Ϊ��һ�μ���
//					//					for (i = 0; i <RCV3_MAX; i++)
//					//						ReceiveGPRSBuf[i] = 0; //�����ReceiveGPRSBuf��Ȼ���ٰѽ��յ������ݷŵ�����
//					memset(ReceiveGPRSBuf, 0, RCV3_MAX);
//					m = ReceiveGPRSBufTwoDateLen;
//					for (RecNumBit = 0; m != 0; RecNumBit++)
//						m /= 10;
//					m = Pw_Link2IP1;
//					for (IP1NumBit = 0; m != 0; IP1NumBit++)
//						m /= 10;
//					m = Pw_Link2IP2;
//					for (IP2NumBit = 0; m != 0; IP2NumBit++)
//						m /= 10;
//					m = Pw_Link2IP3;
//					for (IP3NumBit = 0; m != 0; IP3NumBit++)
//						m /= 10;
//					m = Pw_Link2IP4;
//					for (IP4NumBit = 0; m != 0; IP4NumBit++)
//						m /= 10;
//					m = Pw_Link2Port;
//					for (PortNumBit = 0; m != 0; PortNumBit++)
//						m /= 10;
//					n = RecNumBit + IP1NumBit + IP2NumBit + IP3NumBit + IP4NumBit + PortNumBit;
//					for (i = 24 + n; i <= (ReceiveGPRSBufTwoDateLen + 24 + n); i++)
//					{
//						ReceiveGPRSBuf[j] = Rcv3Buffer[i]; // ���յ��ĵ�2·���ݸ�ReceiveGPRSBuf
//						j++;
//					}
//					if (j >= ReceiveGPRSBufTwoDateLen)
//						j = 0;
//					ChannelNo = 1; // ͨ���ţ��ڼ�·ͨ���յ�����
//					F_AcklinkNum = 2;
//				}
//			}

//			// ��3·
//			if ((Pw_Link3IP1 + Pw_Link3IP2 + Pw_Link3IP3 + Pw_Link3IP4) != 0)
//			{
//				sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link3IP1, Pw_Link3IP2, Pw_Link3IP3, Pw_Link3IP4); // ���IP
//				sprintf((char *)portBuf, "%d", Pw_Link3Port);											   // ��һ·����Զ�˶˿���ת��Ϊascii��
//				sprintf((char *)p_RECV_IPBuf, "%s:%s", ipbuf, portBuf);									   // ��������Ϣ��p_RECV_IPBuf
//				if (strstr((const char *)p2, (const char *)p_RECV_IPBuf))								   // �յ��ĵ�3·����������
//				{
//					p3 = (u8 *)strstr((const char *)p2, "+IPD");
//					//					sprintf((char *)p1_TmpBuf, "%s", p3 + 4); //���յ��ĵ�3·���ݳ����ַ�
//					for (i = 0; i < 15; i++)
//					{
//						p1_TmpBuf[i] = *(p3 + 4);
//						p3++;
//					}
//					for (i = 0; i <= 5; i++) // �ѽ��յ��ĵ�3·���ݳ����ַ�ת��Ϊ16������
//					{
//						if (p1_TmpBuf[i] == 0x0D) // ˵�����ݳ����ַ�������û����
//						{
//							ReceiveGPRSBufThreeDateLen = atoi(&p1_TmpBuf[0], i); // ת��Ϊ16����

//							ReceiveGPRSBufOneDateLen = 0;
//							ReceiveGPRSBufTwoDateLen = 0;
//							ReceiveGPRSBufFourDateLen = 0;
//							break; //
//						}
//					}
//					for (i = 0; i <= 5; i++)
//						p1_TmpBuf[i] = 0; // �ѽ��յĵ�3·�ַ����泤������0��Ϊ��һ�μ���
//					//					for (i = 0; i <RCV3_MAX; i++)
//					//						ReceiveGPRSBuf[i] = 0; //�����ReceiveGPRSBuf��Ȼ���ٰѽ��յ������ݷŵ�����
//					memset(ReceiveGPRSBuf, 0, RCV3_MAX);
//					m = ReceiveGPRSBufThreeDateLen;
//					for (RecNumBit = 0; m != 0; RecNumBit++)
//						m /= 10;
//					m = Pw_Link3IP1;
//					for (IP1NumBit = 0; m != 0; IP1NumBit++)
//						m /= 10;
//					m = Pw_Link3IP2;
//					for (IP2NumBit = 0; m != 0; IP2NumBit++)
//						m /= 10;
//					m = Pw_Link3IP3;
//					for (IP3NumBit = 0; m != 0; IP3NumBit++)
//						m /= 10;
//					m = Pw_Link3IP4;
//					for (IP4NumBit = 0; m != 0; IP4NumBit++)
//						m /= 10;
//					m = Pw_Link3Port;
//					for (PortNumBit = 0; m != 0; PortNumBit++)
//						m /= 10;
//					n = RecNumBit + IP1NumBit + IP2NumBit + IP3NumBit + IP4NumBit + PortNumBit;
//					for (i = 24 + n; i <= (ReceiveGPRSBufThreeDateLen + 24 + n); i++)
//					{
//						ReceiveGPRSBuf[j] = Rcv3Buffer[i]; // ���յ��ĵ�3·���ݸ�ReceiveGPRSBuf
//						j++;
//					}
//					if (j >= ReceiveGPRSBufThreeDateLen)
//						j = 0;
//					ChannelNo = 2; // ͨ���ţ��ڼ�·ͨ���յ�����
//					F_AcklinkNum = 3;
//				}
//			}

//			// ��4·
//			if ((Pw_Link4IP1 + Pw_Link4IP2 + Pw_Link4IP3 + Pw_Link4IP4) != 0)
//			{
//				sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link4IP1, Pw_Link4IP2, Pw_Link4IP3, Pw_Link4IP4); // ���IP
//				sprintf((char *)portBuf, "%d", Pw_Link4Port);											   // ��һ·����Զ�˶˿���ת��Ϊascii��
//				sprintf((char *)p_RECV_IPBuf, "%s:%s", ipbuf, portBuf);									   // ��������Ϣ��p_RECV_IPBuf
//				if (strstr((const char *)p2, (const char *)p_RECV_IPBuf))								   // �յ��ĵ�4·����������
//				{
//					p3 = (u8 *)strstr((const char *)p2, "+IPD");
//					//					sprintf((char *)p1_TmpBuf, "%s", p3 + 4); //���յ��ĵ�4·���ݳ����ַ�
//					for (i = 0; i < 15; i++)
//					{
//						p1_TmpBuf[i] = *(p3 + 4);
//						p3++;
//					}
//					for (i = 0; i <= 5; i++) // �ѽ��յ��ĵ�4·���ݳ����ַ�ת��Ϊ16������
//					{
//						if (p1_TmpBuf[i] == 0x0D) // ˵�����ݳ����ַ�������û����
//						{
//							ReceiveGPRSBufFourDateLen = atoi(&p1_TmpBuf[0], i); // ת��Ϊ16����

//							ReceiveGPRSBufOneDateLen = 0;
//							ReceiveGPRSBufTwoDateLen = 0;
//							ReceiveGPRSBufThreeDateLen = 0;
//							break; //
//						}
//					}
//					for (i = 0; i <= 5; i++)
//						p1_TmpBuf[i] = 0; // �ѽ��յĵ�4·�ַ����泤������0��Ϊ��һ�μ���
//					//					for (i = 0; i <RCV3_MAX; i++)
//					//						ReceiveGPRSBuf[i] = 0; //�����ReceiveGPRSBuf��Ȼ���ٰѽ��յ������ݷŵ�����
//					memset(ReceiveGPRSBuf, 0, RCV3_MAX);
//					m = ReceiveGPRSBufFourDateLen;
//					for (RecNumBit = 0; m != 0; RecNumBit++)
//						m /= 10;
//					m = Pw_Link4IP1;
//					for (IP1NumBit = 0; m != 0; IP1NumBit++)
//						m /= 10;
//					m = Pw_Link4IP2;
//					for (IP2NumBit = 0; m != 0; IP2NumBit++)
//						m /= 10;
//					m = Pw_Link4IP3;
//					for (IP3NumBit = 0; m != 0; IP3NumBit++)
//						m /= 10;
//					m = Pw_Link4IP4;
//					for (IP4NumBit = 0; m != 0; IP4NumBit++)
//						m /= 10;
//					m = Pw_Link4Port;
//					for (PortNumBit = 0; m != 0; PortNumBit++)
//						m /= 10;
//					n = RecNumBit + IP1NumBit + IP2NumBit + IP3NumBit + IP4NumBit + PortNumBit;
//					for (i = 24 + n; i <= (ReceiveGPRSBufFourDateLen + 24 + n); i++)
//					{
//						ReceiveGPRSBuf[j] = Rcv3Buffer[i]; // ���յ��ĵ�4·���ݸ�ReceiveGPRSBuf
//						j++;
//					}
//					if (j >= ReceiveGPRSBufFourDateLen)
//						j = 0;
//					ChannelNo = 3; // ͨ���ţ��ڼ�·ͨ���յ�����
//					F_AcklinkNum = 4;
//				}
//			}

//			delay_ms(50);
//			sim7600ce_DoWithData(); // ������յ�������
//		}

//		// ���յ���Ϣ
//		p2 = (u8 *)strstr((const char *)Rcv3Buffer, "+CMTI:"); // ���յ���Ϣ
//		if (p2)												   // ���ն�������
//		{
//			if (strstr((const char *)p2, "+CMTI:")) // �յ���������
//			{
//				p3 = (u8 *)strstr((const char *)p2, ",");			   // �յ���������
//				p2[0] = 0;											   // ���������
//				sprintf((char *)p1_TmpBuf, "AT+CMGR=%s\r\n", p3 + 1);  // Ҫ���͵Ķ�ȡָ��
//				sim7600ce_send_cmd(p1_TmpBuf, "+CMGR:", 200);		   // ������
//				delay_ms(40);										   // ����Ҫ����ʱ
//				if (sim7600ce_send_cmd(p1_TmpBuf, "+CMGR:", 200) == 0) // ��ȡ����
//				{
//					p3 = (u8 *)strstr((const char *)(Rcv3Buffer), ",");
//					p2 = (u8 *)strstr((const char *)(p3 + 2), "\"");
//					p2[0] = 0;										// ���������
//					sprintf((char *)CallerNumberbuf, "%s", p3 + 5); // ���յ���Ϣ�ĵ绰����

//					p3 = (u8 *)strstr((const char *)(p2 + 1), "\r"); // Ѱ�һس���
//					sprintf((char *)SMSbuf, "%s", p3 + 2);			 // �ѽ��յ����ŵ����ݸ�p1_TmpBuf
//				}
//			}
//		}
//		USART3_RX_STA = 0;
//		Cw_Rcv3 = 0;
//		usart3_rev_finish = 0; // YLS 2023.02.08
//							   // USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // �򿪽����ж�
//	}
//}
//// ���ģ����Ҫ���ӵ�IP����
//// ����� 2018.08.23  15��27
// void sim7600ce_connectstaModularNum(void) // ���ģ����Ҫ���ӵ�IP����
//{

//	if ((Pw_Link1IP1 + Pw_Link1IP2 + Pw_Link1IP3 + Pw_Link1IP4) != 0)
//	{
//		if (!F_OneConnectStat)
//		{
//			ConnectIPNum++;		  // ��Ҫ����IP������
//			F_OneConnectStat = 1; // ��1·��Ҫ����IP
//		}
//	}
//	else
//	{
//		if (F_OneConnectStat)
//		{
//			ConnectIPNum--;		  // ��Ҫ����IP������
//			F_OneConnectStat = 0; // ��1·����Ҫ����IP
//		}
//	}
//	if ((Pw_Link2IP1 + Pw_Link2IP2 + Pw_Link2IP3 + Pw_Link2IP4) != 0)
//	{
//		if (!F_TWOConnectStat)
//		{
//			ConnectIPNum++;		  // ��Ҫ����IP������
//			F_TWOConnectStat = 1; // ��2·��Ҫ����IP
//		}
//	}
//	else
//	{
//		if (F_TWOConnectStat)
//		{
//			ConnectIPNum--;		  // ��Ҫ����IP������
//			F_TWOConnectStat = 0; // ��2·����Ҫ����IP
//		}
//	}
//	if ((Pw_Link3IP1 + Pw_Link3IP2 + Pw_Link3IP3 + Pw_Link3IP4) != 0)
//	{
//		if (!F_ThreeConnectStat)
//		{
//			ConnectIPNum++;			// ��Ҫ����IP������
//			F_ThreeConnectStat = 1; // ��3·��Ҫ����IP
//		}
//	}
//	else
//	{
//		if (F_ThreeConnectStat)
//		{
//			ConnectIPNum--;			// ��Ҫ����IP������
//			F_ThreeConnectStat = 0; // ��3·����Ҫ����IP
//		}
//	}
//	if ((Pw_Link4IP1 + Pw_Link4IP2 + Pw_Link4IP3 + Pw_Link4IP4) != 0)
//	{
//		if (!F_FourConnectStat)
//		{
//			ConnectIPNum++;		   // ��Ҫ����IP������
//			F_FourConnectStat = 1; // ��4·��Ҫ����IP
//		}
//	}
//	else
//	{
//		if (F_FourConnectStat)
//		{
//			ConnectIPNum--;		   // ��Ҫ����IP������
//			F_FourConnectStat = 0; // ��4·����Ҫ����IP
//		}
//	}
//}

//// ���Ž��ճ�ʼ������
// void sim7600ce_sms_ReciveInit(void)
//{
//	sim7600ce_send_cmd("AT+CMGF=1", "OK", 200);		  // ����Ϊ�ı�ģʽ
//	sim7600ce_send_cmd("AT+CSCS=\"GSM\"", "OK", 200); // ����GSM�ַ���
//	sim7600ce_send_cmd("AT+CNMI=2,1", "OK", 200);	  // ��������Ϣ��ʾ
// }

//// SendDateBufҪ���Ͷ��ŵ����ݣ�������ASC���ַ�
//// sim7600ce������
// void sim7600ce_sms_send(u8 *SendDateBuf)
//{
//	u8 i;
//	u8 sms_SendBuf[21]; // �������ݻ���
//	u8 phonebuf[12];	// ���뻺��

//	if (CallerNumberbuf[0] + CallerNumberbuf[1] + CallerNumberbuf[2] != 0) // �ֻ�����ǰ3λ����0��ʱ����Է��Ͷ���
//	{
//		memcpy(phonebuf, CallerNumberbuf, 11); // ���ƺ���
//		phonebuf[11] = '\0';				   // �����ַ�
//		sprintf((char *)sms_SendBuf, "AT+CMGS=\"%s\"", phonebuf);
//		sim7600ce_send_cmd("AT+CSCS=\"GSM\"", "OK", 200);	// ����GSM�ַ���
//		sim7600ce_send_cmd("AT+CMGF=1", "OK", 200);			// ����Ϊ�ı�ģʽ
//		if (sim7600ce_send_cmd(sms_SendBuf, ">", 200) == 0) // ����Ҫ�����ֻ��ĵ绰����
//		{
//			delay_ms(200);									//
//			Com2_printf("%s", SendDateBuf);					// ���Ͷ�������
//			delay_ms(500);									// ������ʱ�������ܷ��Ͷ���
//			sim7600ce_send_cmd((u8 *)0X1A, "+CMGS:", 2000); // ���ͽ�����,�ȴ��������(��ȴ�20����,��Ϊ���ų��˵Ļ�,�ȴ�ʱ��᳤һЩ)
//			delay_ms(100);									//
//			sim7600ce_sms_ReciveInit();						// ����Ϊ���ܶ���ģʽ
//		}
//		for (i = 0; i <= 49; i++)
//			SMSbuf[i] = 0; // ���յ�����Ϣ���
//		for (i = 0; i <= 11; i++)
//			CallerNumberbuf[i] = 0; // ����Ҫ���͵��ֻ�����0��Ϊ��һ�η���
//		USART3_RX_STA = 0;
//		Cw_Rcv3 = 0;
//		usart3_rev_finish = 0; // YLS 2023.02.08
//	}
//}

//// ���Ŷ����ݼ���
// const u8 *modetb2[2] = {"UDP", "TCP"}; // ����ģʽ//mode:1:TCP����;0,UDP����)
// void sim7600ce_smsread_UpLoaded(void)
//{
//	//	u8 *p1, *p2;
//	// ��ѯ��Ϣ

//	if (strstr((const char *)SMSbuf, "GPR+01;")) // ��ȡ��1·��IP�Ͷ˿ڡ�01���д���ǵ�1·��IP�Ͷ˿ڡ�����(GPR+01;)
//	{
//		sprintf((char *)p1_TmpBuf, "%s:%d.%d.%d.%d:%d", modetb2[(u8)Pw_Link1TCP_UDPType], Pw_Link1IP1, Pw_Link1IP2, Pw_Link1IP3, Pw_Link1IP4, Pw_Link1Port); // ��ȡ��1·��IP�Ͷ˿ڡ�
//		sprintf((char *)p_TmpBuf, "GPR+01, IP1:\r\n\"%s\"", p1_TmpBuf);																						 // дҪ���ص�DTU���ݡ�
//		sim7600ce_sms_send(p_TmpBuf);
//	}
//	else if (strstr((const char *)SMSbuf, "GPR+02;")) // ��ȡ��2·��IP�Ͷ˿ڡ�02����(GPR+02;)
//	{
//		sprintf((char *)p1_TmpBuf, "%s:%d.%d.%d.%d:%d", modetb2[(u8)Pw_Link2TCP_UDPType], Pw_Link2IP1, Pw_Link2IP2, Pw_Link2IP3, Pw_Link2IP4, Pw_Link2Port); // ��ȡ��2·��IP�Ͷ˿ڡ�
//		sprintf((char *)p_TmpBuf, "GPR+02, IP2:\r\n\"%s\"", p1_TmpBuf);																						 // дҪ���ص�DTU���ݡ�
//		sim7600ce_sms_send(p_TmpBuf);
//	}
//	else if (strstr((const char *)SMSbuf, "GPR+03;")) // ��ȡ��3·��IP�Ͷ˿ڡ�03����(GPR+03;)
//	{
//		sprintf((char *)p1_TmpBuf, "%s:%d.%d.%d.%d:%d", modetb2[(u8)Pw_Link3TCP_UDPType], Pw_Link3IP1, Pw_Link3IP2, Pw_Link3IP3, Pw_Link3IP4, Pw_Link3Port); // ��ȡ��3·��IP�Ͷ˿ڡ�
//		sprintf((char *)p_TmpBuf, "GPR+03, IP3:\r\n\"%s\"", p1_TmpBuf);																						 // дҪ���ص�DTU���ݡ�
//		sim7600ce_sms_send(p_TmpBuf);
//	}
//	else if (strstr((const char *)SMSbuf, "GPR+04;")) // ��ȡ��4·��IP�Ͷ˿ڡ�04����(GPR+04;)
//	{
//		sprintf((char *)p1_TmpBuf, "%s:%d.%d.%d.%d:%d", modetb2[(u8)Pw_Link4TCP_UDPType], Pw_Link4IP1, Pw_Link4IP2, Pw_Link4IP3, Pw_Link4IP4, Pw_Link4Port); // ��ȡ��4·��IP�Ͷ˿ڡ�
//		sprintf((char *)p_TmpBuf, "GPR+04, IP4:\r\n\"%s\"", p1_TmpBuf);																						 // дҪ���ص�DTU���ݡ�
//		sim7600ce_sms_send(p_TmpBuf);
//	}
//	else if (strstr((const char *)SMSbuf, "GPR+05;")) // ��ȡ����ģ��ĵ�ַ��05����(GPR+05;)
//	{
//		sprintf((char *)p1_TmpBuf, "%d", Pw_RFModularAddress);			   // ��ȡ����ģ��ĵ�ַ��
//		sprintf((char *)p_TmpBuf, "GPR+05, RF-Address:\"%s\"", p1_TmpBuf); // дҪ���ص�DTU���ݡ�
//		sim7600ce_sms_send(p_TmpBuf);
//	}
//	else if (strstr((const char *)SMSbuf, "GPR+06;")) // ��ȡDTU���롣06����(GPR+06;)
//	{
//		sprintf((char *)p1_TmpBuf, "%d%d%d", Pw_DTUIDNUM1, Pw_DTUIDNUM2, Pw_DTUIDNUM3); // ȡ��DTU���롣
//		//		sprintf((char *)p1_TmpBuf, "%s", &GprsPar[DtuNoBase]);		// ȡ��DTU���롣	//YLS 2023.02.08
//		sprintf((char *)p_TmpBuf, "GPR+06, DTU:\"%s\"", p1_TmpBuf); // дҪ���ص�DTU���ݡ�
//		sim7600ce_sms_send(p_TmpBuf);
//	}
//}
//// ����д���ݼ���
// const u8 SMS_SetOk[] = {"SMS SETOK!"};		 //
// const u8 SMS_SetERROR[] = {"SMS SETERROR!"}; //
// u8 IPDotNUM = 0;							 // IP�������
// void sim7600ce_smsWrite_UpLoaded(void)
//{
//	u8 i, j = 0, NowWriteOK = 0;
//	// д��Ϣ
//	if (strstr((const char *)SMSbuf, "GPW+01,")) // ���д���ǵ�1·��IP�Ͷ˿ڡ�����(GPW+01,UDP:222.173.103.226:5008;)
//	{
//		if (SMSbuf[6] == ',' && SMSbuf[10] == ':')
//		{
//			j = 0;			// �����
//			NowWriteOK = 0; // �����
//			IPDotNUM = 0;	// ��ʼ����ֵ
//			if (SMSbuf[7] == 'T')
//				Pw_Link1TCP_UDPType = 1; // Э������
//			else
//				Pw_Link1TCP_UDPType = 0;
//			if (j == 0)
//			{
//				for (i = 0; i <= 4; i++) // ip1�ĵ�1λȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == '.') // ˵��ip1�ĵ�1λ���ݳ����ַ�������û����
//					{
//						Pw_Link1IP1 = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = IPDotNUM + i + 1;				   // �ҵ���һ��IP1�ĵ�2��IP�����д��λ��
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+01,IP1:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				for (i = 0; i <= 4; i++) // ip1�ĵ�2λȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == '.') // ˵��ip1�ĵ�2λ���ݳ����ַ�������û����
//					{
//						Pw_Link1IP2 = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = IPDotNUM + i + 1;				   // �ҵ���һ��IP1�ĵ�3��IP�����д��λ��
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+01,IP1:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				for (i = 0; i <= 4; i++) // ip1�ĵ�3λȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == '.') // ˵��ip1�ĵ�3λ���ݳ����ַ�������û����
//					{
//						Pw_Link1IP3 = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = IPDotNUM + i + 1;				   // �ҵ���һ��IP1�ĵ�4��IP�����д��λ��
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+01,IP1:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				for (i = 0; i <= 4; i++) // ip1�ĵ�4λȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == ':') // ˵��ip1�ĵ�4λ���ݳ����ַ�������û����
//					{
//						Pw_Link1IP4 = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = IPDotNUM + i + 1;				   // �ҵ���һ��IP1�Ķ˿������д��λ��
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+01,IP1:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				for (i = 0; i <= 5; i++) // ip1�Ķ˿�ȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == ';') // ˵��ip1�Ķ˿����ݳ����ַ�������û����
//					{
//						Pw_Link1Port = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = 0;									// �ҵ���һ��IP1�����д��λ����0
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+01,IP1:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				SPI_FMRAM_BufferWrite((u8 *)(&Pw_ParLst_GPRS[0]), 0, FLASH_PAR_SIZE * 2); // FLASH_PAR_SIZE 256���趨����һ��������
//				sprintf((char *)p_TmpBuf, "GPW+01,IP1:\"%s\"", SMS_SetOk);				  // дҪ���ص�DTU���ݡ�

//				F_RunAgain = 1;		   // ��������ģ��
//				sim_ready = 0;		   // ���sim��׼������
//				F_RunCheckModular = 1; // ���й����м��ģ���־
//			}
//			if (j == 1)
//			{
//				Pw_Link1IP1 = 0;
//				Pw_Link1IP2 = 0;
//				Pw_Link1IP3 = 0;
//				Pw_Link1IP4 = 0;
//				Pw_Link1Port = 0;
//				j = 0; // �¸��������м����
//			}
//		}
//		else
//		{
//			sprintf((char *)p_TmpBuf, "GPW+01,IP1:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//		}
//		sim7600ce_sms_send(p_TmpBuf); // ���ض���
//	}
//	else if (strstr((const char *)SMSbuf, "GPW+02,")) // ���д���ǵ�2·��IP�Ͷ˿ڡ�����(GPW+02,UDP:222.173.103.226:5008;)
//	{
//		if (SMSbuf[6] == ',' && SMSbuf[10] == ':')
//		{
//			j = 0;			// �����
//			NowWriteOK = 0; // �����
//			IPDotNUM = 0;	// ��ʼ����ֵ
//			if (SMSbuf[7] == 'T')
//				Pw_Link2TCP_UDPType = 1;
//			else
//				Pw_Link2TCP_UDPType = 0;
//			if (j == 0)
//			{
//				for (i = 0; i <= 4; i++) // ip2�ĵ�1λȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == '.') // ˵��ip2�ĵ�1λ���ݳ����ַ�������û����
//					{
//						Pw_Link2IP1 = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = IPDotNUM + i + 1;				   // �ҵ���һ��IP2�ĵ�2��IP�����д��λ��
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+02,IP2:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				for (i = 0; i <= 4; i++) // ip2�ĵ�2λȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == '.') // ˵��ip2�ĵ�2λ���ݳ����ַ�������û����
//					{
//						Pw_Link2IP2 = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = IPDotNUM + i + 1;				   // �ҵ���һ��IP2�ĵ�3��IP�����д��λ��
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+02,IP2:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				for (i = 0; i <= 4; i++) // ip2�ĵ�3λȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == '.') // ˵��ip2�ĵ�3λ���ݳ����ַ�������û����
//					{
//						Pw_Link2IP3 = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = IPDotNUM + i + 1;				   // �ҵ���һ��IP2�ĵ�4��IP�����д��λ��
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+02,IP2:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				for (i = 0; i <= 4; i++) // ip2�ĵ�4λȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == ':') // ˵��ip2�ĵ�4λ���ݳ����ַ�������û����
//					{
//						Pw_Link2IP4 = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = IPDotNUM + i + 1;				   // �ҵ���һ��IP2�Ķ˿������д��λ��
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+02,IP2:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				for (i = 0; i <= 5; i++) // ip2�Ķ˿�ȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == ';') // ˵��ip2�Ķ˿����ݳ����ַ�������û����
//					{
//						Pw_Link2Port = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = 0;									// �ҵ���һ��IP2�����д��λ����0
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+02,IP2:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				SPI_FMRAM_BufferWrite((u8 *)(&Pw_ParLst_GPRS[0]), 0, FLASH_PAR_SIZE * 2); // FLASH_PAR_SIZE 256���趨����һ��������
//				sprintf((char *)p_TmpBuf, "GPW+02,IP2:\"%s\"", SMS_SetOk);				  // дҪ���ص�DTU���ݡ�

//				F_RunAgain = 1;		   // ��������ģ��
//				sim_ready = 0;		   // ���sim��׼������
//				F_RunCheckModular = 1; // ���й����м��ģ���־
//			}
//			if (j == 1)
//			{
//				Pw_Link2IP1 = 0;
//				Pw_Link2IP2 = 0;
//				Pw_Link2IP3 = 0;
//				Pw_Link2IP4 = 0;
//				Pw_Link2Port = 0;
//				j = 0; // �¸��������м����
//			}
//		}
//		else
//		{
//			sprintf((char *)p_TmpBuf, "GPW+02,IP2:\"%s\"", SMS_SetERROR); // дҪ���ص�IP2���ݴ���
//		}
//		sim7600ce_sms_send(p_TmpBuf); // ���ض���
//	}
//	else if (strstr((const char *)SMSbuf, "GPW+03,")) // ���д���ǵ�3·��IP�Ͷ˿ڡ�����(GPW+03,UDP:222.173.103.226:5008;)
//	{
//		if (SMSbuf[6] == ',' && SMSbuf[10] == ':')
//		{
//			j = 0;			// �����
//			NowWriteOK = 0; // �����
//			IPDotNUM = 0;	// ��ʼ����ֵ
//			if (SMSbuf[7] == 'T')
//				Pw_Link3TCP_UDPType = 1;
//			else
//				Pw_Link3TCP_UDPType = 0;
//			if (j == 0)
//			{
//				for (i = 0; i <= 4; i++) // ip3�ĵ�1λȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == '.') // ˵��ip3�ĵ�1λ���ݳ����ַ�������û����
//					{
//						Pw_Link3IP1 = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = IPDotNUM + i + 1;				   // �ҵ���һ��IP3�ĵ�2��IP�����д��λ��
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+03,IP3:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				for (i = 0; i <= 4; i++) // ip3�ĵ�2λȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == '.') // ˵��ip3�ĵ�2λ���ݳ����ַ�������û����
//					{
//						Pw_Link3IP2 = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = IPDotNUM + i + 1;				   // �ҵ���һ��IP3�ĵ�3��IP�����д��λ��
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+03,IP3:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				for (i = 0; i <= 4; i++) // ip3�ĵ�3λȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == '.') // ˵��ip3�ĵ�3λ���ݳ����ַ�������û����
//					{
//						Pw_Link3IP3 = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = IPDotNUM + i + 1;				   // �ҵ���һ��IP3�ĵ�4��IP�����д��λ��
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+03,IP3:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				for (i = 0; i <= 4; i++) // ip3�ĵ�4λȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == ':') // ˵��ip3�ĵ�4λ���ݳ����ַ�������û����
//					{
//						Pw_Link3IP4 = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = IPDotNUM + i + 1;				   // �ҵ���һ��IP3�Ķ˿������д��λ��
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+03,IP3:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				for (i = 0; i <= 5; i++) // ip3�Ķ˿�ȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == ';') // ˵��ip3�Ķ˿����ݳ����ַ�������û����
//					{
//						Pw_Link3Port = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = 0;									// �ҵ���һ��IP3�����д��λ����0
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+03,IP3:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				SPI_FMRAM_BufferWrite((u8 *)(&Pw_ParLst_GPRS[0]), 0, FLASH_PAR_SIZE * 2); // FLASH_PAR_SIZE 256���趨����һ��������
//				sprintf((char *)p_TmpBuf, "GPW+03,IP3:\"%s\"", SMS_SetOk);				  // дҪ���ص�DTU���ݡ�

//				F_RunAgain = 1;		   // ��������ģ��
//				sim_ready = 0;		   // ���sim��׼������
//				F_RunCheckModular = 1; // ���й����м��ģ���־
//			}
//			if (j == 1)
//			{
//				Pw_Link3IP1 = 0;
//				Pw_Link3IP2 = 0;
//				Pw_Link3IP3 = 0;
//				Pw_Link3IP4 = 0;
//				Pw_Link3Port = 0;
//				j = 0; // �¸��������м����
//			}
//		}
//		else
//		{
//			sprintf((char *)p_TmpBuf, "GPW+03,IP3:\"%s\"", SMS_SetERROR); // дҪ���ص�IP3���ݴ���
//		}
//		sim7600ce_sms_send(p_TmpBuf); // ���ض���
//	}
//	else if (strstr((const char *)SMSbuf, "GPW+04,")) // ���д���ǵ�4·��IP�Ͷ˿ڡ�����(GPW+04,UDP:222.173.103.226:5008;)
//	{
//		if (SMSbuf[6] == ',' && SMSbuf[10] == ':')
//		{
//			j = 0;			// �����
//			NowWriteOK = 0; // �����
//			IPDotNUM = 0;	// ��ʼ����ֵ
//			if (SMSbuf[7] == 'T')
//				Pw_Link4TCP_UDPType = 1;
//			else
//				Pw_Link4TCP_UDPType = 0;
//			if (j == 0)
//			{
//				for (i = 0; i <= 4; i++) // ip4�ĵ�1λȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == '.') // ˵��ip4�ĵ�1λ���ݳ����ַ�������û����
//					{
//						Pw_Link4IP1 = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = IPDotNUM + i + 1;				   // �ҵ���һ��IP4�ĵ�2��IP�����д��λ��
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+04,IP4:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				for (i = 0; i <= 4; i++) // ip4�ĵ�2λȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == '.') // ˵��ip3�ĵ�2λ���ݳ����ַ�������û����
//					{
//						Pw_Link4IP2 = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = IPDotNUM + i + 1;				   // �ҵ���һ��IP4�ĵ�3��IP�����д��λ��
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+04,IP4:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				for (i = 0; i <= 4; i++) // ip4�ĵ�3λȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == '.') // ˵��ip4�ĵ�3λ���ݳ����ַ�������û����
//					{
//						Pw_Link4IP3 = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = IPDotNUM + i + 1;				   // �ҵ���һ��IP4�ĵ�4��IP�����д��λ��
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+04,IP4:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				for (i = 0; i <= 4; i++) // ip4�ĵ�4λȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == ':') // ˵��ip4�ĵ�4λ���ݳ����ַ�������û����
//					{
//						Pw_Link4IP4 = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = IPDotNUM + i + 1;				   // �ҵ���һ��IP4�Ķ˿������д��λ��
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+04,IP4:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				for (i = 0; i <= 5; i++) // ip4�Ķ˿�ȡ��16������
//				{
//					if (SMSbuf[11 + IPDotNUM + i] == ';') // ˵��ip4�Ķ˿����ݳ����ַ�������û����
//					{
//						Pw_Link4Port = atoi(&SMSbuf[11 + IPDotNUM], i); // ת��Ϊ16����
//						IPDotNUM = 0;									// �ҵ���һ��IP4�����д��λ����0
//						NowWriteOK = 1;
//						break; //
//					}
//				}
//				if (NowWriteOK == 0) // û��д��ȥ
//				{
//					sprintf((char *)p_TmpBuf, "GPW+04,IP4:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//					j = 1;
//				}
//				NowWriteOK = 0; // ��һ�������ж���
//			}
//			if (j == 0)
//			{
//				SPI_FMRAM_BufferWrite((u8 *)(&Pw_ParLst_GPRS[0]), 0, FLASH_PAR_SIZE * 2); // FLASH_PAR_SIZE 256���趨����һ��������
//				sprintf((char *)p_TmpBuf, "GPW+04,IP4:\"%s\"", SMS_SetOk);				  // дҪ���ص�DTU���ݡ�

//				F_RunAgain = 1;		   // ��������ģ��
//				sim_ready = 0;		   // ���sim��׼������
//				F_RunCheckModular = 1; // ���й����м��ģ���־
//			}
//			if (j == 1)
//			{
//				Pw_Link4IP1 = 0;
//				Pw_Link4IP2 = 0;
//				Pw_Link4IP3 = 0;
//				Pw_Link4IP4 = 0;
//				Pw_Link4Port = 0;
//				j = 0; // �¸��������м����
//			}
//		}
//		else
//		{
//			sprintf((char *)p_TmpBuf, "GPW+04,IP4:\"%s\"", SMS_SetERROR); // дҪ���ص�IP4���ݴ���
//		}
//		sim7600ce_sms_send(p_TmpBuf); // ���ض���
//	}
//	else if (strstr((const char *)SMSbuf, "GPW+05,")) // ���д����RF�ĵ�ַ������(GPW+05,24;)
//	{
//		if (SMSbuf[7] >= '0' && SMSbuf[7] <= '9' && SMSbuf[6] == ',')
//		{
//			j = 0;					 // �����
//			NowWriteOK = 0;			 // �����
//			for (i = 0; i <= 4; i++) // RF�ĵ�ַȡ��16������
//			{
//				if (SMSbuf[7 + i] == ';') // ˵��RF��ַ���ݳ����ַ�������û����
//				{
//					Pw_RFModularAddress = atoi(&SMSbuf[7], i); // RF��ַת��Ϊ16����
//					NowWriteOK = 1;
//					break; //
//				}
//			}
//			if (NowWriteOK == 0) // û��д��ȥ
//			{
//				sprintf((char *)p_TmpBuf, "GPW+05,RF Address:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//				j = 1;
//			}
//			NowWriteOK = 0; // ��һ�������ж���
//			if (j == 0)
//			{
//				SPI_FMRAM_BufferWrite((u8 *)(&Pw_ParLst_GPRS[0]), 0, FLASH_PAR_SIZE * 2); // FLASH_PAR_SIZE 256���趨����һ��������
//				sprintf((char *)p_TmpBuf, "GPW+05,RF Address:\"%s\"", SMS_SetOk);		  // дҪ���ص�DTU���ݡ�

//				F_RunAgain = 1;		   // ��������ģ��
//				sim_ready = 0;		   // ���sim��׼������
//				F_RunCheckModular = 1; // ���й����м��ģ���־
//			}
//			j = 0;
//		}
//		else
//		{
//			sprintf((char *)p_TmpBuf, "GPW+05,RF Address:\"%s\"", SMS_SetERROR); // дҪ���صĺ�紫��Э�����ݴ���
//		}
//		sim7600ce_sms_send(p_TmpBuf); // ���ض���
//	}
//	else if (strstr((const char *)SMSbuf, "GPW+06,")) // ���д����DTU��
//	{
//		if (SMSbuf[7] >= '0' && SMSbuf[7] <= '9' && SMSbuf[18] == ';')
//		{
//			Pw_DTUIDNUM1 = atoi(&SMSbuf[7], 4);										  // ����Pw_DTUIDNUM1
//			Pw_DTUIDNUM2 = atoi(&SMSbuf[11], 4);									  // ����Pw_DTUIDNUM2
//			Pw_DTUIDNUM3 = atoi(&SMSbuf[15], 3);									  // ����Pw_DTUIDNUM3
//			SPI_FMRAM_BufferWrite((u8 *)(&Pw_ParLst_GPRS[0]), 0, FLASH_PAR_SIZE * 2); // FLASH_PAR_SIZE 256���趨����һ��������
//			sprintf((char *)p_TmpBuf, "GPW+06,DTU:\"%s\"", SMS_SetOk);				  // дҪ���ص�DTU���ݡ�

//			F_RunAgain = 1;		   // ��������ģ��
//			sim_ready = 0;		   // ���sim��׼������
//			F_RunCheckModular = 1; // ���й����м��ģ���־
//		}
//		else
//		{
//			sprintf((char *)p_TmpBuf, "GPW+06,DTU:\"%s\"", SMS_SetERROR); // дҪ���ص�DTU���ݴ���
//		}
//		sim7600ce_sms_send(p_TmpBuf); // ���ض���
//	}
//	else if (strstr((const char *)SMSbuf, "GPW+07,")) // ���д����͸�����Ǻ�紫��Э�顣=1Ϊ���Э�飬=0Ϊ��ͨ͸��������(GPW+07,01;)
//	{
//		if (SMSbuf[7] >= '0' && SMSbuf[7] <= '9' && SMSbuf[6] == ',')
//		{
//			j = 0;					 // �����
//			NowWriteOK = 0;			 // �����
//			for (i = 0; i <= 4; i++) // ͸�����Ǻ�紫��Э��ģʽȡ��16������
//			{
//				if (SMSbuf[7 + i] == ';') // ˵��͸�����Ǻ�紫��Э��ģʽ���ݳ����ַ�������û����
//				{
//					Pw_GPRSRTModeSelect = atoi(&SMSbuf[7], i); // ͸�����Ǻ�紫��Э��ģʽת��Ϊ16����
//					NowWriteOK = 1;
//					break; //
//				}
//			}
//			if (NowWriteOK == 0) // û��д��ȥ
//			{
//				sprintf((char *)p_TmpBuf, "GPW+07,HongDianMode:\"%s\"", SMS_SetERROR); // дҪ���ص�IP1���ݴ���
//				j = 1;
//			}
//			NowWriteOK = 0; // ��һ�������ж���
//			if (j == 0)
//			{
//				SPI_FMRAM_BufferWrite((u8 *)(&Pw_ParLst_GPRS[0]), 0, FLASH_PAR_SIZE * 2); // FLASH_PAR_SIZE 256���趨����һ��������
//				sprintf((char *)p_TmpBuf, "GPW+07,HongDianMode:\"%s\"", SMS_SetOk);		  // дҪ���ص�DTU���ݡ�

//				F_RunAgain = 1;		   // ��������ģ��
//				sim_ready = 0;		   // ���sim��׼������
//				F_RunCheckModular = 1; // ���й����м��ģ���־
//			}
//			j = 0; // �¸��������м����
//		}
//		else
//		{
//			sprintf((char *)p_TmpBuf, "GPW+07,HongDianMode:\"%s\"", SMS_SetERROR); // дҪ���صĺ�紫��Э�����ݴ���
//		}
//		sim7600ce_sms_send(p_TmpBuf); // ���ض���
//	}
//}

//// GPRSָʾ�����й����е�״̬
////
// void sim7600ce_GPRSLEDRUN(void)
//{
//	//	if (T_GPRSLEDTime != SClk10Ms) // GPRSָʾ�����й����е�״̬,ÿ0.5���л�һ��״̬
//	//	{
//	//		T_GPRSLEDTime = SClk10Ms; //
//	//		C_GPRSLEDTime++;
//	//		if (C_GPRSLEDTime >= 40) // 0.4S
//	//		{
//	//			T_GPRSLEDTime = 100;
//	//			C_GPRSLEDTime = 0;
//	//			LEDGPRS = ~LEDGPRS; //��Դ����˸
//	//		}
//	//	}

//	// ZCL 2017.5.25 �ź��������õ�ʱ����˸
//	if (C_NetLedLightCSQ > 300 && (w_GprsCSQ < 8 || w_GprsCSQ == 99))
//	{
//		C_NetLedLightCSQ = 0;
//		if (NET_STA > 0)
//			NET = 0; //=1,��=0,��
//		else
//			NET = 1; //=1,��=0,��
//	}
//	else if (w_GprsCSQ >= 8 && w_GprsCSQ != 99)
//		NET = 1; //=1,��=0,��

//	// GPRSģ��ģ�PWR.DATA.NET ָʾ�ƵĶ��� 2013.9.1
//	// S_M35������PWR.DATA.NET�ƣ���Ϊ��ͬ�Ľ׶Σ�ָʾ�Ƶ���˸�в�ͬ�ĺ��壩
//	//	if(S_M35<0x11)
//	if (F_RunAgain == 1)
//	{
//		if (S_check_status < 7)
//		{
//			PWR_LIGHT = 1; //=1,��=0,��
//			// NET=1;					//=1,��=0,��   ZCL 2017.5.25  ��NETָʾ������˸ָʾ�źŲ��õ�ʱ��

//			// 1. DATA����˸��������������
//			if (C_LedLight > 300)
//			{
//				C_LedLight = 0;
//				if (COM2_DATASTA == 1)
//					COM2_DATA = 0; //=1,��=0,��
//				else
//					COM2_DATA = 1; //=1,��=0,��
//			}
//		}
//		else
//		{
//			// 2. NET����
//			NET = 0; //=1,��=0,��
//			// 2.1 �ҵ����磬DATAֹͣ��˸
//			COM2_DATA = 1; //=1,��=0,��
//		}
//	}
//	else
//	{
//		// 3.1 PWR��
//		if (CGD0_LinkConnectOK[0] + CGD0_LinkConnectOK[1] + CGD0_LinkConnectOK[2] + CGD0_LinkConnectOK[3] == 0)
//		{
//			// 3.1.1 PWR  ��˸�Ͽ�  ��ʾδ����GPRS��������
//			if (C_LedLight > 50)
//			{
//				C_LedLight = 0;
//				if (PWR_STA > 0)
//					PWR_LIGHT = 0; //=1,��=0,��
//				else
//					PWR_LIGHT = 1; //=1,��=0,��
//			}
//		}
//		else
//		{
//			// 3.1.2 PWR  ��˸����  ��ʾ�ɹ���GPRS��������
//			if (C_LedLight > 1000)
//			{
//				C_LedLight = 0;
//				if (PWR_STA > 0)
//					PWR_LIGHT = 0; //=1,��=0,��
//				else
//					PWR_LIGHT = 1; //=1,��=0,��
//			}
//		}

//		// 3.2 COM2_DATA��
//		if (Cw_Rcv2 + Cw_Txd2 > 0)
//		{
//			// COM2_DATA ��ת  2014.11.27�޸�
//			// ZCL 2020.3.26			GPIO_WriteBit(GPIOC, GPIO_Pin_12,
//			//		               (BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_12))));		//ZCL 2018.12.4
//			COM2_DATA = 0;
//		}
//		else //=1,��
//			COM2_DATA = 1;

//		/*TXD��RXD���ǵ͵�ƽ��Ч��ƽʱ�ߵ�ƽ 2013.8.30 */
//		// 2014.11.27�޸� COM3_DATA ��ת
//		// 2014.12.11 COM3_DATA����S_M35���ƣ��������ݷ�ת
//		if (Cw_Rcv3 + Cw_Txd3 > 0)
//		{
//			// COM3_DATA ��ת 2014.11.27�޸�
//			// ZCL 2020.3.26		GPIO_WriteBit(GPIOD, GPIO_Pin_5,
//			//  								 (BitAction)((1-GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_5))));		//ZCL 2018.12.4	 �յã�û�������
//			COM3_DATA = 0;
//		}
//		else //=1,��
//			COM3_DATA = 1;

//		// 2016.5.27 ����DATA�����һֱ��10�룬���Ź�������2016.5.27
//		if (T_DataLampAllLight != SClk10Ms)
//		{
//			if (COM2_DATASTA == 0)
//			{
//				T_DataLampAllLight = SClk10Ms;
//				C_DataLampAllLight++;
//				if (C_DataLampAllLight > 2000)
//				{
//					C_DataLampAllLight = 0;

//					//				S_M35 = 0x01;						//ģ��ػ�������
//					LCD_DLY_ms(3000); // ��ʱ����û��ι�����豸��λ�� 2013.7.3
//				}
//			}
//			else if (C_DataLampAllLight > 0)
//				C_DataLampAllLight--;
//		}
//	}
//}

//// ����GPRSģ����յ������ݺ���
// void sim7600ce_DoWithData(void)
//{
//	//	u8 i=0;       						// ��ʱ���� k,s,
//	u16 j, m;
//	uchar s; // ttp:transparent protocol ͸��Э��
//	uint k, len1, len2;
//	u16 *p_wRead;
//	u8 *p_bMove;

//	//=1 DDPЭ�� GprsPar[EnDDP0Base+ChannelNo]
//	if (GprsPar[EnDDP0Base + ChannelNo])
//	{
//		ptr2 = (uchar *)strstr((char *)ReceiveGPRSBuf, "{"); //{��λ�� =0x7B
//		if (ptr2 != NULL)
//		{
//			// 1. DTUע��	 7B������ֽ��ǹ����룬0x81������ע�Ṧ����
//			if (*(ptr2 + 1) == 0x81)
//			{
//				ZhuCeOkFLAG[*(ptr + 10) - 0x30] = 1; // ptr+10
//			}
//			// 2. ��ѯDTU����
//			else if (*(ptr2 + 1) == 0x8B)
//				B_Com3Cmd8B = 1;
//			// 3. ����DTU����
//			else if (*(ptr2 + 1) == 0x8D)
//				B_Com3Cmd8D = 1;
//			// 4. �����������նˣ�DTU����ָ��
//			else if (*(ptr2 + 1) == 0x82)
//			{
//				//			S_M35 = 0x01;
//				F_RunAgain = 1;
//				F_RunCheckModular = 1;
//				B_RestartNo = 1; // ZCL 2019.4.19 ������ţ������ӡ�۲�����
//								 // printf("RestartNo=%d\r\n", B_RestartNo);			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
//			}

//			// 2. ��λ��ͨ��DTU������2��ָ��(���ܲ���Ҫ��������)
//			// UDP: RECV FROM:112.125.89.8:34491 +IPD19 7B 89 00 10 31 33 39 31 32 33 34 35 36 37 38 7B 31 32 33
//			// TCP: RECV FROM:112.125.89.8:34491 +IPD19 7B 89 00 13 31 33 39 31 32 33 34 35 36 37 38 31 32 33 7B
//			else if (*(ptr2 + 1) == 0x89) //&& *(ptr+i+19)==0x7B
//			{
//				B_Com3Cmd89 = 1;
//			}
//			else
//				B_OtherCmd = 1; //=1����λ��ͨ��DTU������2������ָ��Ժ��ټӾ���ָ��

//			C_Com3NoRcv = 0; // ZCL 2017.5.30 COM0 û�н����������ݼ�����
//		}

//		// if(B_Com3Cmd89 || B_OtherCmd) 2014.12.5 �Ƶ�����
//	}

//	// 1.�����ϣ���B_Com3Cmd89.B_OtherCmd����ת��(DDPЭ�����Ҫ����,͸��Э���ֱ�Ӵ���)
//	if (B_Com3Cmd89 || B_OtherCmd)
//	{
//		B_Com3Cmd89 = 0;
//		B_OtherCmd = 0;

//		// ���յ�����ֵ���ȣ�DDPЭ������Ҫ��16����������²��ü�
//		len1 = ReceiveGPRSBufOneDateLen + ReceiveGPRSBufTwoDateLen + ReceiveGPRSBufThreeDateLen + ReceiveGPRSBufFourDateLen;
//		if (GprsPar[EnDDP0Base + ChannelNo])
//		{
//			if (len1 >= 16) // ZCL 2019.4.16 ��������
//				len2 = len1 - 16;
//			else
//				len2 = 0;
//		}
//		else
//			len2 = len1;

//		// 2012.10.15 ����DDPЭ��(TCP.UDP���ݸ�ʽ��ͬ)��͸��Э��(TCP.UDPһ��)
//		// 2012.10.10 ����TCPЭ��(=1)��UDPЭ��(=0)
//		// UDP:7B 89 00 10 31 33 39 31 32 33 34 35 36 37 38 7B 31 32 33
//		// UDP˵����7B(��ʼ��־���̶�ֵ) 89(�����ͣ��̶�ֵ) 00 10(�����ȣ�2�ֽڣ��̶�ֵ) 31 33 39 31 32 33 34 35 36 37 38(DTU���ʶ���룬11�ֽ�) 7B(������־���̶�ֵ) 31 32 33(�û����ݣ���1024�ֽ�)
//		// TCP:7B 89 00 13 31 33 39 31 32 33 34 35 36 37 38 31 32 33 7B
//		// TCP˵��:7B(��ʼ��־���̶�ֵ) 89(�����ͣ��̶�ֵ) 00 13(�����ȣ�2�ֽڣ���0x10Ϊʵ��ֵ) 31 33 39 31 32 33 34 35 36 37 38(DTU���ʶ���룬11�ֽ�) 31 32 33(�û����ݣ���1024�ֽ�) 7B(������־���̶�ֵ)
//		for (k = 0; k < len2; k++) // ����3(GPRSģ��)���յ��ķ��͵�����2
//		{
//			//=1 DDPЭ�� GprsPar[EnDDP0Base+ChannelNo]
//			if (GprsPar[EnDDP0Base + ChannelNo])
//			{
//				//=0,UDP
//				if (GprsPar[LinkTCPUDP0Base + ChannelNo] == 0)
//					Txd2Buffer[k] = *(ptr2 + 16 + k); // ȥ��UDP�İ�ͷ������Txd2Buffer
//				//=1,TCP
//				else
//					Txd2Buffer[k] = *(ptr2 + 15 + k); // ȥ��TCP�İ�ͷ������Txd2Buffer
//			}
//			//=0 ͸��Э�� GprsPar[EnDDP0Base+ChannelNo]
//			else
//			{
//				Txd2Buffer[k] = ReceiveGPRSBuf[k]; // ֱ�Ӵ���Txd2Buffer
//			}
//		}

//		// #define	F_GprsTransToCom			MEM_ADDR(BITBAND( (u32)&Pw_GprsSet, 1 ))	// GPRS͸��ģʽ�������COM2		0/1

//		// ZCL 2019.3.11 ����GPRSģʽ�£�Ĭ�ϲ������Ϣ������	0/1
//		// #define	F_GprsMasterNotToCom	MEM_ADDR(BITBAND( (u32)&Pw_GprsSet, 2 ))	// MasterNotToCom

//		// ZCL 2019.3.11 ����GPRSģʽ���趨�����Ϣ������	 2019.3.11
//		// #define	F_GprsMasterToCom			MEM_ADDR(BITBAND( (u32)&Pw_GprsSet, 3 ))	// MasterToCom

//		// ZCL 2019.3.12 ����GPRSģʽ������MODBUS���Ȼ���Լ���������
//		// ע������ʹ�õ���Txd2Buffer���������ġ�����Rcv3Buffer
//		if (F_GprsMasterNotToCom || F_GprsMasterToCom)
//		{
//			if (Txd2Buffer[0] == 2) // �ӵ�ַ��⣭���յ�����λ����ѯָ��
//			{
//				j = CRC16(Txd2Buffer, len2 - 2); // CRC У��
//				k = j >> 8;
//				s = j;
//				if (k == Txd2Buffer[len2 - 2] && s == Txd2Buffer[len2 - 1])
//				{							// CRCУ����ȷ
//					if (Txd2Buffer[1] == 3) // 03��ȡ���ּĴ���
//					{
//						B_Com3Cmd03 = 1;
//						j = Txd2Buffer[2];
//						Lw_Com3RegAddr = (j << 8) + Txd2Buffer[3];
//					}
//					else if (Txd2Buffer[1] == 6) // 06Ԥ�õ��Ĵ���
//					{
//						// C_ForceSavPar=0;		// ǿ�Ʊ������������=0
//						B_Com3Cmd06 = 1;
//						j = Txd2Buffer[2];
//						Lw_Com3RegAddr = (j << 8) + Txd2Buffer[3];

//						// ZCL 2021.7.10  ����Ҫ�޸ĵ�����  ICCARD ȡˮ��
//						// ZCL 2021.11.17

//						// ����Ԥ�ñ����� �趨������ ����������Ҫ����Ԥ�� LORA�ӻ��Ĳ�������Ҫת���ģ�
//						/* 									m=Txd2Buffer[4];
//										w_ParLst[Lw_Com3RegAddr]=(m<<8)+Txd2Buffer[5];	 */

//						// ZCL 2021.11.17 ת��ָ��
//						B_LoraSendWrite = 1; // ZCL 2021.11.17
//					}

//					// ZCL 2021.11.17
//					else if (Txd2Buffer[1] == 16) // 16Ԥ�ö�Ĵ���
//					{
//						// C_ForceSavPar=0;		// ǿ�Ʊ������������=0
//						B_Com3Cmd16 = 1;
//						j = Txd2Buffer[2];
//						Lw_Com3RegAddr = (j << 8) + Txd2Buffer[3];

//						// ����Ԥ�ñ����� �趨������ ����������Ҫ����Ԥ�� LORA�ӻ��Ĳ�������Ҫת���ģ�
//						/* 									if ( Txd2Buffer[5] < 30 )			//ZCL 2021.11.17  ������
//										{
//											p_bGen = Txd2Buffer;
//											p_wTarget = w_ParLst;
//											for ( k = 0; k < Txd2Buffer[5] ; k++ )		// Rcv0Buffer[5]=����
//											{
//												m = *( p_bGen + 7 + k * 2 );
//												n = *( p_bGen + 7 + k * 2 + 1 );
//												*( p_wTarget + Lw_Com3RegAddr + k ) = ( m << 8 ) + n;
//											}
//										} */

//						// ZCL 2021.11.17 ת��ָ��
//						B_LoraSendWrite = 1; // ZCL 2021.11.17
//						B_LoraSendWriteLength = len2;
//						if (B_LoraSendWriteLength > BUFFER_SIZE)
//							B_LoraSendWriteLength = BUFFER_SIZE;	// YLS ���ƴ�С����ֹ�������
//						for (j = 0; j < B_LoraSendWriteLength; j++) // Rcv0Buffer[5]=���� ����ط�Ҫ�����ƽ��յ��ֳ�����ֹ�������鳤��
//						{
//							LoRaTxBuf2[j] = Txd2Buffer[j];
//						}
//					}
//				}
//			}
//		}

//		// ZCL 2019.3.12  ����GPRSģʽ�£�������ָ�׼�����أ�
//		if (B_Com3Cmd03)
//		{
//			Txd3TmpBuffer[0] = 2;				  // �豸�ӵ�ַPw_EquipmentNo
//			Txd3TmpBuffer[1] = Txd2Buffer[1];	  // ������			ZCL 2019.3.12 ����Ƚ����⣬�õ�Txd2Buffer
//			Txd3TmpBuffer[2] = Txd2Buffer[5] * 2; // Rcv2Buffer[5]=���� ��

//			// ת����ַ ZCL 2015.7.11

//			if (Txd2Buffer[5] > 100)
//				Txd2Buffer[5] = 100; // ZCL 2019.4.26 ���ƴ�С����ֹ�������

//			p_wRead = w_GprsParLst; // ��w_GprsParLst PAR��	2019.3.14
//			p_bMove = Txd3TmpBuffer;
//			for (k = 0; k < Txd2Buffer[5]; k++) // ����ѯ����
//			{
//				m = *(p_wRead + Lw_Com3RegAddr + k);
//				*(p_bMove + 3 + k * 2) = m >> 8;
//				*(p_bMove + 3 + k * 2 + 1) = m;
//			}
//			Lw_Txd3ChkSum = CRC16(Txd3TmpBuffer, Txd3TmpBuffer[2] + 3);
//			Txd3TmpBuffer[Txd3TmpBuffer[2] + 3] = Lw_Txd3ChkSum >> 8; // /256
//			Txd3TmpBuffer[Txd3TmpBuffer[2] + 4] = Lw_Txd3ChkSum;	  // ��λ�ֽ�
//			Cw_Txd3TmpMax = Txd3TmpBuffer[2] + 5;
//			//
//			//
//			B_Com3Cmd03 = 0;
//			Cw_Txd3 = 0;

//			// ZCL 2019.3.12 ����ָ��Ƚ���Ҫ��ģ��͸���У������յ����ݣ�ת����GPRS����
//			B_GprsDataReturn = 1; // ģ��͸���У������յ����ݣ�ת����GPRS����

//			// RS485_CON=1;
//			// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
//			// USART_SendData(USART2,Txd2Buffer[Cw_Txd2++]);
//			// USART_ITConfig(USART2, USART_IT_TC, ENABLE);				// ��ʼ����.

//			// UART2_Output_Datas(Txd3TmpBuffer, Cw_Txd3TmpMax);
//		}

//		// ZCL 2019.3.12 ����������ָ��  2021.11.17
//		else if (B_Com3Cmd06) // ZCL 2021.7.10  06ָ��յ��� ��  ���ص� ��һ���ġ�
//		{
//			B_Com3Cmd06 = 0;
//			//...
//			// ZCL 2021.7.10  06ָ��յ��� ��  ���ص� ��һ���ġ�
//			Txd3TmpBuffer[0] = 2;			  // �豸�ӵ�ַPw_EquipmentNo
//			Txd3TmpBuffer[1] = Txd2Buffer[1]; // ������			ZCL 2019.3.12 ����Ƚ����⣬�õ�Txd2Buffer
//			Txd3TmpBuffer[2] = Txd2Buffer[2]; // ��
//			Txd3TmpBuffer[3] = Txd2Buffer[3]; //
//			Txd3TmpBuffer[4] = Txd2Buffer[4]; //
//			Txd3TmpBuffer[5] = Txd2Buffer[5]; //

//			Lw_Txd3ChkSum = CRC16(Txd3TmpBuffer, 6);
//			Txd3TmpBuffer[6] = Lw_Txd3ChkSum >> 8; // /256
//			Txd3TmpBuffer[7] = Lw_Txd3ChkSum;	   // ��λ�ֽ�
//			Cw_Txd3TmpMax = 8;
//			//
//			//
//			B_Com3Cmd06 = 0;
//			Cw_Txd3 = 0;

//			// ZCL 2019.3.12 ����ָ��Ƚ���Ҫ��ģ��͸���У������յ����ݣ�ת����GPRS����
//			B_GprsDataReturn = 1; // ģ��͸���У������յ����ݣ�ת����GPRS����

//			// RS485_CON=1;
//			// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
//			// USART_SendData(USART2,Txd2Buffer[Cw_Txd2++]);
//			// USART_ITConfig(USART2, USART_IT_TC, ENABLE);				// ��ʼ����.
//		}

//		// ZCL 2019.3.12 ����������ָ��  2021.11.17
//		else if (B_Com3Cmd16) // ZCL 2021.7.10  06ָ��յ��� ��  ���ص� ��һ���ġ�
//		{
//			B_Com3Cmd16 = 0;
//			//...
//			// ZCL 2021.7.10  06ָ��յ��� ��  ���ص� ��һ���ġ�
//			Txd3TmpBuffer[0] = 2;			  // �豸�ӵ�ַPw_EquipmentNo
//			Txd3TmpBuffer[1] = Txd2Buffer[1]; // ������			ZCL 2019.3.12 ����Ƚ����⣬�õ�Txd2Buffer
//			Txd3TmpBuffer[2] = Txd2Buffer[2]; // ��
//			Txd3TmpBuffer[3] = Txd2Buffer[3]; //
//			Txd3TmpBuffer[4] = Txd2Buffer[4]; //
//			Txd3TmpBuffer[5] = Txd2Buffer[5]; //

//			Lw_Txd3ChkSum = CRC16(Txd3TmpBuffer, 6);
//			Txd3TmpBuffer[6] = Lw_Txd3ChkSum >> 8; // /256
//			Txd3TmpBuffer[7] = Lw_Txd3ChkSum;	   // ��λ�ֽ�
//			Cw_Txd3TmpMax = 8;
//			//
//			//
//			B_Com3Cmd16 = 0;
//			Cw_Txd3 = 0;

//			// ZCL 2019.3.12 ����ָ��Ƚ���Ҫ��ģ��͸���У������յ����ݣ�ת����GPRS����
//			B_GprsDataReturn = 1; // ģ��͸���У������յ����ݣ�ת����GPRS����

//			// RS485_CON=1;
//			// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
//			// USART_SendData(USART2,Txd2Buffer[Cw_Txd2++]);
//			// USART_ITConfig(USART2, USART_IT_TC, ENABLE);				// ��ʼ����.
//		}

//		// ZCL 2019.3.12 	�Խ��յ������ݣ����д������
//		if (F_GprsTransToCom || F_GprsMasterToCom)
//		{
//			Cw_Txd2Max = len2;
//			Cw_Txd2 = 0;

//			// RS485_CON=1;		//2013.9.2
//			//  2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
//			// USART_ITConfig(USART2, USART_IT_TXE, ENABLE);				// ��ʼ����.
//			USART_ITConfig(USART2, USART_IT_TC, ENABLE); // ��ʼ����.
//			USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]);
//			//			UART2_Output_Datas(Txd2Buffer, Cw_Txd2Max); // ���͵�COM2
//		}
//		//		F_GPRSSendFinish = 0;
//		//		sim7600ceGPRSUpLoaded(Txd3TmpBuffer, Cw_Txd3TmpMax, 1);
//	}

//	// 2.1 (�����ӣ���B_Com3Cmd89.B_OtherCmd�������1����ת��������2) Ŀǰû����
//	// 2.2 �����ӣ�һЩATָ��ؽ��������1����ת��������2 2013.5.20
//	// ZCL 2019.4.23 ��������ݴ���ת���꣬�ٽ���printf��Ϣ���
//	else if (GprsPar[ConsoleInfoTypeBase]) // ����̨��Ϣ����,=1���������Ϣ
//	{
//		if (Cw_BakRcv3 >= 512)
//			Cw_BakRcv3 = 500; // ZCL 2019.4.26 ���ƴ�С����ֹ�������

//		for (k = 0; k < Cw_BakRcv3; k++)
//		{
//			Txd2Buffer[k] = Rcv3Buffer[k]; // ����3(GPRSģ��)���յ��ķ��͵�����2
//		}
//		Cw_Txd2Max = Cw_BakRcv3;
//		Cw_Txd2 = 0;
//		RS485_CON = 1; // 2013.9.2
//		// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
//		// USART_ITConfig(USART2, USART_IT_TXE, ENABLE);				// ��ʼ����.
//		USART_ITConfig(USART2, USART_IT_TC, ENABLE); // ��ʼ����.
//		USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]);
//		//		UART2_Output_Datas(Txd2Buffer, Cw_Txd2Max); // ���͵�COM2
//	}

//	// 2.���ݷ���
//	Cw_Txd3Max = 0; // 2014.12.8
//	// 2012.10.4 ����DDPЭ���͸��Э���ѡ��
//	//=1��DDPЭ��
//	if (GprsPar[EnDDP0Base + ChannelNo]) //=1��DDPЭ�飻=0��͸��Э��
//	{
//		Gprs_TX_Fill(GPRS_ZhenTou, 16); // DDPЭ��
//		// �޸�DTU���ʶ����
//		for (k = 0; k < 11; k++)
//			Txd3Buffer[4 + k] = GprsPar[DtuNoBase + k];

//		// 2012.10.10 ����TCPЭ���UDPЭ�������
//		// UDP: 7B 09 00 10 31 33 39 31 32 33 34 35 36 37 38 7B 61 62 63
//		// TCP: 7B 09 00 13 31 33 39 31 32 33 34 35 36 37 38 61 62 63 7B

//		// TCP
//		if (GprsPar[LinkTCPUDP0Base + ChannelNo]) //=1��TCPЭ�飻=0��UDPЭ��
//		{
//			Cw_Txd3Max = 15; // �޸�Cw_Txd3Maxֵ��16�ĳ�15�� 2012.10.10

//			// ZCL 2019.3.12 �����ƣ�͸����ԭ�ȳ���������GPRSģʽ���³���������ݲ�һ����
//			if (F_GprsTransToCom)
//			{
//				Gprs_TX_Fill(Rcv2Buffer, Cw_BakRcv2); // �������
//				Gprs_TX_Byte(0x7B);
//				// 2012.10.10 �޸ķ��ͳ��ȣ�ʹ��TCPЭ�飬�򳤶�Ϊ���з��������ܳ��ȣ� �ܳ��ڣ�
//				Txd3Buffer[3] = 16 + Cw_BakRcv2;
//			}

//			// ZCL 2019.3.12
//			else if (F_GprsMasterNotToCom || F_GprsMasterToCom)
//			{
//				Gprs_TX_Fill(Txd3TmpBuffer, Cw_Txd3TmpMax); // ������� ZCL 2019.3.12

//				Gprs_TX_Byte(0x7B);
//				// 2012.10.10 �޸ķ��ͳ��ȣ�ʹ��TCPЭ�飬�򳤶�Ϊ���з��������ܳ��ȣ� �ܳ��ڣ�
//				Txd3Buffer[3] = 16 + Cw_Txd3TmpMax;
//			}
//		}

//		// UDPʱ
//		else
//		{
//			// Gprs_TX_Fill(Rcv2Buffer,Cw_BakRcv2);		//�������
//			// ZCL 2019.3.12 �����ƣ�͸����ԭ�ȳ���������GPRSģʽ���³���������ݲ�һ����
//			if (F_GprsTransToCom)
//				Gprs_TX_Fill(Rcv2Buffer, Cw_BakRcv2); // �������

//			else if (F_GprsMasterNotToCom || F_GprsMasterToCom)
//				Gprs_TX_Fill(Txd3TmpBuffer, Cw_Txd3TmpMax); // ������� ZCL 2019.3.12
//		}
//	}
//	//=0��͸��Э��
//	else // ͸��Э�鲻�ü�ͷ��Ҳ����TCP,UDP����
//	{
//		// Gprs_TX_Fill(Rcv2Buffer,Cw_BakRcv2);		//����TCP,UDP����

//		// ZCL 2019.3.12 �����ƣ�͸����ԭ�ȳ���������GPRSģʽ���³���������ݲ�һ����
//		if (F_GprsTransToCom)
//			Gprs_TX_Fill(Rcv2Buffer, Cw_BakRcv2); // �������

//		else if (F_GprsMasterNotToCom || F_GprsMasterToCom)
//			Gprs_TX_Fill(Txd3TmpBuffer, Cw_Txd3TmpMax); // ������� ZCL 2019.3.12
//	}

//	F_GPRSSendFinish = 0;
//	sim7600ceGPRSUpLoaded(Txd3Buffer, Cw_Txd3Max, 1);

//	memset(ReceiveGPRSBuf, 0, RCV3_MAX);
//	memset(Rcv3Buffer, 0, RCV3_MAX);
//}

// void UART1_Output_Datas(uint8_t S_Out[], uint8_t Counter)
//{
//	uint8_t i;
//	for (i = 0; i < Counter; i++)
//	{
//		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
//			; // �ȴ����ͽ���
//		USART_SendData(USART1, S_Out[i]);
//	}
// }

// void UART2_Output_Datas(uint8_t S_Out[], uint8_t Counter)
//{
//	uint8_t i;
//	for (i = 0; i < Counter; i++)
//	{
//		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET)
//			; // �ȴ����ͽ���
//		USART_SendData(USART2, S_Out[i]);
//	}
// }

// void UART3_Output_Datas(uint8_t S_Out[], uint16_t Counter)
//{
//	uint8_t i;
//	for (i = 0; i < Counter; i++)
//	{
//		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET)
//			; // �ȴ����ͽ���
//		USART_SendData(USART3, S_Out[i]);
//	}
//	//        uart_putchar(databuf[i]); //LINK TO UART
// }

//// sim7600ce�����г���
////
// void sim7600ce_GPRSRUN(void)
//{
//	Fill_data();
//	sim7600ce_NetworkCheck();		  // ���й����м�������Ƿ���������������������
//	sim7600ce_connectstaModularNum(); // ���ģ����Ҫ���ӵ�IP����
//									  //	sim7600ce_SendHeartbeat0();		  // ����ģ������
//	sim7600ce_SendHeartbeat();		  // ����ģ������
//	sim7600ce_ReconnectNet();		  // ���ģ���Ƿ���Ҫ��������
//	sim7600ce_ReceiveData();		  // ����GPRS������������
//									  //	Com3_SlaveSend();
//	sim7600ce_GPRSLEDRUN();			  // GPRSָʾ���л�
//	sim7600ce_smsread_UpLoaded();	  ////���Ŷ����ݼ���
//	sim7600ce_smsWrite_UpLoaded();	  // ����д���ݼ���
// }
