/**
  ******************************************************************************
  * @file    com3_232.c
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.27
  * @date    2014-01-03
  * @brief   COM0 USE USART3
	* YLS
	******************************************************************************
	* @note	2014.12.3
	COM3: GPRS SIM7600CE 4Gģ��

	SIM7600CE˵��:
	1. ��������4�ı��ض˿ںŲ��������Դ򿪻��ԣ�=11011ʱ
		 ��������4��������ǰ���@Ϊ�绰���룬�����ط����š� 2014.12.7
		 3E�� >��

	2. �˴���1��MCU�� SIM7600CE 4Gģ��ͨѶ�ڣ�

	3. �����Ƿ������������ģ���DTU����ȥ����Ӧ�ļ�ȥ0x80����ȫ����������
		 A. ������ע�Ṧ���� 0x81
		 B. ��ѯDTU���� 0x8B
		 C. ����DTU���� 0x8D
		 D. �����������նˣ�DTU����ָ�� 0x82

	4. B_RunGprsModule	����GPRSģ���־��(û�����ı�ͨѶ��4����ֵ=1)��
		 B_ATComStatus 		AT����״̬��ע��@11#���룻@22#�˳���
		 B_PcSetGprs			PC����GPRS��־��=1ʱ�����յ�����(���ò�ѯGPRS����)����=2��
						����Sms_SetupInq()�����������������=1��Ȼ���ٵȴ����մ�����һ����Ϣ��
						ע�⣺@33#���룻@44#�˳���
		 B_SmsRunStop		���ſ�����ͣ���豸ͣ������	=1����һ����ָͣ�����B_SmsRunStop=2��
						û���յ�Ӧ����ʱ������10�Σ��յ�Ӧ������=3����=0��Ȼ�����Ӧ��
		 B_GprsDataReturn			GPRS ���ݷ��أ������ӣ��յ�����2���ݣ���ͨ��DTU���ͳ�ȥ
						Զ����λ�����͸�DTU����ָ��ķ��أ��Ѿ����ӵ�ÿ·���Ӷ�����

	5.//2014.12.9 �յ�+CMTI(�¶�����ʾ)������ȥ�����ţ����������ܻ����յ��¶���
		��ʾ��M35������BUG������ͬһ�����ţ�
		+CMTI		�¶���ָʾ
		+CMGR		�����Ķ������ݣ�Ȼ����SmsAndPC_SetupInq()��������
		+CMGS:247  �����Ѿ����ͣ���AT+CMGS="15610566803"��

	6. ���ſ��ƹ��ܣ�
		0x82: ����  0x91��ģ���ϵ�����
		0x92: �豸��ͣ(=0,����; =1,ֹͣ) 0x95:���Ź���λ����

	7. �������Ͷ��Ż�õ绰���룺//�������Ͷ���(�յ�����4��������@�绰�����) 2014.12.7
		uchar B_ZhuDongSendSms;
		2014.12.7 ��������4��������ǰ���@Ϊ�绰���룬�����ط����š�
		�磺@15610566803  GP311����15610566803�ط����ţ�	����������11λ���ֻ��绰����

	GP311˵��: ( main.c )
	//--------------------------------
	1. ����ʱ�ѿ��Ź���ʼ��ע�͵����õ��豸��ʱ�򿪡�
	2. B_RunGprsModule=0��4��û�з��ʣ�B_RunGprsModule=1���˳����ı���������������
	3.	0x00,0x00,0x25,0x80,0x03,0x00,0x00,0x03,						//0D���ڹ�����ʽ
		//1.��������
		//2. 03 ����λΪ8λ��00-5�� 01-6�� 02-7�� 03-8����������ֻ������03��[00-5���� 00Ϊ����ֵ��5Ϊ�ڼ������е���ʾ���ݣ������Դ�����]
		//3. 00 ֹͣλΪ1λ��00-1��04-2�����������������00��04��
		//4. 00 У��λ-�ޣ�00-�ޣ�08-�棬18-ż�����������������00��08��18��
		//5. 03 ����-�����أ�01-Xon/Xoff��02-Ӳ���أ�03-�����أ�04-��˫��485��05-ȫ˫��422��
	4. �������úͲ�ѯ���ڹ�����ʽʱ��
		��Ϊ�������ֻ�����дʱ����ASCII���ʽ�����ԣ�1������ʱ�����ڼ������е���ʾ���ݵ�
		ASCII���ʽ��д���ͣ�GPRSģ���յ���ת������ֵ������������2����ȡʱ��GPRS�е�����ֵ
		ת���ڼ������е���ʾ���ݵ�ASCII���ʽ��д���͡�����//4  //5 �к��ֳ���
		1.���ڲ����� 2.����λ 3.ֹͣλ 4.У��λ 5.����
		GPW+0D,19200,8,1,0,3;
	5. 2013.12.18 �޸�xmodem�ļ���
		(1)���ӿ�ʼ�������������Ϣ��
		-----Begin; -----Result info; -----End
		(2)���Ӵ����ʲ���������ֵ���ݴ������ַ�������(01.05.06.0x21.0x31.0x41.0x51)���ȵ�����
		(3)����������i=FindFirstValue(ptr,0x2C ,0x00); ����==255�����ƣ���ֹû���ҵ�0x2C(,)
	6. GP311ʹ��8M*4=32MƵ�ʣ�system_stm32f10x.c��SystemInit()�������ϵ�󣬻��������ļ�.s
		����ִ���趨Ƶ�ʣ��˴�û�к���Ƶ�ʣ�����ע�͵�72M����main.c��ʹ��RCC_Configuration()
		�����趨Ƶ�ʡ�
		//#define SYSCLK_FREQ_72MHz  72000000		//2013.1.3ע�͵��������ܳ���
	7. �������Ͷ��Ų��ɹ�����������Ϊ��
		//2014.12.9 !!! ����ĩβ�� ; 0x1A   0x0d
		Txd0Buffer[Txd0Max++]=';';
		Txd0Buffer[Txd0Max++]=0x1A;				//CTRL+Z����(0x1A)
		Txd0Buffer[Txd0Max++]=0x0d;

		��������4�ı��ض˿ںŲ��������Դ򿪻��ԣ�=11011ʱ
	  ��������4��������ǰ���@Ϊ�绰���룬�����ط����š� 2014.12.7
		�磺@15610566803  GP311����15610566803�ط����ţ�	����������11λ���ֻ��绰����

	8.	2014.12.11 ����������ʱ���֡��������óɹ����ȷ�����Ų��ڡ�DSC_DEMO�������ʾ������Ӹ�
			��ʱ���÷�����žͿ�����ʾ���Ե�������
		����취�����룺
		else if(S_DtuSetSend==4 && C_BetweenDtuSetSend>300)
	9. ��λ��������DTU���ڷ����ݣ�ORE������������ stm32f10x_it.c
	10. ���Ӻ����ı��������ݣ���ʱ���ͼ��ξͲ��ܷ����ˣ�
		ԭ��At_QISENDAccordingLength ���ͺ������ղ���'>'Ӧ�����
		����취���ղ���'>'Ӧ�����300MS���·� At_QISENDAccordingLength
		�ּ��ϣ�  �ղ���"OK"Ӧ�����1000MS���·����������棬��������γ��򣬿���OK�����յ���

	11. V129�汾��150105-V4 V129 IAR6.4 MDK5.01 �ȴ�CallReady�Ӵ� 100 �룬���15����������������OK
	12. V130�汾��150117-V4 V130 IAR6.4 MDK5.01 CREGע�᲻�ɹ��Ӵ� 95 �룬���15����������������OK

	13. V131�汾��
			DoWith.c
			A. ȡ��Gprs_ParBootLst�����Ķ���Gprs�������ٳ�ʼ��������ֹGPRS����ID��Ϊ��ʼ���ı䣩//ZCL 2016.5.26
			B. Pw_OutPDecMax= 991;  //ZCL 2016.5.26 ���ϵ���һ����־  ���Թۿ���ʲô��ɵĳ�ʼ��

			com0_232.c
			C. �޸ģ���1������ע�᲻�ɹ�������  2016.5.24 ZCL �����ۼ�·��������Ҳ��ᵼ��һ·���Ӳ��ϣ�Ƶ������
				��2���������Ӳ��ɹ�������������ǰ��Ƶ�����ӣ�

			D. ����������˵��  2016.5.24 ZCL
					B_SmsOrFaultHardRestart=11;		//=11����Ӳ������; =1����Ӳ������
				��1����2�ν��յ����루���������ݣ���������
				��2����2�γ���"CONNECT FAIL"�󣬾�����
				��3�����յ���ERROR��������
				��4���޸�24��34��44��54�ֱ����1,2,3,4ͨ����������ʽ��0-UDP��1-TCP��������
				��5��������¼DSC����(5��)���ϱ�־������ע�᲻�ɹ����������� 1·���ӣ���ǰ��1·ʱ������
				//2016.7.5 ȡ����6��������¼DSC����(5��)���ϱ�־������ע�᲻�ɹ����������� 2·��������
				��7����10�ι��ϱ�־���������Ӳ��ɹ�������������ģ��
				��8�������޸�24��34��44��54�ֱ����1,2,3,4ͨ����������ʽ��0-UDP��1-TCP��������

				��9������ָ������

			E.	2016.5.24 ZCL ��ע˵���£���ǰ���д˳���
				if(B_SmsOrFaultHardRestart==1 )
				{
					B_SmsOrFaultHardRestart=2;
					S_M35 = 0x01;						//ģ��ػ�������
				}
				else if(B_SmsOrFaultHardRestart==11 )
				{
					B_SmsOrFaultHardRestart=12;
					S_M35 = 0x01;						//ģ��ػ�������
				}
			F. 	DATA��һֱ�����⡣??? ����DATA�����һֱ��20�룬���Ź�������2016.5.27

	14. ��ע�����÷���������ʱ��Ҫ��main()��B_LockCodeOK=1; ����������У�����˼��
			GetLockCode(void)		// 2013.10.26 �õ��������룻
			�ѻ������д����ʱҲ����ָ��λ��д��һ�����룻
			�������⵽����������ָ��λ�õ�����һ��ʱ��B_LockCodeOK=1��
			ע�����ʹ�÷�������û����B_LockCodeOK=1�������벻�ԣ��ᵼ�´������ݷ���ģ�飬DATA�ƾ�һֱ����������
			��Ҫʱ���԰����뱣��ȡ������

	15. ��ע���޸ĳ����ǵ��޸�com0_232.c��  Զ�̲�ѯGPRSģ��İ汾��
			uc8 DtuProgVersion[7]="1.31.00";					//7 DTU����汾:70
			uc8 DtuProgMakeDate[8]="20160527";				//8 DTU������������:71

	16. ��ע���ı���ʾ�����ϵ�ǰ���ϣ������ͨѶ�ϡ��������Ժ��ٲ����ı���ʾ��������֧�ֲ����趨��
			��ʱ���൱���ı���ʾ��������ָ���ҪGPRSģ������Զ���ˡ�

	17. V131-2 ZCL 2016.7.1
			Com1_RcvProcess()��
			//2014.12.9 ��������������ת��ʱ��ֹ���ա�
			//2016.7.1 ZCL ��Ϊ��������ļ��룬��������ڸ�ÿ·���ӷ�����Ϻ󣬻��ô򿪽��ա�������
			if(B_GprsDataReturn==0)
			{
				USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
				USART3->CR1 |=0x0004;  //.3λ  ���н���
			}

		(1)	M35_Init()�У�
			//2016.7.1 ZCL �򿪽��գ���ֹû�п������жϣ����½��ղ������ݡ�
			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
			USART3->CR1 |=0x0004;  //.3λ  ���н���

		(2) //M35�Ĳ���ѭ��
		switch(S_M35)
		{
			case 0x00:
			case 0x01:  //����.�ػ�.����
									M35_Power_ONOFF();
									if(S_M35PowerOnOff==2 && STATUS_4G)
									{
										S_M35PowerOnOff=0;
										C_M35SendSum = 0;
										S_M35 = 0x03;

										// Enable USART1 Receive and Transmit interrupts
										USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //��ʱ�����գ���ֹ���յ�����

										//2016.7.1 ZCL �򿪽��գ���ֹû�п������жϣ����½��ղ������ݡ�
										USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
										USART3->CR1 |=0x0004;  //.3λ  ���н���
									}
		18.		// ZCL 2016.7.5 (������ڲ�ͣͨѶ���⣬ɾ�����ı�ͨѶ����)				ȡ�������ı�ͨѶ���ܡ�������GP311����ʱ�����յ�SM510���������ͣ�GP311�ط���SM510�յ����ٴη��͵���ѭ����
		ע���ڵ���ʱ���Դ����ı�ͨѶ���ܣ�����Ӧ��ʱ����ע�͵��˹��ܣ��������

		19. // ZCL 2016.7.5
		ע: �ڷ������ʱ��Ҫ��Boot_ParLst()�еĶ�ȡGPRS�������ܣ�������Ϊһ���س���ͻ�ѱ�����FLASH����GPRS ����ɾ���������޷����ԡ�
		   ��������Ӧ���У�����ȡ����ȡGPRS��������ֹ����δ֪�ĳ�ʼ����

		20. // ZCL 2017.5.25  -- 2017.5.31
		ע: (1) case 6�����ź�������ԭ��case 6���ƣ��ź���������>=8����NET����ָʾ�ź���������˸�����źŲ��С�
				��0x13:��������ʱ����  �������ź�����.
				(2)�ź��������õ�ʱ��NET��˸
				SM510��600���������ͣ� SM510��600���GPRS�ϵ��ϵ磨�źŲ���ʱ��

				(3)  ����������������¼���ź�������
				��ע��ʱ�̼��CSQ�ź���������û�мӣ���д��ȽϷ�����������ϣ�
				��ע����ʱ����û�мӣ�����������㹻�࣬��ʱ������Ѻõ�״̬Ҳ�ƻ����������ã�
					DoWith��  ������������������
					//ZCL 2017.5.26 COM1��ֹ���պ�80S��û�лָ����ա�����
					//.3λ  =0 ��ֹ����
					if( (USART3->CR1 && 0x0004)==0
						|| B_Uart0Cmd8B + B_Uart0Cmd8D + B_GprsDataReturn + B_ATComStatus
						+ B_Uart0Cmd89 + B_OtherCmd
						+ HeartSendFault + LinkFault
						+ S_SendSms+S_ReadNewSms+S_SmsFull>0)

					//ZCL 2017.5.30  com1���ղ������ݣ�660S��ʱ����
					//ZCL 2017.5.30  com0���ղ����������ݣ�660S��ʱ����


		//--------ZCL 2019.4.9 --- 2019.4.15  DCM220_SRC_E GPRS����  M35ģ�飬����ģ����ߣ����������⡣
		1.
			//�����ӱ�־����·��ת������
			//ZCL 2019.4.9 �ĳ�ע��ɹ��ķ���
			//else if(CGD0_LinkConnectOK[SendDataReturnNo])
			else if(ZhuCeOkFLAG[SendDataReturnNo])

		2. //2019.4.11 ZCL ���淢��GPRSż�����������ǲ��������⣿������Ϊ��S_M35==5���ղ���CallReady������취���ÿ��Ź���ʱ�������ˣ�
			else if(S_M35==5)
			{
				if(C_M35SendSum>95 )
				{
					S_M35 = 0x01;						//ģ��ػ�������
					Delay_MS(3000);					//��ʱ����û��ι�����豸��λ�� 2019.4.11
				}
				//S_M35 = 0x01;			//ZCL123 ���� 2019.4.10
			}
		3.  �����Ӷ�
				//2015.1.17  15�θĳ�35�Σ�65��(2019.4.4)����ֹ��Ŀ��ٳ����������⡣
				else if(C_M35SendSum >65 )
					S_M35 = 0x01;

		4. //ZCL 2019.4.9 IP�Ͷ˿ں���ȷ�Ϳ��Խ������ӡ� CGD0_LinkConnectOK
				//�磺0, CONNECT OK�� �������Ǳ����û���У�Ҳ���Խ������ӡ��������ӵı�־: CGD0_LinkConnectOK
				//�������������ж��Ƿ����� ����ͨѶ����ֻ�ǽ������ӣ�ƴͨ�� ����ͨѶ�ı�־: ZhuCeOkFLAG

		5.��ע������Ҫ!!! ZCL 2019.3.15
			#define TXD0_MAX		511					// ���������
			#define RCV0_MAX		1536 				// ���ջ��������� //256*6			�����ܴ󣬽���GPRS���������ã�
			#define TXD1_MAX		511					// ���������
			#define RCV1_MAX		2048	//2176 2019.4.5 	// ���ջ��������� //256*8.5		�����ܴ󣬽���Xmodem�ã�

		6. �޸�xmodem.c�����ݣ���xmodem.c�е��ļ�ͷע�ͣ�

		//LORA�������д˹��ܡ�
		7. //ZCL 2019.3.12 ����GPRSģʽ������MODBUS���Ȼ���Լ���������
					//ע������ʹ�õ���Txd2Buffer���������ġ�����Rcv3Buffer
					if(F_GprsMasterNotToCom || F_GprsMasterToCom)
					{
						if( Txd2Buffer[0]==2 )		// �ӵ�ַ��⣭���յ�����λ����ѯָ��
						{
							������������
						}
						else
							i=3;
					}

			//ZCL 2019.3.12  ����GPRSģʽ�£�������ָ�׼�����أ�

		8. (3.4) //2019.4.11 ZCL
			���⣺GPRSż�����������ǲ����ߣ����淢����Ϊ��������������S_M35==5���ղ���CallReady������S_M35=1���������á�
			����취���ÿ��Ź���ʱ�������ˣ�
			else if(S_M35==5)
			{
				if(C_M35SendSum>95 )
				{
					S_M35 = 0x01;						//ģ��ػ�������
					Delay_MS(3000);					//��ʱ����û��ι�����豸��λ�� 2019.4.11
				}
				//S_M35 = 0x01;			//ZCL123 ���� 2019.4.10
			}


		//GP311
		8. ZCL 2019.4.19	������ţ������ӡ�۲�����
			����  B_RestartNo=1;			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
					printf("RestartNo=%d\r\n", B_RestartNo);			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
			(1) B_RestartNo=1;
					[	//4. �����������նˣ�DTU����ָ��
						else if( *(ptr2+1)==0x82 )
						{
							S_M35 = 0x01;
							B_RestartNo=1;			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
							printf("RestartNo=%d\r\n", B_RestartNo);			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
						}		]

			(2)	B_RestartNo=2;		//0x82������	  GPW+82,1;
					[	//1. ���ſ���DTU�����ɹ����Ͷ��ŷ���
					if(B_SmsDtuRestart==1 )
					{
						B_SmsDtuRestart=2;
						S_M35 = 0x01;						//ģ��ػ�������
						B_RestartNo=2;			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
						printf("RestartNo=%d\r\n", B_RestartNo);			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
					}		]

				ʵ�����ݣ������յ���������ʾ��������������Ϊ���յ�ָ���ASCII�룺
					[2019-04-24_00:21:06]+CMTI: "SM",12
					[2019-04-24_00:21:08]1
					[2019-04-24_00:21:08]+CMGR: "REC UNREAD","+8615610566803","","2019/04/24 00:21:06+32"
					[2019-04-24_00:21:08]004700500057002B00380032002C0031003B0009 		//ZCL ???����ASCII��(��ô���ǣ�GPW+82,1;)   �������дGPW+82,1;ָ�����ָ�����������ַ�������

					[2019-04-24_00:21:08]OK

			(3)	B_RestartNo=3;			//0x91��ģ���ϵ�����  GPW+91,1;
					[	//2. ���� ���߹���(2014.12.8) ����DTUӲ�������ɹ����Ͷ��ŷ���
					if(B_SmsOrFaultHardRestart==1 )
					{
						B_SmsOrFaultHardRestart=2;
						S_M35 = 0x01;						//ģ��ػ�������
						B_RestartNo=3;			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
						printf("RestartNo=%d\r\n", B_RestartNo);			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
					}	]

				ʵ�����ݣ�
					[2019-04-23_15:28:06]+CMGR: "REC UNREAD","+8615610566803","","2019/04/23 15:28:02+32"
					[2019-04-23_15:28:06]GPW+91,1;

					//------------���Ҳ�����˲��Ե�����������ǶԵģ�
					[2019-04-24_00:47:24]+CMTI: "SM",24
					[2019-04-24_00:47:26]1
					[2019-04-24_00:47:26]+CMGR: "REC UNREAD","+8615610566803","","2019/04/24 00:47:21+32"
					[2019-04-24_00:47:26]004700500057002B00390031002C0031003B0009			//ZCL ???����ASCII��  �������дGPW+82,1;ָ�����ָ�����������ַ�������

					[2019-04-24_00:47:26]OK


			(5)	B_RestartNo=5;
					[	//2.5 ���ſ��Ź���λ���� 2014.12.8
						if(B_SmsWatchdogRestart==1)
						{
							B_SmsWatchdogRestart=2;
							S_M35 = 0x01;						//ģ��ػ�������
							B_RestartNo=5;			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
							printf("RestartNo=%d\r\n", B_RestartNo);			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
							//ZCL 2019.4.19 Delay_MS(3000);					//��ʱ����û��ι�����豸��λ�� 2013.7.3
						}	]

				ʵ�����ݣ�
					[2019-04-24_00:36:14]+CMTI: "SM",16
					[2019-04-24_00:36:16]1
					[2019-04-24_00:36:16]+CMGR: "REC UNREAD","+8615610566803","","2019/04/24 00:36:12+32"
					[2019-04-24_00:36:16]GPW+95,1;

					[2019-04-24_00:36:16]OK
					[2019-04-24_00:36:16]RestartNo=5
					[2019-04-24_00:36:19]��
					[2019-04-24_00:36:20]Exit Xmodem , Enter normal run mode (2)

			(6)	B_RestartNo=6��7��8
					[	//2015.1.5 �����������Ϊ����3G���ȴ�CallReady�ź���Ҫ������ʱ�䣬16���ˡ�
						//2019.4.11 ZCL ���淢��GPRSż�����������ǲ��������⣿������Ϊ��S_M35==5���ղ���CallReady������취���ÿ��Ź���ʱ�������ˣ�
						else if(S_M35==5)
						{
							if(C_M35SendSum>95 )
							{
								S_M35 = 0x01;						//ģ��ػ�������
								B_RestartNo=6;			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
								printf("RestartNo=%d\r\n", B_RestartNo);			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
								//ZCL 2019.4.19 Delay_MS(3000);					//��ʱ����û��ι�����豸��λ�� 2019.4.11
							}
							//S_M35 = 0x01;			//ZCL123 ���� 2019.4.10
						}
						//2015.1.17 �����������Ϊ����+CREG: 0,2 17�βų���+CREG: 0,1��ע��ɹ���
						else if(S_M35==7)		//ZCL 2017.5.25 6�ĳ�7,��Ϊ���case 6Ϊ�ź�ǿ�ȼ�⣻
						{
							if(C_M35SendSum>95 )
							{
								S_M35 = 0x01;
								B_RestartNo=7;			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
								printf("RestartNo=%d\r\n", B_RestartNo);			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
							}
						}
						//2015.1.17  15�θĳ�35�Σ�65��(2019.4.18)����ֹ��Ŀ��ٳ����������⡣
						else if(C_M35SendSum >65 )
						{
							S_M35 = 0x01;
							B_RestartNo=8;			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
							printf("RestartNo=%d\r\n", B_RestartNo);			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
						}
					]

			(4) ԭ�ȣ�B_SmsOrFaultHardRestart=11ʱ B_RestartNo=4	 �����޸ģ�
					��ΪB_SmsOrFaultHardRestart=11 ԭ���кܶ������������ֿ�  ZCL 2019.4.23
					������B_SmsOrFaultHardRestart=11,12,13,14,15,16  ��󸳸� B_RestartNo=11,12,13,14,15,16
				�޸����£�
			(4.1)	B_SmsOrFaultHardRestart=11
						//��2�ν��յ����루���������ݣ���������
						else
						{
							B_RcvFail[ChannelNo]=0;
							B_SmsOrFaultHardRestart=11;		//=11����Ӳ������; =1����Ӳ������
						}
			(4.2)	B_SmsOrFaultHardRestart=12
						//��2�γ���"CONNECT FAIL"�󣬾�����
						else if(B_ZhuCeOkNum==0)
						{
							B_SmsOrFaultHardRestart=12;		//=11����Ӳ������; =1����Ӳ������
						}
			(4.3)	B_SmsOrFaultHardRestart=13
						//2013.6.20 ���յ���ERROR��������
						else if( strstr(Rcv0Buffer,"ERROR")!=NULL )
						{
							if(BakRcv0Count==9 && B_ZhuCeOkNum==0)
							{
								B_SmsOrFaultHardRestart=13;		//=11����Ӳ������; =1����Ӳ������
							}
						}
			(4.4)	B_SmsOrFaultHardRestart=14
							//���� �޸�24��34��44��54�ֱ����1,2,3,4ͨ����������ʽ��0-UDP��1-TCP������Ҫ����
							else if(CommandNo==0x24 || CommandNo==0x34 || CommandNo==0x44
							|| CommandNo==0x54)
							{
								//2013.6.21 ԭ�������ӣ�Ӳ������
								for(j=0;j<4;j++)		//2013.6.19
								{
									if(CGD0_LinkConnectOK[j] && CommandNo==0x24+0x10*j)		//(TCP,UDPЭ���޸�)
									{
										B_SmsOrFaultHardRestart=14;		//=14����Ӳ������; =1����Ӳ������
									}
								}
							}
			(4.5)	B_SmsOrFaultHardRestart=15
						//5. ��10�ι��ϱ�־���������Ӳ��ɹ�������������ģ�顣
						//���������ʱ����=0����DTU����ע��ʧ�ܺ������������Ӳ����
						//���������ʱ����!=0���򲻻�������һֱ��������ʱ����������

						//if(j==0 && B_LinkFault[i]>=10)			//
						if(B_LinkFault[i]>=10)			//2016.5.24 �޸ģ�ȥ��j==0
						{
							B_SmsOrFaultHardRestart=15;		//=15����Ӳ������; =1����Ӳ������
						}
			(4.6)	B_SmsOrFaultHardRestart=16
						//���� TCP,UDPЭ���޸ģ�ԭ�������ӣ�Ӳ������ 2013.6.21 24��34��44��54�ֱ����1,2,3,4ͨ����������ʽ��0-UDP��1-TCP��
						for(j=0;j<4;j++)
						{
							if(CGD0_LinkConnectOK[j] && CommandNo==0x24+0x10*j)		//()
							{
								B_SmsOrFaultHardRestart=16;		//=16����Ӳ������; =1����Ӳ������
							}
						}

		9. //ZCL 2019.4.23  ȥ��case 0x05: �е� B_WaitCallReady��س��򣬷��� GPW+91,1;
				�����󣬿��Խ��յ�GP311�Ļط����ţ�GPW+91,1, DtuHardRestart, Set OK! ;

		10.//ZCL 2019.4.23 �ս���ʱ����ӡһ�Σ�Ȼ����ʱ�� ������������д�ӡ���ǳ���Ҫ����������������ͻ
				//ZCL 2019.4.19 //=5��=6 Ϊ���Ź��������ȴ���ӡ��ϡ�
				if(B_RestartNo>0 && S_M35==0x01 && Txd1Max==0)
				{
					if( T_WatchdogRestartDelay!=SClk10Ms )
					{
						T_WatchdogRestartDelay=SClk10Ms;

						//ZCL 2019.4.23 �ս���ʱ����ӡһ�Σ�Ȼ����ʱ�� ������������д�ӡ���ǳ���Ҫ����������������ͻ
						if(C_WatchdogRestartDelay==0)
							printf("RestartNo=%d\r\n", B_RestartNo);			//ZCL 2019.4.19 ������ţ������ӡ�۲�����

						C_WatchdogRestartDelay++;

						if ( C_WatchdogRestartDelay>200 )
						{
							C_WatchdogRestartDelay=0;
							if(B_RestartNo==5 || B_RestartNo==6)
								Delay_MS(3000);					//��ʱ����û��ι�����豸��λ�� 2013.7.3
							//
							B_RestartNo=0;
						}
					}
				}

		11. [2019-04-24_01:18:52]ERROR
				[2019-04-24_01:18:52]RestartNo=13

		12. ZCL 2019.4.26
			�޸�for(,.) �б��������ƴ�С����ֹ�������

		13. ZCL 2019.4.26
			//0x95:GP311ģ�鿴�Ź���λ���� 2013.7.3  //GPW+95,1;
			//ZCL 2019.4.26 ���Ź�������ģ�鲻�᷵�ض��ţ���Ϊ�����ˣ���־û���ˣ�

		//DCM200_SCR_E(V2)
		14. ZCL 2019.4.26  SysTick_Handler()�м���ι�����ơ���ΪҺ�������ϵ续��������ʱ����ʾ�겻���ж���ι��
			if (S_Display==0) 				//ZCL 2019.4.26  =1������ʾ����ҳ���ˣ�����ι��
				IWDG_Feed();						// =1 ȥ���ж��е�ι��	����DTU�����Ͳ������� ZCL

		15. GPW+91,1; ָ��  //DCM200_SCR_E(V2)  �����ã� ��GP311���ã�

				(1) DCM200_SCR_E(V2)  �����ã�
				ZCL 2019.4.26  ���⣺GPW+91,1;  ģ���ϵ�������ʵ�ֲ��ˣ���ӡ��B_RestartNo=3; ����2��OK�ͽ��в���ȥ��
				������֣�B_RestartNo=6; ����ת���� �ղ���CallReady�����Ź���ʱ������Ϊʲô��

				//���ſ���DTUӲ������
				if(B_SmsOrFaultHardRestart==1 )	//0x91��ģ���ϵ�����		GPW+91,1;
				{
					B_SmsOrFaultHardRestart=2;
					S_M35 = 0x01;						//ģ��ػ�������
					B_RestartNo=3;

				ʵ�����ݣ�
				[2019-04-25_22:55:38]+CMTI: "SM",19
				[2019-04-25_22:55:40]1
				[2019-04-25_22:55:40]+CMGR: "REC UNREAD","+8615610566803","","2019/04/25 22:55:38+32"
				[2019-04-25_22:55:40]GPW+91,1;

				[2019-04-25_22:55:40]OK
				2019-04-25_22:55:40]RestartNo=3
				[2019-04-25_22:55:48]
				[2019-04-25_22:55:48]OK
				[2019-04-25_22:55:48]
				[2019-04-25_22:55:48]OK
				[2019-04-25_22:57:24]RestartNo=6

				(2)GP311���ã�
				//GP311 ��ȷ  GPW+91,1;

				ʵ�����ݣ�
				[2019-04-26_23:34:22]+CMTI: "SM",45
				[2019-04-26_23:34:23]1
				[2019-04-26_23:34:23]+CMGR: "REC UNREAD","+8615610566803","","2019/04/26 23:34:23+32"
				[2019-04-26_23:34:23]GPW+91,1;

				[2019-04-26_23:34:23]OK
				[2019-04-26_23:34:23]RestartNo=3
				[2019-04-26_23:34:25]
				[2019-04-26_23:34:32]
				[2019-04-26_23:34:32]RDY
				[2019-04-26_23:34:32]
				[2019-04-26_23:34:32]+CFUN: 1
				[2019-04-26_23:34:32]
				[2019-04-26_23:34:32]OK

	*/

/* Includes ------------------------------------------------------------------*/
#include "com1_232.h"
#include "com3_232.h"
#include "Gprs_at.h"
#include "GlobalV_Extern.h" // ȫ�ֱ�������
#include "GlobalConst.h"
#include "string.h" //strstr�������ҳ�str2�ַ�����str1�ַ����е�һ�γ��ֵ�λ��
#include <stdio.h>	//���ϴ˾������printf
#include "sim7600ce.h"
#include <stdarg.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define DDPHEADTAILLEN 16 // DDPЭ��ͷ��β����

// 1. ������� �й��ƶ��ķ��������*99***1#���й���ͨ�ķ��������#777��
// 2. APN ע��CDMA���粻�Ƿ��齻�����磬��ˣ�����Ҫ��д��ֵ��
uc8 Gprs_ParBootLst[288] = {
	'*', '9', '9', '*', '*', '*', '1', '#', 0, // 01(Hex,������ͬ,����ָ���������) =�������8+1
	'C', 'M', 'N', 'E', 'T', 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,					  // 05=  24+1 Access Point Name ���������
	'1', '3', '9', '1', '2', '3', '4', '5', '6', '7', '8', 0, // 06:	11+1 DTU���ʶ����
	0x01, 0x00, 0x01, 0x90, 0x01, 0x00, 0x00, 0x1E,			  // 07:	2 ��������,08: 2 ��������ʱ����,09:1 ֧�����ݻ�ִ,0A: 1 �Զ�Ӧ��,0B:2 �ز���ʱ����
	0x00, 0x00, 0x00, 0x25, 0x80, 0x03, 0x00, 0x00, 0x03,	  // 0C:1 ����̨��Ϣ����  0D���ڹ�����ʽ:8 ���ڹ�����ʽ
	0x0A, 0x00, 0x00, 0x02, 0x00, 0x3C,						  // 0E:1 ���ݰ��ָ���ʶ,0F:1 ����TDPЭ��,10:1 �ն�����,  11:1 ���з�ʽ,  12:2 ����ʱ����
	0x00, 0x00, 0x07,										  // 13:2 ��������ʱ����, 14:1 ��������ģʽ

	// 222.173.103.226	��˾������	0xDE,0xAD,0x67,0xE2
	// 219.147.19.110	��˾������	0xDB,0x93,0x13,0x6E		//ZCL 2019.3.13
	// 114.215.155.112 �Ʒ�����		0x72,0xD7,0x9B,0x70		//ZCL 2019.3.13

	//	0xDE,0xAD,0x67,0xE2,					0xDB,0x93,0x13,0x6E,					//20.30		IP  222.173.103.226
	//  0xDE,0xAD,0x67,0xE2,					0x00,0x00,0x00,0x00,					//20.30		IP  222.173.103.226
	47, 93, 6, 250, 39, 106, 64, 39,				// 20:4 ��1·IP��ַ 30:4 ��2·IP��ַ		IP  112.125.89.8	������IP��ַ
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 40:4 ��3·IP��ַ 50:4 ��4·IP��ַ							������IP��ַ

	0x13, 0x90, 0x27, 0x48, 0, 0, 0, 0,				// 22.32.42.52		5008=0x1390 10056=0x2748 0x1D,0x4C,   0x00,0x00,		�������˿�
	0x13, 0x89, 0x13, 0x8A, 0x13, 0x8B, 0x13, 0x8C, // 23.33.43.53		5001-5004	���ض˿�
	0x00, 0x00, 0x00, 0x00,							// 24.34.44.54		=0 UDP; =1 TCP
	0x01, 0x01, 0x01, 0x01,							// 25.35.45.55		=1 DDPЭ��; =0
	//	0x00,0x28,		 0x00,0x28,			0x00,0x28,		  0x00,0x28,		//26.36.46.56		����ʱ��40S
	0x00, 0x18, 0x00, 0x18, 0x00, 0x18, 0x00, 0x18, // 26.36.46.56		����ʱ��40S
	0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, // 27.37.47.57	������ʱ����
	0x05, 0x05, 0x05, 0x05,							// 28.38.48.58	����½DSC����
	0x05, 0x05, 0x05, 0x05,							// 29.39.49.59	������������
	0x07, 0x07, 0x07, 0x07,							// 2B.3B.4B.5B	���Ƽ���
	//	0x01,0x2C,		 0x01,0x2C,			0x01,0x2C,			0x01,0x2C,		//2C.3C.4C.5C	������ʱ����
	0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C, // 2C.3C.4C.5C	������ʱ����
	// 60.61.62 ͨ������ģʽ TCP����ʱ���� DNS��ַ
	0x02, 0x00, 0x05, 0xCA, 0x60, 0x86, 0x85, // 202.96.134.133
	0x00, 0x00,								  // 63 PPP����ʱ����

	/* 	'w' , 'n', 'c', 's', '.', 's', 'a', 'n', 'l', 'e', 					//21	����
		'y' , '.', 'c', 'n',   0,   0,   0,   0,   0,   0,
		   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 21	����
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 31	����
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 41	����
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 51	����

};

// GPRS ָ�����
uc8 CmdNo[75] = {
	0x01,
	0x05,
	0x06,
	0x07,
	0x08,
	0x09,
	0x0A,
	0x0B,
	0x0C,
	0x0D,
	0x0E,
	0x0F,
	0x10,
	0x11,
	0x12,
	0x13,
	0x14,
	0x20,
	0x30,
	0x40,
	0x50,
	0x22,
	0x32,
	0x42,
	0x52,
	0x23,
	0x33,
	0x43,
	0x53,
	0x24,
	0x34,
	0x44,
	0x54,
	0x25,
	0x35,
	0x45,
	0x55,
	0x26,
	0x36,
	0x46,
	0x56,
	0x27,
	0x37,
	0x47,
	0x57,
	0x28,
	0x38,
	0x48,
	0x58,
	0x29,
	0x39,
	0x49,
	0x59,
	0x2B,
	0x3B,
	0x4B,
	0x5B,
	0x2C,
	0x3C,
	0x4C,
	0x5C,
	0x60,
	0x61,
	0x62,
	0x63,
	0x21,
	0x31,
	0x41,
	0x51,
	0x70,
	0x71,
	0x73,
	0x74,
	0x75,
	0x00,
}; // ����0��Ϊ���жϽ���

// GPRS ָ���
uc8 CmdLen[75] = {
	8,
	24,
	11,
	2,
	2,
	1,
	1,
	2,
	1,
	8,
	1,
	1,
	1,
	1,
	2,
	2,
	1,
	4,
	4,
	4,
	4,
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	2,
	2,
	2,
	2,
	1,
	2,
	4,
	2,
	31,
	31,
	31,
	31,
	7,
	8,
	19,
	6,
	5,
	0,
}; // ����0��Ϊ���жϽ���

// GPRS ָ���λ�� (������GprsCmdNo��Ӧ)
uc16 CmdPst[75] = {0, 9, 34, 46, 48, 50, 51, 52, 54,
				   55, 63, 64, 65, 66, 67, 69, 71,
				   72, 76, 80, 84, 88, 90, 92, 94,
				   96, 98, 100, 102, 104, 105, 106, 107,
				   108, 109, 110, 111, 112, 114, 116, 118,
				   120, 122, 124, 126, 128, 129, 130, 131,
				   132, 133, 134, 135, 136, 137, 138, 139,
				   140, 142, 144, 146, 148, 149, 151, 155,
				   157, 188, 219, 250, 0, 0, 0, 0,
				   0, 0};

// GPRS����ֵ�����ֽڵ������б�
uc8 GprsCmd2Byte[28] = {0x07, 0x08, 0x0B, 0x12, 0x13, 0x22, 0x32, 0x42, 0x52,
						0x23, 0x33, 0x43, 0x53, 0x26, 0x36, 0x46, 0x56,
						0x27, 0x37, 0x47, 0x57, 0x2C, 0x3C, 0x4C, 0x5C,
						0x61, 0x63, 0X00}; // ����0��Ϊ���жϽ���

// GPRS ����ע��������
uc8 GPRS_ZhuCe[22] = {0x7B, 0x01, 0x00, 0x16, 0x31, 0x33, 0x39, 0x31, 0x32, 0x33, 0x34, 0x35,
					  0x36, 0x37, 0x38, 0x0A, 0x66, 0x9A, 0x00, 0x0F, 0xA2, 0x7B}; //,0x1A

// GPRS ��ѯ���ص���ʼͷ��01-04������
uc8 GPRS_ServiceCode[31] = {
	0x7B,
	0x0B,
	0x01,
	0x6C,
	0x31,
	0x33,
	0x39,
	0x31,
	0x32,
	0x33,
	0x34,
	0x35,
	0x36,
	0x37,
	0x38, // ���Ⱥ�DTU���ʶ����� 15��������һ��0x7B
	0x01,
	0x0A,
	0x2A,
	0x39,
	0x39,
	0x2A,
	0x2A,
	0x2A,
	0x31,
	0x23, // 01����������
	0x02,
	0x02, // 02����PPP�û���
	0x03,
	0x02, // 03����PPP����
	0x04,
	0x02,
}; // 04��ʾSIM PIN��δ����

// GPRS ���ò�������
uc8 GPRS_SetParReturn[16] = {
	0x7B,
	0x0D,
	0x00,
	0x10,
	0x31,
	0x33,
	0x39,
	0x31,
	0x32,
	0x33,
	0x34,
	0x35,
	0x36,
	0x37,
	0x38,
	0x7B,
};

// GPRS ��������֡ͷ
uc8 GPRS_ZhenTou[16] = {0x7B, 0x09, 0x00, 0x10, 0x31, 0x33, 0x39, 0x31, 0x32, 0x33,
						0x34, 0x35, 0x36, 0x37, 0x38, 0x7B};

uc8 DtuProgVersion[7] = "5.30";		 // 7 DTU����汾:70		�޸�������Ҫ����2���ط� YLS 2023.12.06
uc8 DtuProgMakeDate[8] = "20250205"; // 8 DTU������������:71		�޸�������Ҫ����2���ط� YLS 2023.12.06

uc8 DtuHardWare[14] = "V5.3-F103-4G  "; // 14+7 DTUӲ��ƽ̨�汾:73
uc8 DtuManufacture[6] = "SANLEY";		// 6 DTU������LOGO:74
uc8 DtuModel[6] = "ZGP331";				// 6 DTU�ͺ�����:75

uc8 SetOK[9] = " Set OK! ";							 // ���ò����ɹ�
uc8 SetFail[11] = " Set Fail! ";					 // ���ò���ʧ��
uc8 ParameterError[18] = " Parameter Error! ";		 // ��������
uc8 CommandError[16] = " Command Error! ";			 // �������
uc8 PasswordError[17] = " Password Error! ";		 // �������
uc8 ReadOK[10] = " Read OK! ";						 // �������ɹ�
uc8 Parameter[12] = " Parameter: ";					 // ����
uc8 DtuRestartOK[12] = " DtuRestart,";				 // ����DTU���� OK
uc8 DtuHardRestartOK[16] = " DtuHardRestart,";		 // ����DTUӲ������ OK
uc8 SmsControlStopOK[16] = " SmsControlStop,";		 // ���ſ����豸ֹͣ OK
uc8 SmsControlRunOK[15] = " SmsControlRun,";		 // ���ſ����豸���� OK
uc8 SmsWatchdogRestart[20] = " SmsWatchdogRestart,"; // ���ſ��ƿ��Ź���λ OK

uc8 BaudRateError[21] = " BaudRate Par Error! ";					// BaudRate��������
uc8 DataBitLengthError[26] = " DataBitLength Par Error! ";			// DataBitLength��������
uc8 StopBitLengthError[26] = " StopBitLength Par Error! ";			// StopBitLength��������
uc8 ParityBitLengthError[28] = " ParityBitLength Par Error! ";		// ParityBitLength��������
uc8 FlowControlError[24] = " FlowControl Par Error! ";				// FlowControl��������
uc8 SerialPortItemLackError[30] = " Serial Port Item Lack Error! "; // ���ô��п���ȱ�ٴ���
uc8 StringlengthError[31] = " String Length Overlong Error! ";		// �ַ����������ô���
uc8 ZhuDongSendSMS[21] = " This is a test SMS! ";					// �������Ͷ�������

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uchar Txd3Buffer[TXD3_MAX];		 // ���ͻ�����
char Rcv3Buffer[RCV3_MAX];		 // ���ջ�����
char Rcv3_Back_Buffer[RCV3_MAX]; // ���ջ�����
uchar Txd3TmpBuffer[TXD3_MAX];	 // ZCL 2019.3.14
uint Cw_Txd3TmpMax;				 // ZCL 2019.3.14

uint Cw_Rcv3;		 // ���ռ�����//
uint Cw_Txd3;		 // ���ͼ�����//
uint Cw_BakRcv3;	 // ���ռ�����//
uint Cw_Txd3Max;	 // �ж��ٸ��ַ���Ҫ����//
uint Lw_Txd3ChkSum;	 // ����У��ͣ�lo,hi ��λ//
uint Lw_Com3RegAddr; // ����3�Ĵ�����ַ
//
uchar B_Com3Cmd03;
uchar B_Com3Cmd16;
uchar B_Com3Cmd01;
uchar B_Com3Cmd06;
uchar B_Com3Cmd73;
uchar T_NoRcv3; // û�н��ռ�����
uint C_NoRcv3;

// GPRS����
uint C_M35Step;			// M35ģ�鲽����� 2012.7.20
uchar ComeSmsPhone[11]; // �����ŵĵ绰����
uchar S_M35PowerOnOff;	// s���裺M35�ϵ�ϵ�
uchar S_M35;			// s���裺M35����

uchar AT_ComOk_Flag; // �յ�OK�ַ��ı�־
uchar AT_CIPSEND_Flag;
uchar Net_Error_Flag;	   // ����رձ�־
uchar AT_3EOk_Flag;		   // 0x3E='>'�����Ա�־��׼�����뷢�����ݣ�
uchar AT_CallReadyOk_Flag; // �յ�Call Ready��־
uint C_M35PowerOnOff;	   // M35�ϵ�ϵ������
uint C_M35SendSum;		   // M35����ÿ���ķ�������
uchar ChannelNo;		   // Ӧ��ͨ��,�ĳ���ֵ
uchar B_Com3Cmd8B;		   // ��ѯDTU�������� 2012.8.17
uchar B_Com3Cmd8D;		   // ����DTU�������� 2012.8.24
uchar B_Com3Cmd89;		   // ��������������ָ�� 2011.8.24
uchar B_OtherCmd;		   // ��λ��ͨ��DTU������2������ָ��跢�͵�����2��
uchar B_DoWithData;		   // �������ݱ�־
uchar S_DtuInquireSend;	   // DTU��ѯ���Ͳ��� 2012.8.19
uchar S_DtuSetSend;		   // DTU���ò��� 		2012.8.24
uchar S_GprsDataSend;	   // Gprs���ݷ��� 		2012.8.24

uint w_StringLength;	   // �ַ�������
uchar StringBuffer[12];	   // BCD��ת���ַ������档��Ҫ��IP��ַ�ͳ��Ƚ���ת��
uchar AccessPointLength;   // 05ָ��		2012.8.20
uchar DtuNoLength;		   // 06ָ��
uchar DomainName0Length;   // 21(Hex)ָ���
uchar DomainName1Length;   // 31(Hex)ָ���
uchar DomainName2Length;   // 41(Hex)ָ���
uchar DomainName3Length;   // 51(Hex)ָ���
uint w_Txd3Number;		   // ����3��������
uint C_HeartDelay[4];	   // ������������ʱ������
uchar B_HeartSendAsk[4];   // ���������������־
uchar B_HeartSendFault[4]; // �������������ϱ�־
uchar S_HeartSendNow[4];   // ���������� ����
uint C_HeartSendNow[4];	   // ���������� ������
uchar S_HeartSend;		   // s���裺����������

uint C_LinkDelay[4];			 // ���ӷ�����ʱ������  2012.10.4
uchar B_LinkAsk[4];				 // �������������־
uchar B_LinkAsk_DomainName[4];	 // �������������־
uchar B_LinkFault[4];			 // �������ӹ��ϱ�־
uchar S_LinkNow[4];				 // �������� ����
uchar S_LinkNow_DoName[4];		 // �������� ����
uint C_LinkNow[4];				 // �������� ������
uint C_LinkNow_DoName[4];		 // �������� ������
uchar S_SendLink;				 // �������Ӳ���
uchar S_SendLink_DoName;		 // �������Ӳ���
uchar B_FirstLink[4];			 // ��һ�����ӱ�־
uchar B_FirstLink_DomainName[4]; // ��һ�����ӱ�־

uchar SendHeartNo; // ������������� 0-4·��ѭ���������
uchar ConnectNum;  // ��������
uchar ConnectNum_DomainName;
uchar CGD0_LinkConnectOK[6]; // ����0���ӱ�־
uchar F_IPOK[6];			 // ����0���ӱ�־
uchar CGD1_LinkConnectOK[6]; // ����1���ӱ�־
uchar ZhuCeOkFLAG[6];		 // ע��ɹ���־
uchar CGD0_ConnectNo;		 // ����0�������
uchar DomainName_No;		 // �����������
uchar CGD1_ConnectNo;		 // ����1�������
uchar SendDataReturnNo;		 // �������ݷ������(���ڹ��������ݷ���ÿ������)
uint C_DtuInquireSend;		 // DTU��ѯָ�������
uint C_DtuSetSend;			 // DTU����ָ�������
uint C_GprsDataSend;		 // Gprs���ݴ��������
uint w_NewSmsNo;			 // �µĶ������
uint w_SmsMaxSum;			 // SIM������װ�µ������������
uint w_SmsNowSum;			 // ��ǰSMS����

uchar CommandNo;	   // �������
uchar SendSmsBuf[150]; // ���Ͷ��Ż��棡 �ܳ��� 2013.12.15 ����̫С������������ˣ��������
uchar SendSmsLength;   // ���Ͷ��ų���

uchar S_SendSms;		// ���Ͷ��Ų���
uint C_SendSms;			// ���Ͷ��ż�����
uchar S_ReadNewSms;		// ���¶��Ų���
uint C_ReadNewSms;		// ���¶��ż�����
uchar S_SmsFull;		// ����������
uint C_SmsFull;			// ������������
uchar SmsFull;			// ��������־
uchar B_SmsSetup;		// �����趨����
uchar B_GprsDataReturn; // GPRS ���ݷ��ء������ӣ��յ�����2���ݣ���ͨ��DTU���ͳ�ȥ

uchar B_SmsDtuRestart;		   // ��������				����������������M35_Init()�г�ʼ��
uchar B_SmsOrFaultHardRestart; // ���Ż��߹���Ӳ������
uchar B_SmsWatchdogRestart;	   // ���ſ��Ź���λ����

uchar B_SmsRunStop;				 // ���ſ�����ͣ���豸ͣ����
uchar SmsControlStopValue;		 // ���ſ���ֹͣ��ֵ���豸ͣ���� =1ֹͣ��=0����
uint C_DelayRunGprsModule;		 // ��ʱ����GPRSģ���־
uchar B_CloseLink[4];			 // �ر����ӱ�־��Ȼ����ʱ��������ʱ�䵽������������
uchar *ptr, *ptr1, *ptr2, *ptr3; // ָ��
uchar Csq[10];					 // ����ź�ǿ��
uchar B_ATComStatus;			 // AT����״̬ 2013.5.20
uchar B_ConnectFail[4];			 // ���յ�CONNECT FAIL����������
uchar B_NextS35Cmd;				 // ִ����һ��S35����
uchar B_ZhuCeOkNum;				 // ע��OK������
uchar B_RcvFail[4];				 // ���յ��������
uint C_SmsRunStop;				 // ��������ֹͣ������
uchar BKCom1TxBuf[8];			 // ����Com1���ͻ���
uchar B_SendSmsRunStopCmdCount;	 // ���Ͷ�������ֹͣ�������
uchar B_ZhuDongSendSms;			 // �������Ͷ���(�յ�����4��������@�绰�����) 2014.12.7

uint C_BetweenDtuInquireSend; // ����ǰ��ʱ3MS���ղ���>��־��ʱ300MS�ط� 2014.12.11
uint C_BetweenDtuSetSend;	  // ����ǰ��ʱ3MS���ղ���>��־��ʱ300MS�ط�
uint C_BetweenSendDataReturn; // ����ǰ��ʱ3MS���ղ���>��־��ʱ300MS�ط�
uint C_BetweenHeartSendNow;	  // ����ǰ��ʱ3MS���ղ���>��־��ʱ300MS�ط�
uint C_BetweenSendSms;		  // ����ǰ��ʱ3MS���ղ���>��־��ʱ300MS�ط�

uchar B_WaitCallReady;		  // ZCL 2018.12.12
uchar B_RestartNo;			  // ZCL 2019.4.19  ������ţ������ӡ�۲�����
uchar T_WatchdogRestartDelay; // ZCL 2019.4.19 ���Ź�������ʱ
uint C_WatchdogRestartDelay;  // ZCL 2019.4.19 ���Ź�������ʱ
u8 p1_TmpBuf[100] = {0};	  // ����2
u8 p_TmpBuf[512] = {0};		  // ����1
u8 *p_Txd1Buffer;
u8 domain_name[31] = {0};
/* Private variables extern --------------------------------------------------*/

extern uchar Txd2Buffer[];
extern uint Cw_Txd2;	// ���ͼ�����
extern uint Cw_Txd2Max; // �ж��ٸ��ַ���Ҫ����

extern uchar Rcv2Buffer[];
extern uint Cw_BakRcv2;		// ���ݽ��ռ�����
extern uchar B_ForceSavPar; // ��־
extern uchar B_PcSetGprs;	// 2013.6.6
extern uint Lw_GprsTmp;
extern uint C_PowenOnDelay;
extern uint C_Com3NoRcv; // COM0 û�н����������ݼ����� ZCL 2017.5.30

extern u8 B_LoraSendWrite; // ZCL 2021.11.16 LORA д����ķ���
extern u8 B_LoraSendWriteLength;
extern uint8_t LoRaTxBuf2[]; // ZCL 2021.11.18
extern u8 Txd1Buffer[];		 // ���ͻ�����
extern u16 Cw_Txd1;			 // ���ͼ�����//
extern u16 Cw_Txd1Max;		 // �ж��ٸ��ַ���Ҫ����//
extern u8 Txd2Buffer_TEMP[];
extern u8 Txd1Buffer_TEMP[];

extern void UART3_Output_Datas(uint8_t S_Out[], uint16_t Counter);
extern void Fill_data(void);
extern unsigned int atoi(unsigned char *s, unsigned char sz);
extern void delay_ms(vu16 nCount);
extern void LCD_DLY_ms(u32 Time);
extern void At_DomainName(u8 i);
// char *tcp_ip = "AT+CIPOPEN=0,\"TCP\",\"119.23.42.226\",8050\r\n";	         // �޸ķ�����IP�Ͷ˿ں�
// char *tcp_ip = "AT+CIPOPEN=0,\"TCP\",\"112.125.89.8\",36987\r\n"; // �޸ķ�����IP�Ͷ˿ں�

// uint8_t lte_power_on_flag = 0, lte_init_flag = 0, lte_net_flag = 0, lte_connect_flag = 0; // lte��ʼ����ɱ�־������������ɱ�־�����ӳɹ���־
// uint8_t err_cnt = 0, err_num = 0;														  // ERROR ����������ֵ
// uint8_t heart_beat_flag = 0;															  // ������־

/* Private function prototypes -----------------------------------------------*/
void GPIO_Com3_Configuration(void); // GPIO����
void Com3_config(void);
uint CRC16(uchar *pCrcData, uchar CrcDataLen);
void Delay_MS(vu16 nCount);
void Enable_USART2_NVIC(void);
void Disable_USART2_NVIC(void);
void Com2_printf(char *fmt, ...);
void Com1_printf(char *fmt, ...);
void Com1_sendData(uint8_t txd_buffer[], uint16_t send_num);
void UART1_Output_Datas(uint8_t S_Out[], uint8_t Counter);
void UART2_Output_Datas(uint8_t S_Out[], uint8_t Counter);
void Send_DomainName(void);
void Fill_Dtu_Par(void);

// static uint16_t tim_cnt = 0;                         // ��ʱ������
// static uint8_t heart_beat_cnt = 0;                   // ��������
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
 * Function Name  : GPIO_Com3_Configuration
 * Description    : Configures the different GPIO ports.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void GPIO_Com3_Configuration(void) // ����GPIO����
{
	GPIO_InitTypeDef GPIO_InitStructure; // ����GPIO��ʼ���ṹ��

	/* Configure USART3 Rx (PB.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* Configure USART3 Tx (PB.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// ZCL 2018.11.5
}

/* USART3  configuration ------------------------------------------------------*/
/* USART  configured as follow:
		- BaudRate = 9600 baud
		- Word Length = 8 Bits
		- One Stop Bit
		- No parity
		- Hardware flow control disabled (RTS and CTS signals)
		- Receive and transmit enabled
				RTS/CTSЭ��(Request To Send/Clear To Send)��������/������Э��
*/
void Com3_config(void) // ����ͨѶ����
{
	USART_InitTypeDef USART_InitStructure;
	USART_DeInit(USART3);					// ��λ USART3
	USART_StructInit(&USART_InitStructure); // ��USART_InitStruct�е�ÿһ��������ȱʡֵ����

//	Pw_GprsBaudRate3 = 57600;					 // 57600;19200;
	USART_InitStructure.USART_BaudRate = 115200; // 2.25M/BPS   RS232оƬ:230.4kbps  
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure ����1-USART3 */
	USART_Init(USART3, &USART_InitStructure);
	/* Enable USART3 Receive and Transmit interrupts */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	/* Enable the USART3 */
	USART_Cmd(USART3, ENABLE);
}

/*******************************************************************************
 * Function Name  : Com3_Init
 * Description    : Initializes the peripherals used by the Usart driver.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Com3_Init(void) // ���ڳ�ʼ��
{
	GPIO_Com3_Configuration();
	Com3_config();
	//
	Cw_Txd3 = 0;
	Cw_Rcv3 = 0;
	//
	Delay_MS(35); // 2013.6.22 ����VBAT�ϵ絽�ܽ�PWRKEY����֮���ʱ����Ϊ30MS
}

// 2007.10.29
// ���մ������ У�����
void Com3_RcvProcess(void) // ���մ������ У�����
{
	u8 i = 0; // ��ʱ���� k,s,
	u16 j, m;
	uchar s; // ttp:transparent protocol ͸��Э��
	uint k, len1, len2;
	u16 *p_wRead;
	u8 *p_bMove;
	u16 *p_wTarget;
	//	u8 ipbuf[15]; // IP����

	/* 	u8	 *p_bGen;
		u16	 *p_wTarget;			// ָ��Ŀ���ַ�����xdata zcl  2021.11.17	 */

	// ��Ϊ����,ָ������ʱ�䵽��,�Ϳ��Դ�����յ����ַ�����
	//  ��û�յ������ַ���ʱ�䳬���趨ʱ�����ԶԽ��ջ�����д�����
	//  **********************************rcv_counter<>0,�յ��ַ����ܴ���
	// 2012.10.13 ����Cw_Txd2==0������
	if (Cw_Rcv3 != 0 && T_NoRcv3 != SClk1Ms) //&& Cw_Txd2 == 0
	{										 // ���մ������
		// Com3_CON=0;
		T_NoRcv3 = SClk1Ms; //
		C_NoRcv3++;
		if (C_NoRcv3 > NORCVMAXMS) // && Cw_Rcv3!=1 && Rcv3Buffer[0]!=0)
		{
			USART_ITConfig(USART3, USART_IT_RXNE, DISABLE); // ��ֹUSART3�����ж�,��ֹ�ڴ����ڼ����
			//
			usart3_rev_finish = 1;
			Rcv3Buffer[Cw_Rcv3] = 0; // 2013.5.9 �ܳ��� ���յ������ݣ�ĩβ��0��
			// if (GprsPar[ConsoleInfoTypeBase])
			// 	Com2_printf("Receive Data��\r\n %s\r\n", Rcv3Buffer);

			Cw_BakRcv3 = Cw_Rcv3; // �� Cw_Rcv3 ����
			Cw_Rcv3 = 0;		  // ׼���´ν��յ����濪ʼ
			C_NoRcv3 = 0;		  // ��û�н��ռ�����
			//
			if (Cw_BakRcv3 <= RCV3_MAX) // ���ճ�����ȷ,��������.
			{							//;��ʼ��������Ϣ
										//;ͨ����,����Ӧ��,�ط����� (�������ӻ�)
				if (S_M35 < 6)
				{
					if (strstr(Rcv3Buffer, "PB DONE") != NULL) //"Call Ready"
					{
						AT_CallReadyOk_Flag = TRUE;
					}
				}

				// �ܳ��� 2012.7.11 GPRS����ģʽ�·������ݵĴ���
				switch (S_M35)
				{
				case 0x01: // ������ϣ�����ES0=0���ٽ����ж�
					break; // �ܳ��� 2012.9.12
				case 0x03: // �ȴ�ͨ�ųɹ�  41 54 0D 0D 0A 4F 4B 0D 0A(AT...OK..)
				case 0x04: // ����AT�������
					goto ComOkCheck;

				case 0x05: // �ȴ�Call Ready�źŵ���		2012.9.12
					/* 											if(B_WaitCallReady==0)			//ZCL 2019.4.23 ɾ��  ������������������ض��ţ���������2��
																{
																	if(strstr(Rcv0Buffer,"OK")!=NULL )
																	{
																		AT_ComOk_Flag = TRUE;
																		B_WaitCallReady=1;
																	}
																} */

					if (strstr(Rcv3Buffer, "OK") != NULL) //"Call Ready"PB DONE
					{
						AT_CallReadyOk_Flag = TRUE;
						AT_ComOk_Flag = TRUE;
					}
					break;

					// 2017.5.25�޸ĳɼ���ź�ǿ�ȣ�case 0x40 �ĳ�case 0x06������case ��˳������ (6-E�ĳ�7-F)
				case 0x06:												   // 0D,0A, +CSQ: 23,0(һ��15���ϾͿ�����)
					if ((ptr1 = (u8 *)strstr(Rcv3Buffer, "+CSQ")) != NULL) // ZCL �ټ��ź�ǿ�ȼ��
					{
						ptr = (u8 *)strstr(Rcv3Buffer, ",");
						if (ptr != NULL)
						{
							i = 0;
							while (*ptr1 != 0x2c) //'\,'
							{
								Csq[i++] = *ptr1++;
							}
							if (*(ptr - 2) == 0x20) // �ո�
							{
								w_GprsCSQ = *(ptr - 1) - 0x30;	  //'0'
								GprsPar[OfflineIntervalBase] = 0; // ZCL 2017.5.30 �ÿ�������ʱ�����������ź�����
								GprsPar[OfflineIntervalBase + 1] = w_GprsCSQ;
							}
							else
							{
								w_GprsCSQ = (*(ptr - 2) - 0x30) * 10 + *(ptr - 1) - 0x30;
								GprsPar[OfflineIntervalBase] = 0; // ZCL 2017.5.30 �ÿ�������ʱ�����������ź�����
								GprsPar[OfflineIntervalBase + 1] = w_GprsCSQ;
							}
						}
						else
							w_GprsCSQ = 0;

						if (w_GprsCSQ >= 8 && w_GprsCSQ != 99)
							AT_ComOk_Flag = TRUE;
					}

				case 0x07: // �ȴ�ע������
					ptr = (u8 *)strstr(Rcv3Buffer, "+CREG:");
					if (ptr != NULL)
					{
						if (*(ptr + 9) == '1' || *(ptr + 9) == '5')
							AT_ComOk_Flag = TRUE;
					}
					break;

				case 0x08: // ���ò�����

				// ���ų�ʼ��
				case 0x09: // ѡ�����Ϣ�洢��
				case 0x0A: // ���ö���Ϣģʽ 		=0 PDU�� =1 �ı�
				case 0x0B: // ѡ�� TE�ַ���
				case 0x0C: // ������ʾ����
				case 0x0D: // �����ı���ʽ����
				case 0x0E: // ��ʾ�ı���ʽ���� =0��ʾ���ֲ��� =1��ʾȫ������
					goto ComOkCheck;
				case 0x0F: // ȷ�϶���Ϣ�洢��,����ѯ����
					// 0D 0A +CPMS: "SM",19,40,"SM",19,40,"SM",19,40
					// OK
					if (strstr(Rcv3Buffer, "+CPMS: ") != NULL)
					{
						AT_ComOk_Flag = TRUE;

						ptr = (u8 *)strstr(Rcv3Buffer, ","); // ���ҵ�һ��','
						if (ptr != NULL)
						{
							// ���յ���10���� ASCII�ַ�����19 ���� 0x31,0x39
							w_SmsNowSum = Str2Bcd(ptr + 1);
							ptr2 = (u8 *)strstr((char *)(ptr + 1), ","); // ���ҵڶ���','
							if (ptr2 != NULL)
							{
								w_SmsMaxSum = Str2Bcd(ptr2 + 1);
							}
						}
						// w_SmsMaxSum>=20 ����SIM�����Ŵ洢������10��
						if (w_SmsNowSum >= w_SmsMaxSum - 5 && w_SmsMaxSum >= 10)
							SmsFull = 1;
					}
					break;

				// GPRS
				case 0x10: // GPRS����
				case 0x11: // ����ǰ�ó���
				case 0x12: // ����GPRS���ӷ�ʽ 0:CSD����	1:GPRS����
				case 0x13: // ���÷�������ַΪIP��ʽ
				case 0x14: // �鿴������ʾ��ʽ 1������IP���˿ں�
				case 0x15: // �鿴������ʾ��ʽ 1������IPD<Len>
				case 0x16: // �鿴������ʾ��ʽ 1������Э������
				case 0x17: // 0:��·����   1:��·����
				case 0x18: //=1ʹ��͸��ģʽ =0��͸��
				case 0x19: // �յ��������ݺ�ȴ�200ms���ͣ��򴮿����ݴﵽ512�ֽں�
				case 0x1A: // ����AT�������
				ComOkCheck:
					if (strstr(Rcv3Buffer, "OK") != NULL)
					{
						AT_ComOk_Flag = TRUE;
					}
					break;

				case 0x1B:
					// һ.DTU���յ������ݴ���	���õĴ�������һЩ���ŵ�ǰ�棩
					// 		��.������·����ָ�������أ�UDP OR TCP��
					// 			UDP�����Ϸ�������ע�����1�ȷ�ָ��.ͨ����.���� At +
					// 	QISEND = x,
					// 	Y 2������ ��.���Ž��� 2012.9.19

					// 	һ.DTU���յ������ݴ���
					ptr = (uchar *)strstr(Rcv3Buffer, "RECV FROM:");

					if (ptr != NULL)
					{
						Com3_ReceiveData(); // YLS 2023.02.20
						// ChannelNo = *(ptr + 10) - 0x30; // Ӧ��ͨ��,�ĳ���ֵ

						// ���յ��Ĵ�Э�������ܳ���  �磺+RECEIVE: 0, 16
						// len1 = Str2Bcd(ptr + 13); // �ַ���ת10����ֵ
						// 2013.6.19 ż������һ�����պ󣬺����ָ�һ��������ݽ������룬��⵽�ȹ�����
						// 2013.6.19 ���޸ģ����κ�ֱ������

						len1 = ReceiveGPRSBufOneDateLen + ReceiveGPRSBufTwoDateLen + ReceiveGPRSBufThreeDateLen + ReceiveGPRSBufFourDateLen;
						if (len1 >= 512)
						{
							len1 = 500; // ZCL 2019.4.26 ���ƴ�С����ֹ�������

							if (B_RcvFail[ChannelNo] < 1)
							{
								B_RcvFail[ChannelNo]++;
								B_CloseLink[ChannelNo] = 1;
							}
							// ��2�ν��յ����루���������ݣ���������
							else
							{
								B_RcvFail[ChannelNo] = 0;
								B_SmsOrFaultHardRestart = 11; //=11����Ӳ������; =1����Ӳ������
							}
						}
						else
							B_RcvFail[ChannelNo] = 0;

						ptr2 = (uchar *)strstr(Rcv3Buffer, "{"); //{��λ�� =0x7B
						if (ptr2 != NULL)
						{
							// 1. DTUע��	 7B������ֽ��ǹ����룬0x81������ע�Ṧ����
							if (*(ptr2 + 1) == 0x81)
							{
								ZhuCeOkFLAG[*(ptr + 10) - 0x30] = 1; // ptr+10
							}
							// 2. ��ѯDTU����
							else if (*(ptr2 + 1) == 0x8B)
								B_Com3Cmd8B = 1;
							// 3. ����DTU����
							else if (*(ptr2 + 1) == 0x8D)
								B_Com3Cmd8D = 1;
							// 4. �����������նˣ�DTU����ָ��
							else if (*(ptr2 + 1) == 0x82)
							{
								S_M35 = 0x01;
								B_RestartNo = 1; // ZCL 2019.4.19 ������ţ������ӡ�۲�����
												 // printf("RestartNo=%d\r\n", B_RestartNo);			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
							}

							// 2. ��λ��ͨ��DTU������2��ָ��(���ܲ���Ҫ��������)
							// IPD17UDP:7B 89 00 10 31 33 39 31 32 33 34 35 36 37 38 7B 31 32 33
							// IPD17TCP:7B 89 00 13 31 33 39 31 32 33 34 35 36 37 38 31 32 33 7B
							else if (*(ptr2 + 1) == 0x89) //&& *(ptr+i+19)==0x7B
							{
								B_Com3Cmd89 = 1;
							}
							else
								B_OtherCmd = 1; //=1����λ��ͨ��DTU������2������ָ��Ժ��ټӾ���ָ��

							C_Com3NoRcv = 0; // ZCL 2017.5.30 COM0 û�н����������ݼ�����
						}

						// if(B_Com3Cmd89 || B_OtherCmd) 2014.12.5 �Ƶ�����
					}

					// ��.������·���ӣ�UDP OR TCP��		0, CONNECT OK  'C'��Rcv3Buffer[5]
					// ��ʱ��ǰ�滹���һ��OK(0D 0A O K 0D 0A), 'C'��Rcv3Buffer[11]
					else if ((ptr2 = (uchar *)strstr(Rcv3Buffer, "+CIPOPEN:")) != NULL) // YLS 2023.02.20
					{
						s = *(ptr2 + 10); // YLS 2023.02.20
						if (s >= 0x30)
						{
							// ����ԭ��ʹ�� CGD0_ConnectNo���ڲ���״̬�᲻�ԣ���Ϊ��ͨ���������ַ�
							// ָ�����ĳ��ý��յ��������ж����Ӻ�
							CGD0_LinkConnectOK[s - 0x30] = TRUE;
							// S_SendLink=0; 2013.6.19
						}
					}

					// UDP�����Ϸ�������ע�����1�ȷ�ָ��.ͨ����.����
					else if (strstr(Rcv3Buffer, ">") != NULL)
					{
						AT_3EOk_Flag = TRUE;
					}

					else if ((ptr2 = (uchar *)strstr(Rcv3Buffer, "+CIPSEND:")) != NULL) // YLS 2023.02.20
					{
						s = *(ptr2 + 10); // YLS 2023.02.20
						if (s >= 0x30)
						{
							// ����ԭ��ʹ�� CGD0_ConnectNo���ڲ���״̬�᲻�ԣ���Ϊ��ͨ���������ַ�
							// ָ�����ĳ��ý��յ��������ж����Ӻ�
							ZhuCeOkFLAG[s - 0x30] = TRUE;
							// S_SendLink=0; 2013.6.19
						}
						AT_CIPSEND_Flag = TRUE;
					}

					// 2013.6.19 û�����ӳɹ���CONNECT FAIL�����ر�����
					// else if ((ptr2 = (uchar *)strstr(Rcv3Buffer, "+CIPERROR")) != NULL)
					else if ((ptr2 = (uchar *)strstr(Rcv3Buffer, "+IPCLOSE:")) != NULL)
					{
						if ((ptr2 = (uchar *)strstr(Rcv3Buffer, ",")) != NULL)
						{
							s = *(ptr2 - 1) - 0x30;
							if (s <= 4)
							{
								CGD0_LinkConnectOK[s] = 0;
								ZhuCeOkFLAG[s] = 0;
								if (B_ConnectFail[s] < 3) // 1	ZCL 2019.4.24
								{
									B_ConnectFail[s]++;
									B_CloseLink[s] = 1;
								}
								// ��2�γ���"CONNECT FAIL"�󣬾�����
								//								else if (B_ZhuCeOkNum == 0)
								//								{
								//									B_SmsOrFaultHardRestart = 12; //=12����Ӳ������; =1����Ӳ������ ZCL 2019.4.19
								//								}
							}
						}
					}
					else if (strstr(Rcv3Buffer, "+CDNSGIP: 1") != NULL)
					{
						DomainNameResolution();
					}
					else if (strstr(Rcv3Buffer, "+CIPERROR:") != NULL)
					{
						Net_Error_Flag = TRUE;
					}

					// UDP�����Ϸ�������ע�����2������ SEND OK		//+CIPSEND:
					else if (strstr(Rcv3Buffer, "OK") != NULL)
					{
						AT_ComOk_Flag = TRUE;
					}
					// ��.���Ž��� 2012.9.19
					//+CMTI: "SM",2  ��λ��2���¶���
					else if (strstr(Rcv3Buffer, "+CMTI") != NULL)
					{
						ptr = (uchar *)strstr(Rcv3Buffer, ",");
						if (ptr != NULL)
							w_NewSmsNo = Str2Bcd(ptr + 1);
						// ���ܶ������������ж� 2012.9.24
						if (w_NewSmsNo >= w_SmsMaxSum - 5)
							SmsFull = 1;
					}
					// 0D 0A +CMGR: "REC UNREAD","+8615610566803","","2012/09/18
					// 22:38:13+32" 0D 0A
					// �����Ķ�������
					else if (strstr(Rcv3Buffer, "+CMGR") != NULL)
					{
						S_ReadNewSms = 0;
						w_NewSmsNo = 0;
						// ���������ŵĺ��� "+8615610566803"
						ptr2 = (uchar *)strstr(Rcv3Buffer, "+86");
						if (ptr2 != NULL)
						{
							for (s = 0; s < 11; s++) // ���������ŵĵ绰����
							{
								ComeSmsPhone[s] = *(ptr2 + 3 + s);
							}
						}
						// ���ź�PC���úͲ�ѯGPRS���� !!!!
						SmsAndPC_SetupInq();
					}
					// ���ŷ��ͳɹ�������+CMGS: 91  OK
					else if (strstr(Rcv3Buffer, "+CMGS") != NULL)
					{
						AT_ComOk_Flag = TRUE;
					}
					// ����ɾ�� 2013.5.23
					else if (Rcv3Buffer[2] == 'O' && Rcv3Buffer[3] == 'K')
					{
						if (S_SmsFull == 2 && S_SendSms == 0) // AT+CMGD=1,4 �ոշ�������ȫ��ɾ��ָ��
							AT_ComOk_Flag = TRUE;
					}
					// 2013.6.20 ���յ���ERROR��������
					else if (strstr(Rcv3Buffer, "ERROR") != NULL) // ZCL 2019.4.24 ע��������
					{
						if (B_ZhuCeOkNum == 0) // Cw_BakRcv3 == 9 &&
						{
							B_SmsOrFaultHardRestart = 13; //=13����Ӳ������; =1����Ӳ������ ZCL 2019.4.19
						}
					}

					// ���Լ�����������
					else
						;

					break;
					//----------------------------

				case 0x30: // �ݴ�
				case 0x31: // �ݴ�
				case 0x32: // �ݴ�

					break;
				case 0x41: // ģ��˯��������
					if (strstr(Rcv3Buffer, "OK") != NULL)
						AT_ComOk_Flag = TRUE;
					break;
				default:
					return;
				} // switch END

				// �ܳ��� 2012.7.7
				// 1.�����ϣ���B_Com3Cmd89.B_OtherCmd����ת��(DDPЭ�����Ҫ����,͸��Э���ֱ�Ӵ���)
				if (GprsPar[EnDDP0Base + ChannelNo] == 1 && (B_Com3Cmd89 || B_OtherCmd))
				{
					B_Com3Cmd89 = 0;
					B_OtherCmd = 0;
					B_DoWithData = 1;
				}
				else if (GprsPar[EnDDP0Base + ChannelNo] == 0 && ReceiveGPRSBuf[0] == 0x02)
				{
					B_DoWithData = 1;
				}

				if (B_DoWithData)
				{
					B_DoWithData = 0;

					// ���յ�����ֵ���ȣ�DDPЭ������Ҫ��16����������²��ü�
					len1 = ReceiveGPRSBufOneDateLen + ReceiveGPRSBufTwoDateLen + ReceiveGPRSBufThreeDateLen + ReceiveGPRSBufFourDateLen;
					if (GprsPar[EnDDP0Base + ChannelNo])
					{
						if (len1 >= 16) // ZCL 2019.4.16 ��������
							len2 = len1 - 16;
						else
							len2 = 0;
					}
					else
						len2 = len1;

					// 2012.10.15 ����DDPЭ��(TCP.UDP���ݸ�ʽ��ͬ)��͸��Э��(TCP.UDPһ��)
					// 2012.10.10 ����TCPЭ��(=1)��UDPЭ��(=0)
					// UDP:7B 89 00 10 31 33 39 31 32 33 34 35 36 37 38 7B 31 32 33
					// UDP˵����7B(��ʼ��־���̶�ֵ) 89(�����ͣ��̶�ֵ) 00 10(�����ȣ�2�ֽڣ��̶�ֵ) 31 33 39 31 32 33 34 35 36 37 38(DTU���ʶ���룬11�ֽ�) 7B(������־���̶�ֵ) 31 32 33(�û����ݣ���1024�ֽ�)
					// TCP:7B 89 00 13 31 33 39 31 32 33 34 35 36 37 38 31 32 33 7B
					// TCP˵��:7B(��ʼ��־���̶�ֵ) 89(�����ͣ��̶�ֵ) 00 13(�����ȣ�2�ֽڣ���0x10Ϊʵ��ֵ) 31 33 39 31 32 33 34 35 36 37 38(DTU���ʶ���룬11�ֽ�) 31 32 33(�û����ݣ���1024�ֽ�) 7B(������־���̶�ֵ)
					for (k = 0; k < len2; k++) // ����3(GPRSģ��)���յ��ķ��͵�����2
					{
						//=1 DDPЭ�� GprsPar[EnDDP0Base+ChannelNo]
						if (GprsPar[EnDDP0Base + ChannelNo])
						{
							//=0,UDP
							if (GprsPar[LinkTCPUDP0Base + ChannelNo] == 0)
								Txd2Buffer[k] = *(ptr2 + 16 + k);
							//=1,TCP
							else
								Txd2Buffer[k] = *(ptr2 + 15 + k);
						}
						//=0 ͸��Э�� GprsPar[EnDDP0Base+ChannelNo]
						// IPD3UDP:***  (***������) 2014.11.27 ����˵��
						else
						{
							// ptr3 = (u8 *)strstr(Rcv3Buffer, "P:"); // P:��λ��
							// if (ptr3 != NULL)
							// {
							// 	Txd2Buffer[k] = *(ptr3 + 2 + k);
							// }
							Txd2Buffer[k] = ReceiveGPRSBuf[k]; // ֱ�Ӵ���Txd2Buffer
						}
					}
					memset(ReceiveGPRSBuf, 0, len1);

					// ZCL 2019.3.12 ����GPRSģʽ������MODBUS���Ȼ���Լ���������
					// ע������ʹ�õ���Txd2Buffer���������ġ�����Rcv3Buffer
					if (Pw_EquipmentType == 0) // ˫����
					{
						if (F_GprsMasterNotToCom || F_GprsMasterToCom) // ZCL 2019.3.11 ����GPRSģʽ���趨�����Ϣ������
						{
							if (Txd2Buffer[0] == Pw_LoRaEquipmentNo) // �ӵ�ַ��⣭���յ�����λ����ѯָ��
							{
								j = CRC16(Txd2Buffer, len2 - 2); // CRC У��
								k = j >> 8;
								s = j;
								if (k == Txd2Buffer[len2 - 2] && s == Txd2Buffer[len2 - 1])
								{							// CRCУ����ȷ
									if (Txd2Buffer[1] == 3) // 03��ȡ���ּĴ���
									{
										B_Com3Cmd03 = 1;
										j = Txd2Buffer[2];
										Lw_Com3RegAddr = (j << 8) + Txd2Buffer[3];

										if (Lw_Com3RegAddr < 10000 || Lw_Com3RegAddr >= 60000)
										{
											Txd3TmpBuffer[0] = Pw_LoRaEquipmentNo; // ����LoRa��ַ
											Txd3TmpBuffer[1] = Txd2Buffer[1];	   // ������			ZCL 2019.3.12 ����Ƚ����⣬�õ�Txd2Buffer
											Txd3TmpBuffer[2] = Txd2Buffer[5] * 2;  // Rcv2Buffer[5]=���� ��

											if (Txd2Buffer[5] > 125)
												Txd2Buffer[5] = 125; // ZCL 2019.4.26 ���ƴ�С����ֹ�������

											if (Lw_Com3RegAddr < 10000)
											{
												p_wRead = w_DNBParLst; // w_DNBParLst PAR��
												p_wRead += Lw_Com3RegAddr - DNB_ADDRESS;
											}
											else if (Lw_Com3RegAddr >= 60000)
											{
												p_wRead = w_ParLst; // w_ParLst��
												p_wRead += Lw_Com3RegAddr - 60000;
											}

											p_bMove = Txd3TmpBuffer;
											for (k = 0; k < Txd2Buffer[5]; k++) // ����ѯ����
											{
												// m = *(p_wRead + Lw_Com3RegAddr + k);// 2023.12.16 YLS
												m = *(p_wRead + k); // 2023.12.16 YLS
												*(p_bMove + 3 + k * 2) = m >> 8;
												*(p_bMove + 3 + k * 2 + 1) = m;
											}
											Lw_Txd3ChkSum = CRC16(Txd3TmpBuffer, Txd3TmpBuffer[2] + 3);
											Txd3TmpBuffer[Txd3TmpBuffer[2] + 3] = Lw_Txd3ChkSum >> 8; // /256
											Txd3TmpBuffer[Txd3TmpBuffer[2] + 4] = Lw_Txd3ChkSum;	  // ��λ�ֽ�
											Cw_Txd3TmpMax = Txd3TmpBuffer[2] + 5;
											//
											B_Com3Cmd03 = 0;
											Cw_Txd3 = 0;

											// ZCL 2019.3.12 ����ָ��Ƚ���Ҫ��ģ��͸���У������յ����ݣ�ת����GPRS����
											B_GprsDataReturn = 1; // ģ��͸���У������յ����ݣ�ת����GPRS����
										}
										else if (Lw_Com3RegAddr >= 10000 && Lw_Com3RegAddr < 60000) // ��ַ>10000��Ҫͨ��LoRaת������ѯ�ӻ��Ĳ���
										{
											w_ZhuanFaAdd = Lw_Com3RegAddr;
											m = Txd2Buffer[4];
											m = (m << 8) + Txd2Buffer[5];
											w_ZhuanFaData = m;

											F_ModeParLora = 2; // ��LoRa���������У�=2��ת����ȥ����ѯĳһ̨�ӻ��Ĳ���
											B_Com3Cmd03 = 0;
										}
									}

									else if (Txd2Buffer[1] == 6) // 06Ԥ�õ��Ĵ���
									{
										B_Com3Cmd06 = 1;
										j = Txd2Buffer[2];
										Lw_Com3RegAddr = (j << 8) + Txd2Buffer[3];

										if (Lw_Com3RegAddr < 60000) // �����ַ��10000���£���ͨ��LoRaת������λ��
										{
											w_ZhuanFaAdd = Lw_Com3RegAddr;
											m = Txd2Buffer[4];
											m = (m << 8) + Txd2Buffer[5];
											w_ZhuanFaData = m;

											if (Lw_Com3RegAddr < 10000) // �����ַ��10000���£���ͨ��LoRaת������λ��
												F_ModeParLora = 1;		// ��LoRa���������У�=1��ת����ȥ��дĳһ̨�ӻ�����ͣ���趨ѹ�����趨Ƶ����3������
											else if (Lw_Com3RegAddr >= 10000 && Lw_Com3RegAddr < 60000)
												F_ModeParLora = 3; // ��LoRa���������У�=3��ת����ȥ��дĳһ̨�ӻ����趨����

											B_Com3Cmd06 = 0;
										}
										else if (Lw_Com3RegAddr >= 60000) // �����ַ��60000���ϣ����޸�ZGP331����Ĳ���
										{
											// ����Ԥ�ñ����� �趨������
											p_wTarget = w_ParLst; // дSTM32 PAR��
											p_wTarget += Lw_Com3RegAddr - 60000;
											m = Txd2Buffer[4];
											// w_ParLst[Lw_Com3RegAddr] = (m << 8) + Txd2Buffer[5];
											*p_wTarget = (m << 8) + Txd2Buffer[5]; // �޸Ĳ���

											// ZCL 2021.7.10  06ָ��յ��� ��  ���ص� ��һ���ġ�
											Txd3TmpBuffer[0] = Pw_LoRaEquipmentNo; // �豸�ӵ�ַPw_EquipmentNo
											Txd3TmpBuffer[1] = Txd2Buffer[1];	   // ������			ZCL 2019.3.12 ����Ƚ����⣬�õ�Txd2Buffer
											Txd3TmpBuffer[2] = Txd2Buffer[2];	   // ��
											Txd3TmpBuffer[3] = Txd2Buffer[3];	   //
											Txd3TmpBuffer[4] = Txd2Buffer[4];	   //
											Txd3TmpBuffer[5] = Txd2Buffer[5];	   //

											Lw_Txd3ChkSum = CRC16(Txd3TmpBuffer, 6);
											Txd3TmpBuffer[6] = Lw_Txd3ChkSum >> 8; // /256
											Txd3TmpBuffer[7] = Lw_Txd3ChkSum;	   // ��λ�ֽ�
											Cw_Txd3TmpMax = 8;
											//
											B_Com3Cmd06 = 0;
											Cw_Txd3 = 0;
											// ZCL 2019.3.12 ����ָ��Ƚ���Ҫ��ģ��͸���У������յ����ݣ�ת����GPRS����
											B_GprsDataReturn = 1; // ģ��͸���У������յ����ݣ�ת����GPRS����
										}
									}
								}
							}
						}
					}
					else // ��Ƶ���
					{
						if (F_GprsMasterNotToCom || F_GprsMasterToCom)
						{
							if (Txd2Buffer[0] == 2) // �ӵ�ַ��⣭���յ�����λ����ѯָ��
							{
								j = CRC16(Txd2Buffer, len2 - 2); // CRC У��
								k = j >> 8;
								s = j;
								if (k == Txd2Buffer[len2 - 2] && s == Txd2Buffer[len2 - 1])
								{							// CRCУ����ȷ
									if (Txd2Buffer[1] == 3) // 03��ȡ���ּĴ���
									{
										B_Com3Cmd03 = 1;
										j = Txd2Buffer[2];
										Lw_Com3RegAddr = (j << 8) + Txd2Buffer[3];
									}
									else if (Txd2Buffer[1] == 1) // 01��ȡ��Ȧ״̬
									{
										B_Com3Cmd01 = 1;
									}
									else if (Txd2Buffer[1] == 6) // 06Ԥ�õ��Ĵ���
									{
										// C_ForceSavPar=0;		// ǿ�Ʊ������������=0
										B_Com3Cmd06 = 1;
										j = Txd2Buffer[2];
										Lw_Com3RegAddr = (j << 8) + Txd2Buffer[3];

										// ZCL 2021.7.10  ����Ҫ�޸ĵ�����  ICCARD ȡˮ��
										// ZCL 2021.11.17

										// ����Ԥ�ñ����� �趨������ ����������Ҫ����Ԥ�� LORA�ӻ��Ĳ�������Ҫת���ģ�
										/* 									m=Txd2Buffer[4];
																			w_ParLst[Lw_Com3RegAddr]=(m<<8)+Txd2Buffer[5];	 */

										// ZCL 2021.11.17 ת��ָ��
										B_LoraSendWrite = 1; // ZCL 2021.11.17

										//									w_ZhuanFaAdd = Lw_Com3RegAddr;
										m = Txd2Buffer[4];
										m = (m << 8) + Txd2Buffer[5];
										//				*(p_wTarget + Lw_Com2RegAddr) = m;
										//									w_ZhuanFaData = m;
										F_ModeParLora = 1;
									}

									// ZCL 2021.11.17
									else if (Txd2Buffer[1] == 16) // 16Ԥ�ö�Ĵ���
									{
										// C_ForceSavPar=0;		// ǿ�Ʊ������������=0
										B_Com3Cmd16 = 1;
										j = Txd2Buffer[2];
										Lw_Com3RegAddr = (j << 8) + Txd2Buffer[3];

										// ����Ԥ�ñ����� �趨������ ����������Ҫ����Ԥ�� LORA�ӻ��Ĳ�������Ҫת���ģ�
										/* 									if ( Txd2Buffer[5] < 30 )			//ZCL 2021.11.17  ������
																			{
																				p_bGen = Txd2Buffer;
																				p_wTarget = w_ParLst;
																				for ( k = 0; k < Txd2Buffer[5] ; k++ )		// Rcv0Buffer[5]=����
																				{
																					m = *( p_bGen + 7 + k * 2 );
																					n = *( p_bGen + 7 + k * 2 + 1 );
																					*( p_wTarget + Lw_Com3RegAddr + k ) = ( m << 8 ) + n;
																				}
																			} */

										// ZCL 2021.11.17 ת��ָ��
										B_LoraSendWrite = 1; // ZCL 2021.11.17
										B_LoraSendWriteLength = len2;

										for (j = 0; j < B_LoraSendWriteLength; j++) // Rcv0Buffer[5]=����
										{
											LoRaTxBuf2[j] = Txd2Buffer[j];
										}
									}

									//								else
									//									i = 1;
								}
								//							else
								//								i = 2;
							}
							//						else
							//							i = 3;
						}

						// ZCL 2019.3.12  ����GPRSģʽ�£�������ָ�׼�����أ�
						if (B_Com3Cmd03)
						{
							Txd3TmpBuffer[0] = 2;				  // �豸�ӵ�ַPw_EquipmentNo
							Txd3TmpBuffer[1] = Txd2Buffer[1];	  // ������			ZCL 2019.3.12 ����Ƚ����⣬�õ�Txd2Buffer
							Txd3TmpBuffer[2] = Txd2Buffer[5] * 2; // Rcv2Buffer[5]=���� ��

							// ת����ַ ZCL 2015.7.11

							if (Txd2Buffer[5] > 125)
								Txd2Buffer[5] = 125; // ZCL 2019.4.26 ���ƴ�С����ֹ�������

							p_wRead = w_GprsParLst; // ��w_GprsParLst PAR��	2019.3.14
							p_bMove = Txd3TmpBuffer;
							for (k = 0; k < Txd2Buffer[5]; k++) // ����ѯ����
							{
								m = *(p_wRead + Lw_Com3RegAddr + k);
								*(p_bMove + 3 + k * 2) = m >> 8;
								*(p_bMove + 3 + k * 2 + 1) = m;
							}
							Lw_Txd3ChkSum = CRC16(Txd3TmpBuffer, Txd3TmpBuffer[2] + 3);
							Txd3TmpBuffer[Txd3TmpBuffer[2] + 3] = Lw_Txd3ChkSum >> 8; // /256
							Txd3TmpBuffer[Txd3TmpBuffer[2] + 4] = Lw_Txd3ChkSum;	  // ��λ�ֽ�
							Cw_Txd3TmpMax = Txd3TmpBuffer[2] + 5;
							//
							//
							B_Com3Cmd03 = 0;
							Cw_Txd3 = 0;

							// ZCL 2019.3.12 ����ָ��Ƚ���Ҫ��ģ��͸���У������յ����ݣ�ת����GPRS����
							B_GprsDataReturn = 1; // ģ��͸���У������յ����ݣ�ת����GPRS����

							// RS485_CON=1;
							// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
							// USART_SendData(USART2,Txd2Buffer[Cw_Txd2++]);
							// USART_ITConfig(USART2, USART_IT_TC, ENABLE);				// ��ʼ����.
						}

						// ZCL 2019.3.12 ����������ָ��  2021.11.17
						else if (B_Com3Cmd06) // ZCL 2021.7.10  06ָ��յ��� ��  ���ص� ��һ���ġ�
						{
							B_Com3Cmd06 = 0;
							//...
							// ZCL 2021.7.10  06ָ��յ��� ��  ���ص� ��һ���ġ�
							Txd3TmpBuffer[0] = 2;			  // �豸�ӵ�ַPw_EquipmentNo
							Txd3TmpBuffer[1] = Txd2Buffer[1]; // ������			ZCL 2019.3.12 ����Ƚ����⣬�õ�Txd2Buffer
							Txd3TmpBuffer[2] = Txd2Buffer[2]; // ��
							Txd3TmpBuffer[3] = Txd2Buffer[3]; //
							Txd3TmpBuffer[4] = Txd2Buffer[4]; //
							Txd3TmpBuffer[5] = Txd2Buffer[5]; //

							Lw_Txd3ChkSum = CRC16(Txd3TmpBuffer, 6);
							Txd3TmpBuffer[6] = Lw_Txd3ChkSum >> 8; // /256
							Txd3TmpBuffer[7] = Lw_Txd3ChkSum;	   // ��λ�ֽ�
							Cw_Txd3TmpMax = 8;
							//
							//
							B_Com3Cmd06 = 0;
							Cw_Txd3 = 0;

							// ZCL 2019.3.12 ����ָ��Ƚ���Ҫ��ģ��͸���У������յ����ݣ�ת����GPRS����
							B_GprsDataReturn = 1; // ģ��͸���У������յ����ݣ�ת����GPRS����

							// RS485_CON=1;
							// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
							// USART_SendData(USART2,Txd2Buffer[Cw_Txd2++]);
							// USART_ITConfig(USART2, USART_IT_TC, ENABLE);				// ��ʼ����.

							// if (B_Com2Cmd06) // Ԥ�õ���

							// else if (B_Com2Cmd16) // Ԥ�ö��
							// {
							// 	if (Rcv2Buffer[6] == 2)
							// 	{
							// 		m = Rcv2Buffer[2];
							// 		m = (m << 8) + Rcv2Buffer[3];
							// 		w_ZhuanFaAdd = m;

							// 		m = Rcv2Buffer[7];
							// 		m = (m << 8) + Rcv2Buffer[8];
							// 		w_ZhuanFaData = m;

							// 		F_ModeParLora = 1;
							// 	}
							// 	else
							// 		F_ModeParLora = 0;
							// }
						}

						// ZCL 2019.3.12 ����������ָ��  2021.11.17
						else if (B_Com3Cmd16) // ZCL 2021.7.10  06ָ��յ��� ��  ���ص� ��һ���ġ�
						{
							B_Com3Cmd16 = 0;
							//...
							// ZCL 2021.7.10  06ָ��յ��� ��  ���ص� ��һ���ġ�
							Txd3TmpBuffer[0] = 2;			  // �豸�ӵ�ַPw_EquipmentNo
							Txd3TmpBuffer[1] = Txd2Buffer[1]; // ������			ZCL 2019.3.12 ����Ƚ����⣬�õ�Txd2Buffer
							Txd3TmpBuffer[2] = Txd2Buffer[2]; // ��
							Txd3TmpBuffer[3] = Txd2Buffer[3]; //
							Txd3TmpBuffer[4] = Txd2Buffer[4]; //
							Txd3TmpBuffer[5] = Txd2Buffer[5]; //

							Lw_Txd3ChkSum = CRC16(Txd3TmpBuffer, 6);
							Txd3TmpBuffer[6] = Lw_Txd3ChkSum >> 8; // /256
							Txd3TmpBuffer[7] = Lw_Txd3ChkSum;	   // ��λ�ֽ�
							Cw_Txd3TmpMax = 8;
							//
							//
							B_Com3Cmd16 = 0;
							Cw_Txd3 = 0;

							// ZCL 2019.3.12 ����ָ��Ƚ���Ҫ��ģ��͸���У������յ����ݣ�ת����GPRS����
							B_GprsDataReturn = 1; // ģ��͸���У������յ����ݣ�ת����GPRS����

							// RS485_CON=1;
							// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
							// USART_SendData(USART2,Txd2Buffer[Cw_Txd2++]);
							// USART_ITConfig(USART2, USART_IT_TC, ENABLE);				// ��ʼ����.
						}
					}

					// ZCL 2019.3.12 	�Խ��յ������ݣ����д������
					if (F_GprsTransToCom) //|| F_GprsMasterToCom	YLS 2023.03.28
					{
						Cw_Txd2Max = len2;
						Cw_Txd2 = 0;

						// RS485_CON=1;		//2013.9.2
						//  2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
						// USART_ITConfig(USART2, USART_IT_TXE, ENABLE);				// ��ʼ����.
						USART_ITConfig(USART2, USART_IT_TC, ENABLE); // ��ʼ����.
						USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]);
					}
				}

				// 2.1 (�����ӣ���B_Com3Cmd89.B_OtherCmd�������1����ת��������2) Ŀǰû����
				// 2.2 �����ӣ�һЩATָ��ؽ��������1����ת��������2 2013.5.20
				// ZCL 2019.4.23 ��������ݴ���ת���꣬�ٽ���printf��Ϣ���
				else if (GprsPar[ConsoleInfoTypeBase]) // ����̨��Ϣ����,=1���������Ϣ
				{
					if (Cw_BakRcv3 >= 512)
						Cw_BakRcv3 = 500; // ZCL 2019.4.26 ���ƴ�С����ֹ�������

					for (k = 0; k < Cw_BakRcv3; k++)
					{
						Txd2Buffer[k] = Rcv3Buffer[k]; // ����3(GPRSģ��)���յ��ķ��͵�����1�ʹ���2
					}

					Cw_Txd2Max = Cw_BakRcv3;
					Cw_Txd2 = 0;
					RS485_CON = 1; // 2013.9.2
					// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
					// USART_ITConfig(USART2, USART_IT_TXE, ENABLE);				// ��ʼ����.
					USART_ITConfig(USART2, USART_IT_TC, ENABLE); // ��ʼ����.
					USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]);
				}
				else if (Pw_ConsoleInfo > 0) // Һ��������������̨��Ϣ����,=1���������Ϣ
				{
					if (Cw_BakRcv3 >= 512)
						Cw_BakRcv3 = 500; // ZCL 2019.4.26 ���ƴ�С����ֹ�������

					for (k = 0; k < Cw_BakRcv3; k++)
					{
						Txd1Buffer[k] = Rcv3Buffer[k]; // ����3(GPRSģ��)���յ��ķ��͵�����1�ʹ���2
					}
					Cw_Txd1Max = Cw_BakRcv3;
					Cw_Txd1 = 0;
					USART_ITConfig(USART1, USART_IT_TC, ENABLE); // ��ʼ����.
					USART_SendData(USART1, Txd1Buffer[Cw_Txd1++]);
				}
			}

			for (j = 0; j < Cw_BakRcv3; j++)
			{
				Rcv3_Back_Buffer[j] = Rcv3Buffer[j];
			}

			for (j = 0; j < Cw_BakRcv3; j++)
			{
				Rcv3Buffer[j] = 0;
			}

			// ���ж�
			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // 2014.12.5 �о��ں��洦���꣬�ٿ��ж�
		}
	}
}

void Com3_SlaveSend(void) // ����3�ӻ�����
{
	uint k, w;
	uchar j = 0, i, t, u, oneCmdLen, allCmdLen;
	u16 data_len;
	// uint	 *p_wRead;
	// uchar	 *p_bMove;
	// uchar	 *p_bGen;
	// uint	 *p_wTarget;			// ָ��Ŀ���ַ�����xdata zcl
	// uchar  *ptr;

	// ZCL 2018.12.10 ɾ�� 03 06 16ָ��
	if (S_HeartSend + S_SendLink + S_SendSms + S_ReadNewSms + S_SmsFull > 0)
	{
		;
	}

	// 1.��ѯDTU��������  2013.5.15�޸�  ������ݣ����㳤��
	else if (B_Com3Cmd8B && S_DtuSetSend + S_GprsDataSend == 0)
	{
		// 1.0����ATָ�������ݣ����㳤��
		if (S_DtuInquireSend == 0 && Cw_Txd3Max == 0)
		{
			S_DtuInquireSend = 1;
			C_DtuInquireSend = 0;
			C_BetweenDtuInquireSend = 0;
		}

		// 1.1����ATָ�������ݣ����㳤��
		else if (S_DtuInquireSend == 1 && Cw_Txd3Max == 0 && C_BetweenDtuInquireSend > 3)
		{
			S_DtuInquireSend = 2;
			C_BetweenDtuInquireSend = 0;

			// ���Ҫ��ѯ�Ĳ�������31-353��������354���ֽڣ���ַ��0��ʼ��ǰ��31���ֽ�Ϊ�̶��ģ�DTU���������룩
			Fill_Dtu_Par();

			// f. AT����ָ��		�ȷ�ָ��.ͨ����.����
			Cw_Txd3Max = 0; // 2013.5.11 ��һ��ǳ���Ҫ��At_QISENDAccordingLengthָ������0��ַ��ʼ
			At_QISENDAccordingLength(ChannelNo, w_StringLength);
			// F_GPRSSendFinish = 0;
			// sim7600ceGPRSUpLoaded(Txd3Buffer, w_StringLength, 1);
		}
		// 2014.12.11 	��Ϊ�����ղ���AT_3EOk_Flag��־�����Լ�����ʱ���ط�����
		else if (S_DtuInquireSend == 2 && C_BetweenDtuInquireSend > 300)
		{
			S_DtuInquireSend = 1; // ZCL1
			C_BetweenDtuInquireSend = 0;
		}
		// 1.2 ����ǰ250���ֽ�
		else if (S_DtuInquireSend == 2 && AT_3EOk_Flag) //
		{
			S_DtuInquireSend = 3;
			AT_3EOk_Flag = 0;
			// 7.�������ǰ31���ֽ�
			Cw_Txd3Max = 0;						// 2013.5.11 ��һ��ǳ���Ҫ���������ǰ31���ֽ�
			Gprs_TX_Fill(GPRS_ServiceCode, 31); // ��ʼͷ �� 01-04
			// 8.�޸ĳ���
			Txd3Buffer[2] = (w_StringLength >> 8); // �޸ĳ��ȸ��ֽ�
			Txd3Buffer[3] = w_StringLength;		   // �޸ĳ��ȵ��ֽ�
			// 9.�޸�DTU���ʶ����
			for (t = 0; t < 11; t++)
				Txd3Buffer[4 + t] = GprsPar[DtuNoBase + t];

			// �ٴ����Ҫ��ѯ�Ĳ�������31-353��������354���ֽڣ���ַ��0��ʼ��ǰ��31���ֽ�Ϊ�̶��ģ�DTU���������룩
			Fill_Dtu_Par();

			// ���ݷ���
			Cw_Txd3 = 0;
			Cw_Txd3Max = 250;
			USART_SendData(USART3, Txd3Buffer[Cw_Txd3++]);
			USART_ITConfig(USART3, USART_IT_TC, ENABLE); // �ĳ�USART_IT_TC,��ʼ����.
														 // UART3_Output_Datas(Txd3Buffer, Cw_Txd3Max);
		}
		// 1.3 ���ͺ�����ֽ�(w_StringLength-250)
		else if (S_DtuInquireSend == 3 && Cw_Txd3Max == 0)
		{
			S_DtuInquireSend = 4;
			// ���ݷ���
			Cw_Txd3 = 0;
			// Cw_Txd3Max=w_StringLength-250;

			// ZCL 2019.4.26 ���ƴ�С����ֹ�������
			if (w_StringLength >= 250)
				Cw_Txd3Max = w_StringLength - 250;
			else
				Cw_Txd3Max = 0;
			if (Cw_Txd3Max >= 512)
				Cw_Txd3Max = 500;

			for (i = 0; i < Cw_Txd3Max; i++) // �ƶ�����
			{
				Txd3Buffer[i] = Txd3Buffer[250 + i];
			}
			USART_SendData(USART3, Txd3Buffer[Cw_Txd3++]);
			USART_ITConfig(USART3, USART_IT_TC, ENABLE); // �ĳ�USART_IT_TC,��ʼ����.
			// UART3_Output_Datas(Txd3Buffer, Cw_Txd3Max);
			//
			C_BetweenDtuInquireSend = 0;
		}
		// 2014.12.11 	��Ϊ�����ղ���AT_ComOk_Flag��־�����Լ�����ʱ���ط�����
		else if (S_DtuInquireSend == 4 && C_BetweenDtuInquireSend > 1000)
		{
			S_DtuInquireSend = 1; // ZCL2
			C_BetweenDtuInquireSend = 0;
		}
		// 1.4 ������ɣ����־
		else if (S_DtuInquireSend == 4) //&& AT_ComOk_Flag
		{
			AT_ComOk_Flag = 0;
			S_DtuInquireSend = 0;
			B_Com3Cmd8B = 0;
			// ������÷�����������־
			C_HeartDelay[ChannelNo] = 0;
			B_HeartSendAsk[ChannelNo] = 0;
		}

		// 1.5 ��ʱ
		if (C_DtuInquireSend > 20)
		{
			S_DtuInquireSend = 0;
			B_Com3Cmd8B = 0;
		}
	}

	// 2.����DTU�����ͷ��� 2012.8.24
	else if (B_Com3Cmd8D && S_DtuInquireSend + S_GprsDataSend == 0)
	{
		// ����DTU����
		if (S_DtuSetSend == 0)
		{
			S_DtuSetSend = 1;
			C_DtuSetSend = 0;
			C_BetweenDtuSetSend = 0;

			// 0x7B(��һ������) �磺7B 8D 00 13 31 33 39 31 32 33 34 35 36 37 38 14 03 06 7B
			//			ptr = (u8 *)strstr(Rcv3Buffer, "{"); //"{"����0x7B

			ptr = (u8 *)strstr(Rcv3_Back_Buffer, "{"); //"{"����0x7B
			if (ptr != NULL)
			{
				oneCmdLen = 0;
				// ����ʱ�����ܳ���255�ֽڣ�����ֻʹ��һ���ֽڳ����������allCmdLen��
				allCmdLen = *(ptr + 3);			// DDPָ�����ֽ���	�磺UDP:7B 8D 00 13(13�������ֽ���)
				ptr = ptr + DDPHEADTAILLEN - 1; // ָ����� (Ҫ�������һ��ĵ�һ���ֽ�)

				// ��Ҫ�����ʣ���ֽ�������һ�δ���һ��ָ����Ⱥ����ݣ�
				while (allCmdLen - DDPHEADTAILLEN - oneCmdLen) // DDPHEADTAILLEN=16  :DDPЭ��ͷ��β����
				{
					CommandNo = *ptr++;
					if (CommandNo != 0x7B) // ���ǽ�����
					{
						// 1.�ҵ�ָ��ֵ��GPRS �����������е�λ�ã����ж��Ƿ���Ч
						k = FindFirstValue(CmdNo, CommandNo, 0x00);
						if (k == 255)
							break;

						// 2.ȡָ��� (Ҫ�������һ��ĵڶ����ֽ�)
						t = *ptr++;

						if (t == 2) // ���t=2������û������  �磺2C 02 ����û�и�����
						{
							GprsPar[CmdPst[k]] = 0; // ��ֵΪ0
						}
						else if (t > 2) // ZCL 2019.4.26 + if(t>2)
						{
							// 3.ȡָ�����ݣ����� -��-2(�磺14 03 06)
							for (j = 0; j < t - 2; j++)
							{
								u = *(ptr++);				// ����
								GprsPar[CmdPst[k] + j] = u; // ���и�ֵ
							}
							if (!B_ZhuDongSendSms) // 2014.12.9 �������Ͷ��ţ����ñ���
							{
								B_ForceSavPar = 1; // Ҫ�ȴ�����2�����꣬������1�����꣬�ſ��Ա������
								SavePar_Prompt();  // ������� 2012.10.11
							}

							// 4. ����ж�������ַ�������Ҫ��ĩβ��0  �磺0x05��CMNET\0
							if (CommandNo == 0x01 || CommandNo == 0x05 || CommandNo == 0x06 || CommandNo == 0x21 || CommandNo == 0x31 || CommandNo == 0x41 || CommandNo == 0x51)
							{
								GprsPar[CmdPst[k] + j] = 0; // 2012.9.20 �ܳ��� �ַ�����ĩβ��0

								if (!B_ZhuDongSendSms) // 2014.12.9 �������Ͷ��ţ����ñ���
								{
									B_ForceSavPar = 1; // Ҫ�ȴ�����2�����꣬������1�����꣬�ſ��Ա������
									SavePar_Prompt();  // ������� 2012.10.11
								}
								// 2013.6.14 DTU���ʶ���롣ԭ�������ӣ��ȹرգ�2�������
								if (CommandNo == 0x06)
								{
									for (j = 0; j < 4; j++)
									{
										if (CGD0_LinkConnectOK[j]) //(DTU���ʶ�����޸���)
											B_CloseLink[j] = 1;	   // �Զ��رո������ӣ�����̫����
																   // S_M35 = 0x01;							//����
									}
								}
								// 2014.12.7 ��������4��������ǰ���@Ϊ�绰���룬�����ط����š�
								else if (CommandNo == 0x51)
								{
									if (GprsPar[DomainName3Base] == '@' && t - 2 == 12)
									{
										GprsPar[DomainName3Base] = 0; // ��@
										B_ZhuDongSendSms = 1;
									}
								}
							}
							// 20��30��40��50�ֱ����1,2,3,4ͨ��DSC��IP��ַ
							else if (CommandNo == 0x20 || CommandNo == 0x30 || CommandNo == 0x40 || CommandNo == 0x50)
							{
								// 2013.6.11 ԭ�������ӣ��ȹرգ�2�������
								for (j = 0; j < 4; j++)
								{
									if (CGD0_LinkConnectOK[j] && CommandNo == 0x20 + 0x10 * j) //(IP��ַ�޸���)
										B_CloseLink[j] = 1;
									//										S_M35 = 0x01;							//����
								}
							}
							// 22��32��42��52�ֱ����1,2,3,4ͨ��DSC�Ķ˿ں�
							else if (CommandNo == 0x22 || CommandNo == 0x32 || CommandNo == 0x42 || CommandNo == 0x52)
							{
								// 2013.6.11 ԭ�������ӣ��ȹرգ�2�������
								for (j = 0; j < 4; j++)
								{
									if (CGD0_LinkConnectOK[j] && CommandNo == 0x22 + 0x10 * j) //(�˿��޸���)
										B_CloseLink[j] = 1;
									//										S_M35 = 0x01;							//����
								}
							}
							// ���� �޸�24��34��44��54�ֱ����1,2,3,4ͨ����������ʽ��0-UDP��1-TCP������Ҫ����
							else if (CommandNo == 0x24 || CommandNo == 0x34 || CommandNo == 0x44 || CommandNo == 0x54)
							{
								// 2013.6.21 ԭ�������ӣ�Ӳ������
								for (j = 0; j < 4; j++) // 2013.6.19
								{
									if (CGD0_LinkConnectOK[j] && CommandNo == 0x24 + 0x10 * j) //(TCP,UDPЭ���޸�)
									{
										B_SmsOrFaultHardRestart = 14; //=14����Ӳ������; =1����Ӳ������ ZCL 2019.4.19
																	  // B_CloseLink[j] = 1;
									}
									//									S_M35 = 0x01;							//����
								}
							}
							// 2013.12.13 �ܳ��� ����2������
							else if (CommandNo == 0x0D)
							{
								// 1.��������9600
								w = (uint)(GprsPar[BaudRateBase + 2]) << 8;
								w = w + GprsPar[BaudRateBase + 3];
								if (w != 2400 && w != 4800 && w != 9600 && w != 19200 && w != 38400 && w != 57600)
								{
									GprsPar[BaudRateBase + 2] = (9600 >> 8);
									GprsPar[BaudRateBase + 3] = (9600 & 0x00ff); // YLS 2023.03.12 ����ط�ԭ����&&��Ӧ���Ǵ���
								}
								// 2. 03 ����λΪ8λ��00-5�� 01-6�� 02-7�� 03-8����������ֻ������03��
								if (GprsPar[BaudRateBase + 4] != 0x03)
									GprsPar[BaudRateBase + 4] = 0x03;
								// 3. 00 ֹͣλΪ1λ��00-1��04-2�����������������00��04��
								if (GprsPar[BaudRateBase + 5] != 0x00 && GprsPar[BaudRateBase + 5] != 0x04)
									GprsPar[BaudRateBase + 5] = 0x00;
								// 4. 00 У��λ-�ޣ�00-�ޣ�08-�棬18-ż�����������������00��08��18��
								if (GprsPar[BaudRateBase + 6] != 0x00 && GprsPar[BaudRateBase + 6] != 0x08 && GprsPar[BaudRateBase + 6] != 0x18)
									GprsPar[BaudRateBase + 6] = 0x00;
								// 5. 03 ����-�����أ�01-Xon/Xoff��02-Ӳ���أ�03-�����أ�04-��˫��485��05-ȫ˫��422��
								// ������ֻ������02��03��
								if (GprsPar[BaudRateBase + 7] != 0x03 && GprsPar[BaudRateBase + 7] != 0x02)
									GprsPar[BaudRateBase + 7] = 0x03;
							}
						}
						// 5.��������ִ�У���Ϊ���ò������Զ����������һ�鴫������
						oneCmdLen += t; // ָ����һ������λ��
					}
				}
			}
		}

		// DTU����
		else if (S_DtuSetSend == 1 && Cw_Txd3Max == 0 && C_BetweenDtuSetSend > 3)
		{
			S_DtuSetSend = 2;
			C_BetweenDtuSetSend = 0;
			// 1. AT����ָ��		�ȷ�ָ��.ͨ����.����
			// Cw_Txd3Max=0;		//�ж���������Cw_Txd3Max==0�����Դ˾�ע�͵�
			At_QISENDAccordingLength(ChannelNo, 16);
			// F_GPRSSendFinish = 0;
			// sim7600ceGPRSUpLoaded(Txd3Buffer, 16, 1);
		}
		// 2014.12.11 	��Ϊ�����ղ���AT_3EOk_Flag��־�����Լ�����ʱ���ط�����
		else if (S_DtuSetSend == 2 && C_BetweenDtuSetSend > 300)
		{
			S_DtuSetSend = 1; // ZCL1
			C_BetweenDtuSetSend = 0;
		}

		else if (S_DtuSetSend == 2 && AT_3EOk_Flag) //
		{
			S_DtuSetSend = 3;
			AT_3EOk_Flag = 0;
			Cw_Txd3Max = 0; // 2014.12.8
			// ���������
			Gprs_TX_Fill(GPRS_SetParReturn, 16); //	Cw_Txd3Max���ű仯
			// �޸�DTU���ʶ����
			for (t = 0; t < 11; t++)
				Txd3Buffer[4 + t] = GprsPar[DtuNoBase + t];

			// ���ݷ���
			Gprs_TX_Start();
			//
			C_BetweenDtuSetSend = 0;
		}
		// 2014.12.11 	��Ϊ�����ղ���AT_ComOk_Flag��־�����Լ�����ʱ���ط�����
		else if (S_DtuSetSend == 3 && C_BetweenDtuSetSend > 1000)
		{
			S_DtuSetSend = 1; // ZCL2
			C_BetweenDtuSetSend = 0;
		}
		else if (S_DtuSetSend == 3 && AT_ComOk_Flag) //&& Rcv3Buffer[2]=='S'
		{
			S_DtuSetSend = 4;
			AT_ComOk_Flag = 0;
			C_BetweenDtuSetSend = 0;

			if (!B_ZhuDongSendSms) // 2014.12.9 �������Ͷ��ţ����ñ���
			{
				B_ForceSavPar = 1; // Ҫ�ȴ�����2�����꣬������1�����꣬�ſ��Ա������
				SavePar_Prompt();  // ������� 2012.10.11
			}
			// ������÷�����������־
			C_HeartDelay[ChannelNo] = 0;
			B_HeartSendAsk[ChannelNo] = 0;
		}

		// 2014.12.11 ����������ʱ���֡��������óɹ����ȷ�����Ų���ʾ������Ӹ���ʱ���÷�����ţ�
		else if (S_DtuSetSend == 4 && C_BetweenDtuSetSend > 300) // MS
		{
			S_DtuSetSend = 0;
			B_Com3Cmd8D = 0;
			C_BetweenDtuSetSend = 0;
		}

		// 4. ��ʱ
		if (C_DtuSetSend > 20)
		{
			S_DtuSetSend = 0;
			B_Com3Cmd8D = 0;
		}
	}

	// 3.Զ����λ�����͸�DTU����ָ��ķ��أ��Ѿ����ӵ�ÿ·���Ӷ����� 2012.8.24
	else if (B_GprsDataReturn && S_DtuInquireSend + S_DtuSetSend == 0)
	{
		// ��ų���������ӣ���ʱ5MS�����B_GprsDataReturn
		if (SendDataReturnNo > 3)
		{
			if (C_BetweenSendDataReturn > 5) // Ms
			{
				SendDataReturnNo = 0;
				B_GprsDataReturn = 0;
				C_GprsDataSend = 0;
				S_GprsDataSend = 0;
				/* Enable USART2 Receive interrupts */ // 2014.11.27 ת���꣬�ٿ��ж�
				USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
				USART2->CR1 |= 0x0004; //.3λ  ���н���
			}
		}

		// �����ӱ�־����·��ת������
		// ZCL 2019.4.9 �ĳ�ע��ɹ��ķ���
		else if (CGD0_LinkConnectOK[SendDataReturnNo])
		// else if (ZhuCeOkFLAG[SendDataReturnNo])
		{
			// 1. AT����ָ��		�ȷ�ָ��.ͨ����.����
			if (S_GprsDataSend == 0 && Cw_Txd3Max == 0)
			{
				S_GprsDataSend = 1;
				C_GprsDataSend = 0;
				C_BetweenSendDataReturn = 0;
			}

			else if (S_GprsDataSend == 1 && Cw_Txd3Max == 0 && C_BetweenSendDataReturn > 3)
			{
				S_GprsDataSend = 2;
				C_BetweenSendDataReturn = 0;
				// 2012.10.4 ����DDPЭ���͸��Э���ѡ��
				// Cw_Txd3Max=0;		//�ж���������Cw_Txd3Max==0�����Դ˾�ע�͵�

				// ZCL 2019.3.14 Cw_BakRcv2 ���� Cw_Txd3TmpMax
				if (GprsPar[EnDDP0Base + SendDataReturnNo]) //=1��DDPЭ�飻=0��͸��Э��
				{
					At_QISENDAccordingLength(SendDataReturnNo, 16 + Cw_Txd3TmpMax); // DDPЭ��	Cw_BakRcv2
																					// F_GPRSSendFinish = 0;
																					// sim7600ceGPRSUpLoaded(Txd3Buffer, 16 + Cw_Txd3TmpMax, 1);
				}
				else
				{
					At_QISENDAccordingLength(SendDataReturnNo, Cw_Txd3TmpMax); // ͸��Э��	Cw_BakRcv2
																			   // F_GPRSSendFinish = 0;
																			   // sim7600ceGPRSUpLoaded(Txd3Buffer, Cw_Txd3TmpMax, 1);
				}
			}

			// 2014.12.11 	��Ϊ�����ղ���AT_3EOk_Flag��־�����Լ�����ʱ���ط�����
			else if (S_GprsDataSend == 2 && C_BetweenSendDataReturn > 300)
			{
				S_GprsDataSend = 1; // ZCL1
				C_BetweenSendDataReturn = 0;
			}

			// 2.���ݷ���
			else if (S_GprsDataSend == 2 && AT_3EOk_Flag) //
			{
				S_GprsDataSend = 3;
				AT_3EOk_Flag = 0;
				Cw_Txd3Max = 0; // 2014.12.8
				// 2012.10.4 ����DDPЭ���͸��Э���ѡ��
				//=1��DDPЭ��
				if (GprsPar[EnDDP0Base + SendDataReturnNo]) //=1��DDPЭ�飻=0��͸��Э��
				{
					Gprs_TX_Fill(GPRS_ZhenTou, 16); // DDPЭ��
					// �޸�DTU���ʶ����
					for (t = 0; t < 11; t++)
						Txd3Buffer[4 + t] = GprsPar[DtuNoBase + t];

					// 2012.10.10 ����TCPЭ���UDPЭ�������
					// UDP: 7B 09 00 10 31 33 39 31 32 33 34 35 36 37 38 7B 61 62 63
					// TCP: 7B 09 00 13 31 33 39 31 32 33 34 35 36 37 38 61 62 63 7B

					// TCP
					if (GprsPar[LinkTCPUDP0Base + SendDataReturnNo]) //=1��TCPЭ�飻=0��UDPЭ��
					{
						Cw_Txd3Max = 15; // �޸�Cw_Txd3Maxֵ��16�ĳ�15�� 2012.10.10

						// ZCL 2019.3.12 �����ƣ�͸����ԭ�ȳ���������GPRSģʽ���³���������ݲ�һ����
						if (F_GprsTransToCom)
						{
							Gprs_TX_Fill(Rcv2Buffer, Cw_BakRcv2); // �������
							Gprs_TX_Byte(0x7B);
							// 2012.10.10 �޸ķ��ͳ��ȣ�ʹ��TCPЭ�飬�򳤶�Ϊ���з��������ܳ��ȣ� �ܳ��ڣ�
							//							Txd3Buffer[3] = 16 + Cw_BakRcv2;
							data_len = 16 + Cw_BakRcv2;
							Txd3Buffer[2] = data_len >> 8;
							Txd3Buffer[3] = data_len; // YLS 2023.10.13 ���һ��BUG��ԭ��ֻȡ�˵��ֽڣ�����TCPģʽ�£����ܲ�ѯ(255-21)=234�ֽڣ�Ҳ���ǲ��ܲ�ѯ����117����
						}

						// ZCL 2019.3.12
						else if (F_GprsMasterNotToCom || F_GprsMasterToCom)
						{
							Gprs_TX_Fill(Txd3TmpBuffer, Cw_Txd3TmpMax); // ������� ZCL 2019.3.12

							Gprs_TX_Byte(0x7B);
							// 2012.10.10 �޸ķ��ͳ��ȣ�ʹ��TCPЭ�飬�򳤶�Ϊ���з��������ܳ��ȣ� �ܳ��ڣ�
							//							Txd3Buffer[3] = 16 + Cw_Txd3TmpMax;
							data_len = 16 + Cw_Txd3TmpMax;
							Txd3Buffer[2] = data_len >> 8;
							Txd3Buffer[3] = data_len; // YLS 2023.10.13 ���һ��BUG��ԭ��ֻȡ�˵��ֽڣ�����TCPģʽ�£����ܲ�ѯ����(255-21)=234�ֽڣ�Ҳ���ǲ��ܲ�ѯ����117����
						}
					}

					// UDPʱ
					else
					{
						// Gprs_TX_Fill(Rcv2Buffer,Cw_BakRcv2);		//�������
						// ZCL 2019.3.12 �����ƣ�͸����ԭ�ȳ���������GPRSģʽ���³���������ݲ�һ����
						if (F_GprsTransToCom)
							Gprs_TX_Fill(Rcv2Buffer, Cw_BakRcv2); // �������

						else if (F_GprsMasterNotToCom || F_GprsMasterToCom)
							Gprs_TX_Fill(Txd3TmpBuffer, Cw_Txd3TmpMax); // ������� ZCL 2019.3.12

						// YLS 2024.03.21 ��Ҫ���͵������ݣ����д������
						//						if (F_GprsTransToCom) //|| F_GprsMasterToCom	YLS 2023.03.28
						//						{
						//							if (Cw_Txd3Max >= 512)
						//								Cw_Txd3Max = 500; //

						//							for (k = 0; k < Cw_Txd3Max; k++)
						//							{
						//								Txd2Buffer[k] = Txd3Buffer[k]; // ����3(GPRSģ��)���͵�����2
						//							}
						//							Cw_Txd2Max = Cw_Txd3Max;
						//							Cw_Txd2 = 0;
						//							RS485_CON = 1; // 2013.9.2
						//							USART_ITConfig(USART2, USART_IT_TC, ENABLE); // ��ʼ����.
						//							USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]);
						//							Delay_MS(1000);	//��ʱ10ms
						//						}
					}
				}
				//=0��͸��Э��
				else // ͸��Э�鲻�ü�ͷ��Ҳ����TCP,UDP����
				{
					// Gprs_TX_Fill(Rcv2Buffer,Cw_BakRcv2);		//����TCP,UDP����

					// ZCL 2019.3.12 �����ƣ�͸����ԭ�ȳ���������GPRSģʽ���³���������ݲ�һ����
					if (F_GprsTransToCom)
						Gprs_TX_Fill(Rcv2Buffer, Cw_BakRcv2); // �������

					else if (F_GprsMasterNotToCom || F_GprsMasterToCom)
						Gprs_TX_Fill(Txd3TmpBuffer, Cw_Txd3TmpMax); // ������� ZCL 2019.3.12
				}

				// 2.���ݷ���
				Gprs_TX_Start();
				// if (GprsPar[ConsoleInfoTypeBase])
				// 	Com2_printf("SEND DATA:%s", Txd3Buffer);
				//
				C_BetweenSendDataReturn = 0;
			}
			// 2014.12.11 	��Ϊ�����ղ���AT_ComOk_Flag��־�����Լ�����ʱ���ط�����
			else if (S_GprsDataSend == 3 && C_BetweenSendDataReturn > 1000)
			{
				S_GprsDataSend = 1; // ZCL2
				C_BetweenSendDataReturn = 0;
			}
			// 3. �յ���ȷOK��־�����ͽ���
			else if (S_GprsDataSend == 3 && AT_CIPSEND_Flag && C_BetweenSendDataReturn > 100) // ������������ʱ������Ҫ YLS 2023.03.07
			{
				//				AT_ComOk_Flag = 0;
				AT_CIPSEND_Flag = 0;
				//
				S_GprsDataSend = 0;
				C_GprsDataSend = 0;
				// SendDataReturnNo++;				//ָ����һ���������	 �Ƶ����� 2014.11.27
				// ������÷�����������־
				C_HeartDelay[SendDataReturnNo] = 0;
				B_HeartSendAsk[SendDataReturnNo] = 0;
				// �Ƶ����� 2014.11.27
				SendDataReturnNo++; // ָ����һ���������
				C_BetweenSendDataReturn = 0;
			}

			// 4. û����ȷ���ͣ���ʱ
			if (C_GprsDataSend > 1) // S	YLS 2023.03.23
			{
				S_GprsDataSend = 0;
				C_GprsDataSend = 0;
				SendDataReturnNo++; // ָ����һ���������
				C_BetweenSendDataReturn = 0;
			}
		}
		// û�����ӵ���·��������ת����һ������
		else
		{
			S_GprsDataSend = 0;
			C_GprsDataSend = 0;
			SendDataReturnNo++; // ָ����һ���������
			C_BetweenSendDataReturn = 0;
		}
	}

	// else if  4.���Լ�����ӱ�Ĵ���
}

void Com3_MasterSend(void) // ����1��������
{
	/* 	if(S_SendLink+S_DtuInquireSend+S_DtuSetSend+S_GprsDataSend
				+S_HeartSend+S_SendSms+S_ReadNewSms+S_SmsFull ==0 )
		{

		} */
}

/*2012.7.10  �ܳ��� GPRS��� */
void Gprs_TX_Byte(u8 i) // ���ͻ�����д��һ���ֽ�
{
	Txd3Buffer[Cw_Txd3Max++] = i;
}

void Gprs_TX_String(u8 *String) // ���ͻ�����д��һ���ַ���
{
	u8 *Data = String;

	while (*Data != 0)
	{
		Txd3Buffer[Cw_Txd3Max++] = *Data;
		Data++;
	}
}

void Gprs_TX_Fill(uc8 *String, u8 Length) // ��䷢�ͻ�����
{
	uc8 *Data = String;

	while (Length--)
	{
		Txd3Buffer[Cw_Txd3Max++] = *Data;
		Data++;
	}
}

void Gprs_TX_Start(void) // ��������
{
	Cw_Txd3 = 0;
	// 2010.7.4 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
	// USART_ITConfig(USART3, USART_IT_TXE, ENABLE);				// ��ʼ����.
	USART_SendData(USART3, Txd3Buffer[Cw_Txd3++]);
	USART_ITConfig(USART3, USART_IT_TC, ENABLE); // ��ʼ����.

	// UART3_Output_Datas(Txd3Buffer, Cw_Txd3Max);
}

uchar Gprs_StrCmdLen(u8 *String) // �����ַ�������
{
	u8 i = 0; //,j
	while (*String++ != 0)
	{
		i++;
		// j=*String;
	}
	return i;
}

/* 1����ת10�����ַ��� */ // ����ֵΪ�ַ�����
uchar Bcd2Str(uint Val)	  // 1����ת10�����ַ���
{
	u8 i = 0;
	// ��ֵת��ASCII��

	if (Val >= 10000)
		goto Wanwei;
	else if (Val >= 1000)
		goto Qianwei;
	else if (Val >= 100)
		goto Baiwei;
	else if (Val >= 10)
		goto Shiwei;
	else
		goto Gewei;
Wanwei:
	StringBuffer[i++] = Val / 10000 + 0x30; // ��λ
Qianwei:
	StringBuffer[i++] = (Val % 10000) / 1000 + 0x30; // ǧλ
Baiwei:
	StringBuffer[i++] = (Val % 1000) / 100 + 0x30; // ��λ
Shiwei:
	StringBuffer[i++] = (Val % 100) / 10 + 0x30; // ʮλ
Gewei:
	StringBuffer[i++] = (Val % 10) + 0x30; // ��λ
	return i;							   // ZCL 2019.4.26 ����ֵ���Ϊ5
}

/* 10�����ַ���ת1���� */ // ����ֵΪת��ֵ���֣�
uint Str2Bcd(u8 *String)  // 10�����ַ���ת1����
{
	uint sum = 0;
	u8 j, k = 0;
	u8 *Data = String;

	// 0x0D:�س���  0x2C:',' 0x2E:'.'  0x3B:';'
	while (*Data != 0x0D && *Data != 0x2C && *Data != 0x2E && *Data != 0x3B)
	{
		j = *Data;
		// ������λ������10���������ﲻ��ָ��������ʵ�֣�Ҳ����ȷ�� 2012.9.21
		if (k == 0)
			sum = j - 0x30;
		else
			sum = sum * 10 + (j - 0x30);
		Data++;
		k++;
	}
	return sum;
}

/* 16�����ַ���ת1���� */ // ����ֵΪת��ֵ���֣�
uint Str2Hex(u8 *String)  // 16�����ַ���ת1����
{
	uint sum = 0;
	u8 j, k = 0;
	u8 *Data = String;

	// 0x0D:�س���  0x2C:',' 0x2E:'.'  0x3B:';'
	while (*Data != 0x0D && *Data != 0x2C && *Data != 0x2E && *Data != 0x3B && *Data != 0x29) // 2013.6.1 �� 0x29:')'
	{
		j = *Data;
		//
		if (j >= 0x41) // A-F(A:0x41)
			j = j - 55;
		else // 0-9
			j = j - 0x30;

		// ������λ������16���������ﲻ��ָ��������Ҳ����ȷ�� 2012.9.21
		if (k == 0)
			sum = j;
		else
			sum = sum * 16 + j;
		Data++;
		k++;
	}
	return sum;
}

// ���ҵ�һ��ֵ����ָ�������У�
uchar FindFirstValue(uc8 *Buffer, u8 FindValue, u8 EndValue) // ���ҵ�һ��ֵ����ָ�������У�
{
	uchar i = 0; //,j
	uc8 *Data = Buffer;

	// j=*Data;
	while (*Data != FindValue) // ȡ��ֵ����Ҫ�ҵ�ֵ
	{
		if (*Data != EndValue) // ȡ��ֵ���ǽ���ֵ
		{
			i++; // ��������1
			Data++;
			// j=*Data;
			if (i == 255) // �������������ǿ���˳�
				break;
		}
		else // ȡ��ֵ�ǽ���ֵ
		{
			i = 255; // ��ֵi=255,Ϊ����ֵ
			break;	 // ǿ���˳�
		}
	}
	return i;
}

// ZCL 2019.4.9 IP�Ͷ˿ں���ȷ�Ϳ��Խ������ӡ� CGD0_LinkConnectOK
// �磺0, CONNECT OK�� �������Ǳ����û���У�Ҳ���Խ������ӡ��������ӵı�־��CGD0_LinkConnectOK
// �������������ж��Ƿ����� ����ͨѶ����ֻ�ǽ������ӣ�ƴͨ�� ����ͨѶ�ı�־��ZhuCeOkFLAG
void Link_Function(void) // ������· UDP,TCP����
{
	uchar i, k;
	uint j;

	//---------------------------------------------A.B.C��������ĵط����뷢�������Ƚϣ�
	// A. ͳ����Ҫ���ӵ�����
	ConnectNum = 0;
	for (i = 0; i < 4; i++)
	{
		if (GprsPar[Ip0Base + i * 4] + GprsPar[DomainName0Base + i * 31] != 0)
			ConnectNum++;
	}

	// B. �ر����� 2012.9.16  2012.10.11�����־����  2012.10.12 �¼�SendSmsLength
	if (S_SendLink + S_DtuInquireSend + S_DtuSetSend + S_GprsDataSend + S_HeartSend + S_SendSms + S_ReadNewSms + S_SmsFull == 0)
	{
		for (j = 0; j < 4; j++)
		{
			if (B_CloseLink[j] == 1) // 2013.6.14 �޸ĳ� B_CloseLink[j]
			{
				At_QICLOSE(j);
				CGD0_LinkConnectOK[j] = 0;
				B_CloseLink[j] = 2;
				Delay_MS(80); // 2013.6.14 ������ʱ 1��Ϳ���
			}
		}
	}

	// C. ��һ�����ӣ���IP��ַ������0��������������0��ͨ���������������־
	for (i = 0; i < 4; i++)
	{
		// 2012.10.13 �����������ó�ASCII 0�����ӣ�����ֻ��һλ������ַ������
		// 2014.12.3 �����4·����ĸΪ@�����ӣ�������Ϊ���õĶ����������ĵ绰���룡
		if (GprsPar[DomainName0Base + i * 31] == 0x30 || GprsPar[DomainName0Base + i * 31 + 1] == 0 || (i == 3 && GprsPar[DomainName0Base + i * 31] == '@')) // 2014.12.3 �����4·����ĸΪ@������
			k = 0;
		else
			k = GprsPar[DomainName0Base + i * 31]; // kΪ����

		// IP��ַ������0��������������0��ԭ��û�н��й����ӣ������������ӣ�����������ʱ��������
		if (GprsPar[Ip0Base + i * 4]) //|| k
		{
			if (B_FirstLink[i] == 0) //!!! �ӿ����ӵĹ��� 2014.12.3��ע
			{
				B_LinkAsk[i] = 1;
				B_FirstLink[i] = 1;
			}
		}
		// û�����ӵ�ַ�������һ�����ӱ�־
		else
			B_FirstLink[i] = 0;
	}
	//---------------------------------------------����A.B.C��������ĵط����뷢�������Ƚϣ�

	// 1. ���ӽ�����ʱ�ж�
	for (i = 0; i < 4; i++)
	{
		if (B_LinkAsk[i] == 0)
		{
			// �Ѿ������� ���� IP��ַ��������ַΪ0�ˣ�������Ҫ�����ˣ� 2014.12.3
			if (CGD0_LinkConnectOK[i] || (GprsPar[Ip0Base + i * 4] + k) == 0)
				C_LinkDelay[i] = 0;
			// ʱ�䵽�������������־B_LinkAsk[i]=1
			else
			{
				// ������ʱ������300S����2���ֽ�
				j = GprsPar[ReConnect0Base + i * 2];
				j = (j << 8) + GprsPar[ReConnect0Base + 1 + i * 2];
				// 2013.6.11 �йر����ӱ�־��������ʱ2��󣬲ſ��������ӡ���Ϊ�˿������ӣ�
				if (B_CloseLink[i] == 2)
				{
					if (C_LinkDelay[i] > 3)
					{
						B_LinkAsk[i] = 1; // ʱ�䵽�������������־
						B_CloseLink[i] = 0;
					}
				}
				// ����ֻ�е�������ʱ������300S����ſ������ӡ�
				else if (C_LinkDelay[i] > j)
				{
					B_LinkAsk[i] = 1; // ʱ�䵽�������������־
				}
			}
		}
		else
			C_LinkDelay[i] = 0;
	}

	// 2. ���ڷ������� ��ÿ·����ѭ�����ͣ��з��������־�ͷ������ӣ���
	if (CGD0_ConnectNo > 3)
		CGD0_ConnectNo = 0;
	i = CGD0_ConnectNo;

	if (B_LinkAsk[i] == 0)
	{
		CGD0_ConnectNo++; // ָ����һ·���ӵķ���
	}
	// �����������ȷ��������� ������
	else if (S_LinkNow[i] == 0 && B_HeartSendAsk[0] + B_HeartSendAsk[1] + B_HeartSendAsk[2] + B_HeartSendAsk[3] > 0)
		;
	//
	else
	{
		// ָ�������
		// ���Ըĳ�==100������������������������
		if (S_HeartSend + S_DtuInquireSend + S_DtuSetSend + S_GprsDataSend + S_SendSms + S_ReadNewSms + S_SmsFull == 0) //=0  =100����ʹ�ã��ݲ�����
		{
			if (S_LinkNow[i] == 0)
			{
				S_LinkNow[i] = 1;
				C_LinkNow[i] = 0;
				/* 				Disable_USART2_NVIC();
								printf("\r\n\r\nSend link %d CMD!", i);
								Enable_USART2_NVIC(); */
			}

			if (S_LinkNow[i] == 1) //&& C_LinkNow[i] > 2  ���ò�������OK  1, CONNECT OK һ���
			{
				S_LinkNow[i] = 2;
				Cw_Txd3Max = 0;	 // ����ɾ 2012.10.4
				At_QIMuxOPEN(i); // ��������ָ��
				C_LinkNow[i] = 0;
			}
			else if (S_LinkNow[i] == 2)
			{
				// 3. �����ӱ�־�����������־
				if (CGD0_LinkConnectOK[i])
				{
					C_LinkDelay[i] = 0;
					C_LinkNow[i] = 0;
					S_LinkNow[i] = 0;
					B_LinkAsk[i] = 0;
					B_LinkFault[i] = 0;
					// ��һ�������ϣ������������������־  !!!
					B_HeartSendAsk[i] = 1;
				}
			}
		}

		// 4. ��ʱ��û�����ӳɹ������
		if (S_LinkNow[i] > 0)
		{
			if (C_LinkNow[i] > 5)
			{
				C_LinkDelay[i] = 0; //=0�����Ƶ�ǰ�棬�����и������ֵ 2013.6.28
				//
				if (B_LinkFault[i] < 3) // ǰ4��û�гɹ������������� 2013.6.23
				{
					if (ConnectNum <= 1) // ֻ��1·�������ÿ���ֱ���ٷ��������� 2013.6.28
						B_LinkAsk[i] = 1;
					else // ��2·�������������ã�����ʱ�ٷ���������(����������ʱ��)
					{
						B_LinkAsk[i] = 0;
						//
						Lw_GprsTmp = GprsPar[ReConnect0Base + i * 2];
						Lw_GprsTmp = (Lw_GprsTmp << 8) + GprsPar[ReConnect0Base + 1 + i * 2];
						if (Lw_GprsTmp > 8)
							C_LinkDelay[i] = Lw_GprsTmp - 8;
						else
							C_LinkDelay[i] = Lw_GprsTmp;
					}
				}
				else // ������ϴ���3�Σ������B_LinkAsk[i]��־�����¼�ʱ
					B_LinkAsk[i] = 0;
				//
				C_LinkNow[i] = 0;
				S_LinkNow[i] = 0;
				B_LinkFault[i]++;
			}
		}

		// 5. ��10�ι��ϱ�־���������Ӳ��ɹ�������������ģ�顣
		// ���������ʱ����=0����DTU����ע��ʧ�ܺ������������Ӳ����
		// ���������ʱ����!=0���򲻻�������һֱ��������ʱ����������

		// if(j==0 && B_LinkFault[i]>=10)			//
		if (B_LinkFault[i] >= 10) // 2016.5.24 �޸ģ�ȥ��j==0
		{
			B_SmsOrFaultHardRestart = 15; //=15����Ӳ������; =1����Ӳ������ ZCL 2019.4.19
		}
	}

	// �ϲ�һ��״̬
	S_SendLink = S_LinkNow[0] + S_LinkNow[1] + S_LinkNow[2] + S_LinkNow[3];
}

// ����������ʱ����  ʱ���������ÿһ�����һ��
// 1. UDP����ע���: �ȷ�ָ��.ͨ����.����		At+QISEND=X,Y
//		At_QISENDAccordingLength( i,22 );		//DTU ���ݷ��Ͱ��ճ���
void Gprs_HeartDelaySend(void) // Gprs����������ʱ���͡�2012.8.25
{
	uchar i, j;

	// 1. �����ж� ����ʱ���������� �� ������������ʱ��ʱ��
	for (i = 0; i < 4; i++)
	{
		if (B_HeartSendAsk[i] == 0) // 2. ��������ʱ��ʱ
		{
			// û�����ӱ�־������������
			if (CGD0_LinkConnectOK[i] == 0)
				C_HeartDelay[i] = 0;
			// �����ӱ�־���ж�����ʱ���Ƿ�	2013.6.28
			else
			{
				Lw_GprsTmp = GprsPar[OnlineReportInterval0Base + i * 2]; // ����ʱ�䣬2���ֽ�  40S
				Lw_GprsTmp = (Lw_GprsTmp << 8) + GprsPar[OnlineReportInterval0Base + 1 + i * 2];
				if (C_HeartDelay[i] > Lw_GprsTmp)
				{
					B_HeartSendAsk[i] = 1; // ʱ�䵽���÷��������־
					C_HeartDelay[i] = 0;
				}
			}
		}
		else
			C_HeartDelay[i] = 0;
	}

	// 2. ���ڷ��������� ��ÿ·����ѭ�����ͣ��з��������־�ͷ�������������
	if (SendHeartNo > 3)
		SendHeartNo = 0;
	i = SendHeartNo;

	if (B_HeartSendAsk[i] == 0)
	{
		SendHeartNo++; // ָ����һ·�������ķ���
	}
	// ���ͱ�·������
	else
	{
		// UDP�����Ϸ�������ע�����1�ȷ�ָ��.ͨ����.����		At+QISEND=X,Y  2������
		if (S_SendLink + S_DtuInquireSend + S_DtuSetSend + S_GprsDataSend + S_SendSms + S_ReadNewSms + S_SmsFull == 0)
		{
			if (S_HeartSendNow[i] == 0)
			{
				S_HeartSendNow[i] = 1;
				C_HeartSendNow[i] = 0;
				ZhuCeOkFLAG[i] = 0;
				C_BetweenHeartSendNow = 0; // 2014.12.11
										   /* 				Disable_USART2_NVIC();
														   Com2_printf("\r\n\r\nSend heart %d CMD!", i);
														   Enable_USART2_NVIC();	 */
			}

			// 1. UDP����ע���: �ȷ�ָ��.ͨ����.����		At+QISEND=X,Y
			if (S_HeartSendNow[i] == 1 && C_BetweenHeartSendNow > 3)
			{
				S_HeartSendNow[i] = 2;
				C_BetweenHeartSendNow = 0;
				Cw_Txd3Max = 0; // ����ɾ 2012.10.4

				At_QISENDAccordingLength(i, 22); // DTU ���ݷ��Ͱ��ճ���
			}
			// 1.5��Ϊ�����ղ���AT_3EOk_Flag��־�����Լ�����ʱ���ط�����		2014.12.11
			else if (S_HeartSendNow[i] == 2 && C_BetweenHeartSendNow > 3000) // YLS 2023.03.11
			{
				S_HeartSendNow[i] = 1; // ZCL1 !!! �������1���ط�
				C_BetweenHeartSendNow = 0;
			}
			// 2. ������
			else if (S_HeartSendNow[i] == 2 && AT_3EOk_Flag) //>
			{
				S_HeartSendNow[i] = 3;
				AT_3EOk_Flag = 0;
				Gprs_Send_ZhuCe();
				C_BetweenHeartSendNow = 0;
			}
			// 2.5 ��Ϊ�����ղ���AT_ComOk_Flag��־�����Լ�����ʱ���ط�����			2014.12.11
			else if (S_HeartSendNow[i] == 3 && C_BetweenHeartSendNow > 1000)
			{
				S_HeartSendNow[i] = 1; // ZCL2 !!! �������1���ط�
				C_BetweenHeartSendNow = 0;
			}
			// 3. ���յ�OK��־
			else if (S_HeartSendNow[i] == 3 && AT_CIPSEND_Flag && C_BetweenHeartSendNow > 100) //&& AT_ComOk_Flag	YLS 2023.02.26
			{
				S_HeartSendNow[i] = 4;
				AT_CIPSEND_Flag = 0;
				// AT_ComOk_Flag = 0;
			}
			else if (S_HeartSendNow[i] == 4)
			{
				// 3. ��ע���־�����������־��ע���־���� (���շ�����������Ϣ)
				if (ZhuCeOkFLAG[i])
				{
					C_HeartDelay[i] = 0; // 2013.6.27
					C_HeartSendNow[i] = 0;
					S_HeartSendNow[i] = 0;
					//
					B_HeartSendAsk[i] = 0;
					B_HeartSendFault[i] = 0;
					B_ConnectFail[i] = 0; // ���ӹ�������
					SendHeartNo++;		  // 2013.6.27 ָ����һ��"�����������"
				}
			}
		}

		// 4. ��ʱ��û��ע��ɹ������
		if (S_HeartSendNow[i] > 0)
		{
			if (C_HeartSendNow[i] > 1) // 2013.5.7 5����û�з���ȷ�ϰ������ط�
			{
				C_HeartDelay[i] = 0; //=0�����Ƶ�ǰ�棬�����и������ֵ 2013.6.28
				//
				if (B_HeartSendFault[i] < 2) // ǰ3��û�гɹ���3����ʱ�ط��������� 2013.6.23
				{
					if (ConnectNum <= 1)	   // ֻ��1·�������ÿ���ֱ�ӷ������� 2013.6.28
						B_HeartSendAsk[i] = 1; // 2013.6.28		��1
					else					   // ��2·�������������ã�����ʱ�ٷ�������(����������ʱ��)
					{
						B_HeartSendAsk[i] = 0; // 2013.6.28		��0
						//
						Lw_GprsTmp = GprsPar[OnlineReportInterval0Base + i * 2];
						Lw_GprsTmp = (Lw_GprsTmp << 8) + GprsPar[OnlineReportInterval0Base + 1 + i * 2];
						if (Lw_GprsTmp > 3)
							C_HeartDelay[i] = Lw_GprsTmp - 3; // �������ú�3��󼴿����·���������
						else
							C_HeartDelay[i] = Lw_GprsTmp;
					}
				}
				else					   // ������ϴ���3�Σ������B_HeartSendAsk[i]��־�����¼�ʱ
					B_HeartSendAsk[i] = 0; // 2013.6.28		��0

				C_HeartSendNow[i] = 0;
				S_HeartSendNow[i] = 0;
				B_HeartSendFault[i]++;
				ZhuCeOkFLAG[i] = 0;
				SendHeartNo++; // 2013.6.27
			}
		}

		// 5. ������¼DSC����(5��)���ϱ�־������ע�᲻�ɹ������ر�ͨ�� 2013.5.7
		j = GprsPar[MaxLoginDSCNum0Base + i];
		if (B_HeartSendFault[i] >= j)
		{
			/* 			if(B_ZhuCeOkNum==0)				//û��1·���ӣ�ģ������
						{
							B_SmsOrFaultHardRestart=17;		//=17����Ӳ������; =1����Ӳ������	 ZCL 2019.4.19
						}
						else */
			// ZCL 2017.5.27
			//			At_QICLOSE(i); // ���������ӣ�ֻ�رմ�ͨ�� 2013.5.7 YLS 2023.03.11
			//
			CGD0_LinkConnectOK[i] = 0;

			// ZCL 2017.5.26 ע�͵�
			// B_HeartSendFault[i]=0;		//2013.5.23		ZCL 2019.4.17
		}
	}

	// �ϲ�һ��״̬
	S_HeartSend = S_HeartSendNow[0] + S_HeartSendNow[1] + S_HeartSendNow[2] + S_HeartSendNow[3];
	B_ZhuCeOkNum = ZhuCeOkFLAG[0] + ZhuCeOkFLAG[1] + ZhuCeOkFLAG[2] + ZhuCeOkFLAG[3];
}

// ��������ע������ķ���
void Gprs_Send_ZhuCe(void) // ��������ע������ķ���			//DTU ���ݷ���
{
	uchar i;

	for (i = 0; i < 22; i++)
		Txd3Buffer[i] = GPRS_ZhuCe[i];
	// �޸�DTU���ʶ����
	for (i = 0; i < 11; i++)
		Txd3Buffer[4 + i] = GprsPar[DtuNoBase + i];
	Cw_Txd3Max = 22;
	Txd3Buffer[Cw_Txd3Max++] = 0x0d;
	Gprs_TX_Start();
}

/* void SMS_SendGSM(void)			//���Ͷ���
{
  GSM_CipSend_Flag = FALSE;

  Gprs_TX_String("AT+CMGS=\0");
  Gprs_TX_Byte('"');
  Gprs_TX_String((u8 *)Device_Info.Phone_Num);
  Gprs_TX_Byte('"');
  Gprs_TX_Byte(0x0d);

  while(!GSM_CipSend_Flag);
  Gprs_TX_String((u8 *)Device_Info.SMS_Buf);
  Gprs_TX_Byte(0x1a);
  SendSMS_Flag = FALSE;
} */

// �������úͲ�ѯ + PC����������GPRS����
//+CMGR: "REC UNREAD","+8615610566803","","2012/09/18 22:38:13+32"
// �����Ķ�������    �磺GPR+62;		�磺GPW+26,42;
// GPRSģ����ŷ��أ��磺GPR+62, Parameter: 202.96.134.133, Read OK! ;
// GPRSģ����ŷ��أ��磺GPW+26,42, Set OK! ;
// Com2_RcvProcess()��main()�е���SmsAndPC_SetupInq()
void SmsAndPC_SetupInq(void) // ���ź�PC���úͲ�ѯGPRS����
{
	uchar i, j, k, r, s, err = 0, value;
	uchar r2; // ZCL 2019.4.26 ���Ӹ�r2������r������r==25���
	uint ip[4], w1, n = 0;
	// uchar *ptr;

	// һ������ָ��W ���� w
	if (B_PcSetGprs == 2)							   // 2013.6.5 ����������
		ptr = (u8 *)strstr((char *)Rcv2Buffer, "GPW"); // ͨ��PC���ڵ��������������GPRS����
	else											   // 2013.6.5 ����������
		ptr = (u8 *)strstr(Rcv3Buffer, "GPW");
	Rcv3Buffer[0] = 0x31; // ��Rcv3Buffer[0]��Ϊ'G'���൱������

	if (ptr != NULL && SendSmsLength == 0)
	{
		// 1. �ϳ���������������Ӧ�ַ�λ����������һ��ʹ��
		// ���յ����ַ��� �磺GPW+26,42;		0x32��0x36�ϳ�16����ָ��ֵ0x26
		CommandNo = Str2Hex(ptr + 4);
		if (CommandNo <= 0x63)
			n = FindFirstValue(CmdNo, CommandNo, 0x00); // n:�ҵ�ָ��ֵ��GPRS �����������е�λ��
		i = FindFirstValue(ptr, 0x2C, 0x00);			// 0x2C=,		//���ҵ�һ��','
		r = FindFirstValue(ptr, 0x3B, 0x00);			// 0x3B=;		//���ҽ�����';'
		r2 = r;											// ZCL 2019.4.26 ���Ӹ�r2������r������r==25���

		// 2.����ָ��Ĵ���; ZCL ��� 2012.9.23
		// n==255 û���ҵ������CommandNo>0xA0 ����������ڵ�����
		if (n == 255 || i == 255 || r2 == 255 || CommandNo > 0xA0) // ZCL 2019.4.26 ���Ӹ�r2������r������r==25���
		{
			// д�Ĵ���Ŵ�1��ʼ ������1-9��
			// �ô����־ err=1
			err = 1;
			for (s = 0; s < 16; s++) // ����������Ķ������ݣ�׼�����Ͷ���
				SendSmsBuf[SendSmsLength++] = CommandError[s];
		}
		else
		{
			if (r >= 150)
				r = 149; // ZCL 2019.4.26 �������鳤�ȣ�uchar SendSmsBuf[150];

			// 3.����ָ�� �磺GPW+26,42��û�зֺţ�
			for (s = 0; s < r; s++)
				SendSmsBuf[s] = *(ptr + s);
			// ����','�ţ�SendSmsLength=r+1
			SendSmsBuf[r] = ',';
			SendSmsLength = r + 1;
		}

		// �ܳ��� 2013.2.2 	�д�������
		ptr = ptr + i + 1; // ָ�����ݵĵ�ַ
		value = *ptr;	   // ָ�����ݵ�ֵ
		if (err)
			;
		// 4. ������ſ��ƹ��� 2012.10.12
		// 0x82������  0x91��ģ���ϵ�����
		// 0x92:�豸��ͣ(=0,����; =1,ֹͣ) 0x95:���Ź���λ����

		// 0x82������
		else if (CommandNo == 0x82)
		{
			// GPW+82,1;		ע���յ���1ΪASCII��0x31
			if (value == 0x31)
				B_SmsDtuRestart = 1; // ��DTU������־
		}
		// 0x91��ģ���ϵ�����		//GPW+91,1;
		else if (CommandNo == 0x91)
		{
			// GPW+91,1;		ע���յ���1ΪASCII��0x31
			if (value == 0x31)
				B_SmsOrFaultHardRestart = 1; // ��ģ���ϵ�������־
											 // S_M35 = 0x01;		//ZCL 2019.4.26 ���ԣ�ȡ�����棬��������������������� Ϊʲô��
		}
		// 0x92:�豸��ͣ(=0,����; =1,ֹͣ)  //GPW+92,1;	GPW+92,0;
		else if (CommandNo == 0x92)
		{
			// GPW+92,1;	GPW+92,0;		ע���յ���1ΪASCII��0x31���յ���0ΪASCII��0x30
			if (value == 0x31 || value == 0x30)
			{
				// SmsControlStopValue ��ֵ���������Ϊ1������Ϊ0
				SmsControlStopValue = value - 0x30;
				B_SmsRunStop = 1;
			}
		}
		// 0x95:GP311ģ�鿴�Ź���λ���� 2013.7.3  //GPW+95,1;
		// ZCL 2019.4.26 ���Ź�������ģ�鲻�᷵�ض��ţ���Ϊ�����ˣ���־û����
		else if (CommandNo == 0x95)
		{
			if (value == 0x31)
				B_SmsWatchdogRestart = 1;
		}

		// ZCL 2019.4.26 ���ԣ���㶨���0x89������������Ϊʲô0x91 ���У�
		else if (CommandNo == 0x89)
		{
			if (value == 0x31)
			{
				S_M35 = 0x01;
			}
		}

		// 5. IP��ַ: ���͹�����222.173.103.226 ��Ϊ��'.'��Ҫȥ��
		// 0x20,0x30,0x40,0x50 ��IP��ַ, 0x62��DNS��ַ
		else if (CommandNo == 0x20 || CommandNo == 0x30 || CommandNo == 0x40 || CommandNo == 0x50 || CommandNo == 0x62)
		{
			// ���յ����ַ��� �磺0x32,0x32,0x32=222���ϳ�10����ֵ222(���ڴ��б���Ϊ0xDE)
			for (s = 0; s < 4; s++)
			{
				ip[s] = Str2Bcd(ptr);	  // �ַ���ת10����ֵ
				if (s == 0 && ip[s] == 0) // ���IP����Ϊ��ʱ��ʾ�������� 2013.6.4
				{
					ip[1] = 0;
					ip[2] = 0;
					ip[3] = 0;
					s = 3;
				}
				if (s == 3)
				{
					// 2013.6.11 ԭ�������ӣ��ȹرգ�2�������
					for (j = 0; j < 4; j++)
					{
						if (CGD0_LinkConnectOK[j] && CommandNo == 0x20 + 0x10 * j) //(IP��ַ�޸���)
							B_CloseLink[j] = 1;
					}
					// 2013.6.11 �����¼�
					break; // �����û��'.'��
				}
				// ���ҵ�һ������'.' ����s==3ʱ�������break�˳�ѭ���ˣ�����ִ��������䡣
				i = FindFirstValue(ptr, 0x2E, 0x00); // 0x2E=.
				ptr = ptr + i + 1;
			}

			// �����ȷ����б��棻��������򷵻ش�����Ϣ
			for (s = 0; s < 4; s++)
			{
				if (ip[s] <= 255)
				{
					// n: ָ��ֵ��GPRS �����������е�λ��
					// ��ȷ���棬��IP��ַд��GprsPar������
					GprsPar[CmdPst[n] + s] = ip[s];
				}
				else
				{
					// ������ ��ǣ�err=2
					err = 2;
					for (s = 0; s < 18; s++) // ����������Ķ������ݣ�׼�����Ͷ���
					{
						SendSmsBuf[SendSmsLength++] = ParameterError[s];
					}
					// 2013.1.24 �ܳ��� �����˳�ѭ��
					break;
				}
			}
		}

		// 6. �ַ��������ã�������ֵ�� �磺0x05��CMNET
		// �������:01		���������:05		DTU���ʶ����:06		����:21.31.41.51
		else if (CommandNo == 0x01 || CommandNo == 0x05 || CommandNo == 0x06 || CommandNo == 0x21 || CommandNo == 0x31 || CommandNo == 0x41 || CommandNo == 0x51)
		{
			// r(;λ��)  i(,λ��)
			if (CommandNo == 0x01 && r - i - 1 > 8)
				err = 3;
			if (CommandNo == 0x05 && r - i - 1 > 24)
				err = 3;
			if (CommandNo == 0x06 && r - i - 1 > 11)
				err = 3;
			if (CommandNo == 0x21 && r - i - 1 > 30)
				err = 3;
			if (CommandNo == 0x31 && r - i - 1 > 30)
				err = 3;
			if (CommandNo == 0x41 && r - i - 1 > 30)
				err = 3;
			if (CommandNo == 0x51 && r - i - 1 > 30)
				err = 3;

			if (r < i + 1) // ZCL 2019.4.26 ���ƴ�С����ֹ���
				err = 3;

			if (err == 3) // ������
			{
				// ����������Ķ������ݣ�׼�����Ͷ���
				for (s = 0; s < 31; s++)
				{
					SendSmsBuf[SendSmsLength++] = StringlengthError[s];
				}
			}
			else // û�д���
			{
				for (s = 0; s < (r - i - 1); s++)
				{
					GprsPar[CmdPst[n] + s] = *(ptr + s);
				}
				GprsPar[CmdPst[n] + s] = 0; // 2012.9.20 �ܳ��� �ַ�����ĩβ��0

				// 2013.6.14 DTU���ʶ���롣ԭ�������ӣ��ȹرգ�2�������
				if (CommandNo == 0x06)
				{
					for (j = 0; j < 4; j++)
					{
						if (CGD0_LinkConnectOK[j]) //(DTU���ʶ�����޸���)
							B_CloseLink[j] = 1;	   // �Զ��رո������ӣ�����̫����
												   // S_M35 = 0x01;						//����  2014.12.4ע�͵�
					}
				}
			}
		}

		// 8. ���ڵ����ã�0x0Dָ�������  0x00,0x00,0x25,0x80 = 9600
		//  Ŀǰ����ֻ֧�����һ���ֵĲ��������ã�ֻ֧�ֵ�0-65535
		else if (CommandNo == 0x0D)
		{
			// 8.1 ������
			// ���յ����ַ��� �磺0x39,0x36,0x30,0x30=9600���ϳ�10����ֵ(���ڴ��б���Ϊ0x25,0x80)
			// ������ֵ����Ҫ����,�ϲ���������
			w1 = Str2Bcd(ptr); // �ַ���ת10����ֵ
			if (w1 == 1200 || w1 == 2400 || w1 == 4800 || w1 == 9600 || w1 == 19200 || w1 == 38400 || w1 == 57600)
			{
				GprsPar[CmdPst[n] + 2] = w1 >> 8; // ���ֽ�
				GprsPar[CmdPst[n] + 3] = w1;	  // ���ֽ�
												  // ��Com1_BaudRateSet()���޸�Pw_BaudRate1
			}
			else if (w1 == 0) // �ܳ��� 2013.12.18 ���ֵΪ�գ�ά��ԭֵ
			{
				;
			}
			else
			{
				// ��������Ĵ���
				err = 4;
				// ����������Ķ������ݣ�׼�����Ͷ���
				for (s = 0; s < 21; s++)
				{
					SendSmsBuf[SendSmsLength++] = BaudRateError[s];
				}
			}

			// 8.2 ����λ ����λΪ8λ��00-5��01-6��02-7��03-8��	������ֵҪ5,6,7,8Ҫ����� 0,1,2,3
			// ʵ����ʾֵ��8������ֻ������03��
			// ������һ��',' [0x2C:',']
			i = FindFirstValue(ptr, 0x2C, 0x00); // 0x2C=,
			if (i == 255)
			{
				w1 = 255;
				goto SMS_SetBaudItemLack_END; // 2013.12.18
			}
			else
			{
				ptr = ptr + i + 1;
				w1 = Str2Bcd(ptr); // �ַ���ת10����ֵ
			}

			if (w1 == 8)
			{
				GprsPar[CmdPst[n] + 4] = w1 - 5;
			}
			else if (w1 == 0) // �ܳ��� 2013.12.18 ���ֵΪ�գ�ά��ԭֵ
			{
				;
			}
			else
			{
				// ��������Ĵ���
				err = 5;
				for (s = 0; s < 26; s++)
				{
					SendSmsBuf[SendSmsLength++] = DataBitLengthError[s];
				}
			}

			// 8.3 ֹͣλ ֹͣλΪ1λ��00-1��04-2��	������ֵҪ1,2Ҫ����� 0,0x04
			// ʵ����ʾֵ��1��2�������������00��04��
			// ������һ��',' [0x2C:',']
			i = FindFirstValue(ptr, 0x2C, 0x00); // 0x2C=,
			if (i == 255)
			{
				w1 = 255;
				goto SMS_SetBaudItemLack_END; // 2013.12.18
			}
			else
			{
				ptr = ptr + i + 1;
				w1 = Str2Bcd(ptr); // �ַ���ת10����ֵ
			}
			if (w1 == 1 || w1 == 2)
			{
				GprsPar[CmdPst[n] + 5] = (w1 - 1) * 4;
			}
			else if (w1 == 0) // �ܳ��� 2013.12.18 ���ֵΪ�գ�ά��ԭֵ
			{
				;
			}
			else
			{
				// ��������Ĵ���
				err = 6;
				for (s = 0; s < 26; s++)
				{
					SendSmsBuf[SendSmsLength++] = StopBitLengthError[s];
				}
			}

			// 8.4 ��żУ�� У��λ-�ޣ�00-��0��08-��1��18-ż2��	������ֵҪ0,1,2Ҫ����� 0��0x08��0x18
			// ʵ����ʾֵ��0��1��2�������������0x00��0x08��0x18��
			// ������һ��',' [0x2C:',']
			i = FindFirstValue(ptr, 0x2C, 0x00); // 0x2C=,
			if (i == 255)
			{
				w1 = 255;
				goto SMS_SetBaudItemLack_END; // 2013.12.18
			}
			else
			{
				ptr = ptr + i + 1;
				if (*ptr != 0x2C)	   // ��һ��λ�ò��ǡ�,���ţ�����ȡֵ������ֱ�Ӹ�w1=100������������
					w1 = Str2Bcd(ptr); // �ַ���ת10����ֵ
				else
					w1 = 100; // ���涼����0����û�иı䣬����0�����õĲ���ֵ�����Ի���100�ˡ�
			}

			if (w1 <= 2)
			{
				if (w1 == 0)
					GprsPar[CmdPst[n] + 6] = 0;
				else if (w1 == 1)
					GprsPar[CmdPst[n] + 6] = 0x08;
				else if (w1 == 2)
					GprsPar[CmdPst[n] + 6] = 0x18;
			}
			else if (w1 == 100) // �ܳ��� 2013.12.19 ���=100��ά��ԭֵ
			{
				;
			}
			else
			{
				// ��������Ĵ���
				err = 7;
				for (s = 0; s < 28; s++)
				{
					SendSmsBuf[SendSmsLength++] = ParityBitLengthError[s];
				}
			}

			// 8.5 ����-�����أ�01-Xon/Xoff1��02-Ӳ����2��03-������3��04-��˫��485 4��05-ȫ˫��422 5��
			// ʵ����ʾֵ��2��3�������������0x02��0x03��
			// ������ֵ����Ҫ���㡣			//������ֻ������02��03��
			// ������һ��',' [0x2C:',']
			i = FindFirstValue(ptr, 0x2C, 0x00); // 0x2C=,
			if (i == 255)
			{
				w1 = 255;
				goto SMS_SetBaudItemLack_END; // 2013.12.18
			}
			else
			{
				ptr = ptr + i + 1;
				w1 = Str2Bcd(ptr); // �ַ���ת10����ֵ
			}
			if (w1 == 2 || w1 == 3)
			{
				GprsPar[CmdPst[n] + 7] = w1;
			}
			else if (w1 == 0) // �ܳ��� 2013.12.18 ���ֵΪ�գ�ά��ԭֵ
			{
				;
			}
			else
			{
				// ��������Ĵ���
				err = 8;
				for (s = 0; s < 24; s++)
				{
					SendSmsBuf[SendSmsLength++] = FlowControlError[s];
				}
			}

		SMS_SetBaudItemLack_END: // ���ò�������������ȱ�ٴ���GPW+0D,9600,8,1,2,3;
			if (w1 == 255 && err == 0)
			{
				// ��������Ĵ���
				err = 9;
				for (s = 0; s < 30; s++)
				{
					SendSmsBuf[SendSmsLength++] = SerialPortItemLackError[s];
				}
			}
		}

		// 9. ˫�ֽڲ����͵��ֽڲ�������ֵ������ 0-65535��Χ��
		else
		{
			// ����ָ���ǲ���˫�ֽڲ���
			s = FindFirstValue(GprsCmd2Byte, CommandNo, 0x00);
			// ������ֵ
			w1 = Str2Bcd(ptr); // �ַ���ת10����ֵ
			// ˫�ֽڲ���
			if (s != 255) //
			{
				GprsPar[CmdPst[n] + 0] = w1 >> 8; // ��������ֵ
				GprsPar[CmdPst[n] + 1] = w1;	  // ��������ֵ
				// �˿��޸��ˣ�ԭ�������ӣ��ȹرգ�2���������2013.6.11 22��32��42��52�ֱ����1,2,3,4ͨ��DSC�Ķ˿ں�
				for (j = 0; j < 4; j++)
				{
					if (CGD0_LinkConnectOK[j] && CommandNo == 0x22 + 0x10 * j) //()
						B_CloseLink[j] = 1;
				}
				// 2013.6.11 �����¼�
			}
			// ���ֽڲ���
			else
			{
				GprsPar[CmdPst[n] + 0] = w1; // ��������ֵ
				// ���� TCP,UDPЭ���޸ģ�ԭ�������ӣ�Ӳ������ 2013.6.21 24��34��44��54�ֱ����1,2,3,4ͨ����������ʽ��0-UDP��1-TCP��
				for (j = 0; j < 4; j++)
				{
					if (CGD0_LinkConnectOK[j] && CommandNo == 0x24 + 0x10 * j) //()
					{
						B_SmsOrFaultHardRestart = 16; //=16����Ӳ������; =1����Ӳ������ ZCL 2019.4.19
					}
				}
				// 2013.6.19 �����¼�
			}
		}

		// 10. ���ŷ���
		// �д�������
		if (err)
			;
		// ��ȷ�Ļ�����䷵�ض������ݣ���������������
		else
		{
			if (B_SmsDtuRestart) // ����DTU����
			{
				for (s = 0; s < 12; s++) // ��䷵�ض�������
					SendSmsBuf[SendSmsLength++] = DtuRestartOK[s];
			}
			else if (B_SmsOrFaultHardRestart == 1 || B_SmsOrFaultHardRestart == 11 || B_SmsOrFaultHardRestart == 12 // ZCL 2019.4.23
					 || B_SmsOrFaultHardRestart == 13 || B_SmsOrFaultHardRestart == 14								// ZCL 2019.4.23
					 || B_SmsOrFaultHardRestart == 15 || B_SmsOrFaultHardRestart == 16								// ZCL 2019.4.23
					 )																								// ���Ż��߹���Ӳ������
			{
				for (s = 0; s < 16; s++) // ��䷵�ض�������
					SendSmsBuf[SendSmsLength++] = DtuHardRestartOK[s];
			}
			else if (B_SmsWatchdogRestart == 1) // ���ſ��Ź�Ӳ������  2014.12.8 //0x95:GP311ģ�鿴�Ź���λ���� 2013.7.3  //GPW+95,1;
			{
				for (s = 0; s < 20; s++) // ��䷵�ض�������
					SendSmsBuf[SendSmsLength++] = SmsWatchdogRestart[s];
			}

			else if (B_SmsRunStop) // ���ſ���SM510�豸��ͣ
			{
				if (SmsControlStopValue) //=1,ͣ��
				{
					for (s = 0; s < 16; s++) // ��䷵�ض�������
						SendSmsBuf[SendSmsLength++] = SmsControlStopOK[s];
				}
				else //=0,����
				{
					for (s = 0; s < 15; s++) // ��䷵�ض�������
						SendSmsBuf[SendSmsLength++] = SmsControlRunOK[s];
				}
			}

			// �������OK�Ķ������ݡ�Sms_Function()�з��Ͷ��š�
			for (s = 0; s < 9; s++)
				SendSmsBuf[SendSmsLength++] = SetOK[s];

			// ���ſ��ƹ��� ���ñ���
			if (B_SmsDtuRestart + B_SmsOrFaultHardRestart + B_SmsRunStop + B_SmsWatchdogRestart > 0) // ���ڿ��ƹ��ܣ�������
				B_SmsSetup = 0;																		 // �����趨������־��=1������Զ����趨�Ĳ������б��棩
			else
				B_SmsSetup = 1; // �����趨������־��=1������Զ����趨�Ĳ������б��棩
		}
		// ����ĩβ�� ; 0x1A   0x0d
		SendSmsBuf[SendSmsLength++] = ';';
		SendSmsBuf[SendSmsLength++] = 0x1A; // CTRL+Z����(0x1A)
		SendSmsBuf[SendSmsLength++] = 0x0d;
	}

	// ������ѯָ�� 2012.9.22
	if (B_PcSetGprs == 2)							   // 2013.6.5 ����������
		ptr = (u8 *)strstr((char *)Rcv2Buffer, "GPR"); // ͨ��PC���ڵ��������������GPRS����
	else											   // 2013.6.5 ����������
		ptr = (u8 *)strstr(Rcv3Buffer, "GPR");
	Rcv3Buffer[0] = 0x31; // ��Rcv3Buffer[0]��Ϊ'G'���൱������

	if (ptr != NULL && SendSmsLength == 0)
	{
		// 1. �ϳ���������������Ӧ�ַ�λ����������һ��ʹ��
		// ���յ����ַ��� �磺GPR+62;  0x36��0x32�ϳ�16����ָ��ֵ0x62
		CommandNo = Str2Hex(ptr + 4);
		// n:�ҵ�ָ��ֵ��GPRS �����������е�λ��
		n = FindFirstValue(CmdNo, CommandNo, 0x00);
		// ���ҽ�����';' ��position
		r = FindFirstValue(ptr, 0x3B, 0x00); // 0x3B=;
		r2 = r;								 // ZCL 2019.4.26 ���Ӹ�r2������r������r==25���

		// 2. ����ָ��Ĵ���;
		// �ܳ��� 2013.2.2 ���r2==255���жϣ��������дָ��û��';'�������Ļ�����
		if (n == 255 || r2 == 255 || CommandNo > 0xA0) // ZCL 2019.4.26 ���Ӹ�r2������r������r==25���
		{
			// ���Ĵ���Ŵ�11��ʼ
			err = 11;
			for (s = 0; s < 16; s++)
			{
				SendSmsBuf[SendSmsLength++] = CommandError[s];
			}
		}
		else
		{
			if (r >= 150)
				r = 149; // ZCL 2019.4.26 �������鳤�ȣ�uchar SendSmsBuf[150];

			// 3. ����ָ�� �磺GPR+43��û�зֺţ�
			for (s = 0; s < r; s++)
			{
				SendSmsBuf[s] = *(ptr + s);
			}
			SendSmsBuf[r] = ',';
			SendSmsLength = r + 1;

			// ���ŷ��أ����� "Parameter:"��ʾ 			2012.10.12
			for (s = 0; s < 12; s++)
			{
				SendSmsBuf[SendSmsLength++] = Parameter[s];
			}
		}

		// �ܳ��� 2013.2.2 	�д�������
		if (err)
			;
		// ���Ŷ�ָ����ȷ
		// 5. IP��ַ: ���͹���Ϊ222.173.103.226 ��Ϊ��'.'��Ҫȥ��
		// 0x20,0x30,0x40,0x50 ��IP��ַ, 0x62��DNS��ַ
		else if (CommandNo == 0x20 || CommandNo == 0x30 || CommandNo == 0x40 || CommandNo == 0x50 || CommandNo == 0x62)
		{
			// ���IPֵ
			for (s = 0; s < 4; s++)
			{
				// 1����ת10�����ַ��� ��:0xDE ת�� 222
				j = Bcd2Str(GprsPar[CmdPst[n] + s]);
				// ������ݵ����ŷ��ػ�����
				for (k = 0; k < j; k++) // ZCL 2019.4.26 ����ֵj���Ϊ5
				{
					SendSmsBuf[SendSmsLength++] = StringBuffer[k];
				}
				// ��ǰ3����ֵ���.  �磺222.173.103.226
				if (s < 3)
					SendSmsBuf[SendSmsLength++] = '.';
			}
		}

		// 6. �ַ����Ĳ�ѯ��������ֵ �磺0x05��CMNET
		// �������:01		���������:05		DTU���ʶ����:06		����:21.31.41.51
		else if (CommandNo == 0x01 || CommandNo == 0x05 || CommandNo == 0x06 || CommandNo == 0x21 || CommandNo == 0x31 || CommandNo == 0x41 || CommandNo == 0x51)
		{
			s = 0;
			// ������ݵ����ŷ��ػ�����
			while (GprsPar[CmdPst[n] + s])
				SendSmsBuf[SendSmsLength++] = GprsPar[CmdPst[n] + s++];
		}

		// 7. �汾�ŵȵĲ�ѯ��������ֵ �磺DTU����汾:70				1.00.00
		else if (CommandNo == 0x70)
		{
			s = 0;
			while (DtuProgVersion[s])
				SendSmsBuf[SendSmsLength++] = DtuProgVersion[s++];
		}
		else if (CommandNo == 0x71)
		{
			s = 0;
			while (DtuProgMakeDate[s])
				SendSmsBuf[SendSmsLength++] = DtuProgMakeDate[s++];
		}
		else if (CommandNo == 0x73)
		{
			s = 0;
			while (DtuHardWare[s])
				SendSmsBuf[SendSmsLength++] = DtuHardWare[s++];
		}
		else if (CommandNo == 0x74)
		{
			s = 0;
			while (DtuManufacture[s])
				SendSmsBuf[SendSmsLength++] = DtuManufacture[s++];
		}
		else if (CommandNo == 0x75)
		{
			s = 0;
			while (DtuModel[s])
				SendSmsBuf[SendSmsLength++] = DtuModel[s++];
		}

		// 8. ���ڵĲ�ѯ��0x0Dָ�������  0x00,0x00,0x25,0x80 = 9600
		// Ŀǰ����ֻ֧�����һ���ֵĲ��������ã�ֻ֧�ֵ�0-65535
		else if (CommandNo == 0x0D)
		{
			// 8.1 ������ 0x00,0x00,0x25,0x80 �к�һ����
			w1 = (GprsPar[CmdPst[n] + 2] << 8) + GprsPar[CmdPst[n] + 3];

			j = Bcd2Str(w1);		// ת���ַ���
			for (k = 0; k < j; k++) // ZCL 2019.4.26 ����ֵj���Ϊ5
			{
				SendSmsBuf[SendSmsLength++] = StringBuffer[k];
			}
			SendSmsBuf[SendSmsLength++] = ',';
			// 8.2 ����λ Ϊ8λ��00-5�� 01-6�� 02-7�� 03-8��	�����ֵΪ0,1,2,3��Ҫ����� 5,6,7,8����ת���ַ�
			w1 = GprsPar[CmdPst[n] + 4];
			if (w1 <= 3)
				SendSmsBuf[SendSmsLength++] = w1 + 5 + 0x30; // ���㲢ת���ַ�
			else
				SendSmsBuf[SendSmsLength++] = '!';
			SendSmsBuf[SendSmsLength++] = ',';
			// 8.3 ֹͣλ ֹͣλΪ1λ��00-1��04-2��	�����ֵ0,4Ҫ����� 1,2����ת���ַ�
			w1 = GprsPar[CmdPst[n] + 5];
			if (w1 == 0 || w1 == 4)
				SendSmsBuf[SendSmsLength++] = w1 / 4 + 1 + 0x30; // ���㲢ת���ַ�
			else
				SendSmsBuf[SendSmsLength++] = '!';
			SendSmsBuf[SendSmsLength++] = ',';
			// 8.4 ��żУ�� У��λ-�ޣ�00-��0��08-��1��18-ż2��	�����ֵ0x00��0x08��0x18Ҫ����� 0��1��2����ת���ַ�
			w1 = GprsPar[CmdPst[n] + 6];
			if (w1 == 0)
				SendSmsBuf[SendSmsLength++] = 0x30; // ���㲢ת���ַ�
			else if (w1 == 0x08)
				SendSmsBuf[SendSmsLength++] = 0x31; // ���㲢ת���ַ�
			else if (w1 == 0x18)
				SendSmsBuf[SendSmsLength++] = 0x32; // ���㲢ת���ַ�
			else
				SendSmsBuf[SendSmsLength++] = '!';
			SendSmsBuf[SendSmsLength++] = ',';
			// 8.5 ����-�����أ�01-Xon/Xoff1��02-Ӳ����2��03-������3��04-��˫��485 4��05-ȫ˫��422 5������ת���ַ�
			w1 = GprsPar[CmdPst[n] + 7];
			if (w1 >= 1 && w1 <= 5)
				SendSmsBuf[SendSmsLength++] = w1 + 0x30; // ���㲢ת���ַ�
			else
				SendSmsBuf[SendSmsLength++] = '!';
		}

		// 9. ˫�ֽڲ����͵��ֽڲ�������ֵ�Ĳ�ѯ 0-65535��Χ��
		else
		{
			// ����ָ���ǲ���˫�ֽڲ���
			s = FindFirstValue(GprsCmd2Byte, CommandNo, 0x00);
			if (s != 255) // ˫�ֽڲ���		//s>0 && 		2013.6.4�����Ӧ��Ҫ
			{
				w1 = (GprsPar[CmdPst[n]] << 8) + GprsPar[CmdPst[n] + 1];

				j = Bcd2Str(w1);
				for (k = 0; k < j; k++) // ZCL 2019.4.26 ����ֵj���Ϊ5
				{
					SendSmsBuf[SendSmsLength++] = StringBuffer[k];
				}
			}
			else // ���ֽڲ���
			{
				w1 = GprsPar[CmdPst[n]];

				j = Bcd2Str(w1);
				for (k = 0; k < j; k++) // ZCL 2019.4.26 ����ֵj���Ϊ5
				{
					SendSmsBuf[SendSmsLength++] = StringBuffer[k];
				}
			}
		}

		// 10. ���ŷ���
		// �д�������
		if (err)
			;
		// ���ò����ɹ�
		else
		{
			SendSmsBuf[SendSmsLength++] = ',';
			for (s = 0; s < 10; s++)
			{
				SendSmsBuf[SendSmsLength++] = ReadOK[s];
			}
		}
		// ����ĩβ�� ; 0x1A   0x0d
		SendSmsBuf[SendSmsLength++] = ';';
		SendSmsBuf[SendSmsLength++] = 0x1A; // CTRL+Z����(0x1A)
		SendSmsBuf[SendSmsLength++] = 0x0d;
	}

	// 11.��PC����GPRS PAR������� 2013.6.6
	if (B_PcSetGprs == 2 && SendSmsLength > 0)
	{
		B_PcSetGprs = 1;					//=1����ʾ��PC����GPRS PAR״̬������û���յ�����ʱ��=2�յ�����
		Cw_Txd2Max = 0;						// 2014.12.3 Ϊ�˱���
		for (s = 0; s < SendSmsLength; s++) // ZCL 2019.4.26 TXD2_MAXΪ512�����ȹ���
			Txd2Buffer[Cw_Txd2Max++] = SendSmsBuf[s];
		SendSmsLength = 0;
		Cw_Txd2 = 0;
		RS485_CON = 1; // 2013.9.2
		// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
		// USART_ITConfig(USART2, USART_IT_TXE, ENABLE);				// ��ʼ����.
		USART_ITConfig(USART2, USART_IT_TC, ENABLE); // ��ʼ����.
		USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]);
	}
}

// ���Ź��ܺ���		M35_LOOP()�е���
void Sms_Function(void) // ����ָ�������Ӳ�����������Ź���λ�������豸��ͣ�����ŷ���-���з��Ͷ��ţ������ţ�������-ɾ������
{
	uchar s;
	uint i;
	// char ComeSmsPhone[11]="15610566803";				//�����ŵĵ绰����

	// 1. ���ſ���DTU�����ɹ����Ͷ��ŷ���
	// 2. ���ſ���DTUӲ�������ɹ����Ͷ��ŷ���
	// 2.5 ���ſ��Ź���λ���� 2014.12.8
	// 3. ���ſ����豸��ͣ��ͨ������2��SM510������ָͣ��������ŷ���ִ�н��
	//--------------------------------------------------------------
	// 4. ���Ų�ѯ�����á�����ָ��� ���ŷ��أ� 2012.9.23  �磺AT+CMGS="13001697270"
	// 5. ����ָ�ȥ�����ţ����յ��¶�����ʾ��
	// 6. ��������ɾ��ȫ������  2012.9.24
	//--------------------------------------------------------------
	// 7. �����ʱ��û���յ���ͣ���أ��ٴη�����ָͣ��

	// ZCL 2017.5.26  1.  2.  2.5 �������Ƴ�
	// 2012.10.12 ������ſ��ƹ���  �������������Ž������������ſ����豸��ͣ
	// 1. ���ſ���DTU�����ɹ����Ͷ��ŷ���
	if (B_SmsDtuRestart == 1) // 0x82������
	{
		B_SmsDtuRestart = 2;
		S_M35 = 0x01;	 // ģ��ػ�������
		B_RestartNo = 2; // ZCL 2019.4.19 ������ţ������ӡ�۲�����
						 // printf("RestartNo=%d\r\n", B_RestartNo);			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
	}
	// ������Ϻ󣬵�S_M35==0x1B������DtuRestartOK������Ϣ
	else if (B_SmsDtuRestart == 2 && S_M35 == 0x1B) //&& C_M35SendSum>5
	{
		B_SmsDtuRestart = 0;
	}

	// 2. ���� ���߹���(2014.12.8) ����DTUӲ�������ɹ����Ͷ��ŷ���
	// ���ſ���DTUӲ������
	if (B_SmsOrFaultHardRestart == 1) // 0x91��ģ���ϵ�����		GPW+91,1;
	{
		B_SmsOrFaultHardRestart = 2;
		S_M35 = 0x01;	 // ģ��ػ�������
		B_RestartNo = 3; // ZCL 2019.4.19 ������ţ������ӡ�۲�����
						 // printf("RestartNo=%d\r\n", B_RestartNo);			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
	}
	// ע������B_DtuHardRestart=3������2����S_M35==0x1B������DtuHardRestartOK������Ϣ
	else if (B_SmsOrFaultHardRestart == 3 && S_M35 == 0x1B)
	{
		B_SmsOrFaultHardRestart = 0;
	}

	// ZCL 2019.4.26	 ���Ͽ���DTUӲ������
	if (B_SmsOrFaultHardRestart >= 11 && B_SmsOrFaultHardRestart <= 16) // ZCL 2019.4.23
	{
		S_M35 = 0x01; // ģ��ػ�������
		// B_RestartNo=4;			//ZCL 2019.4.19 ������ţ������ӡ�۲�����

		B_RestartNo = B_SmsOrFaultHardRestart; // ZCL 2019.4.23
		B_SmsOrFaultHardRestart = 22;		   // 12	ZCL 2019.4.23

		// printf("RestartNo=%d\r\n", B_RestartNo);			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
	}
	else if (B_SmsOrFaultHardRestart == 23 && S_M35 == 0x1B) // 13	ZCL 2019.4.23
	{
		B_SmsOrFaultHardRestart = 0;
	}

	// 2.5 ���ſ��Ź���λ���� 2014.12.8
	if (B_SmsWatchdogRestart == 1) // 0x95:GP311ģ�鿴�Ź���λ���� 2013.7.3  //GPW+95,1;
	{
		B_SmsWatchdogRestart = 2;
		S_M35 = 0x01;	 // ģ��ػ�������
		B_RestartNo = 5; // ZCL 2019.4.19 ������ţ������ӡ�۲�����
						 // printf("RestartNo=%d\r\n", B_RestartNo);			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
						 // ZCL 2019.4.19 Delay_MS(3000);					//��ʱ����û��ι�����豸��λ�� 2013.7.3
	}
	// ������Ϻ󣬵�S_M35==0x1B�����Ͷ�����Ϣ
	else if (B_SmsWatchdogRestart == 2 && S_M35 == 0x1B)
	{
		B_SmsWatchdogRestart = 0;
	}

	// ���ƣ�û�з�����������û�����Ӳ�ѯ�����á�͸��ָ��
	if (S_HeartSend + S_SendLink + S_DtuInquireSend + S_DtuSetSend + S_GprsDataSend == 0)
	{
		// ZCL 2017.5.26  1.  2.  2.5 �Ƴ�������

		// 3. ���ſ����豸��ͣ��ͨ������2��SM510������ָͣ��������ŷ���ִ�н��
		// ֹͣ�豸: 02 06 01 3D 00 01			˵����0x013D: ��SM510�е�317��ַ
		// �����豸: 02 06 01 3D 00 00
		if (B_SmsRunStop == 1) //=1�յ����ſ����豸��ָͣ����ô���2��SM510����ָͣ��
		{
			B_SmsRunStop = 2; // ����һ����ָͣ�����B_SmsRunStop=2��û���յ���ʱ���������յ�=3
			C_SmsRunStop = 0;
			//
			Txd2Buffer[0] = 0x02;				 // �豸�ӵ�ַ
			Txd2Buffer[1] = 0x06;				 // ������
			Txd2Buffer[2] = 0x01;				 // �Ĵ�����ַ
			Txd2Buffer[3] = 0x3D;				 // �Ĵ�����ַ
			Txd2Buffer[4] = 0x00;				 // Ԥ�����ݸ�λ
			Txd2Buffer[5] = SmsControlStopValue; // Ԥ�����ݵ�λ
			i = CRC16(Txd2Buffer, 6);
			Txd2Buffer[6] = i >> 8; // /256
			Txd2Buffer[7] = i;		// ��λ�ֽ�
			// 2013.6.28 ��ֹ������ָͣ��ͨ������2���͸�SM510ʱ��ʧ�����û�з��أ�����10��
			for (i = 0; i < 8; i++)
			{
				BKCom1TxBuf[i] = Txd2Buffer[i];
			}
			B_SendSmsRunStopCmdCount = 0; // ���Ͷ�������ֹͣ���������
			C_SmsRunStop = 0;			  // ��������ֹͣ����������
			//
			Cw_Txd2Max = 8;
			Cw_Txd2 = 0;
			RS485_CON = 1; // 2013.9.2
			// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
			// USART_ITConfig(USART2, USART_IT_TXE, ENABLE);				// ��ʼ����.
			USART_ITConfig(USART2, USART_IT_TC, ENABLE); // ��ʼ����.
			USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]);
		}
		// 2014.12.5�Ƶ���� �����ʱ��û���յ���ͣ���أ��ٴη�����ָͣ��  2013.6.28
		else if (B_SmsRunStop == 2 && C_SmsRunStop > 250) // C_SmsRunStop:10Ms
		{
			if (S_SendSms + S_ReadNewSms + S_SmsFull == 0) //+SendSmsLength
			{
				B_SendSmsRunStopCmdCount++;
				if (B_SendSmsRunStopCmdCount > 10) // ����10�Σ���� 2013.6.30
				{
					B_SendSmsRunStopCmdCount = 0;
					B_SmsRunStop = 0;
					SendSmsLength = 0;
				}
				else
				{
					for (i = 0; i < 8; i++)
					{
						Txd2Buffer[i] = BKCom1TxBuf[i];
					}
					Cw_Txd2Max = 8;
					Cw_Txd2 = 0;
					RS485_CON = 1; // 2013.9.2
					// 2010.8.5 �ܳ��� TXE�ĳ�TC��һ���Ϊ����
					// USART_ITConfig(USART2, USART_IT_TXE, ENABLE);				// ��ʼ����.
					USART_ITConfig(USART2, USART_IT_TC, ENABLE); // ��ʼ����.
					USART_SendData(USART2, Txd2Buffer[Cw_Txd2++]);
				}
			}
			C_SmsRunStop = 0;
		}
		// ͨ������2��SM510������ָͣ��ٽ��յ�SM510���ͻ�����Ӧ���źź�B_SmsControlStop=3
		// Ȼ�󣬶��ŷ���ִ�н��������SmsControlStopOK������Ϣ
		// ����2���ж����� if(Rcv2Buffer[2]==0x01 && Rcv2Buffer[3]==0x3D )
		else if (B_SmsRunStop == 3)
		{
			B_SmsRunStop = 0;
		}

		//--------------------------------------------------------------
		// 4. ���� ��ѯ�����ò���������ָ��Ķ��ŷ��أ����з��Ͷ���
		// ������B_SmsDtuRestart+B_SmsOrFaultHardRestart+B_SmsRunStop==0 ������ƶ��Ѿ�ִ����
		// 2014.12.7 ����B_ZhuDongSendSms(����4��������ǰ���@Ϊ�绰���룬�����ط����š��磺@15610566803)
		// ZCL 2019.4.26 ˵����SendSmsLength>0 ˵�����������Ѿ������ϣ��͵ȸ���־���㣬�Ϳ��Է��Ͷ����ˣ�
		//		���ǲ��ǿ��Ź�����������SendSmsLength�ͱ������ˡ�
		if ((SendSmsLength > 0 || B_ZhuDongSendSms) && S_ReadNewSms + S_SmsFull == 0 && B_SmsDtuRestart + B_SmsOrFaultHardRestart + B_SmsRunStop + B_SmsWatchdogRestart == 0 && B_Com3Cmd8D == 0 && C_PowenOnDelay > 4500) // 2014.12.8 ����ʱ�������
		{
			// ���� AT+CMGS="13001697270"
			if (S_SendSms == 0)
			{
				S_SendSms = 1;
				C_SendSms = 0;
				C_BetweenSendSms = 0;
			}
			if (S_SendSms == 1 && C_BetweenSendSms > 3)
			{
				S_SendSms = 2;
				Cw_Txd3Max = 0;
				//
				Gprs_TX_String("AT+CMGS=");
				Gprs_TX_Byte('"');
				// 2014.12.7 �����������Ͷ��ţ�����õ绰����
				if (B_ZhuDongSendSms)
				{
					for (s = 0; s < 11; s++)
					{
						Txd3Buffer[Cw_Txd3Max++] = GprsPar[DomainName3Base + 1 + s];
					}
				}
				// ���沿��
				else
				{
					for (s = 0; s < 11; s++)
					{
						Txd3Buffer[Cw_Txd3Max++] = ComeSmsPhone[s];
					}
				}
				Gprs_TX_Byte('"');
				Gprs_TX_Byte(0x0d);
				Gprs_TX_Start();
				//
				C_BetweenSendSms = 0;
			}
			// 2014.12.11 	��Ϊ�����ղ���AT_3EOk_Flag��־�����Լ�����ʱ���ط�����
			else if (S_SendSms == 2 && C_BetweenSendSms > 500)
			{
				S_SendSms = 1; // ZCL1
				C_BetweenSendSms = 0;
			}
			else if (S_SendSms == 2 && AT_3EOk_Flag)
			{
				// �յ�'>'�󣬷������� CTRL+Z����(0x1A)
				S_SendSms = 3;
				AT_3EOk_Flag = 0;
				Cw_Txd3Max = 0;
				// 2014.12.7 �����������Ͷ��ţ�����õ绰����
				if (B_ZhuDongSendSms)
				{
					for (s = 0; s < 21; s++)
					{
						Txd3Buffer[Cw_Txd3Max++] = ZhuDongSendSMS[s];
					}
					// 2014.12.9 !!! ����ĩβ�� ; 0x1A   0x0d
					Txd3Buffer[Cw_Txd3Max++] = ';';
					Txd3Buffer[Cw_Txd3Max++] = 0x1A; // CTRL+Z����(0x1A)
					Txd3Buffer[Cw_Txd3Max++] = 0x0d;
				}
				// ���沿��
				else
				{
					for (s = 0; s < SendSmsLength; s++) // ZCL 2019.4.26 TXD3_MAXΪ512�����ȹ���
					{
						Txd3Buffer[Cw_Txd3Max++] = SendSmsBuf[s];
					}
					// 2014.12.9 !!! ����ĩβ�� ; 0x1A  0x0d  (���������úͲ�ѯ�����Ѿ������ʱ����)
				}
				Gprs_TX_Start();
				//
				C_BetweenSendSms = 0;
			}
			// 2014.12.11 	��Ϊ�����ղ���AT_ComOk_Flag��־�����Լ�����ʱ���ط�����
			// ȥ���������ڵ�һ��ʱ�����յ��������š�
			// ԭ�򣺷�����ʱ���յ�OK��ǳ�������Լ4-5�롣�ɴ�ȡ���ˣ�
			/* 			else if(S_SendSms==3 && C_BetweenSendSms>1000)
						{
							S_SendSms=1;		//ZCL2
							C_BetweenSendSms = 0;
						}		 */
			else if (S_SendSms == 3 && AT_ComOk_Flag)
			{
				AT_ComOk_Flag = 0;
				SendSmsLength = 0;
				B_ZhuDongSendSms = 0;
				C_SendSms = 0;
				S_SendSms = 0;
				//
				if (B_SmsSetup)
				{
					B_ForceSavPar = 1; // �Զ����趨�Ĳ��������б���
					SavePar_Prompt();  // ������� 2012.10.11
					//
					B_SmsSetup = 0; // �����趨���� 2013.1.24
				}
			}

			// ���Ͳ��ɹ� =8��ʱ����Է���S_SendSms=1״̬��������
			/* 			if(C_SendSms==8 )
						{
							AT_ComOk_Flag=0;
							//SendSmsLength=0;
							S_SendSms=1;
						}	 */
			// ���ͻ����ɹ� =15��ʱ�򣬶����˳�
			if (C_SendSms > 10)
			{
				AT_ComOk_Flag = 0;
				SendSmsLength = 0;
				B_ZhuDongSendSms = 0;
				C_SendSms = 0;
				S_SendSms = 0;
			}
		}

		// 5. ����ָ�ȥ�����ţ����յ��¶�����ʾ��
		// w_NewSmsNo S_ReadNewSms�ڽ��յ����ź�Com3_RcvProcess()�����㡣
		else if (w_NewSmsNo > 0 && S_SendSms + S_SmsFull == 0) // ���1S�Ż����һ��
		{
			if (S_ReadNewSms == 0)
			{
				S_ReadNewSms = 1;
				C_ReadNewSms = 0;
			}

			// 2014.12.9 �յ�+CMTI(�¶�����ʾ)������ȥ�����ţ����������ܻ����յ��¶���
			//					��ʾ��M35������BUG������ͬһ�����ţ�
			// ����2�����ʱ�� && C_ReadNewSms==2
			if (S_ReadNewSms == 1 && C_ReadNewSms == 2) //!
			{
				S_ReadNewSms = 2;
				At_CMGR(w_NewSmsNo); // ������ָ��
			}
			else if (S_ReadNewSms == 2)
			{
				// �����ɹ� =3,=6��ʱ����Է��� S_ReadNewSms=1״̬������
				if (C_ReadNewSms == 4 || C_ReadNewSms == 8)
				{
					S_ReadNewSms = 1;
				}
				// �������ɹ� =9��ʱ�򣬶����˳�
				else if (C_ReadNewSms > 10)
				{
					w_NewSmsNo = 0;
					S_ReadNewSms = 0;
					C_ReadNewSms = 0;
				}
			}
		}

		// 6. ��������ɾ��ȫ������  2012.9.24
		else if (SmsFull && S_SendSms + S_ReadNewSms == 0)
		{
			if (S_SmsFull == 0)
			{
				S_SmsFull = 1;
				C_SmsFull = 0;
			}

			if (S_SmsFull == 1)
			{
				S_SmsFull = 2;
				At_CMGD('4');
			}
			else if (S_SmsFull == 2)
			{
				if (AT_ComOk_Flag)
				{
					AT_ComOk_Flag = 0;
					SmsFull = 0;
					S_SmsFull = 0;
					C_SmsFull = 0;
				}
			}
			// ɾ�����ɹ� =7��ʱ����Է��� S_SmsFull=1״̬����ɾ��
			if (C_SmsFull == 5)
			{
				AT_ComOk_Flag = 0;
				S_SmsFull = 1;
			}
			// ɾ�������ɹ� =12��ʱ�򣬶����˳�
			else if (C_SmsFull > 10)
			{
				AT_ComOk_Flag = 0;
				SmsFull = 0;
				S_SmsFull = 0;
				C_SmsFull = 0;
			}
		}
	}
}

/*	GPRS��������	 M35��ʼ�����ϵ硢�ϵ� */
void M35_Init(void) // GPRS��������	 M35��ʼ�����ϵ硢�ϵ�
{
	uchar i;
	// PWR_KEY=0;							//ģ�鿪�����߹ػ��� =0���������ܳ����ߣ�ģ�鲻����
	//	DTR=0;									//DTR�ܽ�����20ms��,���ڱ�����

	// S_M35PowerOnOff=0;
	S_M35 = 0x01; // M35��״̬ 2012.7.20
	AT_ComOk_Flag = 0;
	AT_3EOk_Flag = 0;
	AT_CIPSEND_Flag = 0;
	C_M35PowerOnOff = 0;
	C_M35SendSum = 0;
	C_DtuInquireSend = 0; // DTU��ѯָ�������
	C_DtuSetSend = 0;	  // DTU����ָ�������
	C_GprsDataSend = 0;	  // Gprs���ݴ��������
	C_ReadNewSms = 0;	  // ���µĶ��ż�����
	C_SendSms = 0;		  // ���Ͷ���
	C_SmsFull = 0;		  // ��������ɾ������

	S_SendSms = 0;	  // ���Ͷ��Ų���
	S_ReadNewSms = 0; // ���¶��Ų���
	S_SmsFull = 0;	  // ����������
	S_SendLink = 0;	  // �������Ӳ���
	S_HeartSend = 0;
	S_DtuInquireSend = 0;
	S_DtuSetSend = 0;
	S_GprsDataSend = 0;

	CGD0_ConnectNo = 0;
	CGD1_ConnectNo = 0;
	SendDataReturnNo = 0; // �������ݷ������(���ڹ��������ݷ���ÿ������)
	w_NewSmsNo = 0;
	SmsFull = 0; // ��������־
	SendHeartNo = 0;
	ChannelNo = 0;

	B_Com3Cmd8B = 0;  // ��ѯDTU�������� 2012.8.17
	B_Com3Cmd8D = 0;  // ����DTU�������� 2012.8.24
	B_Com3Cmd89 = 0;  // ��������������ָ�� 2011.8.24
	B_DoWithData = 0; // ��������

	B_ATComStatus = 0;
	B_SmsSetup = 0;		  // �����趨����
	B_GprsDataReturn = 0; // GPRS ���ݷ���
	B_NextS35Cmd = 0;	  // ִ����һ��S35����
	B_ZhuCeOkNum = 0;	  // ע��OK������
	B_RunGprsModule = 0;  // �ϵ�û�����ı�ͨѶ��4����ֵ=1
	B_ZhuDongSendSms = 0; // �������Ͷ���(�յ�����4��������@�绰�����) 2014.12.7

	for (i = 0; i < 4; i++)
	{
		C_HeartDelay[i] = 0;
		B_HeartSendAsk[i] = 0;
		B_HeartSendFault[i] = 0;
		S_HeartSendNow[i] = 0;
		C_HeartSendNow[i] = 0;

		C_LinkDelay[i] = 0;
		B_LinkAsk[i] = 0;
		B_LinkFault[i] = 0;
		S_LinkNow[i] = 0;
		C_LinkNow[i] = 0;
		B_FirstLink[i] = 0;
		B_ConnectFail[i] = 0;
		B_RcvFail[i] = 0;

		B_LinkAsk_DomainName[i] = 0;
		S_LinkNow_DoName[i] = 0;
		C_LinkNow_DoName[i] = 0;
		B_FirstLink_DomainName[i] = 0;
	}

	for (i = 0; i < 6; i++)
	{
		CGD0_LinkConnectOK[i] = 0;
		CGD1_LinkConnectOK[i] = 0;
		ZhuCeOkFLAG[i] = 0;
	}

	B_WaitCallReady = 0;
	B_RestartNo = 0; // ZCL 2019.4.19 ������ţ������ӡ�۲�����

	// 2016.7.1 ZCL �򿪽��գ���ֹû�п������жϣ����½��ղ������ݡ�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART2->CR1 |= 0x0004; //.3λ  ���н���
}

/*	PWRKEY����IN������һ�ι涨ʱ�����������߹ػ������������ܺ��ż����á�
		RESET_4G����IN�����͸ý�200ms����������ģ�飻���������ܺ��ż����á�
		STATUS_4G����OUT��ָʾģ�������״̬���ߵ�ƽ��ʾģ�鿪�����͵�ƽ��ʾģ��ػ���
				1.VBAT�ϵ��PWRKEY �ܽſ�������ģ�飬�����ǵ͵�ƽ��������STATUS_4G �ܽ���
		���ߵ�ƽ֮�󣬿����ɹ���PWRKEY �ܽſ����ͷš�����ͨ�����STATUS_4G�ܽŵĵ�ƽ��
		�б�ģ���Ƿ񿪻����Ƽ�ʹ�ÿ���������·������PWRKEY �ܽš�
				2.�����͹ܽ�PWRKEY֮ǰ����֤VBAT��ѹ�ȶ�������VBAT�ϵ絽�ܽ�PWRKEY����
		֮���ʱ����Ϊ30���롣
				3.ע�⣺����ͨ����� STATUS_4G ��ƽ���б�ģ���Ƿ񿪻���STATUS_4G �ܽ�����ߵ�ƽ��
		PWRKEY �����ͷš������ʹ�� STATUS_4G �ܽţ�������  PWRKEY ����26��������ģ�顣
				4.�����ػ�֮��PWRKEY����һ��ʱ���������ģ�顣�ڼ�⵽STATUS_4G�ܽ�Ϊ�͵�ƽ
		֮�󣬵ȴ�����500ms��������ģ�顣
*/
void M35_Power_ONOFF(void) // M35 ��Դ�ϵ�ϵ�
{
	// 2012.10.12 �����������
	if (B_SmsOrFaultHardRestart == 2 || B_SmsOrFaultHardRestart == 22) // ZCL 2019.4.23
	{
		if (S_M35PowerOnOff == 0)
		{
			RESET_4G = 1; // =1���������ܳ�����(>100ms����<500ms���Ƽ�200ms)��ģ������
			S_M35PowerOnOff = 1;
			C_M35PowerOnOff = 0; // YLS 2023.12.06 ԭ��������һ�䣬���¸�λ��ʱʱ��̫�̣�����û�У����¸�λ���ɹ�
		}
		else if (S_M35PowerOnOff == 1 && C_M35PowerOnOff > 20) //(>200ms) �����͵�ƽ��λ�źţ�����200ms������λ���ɹ�
		{
			RESET_4G = 0; // =0 ȡ�������ź�
			S_M35PowerOnOff = 2;
			C_M35PowerOnOff = 0;
		}
		else if (S_M35PowerOnOff == 2 && C_M35PowerOnOff > 210) // �����ػ������ٵȴ�2s��������
		{
			S_M35PowerOnOff = 0;
			C_M35PowerOnOff = 0;
			if (B_SmsOrFaultHardRestart == 2)
				B_SmsOrFaultHardRestart = 3; // �˱�־�ó�3�����水����������
			else if (B_SmsOrFaultHardRestart == 22)
				B_SmsOrFaultHardRestart = 23; // 13	ZCL 2019.4.23
		}
	}

	// �����������ػ�(����)
	else
	{
		// 1. A�ϵ翪��; B�ػ���׼������
		if (S_M35PowerOnOff == 0)
		{
			// A�ϵ翪��
			if (STATUS_4G == 0) // �ж�STATUS_4G==0ԭ��û�п����������ϵ翪��
			{
				PWR_KEY = 1; // ��ʱΪ����	//ģ�鿪�����߹ػ��� =1���������ܳ����ͣ�ģ�鿪����0.5s���ϵĵ͵�ƽ���壬������
				S_M35PowerOnOff = 1;
				C_M35PowerOnOff = 0;
				M35_Init();
			}
			// B�ػ���׼������
			else // �ж�STATUS_4G==1ԭ���Ѿ����������йػ� ����
			{
				PWR_KEY = 1;		  // ��ʱΪ�ػ������������ܳ����ͣ�2.5s��5s�ĵ͵�ƽ���壬�ػ���
				S_M35PowerOnOff = 11; // ģ��ػ�������
				C_M35PowerOnOff = 0;  // ���STATUS_4GΪ�ߵ�ƽ��������״̬����PWR_KEY����һ��2.5s-5s�ĵ͵�ƽ���壬��ʱ26s���ɹػ�
			}
		}
		// 2. �ȴ�STATUS_4G==1�������ɹ�
		// ������STATUS_4Gʱ t > 2000ms��ʹ��STATUS_4Gֻ��Ҫ��ʱ>(1000MS-800MS)���ɣ�
		else if (S_M35PowerOnOff == 1 && STATUS_4G)
		{
			/*2013.10.17 �������PWR_KEY=0ע�͵�����ΪSTATUS_4GΪ�ߵ�ƽ��PWR_KEY����Ҫ��ʱһ���ʱ�䡣
��PWR_KEY=0����ComOK_OverTime:���⵽AT_CallReadyOk_Flag��  */
			// PWR_KEY=0;							//=0���������ܳ����ߣ����������˳�����ʱ��
			//			S_M35PowerOnOff = 2; // �����ɹ�����S_M35PowerOnOff=2������M35_LOOP()���ж���
			//			C_M35PowerOnOff = 0;
			if (C_M35PowerOnOff > 30)
			{
				S_M35PowerOnOff = 2;
				C_M35PowerOnOff = 0;
				PWR_KEY = 0; //=0���������ܳ����ߣ����������˳�����ʱ��
			}
		}

		// �ػ���ʱ���ٷ��ؿ������ȹػ���������
		// 11.�ػ���ʱ
		else if (S_M35PowerOnOff == 11) // ��ʱSTATUS_4G=1
		{
			if (C_M35PowerOnOff > 300) // ����PWRKEY(5S>Pulldown>2.5s)һ��ʱ�䣬ģ��ػ�
			{
				S_M35PowerOnOff = 12;
				C_M35PowerOnOff = 0;
				PWR_KEY = 0; //=0���������ܳ����ߣ����������˳��ػ�ʱ��	(SIM7600_Ӳ������ֲ�_V1.12 P30)
			}
		}
		// 12. �ж��Ƿ��Ѿ��ػ�
		else if (S_M35PowerOnOff == 12)
		{
			// �ڼ�⵽STATUS_4G�ܽ�Ϊ�͵�ƽ֮�󣬵ȴ�����500ms��������ģ�顣(M35_Ӳ������ֲ�_V1.2 P31)
			if (!STATUS_4G) // ����⵽STATUS_4GΪ��ʱ���ػ���ϣ�
			{				// �ȴ�����1000ms��������ģ�� 2013.6.27��
				S_M35PowerOnOff = 13;
				C_M35PowerOnOff = 0;
			}
			else if (C_M35PowerOnOff > 6000) // ��ֹ��ʱ��STATUS_4G=1
			{
				S_M35PowerOnOff = 0; // �ػ�ʱ�����ֵΪ26s���������60s����û�йػ���ϣ��ص�0״̬���¿���
				C_M35PowerOnOff = 0;
			}
		}
		// 13.	�ػ���ϣ���ʱ1000MS�ٷ��أ��������¿�����
		else if (S_M35PowerOnOff == 13)
		{
			if (C_M35PowerOnOff > 100) // ��ʱ>1s
			{
				S_M35PowerOnOff = 0; // �ص�0״̬���¿���
				C_M35PowerOnOff = 0; // ���STATUS_4GΪ�͵�ƽ�����ػ�״̬����PWR_KEY����һ��0.5s�ĵ͵�ƽ���壬��ʱ12s-16s���ɿ���
			}
		}
	}
}

/*	GPRS��������	 M35ѭ��״̬ */
void M35_LOOP(void) // GPRS��������	 M35ѭ��״̬
{
	// uchar i;
	uint w;

	// C_M35Step�ڶ�ʱ�ж��м�1(10MS������),mian()��C_M35Step>=100ִ��M35_LOOP()
	C_M35Step = 0;
	B_NextS35Cmd = 0; // ִ����һ��S35����

	// �ܳ��� 2012.10.15 ����ģ������һ��ʱ���STATUS_4G=0���ػ�����OK�������Դ�������㵼��
	if (STATUS_4G == 0 && S_M35 == 0x1B)
	{
		// S_M35 = 0x01;			//����ʱ�ã�������������ˣ��������С�
		// B_ForceSavPar=1;							//�������
		// Pw_GprsFaultRestart = Pw_GprsFaultRestart + 1; // ��������������1���ۿ���
	}

	// Ϊ���ı���ʾ����GP311������ͨѶ�á�(�ϵ�û�����ı�ͨѶ��4����ֵ=1)
	// ��C_DelayRunGprsModule>4��˵���ı�û�����ݷ��ʣ��˳����ı���������������GPRS
	if (B_RunGprsModule == 0 && C_DelayRunGprsModule > 4)
	{
		B_RunGprsModule = 1; //=1���ſ�����������GPRS����
							 // S_M35 = 0x01;
	}

	// ZCL 2019.4.19 //=5��=6 Ϊ���Ź��������ȴ���ӡ��ϡ�
	// ZCL 2019.4.23 �ս���ʱ����ӡһ�Σ�Ȼ����ʱ�� ������������д�ӡ���ǳ���Ҫ����������������ͻ
	if (B_RestartNo > 0 && S_M35 == 0x01 && Cw_Txd2Max == 0) // Txd1Max ZCL 2019.4.25
	{
		if (T_WatchdogRestartDelay != SClk10Ms)
		{
			T_WatchdogRestartDelay = SClk10Ms;

			// ZCL 2019.4.23 �ս���ʱ����ӡһ�Σ�Ȼ����ʱ�� ������������д�ӡ���ǳ���Ҫ����������������ͻ
			if (C_WatchdogRestartDelay == 0)
				printf("RestartNo=%d\r\n", B_RestartNo); // ZCL 2019.4.19 ������ţ������ӡ�۲�����

			// printf("B_SmsOrFaultHardRestart=%d\r\n", B_SmsOrFaultHardRestart);
			C_WatchdogRestartDelay++;

			if (C_WatchdogRestartDelay > 200)
			{
				C_WatchdogRestartDelay = 0;
				//=5 0x95 ���ſ��Ź������� =6 �ղ���CallReady����
				if (B_RestartNo == 5 || B_RestartNo == 6)
					Delay_MS(3000); // ��ʱ����û��ι�����豸��λ�� 2013.7.3
				//
				B_RestartNo = 0;
			}
		}
	}

	// B_RunGprsModule=0 B_ATComStatus=1 B_PcSetGprs=1 ����M35ѭ����������� ��
	// B_RunGprsModule=0 �ϵ�4����ж��Ƿ����ı�ͨѶ
	// B_ATComStatus=1		AT����״̬
	if (!B_RunGprsModule || B_ATComStatus || B_PcSetGprs || B_RestartNo > 0) //=5��=6 Ϊ���Ź��������ȴ���ӡ��ϡ�ZCL 2019.4.19
		;
	else
		// M35�Ĳ���ѭ��
		switch (S_M35)
		{
		case 0x00:
		case 0x01: // ����.�ػ�.����
			M35_Power_ONOFF();
			if (S_M35PowerOnOff == 2 && STATUS_4G)
			// if (STATUS_4G)
			{
				S_M35PowerOnOff = 0;
				C_M35SendSum = 0;
				S_M35 = 0x03;

				/* Enable USART3 Receive and Transmit interrupts */
				USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // ��ʱ�����գ���ֹ���յ�����

				// 2016.7.1 ZCL �򿪽��գ���ֹû�п������жϣ����½��ղ������ݡ�
				USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
				USART2->CR1 |= 0x0004; //.3λ  ���н���
			}
			w_GprsCSQ = 0;
			break;
		case 0x03:									// �ȴ�ͨ�ųɹ�
			if (!AT_ComOk_Flag && C_M35SendSum > 2) // �ϵ�ɹ�����ʱ����ָ��
				At_AT();
			goto ComOK_OverTime;
		case 0x04: // ����AT�������
			if (!AT_ComOk_Flag)
			{
				// ��������4�ı��ض˿ںŲ��������Դ򿪻��ԣ�=11011ʱ��
				// 2013.9.26 ע����������ʱ�رջ���
				w = GprsPar[LoclaPort3Base]; // ���ܳ��� 2013.9.26
				w = (w << 8) + GprsPar[LoclaPort3Base + 1];
				if (w == 11011)
					At_ATE('1'); // 0�رջ���	1�򿪻���(Ĭ��)
				else
					At_ATE('0'); // 0�رջ���	1�򿪻���(Ĭ��)
			}

			goto ComOK_OverTime;
		case 0x05: // �ȴ�Call Ready�źŵ���		2012.9.12
			/* 								if(B_WaitCallReady==0)			//ZCL 2019.4.23 ɾ��  ������������������ض��ţ���������2��
											{
												if(!AT_ComOk_Flag && C_M35SendSum >1)	//
													At_QIURC();
											} */
			if (!AT_ComOk_Flag && C_M35SendSum > 3) // �ϵ�ɹ�����ʱ����ָ��
				At_AT();
			goto ComOK_OverTime;

		case 0x06: // ����ź�ǿ��  2017.5.25��ӣ�����case ��˳������ (6-E�ĳ�7-F)
				   //+CSQ: 23,0  (һ��15���Ͼͻ�������)
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_CSQ();
			goto ComOK_OverTime;

		case 0x07:									 // �ȴ�ע������		��ѯGSMע��״̬  ����1,5 ��ע��
			if (!AT_ComOk_Flag && C_M35SendSum >= 1) //
				At_CREG();
			goto ComOK_OverTime;
		case 0x08: // ���ò�����
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_AT();
			// 	At_IPR(); // 19200
			goto ComOK_OverTime;

		// ���ų�ʼ��
		case 0x09:									 // ѡ�����Ϣ�洢��
			if (!AT_ComOk_Flag && C_M35SendSum >= 1) //
				At_CPMS();							 // AT+CPMS="SM","SM","SM"  ����:+CPMS: 8,40,8,40,8,40
			goto ComOK_OverTime;
		case 0x0A: // ���ö���Ϣģʽ =0 PDU�� =1 �ı�
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_CMGF('1'); //=1 �ı�
			goto ComOK_OverTime;
		case 0x0B: // ѡ�� TE�ַ���  Ĭ����GSM�ַ������Ժ����UCS2
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_CSCS(); // Ĭ����GSM�ַ���
			goto ComOK_OverTime;
		case 0x0C: // ������ʾ���� AT+CNMI=2,2,0,0,0
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_CNMI();
			goto ComOK_OverTime;
		case 0x0D: // �����ı���ʽ�·��Ͳ���  AT+CSMP=17,169,0,241
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_CSMP();
			goto ComOK_OverTime;
		case 0x0E: // ��ʾ�ı���ʽ���� =0��ʾ���ֲ��� =1��ʾȫ������
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_CSDH(); //=0
			goto ComOK_OverTime;
		case 0x0F: // ȷ�϶���Ϣ�洢��,����ѯ����
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_CPMSInq(); // AT+CPMS?
			goto ComOK_OverTime;

			// GPRS
		case 0x10: // GPRS����
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_CGATT();
			goto ComOK_OverTime;
		case 0x11: // ����ǰ�ó���
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_AT();
			//                if(!AT_ComOk_Flag)
			//									At_QIFGCNT('0');
			goto ComOK_OverTime;
		case 0x12: // ����GPRS���ӷ�ʽ 0:CSD���� 1: GPRS����
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_AT();
			//                if(!AT_ComOk_Flag)
			//									At_QICSGP();			//1:GPRS����
			goto ComOK_OverTime;
		case 0x13: // ���÷�������ַΪIP��ʽ		=0 IP����; =1 ��������
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_AT();
			//                if(!AT_ComOk_Flag)
			//									At_QIDNSIP('1');		//=1 ����ͬʱ����IP������������ 2012.10.4 �ܳ���
			goto ComOK_OverTime;
		case 0x14: // �鿴������ʾ��ʽ 1: ����IP���˿ں�
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_AT();
			//                if(!AT_ComOk_Flag)
			//									At_QISHOWRA('1');
			goto ComOK_OverTime;
		case 0x15: // �鿴������ʾ��ʽ 1: ����IPD<Len>
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_AT();
			//                if(!AT_ComOk_Flag)
			//									At_QIHEAD('1');
			goto ComOK_OverTime;
		case 0x16: // �鿴������ʾ��ʽ 1: ����Э������
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_AT();
			//                if(!AT_ComOk_Flag)
			//									At_QISHOWPT('1');
			goto ComOK_OverTime;
		case 0x17: // 0:��·����   1: ��·����
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_AT();
			//                if(!AT_ComOk_Flag)
			//									At_QIMUX('1');
			goto ComOK_OverTime;
		case 0x18: //=0��͸��  =1ʹ��͸��ģʽ
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_QIMODE('0'); //=0��͸��  =1͸��ģʽ
			goto ComOK_OverTime;

		case 0x19: // �յ��������ݺ�ȴ�200ms���ͣ��򴮿����ݴﵽ512�ֽں�
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_NETOPEN();
			//                if(!AT_ComOk_Flag)
			//									At_QITCFG();
			goto ComOK_OverTime;
		case 0x1A: // �ر�AT�������
			if (!AT_ComOk_Flag && C_M35SendSum >= 1)
				At_ATE('0'); // 0�رջ���
			goto ComOK_OverTime;

		// ������������	S_M35һֱΪ0x1B����������ʱ��������������ȥ�������������Լ���������
		case 0x1B:				 // ���������ȫ��ע�͵������Խ���M35���ڷ�ָ����� 2012.10.4 �ܳ���
			if (B_RunGprsModule) //=1���ſ�����������GPRS����
			{
				Send_DomainName();
				// ���ӹ��� 2012.10.4  ����ʱ���������0�ĳ�100
				Link_Function();

				// ������ʱ+��������ע�����1�ȷ�ָ��.ͨ����.���� At+QISEND=X,Y��2������
				Gprs_HeartDelaySend(); // ����������ʱ���ͺ�����ÿһ�����һ��
				// ���Ź��� 2012.9.19
				Sms_Function();
			}
			break;

		case 0x30: // �رյ�ǰTCP����
			if (!AT_ComOk_Flag)
				At_QICLOSE(0);
			goto ComOK_OverTime;
		case 0x31: // �رյ�ǰ��GPRS������
			if (!AT_ComOk_Flag)
				At_QIDEACT();
			goto ComOK_OverTime;

		case 0x41: // ģ��˯��������
			if (!AT_ComOk_Flag)
				At_QSCLK('1');
			// goto  ComOK_OverTime;

		ComOK_OverTime:
			if (AT_ComOk_Flag)
			{
				/* 									if(S_M35==0x03)				S_M35 = 0x05;
													else  */
				if (S_M35 == 0x0F)
					S_M35 = 0x10; // ZCL 2017.5.25 0x0E�ĳ�0x0F
				else if (S_M35 == 0x41)
					S_M35 = 0x00;
				else
				{
					S_M35++;
					C_PowenOnDelay = 0;
				}

				AT_ComOk_Flag = FALSE;
				B_NextS35Cmd = 1; // ��1��ִ����һ��S35����
				C_M35SendSum = 0; // ��շ�����ʱ������
								  // break;
			}
			//			else if (AT_CallReadyOk_Flag && S_M35 == 5)
			else if ((AT_ComOk_Flag || AT_CallReadyOk_Flag) && S_M35 == 5)
			{
				S_M35 = 0x06;
				AT_ComOk_Flag = 0;
				AT_CallReadyOk_Flag = 0;

				/*2013.10.17 �������PWR_KEY=0 ���ڴ˴�����ΪSTATUS_4GΪ�ߵ�ƽ��
					PWR_KEY����Ҫ��ʱһ���ʱ�䡣
					��PWR_KEY=0����ComOK_OverTime:���⵽AT_CallReadyOk_Flag��  */
				PWR_KEY = 0; //=0���������ܳ����ߣ����������˳�����ʱ��

				// 2015.1.5 ������һ��
				C_M35SendSum = 0; // ��շ�����ʱ������
			}

			// 2015.1.5 �����������Ϊ����3G���ȴ�CallReady�ź���Ҫ������ʱ�䣬16���ˡ�
			// 2019.4.11 ZCL ���淢��GPRSż�����������ǲ��������⣿������Ϊ��S_M35==5���ղ���CallReady������취���ÿ��Ź���ʱ�������ˣ�
			else if (S_M35 == 5)
			{
				if (C_M35SendSum > 95)
				{
					S_M35 = 0x01;	 // ģ��ػ�������
					B_RestartNo = 6; // ZCL 2019.4.19 ������ţ������ӡ�۲�����
									 // printf("RestartNo=%d\r\n", B_RestartNo);			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
									 // ZCL 2019.4.19 Delay_MS(3000);					//���Ź���������ʱ����û��ι�����豸��λ�� 2019.4.11
				}
				// S_M35 = 0x01;			//ZCL123 ���� 2019.4.10
			}

			// 2015.1.17 �����������Ϊ����+CREG: 0,2 17�βų���+CREG: 0,1��ע��ɹ���
			else if (S_M35 == 7) // ZCL 2017.5.25 6�ĳ�7,��Ϊ���case 6Ϊ�ź�ǿ�ȼ�⣻
			{
				if (C_M35SendSum > 95)
				{
					S_M35 = 0x01;
					B_RestartNo = 7; // ZCL 2019.4.19 ������ţ������ӡ�۲�����
									 // printf("RestartNo=%d\r\n", B_RestartNo);			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
				}
			}

			// 2015.1.17  15�θĳ�35�Σ�65��(2019.4.4)����ֹ��Ŀ��ٳ����������⡣
			else if (C_M35SendSum > 65)
			{
				S_M35 = 0x01;
				B_RestartNo = 8; // ZCL 2019.4.19 ������ţ������ӡ�۲�����
								 // printf("RestartNo=%d\r\n", B_RestartNo);			//ZCL 2019.4.19 ������ţ������ӡ�۲�����
			}
			break;

		case 0xFF:				// �ݴ� һ�㲻̫��Ҫ
			At_ThreePlusSign(); // �˳�͸������������ģʽ +++
			At_ATO0();			// �˳������������͸��ģʽ
			At_ATAndF();		// ��������TA����Ϊ��������
			At_ATAndW();		// �����û����ò���
			At_ATV1();			// ����TA��Ӧ�������ݸ�ʽ�� =0�����룻=1��ϸ��(Ĭ��)
			// At_QPOWD();						//! =0�����ػ���=1�����ػ�
			// At_IPR();							//���ò����ʣ���=0��Ϊ����Ӧ������
			At_CFUN();	 // ����ģ�����й��ܿ�����	�ڶ�������=0������λ������Ч��
			At_CPIN();	 // ȷ��PIN���ѽ⡣ZCL:һ�㲻��Ҫ
			At_QINDI();	 // �鿴���ݽ��շ�ʽ1: ��Ҫ������ȡ����
			At_QISACK(); //! ��������Ƿ��ͳɹ�  +QISACK: 4,4,0 ˵�����ͳ��Ⱥ���ȷ��
			At_QISTAT(); // ��ѯ����״̬
			At_ATI();	 // ��ʾ��Ʒ��Ϣ
			At_ATQ0();	 // ���ý������ʾģʽ  0=��TE���ͽ����(Ĭ��)��1=������
			break;

		default:
			return;
		}
}

void Com3_ReceiveData(void)
{
	u8 RecNumBit, IP1NumBit, IP2NumBit, IP3NumBit, IP4NumBit, PortNumBit; // ���յ������ݳ����Ǽ�λ��
	u16 j = 0, n, m, i;
	u8 *p2, *p3;
	// u8 *p1;
	u8 ipbuf[15];			   // IP����
	u8 portBuf[5] = {0};	   // Զ�˶˿ڻ���
	u8 p_RECV_IPBuf[40] = {0}; // ���ջ���

	Fill_data();

	//	p2 = (u8 *)strstr((const char *)Rcv3Buffer, "RECV FROM:"); // ���յ�TCP/UDP����
	p2 = (u8 *)strstr((const char *)Rcv3Buffer, "RECV FROM:"); // ���յ�TCP/UDP����
	if (p2)													   // ���յ�TCP/UDP����
	{
		COM2_DATA = 1; // ���յ����ݿ���ָʾ��

		// ��1·
		if ((Pw_Link1IP1 + Pw_Link1IP2 + Pw_Link1IP3 + Pw_Link1IP4) != 0) // ��һ·��IP��ַ
		{
			sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link1IP1, Pw_Link1IP2, Pw_Link1IP3, Pw_Link1IP4); // ���IP
			sprintf((char *)portBuf, "%d", Pw_Link1Port);											   // ��һ·����Զ�˶˿���ת��Ϊascii��
			sprintf((char *)p_RECV_IPBuf, "%s:%s", ipbuf, portBuf);									   // ��������Ϣ��p_RECV_IPBuf
			if (strstr((const char *)p2, (const char *)p_RECV_IPBuf))								   // �յ��ĵ�1·����������
			{
				p3 = (u8 *)strstr((const char *)p2, "+IPD");
				//					sprintf((char *)p1_TmpBuf, "%s", p3 + 4); //���յ��ĵ�1·���ݳ����ַ� ����������ָ��ָ����ַ��������鴫ֵ����֪���ܴ��������������׳��������
				for (i = 0; i < 15; i++)
				{
					p1_TmpBuf[i] = *(p3 + 4);
					p3++;
				}
				for (i = 0; i <= 5; i++) // �ѽ��յ��ĵ�1·���ݳ����ַ�ת��Ϊ16������
				{
					if (p1_TmpBuf[i] == 0x0D) // ˵�����ݳ����ַ�������û����
					{
						ReceiveGPRSBufOneDateLen = atoi(&p1_TmpBuf[0], i); // ת��Ϊ16����

						ReceiveGPRSBufTwoDateLen = 0;
						ReceiveGPRSBufThreeDateLen = 0;
						ReceiveGPRSBufFourDateLen = 0;
						break; //
					}
				}
				for (i = 0; i <= 5; i++)
					p1_TmpBuf[i] = 0; // �ѽ��յĵ�1·�ַ����泤������0��Ϊ��һ�μ���
				//					for (i = 0; i <RCV3_MAX; i++)
				//						ReceiveGPRSBuf[i] = 0; //�����ReceiveGPRSBuf��Ȼ���ٰѽ��յ������ݷŵ�����
				memset(ReceiveGPRSBuf, 0, RCV3_MAX);
				m = ReceiveGPRSBufOneDateLen;
				for (RecNumBit = 0; m != 0; RecNumBit++)
					m /= 10;
				m = Pw_Link1IP1;
				for (IP1NumBit = 0; m != 0; IP1NumBit++)
					m /= 10;
				m = Pw_Link1IP2;
				for (IP2NumBit = 0; m != 0; IP2NumBit++)
					m /= 10;
				m = Pw_Link1IP3;
				for (IP3NumBit = 0; m != 0; IP3NumBit++)
					m /= 10;
				m = Pw_Link1IP4;
				for (IP4NumBit = 0; m != 0; IP4NumBit++)
					m /= 10;
				m = Pw_Link1Port;
				for (PortNumBit = 0; m != 0; PortNumBit++)
					m /= 10;
				n = RecNumBit + IP1NumBit + IP2NumBit + IP3NumBit + IP4NumBit + PortNumBit;
				for (i = 24 + n; i <= (ReceiveGPRSBufOneDateLen + 24 + n); i++)
				{
					ReceiveGPRSBuf[j] = Rcv3Buffer[i]; // ���յ��ĵ�1·���ݸ�ReceiveGPRSBuf
													   //					ReceiveGPRSBuf[j] = Rcv3_Back_Buffer[i]; // ���յ��ĵ�1·���ݸ�ReceiveGPRSBuf

					j++;
				}
				if (j >= ReceiveGPRSBufOneDateLen)
					j = 0;
				ChannelNo = 0; // ͨ���ţ��ڼ�·ͨ���յ�����
				F_AcklinkNum = 1;
			}
		}

		// ��2·
		if ((Pw_Link2IP1 + Pw_Link2IP2 + Pw_Link2IP3 + Pw_Link2IP4) != 0)
		{
			sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link2IP1, Pw_Link2IP2, Pw_Link2IP3, Pw_Link2IP4); // ���IP
			sprintf((char *)portBuf, "%d", Pw_Link2Port);											   // ��һ·����Զ�˶˿���ת��Ϊascii��
			sprintf((char *)p_RECV_IPBuf, "%s:%s", ipbuf, portBuf);									   // ��������Ϣ��p_RECV_IPBuf
			if (strstr((const char *)p2, (const char *)p_RECV_IPBuf))								   // �յ��ĵ�2·����������
			{
				p3 = (u8 *)strstr((const char *)p2, "+IPD");
				//					sprintf((char *)p1_TmpBuf, "%s", p3 + 4); //���յ��ĵ�2·���ݳ����ַ�
				for (i = 0; i < 15; i++)
				{
					p1_TmpBuf[i] = *(p3 + 4);
					p3++;
				}
				for (i = 0; i <= 5; i++) // �ѽ��յ��ĵ�2·���ݳ����ַ�ת��Ϊ16������
				{
					if (p1_TmpBuf[i] == 0x0D) // ˵�����ݳ����ַ�������û����
					{
						ReceiveGPRSBufTwoDateLen = atoi(&p1_TmpBuf[0], i); // ת��Ϊ16����

						ReceiveGPRSBufOneDateLen = 0;
						ReceiveGPRSBufThreeDateLen = 0;
						ReceiveGPRSBufFourDateLen = 0;
						break; //
					}
				}
				for (i = 0; i <= 5; i++)
					p1_TmpBuf[i] = 0; // �ѽ��յĵ�2·�ַ����泤������0��Ϊ��һ�μ���
				//					for (i = 0; i <RCV3_MAX; i++)
				//						ReceiveGPRSBuf[i] = 0; //�����ReceiveGPRSBuf��Ȼ���ٰѽ��յ������ݷŵ�����
				memset(ReceiveGPRSBuf, 0, RCV3_MAX);
				m = ReceiveGPRSBufTwoDateLen;
				for (RecNumBit = 0; m != 0; RecNumBit++)
					m /= 10;
				m = Pw_Link2IP1;
				for (IP1NumBit = 0; m != 0; IP1NumBit++)
					m /= 10;
				m = Pw_Link2IP2;
				for (IP2NumBit = 0; m != 0; IP2NumBit++)
					m /= 10;
				m = Pw_Link2IP3;
				for (IP3NumBit = 0; m != 0; IP3NumBit++)
					m /= 10;
				m = Pw_Link2IP4;
				for (IP4NumBit = 0; m != 0; IP4NumBit++)
					m /= 10;
				m = Pw_Link2Port;
				for (PortNumBit = 0; m != 0; PortNumBit++)
					m /= 10;
				n = RecNumBit + IP1NumBit + IP2NumBit + IP3NumBit + IP4NumBit + PortNumBit;
				for (i = 24 + n; i <= (ReceiveGPRSBufTwoDateLen + 24 + n); i++)
				{
					ReceiveGPRSBuf[j] = Rcv3Buffer[i]; // ���յ��ĵ�2·���ݸ�ReceiveGPRSBuf
													   //					ReceiveGPRSBuf[j] = Rcv3_Back_Buffer[i]; // ���յ��ĵ�1·���ݸ�ReceiveGPRSBuf
					j++;
				}
				if (j >= ReceiveGPRSBufTwoDateLen)
					j = 0;
				ChannelNo = 1; // ͨ���ţ��ڼ�·ͨ���յ�����
				F_AcklinkNum = 2;
			}
		}

		// ��3·
		if ((Pw_Link3IP1 + Pw_Link3IP2 + Pw_Link3IP3 + Pw_Link3IP4) != 0)
		{
			sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link3IP1, Pw_Link3IP2, Pw_Link3IP3, Pw_Link3IP4); // ���IP
			sprintf((char *)portBuf, "%d", Pw_Link3Port);											   // ��һ·����Զ�˶˿���ת��Ϊascii��
			sprintf((char *)p_RECV_IPBuf, "%s:%s", ipbuf, portBuf);									   // ��������Ϣ��p_RECV_IPBuf
			if (strstr((const char *)p2, (const char *)p_RECV_IPBuf))								   // �յ��ĵ�3·����������
			{
				p3 = (u8 *)strstr((const char *)p2, "+IPD");
				//					sprintf((char *)p1_TmpBuf, "%s", p3 + 4); //���յ��ĵ�3·���ݳ����ַ�
				for (i = 0; i < 15; i++)
				{
					p1_TmpBuf[i] = *(p3 + 4);
					p3++;
				}
				for (i = 0; i <= 5; i++) // �ѽ��յ��ĵ�3·���ݳ����ַ�ת��Ϊ16������
				{
					if (p1_TmpBuf[i] == 0x0D) // ˵�����ݳ����ַ�������û����
					{
						ReceiveGPRSBufThreeDateLen = atoi(&p1_TmpBuf[0], i); // ת��Ϊ16����

						ReceiveGPRSBufOneDateLen = 0;
						ReceiveGPRSBufTwoDateLen = 0;
						ReceiveGPRSBufFourDateLen = 0;
						break; //
					}
				}
				for (i = 0; i <= 5; i++)
					p1_TmpBuf[i] = 0; // �ѽ��յĵ�3·�ַ����泤������0��Ϊ��һ�μ���
				//					for (i = 0; i <RCV3_MAX; i++)
				//						ReceiveGPRSBuf[i] = 0; //�����ReceiveGPRSBuf��Ȼ���ٰѽ��յ������ݷŵ�����
				memset(ReceiveGPRSBuf, 0, RCV3_MAX);
				m = ReceiveGPRSBufThreeDateLen;
				for (RecNumBit = 0; m != 0; RecNumBit++)
					m /= 10;
				m = Pw_Link3IP1;
				for (IP1NumBit = 0; m != 0; IP1NumBit++)
					m /= 10;
				m = Pw_Link3IP2;
				for (IP2NumBit = 0; m != 0; IP2NumBit++)
					m /= 10;
				m = Pw_Link3IP3;
				for (IP3NumBit = 0; m != 0; IP3NumBit++)
					m /= 10;
				m = Pw_Link3IP4;
				for (IP4NumBit = 0; m != 0; IP4NumBit++)
					m /= 10;
				m = Pw_Link3Port;
				for (PortNumBit = 0; m != 0; PortNumBit++)
					m /= 10;
				n = RecNumBit + IP1NumBit + IP2NumBit + IP3NumBit + IP4NumBit + PortNumBit;
				for (i = 24 + n; i <= (ReceiveGPRSBufThreeDateLen + 24 + n); i++)
				{
					ReceiveGPRSBuf[j] = Rcv3Buffer[i]; // ���յ��ĵ�3·���ݸ�ReceiveGPRSBuf
													   //					ReceiveGPRSBuf[j] = Rcv3_Back_Buffer[i]; // ���յ��ĵ�1·���ݸ�ReceiveGPRSBuf
					j++;
				}
				if (j >= ReceiveGPRSBufThreeDateLen)
					j = 0;
				ChannelNo = 2; // ͨ���ţ��ڼ�·ͨ���յ�����
				F_AcklinkNum = 3;
			}
		}

		// ��4·
		if ((Pw_Link4IP1 + Pw_Link4IP2 + Pw_Link4IP3 + Pw_Link4IP4) != 0)
		{
			sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link4IP1, Pw_Link4IP2, Pw_Link4IP3, Pw_Link4IP4); // ���IP
			sprintf((char *)portBuf, "%d", Pw_Link4Port);											   // ��һ·����Զ�˶˿���ת��Ϊascii��
			sprintf((char *)p_RECV_IPBuf, "%s:%s", ipbuf, portBuf);									   // ��������Ϣ��p_RECV_IPBuf
			if (strstr((const char *)p2, (const char *)p_RECV_IPBuf))								   // �յ��ĵ�4·����������
			{
				p3 = (u8 *)strstr((const char *)p2, "+IPD");
				//					sprintf((char *)p1_TmpBuf, "%s", p3 + 4); //���յ��ĵ�4·���ݳ����ַ�
				for (i = 0; i < 15; i++)
				{
					p1_TmpBuf[i] = *(p3 + 4);
					p3++;
				}
				for (i = 0; i <= 5; i++) // �ѽ��յ��ĵ�4·���ݳ����ַ�ת��Ϊ16������
				{
					if (p1_TmpBuf[i] == 0x0D) // ˵�����ݳ����ַ�������û����
					{
						ReceiveGPRSBufFourDateLen = atoi(&p1_TmpBuf[0], i); // ת��Ϊ16����

						ReceiveGPRSBufOneDateLen = 0;
						ReceiveGPRSBufTwoDateLen = 0;
						ReceiveGPRSBufThreeDateLen = 0;
						break; //
					}
				}
				for (i = 0; i <= 5; i++)
					p1_TmpBuf[i] = 0; // �ѽ��յĵ�4·�ַ����泤������0��Ϊ��һ�μ���
				//					for (i = 0; i <RCV3_MAX; i++)
				//						ReceiveGPRSBuf[i] = 0; //�����ReceiveGPRSBuf��Ȼ���ٰѽ��յ������ݷŵ�����
				memset(ReceiveGPRSBuf, 0, RCV3_MAX);
				m = ReceiveGPRSBufFourDateLen;
				for (RecNumBit = 0; m != 0; RecNumBit++)
					m /= 10;
				m = Pw_Link4IP1;
				for (IP1NumBit = 0; m != 0; IP1NumBit++)
					m /= 10;
				m = Pw_Link4IP2;
				for (IP2NumBit = 0; m != 0; IP2NumBit++)
					m /= 10;
				m = Pw_Link4IP3;
				for (IP3NumBit = 0; m != 0; IP3NumBit++)
					m /= 10;
				m = Pw_Link4IP4;
				for (IP4NumBit = 0; m != 0; IP4NumBit++)
					m /= 10;
				m = Pw_Link4Port;
				for (PortNumBit = 0; m != 0; PortNumBit++)
					m /= 10;
				n = RecNumBit + IP1NumBit + IP2NumBit + IP3NumBit + IP4NumBit + PortNumBit;
				for (i = 24 + n; i <= (ReceiveGPRSBufFourDateLen + 24 + n); i++)
				{
					ReceiveGPRSBuf[j] = Rcv3Buffer[i]; // ���յ��ĵ�4·���ݸ�ReceiveGPRSBuf
													   //					ReceiveGPRSBuf[j] = Rcv3_Back_Buffer[i]; // ���յ��ĵ�1·���ݸ�ReceiveGPRSBuf
					j++;
				}
				if (j >= ReceiveGPRSBufFourDateLen)
					j = 0;
				ChannelNo = 3; // ͨ���ţ��ڼ�·ͨ���յ�����
				F_AcklinkNum = 4;
			}
		}

		// delay_ms(50);
		// sim7600ce_DoWithData(); // ������յ�������
	}
}

// ����������������
void Send_DomainName(void)
{
	uchar i;

	//---------------------------------------------A.B.C��������ĵط����뷢�������Ƚϣ�
	// A. ͳ����Ҫ���ӵ�����
	ConnectNum_DomainName = 0;
	for (i = 0; i < 4; i++)
	{
		if (GprsPar[Ip0Base + i * 4] == 0 && GprsPar[DomainName0Base + i * 31] != 0)
		{
			F_IPOK[i] = 1;
			ConnectNum_DomainName++;
		}
		else
		{
			F_IPOK[i] = 0;
		}
	}

	// C. ��һ�����ӣ���IP��ַ������0��������������0��ͨ���������������־
	for (i = 0; i < 4; i++)
	{
		// IP��ַ������0��������������0��ԭ��û�н��й����ӣ������������ӣ�����������ʱ��������
		if (GprsPar[Ip0Base + i * 4] == 0 && GprsPar[DomainName0Base + i * 31] != 0)
		{
			if (B_FirstLink_DomainName[i] == 0) //!!! �ӿ����ӵĹ��� 2014.12.3��ע
			{
				B_LinkAsk_DomainName[i] = 1;
				B_FirstLink_DomainName[i] = 1;
			}
		}
		// û�����ӵ�ַ�������һ�����ӱ�־
		else
			B_FirstLink_DomainName[i] = 0;
	}
	//---------------------------------------------����A.B.C��������ĵط����뷢�������Ƚϣ�

	// 1. ���ӽ�����ʱ�ж�
	// for (i = 0; i < 4; i++)
	// {
	// 	if (B_LinkAsk_DomainName[i] == 0)
	// 	{
	// 		// �Ѿ������� ���� IP��ַ��������ַΪ0�ˣ�������Ҫ�����ˣ� 2014.12.3
	// 		if (CGD0_LinkConnectOK[i] || (GprsPar[Ip0Base + i * 4] + k) == 0)
	// 			C_LinkDelay[i] = 0;
	// 		// ʱ�䵽�������������־B_LinkAsk_DomainName[i]=1
	// 		else
	// 		{
	// 			// ������ʱ������300S����2���ֽ�
	// 			j = GprsPar[ReConnect0Base + i * 2];
	// 			j = (j << 8) + GprsPar[ReConnect0Base + 1 + i * 2];
	// 			// 2013.6.11 �йر����ӱ�־��������ʱ2��󣬲ſ��������ӡ���Ϊ�˿������ӣ�
	// 			if (B_CloseLink[i] == 2)
	// 			{
	// 				if (C_LinkDelay[i] > 3)
	// 				{
	// 					B_LinkAsk_DomainName[i] = 1; // ʱ�䵽�������������־
	// 					B_CloseLink[i] = 0;
	// 				}
	// 			}
	// 			// ����ֻ�е�������ʱ������300S����ſ������ӡ�
	// 			else if (C_LinkDelay[i] > j)
	// 			{
	// 				B_LinkAsk_DomainName[i] = 1; // ʱ�䵽�������������־
	// 			}
	// 		}
	// 	}
	// 	else
	// 		C_LinkDelay[i] = 0;
	// }

	// 2. ���ڷ������� ��ÿ·����ѭ�����ͣ��з��������־�ͷ������ӣ���
	if (DomainName_No > 3)
		DomainName_No = 0;
	i = DomainName_No;

	if (B_LinkAsk_DomainName[i] == 0)
	{
		DomainName_No++; // ָ����һ·���ӵķ���
	}
	// �����������ȷ��������� ������
	else if (S_LinkNow_DoName[i] == 0 && B_HeartSendAsk[0] + B_HeartSendAsk[1] + B_HeartSendAsk[2] + B_HeartSendAsk[3] > 0)
		;
	//
	else
	{
		// ָ�������
		// ���Ըĳ�==100������������������������
		if (S_HeartSend + S_DtuInquireSend + S_DtuSetSend + S_GprsDataSend + S_SendSms + S_ReadNewSms + S_SmsFull == 0) //=0  =100����ʹ�ã��ݲ�����
		{
			if (S_LinkNow_DoName[i] == 0)
			{
				S_LinkNow_DoName[i] = 1;
				C_LinkNow_DoName[i] = 0;
			}

			if (S_LinkNow_DoName[i] == 1 && C_LinkNow_DoName[i] > 2) //  ���ò�������OK  1, CONNECT OK һ���
			{
				S_LinkNow_DoName[i] = 2;
				Cw_Txd3Max = 0;	  // ����ɾ 2012.10.4
				At_DomainName(i); // ����������ѯָ��
				C_LinkNow_DoName[i] = 0;
			}
			else if (S_LinkNow_DoName[i] == 2)
			{
				// 3. �����ӱ�־�����������־
				//				if (F_IPOK[i])
				{
					// C_LinkDelay[i] = 0;
					C_LinkNow_DoName[i] = 0;
					S_LinkNow_DoName[i] = 0;
					B_LinkAsk_DomainName[i] = 0;
					// B_FirstLink_DomainName[i] = 0;
				}
			}
		}

		// 4. ��ʱ��û�����ӳɹ������
		// if (S_LinkNow_DoName[i] > 0)
		// {
		// 	if (C_LinkNow_DoName[i] > 5)
		// 	{
		// 		C_LinkDelay[i] = 0; //=0�����Ƶ�ǰ�棬�����и������ֵ 2013.6.28
		// 		//
		// 		if (B_LinkFault[i] < 3) // ǰ4��û�гɹ������������� 2013.6.23
		// 		{
		// 			if (ConnectNum <= 1) // ֻ��1·�������ÿ���ֱ���ٷ��������� 2013.6.28
		// 				B_LinkAsk_DomainName[i] = 1;
		// 			else // ��2·�������������ã�����ʱ�ٷ���������(����������ʱ��)
		// 			{
		// 				B_LinkAsk_DomainName[i] = 0;
		// 				//
		// 				Lw_GprsTmp = GprsPar[ReConnect0Base + i * 2];
		// 				Lw_GprsTmp = (Lw_GprsTmp << 8) + GprsPar[ReConnect0Base + 1 + i * 2];
		// 				if (Lw_GprsTmp > 8)
		// 					C_LinkDelay[i] = Lw_GprsTmp - 8;
		// 				else
		// 					C_LinkDelay[i] = Lw_GprsTmp;
		// 			}
		// 		}
		// 		else // ������ϴ���3�Σ������B_LinkAsk_DomainName[i]��־�����¼�ʱ
		// 			B_LinkAsk_DomainName[i] = 0;
		// 		//
		// 		C_LinkNow_DoName[i] = 0;
		// 		S_LinkNow_DoName[i] = 0;
		// 		B_LinkFault[i]++;
		// 	}
		// }
	}

	// �ϲ�һ��״̬
	// S_SendLink = S_LinkNow_DoName[0] + S_LinkNow_DoName[1] + S_LinkNow_DoName[2] + S_LinkNow_DoName[3];
}

// ��������
void DomainNameResolution(void)
{
	u8 *p2, *p3;
	u8 i = 0, k = 0;

	// ��1·����
	for (i = 0; i < 31; i++)
	{
		domain_name[i] = GprsPar[DomainName0Base + i];
	}
	p2 = (u8 *)strstr((const char *)Rcv3Buffer, (const char *)domain_name); // ���ս�����IP��Ϣ
	if (p2)																	// ������ȷ
	{
		p3 = (u8 *)strstr((const char *)p2, ","); //
		sprintf((char *)p_TmpBuf, "%s", p3 + 2);  // �ѽ�����IP��Ϣ��p_TmpBuf
		for (i = 0; i <= 3; i++)				  // ת��Ϊ16������
		{
			if (p_TmpBuf[i] == 0x2E)
			{
				Pw_Link1IP1 = atoi(&p_TmpBuf[0], i); // ת��Ϊ16����
				GprsPar[Ip0Base] = Pw_Link1IP1;		 // YLS 2022.12.27
				k = i + 1;							 // IP��һλ��ֵ
				break;								 //
			}
		}
		for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
		{
			if (p_TmpBuf[i] == 0x2E)
			{
				Pw_Link1IP2 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
				GprsPar[Ip0Base + 1] = Pw_Link1IP2;		 // YLS 2022.12.27
				k = i + 1;								 // IP��һλ��ֵ
				break;									 //
			}
		}
		for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
		{
			if (p_TmpBuf[i] == 0x2E)
			{
				Pw_Link1IP3 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
				GprsPar[Ip0Base + 2] = Pw_Link1IP3;		 // YLS 2022.12.27
				k = i + 1;								 // IP��һλ��ֵ
				break;									 //
			}
		}
		for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
		{
			if (p_TmpBuf[i] == 0x22)
			{
				Pw_Link1IP4 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
				GprsPar[Ip0Base + 3] = Pw_Link1IP4;		 // YLS 2022.12.27
				k = 0;									 //
				break;									 //
			}
		}
	}

	// ��2·����
	for (i = 0; i < 31; i++)
	{
		domain_name[i] = GprsPar[DomainName1Base + i];
	}
	p2 = (u8 *)strstr((const char *)Rcv3Buffer, (const char *)domain_name); // ���ս�����IP��Ϣ
	if (p2)																	// ������ȷ
	{
		p3 = (u8 *)strstr((const char *)p2, ","); //
		sprintf((char *)p_TmpBuf, "%s", p3 + 2);  // �ѽ�����IP��Ϣ��p_TmpBuf
		for (i = 0; i <= 3; i++)				  // ת��Ϊ16������
		{
			if (p_TmpBuf[i] == 0x2E)
			{
				Pw_Link2IP1 = atoi(&p_TmpBuf[0], i); // ת��Ϊ16����
				GprsPar[Ip1Base] = Pw_Link2IP1;		 // YLS 2022.12.27
				k = i + 1;							 // IP��һλ��ֵ
				break;								 //
			}
		}
		for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
		{
			if (p_TmpBuf[i] == 0x2E)
			{
				Pw_Link2IP2 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
				GprsPar[Ip1Base + 1] = Pw_Link2IP2;		 // YLS 2022.12.27
				k = i + 1;								 // IP��һλ��ֵ
				break;									 //
			}
		}
		for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
		{
			if (p_TmpBuf[i] == 0x2E)
			{
				Pw_Link2IP3 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
				GprsPar[Ip1Base + 2] = Pw_Link2IP3;		 // YLS 2022.12.27
				k = i + 1;								 // IP��һλ��ֵ
				break;									 //
			}
		}
		for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
		{
			if (p_TmpBuf[i] == 0x22)
			{
				Pw_Link2IP4 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
				GprsPar[Ip1Base + 3] = Pw_Link2IP4;		 // YLS 2022.12.27
				k = 0;									 //
				break;									 //
			}
		}
	}

	// ��3·����
	for (i = 0; i < 31; i++)
	{
		domain_name[i] = GprsPar[DomainName2Base + i];
	}
	p2 = (u8 *)strstr((const char *)Rcv3Buffer, (const char *)domain_name); // ���ս�����IP��Ϣ
	if (p2)																	// ������ȷ
	{
		p3 = (u8 *)strstr((const char *)p2, ","); //
		sprintf((char *)p_TmpBuf, "%s", p3 + 2);  // �ѽ�����IP��Ϣ��p_TmpBuf
		for (i = 0; i <= 3; i++)				  // ת��Ϊ16������
		{
			if (p_TmpBuf[i] == 0x2E)
			{
				Pw_Link3IP1 = atoi(&p_TmpBuf[0], i); // ת��Ϊ16����
				GprsPar[Ip2Base] = Pw_Link3IP1;		 // YLS 2022.12.27
				k = i + 1;							 // IP��һλ��ֵ
				break;								 //
			}
		}
		for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
		{
			if (p_TmpBuf[i] == 0x2E)
			{
				Pw_Link3IP2 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
				GprsPar[Ip2Base + 1] = Pw_Link3IP2;		 // YLS 2022.12.27
				k = i + 1;								 // IP��һλ��ֵ
				break;									 //
			}
		}
		for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
		{
			if (p_TmpBuf[i] == 0x2E)
			{
				Pw_Link3IP3 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
				GprsPar[Ip2Base + 2] = Pw_Link3IP3;		 // YLS 2022.12.27
				k = i + 1;								 // IP��һλ��ֵ
				break;									 //
			}
		}
		for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
		{
			if (p_TmpBuf[i] == 0x22)
			{
				Pw_Link3IP4 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
				GprsPar[Ip2Base + 3] = Pw_Link3IP4;		 // YLS 2022.12.27
				k = 0;									 //
				break;									 //
			}
		}
	}

	// ��4·����
	for (i = 0; i < 31; i++)
	{
		domain_name[i] = GprsPar[DomainName3Base + i];
	}
	p2 = (u8 *)strstr((const char *)Rcv3Buffer, (const char *)domain_name); // ���ս�����IP��Ϣ
	if (p2)																	// ������ȷ
	{
		p3 = (u8 *)strstr((const char *)p2, ","); //
		sprintf((char *)p_TmpBuf, "%s", p3 + 2);  // �ѽ�����IP��Ϣ��p_TmpBuf
		for (i = 0; i <= 3; i++)				  // ת��Ϊ16������
		{
			if (p_TmpBuf[i] == 0x2E)
			{
				Pw_Link4IP1 = atoi(&p_TmpBuf[0], i); // ת��Ϊ16����
				GprsPar[Ip3Base] = Pw_Link4IP1;		 // YLS 2022.12.27
				k = i + 1;							 // IP��һλ��ֵ
				break;								 //
			}
		}
		for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
		{
			if (p_TmpBuf[i] == 0x2E)
			{
				Pw_Link4IP2 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
				GprsPar[Ip3Base + 1] = Pw_Link4IP2;		 // YLS 2022.12.27
				k = i + 1;								 // IP��һλ��ֵ
				break;									 //
			}
		}
		for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
		{
			if (p_TmpBuf[i] == 0x2E)
			{
				Pw_Link4IP3 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
				GprsPar[Ip3Base + 2] = Pw_Link4IP3;		 // YLS 2022.12.27
				k = i + 1;								 // IP��һλ��ֵ
				break;									 //
			}
		}
		for (i = k; i <= 3 + k; i++) // ת��Ϊ16������
		{
			if (p_TmpBuf[i] == 0x22)
			{
				Pw_Link4IP4 = atoi(&p_TmpBuf[k], i - k); // ת��Ϊ16����
				GprsPar[Ip3Base + 3] = Pw_Link4IP4;		 // YLS 2022.12.27
				k = 0;									 //
				break;									 //
			}
		}
	}
}

void Fill_data(void)
{
	// ������(DSC)��IP��ַ
	Pw_Link1IP1 = GprsPar[Ip0Base];
	Pw_Link1IP2 = GprsPar[Ip0Base + 1];
	Pw_Link1IP3 = GprsPar[Ip0Base + 2];
	Pw_Link1IP4 = GprsPar[Ip0Base + 3];

	Pw_Link2IP1 = GprsPar[Ip1Base];
	Pw_Link2IP2 = GprsPar[Ip1Base + 1];
	Pw_Link2IP3 = GprsPar[Ip1Base + 2];
	Pw_Link2IP4 = GprsPar[Ip1Base + 3];

	Pw_Link3IP1 = GprsPar[Ip2Base];
	Pw_Link3IP2 = GprsPar[Ip2Base + 1];
	Pw_Link3IP3 = GprsPar[Ip2Base + 2];
	Pw_Link3IP4 = GprsPar[Ip2Base + 3];

	Pw_Link4IP1 = GprsPar[Ip3Base];
	Pw_Link4IP2 = GprsPar[Ip3Base + 1];
	Pw_Link4IP3 = GprsPar[Ip3Base + 2];
	Pw_Link4IP4 = GprsPar[Ip3Base + 3];

	// ������(DSC)�Ķ˿�
	Pw_Link1Port = (GprsPar[Port0Base] << 8) + GprsPar[Port0Base + 1];
	Pw_Link2Port = (GprsPar[Port1Base] << 8) + GprsPar[Port1Base + 1];
	Pw_Link3Port = (GprsPar[Port2Base] << 8) + GprsPar[Port2Base + 1];
	Pw_Link4Port = (GprsPar[Port3Base] << 8) + GprsPar[Port3Base + 1];

	// ���߱���ʱ����(����)(*10����)
	Pw_link1OnlineReportTime = (GprsPar[OnlineReportInterval0Base] << 8) + GprsPar[OnlineReportInterval0Base + 1];
	Pw_link2OnlineReportTime = (GprsPar[OnlineReportInterval1Base] << 8) + GprsPar[OnlineReportInterval1Base + 1];
	Pw_link3OnlineReportTime = (GprsPar[OnlineReportInterval2Base] << 8) + GprsPar[OnlineReportInterval2Base + 1];
	Pw_link4OnlineReportTime = (GprsPar[OnlineReportInterval3Base] << 8) + GprsPar[OnlineReportInterval3Base + 1];

	// ����������ʱ�䣨�룩
	Pw_Link1NetWorkCheckTime = (GprsPar[NetCheckInterval0Base] << 8) + GprsPar[NetCheckInterval0Base + 1];
	Pw_Link2NetWorkCheckTime = (GprsPar[NetCheckInterval1Base] << 8) + GprsPar[NetCheckInterval1Base + 1];
	Pw_Link3NetWorkCheckTime = (GprsPar[NetCheckInterval2Base] << 8) + GprsPar[NetCheckInterval2Base + 1];
	Pw_Link4NetWorkCheckTime = (GprsPar[NetCheckInterval3Base] << 8) + GprsPar[NetCheckInterval3Base + 1];

	// ���ض˿�
	Pw_Link1localPort = (GprsPar[LoclaPort0Base] << 8) + GprsPar[LoclaPort0Base + 1];
	Pw_Link2localPort = (GprsPar[LoclaPort1Base] << 8) + GprsPar[LoclaPort1Base + 1];
	Pw_Link3localPort = (GprsPar[LoclaPort2Base] << 8) + GprsPar[LoclaPort2Base + 1];
	Pw_Link4localPort = (GprsPar[LoclaPort3Base] << 8) + GprsPar[LoclaPort3Base + 1];

	// TCP=1.UDP=0
	Pw_Link1TCP_UDPType = GprsPar[LinkTCPUDP0Base];
	Pw_Link2TCP_UDPType = GprsPar[LinkTCPUDP1Base];
	Pw_Link3TCP_UDPType = GprsPar[LinkTCPUDP2Base];
	Pw_Link4TCP_UDPType = GprsPar[LinkTCPUDP3Base];

	// �ز���ʱ����
	Pw_link1ReconnectTimeBetwen = (GprsPar[RedialIntervalBase] << 8) + GprsPar[RedialIntervalBase + 1];

	// ֧�����ݻ�ִ
	Pw_SupportDataReceipt = GprsPar[DataReturnBase];
	// ��������ʱ����
	Pw_LastPacketIdleTime = GprsPar[LastPacketIdleIntervalBase];
}

void Com2_printf(char *fmt, ...)
{
	uint16_t txd_max;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)Txd2Buffer_TEMP, fmt, ap); // vsprintf   sprintf
	va_end(ap);

	// ������ʽ����
	txd_max = strlen((const char *)Txd2Buffer_TEMP);
	UART2_Output_Datas(Txd2Buffer_TEMP, txd_max);
}

void Com1_printf(char *fmt, ...)
{
	uint16_t txd_max;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)Txd1Buffer_TEMP, fmt, ap); // vsprintf   sprintf
	va_end(ap);
	// ������ʽ����
	txd_max = strlen((const char *)Txd1Buffer_TEMP);
	UART1_Output_Datas(Txd1Buffer_TEMP, txd_max);
}

void Com1_sendData(uint8_t txd_buffer[], uint16_t send_num)
{
	uint16_t txd_max;

	if (send_num == 0)
	{
		txd_max = strlen((const char *)txd_buffer);
	}
	else
	{
		txd_max = send_num;
	}
	UART1_Output_Datas(txd_buffer, txd_max);
}

void UART1_Output_Datas(uint8_t S_Out[], uint8_t Counter)
{
	uint8_t i;
	USART_ITConfig(USART1, USART_IT_TC, DISABLE); // ��ֹ�����ж�
	for (i = 0; i < Counter; i++)
	{
		USART_SendData(USART1, S_Out[i]);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; // �ȴ����ͽ���
	}
	USART_ITConfig(USART1, USART_IT_TC, ENABLE); // �������ж�
}

void UART2_Output_Datas(uint8_t S_Out[], uint8_t Counter)
{
	uint8_t i;

	USART_ITConfig(USART2, USART_IT_TC, DISABLE); // ��ֹ�����ж�
	for (i = 0; i < Counter; i++)
	{
		USART_SendData(USART2, S_Out[i]);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET)
			; // �ȴ����ͽ���
	}
	USART_ITConfig(USART2, USART_IT_TC, ENABLE); // �������ж�
}

// ���Ҫ��ѯ���ص�DTU����
void Fill_Dtu_Par(void)
{
	uchar i, len;

	// a.���ȼ���
	AccessPointLength = Gprs_StrCmdLen(&GprsPar[AccessPointBase]); // 05
	DtuNoLength = Gprs_StrCmdLen(&GprsPar[DtuNoBase]);			   // 06
	DomainName0Length = Gprs_StrCmdLen(&GprsPar[DomainName0Base]); // 21
	DomainName1Length = Gprs_StrCmdLen(&GprsPar[DomainName1Base]); // 31
	DomainName2Length = Gprs_StrCmdLen(&GprsPar[DomainName2Base]); // 41
	DomainName3Length = Gprs_StrCmdLen(&GprsPar[DomainName3Base]); // 51

	// b.ͷ���ȣ��ڵڶ���������䣬��ΪAt_QISENDAccordingLengthָ��Ḳ�ǿ�ʼ���֡�
	Cw_Txd3Max = 31; // ���ͳ��ȸ�λ�ֽ�.��λ�ֽ� ����
					 // ��ʼͷ31���ֽ�

	// c.��䱣���ָ��
	for (i = 1; i < 69; i++) // 69��  0x70֮ǰ��ָ��
	{
		len = CmdLen[i];

		if (CmdNo[i] == 0x05)
			len = AccessPointLength;
		else if (CmdNo[i] == 0x06)
			len = DtuNoLength;
		else if (CmdNo[i] == 0x21)
			len = DomainName0Length;
		else if (CmdNo[i] == 0x31)
			len = DomainName1Length;
		else if (CmdNo[i] == 0x41)
			len = DomainName2Length;
		else if (CmdNo[i] == 0x51)
			len = DomainName3Length;
		Txd3Buffer[Cw_Txd3Max++] = CmdNo[i];	// ָ��
		Txd3Buffer[Cw_Txd3Max++] = len + 2;		// ����
		Gprs_TX_Fill(&GprsPar[CmdPst[i]], len); // ����
	}
	// d.��䲻�����ָ��
	Txd3Buffer[Cw_Txd3Max++] = 0x70;  // 70: DTU����汾
	Txd3Buffer[Cw_Txd3Max++] = 9;	  // ����
	Gprs_TX_Fill(DtuProgVersion, 7);  // ����
	Txd3Buffer[Cw_Txd3Max++] = 0x71;  // 71: DTU������������
	Txd3Buffer[Cw_Txd3Max++] = 10;	  // ����
	Gprs_TX_Fill(DtuProgMakeDate, 8); // ����
	Txd3Buffer[Cw_Txd3Max++] = 0x73;  // 73: DTUӲ��ƽ̨�汾
	Txd3Buffer[Cw_Txd3Max++] = 24;	  // ����
	Gprs_TX_Fill(DtuHardWare, 14);	  // ����
	Gprs_TX_Fill(Csq, 8);			  // ����
	Txd3Buffer[Cw_Txd3Max++] = 0x74;  // 74: DTU������LOGO
	Txd3Buffer[Cw_Txd3Max++] = 8;	  // ����
	Gprs_TX_Fill(DtuManufacture, 6);  // ����
	Txd3Buffer[Cw_Txd3Max++] = 0x75;  // 75: DTU�ͺ�����
	Txd3Buffer[Cw_Txd3Max++] = 8;	  // ����
	Gprs_TX_Fill(DtuModel, 6);		  // ����
	// e.ĩβ
	Txd3Buffer[Cw_Txd3Max++] = 0x7B; // ������־
	w_StringLength = Cw_Txd3Max;
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
