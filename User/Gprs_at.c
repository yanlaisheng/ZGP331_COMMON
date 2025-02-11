/**
 ******************************************************************************
 * @file    Gprs_at.c
 * @author  ChengLei Zhou  - �ܳ���
 * @version V1.27
 * @date    2014-01-03
 * @brief   AT COMMAND
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "Gprs_at.h"
#include "GlobalV_Extern.h" // ȫ�ֱ�������
#include "GlobalConst.h"
#include "com3_232.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "math.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private variables Extern---------------------------------------------------*/
extern uchar Txd3Buffer[];     // ���ͻ�����
extern uint Cw_Txd3Max;        // �ж��ٸ��ַ���Ҫ����//
extern uchar StringBuffer[12]; // BCD��ת���ַ������档��Ҫ��IP��ַ�ͳ��Ƚ���ת��
extern uchar S_M35;            // s���裺M35����
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern void Com2_printf(char *fmt, ...);

/*ͨ��ATָ�� */
void At_AT(void) // ����AT�ַ�����ͬ��������
{
  // Com2_printf("AT");
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT"); // YLS ��ͬ����
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_ATE(u8 i) // �������ģʽ  =1���û��ԣ�Ĭ�ϣ�=0������(����)
{
  // Com2_printf("ATE0");
  Cw_Txd3Max = 0;
  Gprs_TX_String("ATE"); // YLS ��ͬ����
  Gprs_TX_Byte(i);
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_CSQ(void) // ����ź�ǿ��
{
  // Com2_printf("AT+CSQ");
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT+CSQ"); // YLS ��ͬ����
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_CREG(void) // ��ѯGSMע��״̬  ����1,5 ��ע��
{
  // Com2_printf("AT+CREG?");
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT+CREG?"); // 0D,0A, +CREG: 0,1		YLS ��ͬ����
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_ThreePlusSign(void) // �˳�͸������������ģʽ +++
{
  Cw_Txd3Max = 0;
  Gprs_TX_String("+++"); // YLS ��ͬ����
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_ATO0(void) // �˳������������͸��ģʽ
{
  Cw_Txd3Max = 0;
  //  Gprs_TX_String("AT+ATO0");
  Gprs_TX_String("AT+ATO");
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_ATI(void) // ��ʾ��Ʒ��Ϣ
{
  Cw_Txd3Max = 0;
  Gprs_TX_String("ATI"); // YLS ��ͬ����
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_ATQ0(void) // ���ý������ʾģʽ  0=��TE���ͽ����(Ĭ��)��1=������
{
  Cw_Txd3Max = 0;
  Gprs_TX_String("ATQ0");
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_ATAndF(void) // ��������TA����Ϊ��������
{
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT&F");
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_ATAndW(void) // �����û����ò���
{
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT&W");
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_ATV1(void) // ����TA��Ӧ�������ݸ�ʽ�� =0�����룻=1��ϸ��(Ĭ��)
{
  Cw_Txd3Max = 0;
  Gprs_TX_String("ATV1");
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_QPOWD(void) // �����ػ�
{
  Cw_Txd3Max = 0;
  //  Gprs_TX_String("AT+QPOWD=1");
  Gprs_TX_String("AT+CPOF"); // �ػ�
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_IPR(void) // ���ò����ʣ���=0��Ϊ����Ӧ������
{
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT+IPR=115200");
  // Gprs_TX_String("AT+IPR=57600&W");
  //	Gprs_TX_String("AT+IPRX=57600");	//YLS 2022.12.25
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

/*	SMS��������	*/
void At_CPMS(void) // ѡ�����Ϣ�洢��
{
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT+CPMS="); // AT+CPMS="SM","SM","SM" 	//YLS ��ͬ����
  Gprs_TX_Byte('"');
  Gprs_TX_String("SM");
  Gprs_TX_Byte('"');
  Gprs_TX_Byte(',');
  Gprs_TX_Byte('"');
  Gprs_TX_String("SM");
  Gprs_TX_Byte('"');
  Gprs_TX_Byte(',');
  Gprs_TX_Byte('"');
  Gprs_TX_String("SM");
  Gprs_TX_Byte('"');
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_CMGF(u8 i) // ���ö���Ϣģʽ =0 PDU�� =1 �ı�
{
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT+CMGF="); // YLS ��ͬ����
  Gprs_TX_Byte(i);
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_CSCS(void) // ѡ�� TE�ַ���
{
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT+CSCS="); // YLS ��ͬ����
  Gprs_TX_Byte('"');
  Gprs_TX_String("GSM");
  Gprs_TX_Byte('"');
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_CNMI(void) // ������ʾ����
{
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT+CNMI=2,1,0,0,0"); // YLS ��ͬ����
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_CSMP(void) // �����ı���ʽ����
{
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT+CSMP=17,169,0,241"); // YLS ��ͬ����
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_CSDH(void) // ��ʾ�ı���ʽ���� =0��ʾ���ֲ��� =1��ʾȫ������
{
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT+CSDH=0"); // YLS ��ͬ����
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_CPMSInq(void) // ȷ�϶���Ϣ�洢��,����ѯ����
{
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT+CPMS?"); // AT+CPMS?	//YLS ��ͬ����
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_CMGR(uint i) // ��ȡ����Ϣ,ָ���洢λ��
{
  uchar j, k;
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT+CMGR="); // YLS ��ͬ����
  j = Bcd2Str(i);             // 1����ת10�����ַ��� ��:0xDE ת�� 222
  for (k = 0; k < j; k++)
  {
    Txd3Buffer[Cw_Txd3Max++] = StringBuffer[k];
  }
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_CMGD(uchar i) // ɾ������Ϣ AT+CMGD=1,4(ɾ����ǰ�洢����ȫ������)
{
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT+CMGD=1,"); // �ܳ���ע��1,4�����ã��ĳ�0,4�ź���  2013.5.14 !!!
  Gprs_TX_Byte(i);
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_QSCLK(u8 i) // ��ʱ������  =1������ʱ��
{
  Cw_Txd3Max = 0;
  //  Gprs_TX_String("AT+QSCLK=");
  Gprs_TX_Byte(i);
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

/*	GPRS��������	 TCP��·͸������ */
void At_CFUN(void) // ����ģ�����й��ܿ�����	�ڶ�������=0������λ������Ч��
{
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT+CFUN=1"); // ��һ������=0����С���ܣ�=1�����(Ĭ��)��=4���ر���Ƶ
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_CPIN(void) // ȷ��PIN���ѽ⡣ZCL:һ�㲻��Ҫ
{
  // Com2_printf("AT+CPIN=?");
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT+CPIN=?");
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_CGATT(void) // GPRS����
{
  // Com2_printf("AT+CGATT=1");
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT+CGATT=1");
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_QIFGCNT(u8 i) // ����ǰ�ó��� ����ֵ: 0-1  P142
{
  Cw_Txd3Max = 0;
  //  Gprs_TX_String("AT+QIFGCNT=");
  Gprs_TX_Byte(i);
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_QICSGP(void) // ����GPRS���ӷ�ʽ 0:CSD����  1:GPRS����
{
  Cw_Txd3Max = 0;
  //  Gprs_TX_String("AT+QICSGP=1,");
  Gprs_TX_Byte('"');
  Gprs_TX_String("CMNET");
  Gprs_TX_Byte('"');
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_QIDNSIP(u8 i) // ���÷�������ַ��ʽ =0 IP����; =1 ��������
{
  Cw_Txd3Max = 0;
  //  Gprs_TX_String("AT+QIDNSIP=");
  Gprs_TX_Byte(i);
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_QINDI(void) // �鿴���ݽ��շ�ʽ1: ��Ҫ������ȡ����
{
  Cw_Txd3Max = 0;
  //  Gprs_TX_String("AT+QINDI?");
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_QISHOWRA(u8 i) // �鿴������ʾ��ʽ 1: ����IP���˿ں�
{
  Cw_Txd3Max = 0;
  //  Gprs_TX_String("AT+QISHOWRA=");
  Gprs_TX_Byte(i);
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_QIHEAD(u8 i) // �鿴������ʾ��ʽ 1: ����IPD<Len>
{
  Cw_Txd3Max = 0;
  //  Gprs_TX_String("AT+QIHEAD=");
  Gprs_TX_Byte(i);
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_QISHOWPT(u8 i) // �鿴������ʾ��ʽ 1: ����Э������
{
  Cw_Txd3Max = 0;
  //  Gprs_TX_String("AT+QISHOWPT=");
  Gprs_TX_Byte(i);
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_QIMUX(u8 i) // 0:��·����   1: ��·����
{
  Cw_Txd3Max = 0;
  //  Gprs_TX_String("AT+QIMUX=");
  // Gprs_TX_String("AT+CMUX=");
  Gprs_TX_Byte(i);
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_QIMODE(u8 i) //=1ʹ��͸��ģʽ
{
  // Com2_printf("AT+CIPMODE=0");
  Cw_Txd3Max = 0;
  //  Gprs_TX_String("AT+QIMODE=");
  Gprs_TX_String("AT+CIPMODE="); // YLS 2022.12.24
  Gprs_TX_Byte(i);
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_QITCFG(void) // �յ��������ݺ�ȴ�600ms���ͣ��򴮿����ݴﵽ512�ֽں�
{                    // �������͡� <����͸������ģʽ >
  Cw_Txd3Max = 0;
  //  Gprs_TX_String("AT+QITCFG=3,6,512,1");
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_QIMuxOPEN(u8 i) // ������· TCP���ӻ�ע��UDP�˿ں�  (0-5������)
{                       //"TCP", "222.173.103.226",7500		��˾
  uchar j, k, m;
  uint w = 0;
  const u8 *modetbl[2] = {"UDP", "TCP"}; // ����ģʽ//modetbl:1:TCP����;0,UDP����)
  u8 p_TmpBuf[512] = {0};                // ����1
  u8 localPortBuf[5] = {0};              // ���ض˿ڻ���
  u8 ipbuf[15];                          // IP����
  u8 portBuf[5] = {0};                   // Զ�˶˿ڻ���

  Pw_Link1localPort = (GprsPar[LoclaPort0Base + i * 2] << 8) + GprsPar[LoclaPort0Base + i * 2 + 1]; // YLS 2022.12.27
  sprintf((char *)localPortBuf, "%d", Pw_Link1localPort);                                           // ��һ·���ӱ��ض˿���ת��Ϊascii��
  //			sprintf((char *)ipbuf, "%d.%d.%d.%d", Pw_Link1IP1, Pw_Link1IP2, Pw_Link1IP3, Pw_Link1IP4); //���IP
  sprintf((char *)ipbuf, "%d.%d.%d.%d", GprsPar[Ip0Base + i * 4], GprsPar[Ip0Base + i * 4 + 1], GprsPar[Ip0Base + i * 4 + 2], GprsPar[Ip0Base + i * 4 + 3]); // ���IP
  Pw_Link1Port = (GprsPar[Port0Base + i * 2] << 8) + GprsPar[Port0Base + i * 2 + 1];                                                                         // YLS 2022.12.27
  sprintf((char *)portBuf, "%d", Pw_Link1Port);                                                                                                              // ��һ·����Զ�˶˿���ת��Ϊascii��
  if (GprsPar[LinkTCPUDP0Base + i] == 0)                                                                                                                     // �ڶ�·ΪUDP
  {
    sprintf((char *)p_TmpBuf, "AT+CIPOPEN=0,\"%s\",\"%s\",%s,%s", modetbl[(u8)GprsPar[LinkTCPUDP0Base + i]], ipbuf, portBuf, localPortBuf);
  }
  else
  {
    sprintf((char *)p_TmpBuf, "AT+CIPOPEN=0,\"%s\",\"%s\",%s", modetbl[(u8)GprsPar[LinkTCPUDP0Base + i]], ipbuf, portBuf); // ��������Ϣ������Ŀռ�p_TmpBuf
  }
  // Com2_printf("%s\r\n", p_TmpBuf);
  Cw_Txd3Max = 0;

  //  Gprs_TX_String("AT+QIOPEN=");		//"TCP", "119.75.218.77",80		�ٶ�
  Gprs_TX_String("AT+CIPOPEN="); //"TCP", "119.75.218.77",80		�ٶ�	YLS 2022.12.24
  Gprs_TX_Byte(i + 0x30);        // ���Ӻ�
  Gprs_TX_Byte(',');

  Gprs_TX_Byte('"');
  if (GprsPar[LinkTCPUDP0Base + i] == 0) // ��������
    Gprs_TX_String("UDP");
  else
    Gprs_TX_String("TCP");
  Gprs_TX_Byte('"');
  Gprs_TX_Byte(',');

  Gprs_TX_Byte('"');

  // ��������ַ��Ϊ0��IP��ַ������Ϊ����
  // IP��ַ��BCD��ת���ַ����������
  if (GprsPar[Ip0Base + i * 4]) // IP��ַ
  {
    for (m = 0; m < 4; m++) // 4���ֽ� ��DE AD 67 E2��2012.8.20
    {
      w = GprsPar[Ip0Base + i * 4 + m]; // ȡ��һ��ֵ(�ֽ�)
      j = Bcd2Str(w);                   // 1����ת10�����ַ��� ��:0xDE ת�� 222
      for (k = 0; k < j; k++)
      {
        Txd3Buffer[Cw_Txd3Max++] = StringBuffer[k];
      }
      if (m < 3)
        Txd3Buffer[Cw_Txd3Max++] = '.';
    }
  }
  // �������ݣ����
  // else
  // {
  //   k = 0;
  //   while (GprsPar[DomainName0Base + i * 31 + k] != 0)
  //   {
  //     Txd3Buffer[Cw_Txd3Max++] = GprsPar[DomainName0Base + i * 31 + k];
  //     k++;
  //   }
  // }
  Gprs_TX_Byte('"');
  Gprs_TX_Byte(',');

  // �˿�2���ֽ���Ҫ�ϳ�������BCD��ת���ַ��� 2012.8.21
  w = GprsPar[Port0Base + i * 2]; // ���ܳ��� 2012.9.11 ������2 ע��
  w = (w << 8) + GprsPar[Port0Base + i * 2 + 1];
  j = Bcd2Str(w); // 1����ת10�����ַ��� ��:0x1D4C ת�� 7500
  for (k = 0; k < j; k++)
  {
    Txd3Buffer[Cw_Txd3Max++] = StringBuffer[k];
  }

  // UDPģʽ����䱾�ض˿�
  if (GprsPar[LinkTCPUDP0Base + i] == 0) // ��������
  {
    Gprs_TX_Byte(',');
    // �˿�2���ֽ���Ҫ�ϳ�������BCD��ת���ַ��� 2012.8.21
    w = GprsPar[LoclaPort0Base + i * 2]; // ���ܳ��� 2012.9.11 ������2 ע��
    w = (w << 8) + GprsPar[LoclaPort0Base + i * 2 + 1];
    j = Bcd2Str(w); // 1����ת10�����ַ��� ��:0x1D4C ת�� 7500
    for (k = 0; k < j; k++)
    {
      Txd3Buffer[Cw_Txd3Max++] = StringBuffer[k];
    }
  }

  // ����س���
  Gprs_TX_Byte(0x0d);
  // ��������
  Gprs_TX_Start();
}

// ����������ѯ����
void At_DomainName(u8 i)
{
  u8 k;

  Gprs_TX_String("AT+CDNSGIP ="); //"TCP", "119.75.218.77",80		�ٶ�	YLS 2022.12.24
  Gprs_TX_Byte('"');
  k = 0;
  while (GprsPar[DomainName0Base + i * 31 + k] != 0)
  {
    Txd3Buffer[Cw_Txd3Max++] = GprsPar[DomainName0Base + i * 31 + k];
    k++;
  }
  Gprs_TX_Byte('"');
  // ����س���
  Gprs_TX_Byte(0x0d);
  // ��������
  Gprs_TX_Start();

  Com2_printf("Send Data��%s\r\n", Txd3Buffer);
}

/* void At_QISEND(u8 LinkNo)			//DTU���ݷ���,�����������ʹ��(CTRL+Z)���� ʵ��Ϊ��0x1A
{
  Gprs_TX_String("AT+QISEND=");
  Gprs_TX_Byte(LinkNo+0x30);
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
} */

void At_QISENDAccordingLength(u8 LinkNo, uint Length) // DTU ���ݷ��Ͱ��ճ���
{
  uchar k;
  u8 ipbuf[15];              // IP����
  u8 portBuf[5] = {0};       // Զ�˶˿ڻ���
  u8 p_SendTmpBuf[50] = {0}; // ����
  u16 LinkIP1;
  u16 LinkIP2;
  u16 LinkIP3;
  u16 LinkIP4;
  u16 LinkPort;
  u8 LinkTCP_UDPType;
  // ������(DSC)��IP��ַ
  if (LinkNo == 0)
  {
    LinkIP1 = GprsPar[Ip0Base];
    LinkIP2 = GprsPar[Ip0Base + 1];
    LinkIP3 = GprsPar[Ip0Base + 2];
    LinkIP4 = GprsPar[Ip0Base + 3];
    LinkPort = (GprsPar[Port0Base] << 8) + GprsPar[Port0Base + 1];
    LinkTCP_UDPType = GprsPar[LinkTCPUDP0Base];
  }
  else if (LinkNo == 1)
  {
    LinkIP1 = GprsPar[Ip1Base];
    LinkIP2 = GprsPar[Ip1Base + 1];
    LinkIP3 = GprsPar[Ip1Base + 2];
    LinkIP4 = GprsPar[Ip1Base + 3];
    LinkPort = (GprsPar[Port1Base] << 8) + GprsPar[Port1Base + 1];
    LinkTCP_UDPType = GprsPar[LinkTCPUDP1Base];
  }
  if (LinkNo == 2)
  {
    LinkIP1 = GprsPar[Ip2Base];
    LinkIP2 = GprsPar[Ip2Base + 1];
    LinkIP3 = GprsPar[Ip2Base + 2];
    LinkIP4 = GprsPar[Ip2Base + 3];
    LinkPort = (GprsPar[Port2Base] << 8) + GprsPar[Port2Base + 1];
    LinkTCP_UDPType = GprsPar[LinkTCPUDP2Base];
  }
  if (LinkNo == 3)
  {
    LinkIP1 = GprsPar[Ip3Base];
    LinkIP2 = GprsPar[Ip3Base + 1];
    LinkIP3 = GprsPar[Ip3Base + 2];
    LinkIP4 = GprsPar[Ip3Base + 3];
    LinkPort = (GprsPar[Port3Base] << 8) + GprsPar[Port3Base + 1];
    LinkTCP_UDPType = GprsPar[LinkTCPUDP3Base];
  }

  Cw_Txd3Max = 0;

  // Gprs_TX_String("AT+CIPSEND="); // YLS 2022.12.24
  // Gprs_TX_Byte(LinkNo + 0x30);
  // Gprs_TX_Byte(',');
  // // ���ͳ��ȣ�ת���ַ���
  // j = Bcd2Str(Length); // 1����ת10�����ַ��� ��:0xDE ת�� 222
  // for (k = 0; k < j; k++)
  // {
  //   Txd3Buffer[Cw_Txd3Max++] = StringBuffer[k];
  // }

  sprintf((char *)ipbuf, "%d.%d.%d.%d", LinkIP1, LinkIP2, LinkIP3, LinkIP4); // ���IP
  sprintf((char *)portBuf, "%d", LinkPort);                                  // ��һ·����Զ�˶˿���ת��Ϊascii��
  if (LinkTCP_UDPType == 0)                                                  // ��һ·ΪUDP
  {
    sprintf((char *)p_SendTmpBuf, "AT+CIPSEND=%d,%d,\"%s\",%s", LinkNo, Length, ipbuf, portBuf); // ��������Ϣ������Ŀռ�p_LinkTmpBuf
  }
  else if (LinkTCP_UDPType == 1) ////��һ·ΪTCP Client
  {
    sprintf((char *)p_SendTmpBuf, "AT+CIPSEND=%d,%d", LinkNo, Length);
  }
  // Com2_printf("%s\r\n", p_SendTmpBuf);
  for (k = 0; k < strlen((char *)p_SendTmpBuf); k++)
  {
    Txd3Buffer[Cw_Txd3Max++] = p_SendTmpBuf[k];
  }

  // �س���
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_QISACK(void) // ��������Ƿ��ͳɹ�  +QISACK: 4,4,0 ˵�����ͳ��Ⱥ���ȷ��
{                    // ���ͳɹ������ݶ���4��������ϣ�0��ʾδ���ͳɹ������ݸ���
  Cw_Txd3Max = 0;
  //  Gprs_TX_String("AT+QISACK");
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_QICLOSE(u8 i) // �رյ�ǰָ��������
{
  // printf("S_M35=%d\r\n", S_M35);
  // Com2_printf("AT+CIPCLOSE=");
  Cw_Txd3Max = 0;
  //  Gprs_TX_String("AT+QICLOSE=");
  Gprs_TX_String("AT+CIPCLOSE="); // YLS 2022.12.24
  Gprs_TX_Byte(i + 0x30);
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_NETOPEN(void) // ������
{
  // printf("S_M35=%d\r\n", S_M35);
  // Com2_printf("AT+NETOPEN");
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT+NETOPEN"); // YLS 2022.12.24
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_NETCLOSE(void) // �ر�����
{
  // printf("S_M35=%d\r\n", S_M35);
  // Com2_printf("AT+NETCLOSE");
  Cw_Txd3Max = 0;
  Gprs_TX_String("AT+NETCLOSE"); // YLS 2022.12.24
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_QIDEACT(void) // �ر�GPRS������֮�����͸������·�Ȳ�����������ִ���
{
  Cw_Txd3Max = 0;
  //  Gprs_TX_String("AT+QIDEACT");
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

void At_QISTAT(void) // ��ѯ����״̬
{
  Cw_Txd3Max = 0;
  //  Gprs_TX_String("AT+QISTAT");
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

// ZCL 2018.12.10
void At_QIURC(void) // ��ʼ�������ʾ�ϱ�
{
  Cw_Txd3Max = 0;
  //  Gprs_TX_String("AT+QIURC=1");
  Gprs_TX_Byte(0x0d);
  Gprs_TX_Start();
}

/****END OF FILE****/
