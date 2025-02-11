/**
  ******************************************************************************
  * @file    Xmodem.c
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.20
  * @date    2013.6.2
  * @brief   XmodemЭ�鴫���ļ���ʹ��CRCУ�顣
	******************************************************************************
	1. 2013.9.4 �ص�˵��������һ���üĴ�����ʽ���ã��ÿⷽʽ��Ȼ���У�����BUG
		//������������ USART2->SR 06λTC�Ѿ�Ϊ1��
		��USART_GetFlagStatus(USART2, USART_IT_TC) ��ⲻ��

		// Loop until the end of transmission
		//while (USART_GetFlagStatus(USART2, USART_IT_TC) == RESET);		//��䲻�У�����BUG  ZCL 2013.9.4
		while((USART2->SR&0X40)==0);	//ѭ������,ֱ��������� 	//������� OK    ZCL 2013.9.4

	2. //Xmodem����ʱ��Ҫ57600�ϸߵĲ����� 2013.7.3
		�˳�����ʱ���ָ��ɴ��������Ĳ����ʡ�
		Pw_GprsBaudRate2=w_GprsSaveBaudRate2;

	3. �ϵ���������ļ����䣬���ð��ո����
	  //���ո��������м����£���������λ�����Ͳ��������ļ�GP311.ini
		if(1)		//ZCL 2019.3.14 ����ʱ
		//if( CheckSPACEInput() )

	4.  ZCL 2019.4.5
		���⣺.INI GPRS�����ļ����� 2048�ֽڣ����������
		����취��GPRS�����ļ���ȥ���ظ��ĺ��֣�GP311 TwoLink  V133 V132 2019.3.20 �·�����+����.ini
							RCV2_MAX		2048

	5. ZCL 2019.4.5
		���⣺XmodemFileTransferʱ����ʱ�򲻴�ӡ��"Xmodem set end��set par item 69 times! OK" ?
		����취��������ʱ������ �о�����û�д����꣬���Ա�����ʱ����ʱ����50MS�����Ժ��ã�
		w_GprsXmodemRcvLen=XmodemReceive(Rcv2Buffer,RCV2_MAX);		//ZCL 2019.4.5

		Delay_MS(500);	//ZCL 2019.4.5	����Ҫ��OK		2000,1000,500,200,50��������ʱ����ӡ����69���������
										//2MS���С�10,13,16,20,30MS,���С� 20MSż���й�
		if(w_GprsXmodemRcvLen>0)
		{
			XmodemRcvDoWith();
			w_GprsXmodemFTItem=Lw_GprsTmp_Com1;	//Xmodem�ļ����� ��  ZCL 2019.4.5
			B_ForceSavPar=1;								//�˳��趨��������� 2013.6.6
		}

	*/

/* Includes ------------------------------------------------------------------*/
#include "xmodem_1.h"
#include "xmodem.h"
#include "GlobalV_Extern.h" // ȫ�ֱ�������
#include "GlobalConst.h"
#include "string.h"
#include <stdio.h> //���ϴ˾������printf

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint LastError_Com1 = 0;
uint C_ReceiveChar_Com1; // �����ַ�ʱ�������
uint Lw_GprsReceiveCount_Com1;
uint Lw_GprsTmp_Com1;
u8 tmp_Com1;

/* Private variables extern --------------------------------------------------*/
extern uchar Rcv1Buffer[];		 // ���ջ�����
extern uchar *ptr, *ptr2, *ptr3; // ָ��
extern uc16 CmdPst[];
extern uc8 GprsCmd2Byte[];
extern uc8 CmdNo[];
extern uchar CommandNo;		// �������
extern uchar B_ForceSavPar; // ǿ�Ʊ��������־

/* Private function prototypes -----------------------------------------------*/
extern void Delay_US(vu16 nCount);
extern void Delay_MS(vu16 nCount);

extern uint Str2Bcd(u8 *String);
extern uchar FindFirstValue(uc8 *Buffer, u8 FindValue, u8 EndValue);
extern uint Str2Hex(u8 *String);
extern void Com1_Init(void);
extern void Com1_printf(char *fmt, ...);
extern u16 Crc16_ccitt(const char *buf, int len);
extern u16 Check(int crc, const uchar *buf, int sz);
/***********************************************************************************
** �������� : Port_OutByte
** �������� : �򴮿ڷ���һ���ַ�����
** ��ڲ��� : <sendchar> ���͵�����
** ���ڲ��� : ��
** �� �� ֵ : ��
** ����˵�� : ��
***********************************************************************************/
void Com1_Port_OutByte(u8 sendchar)
{
	RS485_CON = 1; // 2013.9.2
	Delay_US(10);  // 2013.6.5 Ϊ�˼�С����У���ַ�C��Ƶ��
	tmp_Com1 = sendchar;

	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		;										  //==0���ڷ��ͣ��ȴ���
	USART_ClearITPendingBit(USART1, USART_IT_TC); // 2013.9.2	 ���USART_IT_TC��־
	USART_SendData(USART1, tmp_Com1);			  // ��������

	// 2013.9.4 �ص�˵��������һ���üĴ�����ʽ���ã��ÿⷽʽ��Ȼ���У�����BUG
	// ������������ USART1->SR 06λTC�Ѿ�Ϊ1����USART_GetFlagStatus(USART1, USART_IT_TC) ��ⲻ��
	/* Loop until the end of transmission */
	// while (USART_GetFlagStatus(USART1, USART_IT_TC) == RESET);		//��䲻�У�����BUG  ZCL 2013.9.4
	while ((USART1->SR & 0X40) == 0)
		; // ѭ������,ֱ��������� 	//������� OK    ZCL 2013.9.4

	USART_ClearITPendingBit(USART1, USART_IT_TC); // 2013.9.2
}

/***********************************************************************************
** �������� : Port_InByte
** �������� : �򴮿ڽ���һ���ַ�����
** ��ڲ��� : <Time> ��ȡ���������ʱʱ��(ms)
** ���ڲ��� : ��
** �� �� ֵ : �Ӵ��ڽ��յ������ݻ���Ϊ0
** ����˵�� : ��
***********************************************************************************/
u8 Com1_Port_InByte(u16 Time)
{
	u8 ch;

	RS485_CON = 0; // 2013.9.2
	ch = 0;
	C_ReceiveChar_Com1 = 0; // �����ַ�ʱ�������
	LastError_Com1 = 0;

	while (C_ReceiveChar_Com1 < Time) // C_ReceiveChar��SysTickHandler��ÿ����++
	{
		if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
		{
			/* Read one byte from the receive data register */
			ch = USART_ReceiveData(USART1);
			/* Clear the USART1 Receive interrupt */
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
			// if(ch>0)  //�˾�ļ��뵼�´���������յ�����Ϊ0����LastError=1������ 2013.6.4
			return ch;
		}
	}

	LastError_Com1 = 1;
	return ch;
}

/********************************************************************************
 * ��������:  CheckSPACEInput(void)
 * ��������: ͨ�����ڼ���Ƿ��пո������,û����Ч���뷵��0������Ч���뷵��1
 * ��ڲ���: ��
 * ���ڲ���������������ո�����룬��������1
 * ����Ӧ�ã���
 * �ر�ע�⣺
 ********************************************************************************/
uchar Com1_CheckSPACEInput(void)
{
	uint i;

	// ����ʱ�������ڷ���ʱ������ء������նˡ����棬���ո������0x20;  2013.6.4
	// ����Ӧ��ʱ����ʱ�����̣����� �ܳ���
	Delay_MS(200); // 2000
				   // i=USART_ReceiveData(USART2);  �˷�ʽ���У�����ĳ�����Ľ��շ�ʽ������ 2013.6.5
	i = Com1_Port_InByte(DLY_1S);
	if (i == SPACE)
	{
		Delay_MS(100);
		// i=USART_ReceiveData(USART2);  �˷�ʽ���У�����ĳ�����Ľ��շ�ʽ������ 2013.6.5
		i = Com1_Port_InByte(DLY_1S);
		if (i == SPACE)
		{
			Com1_printf("\r\n------------------------------------Begin \r\n");
			Com1_printf("Enter Xmodem set gprs par mode\r\n");
			return 1; // ����������ո��������1
		}
	}
	return 0;
}

/********************************************************************************
 * ��������:  XmodemFileTransfer(void)
 * ��������: ʹ��XmodemЭ������ļ����� ���ļ����䣺File Transfer ��
 * ��ڲ���: ��
 * ���ڲ�������
 * ����Ӧ�ã���
 * �ر�ע�⣺
 ********************************************************************************/
u8 B_Xmodem_Com1;				   // ZCL 2020.4.17
void XmodemFileTransfer_Com1(void) // ZCL 2013.5.29 	XmodemЭ������ļ����� ���ļ����䣺File Transfer ��
{
	// Xmodem����ʱ��Ҫ57600�ϸߵĲ����� 2013.7.3
	u8 i = 0;
	w_GprsSaveBaudRate2 = Pw_GprsBaudRate2;
	w_GprsSaveDataBitLen2 = Pw_GprsDataBitLen2;
	w_GprsSaveStopBitLen2 = Pw_GprsStopBitLen2;
	w_GprsSaveParityBitLen2 = Pw_GprsParityBitLen2;
	w_GprsSaveFlowControl2 = Pw_GprsFlowControl2;
	Pw_GprsBaudRate2 = 57600;	 // 57600:�ϸߵĲ�����
	Pw_GprsDataBitLen2 = 0x03;	 // 0x03:8λ����λ
	Pw_GprsStopBitLen2 = 0x00;	 // 0x00:1λֹͣλ
	Pw_GprsParityBitLen2 = 0x00; // 0x00:��У��
	Pw_GprsFlowControl2 = 0x03;	 // 0x03:������

	Com1_Init();

	B_Xmodem_Com1 = 1;			 // ZCL 2020.4.17
	Com1_printf("  Xmodem\r\n"); // ZCL 2018.12.10

	w_GprsXmodemFTItem = 555; // û���ļ������ֵ��Xmodem�ļ����� ��  ZCL 2019.4.5
	// ���ո��������м����£���������λ�����Ͳ��������ļ�GP311.ini
	// if(1)		//ZCL 2019.3.14 ����ʱ
	if (Com1_CheckSPACEInput())
	{

		w_GprsXmodemRcvLen = XmodemReceive_Com1(Rcv1Buffer, RCV1_MAX); // ZCL 2019.4.5

		Delay_MS(500); // ZCL 2019.4.5	����Ҫ��OK		2000,1000,500,200,50��������ʱ����ӡ����69���������
					   // 2MS���С�10,13,16,20,30MS,���С� 20MSż���й�
		if (w_GprsXmodemRcvLen > 0)
		{
			XmodemRcvDoWith_Com1();
			w_GprsXmodemFTItem = Lw_GprsTmp_Com1; // Xmodem�ļ����� ��  ZCL 2019.4.5
			B_ForceSavPar = 1;					  // �˳��趨��������� 2013.6.6
		}

		// ��ӡ����������ۿ����������������ر��Ƕ������Ź������У� 2013.7.3
		Com1_printf("Exit Xmodem , Enter normal run mode. \r\n");
		Com1_printf("-------------------------------------End \r\n");
		i = 1;
	}

	// Delay_MS(100);		//ZCL 2019.4.5

	// �˳�Xmodemʱ�������貨����  2013.7.3
	Pw_GprsBaudRate2 = w_GprsSaveBaudRate2; // ����Xmodemģʽ��ʹ��57600�ϸߵĲ�����
	Pw_GprsDataBitLen2 = w_GprsSaveDataBitLen2;
	Pw_GprsStopBitLen2 = w_GprsSaveStopBitLen2;
	Pw_GprsParityBitLen2 = w_GprsSaveParityBitLen2;
	Pw_GprsFlowControl2 = w_GprsSaveFlowControl2;
	Com1_Init();

	if (i == 0)
		// ��ӡ����������ۿ����������������ر��Ƕ������Ź������У� 2013.7.3
		// Com2_printf("\r\nExit Xmodem , Enter normal run mode (2) \r\n");	  //ZCL 2020.4.17 ����䣬û�к���GPRSģ���ʱ��������ȥ
		B_Xmodem_Com1 = 2; // ZCL 2020.4.17
}

/********************************************************************************
** �������� : XmodemReceive
** �������� : xmodemЭ������ļ�
** ��ڲ��� : <checkType>[in] �����ļ���У�鷽ʽ��'C':crcУ�飬NAK:�ۼӺ�У��
** ���ڲ��� : ��
** �� �� ֵ : �����ļ�����ʱ����ش������
** ����˵�� : ��
********************************************************************************/
// destination Ŀ��		sz:size:�ߴ磬����
int XmodemReceive_Com1(uchar *dest, int destsz) // xmodemЭ������ļ�
{
	uchar xbuff[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */
	uchar *p;
	int bufsz, crc = 0;
	uchar checkchar = 'C';
	uchar packetno = 1;
	int i, c, len = 0;
	int retry, retrans = MAXRETRANS;

	for (;;)
	{
		for (retry = 0; retry < 16; ++retry) //? 16 ���еĳ�ʱ�������¼���������10��
		{
			if (checkchar)
			{
				Delay_MS(200);				  // 2013.6.5 Ϊ�˼�С����У���ַ�C��Ƶ��
				Com1_Port_OutByte(checkchar); // ���ͣ�У��ģʽ�ַ�
			}
			c = Com1_Port_InByte((DLY_1S) << 1); // ���գ���2Sʱ���ڽ��յ����ַ�
			if (LastError_Com1 == 0)
			{
				switch (c)
				{
				case SOH: // SOH��׼XmodemЭ�飨ÿ�����ݰ�����128�ֽ����ݣ�
					bufsz = 128;
					goto start_recv;
				case STX: // STX 1k-Xmodem��ÿ�����ݰ�����1024�ֽ����ݣ�
					bufsz = 1024;
					goto start_recv;
				case EOT: // EOT:����
					// Flushinput();
					Com1_Port_OutByte(ACK); // ���շ��� ACK ����ȷ��
					//*(dest+len)=0;
					return len; /* normal end */
				case CAN_X:		// CAN_X:ȡ������
					c = Com1_Port_InByte(DLY_1S);

					if (c == CAN_X)
					{
						// Flushinput();
						Com1_Port_OutByte(ACK);
						return -1; /* canceled by remote */
					}
					break;
				default:
					break;
				}
			}
		}

		if (checkchar == 'C')
		{
			checkchar = NAK;
			continue;
		}

		// Flushinput();
		Com1_Port_OutByte(CAN_X);
		Com1_Port_OutByte(CAN_X);
		Com1_Port_OutByte(CAN_X);
		return -2; /* sync error */

	start_recv:
		if (checkchar == 'C') // CRCУ��
			crc = 1;
		checkchar = 0;									  // У��ģʽ�ַ����㣬���ٷ���У��ģʽ
		p = xbuff;										  // ָ��
		*p++ = c;										  // �����һ���ַ�
		for (i = 0; i < (bufsz + (crc ? 1 : 0) + 3); ++i) // ѭ�����������ַ�
		{
			c = Com1_Port_InByte(DLY_1S); // �ַ���1����ʱ
			Lw_GprsReceiveCount_Com1 = i;

			if (LastError_Com1 != 0) // �д�������������NAK
				goto reject;
			*p++ = c; // û�д��󱣴��ַ�
		}

		// ���������һ֡����
		// ���������ţ���Ų��룬У��
		if (xbuff[1] == (uchar)(~xbuff[2]) && (xbuff[1] == packetno || xbuff[1] == (uchar)packetno - 1) && Check(crc, &xbuff[3], bufsz))
		{
			if (xbuff[1] == packetno)
			{
				int count = destsz - len;
				if (count > bufsz)
					count = bufsz;
				if (count > 0)
				{
					memcpy(&dest[len], &xbuff[3], count);
					// strcpy (&dest[len], &xbuff[3]);
					len += count;
				}
				++packetno; // ����ż�1
				retrans = MAXRETRANS + 1;
			}
			if (--retrans <= 0)
			{
				// Flushinput();
				Com1_Port_OutByte(CAN_X);
				Com1_Port_OutByte(CAN_X);
				Com1_Port_OutByte(CAN_X);
				return -3; /* too many retry error */
			}

			Com1_Port_OutByte(ACK);
			continue;
		}

	reject:
		// Flushinput();
		Com1_Port_OutByte(NAK);
	}
}

/********************************************************************************
** �������� : XmodemRcvDoWith
** �������� : xmodemЭ����յ����ݽ��д���
** ��ڲ��� : ��
** ���ڲ��� : ��
** �� �� ֵ : ��
** ����˵�� : �����ն������á���ѯ����
********************************************************************************/
void XmodemRcvDoWith_Com1(void) // xmodemЭ����յ����ݽ��д���			//ZCL 2013.5.30
{
	uchar i, r, s, err = 0;
	uint ip[4], w1, n = 0;
	// char *ptr;

	ptr = Rcv1Buffer;
	Lw_GprsTmp_Com1 = 0;

	// һ������ָ��W ���� w
	while (1)
	{
		ptr = (u8 *)strstr((char *)ptr, ")=");
		if (ptr != NULL)
		{
			if (Lw_GprsTmp_Com1 == 0) // ��ӡ�����з����÷ֱ���Ϣ
				Com1_printf("\r\n-------------------Result info\r\n");
			// 1. �ϳ���������������Ӧ�ַ�λ����������һ��ʹ��
			// ���յ����ַ��� �磺GPW+26,42;		0x32��0x36�ϳ�16����ָ��ֵ0x26
			CommandNo = Str2Hex(ptr - 2);
			if (CommandNo <= 0x63)
				n = FindFirstValue(CmdNo, CommandNo, 0x00); // n:�ҵ�ָ��ֵ��GPRS �����������е�λ��
			i = 1;											// ���ҵ�һ��'='
			r = FindFirstValue(ptr, 0x0D, 0x00);			// 0x0D=�س�		//���ҽ�����';'

			// 2.����ָ��Ĵ���; ZCL ��� 2012.9.23
			// n==255 û���ҵ������CommandNo>0x90 ����������ڵ�����
			if (n == 255 || i == 255 || r == 255 || CommandNo > 0x90)
			{
				// д�Ĵ���Ŵ�1��ʼ ������1-9��
				// �ô����־ err=1
				err = 1;
				Com1_printf("CommandNo Error!\r\n");
				// break;  �ں�����err��break����
			}

			// �ܳ��� 2013.2.2 	�д�������
			ptr = ptr + 2; // ָ�����ݵĵ�ַ
			if (err)
				;

			// 5. IP��ַ: ���͹�����222.173.103.226 ��Ϊ��'.'��Ҫȥ��
			// 0x20,0x30,0x40,0x50 ��IP��ַ, 0x62��DNS��ַ
			else if (CommandNo == 0x20 || CommandNo == 0x30 || CommandNo == 0x40 || CommandNo == 0x50 || CommandNo == 0x62)
			{
				// ���յ����ַ��� �磺0x32,0x32,0x32=222���ϳ�10����ֵ222(���ڴ��б���Ϊ0xDE)
				for (s = 0; s < 4; s++)
				{
					ip[s] = Str2Bcd(ptr); // ��һ��λ��
					if (s == 0 && ip[s] == 0)
					{
						ip[1] = 0;
						ip[2] = 0;
						ip[3] = 0;
						s = 3; // 2013.6.4 ���IP����Ϊ��ʱ��ʾ��������
					}
					if (s == 3)
						break; // �����û��'.'��
					// ���ҵ�һ������'.'
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
						Com1_printf("IP Address Set Error!\r\n");
						// 2013.1.24 �ܳ��� �˳�forѭ��
						break;
					}
				}
			}

			// 6. �ַ��������ã�������ֵ�� �磺0x05��CMNET
			// �������:01		���������:05		DTU���ʶ����:06		����:21.31.41.51
			else if (CommandNo == 0x01 || CommandNo == 0x05 || CommandNo == 0x06 || CommandNo == 0x21 || CommandNo == 0x31 || CommandNo == 0x41 || CommandNo == 0x51)
			{
				if (CommandNo == 0x01 && r - i - 1 > 8)
					err = 3;
				else if (CommandNo == 0x05 && r - i - 1 > 24)
					err = 3;
				else if (CommandNo == 0x06 && r - i - 1 > 11)
					err = 3;
				else if (CommandNo == 0x21 && r - i - 1 > 30)
					err = 3;
				else if (CommandNo == 0x31 && r - i - 1 > 30)
					err = 3;
				else if (CommandNo == 0x41 && r - i - 1 > 30)
					err = 3;
				else if (CommandNo == 0x51 && r - i - 1 > 30)
					err = 3;
				if (err == 3)
				{
					// ����������Ķ������ݣ�׼�����Ͷ���
					// ��������Ĵ���
					Com1_printf("Par Item 0x%x: String Length Overlong Error! \r\n", CommandNo);
				}
				else
				{
					// r(0x0D=�س� λ��)  i(=λ��)
					for (s = 0; s < (r - i - 1); s++)
					{
						GprsPar[CmdPst[n] + s] = *(ptr + s);
					}
					// 2012.9.20 �ܳ��� �ַ�����ĩβ��0
					GprsPar[CmdPst[n] + s] = 0;
				}
			}

			// 8. ���ڵ����ã�0x0Dָ�������  0x00,0x00,0x25,0x80 = 9600
			//  Ŀǰ����ֻ֧�����һ���ֵĲ��������ã�ֻ֧�ֵ�0-65535
			else if (CommandNo == 0x0D)
			{
				// 8.1 ������
				// ���յ����ַ��� �磺0x39,0x36,0x30,0x30=9600���ϳ�10����ֵ(���ڴ��б���Ϊ0x25,0x80)
				// ������ֵ����Ҫ����,�ϲ���������
				w1 = Str2Bcd(ptr); // ��һ��λ��
				if (w1 == 1200 || w1 == 2400 || w1 == 4800 || w1 == 9600 || w1 == 19200 || w1 == 38400 || w1 == 57600)
				{
					GprsPar[CmdPst[n] + 2] = w1 >> 8; // ���ֽ�
					GprsPar[CmdPst[n] + 3] = w1;	  // ���ֽ�
													  // ��Com1_BaudRateSet()���޸�Pw_GprsBaudRate2
				}
				else if (w1 == 0) // �ܳ��� 2013.12.18 ���ֵΪ�գ�ά��ԭֵ
				{
					;
				}
				else
				{
					// ��������Ĵ���
					err = 4;
					Com1_printf("Com1 Baudrate Set Error!\r\n");
				}

				// 8.2 ����λ ����λΪ8λ��00-5�� 01-6�� 02-7�� 03-8��	������ֵ5,6,7,8Ҫ����� 0,1,2,3
				// ������һ����',' [0x2C:',']
				i = FindFirstValue(ptr, 0x2C, 0x00); // 0x2C=,
				if (i == 255)
				{
					w1 = 255;
					goto Xmodem_SetBaudItemLack_END_Com1; // 2013.12.18
				}
				else
				{
					ptr = ptr + i + 1;
					w1 = Str2Bcd(ptr); // ��һ��λ��
				}

				if (w1 <= 8 && w1 >= 5)
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
					Com1_printf("Com1 DataBitLength Set Error!\r\n");
				}

				// 8.3 ֹͣλ ֹͣλΪ1λ��00-1��04-2��	������ֵ1,2Ҫ����� 0,0x04
				// ������һ����',' [0x2C:',']
				i = FindFirstValue(ptr, 0x2C, 0x00); // 0x2C=,
				if (i == 255)
				{
					w1 = 255;
					goto Xmodem_SetBaudItemLack_END_Com1; // 2013.12.18
				}
				else
				{
					ptr = ptr + i + 1;
					w1 = Str2Bcd(ptr); // ��һ��λ��
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
					Com1_printf("Com1 StopBitLength Set Error!\r\n");
				}

				// 8.4 ��żУ�� У��λ-�ޣ�00-��0��08-��1��18-ż2��	������ֵ0,1,2Ҫ����� 0��0x08��0x18
				// ������һ����',' [0x2C:',']
				i = FindFirstValue(ptr, 0x2C, 0x00); // 0x2C=,
				if (i == 255)
				{
					w1 = 255;
					goto Xmodem_SetBaudItemLack_END_Com1; // 2013.12.18
				}
				else
				{
					ptr = ptr + i + 1;
					w1 = Str2Bcd(ptr); // ��һ��λ��
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
				else
				{
					// ��������Ĵ���
					err = 7;
					Com1_printf("Com1 ParityBitLength Set Error!\r\n");
				}

				// 8.5 ����-�����أ�01-Xon/Xoff1��02-Ӳ����2��03-������3��04-��˫��485 4��05-ȫ˫��422 5��
				// ������ֵ����Ҫ����
				// ������һ����',' [0x2C:',']
				i = FindFirstValue(ptr, 0x2C, 0x00); // 0x2C=,
				if (i == 255)
				{
					w1 = 255;
					goto Xmodem_SetBaudItemLack_END_Com1; // 2013.12.18
				}
				else
				{
					ptr = ptr + i + 1;
					w1 = Str2Bcd(ptr); // ��һ��λ��
				}
				if (w1 <= 5 && w1 > 0)
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
					Com1_printf("Com1 FlowControl Set Error!\r\n");
				}

			Xmodem_SetBaudItemLack_END_Com1: // ���ò�������������ȱ�ٴ���GPW+0D,9600,8,1,2,3;
				if (w1 == 255 && err == 0)
				{
					// ��������Ĵ���	//���ô��п���ȱ�ٴ���
					err = 9;
					Com1_printf("Serial Port Item Lack Error!\r\n");
				}
			}

			// 9. ˫�ֽڲ����͵��ֽڲ�������ֵ������ 0-65535��Χ��
			else
			{
				// ����ָ���ǲ���˫�ֽڲ���
				s = FindFirstValue(GprsCmd2Byte, CommandNo, 0x00);
				// ������ֵ
				w1 = Str2Bcd(ptr); // ��һ��λ��
				// ˫�ֽڲ���
				if (s != 255) // s>0 && 		//2013.6.4�����Ӧ��Ҫ
				{
					GprsPar[CmdPst[n] + 0] = w1 >> 8;
					GprsPar[CmdPst[n] + 1] = w1;
				}
				// ���ֽڲ���
				else
				{
					GprsPar[CmdPst[n] + 0] = w1;
				}
			}

			// �ܳ��� ���ִ����������������ټ����һ�����
			if (err)
			{
				Com1_printf("Xmodem File Transfer err= %d  !!!\r\n", err);
				Com1_printf("Please restart File Transfer !!!\r\n");
				break;
			}
			else
				Lw_GprsTmp_Com1++;
		}

		// û��������")="������ѭ�� 2013.6.4
		else
		{
			Com1_printf("Xmodem set end��set par item %d times! OK\r\n", Lw_GprsTmp_Com1);
			break;
		}
	}
}

// /* CRC16 implementation acording to CCITT standards */
// u16 Crc16_ccitt(const char *buf, int len) // Crc16		ZCL: Ӧ��ͬ���ڵ�CRC16У�飬д����̫һ��
// {
// 	int counter;
// 	u16 crc = 0;
// 	for (counter = 0; counter < len; counter++)
// 		crc = (crc << 8) ^ crc16tab[((crc >> 8) ^ *(char *)buf++) & 0x00FF];
// 	return crc;
// }
