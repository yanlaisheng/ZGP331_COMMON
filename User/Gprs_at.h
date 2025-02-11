/**
 ******************************************************************************
 * @file    Gprs_at.h
 * @author  ChengLei Zhou  - �ܳ���
 * @version V1.27
 * @date    2014-01-03
 * @brief   AT COMMAND
 ******************************************************************************
 */

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __GPRS_AT_H
#define __GPRS_AT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void At_AT(void);            // ����AT�ַ�����ͬ��������
void At_ATE(u8 i);           // �������ģʽ  =1���û��ԣ�Ĭ�ϣ�=0������(����);
void At_CSQ(void);           // ����ź�ǿ��
void At_CREG(void);          // ��ѯGSMע��״̬  ����1,5 ��ע��
void At_ThreePlusSign(void); // �˳�͸������������ģʽ +++
void At_ATO0(void);          // �˳������������͸��ģʽ
void At_ATI(void);           // ��ʾ��Ʒ��Ϣ
void At_ATQ0(void);          // ���ý������ʾģʽ  0=��TE���ͽ����(Ĭ��);1=������
void At_ATAndF(void);        // ��������TA����Ϊ��������
void At_ATAndW(void);        // �����û����ò���
void At_ATV1(void);          // ����TA��Ӧ�������ݸ�ʽ�� =0�����룻=1��ϸ��(Ĭ��);
void At_QPOWD(void);         //=0�����ػ���=1�����ػ�
void At_IPR(void);           // ���ò����ʣ���=0��Ϊ����Ӧ������
void At_CPMS(void);          // ѡ�����Ϣ�洢��
void At_CMGF(u8 i);          // ���ö���Ϣģʽ =0 PDU�� =1 �ı�
void At_CSCS(void);          // ѡ�� TE�ַ���
void At_CNMI(void);          // ������ʾ����
void At_CSMP(void);          // �����ı���ʽ����
void At_CSDH(void);          // ��ʾ�ı���ʽ���� =0��ʾ���ֲ��� =1��ʾȫ������
void At_CPMSInq(void);       // ȷ�϶���Ϣ�洢��,����ѯ����
void At_CMGR(uint i);        // ��ȡ����Ϣ,ָ���洢λ��
void At_CMGD(uchar i);       // ��ȡ����Ϣ,ָ���洢λ��
void At_QSCLK(u8 i);         // ��ʱ������  =1������ʱ��
void At_CFUN(void);          // ����ģ�����й��ܿ�����	�ڶ�������=0������λ������Ч��
void At_CPIN(void);          // ȷ��PIN���ѽ⡣ZCL:һ�㲻��Ҫ
void At_CGATT(void);         // GPRS����
void At_QIFGCNT(u8 i);       // ����ǰ�ó��� ����ֵ: 0-1  P142
void At_QICSGP(void);        // ����GPRS���ӷ�ʽ 0:CSD����  1:GPRS����
void At_QIDNSIP(u8 i);       // ���÷�������ַ��ʽ =0 IP����; =1 ��������
void At_QINDI(void);         // �鿴���ݽ��շ�ʽ1: ��Ҫ������ȡ����
void At_QISHOWRA(u8 i);      // �鿴������ʾ��ʽ 1: ����IP���˿ں�
void At_QIHEAD(u8 i);        // �鿴������ʾ��ʽ 1: ����IPD<Len>
void At_QISHOWPT(u8 i);      // �鿴������ʾ��ʽ 1: ����Э������
void At_QIMUX(u8 i);         // 0:��·����   1: ��·����
void At_QIMODE(u8 i);        //=1ʹ��͸��ģʽ
void At_QITCFG(void);        // �յ��������ݺ�ȴ�600ms���ͣ��򴮿����ݴﵽ512�ֽں�
                      // �������͡� <����͸������ģʽ >
void At_QIMuxOPEN(u8 i); // ������· TCP���ӻ�ע��UDP�˿ں�  (0-5������)
                         //"TCP", "222.173.103.226",7500		��˾
void At_QISENDAccordingLength(u8 LinkNo, uint Length); // DTU ���ݷ��Ͱ��ճ���
void At_QISACK(void);                                  // ��������Ƿ��ͳɹ�  +QISACK: 4,4,0 ˵�����ͳ��Ⱥ���ȷ��
void At_QICLOSE(u8 i);                                 // �رյ�ǰָ��������
void At_NETCLOSE(void);                                // �ر�����
void At_NETOPEN(void);                                 // ������
void At_QIDEACT(void);                                 // �ر�GPRS������֮�����͸������·�Ȳ�����������ִ���
void At_QISTAT(void);                                  // ��ѯ����״̬

void At_QIURC(void); // ��ʼ�������ʾ�ϱ�  ZCL 2018.12.10

// ���ͻ�����д��һ���ֽ�
void Gprs_TX_Byte(u8 i);
// ���ͻ�����д��һ���ַ���
void Gprs_TX_String(u8 *String);
void Gprs_TX_Fill(uc8 *String, u8 Length);
// ��������
void Gprs_TX_Start(void);
// �����ַ�������
uchar Gprs_String_Length(u8 *String);
void At_DomainName(u8 i);
#endif /* __GPRS_AT_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
