/**
 ******************************************************************************
 * @file    GlobalV_Extern.h
 * @author  ChengLei Zhou  - �ܳ���
 * @version V1.27
 * @date    2014-01-03
 * @brief   ȫ�ֱ�������
 ******************************************************************************
 */

#ifndef __GLOBALV_EXTERN_H
#define __GLOBALV_EXTERN_H

#include "sx1276-LoRa.h"

// (�������)
extern u8 SoftClock[];     // ����ʱ��
extern u16 w_ParLst[];     // �������б���
extern u8 DInb[];          // ��������������б���
extern u8 DOutb[];         // ��������������б���
extern u16 w_AI[];         // ģ������������б���
extern u8 Tmp1;            // �ݴ�ֵ
extern u8 Tmp2;            // �ݴ�ֵ
extern u8 Tmp3;            // �ݴ�ֵ
extern u8 Tmp4;            // �ݴ�ֵ
extern u8 Tmp5;            // �ݴ�ֵ
extern u16 Temp1;          // �ݴ�ֵ
extern u16 Temp2;          // �ݴ�ֵ
extern u16 Temp3;          // �ݴ�ֵ
extern u16 Temp4;          // �ݴ�ֵ
extern u8 Value[5];        // ����
extern u16 sysTickMsValue; // ϵͳʱ��MSֵ

extern u16 w_ParLst[];  // �������б���
extern u16 Pw_ParLst[]; // �������б���

extern u16 w_dspParLst[];  // DSP�������б��� ֻ�� 2015.7.1
extern u16 Pw_dspParLst[]; // DSP�������б��� �趨 2015.7.1
extern u16 w_ScrParLst[];  // ZCL 2018.8.3 SCR�������

extern u8 B_ModPar;
extern u16 Lw_Reg;
extern u16 Lw_SaveSetValue;
extern u16 Lw_DCM200SCR_VERSION;
extern u16 Lw_SaveEquipOperateStatus; // ZCL 2015.9.7
extern u16 Lw_SaveEquipAlarmStatus;
extern u16 Lw_SaveDspStopStatus;
extern u16 Lw_SaveDspFaultStatus;
extern u16 Lw_SaveKglInStatus;  // ZCL 2019.8.31 2019.9.11
extern u16 Lw_SaveKglOutStatus; // ZCL 2019.8.31 2019.9.11

//-----------GPRS  ZCL 2018.12.10
extern u16 w_GprsParLst[]; // �������б��� ��д
extern u8 B_RunGprsModule; // ����GPRSģ���־
extern u8 GprsPar[];       // Gprs����
extern u8 Com1_LedStatus;  // COM1ָʾ��״̬
extern u8 Com0_LedStatus;  // COM0ָʾ��״̬

//----------LoRa ���������� �����ӻ��� ����������
extern u16 w_dsp1ParLst[]; // �������б��� ��д ZCL 2019.3.7
extern u16 w_dsp2ParLst[]; // �������б��� ��д ZCL 2019.3.7
extern u16 w_dsp3ParLst[]; // �������б��� ��д ZCL 2019.3.7
extern u16 w_dsp4ParLst[]; // �������б��� ��д ZCL 2019.3.7

extern tLoRaSettings LoRaSettings; // ZCL 2019.3.22

extern u16 w_DNBParLst[]; // �������б���  ZCL 2022.5.19

extern u8 F_ModeParLora; // ZCL 2023.2.10  �޸�LORA������־

// �����Ǻ���㶨��Ĳ���
extern u8 APN_Name[]; // �����

extern u16 Pw_ParLst_GPRS[256]; // �趨�������б���

extern u8 ServerCodeSrc[8];      // �����������ַ���
extern u8 AccessPointNameSrc[5]; // ����������ַ���

extern u8 S_XmodemConfigProcess; // S_Xmodem���ò���
extern u8 F_STOPTransmit;        // Xmodemֹͣ�����־

extern u16 ReceiveGPRSBufOneDateLen;   // ���յ�1·GPRSBuf�����ݳ���
extern u16 ReceiveGPRSBufTwoDateLen;   // ���յ�2·GPRSBuf�����ݳ���
extern u16 ReceiveGPRSBufThreeDateLen; // ���յ�3·GPRSBuf�����ݳ���
extern u16 ReceiveGPRSBufFourDateLen;  // ���յ�4·GPRSBuf�����ݳ���
extern u8 ReceiveGPRSBuf[];            // GPRS���յ������ݷŵ�GPRSBUF����
extern u8 F_GPRSSendFinish;            // GPRS������ɱ�־
extern u8 F_UDP_OR_TCP;                //=0ΪUDP��=1ΪTCP
extern u8 F_PowerOnRegister;           // ���Э���ϵ�ע���־
extern u8 F_AcklinkNum;
;                             // �������ݺ󣬼�¼����һ·�����������ݣ��������ݵ�ʱ�򷵻ظ���һ·��
extern u8 F_NowNetworkSystem; // ��ǰ������ʽ

extern u8 CSQBuf[2];                  // �ź�����
extern u8 local_port_Byte[2];         // ���ض˿���ʱ�洢ȥ
extern u8 DTUIDBuf[11];               // �����DTUID��
extern u16 T_link1OnlineReportTimeMS; // ����1����ʱ��MS
extern u16 T_link2OnlineReportTimeMS; // ����2����ʱ��MS
extern u16 T_link3OnlineReportTimeMS; // ����3����ʱ��MS
extern u16 T_link4OnlineReportTimeMS; // ����4����ʱ��MS

extern uint8_t usart3_rev_cnt;
extern uint8_t usart3_rev_flag;
extern uint8_t usart3_rev_finish; // ����3���ռ���������3���ձ�־���ڽ�����ɱ�־

extern uint8_t lte_power_on_flag, lte_init_flag, lte_net_flag, lte_connect_flag;
extern uint8_t err_cnt, err_num;
extern uint8_t heart_beat_flag; // ������־

extern uchar T_DataLampAllLight; // ����DATA������ 2016.5.27 ZCL
extern uint C_DataLampAllLight;  // ����DATA������ ������
extern uint C_LedLight;          // LED��˸��ʱ������
extern uint C_NetLedLightCSQ;    // Net LED��˸CSQָʾ������ ZCL 2017.5.25
extern u16 Cw_Rcv2;              // ���ռ�����
extern u16 Cw_Txd2;              // ���ͼ�����
extern u8 CGD0_LinkConnectOK[];  // ����0���ӱ�־
extern u8 F_RunCheckModular;

extern u8 B_Com1ErrWithModule1; // ��־ ����ģ��1ͨѶʧ��
extern u8 B_Com1ErrWithModule2; // ��־ ����ģ��2ͨѶʧ��
extern u8 F_RemoteQuery;        // ͨ��GPRSԶ�̲�ѯ������־
extern u8 F_Reset_GPS;          // ����GPS��־
extern u16 T_Reset_GPS;
extern u16 C_Reset_GPS;
extern u16 T_PowerOnOff_GPS;
extern u16 C_PowerOnOfft_GPS;

extern u8 B_LoRaErrWithModule1; // ��־ ����ģ��1ͨѶʧ��
extern u8 B_LoRaErrWithModule2; // ��־ ����ģ��2ͨѶʧ��
extern u8 B_LoRaErrWithModule3; // ��־ ����ģ��3ͨѶʧ��
extern u8 B_LoRaErrWithModule4; // ��־ ����ģ��4ͨѶʧ��
extern u8 B_LoRaErrWithModule5; // ��־ ����ģ��5ͨѶʧ��
extern u8 B_LoRaErrWithModule6; // ��־ ����ģ��6ͨѶʧ��
extern u8 B_LoRaErrWithModule7; // ��־ ����ģ��7ͨѶʧ��
extern u8 B_LoRaErrWithModule8; // ��־ ����ģ��8ͨѶʧ��

extern u16 T_Judge_No1_VVVF_Comm; // �ж�1#�ñ�Ƶ��ͨѶ���ϼ�ʱ
extern u16 C_Judge_No1_VVVF_Comm; // �ж�1#�ñ�Ƶ��ͨѶ���ϼ���
extern u16 T_Judge_No2_VVVF_Comm; // �ж�2#�ñ�Ƶ��ͨѶ���ϼ�ʱ
extern u16 C_Judge_No2_VVVF_Comm; // �ж�2#�ñ�Ƶ��ͨѶ���ϼ���
extern u16 T_Judge_No3_VVVF_Comm; // �ж�3#�ñ�Ƶ��ͨѶ���ϼ�ʱ
extern u16 C_Judge_No3_VVVF_Comm; // �ж�3#�ñ�Ƶ��ͨѶ���ϼ���
extern u16 T_Judge_No4_VVVF_Comm; // �ж�4#�ñ�Ƶ��ͨѶ���ϼ�ʱ
extern u16 C_Judge_No4_VVVF_Comm; // �ж�4#�ñ�Ƶ��ͨѶ���ϼ���
extern u16 T_Judge_No5_VVVF_Comm; // �ж�5#�ñ�Ƶ��ͨѶ���ϼ�ʱ
extern u16 C_Judge_No5_VVVF_Comm; // �ж�5#�ñ�Ƶ��ͨѶ���ϼ���
extern u16 T_Judge_No6_VVVF_Comm; // �ж�6#�ñ�Ƶ��ͨѶ���ϼ�ʱ
extern u16 C_Judge_No6_VVVF_Comm; // �ж�6#�ñ�Ƶ��ͨѶ���ϼ���
extern u16 T_Judge_No7_VVVF_Comm; // �ж�7#�ñ�Ƶ��ͨѶ���ϼ�ʱ
extern u16 C_Judge_No7_VVVF_Comm; // �ж�7#�ñ�Ƶ��ͨѶ���ϼ���
extern u16 T_Judge_No8_VVVF_Comm; // �ж�8#�ñ�Ƶ��ͨѶ���ϼ�ʱ
extern u16 C_Judge_No8_VVVF_Comm; // �ж�8#�ñ�Ƶ��ͨѶ���ϼ���

extern u8 F_No1_VVVFComm_Fault; // 1#�ñ�Ƶ��ͨѶ�쳣��־
extern u8 F_No2_VVVFComm_Fault; // 2#�ñ�Ƶ��ͨѶ�쳣��־
extern u8 F_No3_VVVFComm_Fault; // 3#�ñ�Ƶ��ͨѶ�쳣��־
extern u8 F_No4_VVVFComm_Fault; // 4#�ñ�Ƶ��ͨѶ�쳣��־
extern u8 F_No5_VVVFComm_Fault; // 5#�ñ�Ƶ��ͨѶ�쳣��־
extern u8 F_No6_VVVFComm_Fault; // 6#�ñ�Ƶ��ͨѶ�쳣��־
extern u8 F_No7_VVVFComm_Fault; // 7#�ñ�Ƶ��ͨѶ�쳣��־
extern u8 F_No8_VVVFComm_Fault; // 8#�ñ�Ƶ��ͨѶ�쳣��־

extern u16 Count_Save1_No1_VVVF_Comm; // 1#�ñ�Ƶ��ͨѶ������1
extern u16 Count_Save2_No1_VVVF_Comm; // 1#�ñ�Ƶ��ͨѶ������2
extern u16 Count_Save1_No2_VVVF_Comm; // 2#�ñ�Ƶ��ͨѶ������1
extern u16 Count_Save2_No2_VVVF_Comm; // 2#�ñ�Ƶ��ͨѶ������2
extern u16 Count_Save1_No3_VVVF_Comm; // 3#�ñ�Ƶ��ͨѶ������1
extern u16 Count_Save2_No3_VVVF_Comm; // 3#�ñ�Ƶ��ͨѶ������2
extern u16 Count_Save1_No4_VVVF_Comm; // 4#�ñ�Ƶ��ͨѶ������1
extern u16 Count_Save2_No4_VVVF_Comm; // 4#�ñ�Ƶ��ͨѶ������2
extern u16 Count_Save1_No5_VVVF_Comm; // 5#�ñ�Ƶ��ͨѶ������1
extern u16 Count_Save2_No5_VVVF_Comm; // 5#�ñ�Ƶ��ͨѶ������2
extern u16 Count_Save1_No6_VVVF_Comm; // 6#�ñ�Ƶ��ͨѶ������1
extern u16 Count_Save2_No6_VVVF_Comm; // 6#�ñ�Ƶ��ͨѶ������2
extern u16 Count_Save1_No7_VVVF_Comm; // 7#�ñ�Ƶ��ͨѶ������1
extern u16 Count_Save2_No7_VVVF_Comm; // 7#�ñ�Ƶ��ͨѶ������2
extern u16 Count_Save1_No8_VVVF_Comm; // 8#�ñ�Ƶ��ͨѶ������1
extern u16 Count_Save2_No8_VVVF_Comm; // 8#�ñ�Ƶ��ͨѶ������2

extern u8 B_Com1Con; // ����1���Ϳ���
#endif               /* __GLOBALV_EXTERN_H */
