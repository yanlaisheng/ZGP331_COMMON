#ifndef __SIM7600CE_H__
#define __SIM7600CE_H__
#include "GlobalConst.h"
#include "GlobalV_Extern.h"

extern u8 Scan_Wtime;
extern u8 sim_ready;      // SIM������״̬=1������=0δ�ҵ�
extern u8 F_RunAgain;     // �������б�־
extern u8 p_TmpBuf[];     // ����1
extern u8 p1_TmpBuf[];    // ����2
extern u8 ModularADDIP[]; // ģ�鱾���IP��ַ
extern u8 domain_name[];
extern u8 domain_name1[];
extern u8 domain_name2[];
extern u8 domain_name3[];

void sim_at_response(u8 mode);
u8 *sim7600ce_check_cmd(u8 *str);
u8 sim7600ce_send_cmd(u8 *cmd, u8 *ack, u16 waittime);
u8 sim7600ce_wait_request(u8 *request, u16 waittime);
u8 sim7600ce_chr2hex(u8 chr);
u8 sim7600ce_hex2chr(u8 hex);
void sim7600ce_unigbk_exchange(u8 *src, u8 *dst, u8 mode);

void sim7600ce_connectstaModularNum(void);                                   // ���ģ����Ҫ���ӵ�IP����
u8 sim7600ce_send_Modular_IP(u8 *cmd, u16 waittime);                         // ���ģ��IP
u8 GPRSNetConfig(u8 link_num);                                               // ����������Ϣ
void sim7600ceGPRSUpLoaded(u8 *SendDateBuf, u16 SendBufLen, u8 Star_OR_hex); // GPRS���ݼ���
void sim7600ce_Modular_Information(void);                                    // ģ����Ϣ��ʾ
u8 sim7600ce_GSM_Information(void);                                          // ��ʾGSMģ����Ϣ
void sim7600ce_Netlink_Information(void);                                    // ��ʾ����������Ϣ
void ntp_update(void);                                                       // ����ͬ��ʱ��
void sim7600ce_GPRSRUN(void);                                                // SIM7600ce�����к���
void SIM7600CE_GPIO_Init(void);                                              // SIM7600ce��GPIO��ʼ��
void SIM7600ce_Init(void);                                                   // SIM7600ce��ʼ��
void SIM7600ce_NetworkCheck(void);                                           // SIM7600ce���й�����ÿ��һ��ʱ��������
void sim7600ce_ReconnectNet(void);                                           // GPRSģ������ʧ�ܺ���������
void sim7600ce_SendHeartbeat(void);                                          // GPRSģ�鷢����������
void sim7600ce_ReceiveData(void);                                            // GPRSģ�����ݽ��պ���
void sim7600ce_sms_ReciveInit(void);                                         // ���Ž��ճ�ʼ������
void sim7600ce_sms_send(u8 *SendDateBuf);                                    // SIM7600ce������
void sim7600ce_smsread_UpLoaded(void);                                       // ���Ŷ����ݼ���
void sim7600ce_smsWrite_UpLoaded(void);                                      // ����д���ݼ���
void usart3_send_string(unsigned char *str, unsigned short len);
void lte_clear(void);
u8 sim7600ce_send_cmd(u8 *cmd, u8 *ack, u16 waittime);
void sim7600ce_DoWithData(void);
#endif
