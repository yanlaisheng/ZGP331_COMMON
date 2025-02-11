/**
 ******************************************************************************
 * @file    GlobalV.h
 * @author  ChengLei Zhou  - �ܳ���
 * @version V1.27
 * @date    2014-01-03
 * @brief   ȫ�ֱ�������
 ******************************************************************************
 */

#ifndef __GLOBALV_H
#define __GLOBALV_H

#include "GlobalConst.h"

// (�������)
u8 SoftClock[9]; // ����ʱ��

u8 DInb[6];         // ��������������б���
u8 DOutb[6];        // ��������������б���
u16 w_AI[8];        // ģ������������б���
u8 Tmp1;            // �ݴ�ֵ
u8 Tmp2;            // �ݴ�ֵ
u8 Tmp3;            // �ݴ�ֵ
u8 Tmp4;            // �ݴ�ֵ
u8 Tmp5;            // �ݴ�ֵ
u16 Temp1;          // �ݴ�ֵ
u16 Temp2;          // �ݴ�ֵ
u16 Temp3;          // �ݴ�ֵ
u16 Temp4;          // �ݴ�ֵ
u8 Value[5];        // ����
u16 sysTickMsValue; // ϵͳʱ��MSֵ

u16 Pw_ParLst[500]; // �������б��� �趨		//0������
u16 w_ParLst[1000]; // �������б��� ֻ��		//1������

u16 Pw_dspParLst[100]; // DSP�������б��� �趨 2015.7.1			//2������
u16 w_dspParLst[150];  // DSP�������б��� ֻ�� 2015.7.1	ZCL 2018.9.21 ����100̫��			//3������

// u16  	w_ScrParLst[50];			// ZCL 2018.8.3 SCR�������  			ZCL 2018.9.21 ����10̫��
//			w_GprsParLst[]

u8 B_ModPar;
u16 Lw_Reg;
u16 Lw_SaveSetValue;
u16 Lw_DCM200SCR_VERSION;
u16 Lw_SaveEquipOperateStatus;
u16 Lw_SaveEquipAlarmStatus;

u16 Lw_SaveDspStopStatus;  // ZCL 2018.4.25
u16 Lw_SaveDspFaultStatus; // ZCL 2018.4.25

u16 Lw_SaveKglInStatus;  // ZCL 2019.8.31 2019.9.11
u16 Lw_SaveKglOutStatus; // ZCL 2019.8.31 2019.9.11

//-----------GPRS  ZCL 2018.12.10 ��ZCL 2019.9.11 150
u16 w_GprsParLst[280]; // �������б��� ��д ZCL 2018.12.10		//4�����ʣ�������������ʵ�ʲ���4����

u8 B_RunGprsModule; // ����GPRSģ���־(û�����ı�ͨѶ��4����ֵ=1)
u8 GprsPar[288];    // Gprs����
u8 Com1_LedStatus;  // COM1ָʾ��״̬
u8 Com0_LedStatus;  // COM0ָʾ��״̬

//----------LoRa ���������� �����ӻ��� ����������
// u16  	w_dsp1ParLst[50];				// �������б��� ��д ZCL 2019.3.7			//5�����ʣ�������������ʵ�ʲ���5����
// u16  	w_dsp2ParLst[50];				// �������б��� ��д ZCL 2019.3.7			//5�����ʣ�������������ʵ�ʲ���5����
// u16  	w_dsp3ParLst[50];				// �������б��� ��д ZCL 2019.3.7			//5�����ʣ�������������ʵ�ʲ���5����
// u16  	w_dsp4ParLst[50];				// �������б��� ��д ZCL 2019.3.7			//5�����ʣ�������������ʵ�ʲ���5����

u16 w_DNBParLst[1000]; // �������б���  ZCL 2022.5.19

u8 F_ModeParLora; // ZCL 2023.2.10  �޸�LORA������־

// �����Ǻ���㶨��Ĳ���
u8 APN_Name[31]; // �����

u16 Pw_ParLst_GPRS[256]; // �趨�������б���

u8 ServerCodeSrc[8];      // �����������ַ���
u8 AccessPointNameSrc[5]; // ����������ַ���

u8 S_XmodemConfigProcess; // S_Xmodem���ò���
u8 F_STOPTransmit;        // Xmodemֹͣ�����־

u16 ReceiveGPRSBufOneDateLen;   // ���յ�1·GPRSBuf�����ݳ���
u16 ReceiveGPRSBufTwoDateLen;   // ���յ�2·GPRSBuf�����ݳ���
u16 ReceiveGPRSBufThreeDateLen; // ���յ�3·GPRSBuf�����ݳ���
u16 ReceiveGPRSBufFourDateLen;  // ���յ�4·GPRSBuf�����ݳ���
u8 ReceiveGPRSBuf[RCV3_MAX];    // GPRS���յ������ݷŵ�GPRSBUF����
u8 F_GPRSSendFinish;            // GPRS������ɱ�־
u8 F_UDP_OR_TCP;                //=0ΪUDP��=1ΪTCP
u8 F_PowerOnRegister;           // ���Э���ϵ�ע���־
u8 F_AcklinkNum = 0;
;                      // �������ݺ󣬼�¼����һ·�����������ݣ��������ݵ�ʱ�򷵻ظ���һ·��
u8 F_NowNetworkSystem; // ��ǰ������ʽ

u8 CSQBuf[2];                  // �ź�����
u8 local_port_Byte[2];         // ���ض˿���ʱ�洢��
u8 DTUIDBuf[11];               // �����DTUID��
u16 T_link1OnlineReportTimeMS; // ����1����ʱ��MS
u16 T_link2OnlineReportTimeMS; // ����2����ʱ��MS
u16 T_link3OnlineReportTimeMS; // ����3����ʱ��MS
u16 T_link4OnlineReportTimeMS; // ����4����ʱ��MS

uint8_t usart3_rev_cnt = 0;
uint8_t usart3_rev_flag = 0;
uint8_t usart3_rev_finish = 0; // ����3���ռ���������3���ձ�־���ڽ�����ɱ�־

u8 B_Com1ErrWithModule1; // ��־ ����ģ��1ͨѶʧ��
u8 B_Com1ErrWithModule2; // ��־ ����ģ��2ͨѶʧ��

u8 F_RemoteQuery; // ͨ��GPRSԶ�̲�ѯ������־
u8 F_Reset_GPS;   // ����GPS��־
u16 T_Reset_GPS;
u16 C_Reset_GPS;
u16 T_PowerOnOff_GPS;
u16 C_PowerOnOfft_GPS;

u16 T_Judge_No1_VVVF_Comm; // �ж�1#�ñ�Ƶ��ͨѶ���ϼ�ʱ
u16 C_Judge_No1_VVVF_Comm; // �ж�1#�ñ�Ƶ��ͨѶ���ϼ���
u16 T_Judge_No2_VVVF_Comm; // �ж�2#�ñ�Ƶ��ͨѶ���ϼ�ʱ
u16 C_Judge_No2_VVVF_Comm; // �ж�2#�ñ�Ƶ��ͨѶ���ϼ���
u16 T_Judge_No3_VVVF_Comm; // �ж�3#�ñ�Ƶ��ͨѶ���ϼ�ʱ
u16 C_Judge_No3_VVVF_Comm; // �ж�3#�ñ�Ƶ��ͨѶ���ϼ���
u16 T_Judge_No4_VVVF_Comm; // �ж�4#�ñ�Ƶ��ͨѶ���ϼ�ʱ
u16 C_Judge_No4_VVVF_Comm; // �ж�4#�ñ�Ƶ��ͨѶ���ϼ���
u16 T_Judge_No5_VVVF_Comm; // �ж�5#�ñ�Ƶ��ͨѶ���ϼ�ʱ
u16 C_Judge_No5_VVVF_Comm; // �ж�5#�ñ�Ƶ��ͨѶ���ϼ���
u16 T_Judge_No6_VVVF_Comm; // �ж�6#�ñ�Ƶ��ͨѶ���ϼ�ʱ
u16 C_Judge_No6_VVVF_Comm; // �ж�6#�ñ�Ƶ��ͨѶ���ϼ���
u16 T_Judge_No7_VVVF_Comm; // �ж�7#�ñ�Ƶ��ͨѶ���ϼ�ʱ
u16 C_Judge_No7_VVVF_Comm; // �ж�7#�ñ�Ƶ��ͨѶ���ϼ���
u16 T_Judge_No8_VVVF_Comm; // �ж�8#�ñ�Ƶ��ͨѶ���ϼ�ʱ
u16 C_Judge_No8_VVVF_Comm; // �ж�8#�ñ�Ƶ��ͨѶ���ϼ���

u8 F_No1_VVVFComm_Fault; // 1#�ñ�Ƶ��ͨѶ�쳣��־
u8 F_No2_VVVFComm_Fault; // 2#�ñ�Ƶ��ͨѶ�쳣��־
u8 F_No3_VVVFComm_Fault; // 3#�ñ�Ƶ��ͨѶ�쳣��־
u8 F_No4_VVVFComm_Fault; // 4#�ñ�Ƶ��ͨѶ�쳣��־
u8 F_No5_VVVFComm_Fault; // 5#�ñ�Ƶ��ͨѶ�쳣��־
u8 F_No6_VVVFComm_Fault; // 6#�ñ�Ƶ��ͨѶ�쳣��־
u8 F_No7_VVVFComm_Fault; // 7#�ñ�Ƶ��ͨѶ�쳣��־
u8 F_No8_VVVFComm_Fault; // 8#�ñ�Ƶ��ͨѶ�쳣��־

u16 Count_Save1_No1_VVVF_Comm; // 1#�ñ�Ƶ��ͨѶ������1
u16 Count_Save2_No1_VVVF_Comm; // 1#�ñ�Ƶ��ͨѶ������2
u16 Count_Save1_No2_VVVF_Comm; // 2#�ñ�Ƶ��ͨѶ������1
u16 Count_Save2_No2_VVVF_Comm; // 2#�ñ�Ƶ��ͨѶ������2
u16 Count_Save1_No3_VVVF_Comm; // 3#�ñ�Ƶ��ͨѶ������1
u16 Count_Save2_No3_VVVF_Comm; // 3#�ñ�Ƶ��ͨѶ������2
u16 Count_Save1_No4_VVVF_Comm; // 4#�ñ�Ƶ��ͨѶ������1
u16 Count_Save2_No4_VVVF_Comm; // 4#�ñ�Ƶ��ͨѶ������2
u16 Count_Save1_No5_VVVF_Comm; // 5#�ñ�Ƶ��ͨѶ������1
u16 Count_Save2_No5_VVVF_Comm; // 5#�ñ�Ƶ��ͨѶ������2
u16 Count_Save1_No6_VVVF_Comm; // 6#�ñ�Ƶ��ͨѶ������1
u16 Count_Save2_No6_VVVF_Comm; // 6#�ñ�Ƶ��ͨѶ������2
u16 Count_Save1_No7_VVVF_Comm; // 7#�ñ�Ƶ��ͨѶ������1
u16 Count_Save2_No7_VVVF_Comm; // 7#�ñ�Ƶ��ͨѶ������2
u16 Count_Save1_No8_VVVF_Comm; // 8#�ñ�Ƶ��ͨѶ������1
u16 Count_Save2_No8_VVVF_Comm; // 8#�ñ�Ƶ��ͨѶ������2
#endif                         /* __GLOBALV_H */
