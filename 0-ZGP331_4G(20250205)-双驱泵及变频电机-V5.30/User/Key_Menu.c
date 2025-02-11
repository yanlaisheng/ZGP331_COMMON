/**
 ******************************************************************************
 * @file    Key_Menu.c
 * @author  ChengLei Zhou  - �ܳ���
 * @version V1.28
 * @date    2014-01-03
 * @brief   Code  for UG-2864KSWLG01 WiseChip ���ӽӿڣ�IC:SSD1306
 * 				 Hardware	: STM32F103(101) C8T6,CBT6
 * 				 Platform	: IAR6.40,Keil-MDK5.01,LIB:V3.5.0(11/03/2011),JLINK V6-V7
 ******************************************************************************
 * @attention	2014.1.3
 ******************************************************************************/

/* ��������1λС����2018.1.26 ZCL ������������
	 ע�ͺ��������ǰ��һ����2λС����  */
// #define CURRENT_OneXiaoShu	//����1λС��

/*
	// ȡ�� #define CURRENT_OneXiaoShu	//����1λС��
	�ò��� SCRҺ���������������: Pw_ScrCurrentBits 		ZCL 2018.9.13
	*/

/* @note	2014.4.8
  �ܳ��� 2011.5.9
  1. Һ��RAM����LSB���ϣ�MSB����
  2. �����Ǿ�OLEDҺ��1.3��(UG-2864KSWLG01  WiseChip ���ӽӿ�)��ʾ�����ĵײ㺯��

  3. ZCL 2019.4.16 ��ע��DCM200_SCR_E��KEY3��KEY4��KEY5 ��ռ�ã�GetKey()ʹ��ʱҪע�͵���

  */

/* Includes ------------------------------------------------------------------*/
#include "Key_Menu.h"
#include "GlobalV_Extern.h" // ȫ�ֱ�������
#include "GlobalConst.h"
#include "LCD.h"
#include <stdlib.h>

#include "com1_232.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
u8 Key_Pressed;		 // ��������
u8 Key_Data;		 // ����ֵ�������Ǹ�����
u8 Sa_OldKeyData;	 // ����ɼ�ֵ�����������ã�
u16 Lw_PageNo;		 // ��ʾ�Ļ������
u16 Lw_SavePageNo;	 // ���滭�����
u8 F_KeyExec;		 // ����ִ�б�־����ֹ�ظ�ִ��
u8 S_ModParStatus;	 // �޸Ĳ���ʱ=1���޸Ļ������ʱ=0
u8 ModParNo;		 // �޸Ĳ�����ţ���һ�������˳���޸Ŀ��趨�Ĳ�����
u8 F_ModPar1;		 // �޸Ĳ�����־1	 ����ָ����ҳ���Ǹ�����Ҫ�޸�
u8 F_ModPar2;		 // �޸Ĳ�����־2	 ����ָ����ҳ���Ǹ�����Ҫ�޸�
u8 F_ModPar3;		 // �޸Ĳ�����־3	 ����ָ����ҳ���Ǹ�����Ҫ�޸�
u8 F_ModPar4;		 // �޸Ĳ�����־4	 ����ָ����ҳ���Ǹ�����Ҫ�޸�
u8 F_ModPar5;		 // �޸Ĳ�����־5	 ����ָ����ҳ���Ǹ�����Ҫ�޸�
u8 F_ModPar6;		 // �޸Ĳ�����־6	 ����ָ����ҳ���Ǹ�����Ҫ�޸�
u8 Sa_ModParKeyData; // ������޸Ĳ�����ֵ
u8 ModParBitPos;	 // �޸Ĳ���λ-λ��

u8 T_LcdDisplay; // ���ڿ���LCD��ʱ��ʾ��������ʾ��������
u16 C_LcdDisplay;
u8 T_KeyPressOverTime; // ���ڰ�����ʱ�����²ɼ������Ӱ����ٶ�
u16 C_KeyPressOverTime;
u8 B_RightKeyCount;		// �Ҽ������������ڿ����
u8 B_LeftKeyCount;		// ��������������ڿ����
u8 B_PressedStopRunKey; // ����STOP����RUN�� 2016.12.6		2018.5.19

u8 T_TimeUpdatePage; // ���ڶ�ʱ����ҳ�棬�������뼰ʱ���� ZCL 2015.9.17
u16 C_TimeUpdatePage;

u8 B_TimeReadBPDJPar; // ��ʱ��BPDJ���� ZCL 2019.2.18
u8 T_TimeReadBPDJPar; // ��ʱ��BPDJ���� ZCL 2019.2.18
u16 C_TimeReadBPDJPar;

u16 Lw_KEYStableCounter; // ZCL 2018.4.21 ��L_		�����ȶ�������
u16 Lw_TmpModParValue;	 // ZCL 2018.4.21 ��L_		��ʱ�޸Ĳ���ֵ
u16 Lw_OldModParValue;	 // ZCL 2018.5.16 ��L_		�ɵ��޸Ĳ���ֵ������Ա��ã�
u8 S_OldModParStatus;	 // ZCL 2018.5.16					�ɵ��޸Ĳ���״̬������Ա��ã�
u8 S_DisplayPar;		 // ZCL 2018.5.17	��ʾ���� S������ =0��ʱ�У�=1������ʾ

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private variables extern --------------------------------------------------*/
extern uc8 BMP[];
extern volatile u32 sysTickValue;
extern u8 F_ModTime;

extern u8 S_Com1SendNoNum; // Com1���ʹ���
extern u8 S_Com1Send;	   // Com1���Ͳ���
extern u8 F_Com1Send;	   // Com1���ͱ�־
extern u8 T_Com1Send;
extern u16 C_Com1Send;
extern u8 T_Com1TimeSend;
extern u16 C_Com1TimeSend;
extern u8 S_RdWrNo; // ��д���

extern u16 C_LCDBackLight; // ZCL 2017.9.19 �������

extern u16 C_Com1SendDelay; // ZCL 2018.5.14

extern u16 Cw_Rcv1;	 // ���ռ����� ZCL 2018.5.15
extern u16 C_NoRcv1; // û�н��ռ�����

extern u8 B_ModYW310SZM220; // ZCL 2019.10.19

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//********************����Ϊ��ʾ����****************************
// 1.Demo Ƭͷ
// 2.���˵�����

void Menu_Demo(void) // ���� Demo �ָ�����
{
}

void Menu_Welcome(void) // ���� ��ӭ
{
	LCD12864_ClrText(); // ����
	LCD12864_String(0x88, "Booting...");
	LCD_DLY_ms(1500); // ��ʱ1��

	LCD12864_ClrText(); // ����
	LCD12864_String(0x90, "�ൺ�������ܶ���");
	LCD_DLY_ms(5); // ��ʱ5MS;
	LCD12864_String(0x8A, "��ӭ����");
	LCD_DLY_ms(3000); // ��ʱ3000MS;
}

void Menu_Dynamic_TXT(void) // ���� ��̬�ı�
{
}

void Menu_Feature(void) // ���� ��Ʒ�ص�
{
}

void Menu_LaserTxt(void) // ���� �����ı�(ɨ����дЧ��)
{
}

void EnterMenu_InitPar(void) // ����˵���ʼ������  ZCL 2018.5.15
{
	C_TimeUpdatePage = 0; // ���ڶ�ʱ����ҳ�棬�������뼰ʱ���� ZCL 2015.9.17
	S_Com1Send = 0;		  // ������д���������¿�ʼ
	C_Com1Send = 0;
	C_Com1SendDelay = 0; // ������д��������ʱ�����¿�ʼ
	S_ModParStatus = 0;	 // �޸Ĳ���ʱ=1���޸Ļ������ʱ=0 �� ����Ӧ��û�ã����ำֵ0

	// ��2������ܹ��ã���ֹ�л���ҳ�棬�ն����������յ���ҳ�������ķ���ֵ������ֵ����ҳ�Ķ�����
	// ZCL 2018.5.15
	Cw_Rcv1 = 0;  // ���ռ�������ֵ0
	C_NoRcv1 = 0; // �����ռ�����

	S_RdWrNo = 0;	  // �л���ҳ�棬�ӵ�һ��������ʼ��д
	S_DisplayPar = 0; // ZCL 2018.5.17 ������Բ�Ҫ���������
	C_LcdDisplay = 0; // ZCL 2018.5.17

	B_TimeReadBPDJPar = 0; // ZCL 2019.3.8
}

void Menu_StmReadPar1(void) // ���� ������1
{
	// u8 i;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��ˮѹ��");
		LCD12864_String(0x87, "Mp");
		// �ڶ���
		LCD12864_String(0x90, "��ˮѹ��");
		LCD12864_String(0x97, "Mp");
		// ������
		LCD12864_String(0x88, "�趨ѹ��");
		LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "��ƵƵ��");
		LCD12864_String(0x9F, "Hz");
	}

	/*  DispMod_Par(u8 x_pos,u8 length, u8 XiaoShuBits,  u16 *Address, u8 ParType,
	u8 nb_modpar)	 */

	ModParNum(0); // ָ��:�޸Ĳ���������
	// ZCL 2018.5.16 �޸Ĳ�������ʱ���ӿ��ٶȱ�֤�л�����ʾ������ʱ������Һ����ʾ�����ϣ���ʾ����
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x04, 0x03, &w_InPDec, 1, 0);
	DispMod_Par(0x94, 0x04, 0x03, &w_OutPDec, 1, 0);
	DispMod_Par(0x8C, 0x04, 0x03, &w_PIDCalcP, 1, 0);
	DispMod_Par(0x9C, 0x04, 0x01, &w_dspNowHz, 3, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������	ZCL 2019.3.19 �����ṹ������ if(B_ModPar==0)
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_InPDec, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_OutPDec, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_PIDCalcP, 1), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_dspNowHz, 3), 1);
	}

	// ZCL 2015.8.31 OK �����ں������ʾ'A'
	/* 	LCD12864_WriteCommand(0x84);
		i=LCD12864_ReadData();
		LCD12864_WriteData(i);
		LCD12864_WriteData('A'); */
}

void Menu_StmReadPar2(void) // ���� ������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "ĸ�ߵ�ѹ");
		LCD12864_String(0x87, " V");

		// �ڶ���
		LCD12864_String(0x90, "�����ѹ");
		LCD12864_String(0x97, " V");

		// ������
		LCD12864_String(0x88, "�������");
		LCD12864_String(0x8F, " A");

		// ������
		LCD12864_String(0x98, "ģ���¶�");
		LCD12864_String(0x9F, "��");
	}

	ModParNum(0); // ָ��:�޸Ĳ���������
	// ZCL 2018.5.16 �޸Ĳ�������ʱ���ӿ��ٶȱ�֤�л�����ʾ������ʱ������Һ����ʾ�����ϣ���ʾ����
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &w_dspBusBarVoltage, 3, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_dspOutVoltage, 3, 0);
	// DispMod_Par(0x8C, 0x04,0x02, &w_dspAPhaseCurrent,3, 0);

	if (Pw_ScrCurrentBits == 1) // ZCL 2018.8.29  ����1λС��
	{
		DispMod_Par(0x8C, 0x04, 0x01, &w_dspAPhaseCurrent, 3, 0); // ZCL 2018.5.19
	}
	else if (Pw_ScrCurrentBits == 2) // ZCL 2018.8.29  ����2λС��
	{
		DispMod_Par(0x8C, 0x05, 0x02, &w_dspAPhaseCurrent, 3, 0); // ZCL 2018.5.19  2019.6.7  0x04
	}

	/* #ifdef CURRENT_OneXiaoShu		//ZCL 2018.5.19  ����1λС��
	#else
	#endif	 */
	// ZCL 2018.8.29 ��Pw_ScrCurrentBits ����

	DispMod_Par(0x9C, 0x04, 0x01, &w_dspNowTemperature, 3, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspBusBarVoltage, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspOutVoltage, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspAPhaseCurrent, 3), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_dspNowTemperature, 3), 1);
	}
}

void Menu_StmReadPar3(void) // ���� ������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��ˮ״̬");
		LCD12864_String(0x87, "��");
		// �ڶ���
		LCD12864_String(0x90, "--");

		// ������
		LCD12864_String(0x88, "ͣ��ԭ��");
		LCD12864_String(0x8F, "��");
		// ������
		LCD12864_String(0x98, "--");

		Lw_SaveEquipOperateStatus = 1000;
		Lw_SaveEquipAlarmStatus = 1000;
	}

	ModParNum(0);
	DispMod_Par(0x84, 0x05, 0x00, &w_EquipOperateNum, 1, 0);
	// DispMod_Par(0x94, 0x04,0x02, &w_SoftVer,1, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_EquipStopNum, 1, 0);
	// DispMod_Par(0x9C, 0x04,0x00, &w_Writetime,1, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (Lw_SaveEquipOperateStatus != w_EquipOperateStatus)
	{
		Lw_SaveEquipOperateStatus = w_EquipOperateStatus;
		// ����״̬����ʾ  ZCL 2015.9.7
		switch (w_EquipOperateStatus)
		{
		case 0:
			LCD12864_String(0x91, "��������      ");
			break; //
		case 1:
			LCD12864_String(0x91, "ͣ��״̬      ");
			break; //
		case 2:
			LCD12864_String(0x91, "Ѳ��״̬      ");
			break; //
		case 3:
			LCD12864_String(0x91, "ͨѶʧ��      ");
			break; //
		case 4:
			LCD12864_String(0x91, "С��������    ");
			break; //
		case 5:
			LCD12864_String(0x91, "С�����ƻ�    ");
			break; //
		case 6:
			LCD12864_String(0x91, "С�������    ");
			break; //
		// case  7: LCD12864_String(0x91, "              ");	break;		//
		// case  8: LCD12864_String(0x91, "              ");	break; 		//
		// case  9: LCD12864_String(0x91, "              ");	break;		//
		// case 10: LCD12864_String(0x91, "              ");	break;		//
		// case 11: LCD12864_String(0x91, "              ");	break;		//
		// case 12: LCD12864_String(0x91, "              ");	break;		//
		case 13:
			LCD12864_String(0x91, "��Ƶ����λʧ��");
			break; //
		case 14:
			LCD12864_String(0x91, "������Ԥ��    ");
			break; //
		// case 15: LCD12864_String(0x91, "              ");	break;		//
		case 16:
			LCD12864_String(0x91, "��ʱ����      ");
			break; //
		case 17:
			LCD12864_String(0x91, "�綯������    ");
			break; //	ZCL 2018.8.22
		case 18:
			LCD12864_String(0x91, "����ˮ��ˮ    ");
			break; //	ZCL 2018.8.22
		case 19:
			LCD12864_String(0x91, "ˮ����ˮ      ");
			break; //	ZCL 2018.8.22
		// case 20: LCD12864_String(0x91, "              ");	break;		//
		// case 21: LCD12864_String(0x91, "              ");	break; 		//
		// case 22: LCD12864_String(0x91, "              ");	break;		//
		case 23:
			LCD12864_String(0x91, "�����ѹ������");
			break; //
		case 24:
			LCD12864_String(0x91, "��Ƶ״̬      ");
			break; //	ZCL 2019.9.10
		case 25:
			LCD12864_String(0x91, "��ˮ����ģʽ  ");
			break; //	ZCL 2019.9.11
		case 26:
			LCD12864_String(0x91, "��Ƶ����ģʽ  ");
			break; //	ZCL 2019.9.11
		case 27:
			LCD12864_String(0x91, "����Ƶ����  ");
			break; //	ZCL 2022.10.3
		// case 25: LCD12864_String(0x91, "              ");	break;		//
		// case 26: LCD12864_String(0x91, "              ");	break; 		//
		// case 27: LCD12864_String(0x91, "              ");	break;		//
		// case 28: LCD12864_String(0x91, "              ");	break;		//
		// case 29: LCD12864_String(0x91, "              ");	break;		//
		// case 30: LCD12864_String(0x91, "              ");	break; 		//
		// case 31: LCD12864_String(0x91, "              ");	break; 		//
		default:
			return;
		}
	}

	if (Lw_SaveEquipAlarmStatus != w_EquipAlarmStatus)
	{
		Lw_SaveEquipAlarmStatus = w_EquipAlarmStatus;
		// ͣ��ԭ���״̬����ʾ  ZCL 2015.9.7
		switch (w_EquipAlarmStatus)
		{
		case 0:
			LCD12864_String(0x99, "�ޣ��豸����  ");
			break; //
		case 1:
			LCD12864_String(0x99, "��ˮͣ��      ");
			break; //
		case 2:
			LCD12864_String(0x99, "����ֹͣ״̬  ");
			break; //
		case 3:
			LCD12864_String(0x99, "�ֶ�״̬      ");
			break; //
		case 4:
			LCD12864_String(0x99, "��Ƶ������    ");
			break; //
		case 5:
			LCD12864_String(0x99, "��Դ����      ");
			break; //
		case 6:
			LCD12864_String(0x99, "Һλ����      ");
			break; //
		case 7:
			LCD12864_String(0x99, "������ѹ      ");
			break; //
		case 8:
			LCD12864_String(0x99, "���бù���    ");
			break; //
		case 9:
			LCD12864_String(0x99, "С������ѹ    ");
			break; //
		case 10:
			LCD12864_String(0x99, "ѹ����������  ");
			break; //
		case 11:
			LCD12864_String(0x99, "��ѹ�߽���ͣ��");
			break; //
		case 12:
			LCD12864_String(0x99, "��ʱ��ͣ��    ");
			break; //
		case 13:
			LCD12864_String(0x99, "���ⶨʱͣ��  ");
			break; //
		case 14:
			LCD12864_String(0x99, "Զ��ң��ͣ��  ");
			break; //
		case 15:
			LCD12864_String(0x99, "��ˮѹ�߱�����");
			break; //
		case 16:
			LCD12864_String(0x99, "�����ɱ�����  ");
			break; //
		case 17:
			LCD12864_String(0x99, "��ص��磡    ");
			break; //
		case 18:
			LCD12864_String(0x99, " 1�ӻ�ͨѶʧ��");
			break; //
		case 19:
			LCD12864_String(0x99, " 2�ӻ�ͨѶʧ��");
			break; //
		case 20:
			LCD12864_String(0x99, "��Ƶ��ͨѶʧ��");
			break; //
		case 21:
			LCD12864_String(0x99, "����������ͣ��");
			break; //
		case 22:
			LCD12864_String(0x99, "�����ֶ�ͣ��  ");
			break; //
		case 23:
			LCD12864_String(0x99, "����ֹͣͣ��  ");
			break; //
		case 24:
			LCD12864_String(0x99, "������Ԥ��ͣ��");
			break; //
		case 25:
			LCD12864_String(0x99, "��ˮѹ����ͣ��");
			break; // ZCL 2018.7.17 ���Ǳ���ͣ��
		case 26:
			LCD12864_String(0x99, "              ");
			break; //
		case 27:
			LCD12864_String(0x99, "              ");
			break; //
		case 28:
			LCD12864_String(0x99, "              ");
			break; //
		case 29:
			LCD12864_String(0x99, "              ");
			break; //
		case 30:
			LCD12864_String(0x99, "              ");
			break; //
		case 31:
			LCD12864_String(0x99, "              ");
			break; //
		default:
			return;
		}
	}

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		/* 	�ı��İ汾����ͨѶ������DCM200_SCR�Լ��Ĳ��� */
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_EquipOperateNum, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_EquipOperateStatus, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_EquipStopNum, 1), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_EquipAlarmStatus, 1), 1);
	}
}

void Menu_StmReadPar4(void) // ���� ������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��Ƶ����");
		LCD12864_String(0x87, "��");
		// �ڶ���
		LCD12864_String(0x90, "--");

		// ������
		LCD12864_String(0x88, "������1 ");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "������2 ");
		LCD12864_String(0x9F, "  ");

		Lw_SaveDspFaultStatus = 1000;
	}

	ModParNum(0);
	DispMod_Par(0x84, 0x05, 0x00, &w_DspAlarmNum, 1, 0);
	// DispMod_Par(0x94, 0x04,0x02, &w_SoftVer,1, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_dspSysErrorCode1, 3, 0);
	DispMod_Par(0x9C, 0x05, 0x00, &w_dspSysErrorCode2, 3, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (Lw_SaveDspFaultStatus != w_DspAlarmStatus)
	{
		Lw_SaveDspFaultStatus = w_DspAlarmStatus;
		// ͣ��ԭ���״̬����ʾ  ZCL 2015.9.7
		switch (w_DspAlarmStatus)
		{
		case 0:
			LCD12864_String(0x91, "�޹���- ����  ");
			break; //
		case 1:
			LCD12864_String(0x91, "��������      ");
			break; //
		case 2:
			LCD12864_String(0x91, "��ѹ����      ");
			break; //
		case 3:
			LCD12864_String(0x91, "Ƿѹ����      ");
			break; //
		case 4:
			LCD12864_String(0x91, "ȱ�ౣ��      ");
			break; //
		case 5:
			LCD12864_String(0x91, "IPM ����      ");
			break; //
		case 6:
			LCD12864_String(0x91, "��������������");
			break; //
		case 7:
			LCD12864_String(0x91, "IPM ����      ");
			break; //
		case 8:
			LCD12864_String(0x91, "�������      ");
			break; //
		case 9:
			LCD12864_String(0x91, "���ȹ���      ");
			break; //
		case 10:
			LCD12864_String(0x91, "EEPROM����    ");
			break; //
		case 11:
			LCD12864_String(0x91, "DSP ����������");
			break; //
		case 12:
			LCD12864_String(0x91, "STM ����������");
			break; //
		case 13:
			LCD12864_String(0x91, "�ӵر���      ");
			break; //
		case 14:
			LCD12864_String(0x91, "������Ƿ��ѹ  ");
			break; //
		case 15:
			LCD12864_String(0x91, "�����й���ѹ  ");
			break; //

		default:
			return;
		}
	}

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_DspAlarmNum, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_DspAlarmStatus, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspSysErrorCode1, 3), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_dspSysErrorCode2, 3), 1);
	}
}

void Menu_StmReadPar5(void) // ���� ������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "  ˮ���ۼ�����  ");
		// LCD12864_String(0x87, "  ");

		// �ڶ���
		LCD12864_String(0x90, "Сʱ");
		LCD12864_String(0x95, "    ʱ");

		// ������
		LCD12864_String(0x88, "  ��");
		LCD12864_String(0x8D, "    ��");

		// ������
		LCD12864_String(0x98, "                ");
	}

	ModParNum(0);
	DispMod_Par(0x92, 0x05, 0x00, &w_Pump1RunHour, 1, 0);
	DispMod_Par(0x8A, 0x05, 0x00, &w_Pump1RunSecond, 1, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 2)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_Pump1RunHour, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_Pump1RunSecond, 1), 1);
	}
}

void Menu_StmReadPar6(void) // ���� ������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��ˮ�汾");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "Arm-Year");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "Arm-Date");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
	}

	ModParNum(0);
	// DispMod_Par(0x84, 0x04,0x02, &Lw_DCM200SCR_VERSION,1, 0);
	DispMod_Par(0x84, 0x04, 0x02, &w_SoftVer, 1, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_WriteDate, 1, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_Writetime, 1, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		/* 	�ı��İ汾����ͨѶ������DCM200_SCR�Լ��Ĳ��� */
		// if(S_RdWrNo==1)
		// Read_Stm32Data( Address(&w_DCM200SCR_VERSION,1), 1 );

		if (S_RdWrNo == 1) // ZCL 2018.9.13 ����ifע���ˣ���������else if ����
			Read_Stm32Data(Address(&w_SoftVer, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_WriteDate, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_Writetime, 1), 1);
	}
}

void Menu_StmReadPar7(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��Ƶ�汾");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "�ı��汾");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "��������");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "��������");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(0);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x04, 0x02, &w_dspSoftVersion, 3, 0);
	DispMod_Par(0x94, 0x04, 0x02, &w_ScrVERSION, 5, 0); // ZCL 2019.4.3
	DispMod_Par(0x8C, 0x05, 0x00, &w_ScrWriteYear, 1, 0);
	DispMod_Par(0x9C, 0x05, 0x00, &w_ScrWriteDate, 1, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspSoftVersion, 3), 1);
		//		if(S_RdWrNo==2)
		//			Read_Stm32Data( Address(&w_FmramErrH,1), 1 );
		//		else if(S_RdWrNo==3)
		//			Read_Stm32Data( Address(&w_FmramErrL,1), 1 );
	}
}

// ZCL 2018.4.19 ɾ��
void Menu_StmReadPar8_NoUse(void) // ���� ������
{
	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_StmReadPar9(void) // ���� ������
{
	// u8 i;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��Dsp OK");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "дDsp OK");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "��Dsp Er"); // ZCL 2015.9.9
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "дDsp Er");
		LCD12864_String(0x9F, "  ");
	}

	/*  DispMod_Par(u8 x_pos,u8 length, u8 XiaoShuBits,  u16 *Address, u8 ParType,
	u8 nb_modpar)	 */
	ModParNum(0);
	DispMod_Par(0x84, 0x05, 0x00, &w_ReadDspOKCount, 1, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_WriteDspOKCount, 1, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_ReadDspErrCount, 1, 0);
	DispMod_Par(0x9C, 0x05, 0x00, &w_WriteDspErrCount, 1, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_ReadDspOKCount, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_WriteDspOKCount, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_ReadDspErrCount, 1), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_WriteDspErrCount, 1), 1);
	}
}

void Menu_StmReadPar10(void) // ���� ������
{
	// u8 i;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		LCD12864_String(0x80, "��Dsp ���");
		// LCD12864_ClrText();          //����
		// ��һ��
		// LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "дDsp ���");
		// LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "RstDsp_C  "); // ZCL 2015.9.9
		// LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "RstDspDlyC");
		// LCD12864_String(0x9F, "  ");
	}

	/*  DispMod_Par(u8 x_pos,u8 length, u8 XiaoShuBits,  u16 *Address, u8 ParType,
	u8 nb_modpar)	 */
	ModParNum(0);
	DispMod_Par(0x85, 0x05, 0x00, &w_RdDspNo, 1, 0);
	DispMod_Par(0x95, 0x05, 0x00, &w_WrDspNo, 1, 0);
	DispMod_Par(0x8D, 0x05, 0x00, &w_ResetedDspCount, 1, 0);
	DispMod_Par(0x9D, 0x05, 0x00, &w_ResetDspDelayCount, 1, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_RdDspNo, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_WrDspNo, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_ResetedDspCount, 1), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_ResetDspDelayCount, 1), 1);
	}
}

void Menu_StmReadPar11(void) // ���� ������
{
	// u8 i;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "������ι��ϼ�¼");
		// �ڶ���
		LCD12864_String(0x90, "1:");
		// LCD12864_String(0x97, " L");
		// ������
		// ZCL 2017.6.17 ����д���¶�ʱˢ��ҳ��ʱ��˸
		// LCD12864_String(0x88, "    --    --    ");		//ZCL 2015.9.10
		// ZCL 2017.6.17 �ĳ����£�
		LCD12864_String(0x8A, "--"); // ZCL 2015.9.10
		LCD12864_String(0x8D, "--"); // ZCL 2015.9.10

		// LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "���� 1�Σ�      ");
		// LCD12864_String(0x9F, "Hz");
	}

	/*  DispMod_Par(u8 x_pos,u8 length, u8 XiaoShuBits,  u16 *Address, u8 ParType,
	u8 nb_modpar)	 */
	ModParNum(0);
	// DispMod_Par(0x84, 0x05,0x00, &w_EquipAlarmLast1,1, 0);
	DispMod_Par(0x88, 0x04, 0x00, &w_EquipAlarm1YM, 1, 0);
	DispMod_Par(0x8B, 0x04, 0x00, &w_EquipAlarm1DH, 1, 0);
	DispMod_Par(0x8E, 0x04, 0x00, &w_EquipAlarm1MS, 1, 0);
	//
	FaultDisplay(w_EquipAlarmLast1, 0x91); // ZCL 2018.5.19 ����S_DisplayPar����ʾ���ƣ���ֹƵ��ˢ��
	S_DisplayPar = 0;					   // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_EquipAlarmLast1, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_EquipAlarm1YM, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_EquipAlarm1DH, 1), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_EquipAlarm1MS, 1), 1);
	}
}

void Menu_StmReadPar12(void) // ���� ������
{
	// u8 i;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "������ι��ϼ�¼");
		// �ڶ���
		LCD12864_String(0x90, "2:");
		// LCD12864_String(0x97, " L");
		// ������
		// ZCL 2017.6.17 ����д���¶�ʱˢ��ҳ��ʱ��˸
		// LCD12864_String(0x88, "    --    --    ");		//ZCL 2015.9.10
		// ZCL 2017.6.17 �ĳ����£�
		LCD12864_String(0x8A, "--"); // ZCL 2015.9.10
		LCD12864_String(0x8D, "--"); // ZCL 2015.9.10

		// LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "���� 2�Σ�      ");
		// LCD12864_String(0x9F, "Hz");
	}

	/*  DispMod_Par(u8 x_pos,u8 length, u8 XiaoShuBits,  u16 *Address, u8 ParType,
	u8 nb_modpar)	 */
	ModParNum(0);
	// DispMod_Par(0x84, 0x05,0x00, &w_EquipAlarmLast2,1, 0);
	DispMod_Par(0x88, 0x04, 0x00, &w_EquipAlarm2YM, 1, 0);
	DispMod_Par(0x8B, 0x04, 0x00, &w_EquipAlarm2DH, 1, 0);
	DispMod_Par(0x8E, 0x04, 0x00, &w_EquipAlarm2MS, 1, 0);
	//
	FaultDisplay(w_EquipAlarmLast2, 0x91); // ZCL 2018.5.19 ����S_DisplayPar����ʾ���ƣ���ֹƵ��ˢ��
	S_DisplayPar = 0;					   // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_EquipAlarmLast2, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_EquipAlarm2YM, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_EquipAlarm2DH, 1), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_EquipAlarm2MS, 1), 1);
	}
}

void Menu_StmReadPar13(void) // ���� ������
{
	// u8 i;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "������ι��ϼ�¼");
		// �ڶ���
		LCD12864_String(0x90, "3:");
		// LCD12864_String(0x97, " L");
		// ������
		// ZCL 2017.6.17 ����д���¶�ʱˢ��ҳ��ʱ��˸
		// LCD12864_String(0x88, "    --    --    ");		//ZCL 2015.9.10
		// ZCL 2017.6.17 �ĳ����£�
		LCD12864_String(0x8A, "--"); // ZCL 2015.9.10
		LCD12864_String(0x8D, "--"); // ZCL 2015.9.10

		// LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "���� 3�Σ�      ");
		// LCD12864_String(0x9F, "Hz");
	}

	/*  DispMod_Par(u8 x_pos,u8 length, u8 XiaoShuBits,  u16 *Address, u8 ParType,
	u8 nb_modpar)	 */
	ModParNum(0);
	// DispMod_Par(0x84, 0x05,0x00, &w_EquipAlarmLast3,1, 0);
	DispMod_Par(0x88, 0x04, 0x00, &w_EquipAlarm3YM, 1, 0);
	DispMod_Par(0x8B, 0x04, 0x00, &w_EquipAlarm3DH, 1, 0);
	DispMod_Par(0x8E, 0x04, 0x00, &w_EquipAlarm3MS, 1, 0);
	//
	FaultDisplay(w_EquipAlarmLast3, 0x91); // ZCL 2018.5.19 ����S_DisplayPar����ʾ���ƣ���ֹƵ��ˢ��
	S_DisplayPar = 0;					   // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_EquipAlarmLast3, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_EquipAlarm3YM, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_EquipAlarm3DH, 1), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_EquipAlarm3MS, 1), 1);
	}
}

void Menu_StmReadPar14(void) // ���� ������
{
	// u8 i;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "������ι��ϼ�¼");
		// �ڶ���
		LCD12864_String(0x90, "4:");
		// LCD12864_String(0x97, " L");
		// ������
		// ZCL 2017.6.17 ����д���¶�ʱˢ��ҳ��ʱ��˸
		// LCD12864_String(0x88, "    --    --    ");		//ZCL 2015.9.10
		// ZCL 2017.6.17 �ĳ����£�
		LCD12864_String(0x8A, "--"); // ZCL 2015.9.10
		LCD12864_String(0x8D, "--"); // ZCL 2015.9.10

		// LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "���� 4�Σ�      ");
		// LCD12864_String(0x9F, "Hz");
	}

	/*  DispMod_Par(u8 x_pos,u8 length, u8 XiaoShuBits,  u16 *Address, u8 ParType,
	u8 nb_modpar)	 */
	ModParNum(0);
	// DispMod_Par(0x84, 0x05,0x00, &w_EquipAlarmLast4,1, 0);
	DispMod_Par(0x88, 0x04, 0x00, &w_EquipAlarm4YM, 1, 0);
	DispMod_Par(0x8B, 0x04, 0x00, &w_EquipAlarm4DH, 1, 0);
	DispMod_Par(0x8E, 0x04, 0x00, &w_EquipAlarm4MS, 1, 0);
	//
	FaultDisplay(w_EquipAlarmLast4, 0x91); // ZCL 2018.5.19 ����S_DisplayPar����ʾ���ƣ���ֹƵ��ˢ��
	S_DisplayPar = 0;					   // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_EquipAlarmLast4, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_EquipAlarm4YM, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_EquipAlarm4DH, 1), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_EquipAlarm4MS, 1), 1);
	}
}

void Menu_StmReadPar15(void) // ���� ������
{
	// u8 i;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "������ι��ϼ�¼");
		// �ڶ���
		LCD12864_String(0x90, "5:");
		// LCD12864_String(0x97, " L");
		// ������
		// ZCL 2017.6.17 ����д���¶�ʱˢ��ҳ��ʱ��˸
		// LCD12864_String(0x88, "    --    --    ");		//ZCL 2015.9.10
		// ZCL 2017.6.17 �ĳ����£�
		LCD12864_String(0x8A, "--"); // ZCL 2015.9.10
		LCD12864_String(0x8D, "--"); // ZCL 2015.9.10

		// LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "���� 5�Σ�      ");
		// LCD12864_String(0x9F, "Hz");
	}

	/*  DispMod_Par(u8 x_pos,u8 length, u8 XiaoShuBits,  u16 *Address, u8 ParType,
	u8 nb_modpar)	 */
	ModParNum(0);
	// DispMod_Par(0x84, 0x05,0x00, &w_EquipAlarmLast5,1, 0);
	DispMod_Par(0x88, 0x04, 0x00, &w_EquipAlarm5YM, 1, 0);
	DispMod_Par(0x8B, 0x04, 0x00, &w_EquipAlarm5DH, 1, 0);
	DispMod_Par(0x8E, 0x04, 0x00, &w_EquipAlarm5MS, 1, 0);
	//
	FaultDisplay(w_EquipAlarmLast5, 0x91); // ZCL 2018.5.19 ����S_DisplayPar����ʾ���ƣ���ֹƵ��ˢ��
	S_DisplayPar = 0;					   // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_EquipAlarmLast5, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_EquipAlarm5YM, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_EquipAlarm5DH, 1), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_EquipAlarm5MS, 1), 1);
	}
}

void Menu_StmReadPar16(void) // ���� ������
{
	// u8 i;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "������ι��ϼ�¼");
		// �ڶ���
		LCD12864_String(0x90, "6:");
		// LCD12864_String(0x97, " L");
		// ������
		// ZCL 2017.6.17 ����д���¶�ʱˢ��ҳ��ʱ��˸
		// LCD12864_String(0x88, "    --    --    ");		//ZCL 2015.9.10
		// ZCL 2017.6.17 �ĳ����£�
		LCD12864_String(0x8A, "--"); // ZCL 2015.9.10
		LCD12864_String(0x8D, "--"); // ZCL 2015.9.10

		// LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "���� 6�Σ�      ");
		// LCD12864_String(0x9F, "Hz");
	}

	/*  DispMod_Par(u8 x_pos,u8 length, u8 XiaoShuBits,  u16 *Address, u8 ParType,
	u8 nb_modpar)	 */
	ModParNum(0);
	// DispMod_Par(0x84, 0x05,0x00, &w_EquipAlarmLast6,1, 0);
	DispMod_Par(0x88, 0x04, 0x00, &w_EquipAlarm6YM, 1, 0);
	DispMod_Par(0x8B, 0x04, 0x00, &w_EquipAlarm6DH, 1, 0);
	DispMod_Par(0x8E, 0x04, 0x00, &w_EquipAlarm6MS, 1, 0);
	//
	FaultDisplay(w_EquipAlarmLast6, 0x91); // ZCL 2018.5.19 ����S_DisplayPar����ʾ���ƣ���ֹƵ��ˢ��
	S_DisplayPar = 0;					   // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_EquipAlarmLast6, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_EquipAlarm6YM, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_EquipAlarm6DH, 1), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_EquipAlarm6MS, 1), 1);
	}
}

void Menu_StmReadPar17(void) // ���� ������
{
	// u8 i;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "ѡ����ϼ�¼:   ");
		// �ڶ���
		LCD12864_String(0x90, "n:");
		// LCD12864_String(0x97, " L");
		// ������
		// ZCL 2017.6.17 ����д���¶�ʱˢ��ҳ��ʱ��˸
		// LCD12864_String(0x88, "    --    --    ");		//ZCL 2015.9.10
		// ZCL 2017.6.17 �ĳ����£�
		LCD12864_String(0x8A, "--"); // ZCL 2015.9.10
		LCD12864_String(0x8D, "--"); // ZCL 2015.9.10

		// LCD12864_String(0x8F, "Mp");
		// ������
		// ZCL 2017.6.17 ����д���¶�ʱˢ��ҳ��ʱ��˸
		// LCD12864_String(0x98, "����  ��    �Σ�");
		// ZCL 2017.6.17 �ĳ����£�
		LCD12864_String(0x98, "����");
		LCD12864_String(0x9B, "��");
		LCD12864_String(0x9E, "�Σ�");

		// LCD12864_String(0x9F, "Hz");
	}

	/*  DispMod_Par(u8 x_pos,u8 length, u8 XiaoShuBits,  u16 *Address, u8 ParType,
	u8 nb_modpar)	 */
	ModParNum(1);
	F_ModPar1 = 0;
	// DispMod_Par(0x84, 0x05,0x00, &w_SelEquipAlarm,1, 0);
	DispMod_Par(0x88, 0x04, 0x00, &w_SelEquipAlarm1YM, 1, 0);
	DispMod_Par(0x8B, 0x04, 0x00, &w_SelEquipAlarm1DH, 1, 0);
	DispMod_Par(0x8E, 0x04, 0x00, &w_SelEquipAlarm1MS, 1, 0);
	//
	DispMod_Par(0x9A, 0x02, 0x00, &w_FaultRecNum, 1, 0);
	F_ModPar1 = 1;
	DispMod_Par(0x9D, 0x02, 0x00, &w_SelFaultNo, 1, F_ModPar1);
	//
	FaultDisplay(w_SelEquipAlarm, 0x91); // ZCL 2018.5.19 ����S_DisplayPar����ʾ���ƣ���ֹƵ��ˢ��
	//
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&w_SelFaultNo, 1), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 6)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_SelEquipAlarm, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_SelEquipAlarm1YM, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_SelEquipAlarm1DH, 1), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_SelEquipAlarm1MS, 1), 1);
		else if (S_RdWrNo == 5)
			Read_Stm32Data(Address(&w_SelFaultNo, 1), 1);
		else if (S_RdWrNo == 6)
			Read_Stm32Data(Address(&w_FaultRecNum, 1), 1);
	}
}

void Menu_StmReadPar18(void) // ���� ������
{
	// #define TIMEPAGE					7 				//
	// ע��ʱ��ҳ���޸� TIMEPAGE���޸�  ZCL 2015.9.7
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "����ʱ��:       ");
		// LCD12864_String(0x87, "  ");
		// �ڶ���
		// LCD12864_String(0x90, "      ��    ��  ");
		LCD12864_String(0x90, "  ");
		LCD12864_String(0x93, "��"); // ZCL 2017.6.17
		LCD12864_String(0x96, "��  ");
		// LCD12864_String(0x97, "  ");
		// ������
		// LCD12864_String(0x88, "      ��    ʱ  ");
		LCD12864_String(0x88, "  ");
		LCD12864_String(0x8B, "��"); // ZCL 2017.6.17
		LCD12864_String(0x8E, "ʱ  ");

		// LCD12864_String(0x8F, "  ");
		// ������
		// LCD12864_String(0x98, "      ��    ��  ");
		LCD12864_String(0x98, "  ");
		LCD12864_String(0x9B, "��"); // ZCL 2017.6.17
		LCD12864_String(0x9E, "��  ");
		// LCD12864_String(0x8F, "  ");
	}

	ModParNum(6);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x91, 0x04, 0x00, &w_SetYear, 1, F_ModPar1);
	DispMod_Par(0x95, 0x02, 0x00, &w_SetMonth, 1, F_ModPar2);
	DispMod_Par(0x8A, 0x02, 0x00, &w_SetDay, 1, F_ModPar3);
	DispMod_Par(0x8D, 0x02, 0x00, &w_SetHour, 1, F_ModPar4);
	DispMod_Par(0x9A, 0x02, 0x00, &w_SetMinute, 1, F_ModPar5);
	DispMod_Par(0x9D, 0x02, 0x00, &w_SetSecond, 1, F_ModPar6);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&w_SetYear, 1), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&w_SetMonth, 1), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&w_SetDay, 1), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&w_SetHour, 1), Lw_SaveSetValue);
		else if (S_RdWrNo == 5)
			Write_Stm32Data(Address(&w_SetMinute, 1), Lw_SaveSetValue);
		else if (S_RdWrNo == 6)
			Write_Stm32Data(Address(&w_SetSecond, 1), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 6)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_SetYear, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_SetMonth, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_SetDay, 1), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_SetHour, 1), 1);
		else if (S_RdWrNo == 5)
			Read_Stm32Data(Address(&w_SetMinute, 1), 1);
		else if (S_RdWrNo == 6)
			Read_Stm32Data(Address(&w_SetSecond, 1), 1);
	}
}

// ZCL 2017.9.12 ����AI1-AI4��ֵ��������ԡ�
void Menu_StmReadPar19(void) // ���� ������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "AI1 ��ֵ");
		LCD12864_String(0x87, "  ");

		// �ڶ���
		LCD12864_String(0x90, "AI2 ��ֵ");
		LCD12864_String(0x97, "  ");

		// ������
		LCD12864_String(0x88, "AI3 ��ֵ");
		LCD12864_String(0x8F, "  ");
		// LCD12864_String(0x88, "                ");

		// ������
		LCD12864_String(0x98, "AI4 ��ֵ");
		LCD12864_String(0x9F, "  ");
		// LCD12864_String(0x98, "                ");
	}

	ModParNum(0);
	DispMod_Par(0x84, 0x05, 0x00, &w_AI1, 1, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_AI2, 1, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_AI3, 1, 0);
	DispMod_Par(0x9C, 0x05, 0x00, &w_AI4, 1, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_AI1, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_AI2, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_AI3, 1), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_AI4, 1), 1);
	}
}

// ZCL 2019.8.7 ����������״̬DI�����������״̬DO  ��ʾ
void Menu_StmReadPar20(void) // ���� ������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "DI  ״̬");
		LCD12864_String(0x87, "��");
		// �ڶ���
		LCD12864_String(0x90, "--");

		// ������
		LCD12864_String(0x88, "DO  ״̬");
		LCD12864_String(0x8F, "��");
		// ������
		LCD12864_String(0x98, "--");

		Lw_SaveKglInStatus = 1000;
		Lw_SaveKglOutStatus = 1000;
	}

	ModParNum(0);
	DispMod_Par(0x84, 0x05, 0x00, &w_KglInNum, 1, 0);
	// DispMod_Par(0x94, 0x04,0x02, &w_SoftVer,1, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_KglOutNum, 1, 0);
	// DispMod_Par(0x9C, 0x04,0x00, &w_Writetime,1, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (Lw_SaveKglInStatus != w_KglInStatus)
	{
		Lw_SaveKglInStatus = w_KglInStatus;
		// DI״̬����ʾ  ZCL 2019.8.7
		switch (w_KglInStatus)
		{
		case 0:
			LCD12864_String(0x91, "��DI��ͨ      ");
			break; //
		case 1:
			LCD12864_String(0x91, "DI1 ��ͨ      ");
			break; //
		case 2:
			LCD12864_String(0x91, "DI2 ��ͨ      ");
			break; //
		case 3:
			LCD12864_String(0x91, "DI3 ��ͨ      ");
			break; //
		case 4:
			LCD12864_String(0x91, "DI4 ��ͨ      ");
			break; //

		default:
			return;
		}
	}

	if (Lw_SaveKglOutStatus != w_KglOutStatus)
	{
		Lw_SaveKglOutStatus = w_KglOutStatus;
		// DO״̬����ʾ  ZCL 2019.8.7
		switch (w_KglOutStatus)
		{
		case 0:
			LCD12864_String(0x99, "��Q ��ͨ      ");
			break; //
		case 1:
			LCD12864_String(0x99, "Q1��ͨ        ");
			break; //
		case 2:
			LCD12864_String(0x99, "Q2��ͨ        ");
			break; //

		default:
			return;
		}
	}

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		/* 	�ı��İ汾����ͨѶ������DCM200_SCR�Լ��Ĳ��� */
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_KglInNum, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_KglOutNum, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_KglInStatus, 1), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_KglOutStatus, 1), 1);
	}
}

// ZCL 2018.8.21 �綯��״̬
void Menu_StmReadPar21(void) // ���� ������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�綯��״̬");
		LCD12864_String(0x87, "      ");

		// �ڶ���
		LCD12864_String(0x90, "=0��; =1�ر�; ");
		// ������
		LCD12864_String(0x88, "=2����; =3����");
		// ������
		LCD12864_String(0x98, "=4�ر���;       ");
	}

	ModParNum(0);
	DispMod_Par(0x85, 0x03, 0x00, &w_DDFStatus, 1, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 1)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_DDFStatus, 1), 1);
	}
}

void Menu_StmSetPar1(void) // ���� �趨����1
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "                ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		// ������
		LCD12864_String(0x88, "1.��ˮ�趨����  ");
		// ������
		LCD12864_String(0x98, "                ");
	}

	// ZCL 2017.3.21 �����������һ�Σ�������û�в����Ļ��水OK��������ֽ�������޸�״̬����������
	ModParNum(0);
	// ��ʾ���޸Ĳ���
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_StmSetPar2(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�޸Ĳ���");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "�ָ�����");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "������ַ");
		LCD12864_String(0x8F, "��");
		// ������
		LCD12864_String(0x98, "(DCM100)        ");
	}

	ModParNum(3);
	DispMod_Par(0x84, 0x05, 0x00, &w_ModPar, 1, F_ModPar1);
	DispMod_Par(0x94, 0x05, 0x00, &w_ParInitial, 1, F_ModPar2);
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_EquipmentNo, 0, F_ModPar3);
	// DispMod_Par(0x9C, 0x03,0x01, &w_Pump1Current,1, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&w_ModPar, 1), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&w_ParInitial, 1), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_EquipmentNo, 0), Lw_SaveSetValue);
		// else if(S_RdWrNo==4)
		// Write_Stm32Data( Address(&Pw_NoWaterStopP,0), Lw_SaveSetValue );
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_ModPar, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_ParInitial, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_EquipmentNo, 0), 1);
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_NoWaterStopP,0), 1 );
	}
}

void Menu_StmSetPar3(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��ˮ����");
		LCD12864_String(0x87, "Mp");
		// �ڶ���
		LCD12864_String(0x90, "��ˮ����");
		LCD12864_String(0x97, "Mp");
		// ������
		LCD12864_String(0x88, "�ӻ�ѹ��");
		LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "�ӻ�����YS");
		LCD12864_String(0x9F, " S");
	}

	ModParNum(4); // ZCL 2019.8.7 3
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x04, 0x03, &Pw_InPDecMax, 0, F_ModPar1);
	DispMod_Par(0x94, 0x04, 0x03, &Pw_OutPDecMax, 0, F_ModPar2);
	DispMod_Par(0x8C, 0x04, 0x02, &Pw_SetP, 0, F_ModPar3);			  // ZCL 2018.5.17 ѹ��С��λ3������
	DispMod_Par(0x9D, 0x03, 0x00, &Pw_SlaveStartDelay, 0, F_ModPar4); // ZCL 2019.8.7
	S_DisplayPar = 0;												  // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_InPDecMax, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_OutPDecMax, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_SetP, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_SlaveStartDelay, 0), Lw_SaveSetValue); // ZCL 2019.8.7
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4) // ZCL 2019.8.7  3
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_InPDecMax, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_OutPDecMax, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_SetP, 0), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_SlaveStartDelay, 0), 1); // ZCL 2019.8.7
	}
}

void Menu_StmSetPar4(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "������ʱEn"); // ZCL 5��������
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "������ʱ��");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "                ");
		// ������
		LCD12864_String(0x98, "����������������");
	}

	ModParNum(2);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x85, 0x03, 0x00, &Pw_EnableTime, 0, F_ModPar1);
	DispMod_Par(0x95, 0x03, 0x00, &w_AutoAssignTime, 1, F_ModPar2);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_EnableTime, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&w_AutoAssignTime, 1), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 2)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_EnableTime, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_AutoAssignTime, 1), 1);
	}
}

void Menu_StmSetPar5(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "  ����ʱ��ѹ��  ");
		// LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "����1Tim");
		LCD12864_String(0x97, "HM");
		// ������
		LCD12864_String(0x88, "��ͣ1Tim");
		LCD12864_String(0x8F, "HM");
		// ������
		LCD12864_String(0x98, "��1 ѹ��");
		LCD12864_String(0x9F, "Mp");
	}

	ModParNum(3);
	// ��ʾ���޸Ĳ���
	// DispMod_Par(0x85, 0x03,0x00, &Pw_EnableTime,0, F_ModPar1);
	DispMod_Par(0x94, 0x05, 0x00, &Pw_Time1Run, 0, F_ModPar1);
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_Time1Stop, 0, F_ModPar2);
	DispMod_Par(0x9C, 0x04, 0x02, &Pw_Time1SetP, 0, F_ModPar3); // ZCL 2018.5.17 ѹ��С��λ3������
	S_DisplayPar = 0;											// ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		// if(S_RdWrNo==1)
		// Write_Stm32Data( Address(&Pw_EnableTime,0), Lw_SaveSetValue );
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_Time1Run, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_Time1Stop, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_Time1SetP, 0), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		//  if(S_RdWrNo==1)
		//  Read_Stm32Data( Address(&Pw_EnableTime,0), 1 );
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_Time1Run, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_Time1Stop, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_Time1SetP, 0), 1);
	}
}

void Menu_StmSetPar6(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "����2Tim");
		LCD12864_String(0x87, "HM");
		// �ڶ���
		LCD12864_String(0x90, "��ͣ2Tim");
		LCD12864_String(0x97, "HM");
		// ������
		LCD12864_String(0x88, "��2 ѹ��");
		LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "������ʱ���GN��");
		// LCD12864_String(0x9F, "  ");
	}

	ModParNum(3);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_Time2Run, 0, F_ModPar1);
	DispMod_Par(0x94, 0x05, 0x00, &Pw_Time2Stop, 0, F_ModPar2);
	DispMod_Par(0x8C, 0x04, 0x02, &Pw_Time2SetP, 0, F_ModPar3); // ZCL 2018.5.17 ѹ��С��λ3������
	// DispMod_Par(0x9C, 0x04,0x00, &Pw_OutPSensorZero,0, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_Time2Run, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_Time2Stop, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_Time2SetP, 0), Lw_SaveSetValue);
		// else if(S_RdWrNo==4)
		// Write_Stm32Data( Address(&Pw_OutPSensorZero,0), Lw_SaveSetValue );
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_Time2Run, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_Time2Stop, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_Time2SetP, 0), 1);
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_OutPSensorZero,0), 1 );
	}
}

void Menu_StmSetPar7(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "����3Tim");
		LCD12864_String(0x87, "HM");
		// �ڶ���
		LCD12864_String(0x90, "��ͣ3Tim");
		LCD12864_String(0x97, "HM");
		// ������
		LCD12864_String(0x88, "��3 ѹ��");
		LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "������ʱ���GN��");
		// LCD12864_String(0x9F, "  ");
	}

	ModParNum(3);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_Time3Run, 0, F_ModPar1);
	DispMod_Par(0x94, 0x05, 0x00, &Pw_Time3Stop, 0, F_ModPar2);
	DispMod_Par(0x8C, 0x04, 0x02, &Pw_Time3SetP, 0, F_ModPar3); // ZCL 2018.5.17 ѹ��С��λ3������
	// DispMod_Par(0x9C, 0x04,0x00, &Pw_OutPSensorZero,0, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_Time3Run, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_Time3Stop, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_Time3SetP, 0), Lw_SaveSetValue);
		// else if(S_RdWrNo==4)
		// Write_Stm32Data( Address(&Pw_OutPSensorZero,0), Lw_SaveSetValue );
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_Time3Run, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_Time3Stop, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_Time3SetP, 0), 1);
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_OutPSensorZero,0), 1 );
	}
}

void Menu_StmSetPar8(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "����4Tim");
		LCD12864_String(0x87, "HM");
		// �ڶ���
		LCD12864_String(0x90, "��ͣ4Tim");
		LCD12864_String(0x97, "HM");
		// ������
		LCD12864_String(0x88, "��4 ѹ��");
		LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "������ʱ���GN��");
		// LCD12864_String(0x9F, "  ");
	}

	ModParNum(3);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_Time4Run, 0, F_ModPar1);
	DispMod_Par(0x94, 0x05, 0x00, &Pw_Time4Stop, 0, F_ModPar2);
	DispMod_Par(0x8C, 0x04, 0x02, &Pw_Time4SetP, 0, F_ModPar3); // ZCL 2018.5.17 ѹ��С��λ3������
	// DispMod_Par(0x9C, 0x04,0x00, &Pw_OutPSensorZero,0, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_Time4Run, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_Time4Stop, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_Time4SetP, 0), Lw_SaveSetValue);
		// else if(S_RdWrNo==4)
		// Write_Stm32Data( Address(&Pw_OutPSensorZero,0), Lw_SaveSetValue );
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_Time4Run, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_Time4Stop, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_Time4SetP, 0), 1);
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_OutPSensorZero,0), 1 );
	}
}

void Menu_StmSetPar9(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����

		// ��һ��
		LCD12864_String(0x80, "��ˮ��ѹ");
		LCD12864_String(0x87, "Mp");
		// �ڶ���
		LCD12864_String(0x90, "��ˮͣѹ");
		LCD12864_String(0x97, "Mp");
		// ������
		LCD12864_String(0x88, "��ˮ��ʱ");
		LCD12864_String(0x8F, " S");
		// ������
		LCD12864_String(0x98, "��ˮ��ʱ");
		LCD12864_String(0x9F, " S");
	}

	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x04, 0x02, &Pw_HaveWaterRunP, 0, F_ModPar1); // ZCL 2018.5.17 ѹ��С��λ3������
	DispMod_Par(0x94, 0x04, 0x02, &Pw_NoWaterStopP, 0, F_ModPar2);	// ZCL 2018.5.17 ѹ��С��λ3������
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_HaveWaterRunDelay, 0, F_ModPar3);
	DispMod_Par(0x9C, 0x05, 0x00, &Pw_NoWaterStopDelay, 0, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_HaveWaterRunP, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_NoWaterStopP, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_HaveWaterRunDelay, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_NoWaterStopDelay, 0), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_HaveWaterRunP, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_NoWaterStopP, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_HaveWaterRunDelay, 0), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_NoWaterStopDelay, 0), 1);
	}
}

void Menu_StmSetPar10(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "PIDS����");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "PIDS����");
		LCD12864_String(0x97, "��");
		// ������
		LCD12864_String(0x88, "PIDS����");
		LCD12864_String(0x8F, "��");
		// ������
		LCD12864_String(0x98, "                ");
	}

	ModParNum(3);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x04, 0x03, &Pw_PID_Kc, 0, F_ModPar1);
	DispMod_Par(0x94, 0x04, 0x03, &Pw_PID_Ts, 0, F_ModPar2);
	DispMod_Par(0x8C, 0x04, 0x03, &Pw_PID_Ti, 0, F_ModPar3);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_PID_Kc, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_PID_Ts, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_PID_Ti, 0), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_PID_Kc, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_PID_Ts, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_PID_Ti, 0), 1);
	}
}

void Menu_StmSetPar11(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "PIDX����");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "PIDX����");
		LCD12864_String(0x97, "��");
		// ������
		LCD12864_String(0x88, "PIDX����");
		LCD12864_String(0x8F, "��");
		// ������
		LCD12864_String(0x98, "                ");
	}

	ModParNum(3); // ZCL 2019.8.7 3
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x04, 0x03, &Pw_DownPID_Kc, 0, F_ModPar1);
	DispMod_Par(0x94, 0x04, 0x03, &Pw_DownPID_Ts, 0, F_ModPar2);
	DispMod_Par(0x8C, 0x04, 0x03, &Pw_DownPID_Ti, 0, F_ModPar3);
	// DispMod_Par(0x9C, 0x04,0x00, &Pw_EquipType,0, F_ModPar4);			//ZCL 2019.8.7
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_DownPID_Kc, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_DownPID_Ts, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_DownPID_Ti, 0), Lw_SaveSetValue);
		// else if(S_RdWrNo==4)
		// Write_Stm32Data( Address(&Pw_EquipType,0), Lw_SaveSetValue );			//ZCL 2019.8.7
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3) // ZCL 2019.8.7 3
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_DownPID_Kc, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_DownPID_Ts, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_DownPID_Ti, 0), 1);
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_EquipType,0), 1 );				//ZCL 2019.8.7
	}
}

void Menu_StmSetPar12(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��ˮ����");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "��ˮ����");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "��ˮ��ֵ");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "��ˮ��ֵ");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_InPType, 0, F_ModPar1);
	DispMod_Par(0x94, 0x05, 0x00, &Pw_OutPType, 0, F_ModPar2);
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_InPSensorZero, 0, F_ModPar3);
	DispMod_Par(0x9C, 0x05, 0x00, &Pw_OutPSensorZero, 0, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_InPType, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_OutPType, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_InPSensorZero, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_OutPSensorZero, 0), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_InPType, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_OutPType, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_InPSensorZero, 0), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_OutPSensorZero, 0), 1);
	}
}

void Menu_StmSetPar13(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�����ѹEn");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "��ѹƫ��");
		LCD12864_String(0x97, "Mp");
		// ������
		LCD12864_String(0x88, "��ѹ��ʱ");
		LCD12864_String(0x8F, "MS");
		// ������
		LCD12864_String(0x98, "�������ѹ���ܣ�");
		// LCD12864_String(0x9F, "  ");
	}

	ModParNum(3);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x85, 0x05, 0x00, &Pw_SoftOverPEn, 0, F_ModPar1);
	DispMod_Par(0x94, 0x04, 0x03, &Pw_OverPThanSetPEk, 0, F_ModPar2);
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_SoftOverPDelay, 0, F_ModPar3);
	// DispMod_Par(0x9C, 0x04,0x00, &Pw_OutPSensorZero,0, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_SoftOverPEn, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_OverPThanSetPEk, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_SoftOverPDelay, 0), Lw_SaveSetValue);
		// else if(S_RdWrNo==4)
		// Write_Stm32Data( Address(&Pw_OutPSensorZero,0), Lw_SaveSetValue );
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_SoftOverPEn, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_OverPThanSetPEk, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_SoftOverPDelay, 0), 1);
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_OutPSensorZero,0), 1 );
	}
}

void Menu_StmSetPar14(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��ˮѹ�߽���En");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "ͣ����ʱ");
		LCD12864_String(0x97, " S");
		// ������
		LCD12864_String(0x88, "������ʱ");
		LCD12864_String(0x8F, " S");
		// ������
		LCD12864_String(0x98, "����ˮ�߽���GN��");
		// LCD12864_String(0x9F, "  ");
	}

	ModParNum(3);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x87, 0x01, 0x00, &Pw_InPBigSetEn, 0, F_ModPar1);
	DispMod_Par(0x94, 0x05, 0x00, &Pw_InPBigSetDelay, 0, F_ModPar2);
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_InPNormalDelay, 0, F_ModPar3);
	// DispMod_Par(0x9C, 0x04,0x00, &Pw_OutPSensorZero,0, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_InPBigSetEn, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_InPBigSetDelay, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_InPNormalDelay, 0), Lw_SaveSetValue);
		// else if(S_RdWrNo==4)
		// Write_Stm32Data( Address(&Pw_OutPSensorZero,0), Lw_SaveSetValue );
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_InPBigSetEn, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_InPBigSetDelay, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_InPNormalDelay, 0), 1);
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_OutPSensorZero,0), 1 );
	}
}

void Menu_StmSetPar15(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��ˮѹ�߱���En");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "������ʱ");
		LCD12864_String(0x97, " S");
		// ������
		//  LCD12864_String(0x88, "������ʱ");
		//  LCD12864_String(0x8F, " S");
		LCD12864_String(0x88, "                ");
		// ������
		LCD12864_String(0x98, "����ˮ�߱���GN��");
		// LCD12864_String(0x9F, "  ");
	}

	ModParNum(2);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x87, 0x01, 0x00, &Pw_InPBigOutEn, 0, F_ModPar1);
	DispMod_Par(0x94, 0x05, 0x00, &Pw_InPBigOutDelay, 0, F_ModPar2);
	// DispMod_Par(0x8C, 0x04,0x00, &Pw_InPNormalDelay,0, F_ModPar3);
	// DispMod_Par(0x9C, 0x04,0x00, &Pw_OutPSensorZero,0, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_InPBigOutEn, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_InPBigOutDelay, 0), Lw_SaveSetValue);
		// else if(S_RdWrNo==3)
		// Write_Stm32Data( Address(&Pw_InPNormalDelay,0), Lw_SaveSetValue );
		// else if(S_RdWrNo==4)
		// Write_Stm32Data( Address(&Pw_OutPSensorZero,0), Lw_SaveSetValue );
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 2)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_InPBigOutEn, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_InPBigOutDelay, 0), 1);
		// else if(S_RdWrNo==3)
		// Read_Stm32Data( Address(&Pw_InPNormalDelay,0), 1 );
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_OutPSensorZero,0), 1 );
	}
}

void Menu_StmSetPar16(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "����ʱ");
		LCD12864_String(0x87, "��"); // ZCL 2015.9.23
		// �ڶ���
		// �ڶ���
		LCD12864_String(0x90, "                ");
		// ������
		LCD12864_String(0x88, "                ");
		// ������
		LCD12864_String(0x98, "��ѹ�����л�GN��");
		// LCD12864_String(0x9F, "  ");
	}

	ModParNum(1);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_PSensorProtectDelay, 0, F_ModPar1);
	// DispMod_Par(0x94, 0x04,0x00, &Pw_InPBigOutDelay,0, F_ModPar2);
	// DispMod_Par(0x8C, 0x04,0x00, &Pw_InPNormalDelay,0, F_ModPar3);
	// DispMod_Par(0x9C, 0x04,0x00, &Pw_OutPSensorZero,0, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_PSensorProtectDelay, 0), Lw_SaveSetValue);
		// else if(S_RdWrNo==2)
		// Write_Stm32Data( Address(&Pw_InPBigOutDelay,0), Lw_SaveSetValue );
		// else if(S_RdWrNo==3)
		// Write_Stm32Data( Address(&Pw_InPNormalDelay,0), Lw_SaveSetValue );
		// else if(S_RdWrNo==4)
		// Write_Stm32Data( Address(&Pw_OutPSensorZero,0), Lw_SaveSetValue );
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 1)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_PSensorProtectDelay, 0), 1);
		// else if(S_RdWrNo==2)
		// Read_Stm32Data( Address(&Pw_InPBigOutDelay,0), 1 );
		// else if(S_RdWrNo==3)
		// Read_Stm32Data( Address(&Pw_InPNormalDelay,0), 1 );
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_OutPSensorZero,0), 1 );
	}
}

void Menu_StmSetPar17(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��ͣʹ��");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "ÿ���Ƶ");
		LCD12864_String(0x97, "Hz");
		// ������
		//  LCD12864_String(0x88, "������ʱ");
		//  LCD12864_String(0x8F, " S");

		LCD12864_String(0x88, "                ");
		// ������
		LCD12864_String(0x98, "���豸��ͣ���ܣ�");
		// LCD12864_String(0x9F, "  ");
	}

	ModParNum(2);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_PumpSoftStopEn, 0, F_ModPar1);
	DispMod_Par(0x94, 0x04, 0x01, &Pw_SoftStopDownDec, 0, F_ModPar2);
	// DispMod_Par(0x8C, 0x04,0x00, &Pw_InPNormalDelay,0, F_ModPar3);
	// DispMod_Par(0x9C, 0x04,0x00, &Pw_OutPSensorZero,0, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_PumpSoftStopEn, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_SoftStopDownDec, 0), Lw_SaveSetValue);
		// else if(S_RdWrNo==3)
		// Write_Stm32Data( Address(&Pw_InPNormalDelay,0), Lw_SaveSetValue );
		// else if(S_RdWrNo==4)
		// Write_Stm32Data( Address(&Pw_OutPSensorZero,0), Lw_SaveSetValue );
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 2)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_PumpSoftStopEn, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_SoftStopDownDec, 0), 1);
		// else if(S_RdWrNo==3)
		// Read_Stm32Data( Address(&Pw_InPNormalDelay,0), 1 );
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_OutPSensorZero,0), 1 );
	}
}

void Menu_StmSetPar18(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��ˮ��ƵEn");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "���趨ѹ");
		LCD12864_String(0x97, "Mp");
		// ������
		LCD12864_String(0x88, "ÿ�뽵Ƶ");
		LCD12864_String(0x8F, "Hz");
		// ������
		LCD12864_String(0x98, "���Ƶ��");
		LCD12864_String(0x9F, "Hz");
	}

	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x85, 0x03, 0x00, &Pw_InP_CTLEn, 0, F_ModPar1);
	DispMod_Par(0x94, 0x04, 0x03, &Pw_SetInP, 0, F_ModPar2);
	DispMod_Par(0x8C, 0x04, 0x01, &Pw_VfFreqDownSircle, 0, F_ModPar3);
	DispMod_Par(0x9C, 0x04, 0x01, &Pw_VfFreqDownMin, 0, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_InP_CTLEn, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_SetInP, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_VfFreqDownSircle, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_VfFreqDownMin, 0), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_InP_CTLEn, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_SetInP, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_VfFreqDownSircle, 0), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_VfFreqDownMin, 0), 1);
	}
}

void Menu_StmSetPar19(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "СLLʹ��");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "СLLƵ��");
		LCD12864_String(0x97, "Hz");
		// ������
		LCD12864_String(0x88, "��ѹƫ��");
		LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "��ѹ��ʱ");
		LCD12864_String(0x9F, " S");
	}

	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_EnableSmall, 0, F_ModPar1);
	DispMod_Par(0x94, 0x04, 0x01, &Pw_InSmallFreqMinValue, 0, F_ModPar2);
	DispMod_Par(0x8C, 0x04, 0x03, &Pw_SmallStablePEk, 0, F_ModPar3);
	DispMod_Par(0x9C, 0x05, 0x00, &Pw_SmallJudgeDelay, 0, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_EnableSmall, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_InSmallFreqMinValue, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_SmallStablePEk, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_SmallJudgeDelay, 0), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_EnableSmall, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_InSmallFreqMinValue, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_SmallStablePEk, 0), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_SmallJudgeDelay, 0), 1);
	}
}

void Menu_StmSetPar20(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "СLLѹ��");
		LCD12864_String(0x87, "Mp");
		// �ڶ���
		LCD12864_String(0x90, "СLL���");
		LCD12864_String(0x97, " S");
		// ������
		LCD12864_String(0x88, "СLL�ƻ�");
		LCD12864_String(0x8F, " S");
		// ������
		LCD12864_String(0x98, "СLL��ѹ_r");
		LCD12864_String(0x9F, " S");
	}

	ModParNum(3);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x04, 0x02, &Pw_SmallHoldP, 0, F_ModPar1); // ZCL 2018.5.17 ѹ��С��λ3������
	DispMod_Par(0x94, 0x05, 0x00, &Pw_BetweenSmallSec, 0, F_ModPar2);
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_SmallBadClrSec, 0, F_ModPar3);
	DispMod_Par(0x9D, 0x03, 0x00, &w_SmallStableRunSec, 1, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_SmallHoldP, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_BetweenSmallSec, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_SmallBadClrSec, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&w_SmallStableRunSec, 1), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_SmallHoldP, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_BetweenSmallSec, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_SmallBadClrSec, 0), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_SmallStableRunSec, 1), 1);
	}
}

void Menu_StmSetPar21(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "СLL����");
		LCD12864_String(0x87, "��");
		// �ڶ���
		LCD12864_String(0x90, "����ѹ-1En");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "�˳�ƫ��");
		LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "ÿ����Ƶ");
		LCD12864_String(0x9F, "Hz");
	}

	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_SmallRiseSec, 0, F_ModPar1);
	DispMod_Par(0x95, 0x03, 0x00, &Pw_SmallNoStableSubOneEn, 0, F_ModPar2);
	DispMod_Par(0x8C, 0x04, 0x03, &Pw_ExitSmallEk, 0, F_ModPar3);
	DispMod_Par(0x9C, 0x04, 0x01, &Pw_SmallVfUpValue, 0, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_SmallRiseSec, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_SmallNoStableSubOneEn, 0), Lw_SaveSetValue);
		if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_ExitSmallEk, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_SmallVfUpValue, 0), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_SmallRiseSec, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_SmallNoStableSubOneEn, 0), 1);
		if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_ExitSmallEk, 0), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_SmallVfUpValue, 0), 1);
	}
}

void Menu_StmSetPar22(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "Max AIֵ");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "MaxPIDֵ");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "Max Vfֵ");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "��СƵ��");
		LCD12864_String(0x9F, "Hz");
	}

	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_MaxAIHex, 0, F_ModPar1);
	DispMod_Par(0x94, 0x05, 0x00, &Pw_MaxPIDHex, 0, F_ModPar2);
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_MaxFreqHex, 0, F_ModPar3);
	DispMod_Par(0x9C, 0x04, 0x01, &Pw_MinRunFreqDec, 0, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_MaxAIHex, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_MaxPIDHex, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_MaxFreqHex, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_MinRunFreqDec, 0), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_MaxAIHex, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_MaxPIDHex, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_MaxFreqHex, 0), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_MinRunFreqDec, 0), 1);
	}
}

void Menu_StmSetPar23(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��ѹ����ʱ");
		LCD12864_String(0x87, " S");
		// �ڶ���
		LCD12864_String(0x90, "��ѹ����ʱ");
		LCD12864_String(0x97, " S");
		// ������
		LCD12864_String(0x88, "��ѹϵ��");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "��ˮƫ��");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x85, 0x03, 0x00, &Pw_HengYaLongDelay, 0, F_ModPar1);
	DispMod_Par(0x95, 0x03, 0x00, &Pw_JiaYaShortDelay, 0, F_ModPar2);
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_InPChangeOutPXiShu, 0, F_ModPar3);
	DispMod_Par(0x9C, 0x05, 0x00, &Pw_InPChangeEk, 0, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_HengYaLongDelay, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_JiaYaShortDelay, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_InPChangeOutPXiShu, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_InPChangeEk, 0), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_HengYaLongDelay, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_JiaYaShortDelay, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_InPChangeOutPXiShu, 0), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_InPChangeEk, 0), 1);
	}
}

void Menu_StmSetPar24(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��Ƶģʽ");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "��ƵƵ��");
		LCD12864_String(0x97, "Hz");
		// ������
		LCD12864_String(0x88, "                ");
		// ������
		LCD12864_String(0x98, "����ģʽ�ֶ���");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(2);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_ZDRunMode, 0, F_ModPar1);
	DispMod_Par(0x94, 0x04, 0x01, &Pw_ZDRunHz, 0, F_ModPar2);
	// DispMod_Par(0x8C, 0x04,0x00, &Pw_MaxFreqHex,0, F_ModPar3);
	// DispMod_Par(0x9C, 0x04,0x00, &Pw_ScrFastKeyBetweenMS,0, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_ZDRunMode, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_ZDRunHz, 0), Lw_SaveSetValue);
		// else if(S_RdWrNo==3)
		// Write_Stm32Data( Address(&Pw_MaxFreqHex,0), Lw_SaveSetValue );
		// else if(S_RdWrNo==4)
		// Write_Stm32Data( Address(&Pw_ScrFastKeyBetweenMS,0), Lw_SaveSetValue );
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 2)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_ZDRunMode, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_ZDRunHz, 0), 1);
		// else if(S_RdWrNo==3)
		// Read_Stm32Data( Address(&Pw_MaxFreqHex,0), 1 );
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_ScrFastKeyBetweenMS,0), 1 );
	}
}

void Menu_StmSetPar25(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��ˮƫ���");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "��Ƶ��λ��"); // ZCL 2018.3.8
		LCD12864_String(0x97, "��");
		// ������
		LCD12864_String(0x88, "��Ƶ��λ��"); // ZCL 2018.3.8
		LCD12864_String(0x8F, "��");

		// ������
		LCD12864_String(0x98, "�ѱ�������"); // ZCL 2018.3.8
		LCD12864_String(0x9F, "��");
	}

	ModParNum(3);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x86, 0x02, 0x00, &Pw_InPPidEkXiShu, 0, F_ModPar1); // 0x84
	DispMod_Par(0x95, 0x03, 0x00, &Pw_VvvfResetMax, 0, F_ModPar2);	// 0x94
	DispMod_Par(0x8D, 0x03, 0x00, &Pw_VvvfResetSec, 0, F_ModPar3);	// 0x9C
	DispMod_Par(0x9D, 0x03, 0x00, &w_VvvfAlmNum, 1, 0);				// 0x8C

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_InPPidEkXiShu, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_VvvfResetMax, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_VvvfResetSec, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&w_VvvfAlmNum, 1), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_InPPidEkXiShu, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_VvvfResetMax, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_VvvfResetSec, 0), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_VvvfAlmNum, 1), 1);
	}
}

void Menu_StmSetPar26(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "ѡ�������");
		// LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "    ����ֵ");
		// LCD12864_String(0x97, "Hz");
		// ������
		LCD12864_String(0x88, "�������");
		LCD12864_String(0x8F, "MS");
		// ������
		// LCD12864_String(0x98, "                ");
		LCD12864_String(0x98, "��Ƶ������"); // ZCL 2018.6.13
											 // LCD12864_String(0x9F, " S");
	}

	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x85, 0x05, 0x00, &w_SelPar, 1, F_ModPar1);
	DispMod_Par(0x95, 0x05, 0x00, &w_SelParValue, 1, F_ModPar2);
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_ScrFastKeyBetweenMS, 0, F_ModPar3);
	DispMod_Par(0x9D, 0x05, 0x00, &Pw_VfAlarmSubOneSecs, 0, F_ModPar4);
	// DispMod_Par(0x9C, 0x04,0x00, &Pw_ScrFastKeyBetweenMS,0, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&w_SelPar, 1), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&w_SelParValue, 1), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_ScrFastKeyBetweenMS, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_VfAlarmSubOneSecs, 0), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_SelPar, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_SelParValue, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_ScrFastKeyBetweenMS, 0), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_VfAlarmSubOneSecs, 0), 1);
	}
}

void Menu_StmSetPar27(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "ˮԴ�л�ʹ��:   "); // ZCL 2018.8.21 һ������2������
		// LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		LCD12864_String(0x91, "(0-1) ");
		// ������
		LCD12864_String(0x88, "DDF �ֶ�ʹ��:   ");
		// LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
		LCD12864_String(0x99, "(0-1) ");
	}

	ModParNum(2);
	// ��ʾ���޸Ĳ���
	// DispMod_Par(0x85, 0x05,0x00, &w_SelPar,1, F_ModPar1);
	DispMod_Par(0x94, 0x05, 0x00, &Pw_SelWaterToWater, 0, F_ModPar1);
	// DispMod_Par(0x8C, 0x05,0x00, &Pw_ScrFastKeyBetweenMS,0, F_ModPar2);
	DispMod_Par(0x9C, 0x05, 0x00, &Pw_DDFManuAutoCtlEn, 0, F_ModPar2);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_SelWaterToWater, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_DDFManuAutoCtlEn, 0), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 2)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_SelWaterToWater, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_DDFManuAutoCtlEn, 0), 1);
	}
}

void Menu_StmSetPar28(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "���綯��ʱ��:   "); // ZCL 2018.8.21 һ������2������
		// LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		LCD12864_String(0x97, "��");
		// ������
		LCD12864_String(0x88, "�ص綯��ʱ��:   ");
		// LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
		LCD12864_String(0x9F, "��");
	}

	ModParNum(2);
	// ��ʾ���޸Ĳ���
	// DispMod_Par(0x85, 0x05,0x00, &w_SelPar,1, F_ModPar1);
	DispMod_Par(0x93, 0x05, 0x00, &Pw_DDFOpenDelay, 0, F_ModPar1);
	// DispMod_Par(0x8C, 0x05,0x00, &Pw_ScrFastKeyBetweenMS,0, F_ModPar2);
	DispMod_Par(0x9B, 0x05, 0x00, &Pw_DDFCloseDelay, 0, F_ModPar2);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_DDFOpenDelay, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_DDFCloseDelay, 0), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 2)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_DDFOpenDelay, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_DDFCloseDelay, 0), 1);
	}
}

void Menu_StmSetPar29(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "����ˮ�л�ʱ��:   "); // ZCL 2018.8.21 һ������2������
		// LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		LCD12864_String(0x97, "��");
		// ������
		LCD12864_String(0x88, "����ˮ�л���ʱ:   ");
		// LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
		LCD12864_String(0x9F, "��");
	}

	ModParNum(1);
	// ��ʾ���޸Ĳ���
	// DispMod_Par(0x85, 0x05,0x00, &w_SelPar,1, F_ModPar1);
	DispMod_Par(0x93, 0x05, 0x00, &Pw_ChangeWaterSourceHour, 0, F_ModPar1);
	// DispMod_Par(0x8C, 0x05,0x00, &Pw_ScrFastKeyBetweenMS,0, F_ModPar3);
	DispMod_Par(0x9B, 0x05, 0x00, &w_ChangeWaterRunningHour, 1, 0);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_ChangeWaterSourceHour, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&w_ChangeWaterRunningHour, 1), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 2)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_ChangeWaterSourceHour, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_ChangeWaterRunningHour, 1), 1);
	}
}

void Menu_StmSetPar30(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "ˮ���л�ʱ��:     "); // ZCL 2018.8.21 һ������2������
		// LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		LCD12864_String(0x97, "��");
		// ������
		LCD12864_String(0x88, "ˮ���л���ʱ:     ");
		// LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
		LCD12864_String(0x9F, "��");
	}

	ModParNum(1);
	// ��ʾ���޸Ĳ���
	// DispMod_Par(0x85, 0x05,0x00, &w_SelPar,1, F_ModPar1);
	DispMod_Par(0x93, 0x05, 0x00, &Pw_ChangeWaterSourceHour2, 0, F_ModPar1);
	// DispMod_Par(0x8C, 0x05,0x00, &Pw_ScrFastKeyBetweenMS,0, F_ModPar3);
	DispMod_Par(0x9B, 0x05, 0x00, &w_ChangeWaterRunningHour2, 1, 0);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_ChangeWaterSourceHour2, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&w_ChangeWaterRunningHour2, 1), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 2)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_ChangeWaterSourceHour2, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_ChangeWaterRunningHour2, 1), 1);
	}
}

void Menu_StmSetPar31(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "����ط�ʱ��1:  "); // ZCL 2018.8.21 һ������2������
		// LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		LCD12864_String(0x97, "HM");
		// ������
		LCD12864_String(0x88, "���㿪��ʱ��1:   ");
		// LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
		LCD12864_String(0x9F, "HM");
	}

	ModParNum(2);
	// ��ʾ���޸Ĳ���
	// DispMod_Par(0x85, 0x05,0x00, &w_SelPar,1, F_ModPar1);
	DispMod_Par(0x93, 0x05, 0x00, &Pw_Time1HM, 0, F_ModPar1);
	// DispMod_Par(0x8C, 0x05,0x00, &Pw_ScrFastKeyBetweenMS,0, F_ModPar3);
	DispMod_Par(0x9B, 0x05, 0x00, &Pw_Time3HM, 0, F_ModPar2);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_Time1HM, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_Time3HM, 0), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 2)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_Time1HM, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_Time3HM, 0), 1);
	}
}

void Menu_StmSetPar32(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "����ط�ʱ��2:  "); // ZCL 2018.8.21 һ������2������
		// LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		LCD12864_String(0x97, "HM");
		// ������
		LCD12864_String(0x88, "���㿪��ʱ��2:   ");
		// LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
		LCD12864_String(0x9F, "HM");
	}

	ModParNum(2);
	// ��ʾ���޸Ĳ���
	// DispMod_Par(0x85, 0x05,0x00, &w_SelPar,1, F_ModPar1);
	DispMod_Par(0x93, 0x05, 0x00, &Pw_Time2HM, 0, F_ModPar1);
	// DispMod_Par(0x8C, 0x05,0x00, &Pw_ScrFastKeyBetweenMS,0, F_ModPar3);
	DispMod_Par(0x9B, 0x05, 0x00, &Pw_Time4HM, 0, F_ModPar2);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_Time2HM, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_Time4HM, 0), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 2)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_Time2HM, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_Time4HM, 0), 1);
	}
}

void Menu_StmSetPar33(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "����ط�ʱ��3:  "); // ZCL 2018.8.21 һ������2������
		// LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		LCD12864_String(0x97, "HM");
		// ������
		LCD12864_String(0x88, "���㿪��ʱ��3:   ");
		// LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
		LCD12864_String(0x9F, "HM");
	}

	ModParNum(2);
	// ��ʾ���޸Ĳ���
	// DispMod_Par(0x85, 0x05,0x00, &w_SelPar,1, F_ModPar1);
	DispMod_Par(0x93, 0x05, 0x00, &Pw_Time5HM, 0, F_ModPar1);
	// DispMod_Par(0x8C, 0x05,0x00, &Pw_ScrFastKeyBetweenMS,0, F_ModPar3);
	DispMod_Par(0x9B, 0x05, 0x00, &Pw_Time6HM, 0, F_ModPar2);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_Time5HM, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_Time6HM, 0), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 2)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_Time5HM, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_Time6HM, 0), 1);
	}
}

void Menu_StmSetPar34(void) // ���� �趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�綯���ֶ�����: "); // ZCL 2018.8.21 һ������2������  2019.8.7��
		// LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "�����Ƶ���:   "); // ZCL 2019.8.10
		// LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(2);
	// ��ʾ���޸Ĳ���
	// DispMod_Par(0x85, 0x05,0x00, &w_SelPar,1, F_ModPar1);
	DispMod_Par(0x93, 0x05, 0x00, &w_TouchDDFOnOFF, 1, F_ModPar1);
	DispMod_Par(0x8E, 0x02, 0x00, &w_DelVvvfAlarmed, 1, F_ModPar2);
	// DispMod_Par(0x9B, 0x05,0x00, &Pw_Time6HM,0, F_ModPar2);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&w_TouchDDFOnOFF, 1), Lw_SaveSetValue);
		else if (S_RdWrNo == 2) // ZCL 2022.10.3
			Write_Stm32Data(Address(&w_DelVvvfAlarmed, 1), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 2) // ZCL 2022.10.3
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_TouchDDFOnOFF, 1), 1);
		else if (S_RdWrNo == 2) // ZCL 2022.10.3
			Read_Stm32Data(Address(&w_DelVvvfAlarmed, 1), 1);
	}
}

void Menu_StmSetPar35(void) // ���� �趨���� 2019.8.30
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "TS��ʱ����");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "TS��ʱM-D ");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "TS״̬:         ");
		// LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(2);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x85, 0x05, 0x00, &Pw_TimePwdStopPwd, 0, F_ModPar1);
	DispMod_Par(0x95, 0x05, 0x00, &Pw_TimePwdStopMD, 0, F_ModPar2);
	// DispMod_Par(0x8C, 0x05,0x00, &w_TimePwdStopST,1, 0);
	// DispMod_Par(0x9C, 0x04,0x01, &Pw_MinRunFreqDec,0, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (w_TimePwdStopST == 0)
		LCD12864_String(0x8C, "δ������");
	else if (w_TimePwdStopST == 1)
		LCD12864_String(0x8C, "��������");
	else if (w_TimePwdStopST == 2)
		LCD12864_String(0x8C, "ͣ����  ");

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_TimePwdStopPwd, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_TimePwdStopMD, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&w_TimePwdStopST, 1), Lw_SaveSetValue);
		// else if(S_RdWrNo==4)
		// Write_Stm32Data( Address(&Pw_MinRunFreqDec,0), Lw_SaveSetValue );
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_TimePwdStopPwd, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_TimePwdStopMD, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_TimePwdStopST, 1), 1);
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_MinRunFreqDec,0), 1 );
	}
}

void Menu_StmSetPar36(void) // ���� �趨���� 2019.8.30
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�豸����");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "��ˮ��Ƶģʽ  ");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "(0��ˮ,1��Ƶ)   ");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "������ͣ��EN    ");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(3);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x04, 0x00, &Pw_EquipType, 0, F_ModPar1); // ZCL 2019.8.7;
	DispMod_Par(0x96, 0x03, 0x00, &Pw_ISwitch1, 0, F_ModPar2);
	// DispMod_Par(0x8C, 0x05,0x00, &w_TimePwdStopST,1, 0);
	DispMod_Par(0x9E, 0x03, 0x00, &Pw_BCQErrStopEN, 0, F_ModPar3); // ZCL 2019.9.7
	S_DisplayPar = 0;											   // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_EquipType, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_ISwitch1, 0), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_BCQErrStopEN, 0), Lw_SaveSetValue); // ZCL 2019.9.7
																			// else if(S_RdWrNo==4)
																			// Write_Stm32Data( Address(&Pw_MinRunFreqDec,0), Lw_SaveSetValue );
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_EquipType, 0), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_ISwitch1, 0), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_BCQErrStopEN, 0), 1); // ZCL 2019.9.7
															 // else if(S_RdWrNo==4)
															 // Read_Stm32Data(Address(&Pw_MinRunFreqDec,0), 1 );
	}
}

void Menu_DspReadPar0(void) // ���� DSP������1
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "                ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		// ������
		LCD12864_String(0x88, "2.��Ƶ�ۿ�����  ");
		// ������
		LCD12864_String(0x98, "                ");
	}

	// ZCL 2017.3.21 �����������һ�Σ�������û�в����Ļ��水OK��������ֽ�������޸�״̬����������
	ModParNum(0);
	// ��ʾ���޸Ĳ���
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_DspReadPar1(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��ǰƵ��");
		LCD12864_String(0x87, "Hz");

		// �ڶ���
		LCD12864_String(0x90, "�����ѹ");
		LCD12864_String(0x97, " V");

		// ������
		LCD12864_String(0x88, "�������");
		LCD12864_String(0x8F, " A");

		// ������
		LCD12864_String(0x98, "ģ���¶�");
		LCD12864_String(0x9F, "��");
	}

	ModParNum(0);
	// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

	DispMod_Par(0x84, 0x04, 0x01, &w_dspNowHz, 3, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_dspOutVoltage, 3, 0);

	if (Pw_ScrCurrentBits == 1) // ZCL 2018.8.29  ����1λС��
	{
		DispMod_Par(0x8C, 0x04, 0x01, &w_dspAPhaseCurrent, 3, 0); // ZCL 2018.5.19
	}
	else if (Pw_ScrCurrentBits == 2) // ZCL 2018.8.29  ����2λС��
	{
		DispMod_Par(0x8C, 0x05, 0x02, &w_dspAPhaseCurrent, 3, 0); // ZCL 2018.5.19  2019.6.7  0x04
	}

	/* #ifdef CURRENT_OneXiaoShu		//ZCL 2018.1.26  ����1λС��
	#else
	#endif	 */
	// ZCL 2018.8.29 ��Pw_ScrCurrentBits ����

	DispMod_Par(0x9C, 0x04, 0x01, &w_dspNowTemperature, 3, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// ZCL 2015.9.1 ������ͨѶ����
	if (B_ModPar == 0) // ZCL 2016.12.6  ���if(B_ModPar==0)�� ����ͬ���ĵط�����ӡ�
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspNowHz, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspOutVoltage, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspAPhaseCurrent, 3), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_dspNowTemperature, 3), 1);

		// w_dspAPhaseCurrent =19856;	//ZCL123 2017.7.24 ����
	}
}

void Menu_DspReadPar2(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�����ѹ");
		LCD12864_String(0x87, " V");
		// �ڶ���
		LCD12864_String(0x90, "��ǰת��");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "Ŀ��Ƶ��"); // w_dspTargetHz
		LCD12864_String(0x8F, "Hz");

		// ������
		LCD12864_String(0x98, "��ʱƵ��");
		LCD12864_String(0x9F, "Hz");
		// LCD12864_String(0x98, "                ");
	}

	ModParNum(0);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &w_dspBusBarVoltage, 3, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_dspNowRotateSpeed, 3, 0);
	DispMod_Par(0x8C, 0x04, 0x01, &w_dspTargetHz, 3, 0);
	DispMod_Par(0x9C, 0x04, 0x01, &w_dspTmpTargetHz, 3, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// ������
	if (B_ModPar == 0) // ZCL 2016.12.6  ���if(B_ModPar==0)�� ����ͬ���ĵط�����ӡ�
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspBusBarVoltage, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspNowRotateSpeed, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspTargetHz, 3), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_dspTmpTargetHz, 3), 1);
	}
}

void Menu_DspReadPar3(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "A~����ֵ");
		LCD12864_String(0x87, " A");
		// �ڶ���
		LCD12864_String(0x90, "B~����ֵ");
		LCD12864_String(0x97, " A");
		// ������
		LCD12864_String(0x88, "C~����ֵ");
		LCD12864_String(0x8F, " A");
		// ������
		LCD12864_String(0x98, "                ");
	}

	ModParNum(0);
	/* 			w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ
		w_dspBPhaseCurrent = w_dspBPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ
		w_dspCPhaseCurrent = w_dspCPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ	 */
	// ��ʾ���޸Ĳ���

	// DispMod_Par(0x84, 0x04,0x02, &w_dspAPhaseCurrent,3, 0);		//ZCL 2018.1.26	//ZCL 2016.10.6 �����ĳ�1λС�����Ļ�2λС��

	/* #ifdef CURRENT_OneXiaoShu
	#else
	#endif */
	// ZCL 2018.8.29 ��Pw_ScrCurrentBits ����

	if (Pw_ScrCurrentBits == 1) // ZCL 2018.8.29  ����1λС��
	{
		DispMod_Par(0x84, 0x04, 0x01, &w_dspAPhaseCurrent, 3, 0); // ZCL 2018.1.26
		DispMod_Par(0x94, 0x04, 0x01, &w_dspBPhaseCurrent, 3, 0);
		DispMod_Par(0x8C, 0x04, 0x01, &w_dspCPhaseCurrent, 3, 0);
	}
	else if (Pw_ScrCurrentBits == 2) // ZCL 2018.8.29  ����2λС��
	{
		DispMod_Par(0x84, 0x05, 0x02, &w_dspAPhaseCurrent, 3, 0); // ZCL 2018.5.19  2019.6.7  0x04
		DispMod_Par(0x94, 0x05, 0x02, &w_dspBPhaseCurrent, 3, 0); // ZCL 2018.5.19  2019.6.7  0x04
		DispMod_Par(0x8C, 0x05, 0x02, &w_dspCPhaseCurrent, 3, 0); // ZCL 2018.5.19  2019.6.7  0x04
	}

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	if (B_ModPar == 0) // ZCL 2016.12.6  ���if(B_ModPar==0)�� ����ͬ���ĵط�����ӡ�
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspAPhaseCurrent, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspBPhaseCurrent, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspCPhaseCurrent, 3), 1);
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_dspTorquePromote,3), 1 );
	}
}

void Menu_DspReadPar4(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "A~������");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "B~������");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "C~������");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
	}

	ModParNum(0);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &w_dspAPhaseCurrentADCValue, 3, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_dspBPhaseCurrentADCValue, 3, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_dspCPhaseCurrentADCValue, 3, 0);
	// DispMod_Par(0x9C, 0x04,0x00, &Pw_dspTorquePromote,3, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	if (B_ModPar == 0) // ZCL 2016.12.6  ���if(B_ModPar==0)�� ����ͬ���ĵط�����ӡ�
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspAPhaseCurrentADCValue, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspBPhaseCurrentADCValue, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspCPhaseCurrentADCValue, 3), 1);
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_dspTorquePromote,3), 1 );
	}
}

void Menu_DspReadPar5(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "A~ DLH��");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "B~ DLH��");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "C~ DLH��");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
	}

	ModParNum(0);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &w_dspAPhaseDLHZero, 3, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_dspBPhaseDLHZero, 3, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_dspCPhaseDLHZero, 3, 0);
	// DispMod_Par(0x9C, 0x04,0x00, &Pw_dspTorquePromote,3, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	if (B_ModPar == 0) // ZCL 2016.12.6  ���if(B_ModPar==0)�� ����ͬ���ĵط�����ӡ�
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspAPhaseDLHZero, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspBPhaseDLHZero, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspCPhaseDLHZero, 3), 1);
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_dspTorquePromote,3), 1 );
	}
}

// ZCL 2019.9.7 Menu_DspReadPar6()��ֲ�� V139-3
void Menu_DspReadPar6(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��Ƶͣԭ��");
		LCD12864_String(0x87, "��");
		// �ڶ���
		LCD12864_String(0x90, "--");
		// ������
		LCD12864_String(0x88, "������1 ");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "--");

		Lw_SaveDspStopStatus = 1000;
		Lw_SaveDspFaultStatus = 1000;
	}

	ModParNum(0);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x85, 0x03, 0x00, &w_DspStopNum, 1, 0); // ͣ������ w_dspStopNum	ZCL 2018.8.25
	// DispMod_Par(0x94, 0x05,0x00, &w_dspCounter3,3, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_dspSysErrorCode1, 3, 0); // ������
	// DispMod_Par(0x9C, 0x05,0x00, &w_dspSysErrorCode1,3, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// 1. ��Ƶͣԭ��
	if (Lw_SaveDspStopStatus != w_DspStopStatus)
	{
		Lw_SaveDspStopStatus = w_DspStopStatus;
		// ͣ��ԭ���״̬����ʾ  ZCL 2015.9.7
		switch (w_DspStopStatus)
		{
		case 0:
			LCD12864_String(0x91, "��ͣ��- ����  ");
			break; //
		case 1:
			LCD12864_String(0x91, "����ͣ��      ");
			break; //
		case 2:
			if (Pw_ScrMenuMode == 0)
			{
				LCD12864_String(0x91, "ARM ����ͣ��    ");
				break;
			} // ZCL 2019.9.7 ��ʾ��ARM ����ͣ��������ԭ���ARMȥ��
			else if (Pw_ScrMenuMode == 1 || Pw_ScrMenuMode == 2) // ZCL 2019.9.10 =1Ҳ��ʾ��DI��ͣ��
			{
				LCD12864_String(0x91, "DI��ͣ��      ");
				break;
			} // ZCL 2019.7.20 ��ע����Ƶͣԭ�����ͣ����Դ��ARM�����͸�DSP

		case 3:
			LCD12864_String(0x91, "��Ƶ����ͣ��  ");
			break; //

		default:
			return;
		}
	}

	// 2. ������ ��������
	if (Lw_SaveDspFaultStatus != w_DspAlarmStatus) // w_dspFaultStatus ZCL 2018.8.25
	{
		Lw_SaveDspFaultStatus = w_DspAlarmStatus; // w_dspFaultStatus ZCL 2018.8.25
		// ͣ��ԭ���״̬����ʾ  ZCL 2015.9.7
		switch (w_DspAlarmStatus) // w_dspFaultStatus ZCL 2018.8.25
		{
		case 0:
			LCD12864_String(0x99, "�޹���- ����  ");
			break; //
		case 1:
			LCD12864_String(0x99, "��������      ");
			break; //
		case 2:
			LCD12864_String(0x99, "��ѹ����      ");
			break; //
		case 3:
			LCD12864_String(0x99, "Ƿѹ����      ");
			break; //
		case 4:
			LCD12864_String(0x99, "ȱ�ౣ��      ");
			break; //
		case 5:
			LCD12864_String(0x99, "IPM ����      ");
			break; //
		case 6:
			LCD12864_String(0x99, "��������������");
			break; //
		case 7:
			LCD12864_String(0x99, "IPM ����      ");
			break; //
		case 8:
			LCD12864_String(0x99, "�������      ");
			break; //
		case 9:
			LCD12864_String(0x99, "���ȹ���      ");
			break; //
		case 10:
			LCD12864_String(0x99, "EEPROM����    ");
			break; //
		case 11:
			LCD12864_String(0x99, "DSP ����������");
			break; //
		case 12:
			LCD12864_String(0x99, "STM ����������");
			break; //
		case 13:
			LCD12864_String(0x99, "�ӵر���      ");
			break; //
		case 14:
			LCD12864_String(0x99, "������Ƿ��ѹ  ");
			break; //
		case 15:
			LCD12864_String(0x99, "�����й���ѹ");
			break; //

		default:
			return;
		}
	}

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&w_DspStopNum, 1), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&w_DspStopStatus, 1), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&w_dspSysErrorCode1, 3), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&w_DspAlarmStatus, 1), Lw_SaveSetValue); // w_dspFaultStatus ZCL 2018.8.25
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2016.12.6  ���if(B_ModPar==0)�� ����ͬ���ĵط�����ӡ�
	{
		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_DspStopNum, 1), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_DspStopStatus, 1), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspSysErrorCode1, 3), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_DspAlarmStatus, 1), 1); // w_dspFaultStatus ZCL 2018.8.25
	}
}

void Menu_DspReadPar7(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "ͨѶER_C");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "�ϵ��_C");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "ͨѶOK_C");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "ͨѶ����");
		LCD12864_String(0x9F, "  ");
	}

	// ZCL 2018.5.17
	ModParNum(0); // ָ��:�޸Ĳ���������
	// ZCL 2018.5.16 �޸Ĳ�������ʱ���ӿ��ٶȱ�֤�л�����ʾ������ʱ������Һ����ʾ�����ϣ���ʾ����
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &w_dspCommErrCounter, 3, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_dspCounter2, 3, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_dspCounter4, 3, 0);
	DispMod_Par(0x9C, 0x05, 0x00, &w_dspCounter, 3, 0);
	S_DisplayPar = 0;

	// д���� 2015.9.1

	// ������
	if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspCommErrCounter, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspCounter2, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspCounter4, 3), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_dspCounter, 3), 1);
	}
}

void Menu_DspReadPar8(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "PDPINT_C        ");
		// �ڶ���
		LCD12864_String(0x90, "�������");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "                ");
		// ������
		LCD12864_String(0x98, "                ");
	}

	ModParNum(0);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &w_dspCounter5, 3, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_dspCounter3, 3, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	if (B_ModPar == 0) // ZCL 2016.12.6  ���if(B_ModPar==0)�� ����ͬ���ĵط�����ӡ�
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 2)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspCounter5, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspCounter3, 3), 1);
	}
}

void Menu_DspReadPar29(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "AI1 ����");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "AI2 ����");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "PDPINT_C        ");
		// ������
		LCD12864_String(0x98, "�������");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(0);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &w_dspAI1ADCValue, 3, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_dspAI2ADCValue, 3, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_dspCounter5, 3, 0);
	DispMod_Par(0x9C, 0x05, 0x00, &w_dspCounter3, 3, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	if (B_ModPar == 0) // ZCL 2016.12.6  ���if(B_ModPar==0)�� ����ͬ���ĵط�����ӡ�
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspAI1ADCValue, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspAI2ADCValue, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspCounter5, 3), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_dspCounter3, 3), 1);
	}
}

// ZCL 2019.3.15 ����DspReadPar���棬����DCM220
void Menu_DspReadPar30(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�۲�ֵ 1");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "�۲�ֵ 2");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "�۲�ֵ 3");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
	}

	ModParNum(0);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &w_dspTemp1, 3, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_dspTemp2, 3, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_dspTemp3, 3, 0);
	// DispMod_Par(0x9C, 0x04,0x00, &Pw_dspTorquePromote,3, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	if (B_ModPar == 0) // ZCL 2016.12.6  ���if(B_ModPar==0)�� ����ͬ���ĵط�����ӡ�
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspTemp1, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspTemp2, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspTemp3, 3), 1);
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_dspTorquePromote,3), 1 );
	}
}

void Menu_DspReadPar31(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�۲�ֵ 4");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "�۲�ֵ 5");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "�۲�ֵ 6");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
	}

	ModParNum(0);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &w_dspTemp4, 3, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_dspTemp5, 3, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_dspTemp6, 3, 0);
	// DispMod_Par(0x9C, 0x04,0x00, &Pw_dspTorquePromote,3, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	if (B_ModPar == 0) // ZCL 2016.12.6  ���if(B_ModPar==0)�� ����ͬ���ĵط�����ӡ�
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspTemp4, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspTemp5, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspTemp6, 3), 1);
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_dspTorquePromote,3), 1 );
	}
}

void Menu_DspReadPar32(void) // ���� ������  			//ZCL 2016.10.6 ���Ӵ˻���
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��Ƶ�汾");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "Dsp-Year");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "Dsp-Date");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "�ı��汾");
		LCD12864_String(0x9F, "  "); // ZCL 2018.9.21
	}

	ModParNum(0);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x04, 0x02, &w_dspSoftVersion, 3, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_dspWriteYear, 3, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_dspWriteDate, 3, 0);
	DispMod_Par(0x9C, 0x04, 0x02, &w_ScrVERSION, 5, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// ZCL 2015.9.1 ������ͨѶ����
	if (B_ModPar == 0) // ZCL 2016.12.6  ���if(B_ModPar==0)�� ����ͬ���ĵط�����ӡ�
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		/* 	�ı��İ汾����ͨѶ������DCM200_SCR�Լ��Ĳ��� */

		if (S_RdWrNo == 1) // ZCL 2018.9.13 ����ifע���ˣ���������else if ����
			Read_Stm32Data(Address(&w_dspSoftVersion, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspWriteYear, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspWriteDate, 3), 1);
		// else if(S_RdWrNo==4)
		// Read_Stm32Data( Address(&w_ScrVERSION,4), 1 );
	}
}

// ZCL 2017.3.9
void Menu_DspReadPar33(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "Comp1Ds ");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "Comp1Qs ");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "FrqComp ");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "FrqLimit");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(0);
	DispMod_Par(0x84, 0x05, 0x00, &w_dspdeadComp1Ds, 3, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_dspdeadComp1Qs, 3, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_dspFrqComp, 3, 0);
	DispMod_Par(0x9C, 0x05, 0x00, &w_dspFrqCompAfterLimt, 3, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspdeadComp1Ds, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspdeadComp1Qs, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspFrqComp, 3), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_dspFrqCompAfterLimt, 3), 1);
	}
}

void Menu_DspReadPar34(void) // ���� ������
{
	// u8 i;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "������ι��ϼ�¼");
		// �ڶ���
		LCD12864_String(0x90, "1:");
		// LCD12864_String(0x97, " L");
		// ������
		// ZCL 2017.6.17 ����д���¶�ʱˢ��ҳ��ʱ��˸
		// LCD12864_String(0x88, "    --    --    ");		//ZCL 2015.9.10
		// ZCL 2017.6.17 �ĳ����£�
		LCD12864_String(0x8A, "--"); // ZCL 2015.9.10
		LCD12864_String(0x8D, "--"); // ZCL 2015.9.10

		// LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "���� 1�Σ�      ");
		// LCD12864_String(0x9F, "Hz");
	}

	/*  DispMod_Par(u8 x_pos,u8 length, u8 XiaoShuBits,  u16 *Address, u8 ParType,
	u8 b_modpar)	 */
	ModParNum(0);
	// DispMod_Par(0x84, 0x05,0x00, &w_dspEquipAlarmLast1,3, 0);
	DispMod_Par(0x88, 0x04, 0x00, &w_dspEquipAlarm1YM, 3, 0);
	DispMod_Par(0x8B, 0x04, 0x00, &w_dspEquipAlarm1DH, 3, 0);
	DispMod_Par(0x8E, 0x04, 0x00, &w_dspEquipAlarm1MS, 3, 0);
	//
	DspFaultDisplay(w_dspEquipAlarmLast1, 0x91); // ZCL 2018.5.19 ����S_DisplayPar����ʾ���ƣ���ֹƵ��ˢ��
	S_DisplayPar = 0;							 // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspEquipAlarmLast1, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspEquipAlarm1YM, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspEquipAlarm1DH, 3), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_dspEquipAlarm1MS, 3), 1);
	}
}

void Menu_DspReadPar35(void) // ���� ������
{
	// u8 i;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "������ι��ϼ�¼");
		// �ڶ���
		LCD12864_String(0x90, "2:");
		// LCD12864_String(0x97, " L");
		// ������
		// ZCL 2017.6.17 ����д���¶�ʱˢ��ҳ��ʱ��˸
		// LCD12864_String(0x88, "    --    --    ");		//ZCL 2015.9.10
		// ZCL 2017.6.17 �ĳ����£�
		LCD12864_String(0x8A, "--"); // ZCL 2015.9.10
		LCD12864_String(0x8D, "--"); // ZCL 2015.9.10

		// LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "���� 2�Σ�      ");
		// LCD12864_String(0x9F, "Hz");
	}

	/*  DispMod_Par(u8 x_pos,u8 length, u8 XiaoShuBits,  u16 *Address, u8 ParType,
	u8 b_modpar)	 */
	ModParNum(0);
	// DispMod_Par(0x84, 0x05,0x00, &w_dspEquipAlarmLast1,3, 0);
	DispMod_Par(0x88, 0x04, 0x00, &w_dspEquipAlarm2YM, 3, 0);
	DispMod_Par(0x8B, 0x04, 0x00, &w_dspEquipAlarm2DH, 3, 0);
	DispMod_Par(0x8E, 0x04, 0x00, &w_dspEquipAlarm2MS, 3, 0);
	//
	DspFaultDisplay(w_dspEquipAlarmLast2, 0x91); // ZCL 2018.5.19 ����S_DisplayPar����ʾ���ƣ���ֹƵ��ˢ��
	S_DisplayPar = 0;							 // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspEquipAlarmLast2, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspEquipAlarm2YM, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspEquipAlarm2DH, 3), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_dspEquipAlarm2MS, 3), 1);
	}
}

void Menu_DspReadPar36(void) // ���� ������
{
	// u8 i;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "������ι��ϼ�¼");
		// �ڶ���
		LCD12864_String(0x90, "3:");
		// LCD12864_String(0x97, " L");
		// ������
		// ZCL 2017.6.17 ����д���¶�ʱˢ��ҳ��ʱ��˸
		// LCD12864_String(0x88, "    --    --    ");		//ZCL 2015.9.10
		// ZCL 2017.6.17 �ĳ����£�
		LCD12864_String(0x8A, "--"); // ZCL 2015.9.10
		LCD12864_String(0x8D, "--"); // ZCL 2015.9.10

		// LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "���� 3�Σ�      ");
		// LCD12864_String(0x9F, "Hz");
	}

	/*  DispMod_Par(u8 x_pos,u8 length, u8 XiaoShuBits,  u16 *Address, u8 ParType,
	u8 b_modpar)	 */
	ModParNum(0);
	// DispMod_Par(0x84, 0x05,0x00, &w_dspEquipAlarmLast3,3, 0);
	DispMod_Par(0x88, 0x04, 0x00, &w_dspEquipAlarm3YM, 3, 0);
	DispMod_Par(0x8B, 0x04, 0x00, &w_dspEquipAlarm3DH, 3, 0);
	DispMod_Par(0x8E, 0x04, 0x00, &w_dspEquipAlarm3MS, 3, 0);
	//
	DspFaultDisplay(w_dspEquipAlarmLast3, 0x91); // ZCL 2018.5.19 ����S_DisplayPar����ʾ���ƣ���ֹƵ��ˢ��
	S_DisplayPar = 0;							 // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspEquipAlarmLast3, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspEquipAlarm3YM, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspEquipAlarm3DH, 3), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_dspEquipAlarm3MS, 3), 1);
	}
}

void Menu_DspReadPar37(void) // ���� ������
{
	// u8 i;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "������ι��ϼ�¼");
		// �ڶ���
		LCD12864_String(0x90, "4:");
		// LCD12864_String(0x97, " L");
		// ������
		// ZCL 2017.6.17 ����д���¶�ʱˢ��ҳ��ʱ��˸
		// LCD12864_String(0x88, "    --    --    ");		//ZCL 2015.9.10
		// ZCL 2017.6.17 �ĳ����£�
		LCD12864_String(0x8A, "--"); // ZCL 2015.9.10
		LCD12864_String(0x8D, "--"); // ZCL 2015.9.10

		// LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "���� 4�Σ�      ");
		// LCD12864_String(0x9F, "Hz");
	}

	/*  DispMod_Par(u8 x_pos,u8 length, u8 XiaoShuBits,  u16 *Address, u8 ParType,
	u8 b_modpar)	 */
	ModParNum(0);
	// DispMod_Par(0x84, 0x05,0x00, &w_dspEquipAlarmLast4,3, 0);
	DispMod_Par(0x88, 0x04, 0x00, &w_dspEquipAlarm4YM, 3, 0);
	DispMod_Par(0x8B, 0x04, 0x00, &w_dspEquipAlarm4DH, 3, 0);
	DispMod_Par(0x8E, 0x04, 0x00, &w_dspEquipAlarm4MS, 3, 0);
	//
	DspFaultDisplay(w_dspEquipAlarmLast4, 0x91); // ZCL 2018.5.19 ����S_DisplayPar����ʾ���ƣ���ֹƵ��ˢ��
	S_DisplayPar = 0;							 // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspEquipAlarmLast4, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspEquipAlarm4YM, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspEquipAlarm4DH, 3), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_dspEquipAlarm4MS, 3), 1);
	}
}

void Menu_DspReadPar38(void) // ���� ������
{
	// u8 i;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "������ι��ϼ�¼");
		// �ڶ���
		LCD12864_String(0x90, "5:");
		// LCD12864_String(0x97, " L");
		// ������
		// ZCL 2017.6.17 ����д���¶�ʱˢ��ҳ��ʱ��˸
		// LCD12864_String(0x88, "    --    --    ");		//ZCL 2015.9.10
		// ZCL 2017.6.17 �ĳ����£�
		LCD12864_String(0x8A, "--"); // ZCL 2015.9.10
		LCD12864_String(0x8D, "--"); // ZCL 2015.9.10

		// LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "���� 5�Σ�      ");
		// LCD12864_String(0x9F, "Hz");
	}

	/*  DispMod_Par(u8 x_pos,u8 length, u8 XiaoShuBits,  u16 *Address, u8 ParType,
	u8 b_modpar)	 */
	ModParNum(0);
	// DispMod_Par(0x84, 0x05,0x00, &w_dspEquipAlarmLast5,3, 0);
	DispMod_Par(0x88, 0x04, 0x00, &w_dspEquipAlarm5YM, 3, 0);
	DispMod_Par(0x8B, 0x04, 0x00, &w_dspEquipAlarm5DH, 3, 0);
	DispMod_Par(0x8E, 0x04, 0x00, &w_dspEquipAlarm5MS, 3, 0);
	//
	DspFaultDisplay(w_dspEquipAlarmLast5, 0x91); // ZCL 2018.5.19 ����S_DisplayPar����ʾ���ƣ���ֹƵ��ˢ��
	S_DisplayPar = 0;							 // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspEquipAlarmLast5, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspEquipAlarm5YM, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspEquipAlarm5DH, 3), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_dspEquipAlarm5MS, 3), 1);
	}
}

void Menu_DspReadPar39(void) // ���� ������
{
	// u8 i;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "������ι��ϼ�¼");
		// �ڶ���
		LCD12864_String(0x90, "6:");
		// LCD12864_String(0x97, " L");
		// ������
		// ZCL 2017.6.17 ����д���¶�ʱˢ��ҳ��ʱ��˸
		// LCD12864_String(0x88, "    --    --    ");		//ZCL 2015.9.10
		// ZCL 2017.6.17 �ĳ����£�
		LCD12864_String(0x8A, "--"); // ZCL 2015.9.10
		LCD12864_String(0x8D, "--"); // ZCL 2015.9.10

		// LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "���� 6�Σ�      ");
		// LCD12864_String(0x9F, "Hz");
	}

	/*  DispMod_Par(u8 x_pos,u8 length, u8 XiaoShuBits,  u16 *Address, u8 ParType,
	u8 b_modpar)	 */
	ModParNum(0);

	// DispMod_Par(0x84, 0x05,0x00, &w_dspEquipAlarmLast6,3, 0);
	DispMod_Par(0x88, 0x04, 0x00, &w_dspEquipAlarm6YM, 3, 0);
	DispMod_Par(0x8B, 0x04, 0x00, &w_dspEquipAlarm6DH, 3, 0);
	DispMod_Par(0x8E, 0x04, 0x00, &w_dspEquipAlarm6MS, 3, 0);

	//
	DspFaultDisplay(w_dspEquipAlarmLast6, 0x91); // ZCL 2018.5.19 ����S_DisplayPar����ʾ���ƣ���ֹƵ��ˢ��
	S_DisplayPar = 0;							 // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	if (B_ModPar == 0) // ������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspEquipAlarmLast6, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspEquipAlarm6YM, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspEquipAlarm6DH, 3), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_dspEquipAlarm6MS, 3), 1);
	}
}

void Menu_DspReadPar40(void) // ���� ������
{
	// u8 i;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "ѡ����ϼ�¼:   ");
		// �ڶ���
		LCD12864_String(0x90, "n:");
		// LCD12864_String(0x97, " L");
		// ������
		// ZCL 2017.6.17 ����д���¶�ʱˢ��ҳ��ʱ��˸
		// LCD12864_String(0x88, "    --    --    ");		//ZCL 2015.9.10
		// ZCL 2017.6.17 �ĳ����£�
		LCD12864_String(0x8A, "--"); // ZCL 2015.9.10
		LCD12864_String(0x8D, "--"); // ZCL 2015.9.10

		// LCD12864_String(0x8F, "Mp");
		// ������
		// ZCL 2017.6.17 ����д���¶�ʱˢ��ҳ��ʱ��˸
		// LCD12864_String(0x98, "����  ��    �Σ�");
		// ZCL 2017.6.17 �ĳ����£�
		LCD12864_String(0x98, "����");
		LCD12864_String(0x9B, "��");
		LCD12864_String(0x9E, "�Σ�");

		// LCD12864_String(0x9F, "Hz");
	}

	/*  DispMod_Par(u8 x_pos,u8 length, u8 XiaoShuBits,  u16 *Address, u8 ParType,
	u8 b_modpar)	 */
	ModParNum(1);
	F_ModPar1 = 0;
	// DispMod_Par(0x84, 0x05,0x00, &w_dspSelEquipAlarm,3, 0);
	DispMod_Par(0x88, 0x04, 0x00, &w_dspSelEquipAlarm1YM, 3, 0);
	DispMod_Par(0x8B, 0x04, 0x00, &w_dspSelEquipAlarm1DH, 3, 0);
	DispMod_Par(0x8E, 0x04, 0x00, &w_dspSelEquipAlarm1MS, 3, 0);
	//
	DispMod_Par(0x9A, 0x02, 0x00, &w_dspFaultRecNum, 3, 0);
	F_ModPar1 = 1;
	DispMod_Par(0x9D, 0x02, 0x00, &w_dspSelFaultNo, 3, F_ModPar1);
	//
	DspFaultDisplay(w_dspSelEquipAlarm, 0x91); // ZCL 2018.5.19 ����S_DisplayPar����ʾ���ƣ���ֹƵ��ˢ��
	S_DisplayPar = 0;						   // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&w_dspSelFaultNo, 3), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2016.12.6  ���if(B_ModPar==0)�� ����ͬ���ĵط�����ӡ�
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 6)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspSelEquipAlarm, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspSelEquipAlarm1YM, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_dspSelEquipAlarm1DH, 3), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_dspSelEquipAlarm1MS, 3), 1);
		else if (S_RdWrNo == 5)
			Read_Stm32Data(Address(&w_dspSelFaultNo, 3), 1);
		else if (S_RdWrNo == 6)
			Read_Stm32Data(Address(&w_dspFaultRecNum, 3), 1);
	}
}

/* void Menu_DspReadPar52(void)				//���� DSP������
{
	if(Lw_SavePageNo!=Lw_PageNo)
	{
		Lw_SavePageNo=Lw_PageNo;		C_TimeUpdatePage=0; 	S_Com1Send=0;
		//LCD12864_ClrText();          //����
		//��һ��
		LCD12864_String(0x80, "A~У��ֵ");
		LCD12864_String(0x87, "  ");
		//�ڶ���
		LCD12864_String(0x90, "B~У��ֵ");
		LCD12864_String(0x97, "  ");
		//������
		LCD12864_String(0x88, "C~У��ֵ");
		LCD12864_String(0x8F, "  ");
		//������
		LCD12864_String(0x98, "                ");
		S_RdWrNo=0;
	}

	if(T_LcdDisplay!=SClk10Ms)
	{
		if(T_LcdDisplay!=SClk10Ms)
		C_LcdDisplay++;
		if(C_LcdDisplay>5)
		{
			C_LcdDisplay=0;
			ModParNum(0);
			//��ʾ���޸Ĳ���
			DispMod_Par(0x84, 0x05,0x00, &w_dspAPhaseCalibrationZero,3, 0);
			DispMod_Par(0x94, 0x05,0x00, &w_dspBPhaseCalibrationZero,3, 0);
			DispMod_Par(0x8C, 0x05,0x00, &w_dspCPhaseCalibrationZero,3, 0);
			// DispMod_Par(0x9C, 0x04,0x00, &Pw_dspTorquePromote,2, F_ModPar4);
		}
	}

	//д���� 2015.9.1
	if(B_ModPar==1)
	{
		B_ModPar=2;
		S_Com1Send=0;
		C_Com1Send=0;
		S_Com1SendNoNum=0;
	}
	if(B_ModPar==2)
	{
		if(S_RdWrNo==1)
			Write_Stm32Data( Address(&w_dspAPhaseCalibrationZero,3), Lw_SaveSetValue );
		else if(S_RdWrNo==2)
			Write_Stm32Data( Address(&w_dspBPhaseCalibrationZero,3), Lw_SaveSetValue );
		else if(S_RdWrNo==3)
			Write_Stm32Data( Address(&w_dspCPhaseCalibrationZero,3), Lw_SaveSetValue );
		// else if(S_RdWrNo==4)
			// Write_Stm32Data( Address(&Pw_dspTorquePromote,3), Lw_SaveSetValue );
	}

	//������
	else if(B_ModPar==0)		//ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		//ZCL 2015.9.1 ������ͨѶ����
		if(S_RdWrNo>3)
			S_RdWrNo=0;
		if(S_RdWrNo==0)
		{
			S_RdWrNo=1;
			S_Com1Send=0;
			C_Com1Send=0;
			S_Com1SendNoNum=0;
		}
		//��˳���ȡ����
		if(S_RdWrNo==1)
			Read_Stm32Data( Address(&w_dspAPhaseCalibrationZero,3), 1 );
		else if(S_RdWrNo==2)
			Read_Stm32Data( Address(&w_dspBPhaseCalibrationZero,3), 1 );
		else if(S_RdWrNo==3)
			Read_Stm32Data( Address(&w_dspCPhaseCalibrationZero,3), 1 );
		// else if(S_RdWrNo==4)
			// Read_Stm32Data(Address(&Pw_dspTorquePromote,3), 1 );

	}
}

void Menu_DspReadPar53(void)				//���� DSP������
{
	if(Lw_SavePageNo!=Lw_PageNo)
	{
		Lw_SavePageNo=Lw_PageNo;		C_TimeUpdatePage=0; 	S_Com1Send=0;
		//LCD12864_ClrText();          //����
		//��һ��
		LCD12864_String(0x80, "A~У����");
		LCD12864_String(0x87, "  ");
		//�ڶ���
		LCD12864_String(0x90, "B~У����");
		LCD12864_String(0x97, "  ");
		//������
		LCD12864_String(0x88, "C~У����");
		LCD12864_String(0x8F, "  ");
		//������
		LCD12864_String(0x98, "                ");
		S_RdWrNo=0;
	}

	if(T_LcdDisplay!=SClk10Ms)
	{
		if(T_LcdDisplay!=SClk10Ms)
		C_LcdDisplay++;
		if(C_LcdDisplay>5)
		{
			C_LcdDisplay=0;
			ModParNum(0);
			//��ʾ���޸Ĳ���
			DispMod_Par(0x84, 0x05,0x00, &w_dspAPhaseCalibrationGain,3, 0);
			DispMod_Par(0x94, 0x05,0x00, &w_dspBPhaseCalibrationGain,3, 0);
			DispMod_Par(0x8C, 0x05,0x00, &w_dspCPhaseCalibrationGain,3, 0);
			// DispMod_Par(0x9C, 0x04,0x00, &Pw_dspTorquePromote,2, F_ModPar4);
		}
	}

	//д���� 2015.9.1
	if(B_ModPar==1)
	{
		B_ModPar=2;
		S_Com1Send=0;
		C_Com1Send=0;
		S_Com1SendNoNum=0;
	}
	if(B_ModPar==2)
	{
		if(S_RdWrNo==1)
			Write_Stm32Data( Address(&w_dspAPhaseCalibrationGain,3), Lw_SaveSetValue );
		else if(S_RdWrNo==2)
			Write_Stm32Data( Address(&w_dspBPhaseCalibrationGain,3), Lw_SaveSetValue );
		else if(S_RdWrNo==3)
			Write_Stm32Data( Address(&w_dspCPhaseCalibrationGain,3), Lw_SaveSetValue );
		// else if(S_RdWrNo==4)
			// Write_Stm32Data( Address(&Pw_dspTorquePromote,3), Lw_SaveSetValue );
	}

	//������
	else if(B_ModPar==0)		//ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		//ZCL 2015.9.1 ������ͨѶ����
		if(S_RdWrNo>3)
			S_RdWrNo=0;
		if(S_RdWrNo==0)
		{
			S_RdWrNo=1;
			S_Com1Send=0;
			C_Com1Send=0;
			S_Com1SendNoNum=0;
		}
		//��˳���ȡ����
		if(S_RdWrNo==1)
			Read_Stm32Data( Address(&w_dspAPhaseCalibrationGain,3), 1 );
		else if(S_RdWrNo==2)
			Read_Stm32Data( Address(&w_dspBPhaseCalibrationGain,3), 1 );
		else if(S_RdWrNo==3)
			Read_Stm32Data( Address(&w_dspCPhaseCalibrationGain,3), 1 );
		// else if(S_RdWrNo==4)
			// Read_Stm32Data(Address(&Pw_dspTorquePromote,3), 1 );

	}
}
 */

void Menu_DspSetPar1(void) // ���� DSP�趨����1
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "                ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		// ������
		if (Pw_ScrMenuMode == 0) // ZCL 2018.8.28 =0��������ģʽ
			LCD12864_String(0x88, "3.��Ƶ�趨����  ");
		else if (Pw_ScrMenuMode == 1 || Pw_ScrMenuMode == 2) // ZCL 2018.8.28 =1����Ƶ��ģʽ
			LCD12864_String(0x88, "  ��Ƶ�趨����  ");
		// ������
		LCD12864_String(0x98, "                ");
	}

	// ZCL 2017.3.21 �����������һ�Σ�������û�в����Ļ��水OK��������ֽ�������޸�״̬����������
	ModParNum(0);
	// ��ʾ���޸Ĳ���
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_DspSetPar2(void) // ���� DSP�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "����ʱ��");
		LCD12864_String(0x87, " S");
		// �ڶ���
		LCD12864_String(0x90, "����ʱ��");
		LCD12864_String(0x97, " S");
		// ������
		LCD12864_String(0x88, "���ת��");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "ת������");
		LCD12864_String(0x9F, "  ");
	}

	// ZCL 2018.5.17
	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_dspAddSpeedTime, 2, F_ModPar1);
	DispMod_Par(0x94, 0x05, 0x00, &Pw_dspSubSpeedTime, 2, F_ModPar2);
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_dspMotorDirection, 2, F_ModPar3);
	DispMod_Par(0x9C, 0x05, 0x00, &Pw_dspTorquePromote, 2, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_dspAddSpeedTime, 2), Lw_SaveSetValue); // ZCL 2017.6.19 Pw_dspAddSpeedTime,0������Ӧ��ΪPw_dspAddSpeedTime,2
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_dspSubSpeedTime, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_dspMotorDirection, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_dspTorquePromote, 2), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_dspAddSpeedTime, 2), 1); // ZCL 2017.6.19 Pw_dspAddSpeedTime,0������Ӧ��ΪPw_dspAddSpeedTime,2
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_dspSubSpeedTime, 2), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_dspMotorDirection, 2), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_dspTorquePromote, 2), 1);
	}
}

void Menu_DspSetPar3(void) // ���� DSP�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "VF����  ");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "����Ƶ��");
		LCD12864_String(0x97, "Hz");
		// ������
		LCD12864_String(0x88, "����Ƶ��");
		LCD12864_String(0x8F, "Hz");
		// ������
		LCD12864_String(0x98, "����Ƶ��");
		LCD12864_String(0x9F, "Hz");
	}

	// ZCL 2018.5.17
	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_dspVFCurve, 2, F_ModPar1);
	DispMod_Par(0x94, 0x04, 0x01, &Pw_dspStartHz, 2, F_ModPar2);
	DispMod_Par(0x8C, 0x04, 0x01, &Pw_dspUpperLimitHz, 2, F_ModPar3);
	DispMod_Par(0x9C, 0x04, 0x01, &Pw_dspLowerLimitHz, 2, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_dspVFCurve, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_dspStartHz, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_dspUpperLimitHz, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_dspLowerLimitHz, 2), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_dspVFCurve, 2), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_dspStartHz, 2), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_dspUpperLimitHz, 2), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_dspLowerLimitHz, 2), 1);
	}
}

void Menu_DspSetPar4(void) // ���� DSP�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�ز�Ƶ��");
		if (Pw_ScrMenuMode == 0 || Pw_ScrMenuMode == 1) // ZCL 2019.3.18
			// 2017.6.17 �ĳ�1���֣���ֹ��˸(kǰ��������˸) ZCL 2019.3.15
			LCD12864_String(0x86, " kHz"); // ZCL 2019.4.15 ע����������2����
		else if (Pw_ScrMenuMode == 2)
			LCD12864_String(0x87, "kH"); // ZCL 2019.4.15
		// �ڶ���
		LCD12864_String(0x90, "��������");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "��������");
		LCD12864_String(0x8F, " A");
		// ������
		LCD12864_String(0x98, "��ѹ��ѹ");
		LCD12864_String(0x9F, " V");
	}

	ModParNum(4);
	// ��ʾ���޸Ĳ���
	if (Pw_ScrMenuMode == 0 || Pw_ScrMenuMode == 1)					   // ZCL 2019.3.18
		DispMod_Par(0x84, 0x03, 0x00, &Pw_dspCarrierHz, 2, F_ModPar1); // ZCL 2015.9.29 ע����浥λ����2����
	else if (Pw_ScrMenuMode == 2)									   // ZCL 2019.3.18  =11 DCM220 ��Ƶ��ģʽ
		DispMod_Par(0x84, 0x04, 0x01, &Pw_dspCarrierHz, 2, F_ModPar1); // ZCL 2017.5.4 �ĳ�1λС�� DCM220

	DispMod_Par(0x94, 0x04, 0x02, &Pw_dspDeadSupplement, 2, F_ModPar2);
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_dspOCPCurrent, 2, F_ModPar3); // ZCL 2018.9.13 ע��û��С����
	DispMod_Par(0x9C, 0x05, 0x00, &Pw_dspOVPVoltage, 2, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_dspCarrierHz, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_dspDeadSupplement, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_dspOCPCurrent, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_dspOVPVoltage, 2), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_dspCarrierHz, 2), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_dspDeadSupplement, 2), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_dspOCPCurrent, 2), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_dspOVPVoltage, 2), 1);
	}
}

void Menu_DspSetPar5(void) // ���� DSP�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "Ƿѹ��ѹ");
		LCD12864_String(0x87, " V");
		// �ڶ���
		LCD12864_String(0x90, "�����¶�");
		LCD12864_String(0x97, "��");
		// ������
		LCD12864_String(0x88, "����ת��"); // ZCL 2019.3.16 ��������
		LCD12864_String(0x8F, "��");
		// ������
		LCD12864_String(0x98, "��������");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_dspUVPVoltage, 2, F_ModPar1);
	DispMod_Par(0x94, 0x04, 0x01, &Pw_dspOHPTemperature, 2, F_ModPar2);
	DispMod_Par(0x8C, 0x04, 0x01, &Pw_dspFanRunTemperature, 2, F_ModPar3);
	DispMod_Par(0x9C, 0x05, 0x00, &Pw_dspFanRunMode, 2, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_dspUVPVoltage, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_dspOHPTemperature, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_dspFanRunTemperature, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_dspFanRunMode, 2), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_dspUVPVoltage, 2), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_dspOHPTemperature, 2), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_dspFanRunTemperature, 2), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_dspFanRunMode, 2), 1);
	}
}

void Menu_DspSetPar6(void) // ���� DSP�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "����ͣģʽ");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "DIͣ��ģʽ");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "��������");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "���ϲ���  "); // ZCL 2019.3.23 ��һ���֣������95��9D��ʼд������94��9D��
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x85, 0x03, 0x00, &Pw_dspKeyStopMode, 2, F_ModPar1);
	DispMod_Par(0x95, 0x03, 0x00, &Pw_dspDIStopMode, 2, F_ModPar2);
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_dspHuoErType, 2, F_ModPar3);
	DispMod_Par(0x9D, 0x03, 0x00, &w_dspManMadeFault, 3, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_dspKeyStopMode, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_dspDIStopMode, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_dspHuoErType, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&w_dspManMadeFault, 3), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0)
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_dspKeyStopMode, 2), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_dspDIStopMode, 2), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_dspHuoErType, 2), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_dspManMadeFault, 3), 1);
	}
}

void Menu_DspSetPar7(void) // ���� DSP�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "AO1 ���Դ");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "AO2 ���Դ");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "DO1 ���Դ");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "DO2 ���Դ");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x85, 0x03, 0x00, &Pw_dspAO1OutSource, 2, F_ModPar1);
	DispMod_Par(0x95, 0x03, 0x00, &Pw_dspAO2OutSource, 2, F_ModPar2);
	DispMod_Par(0x8D, 0x03, 0x00, &Pw_dspDO1OutSource, 2, F_ModPar3);
	DispMod_Par(0x9D, 0x03, 0x00, &Pw_dspDO2OutSource, 2, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_dspAO1OutSource, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_dspAO2OutSource, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_dspDO1OutSource, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_dspDO2OutSource, 2), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_dspAO1OutSource, 2), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_dspAO2OutSource, 2), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_dspDO1OutSource, 2), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_dspDO2OutSource, 2), 1);
	}
}

void Menu_DspSetPar8(void) // ���� DSP�趨����
{
	if (Pw_ScrMenuMode == 0 || Pw_ScrMenuMode == 1)
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "���ϸ�λEn");
			LCD12864_String(0x87, "  ");
			// �ڶ���
			LCD12864_String(0x90, "���ϸ�λ��");
			LCD12864_String(0x97, "��");
			// ������
			LCD12864_String(0x88, "                ");
			LCD12864_String(0x8F, "  ");
			// ������
			LCD12864_String(0x98, "                ");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(2);
		// ��ʾ���޸Ĳ���
		DispMod_Par(0x85, 0x03, 0x00, &Pw_dspFaultAutoResetEn, 2, F_ModPar1);
		DispMod_Par(0x95, 0x03, 0x00, &Pw_dspFaultAutoResetSec, 2, F_ModPar2);

		// DispMod_Par(0x8C, 0x04,0x01, &Pw_ScrEquipPower,4, F_ModPar3);

		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		// д���� 2015.9.1
		if (B_ModPar == 1)
		{
			B_ModPar = 2;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
		}
		if (B_ModPar == 2)
		{
			if (S_RdWrNo == 1)
				Write_Stm32Data(Address(&Pw_dspFaultAutoResetEn, 2), Lw_SaveSetValue);
			else if (S_RdWrNo == 2)
				Write_Stm32Data(Address(&Pw_dspFaultAutoResetSec, 2), Lw_SaveSetValue);
		}

		// ������
		else if (B_ModPar == 0)
		{
			if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
			{
				ReadBPDJPar();
				return;
			}

			// ZCL 2015.9.1 ������ͨѶ����
			if (S_RdWrNo > 2)
				S_RdWrNo = 0;
			if (S_RdWrNo == 0)
			{
				S_RdWrNo = 1;
				S_Com1Send = 0;
				C_Com1Send = 0;
				S_Com1SendNoNum = 0;
				if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
				{
					B_TimeReadBPDJPar = 2;
					return; // ZCL 2019.3.19
				}
			}
			// ��˳���ȡ����
			if (S_RdWrNo == 1)
				Read_Stm32Data(Address(&Pw_dspFaultAutoResetEn, 2), 1);
			else if (S_RdWrNo == 2)
				Read_Stm32Data(Address(&Pw_dspFaultAutoResetSec, 2), 1);
		}
	}

	// ZCL 2019.3.19  DCM220 ��Ƶ��ģʽ
	else if (Pw_ScrMenuMode == 2)
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "���ϸ�λEn");
			LCD12864_String(0x87, "  ");
			// �ڶ���
			LCD12864_String(0x90, "���ϸ�λ��");
			LCD12864_String(0x97, "��");
			// ������
			LCD12864_String(0x88, "�趨��λ��");
			LCD12864_String(0x8F, "  ");
			// ������
			LCD12864_String(0x98, "�Ѹ�λ��");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(4);
		// ��ʾ���޸Ĳ���
		DispMod_Par(0x85, 0x03, 0x00, &Pw_dspFaultAutoResetEn, 2, F_ModPar1);
		DispMod_Par(0x95, 0x03, 0x00, &Pw_dspFaultAutoResetSec, 2, F_ModPar2);
		DispMod_Par(0x8D, 0x03, 0x00, &Pw_dspAutoResetMax, 2, F_ModPar3);
		DispMod_Par(0x9C, 0x05, 0x00, &w_dspAutoResetNum, 3, F_ModPar4);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		// д���� 2015.9.1
		if (B_ModPar == 1)
		{
			B_ModPar = 2;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
		}
		if (B_ModPar == 2)
		{
			if (S_RdWrNo == 1)
				Write_Stm32Data(Address(&Pw_dspFaultAutoResetEn, 2), Lw_SaveSetValue);
			else if (S_RdWrNo == 2)
				Write_Stm32Data(Address(&Pw_dspFaultAutoResetSec, 2), Lw_SaveSetValue);
			else if (S_RdWrNo == 3)
				Write_Stm32Data(Address(&Pw_dspAutoResetMax, 2), Lw_SaveSetValue);
			else if (S_RdWrNo == 4)
				Write_Stm32Data(Address(&w_dspAutoResetNum, 3), Lw_SaveSetValue);
		}

		// ������
		else if (B_ModPar == 0)
		{
			// ZCL 2015.9.1 ������ͨѶ����
			if (S_RdWrNo > 4)
				S_RdWrNo = 0;
			if (S_RdWrNo == 0)
			{
				S_RdWrNo = 1;
				S_Com1Send = 0;
				C_Com1Send = 0;
				S_Com1SendNoNum = 0;
			}
			// ��˳���ȡ����
			if (S_RdWrNo == 1)
				Read_Stm32Data(Address(&Pw_dspFaultAutoResetEn, 2), 1);
			else if (S_RdWrNo == 2)
				Read_Stm32Data(Address(&Pw_dspFaultAutoResetSec, 2), 1);
			else if (S_RdWrNo == 3)
				Read_Stm32Data(Address(&Pw_dspAutoResetMax, 2), 1);
			else if (S_RdWrNo == 4)
				Read_Stm32Data(Address(&w_dspAutoResetNum, 3), 1);
		}
	}
}

void Menu_DspSetPar9(void) // ���� DSP�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "����Ƶ��ͣģʽ");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "����ģʽ");
		LCD12864_String(0x95, " (0-1)"); // ZCL 2018.7.30 ԭ��0x97���ã�������Ϊ(01)ռ2���֣�Ӧ����0x96
		// ������
		LCD12864_String(0x88, "����ģʽ");
		LCD12864_String(0x8D, " (0-2)");

		// LCD12864_String(0x88, "                ");
		// LCD12864_String(0x8F, "  ");

		// ������
		LCD12864_String(0x98, "����С��λ");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x87, 0x01, 0x00, &Pw_dspStartHzStopMode, 2, F_ModPar1);
	DispMod_Par(0x94, 0x01, 0x00, &Pw_dspWenDuSel, 2, F_ModPar2);
	DispMod_Par(0x8C, 0x01, 0x00, &Pw_ScrKeyMode, 4, F_ModPar3); // ����4����DispMod_Par()��û���� ZCL 2018.8.3
	DispMod_Par(0x9D, 0x03, 0x00, &Pw_ScrCurrentBits, 4, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_dspStartHzStopMode, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_dspWenDuSel, 2), Lw_SaveSetValue);

		// ZCL 2018.9.13 Pw_ScrKeyMode����д��Ϊ����������дһ�����õı��� w_dspCommErrCounter ������
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&w_dspCommErrCounter, 3), Lw_SaveSetValue);
		// ZCL 2018.9.13 Pw_ScrCurrentBits����д��Ϊ����������дһ�����õı��� w_dspCounter2 ������
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&w_dspCounter2, 3), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0)
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 2)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_dspStartHzStopMode, 2), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_dspWenDuSel, 2), 1);
		// else if(S_RdWrNo==3)
		// Read_Stm32Data( Address(&Pw_dspStartHzStopMode,2), 1 );
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&w_dspManMadeFault,3), 1 );
	}
}

void Menu_DspSetPar10(void) // ���� DSP�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�˵�ģʽ"); // ZCL 2018.3.30
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "(0��ˮ��1=��Ƶ) ");
		// LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "(2= DCM220)     ");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "��Ƶ����        "); // ZCL 2019.9.11
		LCD12864_String(0x9F, "kW");
	}

	ModParNum(2);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_ScrMenuMode, 4, F_ModPar1);

	DispMod_Par(0x9C, 0x04, 0x01, &Pw_ScrEquipPower, 4, F_ModPar2); // ZCL 2019.9.11

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// ZCL 2018.8.29 �ı��Լ��Ĳ���������ͨѶ����д��
	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_DspSetPar30(void) // ���� DSP�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��Ƶģʽ"); // DCM220
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "��ƵƵ��");
		LCD12864_String(0x97, "Hz");
		// ������
		LCD12864_String(0x88, "Ƶ��ѡ��AI");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "���Ƶ��");
		LCD12864_String(0x9F, "Hz");
	}

	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_dspZDRunMode, 2, F_ModPar1);
	DispMod_Par(0x94, 0x04, 0x01, &Pw_dspRunHz, 2, F_ModPar2);
	DispMod_Par(0x8D, 0x03, 0x00, &Pw_dspHzAISource, 2, F_ModPar3);
	DispMod_Par(0x9C, 0x05, 0x00, &Pw_dspMAXFREQ, 2, F_ModPar4);
	/*			DispMod_Par(0x9C, 0x05,0x00, &Pw_dspSelABCShunXu,2, F_ModPar4); */
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_dspZDRunMode, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_dspRunHz, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_dspHzAISource, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_dspMAXFREQ, 2), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0)
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_dspZDRunMode, 2), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_dspRunHz, 2), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_dspHzAISource, 2), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_dspMAXFREQ, 2), 1);
	}
}

void Menu_DspSetPar31(void) // ���� DSP�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�ǶȲ���");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "�ǶȲ�2 ");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "��������");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "����˳��");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_dspAngleCompRef, 2, F_ModPar1);
	DispMod_Par(0x94, 0x05, 0x00, &Pw_dspAngleCompSet, 2, F_ModPar2);
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_dspCompDirection, 2, F_ModPar3);
	DispMod_Par(0x9C, 0x05, 0x00, &Pw_dspSelABCShunXu, 2, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_dspAngleCompRef, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_dspAngleCompSet, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_dspCompDirection, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_dspSelABCShunXu, 2), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0)
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_dspAngleCompRef, 2), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_dspAngleCompSet, 2), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_dspCompDirection, 2), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_dspSelABCShunXu, 2), 1);
	}
}

// ZCL 2017.3.9
void Menu_DspSetPar32(void) // ���� DSP�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�����Ʋ���ֵ��");
		// LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "����ϵ��");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "����ϵ��");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "΢��ϵ��");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(3);
	// ��ʾ���޸Ĳ���
	// DispMod_Par(0x84, 0x05,0x00, &Pw_dspAngleCompRef,2, F_ModPar1);
	DispMod_Par(0x94, 0x05, 0x00, &Pw_dspICurQsRef, 2, F_ModPar1);
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_dspVoltCompUp, 2, F_ModPar2);
	DispMod_Par(0x9C, 0x05, 0x00, &Pw_dspVoltCompUd, 2, F_ModPar3);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			/* 			Write_Stm32Data( Address(&Pw_dspAngleCompRef,2), Lw_SaveSetValue );
					else if(S_RdWrNo==2) */
			Write_Stm32Data(Address(&Pw_dspICurQsRef, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_dspVoltCompUp, 2), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_dspVoltCompUd, 2), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0)
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			/* 			Read_Stm32Data( Address(&Pw_dspAngleCompRef,2), 1 );
					else if(S_RdWrNo==2) */
			Read_Stm32Data(Address(&Pw_dspICurQsRef, 2), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_dspVoltCompUp, 2), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_dspVoltCompUd, 2), 1);
	}
}

// ZCL 2020.4.8
void Menu_DspSetPar33(void) // ���� DSP�趨����
{
	if (Pw_ScrMenuMode == 2) // ZCL 2020.4.8 Ŀǰֻ��DCM220�����ѹϵ�����棬DCM100��DSP��û��
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "��ѹϵ��");
			LCD12864_String(0x87, "  ");
			// �ڶ���
			LCD12864_String(0x90, "���ٹ�ѹֵ");
			LCD12864_String(0x97, " V");
			// ������
			LCD12864_String(0x88, "����Ƿѹֵ");
			LCD12864_String(0x8F, " V");
			// ������
			LCD12864_String(0x98, "                ");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(3);
		// ��ʾ���޸Ĳ���
		DispMod_Par(0x84, 0x04, 0x02, &Pw_dspBusBarVoltageKi, 2, F_ModPar1);
		DispMod_Par(0x95, 0x03, 0x00, &Pw_dspFastOverVoltage, 2, F_ModPar2);
		DispMod_Par(0x8D, 0x03, 0x00, &Pw_dspFastUnderVoltage, 2, F_ModPar3);

		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		// д���� 2015.9.1
		if (B_ModPar == 1)
		{
			B_ModPar = 2;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
		}
		if (B_ModPar == 2)
		{
			if (S_RdWrNo == 1)
				Write_Stm32Data(Address(&Pw_dspBusBarVoltageKi, 2), Lw_SaveSetValue);
			else if (S_RdWrNo == 2)
				Write_Stm32Data(Address(&Pw_dspFastOverVoltage, 2), Lw_SaveSetValue);
			else if (S_RdWrNo == 3)
				Write_Stm32Data(Address(&Pw_dspFastUnderVoltage, 2), Lw_SaveSetValue);
		}

		// ������
		else if (B_ModPar == 0)
		{
			if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
			{
				ReadBPDJPar();
				return;
			}

			// ZCL 2015.9.1 ������ͨѶ����
			if (S_RdWrNo > 3)
				S_RdWrNo = 0;
			if (S_RdWrNo == 0)
			{
				S_RdWrNo = 1;
				S_Com1Send = 0;
				C_Com1Send = 0;
				S_Com1SendNoNum = 0;
				if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
				{
					B_TimeReadBPDJPar = 2;
					return; // ZCL 2019.3.19
				}
			}
			// ��˳���ȡ����
			if (S_RdWrNo == 1)
				Read_Stm32Data(Address(&Pw_dspBusBarVoltageKi, 2), 1);
			else if (S_RdWrNo == 2)
				Read_Stm32Data(Address(&Pw_dspFastOverVoltage, 2), 1);
			else if (S_RdWrNo == 3)
				Read_Stm32Data(Address(&Pw_dspFastUnderVoltage, 2), 1);
		}
	}
}

// ZCL 2019.3.15 ����DCM220
void Menu_DspSetPar50(void) // ���� DSP�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�޸Ĳ���");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "�ָ�����");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "������ַ");
		LCD12864_String(0x8F, "��");
		// ������
		LCD12864_String(0x98, "(DCM220)        ");
	}

	ModParNum(3);
	DispMod_Par(0x84, 0x05, 0x00, &w_dspModPar, 3, F_ModPar1);
	DispMod_Par(0x94, 0x05, 0x00, &w_dspParInitial, 3, F_ModPar2);
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_dspAddress, 2, F_ModPar3);
	// DispMod_Par(0x9C, 0x03,0x01, &w_Pump1Current,1, 0);
	//
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&w_dspModPar, 3), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&w_dspParInitial, 3), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_dspAddress, 2), Lw_SaveSetValue);
		// else if(S_RdWrNo==4)
		// Write_Stm32Data( Address(&Pw_NoWaterStopP,0), Lw_SaveSetValue );
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2016.12.6  ���if(B_ModPar==0)�� ����ͬ���ĵط�����ӡ�
	{
		if (B_TimeReadBPDJPar == 2) // ZCL 2019.3.19
		{
			ReadBPDJPar();
			return;
		}

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			if (B_TimeReadBPDJPar == 1) // ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			{
				B_TimeReadBPDJPar = 2;
				return; // ZCL 2019.3.19
			}
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_dspModPar, 3), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_dspParInitial, 3), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_dspAddress, 2), 1);
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_NoWaterStopP,0), 1 );
	}
}

void Menu_LoRaMasteSetPar1(void) // ���� LoRa�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�޸�LoRa");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "�ָ�LoRa");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "���ӻ��趨      ");
		LCD12864_String(0x8D, "  ");
		// ������
		LCD12864_String(0x98, "������ַ        ");
		LCD12864_String(0x9F, "  ");
	}

	// ZCL 2019.
	ModParNum(4);
	DispMod_Par(0x84, 0x05, 0x00, &w_GprsModPar, 4, F_ModPar1);		// ZCL 2019.4.2
	DispMod_Par(0x94, 0x05, 0x00, &w_GprsParInitial, 4, F_ModPar2); // ZCL 2019.4.2
	DispMod_Par(0x8D, 0x03, 0x00, &Pw_LoRaMasterSlaveSel, 4, F_ModPar3);
	DispMod_Par(0x9D, 0x03, 0x00, &Pw_LoRaEquipmentNo, 4, F_ModPar4);
	//
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_LoRaSetPar1(void) // ���� LoRa�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�ӻ�1 ��ַ      ");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "�ӻ�2 ��ַ      ");
		LCD12864_String(0x97, "  "); // ZCL 2018.7.30 ԭ��0x97���ã�������Ϊ(01)ռ2���֣�Ӧ����0x96
		// ������
		LCD12864_String(0x88, "�ӻ�3 ��ַ      ");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "�ӻ�4 ��ַ      ");
		LCD12864_String(0x9F, "  "); // ZCL 2018.7.30 ԭ��0x97���ã�������Ϊ(01)ռ2���֣�Ӧ����0x96
	}

	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x85, 0x03, 0x00, &Pw_LoRaModule1Add, 4, F_ModPar1);
	DispMod_Par(0x95, 0x03, 0x00, &Pw_LoRaModule2Add, 4, F_ModPar2);
	DispMod_Par(0x8D, 0x03, 0x00, &Pw_LoRaModule3Add, 4, F_ModPar3); // ����4����DispMod_Par()��û���� ZCL 2018.8.3
	DispMod_Par(0x9D, 0x03, 0x00, &Pw_LoRaModule4Add, 4, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_LoRaSetPar2(void) // ���� LoRa�趨����
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "�ӻ�5 ��ַ      ");
			LCD12864_String(0x87, "  ");
			// �ڶ���
			LCD12864_String(0x90, "�ӻ�6 ��ַ      ");
			LCD12864_String(0x97, "  "); // ZCL 2018.7.30 ԭ��0x97���ã�������Ϊ(01)ռ2���֣�Ӧ����0x96
			// ������
			LCD12864_String(0x88, "�ӻ�7 ��ַ      ");
			LCD12864_String(0x8F, "  ");
			// ������
			LCD12864_String(0x98, "�ӻ�8 ��ַ      ");
			LCD12864_String(0x9F, "  "); // ZCL 2018.7.30 ԭ��0x97���ã�������Ϊ(01)ռ2���֣�Ӧ����0x96
		}

		ModParNum(4);
		// ��ʾ���޸Ĳ���
		DispMod_Par(0x85, 0x03, 0x00, &Pw_LoRaModule5Add, 4, F_ModPar1);
		DispMod_Par(0x95, 0x03, 0x00, &Pw_LoRaModule6Add, 4, F_ModPar2);
		DispMod_Par(0x8D, 0x03, 0x00, &Pw_LoRaModule7Add, 4, F_ModPar3); // ����4����DispMod_Par()��û���� ZCL 2018.8.3
		DispMod_Par(0x9D, 0x03, 0x00, &Pw_LoRaModule8Add, 4, F_ModPar4);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		// д���� 2015.9.1

		// ������
		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "LoRaλ����      ");
			LCD12864_String(0x87, "  ");
			// �ڶ���
			LCD12864_String(0x90, "Gprsλ����      ");
			LCD12864_String(0x97, "  ");
			// ������
			LCD12864_String(0x88, "�������1       ");
			LCD12864_String(0x8F, "  ");
			// ������
			LCD12864_String(0x98, "�������2       ");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(4);
		// ��ʾ���޸Ĳ���
		// ע�⣺Pw_LoRa		Pw_Gprs		Pw_Scr	w_LoRa		w_Gprs		w_Scr ���Ƕ����� w_GprsParLst[]�����еġ�
		DispMod_Par(0x85, 0x03, 0x00, &Pw_LoRaSet, 4, F_ModPar1);
		DispMod_Par(0x95, 0x03, 0x00, &Pw_GprsSet, 4, F_ModPar2);
		DispMod_Par(0x8D, 0x03, 0x00, &Pw_ScrLoRaTest1, 4, F_ModPar3); // ����4����DispMod_Par()��û���� ZCL 2018.8.3
		DispMod_Par(0x9D, 0x03, 0x00, &Pw_ScrLoRaSend2, 4, F_ModPar4);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		// д���� 2015.9.1

		// ������
		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_LoRaSetPar3(void) // ���� LoRa�趨����
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "LoRaλ����      ");
			LCD12864_String(0x87, "  ");
			// �ڶ���
			LCD12864_String(0x90, "Gprsλ����      ");
			LCD12864_String(0x97, "  ");
			// ������
			LCD12864_String(0x88, "�������1       ");
			LCD12864_String(0x8F, "  ");
			// ������
			LCD12864_String(0x98, "�������2       ");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(4);
		// ��ʾ���޸Ĳ���
		// ע�⣺Pw_LoRa		Pw_Gprs		Pw_Scr	w_LoRa		w_Gprs		w_Scr ���Ƕ����� w_GprsParLst[]�����еġ�
		DispMod_Par(0x85, 0x03, 0x00, &Pw_LoRaSet, 4, F_ModPar1);
		DispMod_Par(0x95, 0x03, 0x00, &Pw_GprsSet, 4, F_ModPar2);
		DispMod_Par(0x8D, 0x03, 0x00, &Pw_ScrLoRaTest1, 4, F_ModPar3); // ����4����DispMod_Par()��û���� ZCL 2018.8.3
		DispMod_Par(0x9D, 0x03, 0x00, &Pw_ScrLoRaSend2, 4, F_ModPar4);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		// д���� 2015.9.1

		// ������
		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "LoRaƵ��");
			LCD12864_String(0x87, " M");
			// �ڶ���
			LCD12864_String(0x90, "LoRa����");
			LCD12864_String(0x97, "  ");
			// ������
			LCD12864_String(0x88, "LoRa����");
			LCD12864_String(0x8F, "  ");
			// ������
			LCD12864_String(0x98, "��������");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(4);
		// ��ʾ���޸Ĳ���
		DispMod_Par(0x84, 0x05, 0x00, &Pw_LoRaSetFreq, 4, F_ModPar1);
		DispMod_Par(0x94, 0x05, 0x00, &Pw_LoRaSetPower, 4, F_ModPar2);
		DispMod_Par(0x8C, 0x05, 0x00, &Pw_LoRaSetSignalBW, 4, F_ModPar3); // ����4����DispMod_Par()��û���� ZCL 2018.8.3
		DispMod_Par(0x9C, 0x05, 0x00, &Pw_LoRaSetSpreadingFactor, 4, F_ModPar4);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		// д���� 2015.9.1

		// ������
		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_LoRaSetPar4(void) // ���� LoRa�趨����
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "LoRaƵ��");
			LCD12864_String(0x87, " M");
			// �ڶ���
			LCD12864_String(0x90, "LoRa����");
			LCD12864_String(0x97, "  ");
			// ������
			LCD12864_String(0x88, "LoRa����");
			LCD12864_String(0x8F, "  ");
			// ������
			LCD12864_String(0x98, "��������");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(4);
		// ��ʾ���޸Ĳ���
		DispMod_Par(0x84, 0x05, 0x00, &Pw_LoRaSetFreq, 4, F_ModPar1);
		DispMod_Par(0x94, 0x05, 0x00, &Pw_LoRaSetPower, 4, F_ModPar2);
		DispMod_Par(0x8C, 0x05, 0x00, &Pw_LoRaSetSignalBW, 4, F_ModPar3); // ����4����DispMod_Par()��û���� ZCL 2018.8.3
		DispMod_Par(0x9C, 0x05, 0x00, &Pw_LoRaSetSpreadingFactor, 4, F_ModPar4);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		// д���� 2015.9.1

		// ������
		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "�������");
			LCD12864_String(0x87, "  ");
			// �ڶ���
			LCD12864_String(0x90, "CrcOn   ");
			LCD12864_String(0x97, "  "); // ZCL 2018.7.30 ԭ��0x97���ã�������Ϊ(01)ռ2���֣�Ӧ����0x96
			// ������
			LCD12864_String(0x88, "��ʽ��ͷ");
			LCD12864_String(0x8F, "  ");
			// ������
			LCD12864_String(0x98, "���յ���");
			LCD12864_String(0x9F, "  "); // ZCL 2018.7.30 ԭ��0x97���ã�������Ϊ(01)ռ2���֣�Ӧ����0x96
		}

		ModParNum(4);
		// ��ʾ���޸Ĳ���
		DispMod_Par(0x84, 0x05, 0x00, &Pw_LoRaSetErrorCoding, 4, F_ModPar1);
		DispMod_Par(0x94, 0x05, 0x00, &Pw_LoRaSetCrcOn, 4, F_ModPar2);
		DispMod_Par(0x8C, 0x05, 0x00, &Pw_LoRaSetImplicitHeaderOn, 4, F_ModPar3); // ����4����DispMod_Par()��û���� ZCL 2018.8.3
		DispMod_Par(0x9C, 0x05, 0x00, &Pw_LoRaSetRxSingleOn, 4, F_ModPar4);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		// д���� 2015.9.1

		// ������
		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_LoRaSetPar5(void) // ���� LoRa�趨����
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "���ͳ�ʱ      ");
			LCD12864_String(0x87, "ms");
			// �ڶ���
			LCD12864_String(0x90, "���ճ�ʱ      ");
			LCD12864_String(0x97, "ms");
			// ������
			LCD12864_String(0x88, "��������      ");
			LCD12864_String(0x8F, "kb");
			// ������
			LCD12864_String(0x98, "                 ");
			LCD12864_String(0x9F, "  "); // ZCL 2018.7.30 ԭ��0x97���ã�������Ϊ(01)ռ2���֣�Ӧ����0x96
		}

		ModParNum(2);
		// ��ʾ���޸Ĳ���
		DispMod_Par(0x84, 0x05, 0x00, &Pw_LoRaSetTxPacketTimeOut, 4, F_ModPar1);
		DispMod_Par(0x94, 0x05, 0x00, &Pw_LoRaSetRxPacketTimeOut, 4, F_ModPar2);
		DispMod_Par(0x8C, 0x04, 0x02, &w_LoRaDateRate, 5, 0); // ����4��5����DispMod_Par()��û���� ZCL 2018.8.3
		// DispMod_Par(0x9C, 0x05, 0x00, &Pw_LoRaSetRxSingleOn, 4, F_ModPar4);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		// д���� 2015.9.1

		// ������
		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "FreqHopOn ");
			LCD12864_String(0x87, "  ");
			// �ڶ���
			LCD12864_String(0x90, "HopPeriod ");
			LCD12864_String(0x97, "  ");
			// ������
			LCD12864_String(0x88, "PayLoadLen");
			LCD12864_String(0x8F, "  ");
			// ������
			LCD12864_String(0x98, "                ");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(3);
		// ��ʾ���޸Ĳ���
		DispMod_Par(0x85, 0x03, 0x00, &Pw_LoRaSetFreqHopOn, 4, F_ModPar1);
		DispMod_Par(0x95, 0x03, 0x00, &Pw_LoRaSetHopPeriod, 4, F_ModPar2);
		DispMod_Par(0x8D, 0x03, 0x00, &Pw_LoRaSetPayLoadLength, 4, F_ModPar3); // ����4����DispMod_Par()��û���� ZCL 2018.8.3
		// DispMod_Par(0x9D, 0x03,0x00, &Pw_LoRaSetRxSingleOn,4, F_ModPar4);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		// д���� 2015.9.1

		// ������
		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_LoRaSetPar6(void) // ���� LoRa�趨����
{
	// if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "�������      ");
			LCD12864_String(0x87, "��");
			// �ڶ���
			LCD12864_String(0x90, "������Ϣ      ");
			LCD12864_String(0x97, "  ");
			// ������
			LCD12864_String(0x88, "�豸����      ");
			LCD12864_String(0x8F, "  ");
			// ������
			LCD12864_String(0x98, "(0˫����1=��Ƶ) ");
			// LCD12864_String(0x9F, "  ");
		}

		ModParNum(3);
		// ��ʾ���޸Ĳ���
		DispMod_Par(0x84, 0x05, 0x00, &Pw_RebootInterval, 4, F_ModPar1);
		DispMod_Par(0x94, 0x01, 0x00, &Pw_ConsoleInfo, 4, F_ModPar2);
		DispMod_Par(0x8C, 0x01, 0x00, &Pw_EquipmentType, 5, F_ModPar3); // ����4��5����DispMod_Par()��û���� ZCL 2018.8.3
		// DispMod_Par(0x9D, 0x03, 0x00, &Pw_ConsoleInfo, 4, F_ModPar4);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		// д���� 2015.9.1

		// ������
		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	// else // ��Ƶ���
	// {
	// 	if (Lw_SavePageNo != Lw_PageNo)
	// 	{
	// 		Lw_SavePageNo = Lw_PageNo;
	// 		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

	// 		// LCD12864_ClrText();          //����
	// 		// ��һ��
	// 		LCD12864_String(0x80, "���ͳ�ʱ      ");
	// 		LCD12864_String(0x87, "ms");
	// 		// �ڶ���
	// 		LCD12864_String(0x90, "���ճ�ʱ      ");
	// 		LCD12864_String(0x97, "ms");
	// 		// ������
	// 		LCD12864_String(0x88, "��������      ");
	// 		LCD12864_String(0x8F, "kb");
	// 		// ������
	// 		LCD12864_String(0x98, "�������      ");
	// 		LCD12864_String(0x9F, "��");
	// 	}

	// 	ModParNum(4);
	// 	// ��ʾ���޸Ĳ���
	// 	DispMod_Par(0x84, 0x05, 0x00, &Pw_LoRaSetTxPacketTimeOut, 4, F_ModPar1);
	// 	DispMod_Par(0x94, 0x05, 0x00, &Pw_LoRaSetRxPacketTimeOut, 4, F_ModPar2);
	// 	DispMod_Par(0x8C, 0x04, 0x02, &w_LoRaDateRate, 5, F_ModPar3);	 // ����4��5����DispMod_Par()��û���� ZCL 2018.8.3
	// 	DispMod_Par(0x9C, 0x05, 0x00, &Pw_RebootInterval, 4, F_ModPar4); //=0��ͨ��Э�飻=1����ɽ����Э�飨ͨ��3��ͨ��4��
	// 	S_DisplayPar = 0;												 // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// 	// д���� 2015.9.1

	// 	// ������
	// 	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	// }
}

void Menu_LoRaSetPar7(void) // ���� LoRa�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�ı��汾");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "��������");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "��������");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(0);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x02, &w_ScrVERSION, 5, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_ScrWriteYear, 1, 0); // ZCL 2019.4.3
	DispMod_Par(0x8C, 0x05, 0x00, &w_ScrWriteDate, 1, 0);
	// DispMod_Par(0x9C, 0x05, 0x00, &w_ScrWriteDate, 1, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_GprsReadPar1(void) // ���� Gprs����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "    DTU ����    ");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "ID:             ");
		LCD12864_String(0x97, "  "); // ZCL 2018.7.30 ԭ��0x97���ã�������Ϊ(01)ռ2���֣�Ӧ����0x96
		// ������
		LCD12864_String(0x88, "IP1:            ");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "   .   .   .    ");
		LCD12864_String(0x9F, "  "); // ZCL 2018.7.30 ԭ��0x97���ã�������Ϊ(01)ռ2���֣�Ӧ����0x96
	}

	ModParNum(0);
	// ��ʾ���޸Ĳ���

	// DispMod_Par(0x92, 0x04,0x00, &w_GprsDtuID1,5, 0);
	// DispMod_Par(0x94, 0x04,0x00, &w_GprsDtuID2,5, 0);
	// DispMod_Par(0x96, 0x03,0x00, &w_GprsDtuID3,5, 0);

	if (Key_Data == 0) // ZCL 2019.4.5 ���ϰ������ƣ���ֹ����������ʱ��������Ӧ����
	{
		DispMod_Par2_String(0x92, (const char *)&GprsPar[DtuNoBase]);

		// ZCL 2019.4.4 ����ʹ��LCD12864_Numʱ��Ҫ�ӵ���ʱ��������ֹ������ص��� ��1MS��ʱ OK
		DispMod_Par3_Num(0x98, GprsPar[Ip0Base + 0]);
		DispMod_Par3_Num(0x9A, GprsPar[Ip0Base + 1]);
		DispMod_Par3_Num(0x9C, GprsPar[Ip0Base + 2]);
		DispMod_Par3_Num(0x9E, GprsPar[Ip0Base + 3]);
	}
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_GprsReadPar2(void) // ���� Gprs����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "IP2-IP4:        ");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "   .   .   .    ");
		LCD12864_String(0x97, "  "); // ZCL 2018.7.30 ԭ��0x97���ã�������Ϊ(01)ռ2���֣�Ӧ����0x96
		// ������
		LCD12864_String(0x88, "   .   .   .    ");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "   .   .   .    ");
		LCD12864_String(0x9F, "  "); // ZCL 2018.7.30 ԭ��0x97���ã�������Ϊ(01)ռ2���֣�Ӧ����0x96
	}

	ModParNum(0);
	// ��ʾ���޸Ĳ���

	if (Key_Data == 0) // ZCL 2019.4.5 ���ϰ������ƣ���ֹ����������ʱ��������Ӧ����
	{
		// ZCL 2019.4.4 ����ʹ��LCD12864_Numʱ��Ҫ�ӵ���ʱ��������ֹ������ص��� ��1MS��ʱ OK
		DispMod_Par3_Num(0x90, GprsPar[Ip0Base + 4]);
		DispMod_Par3_Num(0x92, GprsPar[Ip0Base + 5]);
		DispMod_Par3_Num(0x94, GprsPar[Ip0Base + 6]);
		DispMod_Par3_Num(0x96, GprsPar[Ip0Base + 7]);

		DispMod_Par3_Num(0x88, GprsPar[Ip0Base + 8]);
		DispMod_Par3_Num(0x8A, GprsPar[Ip0Base + 9]);
		DispMod_Par3_Num(0x8C, GprsPar[Ip0Base + 10]);
		DispMod_Par3_Num(0x8E, GprsPar[Ip0Base + 11]);

		DispMod_Par3_Num(0x98, GprsPar[Ip0Base + 12]);
		DispMod_Par3_Num(0x9A, GprsPar[Ip0Base + 13]);
		DispMod_Par3_Num(0x9C, GprsPar[Ip0Base + 14]);
		DispMod_Par3_Num(0x9E, GprsPar[Ip0Base + 15]);
	}

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_GprsReadPar3(void) // ���� Gprs����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�˿�1-�˿�4:    ");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		LCD12864_String(0x97, "  "); // ZCL 2018.7.30 ԭ��0x97���ã�������Ϊ(01)ռ2���֣�Ӧ����0x96
		// ������
		LCD12864_String(0x88, "                ");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
		LCD12864_String(0x9F, "  "); // ZCL 2018.7.30 ԭ��0x97���ã�������Ϊ(01)ռ2���֣�Ӧ����0x96
	}

	ModParNum(0);
	// ��ʾ���޸Ĳ���

	if (Key_Data == 0) // ZCL 2019.4.5 ���ϰ������ƣ���ֹ����������ʱ��������Ӧ����
	{
		// ZCL 2019.4.4 ����ʹ��LCD12864_Numʱ��Ҫ�ӵ���ʱ��������ֹ������ص��� ��1MS��ʱ OK
		DispMod_Par3_Num(0x91, (GprsPar[Port0Base + 0] << 8) + GprsPar[Port0Base + 1]);
		DispMod_Par3_Num(0x95, (GprsPar[Port1Base + 0] << 8) + GprsPar[Port1Base + 1]);
		DispMod_Par3_Num(0x89, (GprsPar[Port2Base + 0] << 8) + GprsPar[Port2Base + 1]);
		DispMod_Par3_Num(0x8D, (GprsPar[Port3Base + 0] << 8) + GprsPar[Port3Base + 1]);
	}

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_GprsReadPar4(void) // ���� Gprs����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��·Э��UDP.TCP:");
		// LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		LCD12864_String(0x95, " (0-1)"); // ZCL 2018.7.30 ԭ��0x97���ã�������Ϊ(01)ռ2���֣�Ӧ����0x96
		// ������
		LCD12864_String(0x88, "�ź�ǿ��");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "Xmodem��");
		LCD12864_String(0x9F, "  "); // ZCL 2018.7.30 ԭ��0x97���ã�������Ϊ(01)ռ2���֣�Ӧ����0x96
	}

	ModParNum(0);
	// ��ʾ���޸Ĳ���

	if (Key_Data == 0) // ZCL 2019.4.5 ���ϰ������ƣ���ֹ����������ʱ��������Ӧ����
	{
		// ZCL 2019.4.4 ����ʹ��LCD12864_Numʱ��Ҫ�ӵ���ʱ��������ֹ������ص��� ��1MS��ʱ OK
		DispMod_Par3_Num(0x91, GprsPar[LinkTCPUDP0Base + 0]);
		DispMod_Par3_Num(0x92, GprsPar[LinkTCPUDP0Base + 1]);
		DispMod_Par3_Num(0x93, GprsPar[LinkTCPUDP0Base + 2]);
		DispMod_Par3_Num(0x94, GprsPar[LinkTCPUDP0Base + 3]);
	}

	// ��ʾ���޸Ĳ���
	// ע�⣺Pw_LoRa		Pw_Gprs		Pw_Scr	w_LoRa		w_Gprs		w_Scr ���Ƕ����� w_GprsParLst[]�����еġ�
	DispMod_Par(0x8C, 0x05, 0x00, &w_GprsCSQ, 5, 0);
	DispMod_Par(0x9C, 0x05, 0x00, &w_GprsXmodemFTItem, 5, 0);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_Dsp1ReadPar1(void) // ���� DSP������1
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "                ");
			// �ڶ���
			LCD12864_String(0x90, "                ");
			// ������
			LCD12864_String(0x88, "   1�Ÿ�Ƶ˫��  ");
			// ������
			LCD12864_String(0x98, "                ");
		}

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "                ");
			// �ڶ���
			LCD12864_String(0x90, "                ");
			// ������
			LCD12864_String(0x88, "   1�ű�Ƶ���  ");
			// ������
			LCD12864_String(0x98, "                ");
		}

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

// ZCL 2019.3.8 ������ʾ������
void Menu_Dsp1ReadPar2_BK(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��ǰƵ��");
		LCD12864_String(0x87, "Hz");

		// �ڶ���
		LCD12864_String(0x90, "�����ѹ");
		LCD12864_String(0x97, " V");

		// ������
		LCD12864_String(0x88, "�������");
		LCD12864_String(0x8F, " A");

		// ������
		LCD12864_String(0x98, "ģ���¶�");
		LCD12864_String(0x9F, "��");
	}

	ModParNum(0);
	// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

	DispMod_Par(0x84, 0x04, 0x01, &w_dsp1NowHz, 5, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_dsp1OutVoltage, 5, 0);

	if (Pw_ScrCurrentBits == 1) // ZCL 2018.8.29  ����1λС��
	{
		DispMod_Par(0x8C, 0x04, 0x01, &w_dsp1APhaseCurrent, 5, 0); // ZCL 2018.5.19
	}
	else if (Pw_ScrCurrentBits == 2) // ZCL 2018.8.29  ����2λС��
	{
		DispMod_Par(0x8C, 0x04, 0x02, &w_dsp1APhaseCurrent, 5, 0); // ZCL 2018.5.19
	}

	/* #ifdef CURRENT_OneXiaoShu		//ZCL 2018.1.26  ����1λС��
	#else
	#endif	 */
	// ZCL 2018.8.29 ��Pw_ScrCurrentBits ����

	DispMod_Par(0x9C, 0x04, 0x01, &w_dsp1NowTemperature, 5, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

// ZCL 2019.3.8 ������ʾ������
void Menu_Dsp1ReadPar2(void) // ���� DSP������
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		//	u32 nl_i;
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "AB~ ��ѹ");
			LCD12864_String(0x87, " V");

			// �ڶ���
			LCD12864_String(0x90, "A~����  ");
			LCD12864_String(0x97, " A");

			// ������
			LCD12864_String(0x88, "LoRa������");
			LCD12864_String(0x8F, "  ");

			if (B_LoRaErrWithModule1 == 0)
			{
				// ������
				LCD12864_String(0x98, "DNB ������");
				LCD12864_String(0x9F, "  ");
			}
			else
			{
				// ������
				LCD12864_String(0x98, "1#��LoRaͨѶ�쳣");
			}
		}

		ModParNum(0);
		DispMod_Par(0x84, 0x04, 0x01, &w_Lora1UabRms, 5, 0);
		// DispMod_Par(0x93, 0x06,0x01, &w_UaRms,5, 0);
		DispMod_Par(0x93, 0x06, 0x02, &w_Lora1IaRms, 5, 0);

		DispMod_Par(0x8D, 0x05, 0x00, &w_LoraCounter1, 5, 0); // YLS 2024.10.20

		if (B_LoRaErrWithModule1 == 0)
			DispMod_Par(0x9D, 0x05, 0x00, &w_Lora1Counter1_BPQ, 5, 0); // ZCL 2018.5.19

		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "1 ");
			LCD12864_String(0x87, "  ");

			// �ڶ���
			LCD12864_String(0x90, "  ");
			LCD12864_String(0x97, "  ");

			// ������
			LCD12864_String(0x88, "  ");
			LCD12864_String(0x8F, "  ");

			// ������
			LCD12864_String(0x98, "  ");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(0);
		// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

		DispMod_Par(0x81, 0x05, 0x00, &w_WaterMeterID1, 5, 0);
		DispMod_Par(0x84, 0x05, 0x00, &w_WaterMeterID2, 5, 0);

		DispMod_Par(0x91, 0x05, 0x00, &w_WaterMeterSum1, 5, 0);
		DispMod_Par(0x94, 0x05, 0x00, &w_WaterMeterSum2, 5, 0);

		DispMod_Par(0x89, 0x05, 0x00, &w_WaterMeterInstantFlow1, 5, 0);
		DispMod_Par(0x8C, 0x05, 0x00, &w_WaterMeterInstantFlow2, 5, 0);

		DispMod_Par(0x99, 0x05, 0x00, &w_WaterMeterInSum1, 5, 0);
		DispMod_Par(0x9C, 0x05, 0x00, &w_WaterMeterInSum2, 5, 0);

		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_Dsp1ReadPar3(void) // ���� DSP������
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "220 ������");
			LCD12864_String(0x87, "  ");

			// �ڶ���
			LCD12864_String(0x90, "Զ������    ");
			LCD12864_String(0x97, "  ");

			// ������
			LCD12864_String(0x88, "������ͣ    ");
			LCD12864_String(0x8F, "  ");

			// ������
			LCD12864_String(0x98, "                ");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(1);

		DispMod_Par(0x85, 0x05, 0x00, &w_Lora1Counter2, 5, 0); // ZCL 2018.5.19

		DispMod_Par(0x95, 0x05, 0x00, &w_ZhuanFaData, 5, 0); // ZCL 2018.5.19

		DispMod_Par(0x8D, 0x05, 0x00, &w_Pump1RemoteStop, 5, F_ModPar1); // ZCL 2018.5.19

		if (F_ModeParLora == 0)
		{
			if (w_Pump1RemoteStop != w_SavePump1RemoteStop)
			{
				if (w_Pump1RemoteStop > 1)
				{
					w_Pump1RemoteStop = 0;
				}
				else
				{
					w_SavePump1RemoteStop = w_Pump1RemoteStop;

					w_ZhuanFaAdd = Addr_Lora1TouchRunStop;

					w_ZhuanFaData = w_Pump1RemoteStop;

					F_ModeParLora = 1;
				}
			}
		}

		// DispMod_Par(0x94, 0x04,0x01, &w_HighV1DispDianLi5,5, 0);
		// DispMod_Par(0x8C, 0x04,0x01, &w_HighV1DispDianLi6,5, 0);
		// DispMod_Par(0x9C, 0x05,0x00, &w_HighV1DispDianLi7,5, 0);

		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "2 ");
			LCD12864_String(0x87, "  ");

			// �ڶ���
			LCD12864_String(0x90, "  ");
			LCD12864_String(0x97, "  ");

			// ������
			LCD12864_String(0x88, "  ");
			LCD12864_String(0x8F, "  ");

			// ������
			LCD12864_String(0x98, "  ");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(0);
		// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

		DispMod_Par(0x81, 0x05, 0x00, &w_WaterMeterOutSum1, 5, 0);
		DispMod_Par(0x84, 0x05, 0x00, &w_WaterMeterOutSum2, 5, 0);

		DispMod_Par(0x91, 0x05, 0x00, &w_WaterMeterRemainMoney1, 5, 0);
		DispMod_Par(0x94, 0x05, 0x00, &w_WaterMeterRemainMoney2, 5, 0);

		DispMod_Par(0x89, 0x05, 0x00, &w_WaterMeterOverdraft1, 5, 0);
		DispMod_Par(0x8C, 0x05, 0x00, &w_WaterMeterOverdraft2, 5, 0);

		DispMod_Par(0x99, 0x05, 0x00, &w_WaterMeterStatus1, 5, 0);
		DispMod_Par(0x9C, 0x05, 0x00, &w_WaterMeterStatus2, 5, 0);

		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_Dsp1ReadPar3_BK(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "������1 ");
		LCD12864_String(0x87, "  ");

		// �ڶ���
		LCD12864_String(0x90, "����汾");
		LCD12864_String(0x97, "  ");

		// ������
		LCD12864_String(0x88, "�������");
		LCD12864_String(0x8F, "  ");

		// ������
		LCD12864_String(0x98, "                ");
	}

	ModParNum(0);
	// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

	DispMod_Par(0x84, 0x05, 0x00, &w_dsp1SysErrorCode1, 5, 0);
	DispMod_Par(0x94, 0x04, 0x02, &w_dsp1SoftVersion, 5, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_dsp1Counter3, 5, 0); // ZCL 2018.5.19
	// DispMod_Par(0x9C, 0x04,0x01, &w_dspNowTemperature,5, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_Dsp1ReadPar4(void) // ���� DSP������
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "��ˮѹ��");
			LCD12864_String(0x87, "Mp");
			// �ڶ���
			LCD12864_String(0x90, "��ˮѹ��");
			LCD12864_String(0x97, "Mp");
			// ������
			LCD12864_String(0x88, "�趨ѹ��");
			LCD12864_String(0x8F, "Mp");
			// ������
			LCD12864_String(0x98, "��ƵƵ��");
			LCD12864_String(0x9F, "Hz");
		}

		ModParNum(0); // ָ��:�޸Ĳ���������
		// ZCL 2018.5.16 �޸Ĳ�������ʱ���ӿ��ٶȱ�֤�л�����ʾ������ʱ������Һ����ʾ�����ϣ���ʾ����
		// ��ʾ���޸Ĳ���
		DispMod_Par(0x84, 0x04, 0x03, &w_Lora1InP, 5, 0);
		DispMod_Par(0x94, 0x04, 0x03, &w_Lora1OutP, 5, 0);
		DispMod_Par(0x8C, 0x04, 0x03, &w_Lora1SetP, 5, 0);
		DispMod_Par(0x9C, 0x04, 0x01, &w_Lora1VvvfFreq, 5, 0);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "3 ");
			LCD12864_String(0x87, "  ");

			// �ڶ���
			LCD12864_String(0x90, "  ");
			LCD12864_String(0x97, "  ");

			// ������
			LCD12864_String(0x88, "  ");
			LCD12864_String(0x8F, "  ");

			// ������
			LCD12864_String(0x98, "  ");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(0);
		// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

		DispMod_Par(0x81, 0x05, 0x00, &w_WaterMeterNoUse1, 5, 0);
		DispMod_Par(0x84, 0x05, 0x00, &w_WaterMeterNoUse2, 5, 0);

		DispMod_Par(0x91, 0x05, 0x00, &w_WaterMeterTime1, 5, 0);
		DispMod_Par(0x94, 0x05, 0x00, &w_WaterMeterTime2, 5, 0);

		DispMod_Par(0x89, 0x05, 0x00, &w_WaterMeterTime3, 5, 0);
		DispMod_Par(0x8C, 0x05, 0x00, &w_WaterMeterSoftVer, 5, 0);

		DispMod_Par(0x99, 0x05, 0x00, &w_WaterMeterNoUse3, 5, 0);
		DispMod_Par(0x9C, 0x05, 0x00, &w_WaterMeterNoUse4, 5, 0);

		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_Dsp1ReadPar4_BK(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��ˮѹ��");
		LCD12864_String(0x87, "Mp");
		// �ڶ���
		LCD12864_String(0x90, "��ˮѹ��");
		LCD12864_String(0x97, "Mp");
		// ������
		LCD12864_String(0x88, "�趨ѹ��");
		LCD12864_String(0x8F, "Mp");

		// ������
		LCD12864_String(0x98, "                ");
	}

	ModParNum(0);
	// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

	// #define	w_1InPDec								w_GprsParLst[128]		//.3 ��ˮѹ��
	// #define	w_1OutPDec							w_GprsParLst[129]		//.3 ��ˮѹ��
	// #define	w_1PIDCalcP							w_GprsParLst[130]		//.3 �趨ѹ��

	DispMod_Par(0x84, 0x04, 0x03, &w_1InPDec, 5, 0);
	DispMod_Par(0x94, 0x04, 0x03, &w_1OutPDec, 5, 0);
	DispMod_Par(0x8C, 0x04, 0x03, &w_1PIDCalcP, 5, 0); // ZCL 2018.5.19
	// DispMod_Par(0x9C, 0x04,0x01, &w_dspNowTemperature,5, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_Dsp1ReadPar5(void) // ���� DSP������
{
	u8 i1 = 0, i2 = 0;
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "�¶ȱ�1 ");
			LCD12864_String(0x87, "��");
			// �ڶ���
			LCD12864_String(0x90, "�¶ȱ�2 ");
			LCD12864_String(0x97, "��");
			// ������
			LCD12864_String(0x88, "����汾");
			LCD12864_String(0x8F, "  ");
			// ������
			LCD12864_String(0x98, "                ");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(0);

		if (w_Pump1DispWenDu1DecValue >= 10000)
		{
			w_Pump1DispWenDu1DecValue = w_Pump1DispWenDu1DecValue - 10000;
			i1 = 10; // ZCL 2019.10.18 ���ϸ���
		}
		if (w_Pump1DispWenDu2DecValue >= 10000)
		{
			w_Pump1DispWenDu2DecValue = w_Pump1DispWenDu2DecValue - 10000;
			i2 = 10; // ZCL 2019.10.18 ���ϸ���
		}

		DispMod_Par(0x84, 0x04, 0x01, &w_Pump1DispWenDu1DecValue, 5 + i1, 0);
		DispMod_Par(0x94, 0x04, 0x01, &w_Pump1DispWenDu2DecValue, 5 + i2, 0);

		DispMod_Par(0x8C, 0x04, 0x02, &w_Lora1SoftVersion, 5, 0);
		// DispMod_Par(0x9C, 0x05,0x00, &w_HighV1Counter,5, 0);		//ZCL 2018.5.19

		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}

	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "��ˮ״̬        ");
			LCD12864_String(0x87, "  ");
			// �ڶ���
			LCD12864_String(0x90, "ͣ��ԭ��        ");
			LCD12864_String(0x97, "  ");
			// ������
			LCD12864_String(0x88, "��Ƶ����        ");
			LCD12864_String(0x8F, "kW");

			// ������
			LCD12864_String(0x98, "                ");
		}

		ModParNum(0);
		// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

		// #define	w_1EquipOperateStatus  	w_GprsParLst[131]		//.0 ��ˮ״̬
		// #define	w_1EquipAlarmStatus			w_GprsParLst[132]		//.0 ͣ��ԭ��
		// #define	w_1ScrEquipPower				w_GprsParLst[133]		//.0 �豸����

		DispMod_Par(0x84, 0x05, 0x00, &w_1EquipOperateStatus, 5, 0);
		DispMod_Par(0x94, 0x05, 0x00, &w_1EquipAlarmStatus, 5, 0);
		DispMod_Par(0x8C, 0x04, 0x01, &w_1ScrEquipPower, 5, 0); // ZCL 2018.5.19
		// DispMod_Par(0x9C, 0x04,0x01, &w_dspNowTemperature,5, 0);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_Dsp2ReadPar1(void) // ���� DSP������1
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "                ");
			// �ڶ���
			LCD12864_String(0x90, "                ");
			// ������
			LCD12864_String(0x88, "   2�Ÿ�Ƶ˫��  ");
			// ������
			LCD12864_String(0x98, "                ");
		}

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "                ");
			// �ڶ���
			LCD12864_String(0x90, "                ");
			// ������
			LCD12864_String(0x88, "   2�ű�Ƶ���  ");
			// ������
			LCD12864_String(0x98, "                ");
		}

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

// ZCL 2019.3.8 ������ʾ������
void Menu_Dsp2ReadPar2(void) // ���� DSP������
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		//	u32 nl_i;
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "AB~ ��ѹ");
			LCD12864_String(0x87, " V");

			// �ڶ���
			LCD12864_String(0x90, "A~����  ");
			LCD12864_String(0x97, " A");

			// ������
			LCD12864_String(0x88, "LoRa������");
			LCD12864_String(0x8F, "  ");
			if (B_LoRaErrWithModule2 == 0)
			{
				// ������
				LCD12864_String(0x98, "DNB ������");
				LCD12864_String(0x9F, "  ");
			}
			else
			{
				// ������
				LCD12864_String(0x98, "2#��LoRaͨѶ�쳣");
			}
		}

		ModParNum(0);
		DispMod_Par(0x84, 0x04, 0x01, &w_Lora2UabRms, 5, 0);
		// DispMod_Par(0x93, 0x06,0x01, &w_UaRms,5, 0);
		DispMod_Par(0x93, 0x06, 0x02, &w_Lora2IaRms, 5, 0);

		DispMod_Par(0x8D, 0x05, 0x00, &w_LoraCounter2, 5, 0); // YLS 2024.10.20

		if (B_LoRaErrWithModule2 == 0)
			DispMod_Par(0x9D, 0x05, 0x00, &w_Lora2Counter1_BPQ, 5, 0); // ZCL 2018.5.19

		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "��ǰƵ��");
			LCD12864_String(0x87, "Hz");

			// �ڶ���
			LCD12864_String(0x90, "�����ѹ");
			LCD12864_String(0x97, " V");

			// ������
			LCD12864_String(0x88, "�������");
			LCD12864_String(0x8F, " A");

			// ������
			LCD12864_String(0x98, "ģ���¶�");
			LCD12864_String(0x9F, "��");
		}

		ModParNum(0);
		// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

		DispMod_Par(0x84, 0x04, 0x01, &w_dsp2NowHz, 5, 0);
		DispMod_Par(0x94, 0x05, 0x00, &w_dsp2OutVoltage, 5, 0);

		if (Pw_ScrCurrentBits == 1) // ZCL 2018.8.29  ����1λС��
		{
			DispMod_Par(0x8C, 0x04, 0x01, &w_dsp2APhaseCurrent, 5, 0); // ZCL 2018.5.19
		}
		else if (Pw_ScrCurrentBits == 2) // ZCL 2018.8.29  ����2λС��
		{
			DispMod_Par(0x8C, 0x04, 0x02, &w_dsp2APhaseCurrent, 5, 0); // ZCL 2018.5.19
		}

		/* #ifdef CURRENT_OneXiaoShu		//ZCL 2018.1.26  ����1λС��
		#else
		#endif	 */
		// ZCL 2018.8.29 ��Pw_ScrCurrentBits ����

		DispMod_Par(0x9C, 0x04, 0x01, &w_dsp2NowTemperature, 5, 0);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_Dsp2ReadPar3(void) // ���� DSP������
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "220 ������");
			LCD12864_String(0x87, "  ");

			// �ڶ���
			LCD12864_String(0x90, "Զ������    ");
			LCD12864_String(0x97, "  ");

			// ������
			LCD12864_String(0x88, "������ͣ    ");
			LCD12864_String(0x8F, "  ");

			// ������
			LCD12864_String(0x98, "                ");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(1);

		DispMod_Par(0x85, 0x05, 0x00, &w_Lora2Counter2, 5, 0); // ZCL 2018.5.19

		DispMod_Par(0x95, 0x05, 0x00, &w_ZhuanFaData, 5, 0); // ZCL 2018.5.19

		DispMod_Par(0x8D, 0x05, 0x00, &w_Pump2RemoteStop, 5, F_ModPar1); // ZCL 2018.5.19

		if (F_ModeParLora == 0)
		{
			if (w_Pump2RemoteStop != w_SavePump2RemoteStop)
			{
				if (w_Pump2RemoteStop > 1)
				{
					w_Pump2RemoteStop = 0;
				}
				else
				{
					w_SavePump2RemoteStop = w_Pump2RemoteStop;

					w_ZhuanFaAdd = Addr_Lora2TouchRunStop;

					w_ZhuanFaData = w_Pump2RemoteStop;

					F_ModeParLora = 1;
				}
			}
		}

		// DispMod_Par(0x94, 0x04,0x01, &w_HighV1DispDianLi5,5, 0);
		// DispMod_Par(0x8C, 0x04,0x01, &w_HighV1DispDianLi6,5, 0);
		// DispMod_Par(0x9C, 0x05,0x00, &w_HighV1DispDianLi7,5, 0);

		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "������1 ");
			LCD12864_String(0x87, "  ");

			// �ڶ���
			LCD12864_String(0x90, "����汾");
			LCD12864_String(0x97, "  ");

			// ������
			LCD12864_String(0x88, "�������");
			LCD12864_String(0x8F, "  ");

			// ������
			LCD12864_String(0x98, "                ");
		}

		ModParNum(0);
		// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

		DispMod_Par(0x84, 0x05, 0x00, &w_dsp2SysErrorCode1, 5, 0);
		DispMod_Par(0x94, 0x04, 0x02, &w_dsp2SoftVersion, 5, 0);
		DispMod_Par(0x8C, 0x05, 0x00, &w_dsp2Counter3, 5, 0); // ZCL 2018.5.19
		// DispMod_Par(0x9C, 0x04,0x01, &w_dspNowTemperature,5, 0);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_Dsp2ReadPar4(void) // ���� DSP������
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "��ˮѹ��");
			LCD12864_String(0x87, "Mp");
			// �ڶ���
			LCD12864_String(0x90, "��ˮѹ��");
			LCD12864_String(0x97, "Mp");
			// ������
			LCD12864_String(0x88, "�趨ѹ��");
			LCD12864_String(0x8F, "Mp");
			// ������
			LCD12864_String(0x98, "��ƵƵ��");
			LCD12864_String(0x9F, "Hz");
		}

		ModParNum(0); // ָ��:�޸Ĳ���������
		// ZCL 2018.5.16 �޸Ĳ�������ʱ���ӿ��ٶȱ�֤�л�����ʾ������ʱ������Һ����ʾ�����ϣ���ʾ����
		// ��ʾ���޸Ĳ���
		DispMod_Par(0x84, 0x04, 0x03, &w_Lora2InP, 5, 0);
		DispMod_Par(0x94, 0x04, 0x03, &w_Lora2OutP, 5, 0);
		DispMod_Par(0x8C, 0x04, 0x03, &w_Lora2SetP, 5, 0);
		DispMod_Par(0x9C, 0x04, 0x01, &w_Lora2VvvfFreq, 5, 0);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "��ˮѹ��");
			LCD12864_String(0x87, "Mp");
			// �ڶ���
			LCD12864_String(0x90, "��ˮѹ��");
			LCD12864_String(0x97, "Mp");
			// ������
			LCD12864_String(0x88, "�趨ѹ��");
			LCD12864_String(0x8F, "Mp");

			// ������
			LCD12864_String(0x98, "                ");
		}

		ModParNum(0);
		// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

		// #define	w_1InPDec								w_GprsParLst[128]		//.3 ��ˮѹ��
		// #define	w_1OutPDec							w_GprsParLst[129]		//.3 ��ˮѹ��
		// #define	w_1PIDCalcP							w_GprsParLst[130]		//.3 �趨ѹ��

		DispMod_Par(0x84, 0x04, 0x03, &w_2InPDec, 5, 0);
		DispMod_Par(0x94, 0x04, 0x03, &w_2OutPDec, 5, 0);
		DispMod_Par(0x8C, 0x04, 0x03, &w_2PIDCalcP, 5, 0); // ZCL 2018.5.19
		// DispMod_Par(0x9C, 0x04,0x01, &w_dspNowTemperature,5, 0);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_Dsp2ReadPar5(void) // ���� DSP������
{
	u8 i1 = 0, i2 = 0;
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "�¶ȱ�1 ");
			LCD12864_String(0x87, "��");
			// �ڶ���
			LCD12864_String(0x90, "�¶ȱ�2 ");
			LCD12864_String(0x97, "��");
			// ������
			LCD12864_String(0x88, "����汾");
			LCD12864_String(0x8F, "  ");
			// ������
			LCD12864_String(0x98, "                ");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(0);

		if (w_Pump2DispWenDu1DecValue >= 10000)
		{
			w_Pump2DispWenDu1DecValue = w_Pump2DispWenDu1DecValue - 10000;
			i1 = 10; // ZCL 2019.10.18 ���ϸ���
		}
		if (w_Pump2DispWenDu2DecValue >= 10000)
		{
			w_Pump2DispWenDu2DecValue = w_Pump2DispWenDu2DecValue - 10000;
			i2 = 10; // ZCL 2019.10.18 ���ϸ���
		}

		DispMod_Par(0x84, 0x04, 0x01, &w_Pump2DispWenDu1DecValue, 5 + i1, 0);
		DispMod_Par(0x94, 0x04, 0x01, &w_Pump2DispWenDu2DecValue, 5 + i2, 0);

		DispMod_Par(0x8C, 0x04, 0x02, &w_Lora2SoftVersion, 5, 0);
		// DispMod_Par(0x9C, 0x05,0x00, &w_HighV1Counter,5, 0);		//ZCL 2018.5.19

		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "��ˮ״̬        ");
			LCD12864_String(0x87, "  ");
			// �ڶ���
			LCD12864_String(0x90, "ͣ��ԭ��        ");
			LCD12864_String(0x97, "  ");
			// ������
			LCD12864_String(0x88, "��Ƶ����        ");
			LCD12864_String(0x8F, "kW");

			// ������
			LCD12864_String(0x98, "                ");
		}

		ModParNum(0);
		// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

		// #define	w_1EquipOperateStatus  	w_GprsParLst[131]		//.0 ��ˮ״̬
		// #define	w_1EquipAlarmStatus			w_GprsParLst[132]		//.0 ͣ��ԭ��
		// #define	w_1ScrEquipPower				w_GprsParLst[133]		//.0 �豸����

		DispMod_Par(0x84, 0x05, 0x00, &w_2EquipOperateStatus, 5, 0);
		DispMod_Par(0x94, 0x05, 0x00, &w_2EquipAlarmStatus, 5, 0);
		DispMod_Par(0x8C, 0x04, 0x01, &w_2ScrEquipPower, 5, 0); // ZCL 2018.5.19
		// DispMod_Par(0x9C, 0x04,0x01, &w_dspNowTemperature,5, 0);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_Dsp3ReadPar1(void) // ���� DSP������1
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "                ");
			// �ڶ���
			LCD12864_String(0x90, "                ");
			// ������
			LCD12864_String(0x88, "   3�Ÿ�Ƶ˫��  ");
			// ������
			LCD12864_String(0x98, "                ");
		}

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "                ");
			// �ڶ���
			LCD12864_String(0x90, "                ");
			// ������
			LCD12864_String(0x88, "   3�ű�Ƶ���  ");
			// ������
			LCD12864_String(0x98, "                ");
		}

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

// ZCL 2019.3.8 ������ʾ������
void Menu_Dsp3ReadPar2(void) // ���� DSP������
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		//	u32 nl_i;
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "AB~ ��ѹ");
			LCD12864_String(0x87, " V");

			// �ڶ���
			LCD12864_String(0x90, "A~����  ");
			LCD12864_String(0x97, " A");

			// ������
			LCD12864_String(0x88, "LoRa������");
			LCD12864_String(0x8F, "  ");

			if (B_LoRaErrWithModule3 == 0)
			{
				// ������
				LCD12864_String(0x98, "DNB ������");
				LCD12864_String(0x9F, "  ");
			}
			else
			{
				// ������
				LCD12864_String(0x98, "3#��LoRaͨѶ�쳣");
			}
		}

		ModParNum(0);
		DispMod_Par(0x84, 0x04, 0x01, &w_Lora3UabRms, 5, 0);
		// DispMod_Par(0x93, 0x06,0x01, &w_UaRms,5, 0);
		DispMod_Par(0x93, 0x06, 0x02, &w_Lora3IaRms, 5, 0);

		DispMod_Par(0x8D, 0x05, 0x00, &w_LoraCounter3, 5, 0); // YLS 2024.10.20

		if (B_LoRaErrWithModule3 == 0)
			DispMod_Par(0x9D, 0x05, 0x00, &w_Lora3Counter1_BPQ, 5, 0); // ZCL 2018.5.19

		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "��ǰƵ��");
			LCD12864_String(0x87, "Hz");

			// �ڶ���
			LCD12864_String(0x90, "�����ѹ");
			LCD12864_String(0x97, " V");

			// ������
			LCD12864_String(0x88, "�������");
			LCD12864_String(0x8F, " A");

			// ������
			LCD12864_String(0x98, "ģ���¶�");
			LCD12864_String(0x9F, "��");
		}

		ModParNum(0);
		// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

		DispMod_Par(0x84, 0x04, 0x01, &w_dsp3NowHz, 5, 0);
		DispMod_Par(0x94, 0x05, 0x00, &w_dsp3OutVoltage, 5, 0);

		if (Pw_ScrCurrentBits == 1) // ZCL 2018.8.29  ����1λС��
		{
			DispMod_Par(0x8C, 0x04, 0x01, &w_dsp3APhaseCurrent, 5, 0); // ZCL 2018.5.19
		}
		else if (Pw_ScrCurrentBits == 2) // ZCL 2018.8.29  ����2λС��
		{
			DispMod_Par(0x8C, 0x04, 0x02, &w_dsp3APhaseCurrent, 5, 0); // ZCL 2018.5.19
		}

		/* #ifdef CURRENT_OneXiaoShu		//ZCL 2018.1.26  ����1λС��
		#else
		#endif	 */
		// ZCL 2018.8.29 ��Pw_ScrCurrentBits ����

		DispMod_Par(0x9C, 0x04, 0x01, &w_dsp3NowTemperature, 5, 0);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_Dsp3ReadPar3(void) // ���� DSP������
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "220 ������");
			LCD12864_String(0x87, "  ");

			// �ڶ���
			LCD12864_String(0x90, "Զ������    ");
			LCD12864_String(0x97, "  ");

			// ������
			LCD12864_String(0x88, "������ͣ    ");
			LCD12864_String(0x8F, "  ");

			// ������
			LCD12864_String(0x98, "                ");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(1);

		DispMod_Par(0x85, 0x05, 0x00, &w_Lora3Counter2, 5, 0); // ZCL 2018.5.19

		DispMod_Par(0x95, 0x05, 0x00, &w_ZhuanFaData, 5, 0); // ZCL 2018.5.19

		DispMod_Par(0x8D, 0x05, 0x00, &w_Pump3RemoteStop, 5, F_ModPar1); // ZCL 2018.5.19

		if (F_ModeParLora == 0)
		{
			if (w_Pump3RemoteStop != w_SavePump3RemoteStop)
			{
				if (w_Pump3RemoteStop > 1)
				{
					w_Pump3RemoteStop = 0;
				}
				else
				{
					w_SavePump3RemoteStop = w_Pump3RemoteStop;

					w_ZhuanFaAdd = Addr_Lora3TouchRunStop;

					w_ZhuanFaData = w_Pump3RemoteStop;

					F_ModeParLora = 1;
				}
			}
		}

		// DispMod_Par(0x94, 0x04,0x01, &w_HighV1DispDianLi5,5, 0);
		// DispMod_Par(0x8C, 0x04,0x01, &w_HighV1DispDianLi6,5, 0);
		// DispMod_Par(0x9C, 0x05,0x00, &w_HighV1DispDianLi7,5, 0);

		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "������1 ");
			LCD12864_String(0x87, "  ");

			// �ڶ���
			LCD12864_String(0x90, "����汾");
			LCD12864_String(0x97, "  ");

			// ������
			LCD12864_String(0x88, "�������");
			LCD12864_String(0x8F, "  ");

			// ������
			LCD12864_String(0x98, "                ");
		}

		ModParNum(0);
		// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

		DispMod_Par(0x84, 0x05, 0x00, &w_dsp3SysErrorCode1, 5, 0);
		DispMod_Par(0x94, 0x04, 0x02, &w_dsp3SoftVersion, 5, 0);
		DispMod_Par(0x8C, 0x05, 0x00, &w_dsp3Counter3, 5, 0); // ZCL 2018.5.19
		// DispMod_Par(0x9C, 0x04,0x01, &w_dspNowTemperature,5, 0);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_Dsp3ReadPar4(void) // ���� DSP������
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "��ˮѹ��");
			LCD12864_String(0x87, "Mp");
			// �ڶ���
			LCD12864_String(0x90, "��ˮѹ��");
			LCD12864_String(0x97, "Mp");
			// ������
			LCD12864_String(0x88, "�趨ѹ��");
			LCD12864_String(0x8F, "Mp");
			// ������
			LCD12864_String(0x98, "��ƵƵ��");
			LCD12864_String(0x9F, "Hz");
		}

		ModParNum(0); // ָ��:�޸Ĳ���������
		// ZCL 2018.5.16 �޸Ĳ�������ʱ���ӿ��ٶȱ�֤�л�����ʾ������ʱ������Һ����ʾ�����ϣ���ʾ����
		// ��ʾ���޸Ĳ���
		DispMod_Par(0x84, 0x04, 0x03, &w_Lora3InP, 5, 0);
		DispMod_Par(0x94, 0x04, 0x03, &w_Lora3OutP, 5, 0);
		DispMod_Par(0x8C, 0x04, 0x03, &w_Lora3SetP, 5, 0);
		DispMod_Par(0x9C, 0x04, 0x01, &w_Lora3VvvfFreq, 5, 0);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "��ˮѹ��");
			LCD12864_String(0x87, "Mp");
			// �ڶ���
			LCD12864_String(0x90, "��ˮѹ��");
			LCD12864_String(0x97, "Mp");
			// ������
			LCD12864_String(0x88, "�趨ѹ��");
			LCD12864_String(0x8F, "Mp");

			// ������
			LCD12864_String(0x98, "                ");
		}

		ModParNum(0);
		// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

		// #define	w_1InPDec								w_GprsParLst[128]		//.3 ��ˮѹ��
		// #define	w_1OutPDec							w_GprsParLst[129]		//.3 ��ˮѹ��
		// #define	w_1PIDCalcP							w_GprsParLst[130]		//.3 �趨ѹ��

		DispMod_Par(0x84, 0x04, 0x03, &w_3InPDec, 5, 0);
		DispMod_Par(0x94, 0x04, 0x03, &w_3OutPDec, 5, 0);
		DispMod_Par(0x8C, 0x04, 0x03, &w_3PIDCalcP, 5, 0); // ZCL 2018.5.19
		// DispMod_Par(0x9C, 0x04,0x01, &w_dspNowTemperature,5, 0);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_Dsp3ReadPar5(void) // ���� DSP������
{
	u8 i1 = 0, i2 = 0;
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "�¶ȱ�1 ");
			LCD12864_String(0x87, "��");
			// �ڶ���
			LCD12864_String(0x90, "�¶ȱ�2 ");
			LCD12864_String(0x97, "��");
			// ������
			LCD12864_String(0x88, "����汾");
			LCD12864_String(0x8F, "  ");
			// ������
			LCD12864_String(0x98, "                ");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(0);

		if (w_Pump3DispWenDu1DecValue >= 10000)
		{
			w_Pump3DispWenDu1DecValue = w_Pump3DispWenDu1DecValue - 10000;
			i1 = 10; // ZCL 2019.10.18 ���ϸ���
		}
		if (w_Pump3DispWenDu2DecValue >= 10000)
		{
			w_Pump3DispWenDu2DecValue = w_Pump3DispWenDu2DecValue - 10000;
			i2 = 10; // ZCL 2019.10.18 ���ϸ���
		}

		DispMod_Par(0x84, 0x04, 0x01, &w_Pump3DispWenDu1DecValue, 5 + i1, 0);
		DispMod_Par(0x94, 0x04, 0x01, &w_Pump3DispWenDu2DecValue, 5 + i2, 0);

		DispMod_Par(0x8C, 0x04, 0x02, &w_Lora3SoftVersion, 5, 0);
		// DispMod_Par(0x9C, 0x05,0x00, &w_HighV1Counter,5, 0);		//ZCL 2018.5.19

		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "��ˮ״̬        ");
			LCD12864_String(0x87, "  ");
			// �ڶ���
			LCD12864_String(0x90, "ͣ��ԭ��        ");
			LCD12864_String(0x97, "  ");
			// ������
			LCD12864_String(0x88, "��Ƶ����        ");
			LCD12864_String(0x8F, "kW");

			// ������
			LCD12864_String(0x98, "                ");
		}

		ModParNum(0);
		// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

		// #define	w_1EquipOperateStatus  	w_GprsParLst[131]		//.0 ��ˮ״̬
		// #define	w_1EquipAlarmStatus			w_GprsParLst[132]		//.0 ͣ��ԭ��
		// #define	w_1ScrEquipPower				w_GprsParLst[133]		//.0 �豸����

		DispMod_Par(0x84, 0x05, 0x00, &w_3EquipOperateStatus, 5, 0);
		DispMod_Par(0x94, 0x05, 0x00, &w_3EquipAlarmStatus, 5, 0);
		DispMod_Par(0x8C, 0x04, 0x01, &w_3ScrEquipPower, 5, 0); // ZCL 2018.5.19
		// DispMod_Par(0x9C, 0x04,0x01, &w_dspNowTemperature,5, 0);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_Dsp4ReadPar1(void) // ���� DSP������1
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "                ");
			// �ڶ���
			LCD12864_String(0x90, "                ");
			// ������
			LCD12864_String(0x88, "   4�Ÿ�Ƶ˫��  ");
			// ������
			LCD12864_String(0x98, "                ");
		}

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "                ");
			// �ڶ���
			LCD12864_String(0x90, "                ");
			// ������
			LCD12864_String(0x88, "   4�ű�Ƶ���  ");
			// ������
			LCD12864_String(0x98, "                ");
		}

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

// ZCL 2019.3.8 ������ʾ������
void Menu_Dsp4ReadPar2(void) // ���� DSP������
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		//	u32 nl_i;
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "AB~ ��ѹ");
			LCD12864_String(0x87, " V");

			// �ڶ���
			LCD12864_String(0x90, "A~����  ");
			LCD12864_String(0x97, " A");

			// ������
			LCD12864_String(0x88, "LoRa������");
			LCD12864_String(0x8F, "  ");

			if (B_LoRaErrWithModule4 == 0)
			{
				// ������
				LCD12864_String(0x98, "DNB ������");
				LCD12864_String(0x9F, "  ");
			}
			else
			{
				// ������
				LCD12864_String(0x98, "4#��LoRaͨѶ�쳣");
			}
		}

		ModParNum(0);
		DispMod_Par(0x84, 0x04, 0x01, &w_Lora4UabRms, 5, 0);
		// DispMod_Par(0x93, 0x06,0x01, &w_UaRms,5, 0);
		DispMod_Par(0x93, 0x06, 0x02, &w_Lora4IaRms, 5, 0);

		DispMod_Par(0x8D, 0x05, 0x00, &w_LoraCounter4, 5, 0); // YLS 2024.10.20

		if (B_LoRaErrWithModule4 == 0)
			DispMod_Par(0x9D, 0x05, 0x00, &w_Lora4Counter1_BPQ, 5, 0); // ZCL 2018.5.19

		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "��ǰƵ��");
			LCD12864_String(0x87, "Hz");

			// �ڶ���
			LCD12864_String(0x90, "�����ѹ");
			LCD12864_String(0x97, " V");

			// ������
			LCD12864_String(0x88, "�������");
			LCD12864_String(0x8F, " A");

			// ������
			LCD12864_String(0x98, "ģ���¶�");
			LCD12864_String(0x9F, "��");
		}

		ModParNum(0);
		// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

		DispMod_Par(0x84, 0x04, 0x01, &w_dsp4NowHz, 5, 0);
		DispMod_Par(0x94, 0x05, 0x00, &w_dsp4OutVoltage, 5, 0);

		if (Pw_ScrCurrentBits == 1) // ZCL 2018.8.29  ����1λС��
		{
			DispMod_Par(0x8C, 0x04, 0x01, &w_dsp4APhaseCurrent, 5, 0); // ZCL 2018.5.19
		}
		else if (Pw_ScrCurrentBits == 2) // ZCL 2018.8.29  ����2λС��
		{
			DispMod_Par(0x8C, 0x04, 0x02, &w_dsp4APhaseCurrent, 5, 0); // ZCL 2018.5.19
		}

		/* #ifdef CURRENT_OneXiaoShu		//ZCL 2018.1.26  ����1λС��
		#else
		#endif	 */
		// ZCL 2018.8.29 ��Pw_ScrCurrentBits ����

		DispMod_Par(0x9C, 0x04, 0x01, &w_dsp4NowTemperature, 5, 0);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_Dsp4ReadPar3(void) // ���� DSP������
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "220 ������");
			LCD12864_String(0x87, "  ");

			// �ڶ���
			LCD12864_String(0x90, "Զ������    ");
			LCD12864_String(0x97, "  ");

			// ������
			LCD12864_String(0x88, "������ͣ    ");
			LCD12864_String(0x8F, "  ");

			// ������
			LCD12864_String(0x98, "                ");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(1);

		DispMod_Par(0x85, 0x05, 0x00, &w_Lora4Counter2, 5, 0); // ZCL 2018.5.19

		DispMod_Par(0x95, 0x05, 0x00, &w_ZhuanFaData, 5, 0); // ZCL 2018.5.19

		DispMod_Par(0x8D, 0x05, 0x00, &w_Pump4RemoteStop, 5, F_ModPar1); // ZCL 2018.5.19

		if (F_ModeParLora == 0)
		{
			if (w_Pump4RemoteStop != w_SavePump4RemoteStop)
			{
				if (w_Pump4RemoteStop > 1)
				{
					w_Pump4RemoteStop = 0;
				}
				else
				{
					w_SavePump4RemoteStop = w_Pump4RemoteStop;

					w_ZhuanFaAdd = Addr_Lora4TouchRunStop;

					w_ZhuanFaData = w_Pump4RemoteStop;

					F_ModeParLora = 1;
				}
			}
		}

		// DispMod_Par(0x94, 0x04,0x01, &w_HighV1DispDianLi5,5, 0);
		// DispMod_Par(0x8C, 0x04,0x01, &w_HighV1DispDianLi6,5, 0);
		// DispMod_Par(0x9C, 0x05,0x00, &w_HighV1DispDianLi7,5, 0);

		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "������1 ");
			LCD12864_String(0x87, "  ");

			// �ڶ���
			LCD12864_String(0x90, "����汾");
			LCD12864_String(0x97, "  ");

			// ������
			LCD12864_String(0x88, "�������");
			LCD12864_String(0x8F, "  ");

			// ������
			LCD12864_String(0x98, "                ");
		}

		ModParNum(0);
		// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

		DispMod_Par(0x84, 0x05, 0x00, &w_dsp4SysErrorCode1, 5, 0);
		DispMod_Par(0x94, 0x04, 0x02, &w_dsp4SoftVersion, 5, 0);
		DispMod_Par(0x8C, 0x05, 0x00, &w_dsp4Counter3, 5, 0); // ZCL 2018.5.19
		// DispMod_Par(0x9C, 0x04,0x01, &w_dspNowTemperature,5, 0);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_Dsp4ReadPar4(void) // ���� DSP������
{
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "��ˮѹ��");
			LCD12864_String(0x87, "Mp");
			// �ڶ���
			LCD12864_String(0x90, "��ˮѹ��");
			LCD12864_String(0x97, "Mp");
			// ������
			LCD12864_String(0x88, "�趨ѹ��");
			LCD12864_String(0x8F, "Mp");
			// ������
			LCD12864_String(0x98, "��ƵƵ��");
			LCD12864_String(0x9F, "Hz");
		}

		ModParNum(0); // ָ��:�޸Ĳ���������
		// ZCL 2018.5.16 �޸Ĳ�������ʱ���ӿ��ٶȱ�֤�л�����ʾ������ʱ������Һ����ʾ�����ϣ���ʾ����
		// ��ʾ���޸Ĳ���
		DispMod_Par(0x84, 0x04, 0x03, &w_Lora4InP, 5, 0);
		DispMod_Par(0x94, 0x04, 0x03, &w_Lora4OutP, 5, 0);
		DispMod_Par(0x8C, 0x04, 0x03, &w_Lora4SetP, 5, 0);
		DispMod_Par(0x9C, 0x04, 0x01, &w_Lora4VvvfFreq, 5, 0);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "��ˮѹ��");
			LCD12864_String(0x87, "Mp");
			// �ڶ���
			LCD12864_String(0x90, "��ˮѹ��");
			LCD12864_String(0x97, "Mp");
			// ������
			LCD12864_String(0x88, "�趨ѹ��");
			LCD12864_String(0x8F, "Mp");

			// ������
			LCD12864_String(0x98, "                ");
		}

		ModParNum(0);
		// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

		// #define	w_1InPDec								w_GprsParLst[128]		//.3 ��ˮѹ��
		// #define	w_1OutPDec							w_GprsParLst[129]		//.3 ��ˮѹ��
		// #define	w_1PIDCalcP							w_GprsParLst[130]		//.3 �趨ѹ��

		DispMod_Par(0x84, 0x04, 0x03, &w_4InPDec, 5, 0);
		DispMod_Par(0x94, 0x04, 0x03, &w_4OutPDec, 5, 0);
		DispMod_Par(0x8C, 0x04, 0x03, &w_4PIDCalcP, 5, 0); // ZCL 2018.5.19
		// DispMod_Par(0x9C, 0x04,0x01, &w_dspNowTemperature,5, 0);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_Dsp4ReadPar5(void) // ���� DSP������
{
	u8 i1 = 0, i2 = 0;
	if (Pw_EquipmentType == 0) // ˫����
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "�¶ȱ�1 ");
			LCD12864_String(0x87, "��");
			// �ڶ���
			LCD12864_String(0x90, "�¶ȱ�2 ");
			LCD12864_String(0x97, "��");
			// ������
			LCD12864_String(0x88, "����汾");
			LCD12864_String(0x8F, "  ");
			// ������
			LCD12864_String(0x98, "                ");
			LCD12864_String(0x9F, "  ");
		}

		ModParNum(0);

		if (w_Pump4DispWenDu1DecValue >= 10000)
		{
			w_Pump4DispWenDu1DecValue = w_Pump4DispWenDu1DecValue - 10000;
			i1 = 10; // ZCL 2019.10.18 ���ϸ���
		}
		if (w_Pump4DispWenDu2DecValue >= 10000)
		{
			w_Pump4DispWenDu2DecValue = w_Pump4DispWenDu2DecValue - 10000;
			i2 = 10; // ZCL 2019.10.18 ���ϸ���
		}

		DispMod_Par(0x84, 0x04, 0x01, &w_Pump4DispWenDu1DecValue, 5 + i1, 0);
		DispMod_Par(0x94, 0x04, 0x01, &w_Pump4DispWenDu2DecValue, 5 + i2, 0);

		DispMod_Par(0x8C, 0x04, 0x02, &w_Lora4SoftVersion, 5, 0);
		// DispMod_Par(0x9C, 0x05,0x00, &w_HighV1Counter,5, 0);		//ZCL 2018.5.19

		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
	else // ��Ƶ���
	{
		if (Lw_SavePageNo != Lw_PageNo)
		{
			Lw_SavePageNo = Lw_PageNo;
			EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

			// LCD12864_ClrText();          //����
			// ��һ��
			LCD12864_String(0x80, "��ˮ״̬        ");
			LCD12864_String(0x87, "  ");
			// �ڶ���
			LCD12864_String(0x90, "ͣ��ԭ��        ");
			LCD12864_String(0x97, "  ");
			// ������
			LCD12864_String(0x88, "��Ƶ����        ");
			LCD12864_String(0x8F, "kW");

			// ������
			LCD12864_String(0x98, "                ");
		}

		ModParNum(0);
		// w_dspAPhaseCurrent = w_dspAPhaseCurrent/10;		//ZCL 2016.2.20 ԭ��1λС����ȥ��1λ

		// #define	w_1EquipOperateStatus  	w_GprsParLst[131]		//.0 ��ˮ״̬
		// #define	w_1EquipAlarmStatus			w_GprsParLst[132]		//.0 ͣ��ԭ��
		// #define	w_1ScrEquipPower				w_GprsParLst[133]		//.0 �豸����

		DispMod_Par(0x84, 0x05, 0x00, &w_4EquipOperateStatus, 5, 0);
		DispMod_Par(0x94, 0x05, 0x00, &w_4EquipAlarmStatus, 5, 0);
		DispMod_Par(0x8C, 0x04, 0x01, &w_4ScrEquipPower, 5, 0); // ZCL 2018.5.19
		// DispMod_Par(0x9C, 0x04,0x01, &w_dspNowTemperature,5, 0);
		S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

		ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
	}
}

void Menu_Dsp5ReadPar1(void) // ���� DSP������1
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "                ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		// ������
		LCD12864_String(0x88, "   5�Ÿ�Ƶ˫��  ");
		// ������
		LCD12864_String(0x98, "                ");
	}

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

// ZCL 2019.3.8 ������ʾ������
void Menu_Dsp5ReadPar2(void) // ���� DSP������
{
	//	u32 nl_i;
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "AB~ ��ѹ");
		LCD12864_String(0x87, " V");

		// �ڶ���
		LCD12864_String(0x90, "A~����  ");
		LCD12864_String(0x97, " A");

		// ������
		LCD12864_String(0x88, "LoRa������");
		LCD12864_String(0x8F, "  ");

		if (B_LoRaErrWithModule4 == 0)
		{
			// ������
			LCD12864_String(0x98, "DNB ������");
			LCD12864_String(0x9F, "  ");
		}
		else
		{
			// ������
			LCD12864_String(0x98, "5#��LoRaͨѶ�쳣");
		}
	}

	ModParNum(0);
	DispMod_Par(0x84, 0x04, 0x01, &w_Lora5UabRms, 5, 0);
	// DispMod_Par(0x93, 0x06,0x01, &w_UaRms,5, 0);
	DispMod_Par(0x93, 0x06, 0x02, &w_Lora5IaRms, 5, 0);

	DispMod_Par(0x8D, 0x05, 0x00, &w_LoraCounter5, 5, 0);

	if (B_LoRaErrWithModule5 == 0)
		DispMod_Par(0x9D, 0x05, 0x00, &w_Lora5Counter1_BPQ, 5, 0);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_Dsp5ReadPar3(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "220 ������");
		LCD12864_String(0x87, "  ");

		// �ڶ���
		LCD12864_String(0x90, "Զ������    ");
		LCD12864_String(0x97, "  ");

		// ������
		LCD12864_String(0x88, "������ͣ    ");
		LCD12864_String(0x8F, "  ");

		// ������
		LCD12864_String(0x98, "                ");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(1);

	DispMod_Par(0x85, 0x05, 0x00, &w_Lora5Counter2, 5, 0); // ZCL 2018.5.19

	DispMod_Par(0x95, 0x05, 0x00, &w_ZhuanFaData, 5, 0); // ZCL 2018.5.19

	DispMod_Par(0x8D, 0x05, 0x00, &w_Pump5RemoteStop, 5, F_ModPar1); // ZCL 2018.5.19

	if (F_ModeParLora == 0)
	{
		if (w_Pump5RemoteStop != w_SavePump5RemoteStop)
		{
			if (w_Pump5RemoteStop > 1)
			{
				w_Pump5RemoteStop = 0;
			}
			else
			{
				w_SavePump5RemoteStop = w_Pump5RemoteStop;

				w_ZhuanFaAdd = Addr_Lora5TouchRunStop;

				w_ZhuanFaData = w_Pump5RemoteStop;

				F_ModeParLora = 1;
			}
		}
	}

	// DispMod_Par(0x94, 0x04,0x01, &w_HighV1DispDianLi5,5, 0);
	// DispMod_Par(0x8C, 0x04,0x01, &w_HighV1DispDianLi6,5, 0);
	// DispMod_Par(0x9C, 0x05,0x00, &w_HighV1DispDianLi7,5, 0);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_Dsp5ReadPar4(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��ˮѹ��");
		LCD12864_String(0x87, "Mp");
		// �ڶ���
		LCD12864_String(0x90, "��ˮѹ��");
		LCD12864_String(0x97, "Mp");
		// ������
		LCD12864_String(0x88, "�趨ѹ��");
		LCD12864_String(0x8F, "Mp");
		// ������
		LCD12864_String(0x98, "��ƵƵ��");
		LCD12864_String(0x9F, "Hz");
	}

	ModParNum(0); // ָ��:�޸Ĳ���������
	// ZCL 2018.5.16 �޸Ĳ�������ʱ���ӿ��ٶȱ�֤�л�����ʾ������ʱ������Һ����ʾ�����ϣ���ʾ����
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x04, 0x03, &w_Lora5InP, 5, 0);
	DispMod_Par(0x94, 0x04, 0x03, &w_Lora5OutP, 5, 0);
	DispMod_Par(0x8C, 0x04, 0x03, &w_Lora5SetP, 5, 0);
	DispMod_Par(0x9C, 0x04, 0x01, &w_Lora5VvvfFreq, 5, 0);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_Dsp5ReadPar5(void) // ���� DSP������
{
	u8 i1 = 0, i2 = 0;
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�¶ȱ�1 ");
		LCD12864_String(0x87, "��");
		// �ڶ���
		LCD12864_String(0x90, "�¶ȱ�2 ");
		LCD12864_String(0x97, "��");
		// ������
		LCD12864_String(0x88, "����汾");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(0);

	if (w_Pump5DispWenDu1DecValue >= 10000)
	{
		w_Pump5DispWenDu1DecValue = w_Pump5DispWenDu1DecValue - 10000;
		i1 = 10; // ZCL 2019.10.18 ���ϸ���
	}
	if (w_Pump5DispWenDu2DecValue >= 10000)
	{
		w_Pump5DispWenDu2DecValue = w_Pump5DispWenDu2DecValue - 10000;
		i2 = 10; // ZCL 2019.10.18 ���ϸ���
	}

	DispMod_Par(0x84, 0x04, 0x01, &w_Pump5DispWenDu1DecValue, 5 + i1, 0);
	DispMod_Par(0x94, 0x04, 0x01, &w_Pump5DispWenDu2DecValue, 5 + i2, 0);

	DispMod_Par(0x8C, 0x04, 0x02, &w_Lora5SoftVersion, 5, 0);
	// DispMod_Par(0x9C, 0x05,0x00, &w_HighV1Counter,5, 0);		//ZCL 2018.5.19

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_Dsp6ReadPar1(void) // ���� DSP������1
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "                ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		// ������
		LCD12864_String(0x88, "   6�Ÿ�Ƶ˫��  ");
		// ������
		LCD12864_String(0x98, "                ");
	}

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

// ZCL 2019.3.8 ������ʾ������
void Menu_Dsp6ReadPar2(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "���Ƶ��");
		LCD12864_String(0x87, "Hz");

		// �ڶ���
		LCD12864_String(0x90, "�ߵ�ѹ12");
		LCD12864_String(0x97, " V");

		// ������
		LCD12864_String(0x88, "�ߵ�ѹ23");
		LCD12864_String(0x8F, " V");

		// ������
		LCD12864_String(0x98, "�ߵ�ѹ31");
		LCD12864_String(0x9F, " V");
	}

	ModParNum(0);

	DispMod_Par(0x84, 0x04, 0x01, &w_HighV6DispDianLi8, 5, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_HighV6DispDianLi1, 5, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_HighV6DispDianLi2, 5, 0);
	DispMod_Par(0x9C, 0x05, 0x00, &w_HighV6DispDianLi3, 5, 0);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_Dsp6ReadPar3(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, " A�����");
		LCD12864_String(0x87, " A");

		// �ڶ���
		LCD12864_String(0x90, " B�����");
		LCD12864_String(0x97, " A");

		// ������
		LCD12864_String(0x88, " C�����");
		LCD12864_String(0x8F, " A");

		// ������
		LCD12864_String(0x98, "�й�����");
		LCD12864_String(0x9F, "kW");
	}

	ModParNum(0);

	DispMod_Par(0x84, 0x04, 0x01, &w_HighV6DispDianLi4, 5, 0);
	DispMod_Par(0x94, 0x04, 0x01, &w_HighV6DispDianLi5, 5, 0);
	DispMod_Par(0x8C, 0x04, 0x01, &w_HighV6DispDianLi6, 5, 0);
	DispMod_Par(0x9C, 0x05, 0x00, &w_HighV6DispDianLi7, 5, 0);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_Dsp6ReadPar4(void) // ���� DSP������
{
	u8 i1 = 0, i2 = 0, i3 = 0, i4 = 0;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�¶ȱ�1 ");
		LCD12864_String(0x87, "��");
		// �ڶ���
		LCD12864_String(0x90, "�¶ȱ�2 ");
		LCD12864_String(0x97, "��");
		// ������
		LCD12864_String(0x88, "�¶ȱ�3 ");
		LCD12864_String(0x8F, "��");

		// ������
		LCD12864_String(0x98, "�¶ȱ�4 ");
		LCD12864_String(0x9F, "��");
	}

	ModParNum(0);

	if (w_HighV6DispWenDu1DecValue >= 10000)
	{
		w_HighV6DispWenDu1DecValue = w_HighV6DispWenDu1DecValue - 10000;
		i1 = 10; // ZCL 2019.10.18 ���ϸ���
	}
	if (w_HighV6DispWenDu2DecValue >= 10000)
	{
		w_HighV6DispWenDu2DecValue = w_HighV6DispWenDu2DecValue - 10000;
		i2 = 10; // ZCL 2019.10.18 ���ϸ���
	}
	if (w_HighV6DispWenDu3DecValue >= 10000)
	{
		w_HighV6DispWenDu3DecValue = w_HighV6DispWenDu3DecValue - 10000;
		i3 = 10; // ZCL 2019.10.18 ���ϸ���
	}
	if (w_HighV6DispWenDu4DecValue >= 10000)
	{
		w_HighV6DispWenDu4DecValue = w_HighV6DispWenDu4DecValue - 10000;
		i4 = 10; // ZCL 2019.10.18 ���ϸ���
	}

	DispMod_Par(0x84, 0x04, 0x01, &w_HighV6DispWenDu1DecValue, 5 + i1, 0);
	DispMod_Par(0x94, 0x04, 0x01, &w_HighV6DispWenDu2DecValue, 5 + i2, 0);
	DispMod_Par(0x8C, 0x04, 0x01, &w_HighV6DispWenDu3DecValue, 5 + i3, 0);
	DispMod_Par(0x9C, 0x04, 0x01, &w_HighV6DispWenDu4DecValue, 5 + i4, 0);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_Dsp6ReadPar5(void) // ���� DSP������
{
	u8 i5 = 0, i6 = 0;
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�¶ȱ�5 ");
		LCD12864_String(0x87, "��");
		// �ڶ���
		LCD12864_String(0x90, "�¶ȱ�6 ");
		LCD12864_String(0x97, "��");
		// ������
		LCD12864_String(0x88, "����汾");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "�������");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(0);

	if (w_HighV6DispWenDu5DecValue >= 10000)
	{
		w_HighV6DispWenDu5DecValue = w_HighV6DispWenDu5DecValue - 10000;
		i5 = 10; // ZCL 2019.10.18 ���ϸ���
	}
	if (w_HighV6DispWenDu6DecValue >= 10000)
	{
		w_HighV6DispWenDu6DecValue = w_HighV6DispWenDu6DecValue - 10000;
		i6 = 10; // ZCL 2019.10.18 ���ϸ���
	}

	DispMod_Par(0x84, 0x04, 0x01, &w_HighV6DispWenDu5DecValue, 5 + i5, 0);
	DispMod_Par(0x94, 0x04, 0x01, &w_HighV6DispWenDu6DecValue, 5 + i6, 0);

	DispMod_Par(0x8C, 0x04, 0x02, &w_HighV6SoftVersion, 5, 0);
	DispMod_Par(0x9C, 0x05, 0x00, &w_HighV6Counter, 5, 0); // ZCL 2018.5.19

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_Dsp7ReadPar1(void) // ���� DSP������1
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "                ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		// ������
		LCD12864_String(0x88, "   7�Ÿ�Ƶ˫��  ");
		// ������
		LCD12864_String(0x98, "                ");
	}

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

// ZCL 2019.3.8 ������ʾ������
void Menu_Dsp7ReadPar2(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "���Ƶ��");
		LCD12864_String(0x87, "Hz");

		// �ڶ���
		LCD12864_String(0x90, "�ߵ�ѹ12");
		LCD12864_String(0x97, " V");

		// ������
		LCD12864_String(0x88, "�ߵ�ѹ23");
		LCD12864_String(0x8F, " V");

		// ������
		LCD12864_String(0x98, "�ߵ�ѹ31");
		LCD12864_String(0x9F, " V");
	}

	ModParNum(0);

	DispMod_Par(0x84, 0x04, 0x01, &w_HighV7DispDianLi8, 5, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_HighV7DispDianLi1, 5, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_HighV7DispDianLi2, 5, 0);
	DispMod_Par(0x9C, 0x05, 0x00, &w_HighV7DispDianLi3, 5, 0);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_Dsp7ReadPar3(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, " A�����");
		LCD12864_String(0x87, " A");

		// �ڶ���
		LCD12864_String(0x90, " B�����");
		LCD12864_String(0x97, " A");

		// ������
		LCD12864_String(0x88, " C�����");
		LCD12864_String(0x8F, " A");

		// ������
		LCD12864_String(0x98, "�й�����");
		LCD12864_String(0x9F, "kW");
	}

	ModParNum(0);

	DispMod_Par(0x84, 0x04, 0x01, &w_HighV7DispDianLi4, 5, 0);
	DispMod_Par(0x94, 0x04, 0x01, &w_HighV7DispDianLi5, 5, 0);
	DispMod_Par(0x8C, 0x04, 0x01, &w_HighV7DispDianLi6, 5, 0);
	DispMod_Par(0x9C, 0x05, 0x00, &w_HighV7DispDianLi7, 5, 0);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_Dsp7ReadPar4(void) // ���� DSP������
{
	u8 i1 = 0, i2 = 0, i3 = 0, i4 = 0;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�¶ȱ�1 ");
		LCD12864_String(0x87, "��");
		// �ڶ���
		LCD12864_String(0x90, "�¶ȱ�2 ");
		LCD12864_String(0x97, "��");
		// ������
		LCD12864_String(0x88, "�¶ȱ�3 ");
		LCD12864_String(0x8F, "��");

		// ������
		LCD12864_String(0x98, "�¶ȱ�4 ");
		LCD12864_String(0x9F, "��");
	}

	ModParNum(0);

	if (w_HighV7DispWenDu1DecValue >= 10000)
	{
		w_HighV7DispWenDu1DecValue = w_HighV7DispWenDu1DecValue - 10000;
		i1 = 10; // ZCL 2019.10.18 ���ϸ���
	}
	if (w_HighV7DispWenDu2DecValue >= 10000)
	{
		w_HighV7DispWenDu2DecValue = w_HighV7DispWenDu2DecValue - 10000;
		i2 = 10; // ZCL 2019.10.18 ���ϸ���
	}
	if (w_HighV7DispWenDu3DecValue >= 10000)
	{
		w_HighV7DispWenDu3DecValue = w_HighV7DispWenDu3DecValue - 10000;
		i3 = 10; // ZCL 2019.10.18 ���ϸ���
	}
	if (w_HighV7DispWenDu4DecValue >= 10000)
	{
		w_HighV7DispWenDu4DecValue = w_HighV7DispWenDu4DecValue - 10000;
		i4 = 10; // ZCL 2019.10.18 ���ϸ���
	}

	DispMod_Par(0x84, 0x04, 0x01, &w_HighV7DispWenDu1DecValue, 5 + i1, 0);
	DispMod_Par(0x94, 0x04, 0x01, &w_HighV7DispWenDu2DecValue, 5 + i2, 0);
	DispMod_Par(0x8C, 0x04, 0x01, &w_HighV7DispWenDu3DecValue, 5 + i3, 0);
	DispMod_Par(0x9C, 0x04, 0x01, &w_HighV7DispWenDu4DecValue, 5 + i4, 0);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_Dsp7ReadPar5(void) // ���� DSP������
{
	u8 i5 = 0, i6 = 0;
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�¶ȱ�5 ");
		LCD12864_String(0x87, "��");
		// �ڶ���
		LCD12864_String(0x90, "�¶ȱ�6 ");
		LCD12864_String(0x97, "��");
		// ������
		LCD12864_String(0x88, "����汾");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "�������");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(0);

	if (w_HighV7DispWenDu5DecValue >= 10000)
	{
		w_HighV7DispWenDu5DecValue = w_HighV7DispWenDu5DecValue - 10000;
		i5 = 10; // ZCL 2019.10.18 ���ϸ���
	}
	if (w_HighV7DispWenDu6DecValue >= 10000)
	{
		w_HighV7DispWenDu6DecValue = w_HighV7DispWenDu6DecValue - 10000;
		i6 = 10; // ZCL 2019.10.18 ���ϸ���
	}

	DispMod_Par(0x84, 0x04, 0x01, &w_HighV7DispWenDu5DecValue, 5 + i5, 0);
	DispMod_Par(0x94, 0x04, 0x01, &w_HighV7DispWenDu6DecValue, 5 + i6, 0);

	DispMod_Par(0x8C, 0x04, 0x02, &w_HighV7SoftVersion, 5, 0);
	DispMod_Par(0x9C, 0x05, 0x00, &w_HighV7Counter, 5, 0); // ZCL 2018.5.19

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_Dsp8ReadPar1(void) // ���� DSP������1
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "                ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		// ������
		LCD12864_String(0x88, "   8�Ÿ�Ƶ˫��  ");
		// ������
		LCD12864_String(0x98, "                ");
	}

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

// ZCL 2019.3.8 ������ʾ������
void Menu_Dsp8ReadPar2(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "���Ƶ��");
		LCD12864_String(0x87, "Hz");

		// �ڶ���
		LCD12864_String(0x90, "�ߵ�ѹ12");
		LCD12864_String(0x97, " V");

		// ������
		LCD12864_String(0x88, "�ߵ�ѹ23");
		LCD12864_String(0x8F, " V");

		// ������
		LCD12864_String(0x98, "�ߵ�ѹ31");
		LCD12864_String(0x9F, " V");
	}

	ModParNum(0);

	DispMod_Par(0x84, 0x04, 0x01, &w_HighV8DispDianLi8, 5, 0);
	DispMod_Par(0x94, 0x05, 0x00, &w_HighV8DispDianLi1, 5, 0);
	DispMod_Par(0x8C, 0x05, 0x00, &w_HighV8DispDianLi2, 5, 0);
	DispMod_Par(0x9C, 0x05, 0x00, &w_HighV8DispDianLi3, 5, 0);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_Dsp8ReadPar3(void) // ���� DSP������
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, " A�����");
		LCD12864_String(0x87, " A");

		// �ڶ���
		LCD12864_String(0x90, " B�����");
		LCD12864_String(0x97, " A");

		// ������
		LCD12864_String(0x88, " C�����");
		LCD12864_String(0x8F, " A");

		// ������
		LCD12864_String(0x98, "�й�����");
		LCD12864_String(0x9F, "kW");
	}

	ModParNum(0);

	DispMod_Par(0x84, 0x04, 0x01, &w_HighV8DispDianLi4, 5, 0);
	DispMod_Par(0x94, 0x04, 0x01, &w_HighV8DispDianLi5, 5, 0);
	DispMod_Par(0x8C, 0x04, 0x01, &w_HighV8DispDianLi6, 5, 0);
	DispMod_Par(0x9C, 0x05, 0x00, &w_HighV8DispDianLi7, 5, 0);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_Dsp8ReadPar4(void) // ���� DSP������
{
	u8 i1 = 0, i2 = 0, i3 = 0, i4 = 0;

	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�¶ȱ�1 ");
		LCD12864_String(0x87, "��");
		// �ڶ���
		LCD12864_String(0x90, "�¶ȱ�2 ");
		LCD12864_String(0x97, "��");
		// ������
		LCD12864_String(0x88, "�¶ȱ�3 ");
		LCD12864_String(0x8F, "��");

		// ������
		LCD12864_String(0x98, "�¶ȱ�4 ");
		LCD12864_String(0x9F, "��");
	}

	ModParNum(0);

	if (w_HighV8DispWenDu1DecValue >= 10000)
	{
		w_HighV8DispWenDu1DecValue = w_HighV8DispWenDu1DecValue - 10000;
		i1 = 10; // ZCL 2019.10.18 ���ϸ���
	}
	if (w_HighV8DispWenDu2DecValue >= 10000)
	{
		w_HighV8DispWenDu2DecValue = w_HighV8DispWenDu2DecValue - 10000;
		i2 = 10; // ZCL 2019.10.18 ���ϸ���
	}
	if (w_HighV8DispWenDu3DecValue >= 10000)
	{
		w_HighV8DispWenDu3DecValue = w_HighV8DispWenDu3DecValue - 10000;
		i3 = 10; // ZCL 2019.10.18 ���ϸ���
	}
	if (w_HighV8DispWenDu4DecValue >= 10000)
	{
		w_HighV8DispWenDu4DecValue = w_HighV8DispWenDu4DecValue - 10000;
		i4 = 10; // ZCL 2019.10.18 ���ϸ���
	}

	DispMod_Par(0x84, 0x04, 0x01, &w_HighV8DispWenDu1DecValue, 5 + i1, 0);
	DispMod_Par(0x94, 0x04, 0x01, &w_HighV8DispWenDu2DecValue, 5 + i2, 0);
	DispMod_Par(0x8C, 0x04, 0x01, &w_HighV8DispWenDu3DecValue, 5 + i3, 0);
	DispMod_Par(0x9C, 0x04, 0x01, &w_HighV8DispWenDu4DecValue, 5 + i4, 0);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_Dsp8ReadPar5(void) // ���� DSP������
{
	u8 i5 = 0, i6 = 0;
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�¶ȱ�5 ");
		LCD12864_String(0x87, "��");
		// �ڶ���
		LCD12864_String(0x90, "�¶ȱ�6 ");
		LCD12864_String(0x97, "��");
		// ������
		LCD12864_String(0x88, "����汾");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "�������");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(0);

	if (w_HighV8DispWenDu5DecValue >= 10000)
	{
		w_HighV8DispWenDu5DecValue = w_HighV8DispWenDu5DecValue - 10000;
		i5 = 10; // ZCL 2019.10.18 ���ϸ���
	}
	if (w_HighV8DispWenDu6DecValue >= 10000)
	{
		w_HighV8DispWenDu6DecValue = w_HighV8DispWenDu6DecValue - 10000;
		i6 = 10; // ZCL 2019.10.18 ���ϸ���
	}

	DispMod_Par(0x84, 0x04, 0x01, &w_HighV8DispWenDu5DecValue, 5 + i5, 0);
	DispMod_Par(0x94, 0x04, 0x01, &w_HighV8DispWenDu6DecValue, 5 + i6, 0);

	DispMod_Par(0x8C, 0x04, 0x02, &w_HighV8SoftVersion, 5, 0);
	DispMod_Par(0x9C, 0x05, 0x00, &w_HighV8Counter, 5, 0); // ZCL 2018.5.19

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_YW310Par1(void) // ���� DSP������1
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "                ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		// ������
		LCD12864_String(0x88, "   YW310����    ");
		// ������
		LCD12864_String(0x98, "                ");
	}

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

// ZCL 2019.10.19 YW310
void Menu_YW310Par2(void) // ���� YW310�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�޸�YW310 ");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "�ָ�YW310       ");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "�¸е�ַ        ");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "������          ");
		LCD12864_String(0x9F, "  ");
	}

	// ZCL 2019.
	ModParNum(3);
	DispMod_Par(0x85, 0x04, 0x00, &Pw_YW310ModPar, 8, F_ModPar1);	  // ZCL 2019.4.2  2019.10.19 ʹ�� 8���ͣ�
	DispMod_Par(0x95, 0x04, 0x00, &Pw_YW310ParInitial, 8, F_ModPar2); // ZCL 2019.4.2
	DispMod_Par(0x8D, 0x04, 0x00, &Pw_YW310EquipmentNo, 8, F_ModPar3);
	DispMod_Par(0x9D, 0x05, 0x00, &w_YW310Counter, 8, 0);
	//
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	// ReadBPDJPar2();			//ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����

	// ZCL 2019.10.19 �޸�

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_YW310ModPar, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_YW310ParInitial, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_YW310EquipmentNo, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&w_YW310Counter, 8), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		// if(B_TimeReadBPDJPar==2)		//ZCL 2019.3.19
		// {
		// ReadBPDJPar();
		// return;
		// }

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			// if(B_TimeReadBPDJPar==1)		//ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			// {
			// B_TimeReadBPDJPar=2;
			// return;										//ZCL 2019.3.19
			// }
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_YW310ModPar, 8), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_YW310ParInitial, 8), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_YW310EquipmentNo, 8), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_YW310Counter, 8), 1);
	}
}

void Menu_YW310Par3(void) // ���� YW310�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "ѹ���¶�ѡ��    ");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "ѹ��ϵ��        ");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "AI1 ��ֵ        ");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, " YW310�汾      ");
		LCD12864_String(0x9F, "  ");
	}

	// ZCL 2019.
	ModParNum(3);
	DispMod_Par(0x86, 0x03, 0x00, &Pw_YW310PressureWenDuSel, 8, F_ModPar1);	  // ZCL 2019.4.2  2019.10.19 ʹ�� 8���ͣ�
	DispMod_Par(0x94, 0x04, 0x03, &Pw_YW310PressureWenDuXiShu, 8, F_ModPar2); // ZCL 2019.4.2
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_YW310AI1SensorZero, 8, F_ModPar3);
	DispMod_Par(0x9D, 0x04, 0x02, &w_YW310SoftVer, 8, 0);
	//
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	// ReadBPDJPar2();			//ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����

	// ZCL 2019.10.19 �޸�

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_YW310PressureWenDuSel, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_YW310PressureWenDuXiShu, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_YW310AI1SensorZero, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&w_YW310SoftVer, 8), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		// if(B_TimeReadBPDJPar==2)		//ZCL 2019.3.19
		// {
		// ReadBPDJPar();
		// return;
		// }

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			// if(B_TimeReadBPDJPar==1)		//ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			// {
			// B_TimeReadBPDJPar=2;
			// return;										//ZCL 2019.3.19
			// }
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_YW310PressureWenDuSel, 8), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_YW310PressureWenDuXiShu, 8), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_YW310AI1SensorZero, 8), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_YW310SoftVer, 8), 1);
	}
}

void Menu_YW310Par4(void) // ���� YW310�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "��������        ");
		LCD12864_String(0x87, "��");
		// �ڶ���
		LCD12864_String(0x90, "��������        ");
		LCD12864_String(0x97, "��");
		// ������
		LCD12864_String(0x88, "���AIֵ        ");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
		LCD12864_String(0x9F, "  ");
	}

	// ZCL 2019.
	ModParNum(3);
	DispMod_Par(0x84, 0x05, 0x00, &Pw_YW310ZWenDecMax, 8, F_ModPar1); // ZCL 2019.4.2  2019.10.19 ʹ�� 8���ͣ�
	DispMod_Par(0x94, 0x05, 0x00, &Pw_YW310FWenDecMax, 8, F_ModPar2); // ZCL 2019.4.2
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_YW310MaxAIHex, 8, F_ModPar3);
	// DispMod_Par(0x9C, 0x04,0x02, &w_YW310SoftVer,8, 0);
	//
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	// ReadBPDJPar2();			//ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����

	// ZCL 2019.10.19 �޸�

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_YW310ZWenDecMax, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_YW310FWenDecMax, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_YW310MaxAIHex, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&w_YW310SoftVer, 8), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		// if(B_TimeReadBPDJPar==2)		//ZCL 2019.3.19
		// {
		// ReadBPDJPar();
		// return;
		// }

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			// if(B_TimeReadBPDJPar==1)		//ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			// {
			// B_TimeReadBPDJPar=2;
			// return;										//ZCL 2019.3.19
			// }
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_YW310ZWenDecMax, 8), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_YW310FWenDecMax, 8), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_YW310MaxAIHex, 8), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&w_YW310SoftVer, 8), 1);
	}
}

void Menu_SZM220Par1(void) // ���� DSP������1
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "                ");
		// �ڶ���
		LCD12864_String(0x90, "                ");
		// ������
		LCD12864_String(0x88, "   SZM220 ����  ");
		// ������
		LCD12864_String(0x98, "                ");
	}

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_SZM220Par2(void) // ���� SZM220�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�޸�ZM220");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "�ָ�ZM220");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "���ӻ��趨      ");
		LCD12864_String(0x8D, "  ");
		// ������
		LCD12864_String(0x98, "220-LORA��ַ    ");
		LCD12864_String(0x9F, "  ");
	}

	// ZCL 2019.
	ModParNum(4);
	DispMod_Par(0x85, 0x04, 0x00, &Pw_ZM220ModPar, 8, F_ModPar1);	  // ZCL 2019.4.2  2019.10.19 ʹ�� 8���ͣ�
	DispMod_Par(0x95, 0x04, 0x00, &Pw_ZM220ParInitial, 8, F_ModPar2); // ZCL 2019.4.2
	DispMod_Par(0x8D, 0x03, 0x00, &Pw_ZM220LoRaMasterSlaveSel, 8, F_ModPar3);
	DispMod_Par(0x9E, 0x02, 0x00, &Pw_ZM220LoRaEquipmentNo, 8, F_ModPar4);
	//
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	// ReadBPDJPar2();			//ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����

	// ZCL 2019.10.19 �޸�

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_ZM220ModPar, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_ZM220ParInitial, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_ZM220LoRaMasterSlaveSel, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_ZM220LoRaEquipmentNo, 8), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		// if(B_TimeReadBPDJPar==2)		//ZCL 2019.3.19
		// {
		// ReadBPDJPar();
		// return;
		// }

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			// if(B_TimeReadBPDJPar==1)		//ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			// {
			// B_TimeReadBPDJPar=2;
			// return;										//ZCL 2019.3.19
			// }
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_ZM220ModPar, 8), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_ZM220ParInitial, 8), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_ZM220LoRaMasterSlaveSel, 8), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_ZM220LoRaEquipmentNo, 8), 1);
	}
}

void Menu_SZM220Par3(void) // ���� SZM220�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "LoRaƵ��");
		LCD12864_String(0x87, " M");
		// �ڶ���
		LCD12864_String(0x90, "LoRa����");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "LoRa����");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "��������");
		LCD12864_String(0x9F, "  ");
	}

	// ZCL 2019.10.19
	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_ZM220LoRaSetFreq, 8, F_ModPar1);
	DispMod_Par(0x94, 0x05, 0x00, &Pw_ZM220LoRaSetPower, 8, F_ModPar2);
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_ZM220LoRaSetSignalBW, 8, F_ModPar3); // ����4����DispMod_Par()��û���� ZCL 2018.8.3
	DispMod_Par(0x9C, 0x05, 0x00, &Pw_ZM220LoRaSetSpreadingFactor, 8, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	// ReadBPDJPar2();			//ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����

	// ZCL 2019.10.19 �޸�

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_ZM220LoRaSetFreq, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_ZM220LoRaSetPower, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_ZM220LoRaSetSignalBW, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_ZM220LoRaSetSpreadingFactor, 8), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		// if(B_TimeReadBPDJPar==2)		//ZCL 2019.3.19
		// {
		// ReadBPDJPar();
		// return;
		// }

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			// if(B_TimeReadBPDJPar==1)		//ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			// {
			// B_TimeReadBPDJPar=2;
			// return;										//ZCL 2019.3.19
			// }
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_ZM220LoRaSetFreq, 8), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_ZM220LoRaSetPower, 8), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_ZM220LoRaSetSignalBW, 8), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_ZM220LoRaSetSpreadingFactor, 8), 1);
	}
}

void Menu_SZM220Par4(void) // ���� SZM220�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "�������");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "CrcOn   ");
		LCD12864_String(0x97, "  "); // ZCL 2018.7.30 ԭ��0x97���ã�������Ϊ(01)ռ2���֣�Ӧ����0x96
		// ������
		LCD12864_String(0x88, "��ʽ��ͷ");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "���յ���");
		LCD12864_String(0x9F, "  "); // ZCL 2018.7.30 ԭ��0x97���ã�������Ϊ(01)ռ2���֣�Ӧ����0x96
	}

	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_ZM220LoRaSetErrorCoding, 8, F_ModPar1);
	DispMod_Par(0x94, 0x05, 0x00, &Pw_ZM220LoRaSetCrcOn, 8, F_ModPar2);
	DispMod_Par(0x8C, 0x05, 0x00, &Pw_ZM220LoRaSetImplicitHeaderOn, 8, F_ModPar3); // ����4����DispMod_Par()��û���� ZCL 2018.8.3
	DispMod_Par(0x9C, 0x05, 0x00, &Pw_ZM220LoRaSetRxSingleOn, 8, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	// ReadBPDJPar2();			//ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����

	// ZCL 2019.10.19 �޸�

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_ZM220LoRaSetErrorCoding, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_ZM220LoRaSetCrcOn, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_ZM220LoRaSetImplicitHeaderOn, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_ZM220LoRaSetRxSingleOn, 8), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		// if(B_TimeReadBPDJPar==2)		//ZCL 2019.3.19
		// {
		// ReadBPDJPar();
		// return;
		// }

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			// if(B_TimeReadBPDJPar==1)		//ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			// {
			// B_TimeReadBPDJPar=2;
			// return;										//ZCL 2019.3.19
			// }
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_ZM220LoRaSetErrorCoding, 8), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_ZM220LoRaSetCrcOn, 8), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_ZM220LoRaSetImplicitHeaderOn, 8), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_ZM220LoRaSetRxSingleOn, 8), 1);
	}
}

void Menu_SZM220Par5(void) // ���� SZM220�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "FreqHopOn ");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "HopPeriod ");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "PayLoadLen");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "LoRaλ����      ");
		LCD12864_String(0x9F, "  ");
	}

	ModParNum(4);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x85, 0x03, 0x00, &Pw_ZM220LoRaSetFreqHopOn, 8, F_ModPar1);
	DispMod_Par(0x95, 0x03, 0x00, &Pw_ZM220LoRaSetHopPeriod, 8, F_ModPar2);
	DispMod_Par(0x8D, 0x03, 0x00, &Pw_ZM220LoRaSetPayLoadLength, 8, F_ModPar3); // ����4����DispMod_Par()��û���� ZCL 2018.8.3
	DispMod_Par(0x9D, 0x03, 0x00, &Pw_ZM220LoRaSet, 8, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	// ReadBPDJPar2();			//ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����

	// ZCL 2019.10.19 �޸�

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_ZM220LoRaSetFreqHopOn, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_ZM220LoRaSetHopPeriod, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&Pw_ZM220LoRaSetPayLoadLength, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 4)
			Write_Stm32Data(Address(&Pw_ZM220LoRaSet, 8), Lw_SaveSetValue);
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		// if(B_TimeReadBPDJPar==2)		//ZCL 2019.3.19
		// {
		// ReadBPDJPar();
		// return;
		// }

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 4)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			// if(B_TimeReadBPDJPar==1)		//ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			// {
			// B_TimeReadBPDJPar=2;
			// return;										//ZCL 2019.3.19
			// }
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_ZM220LoRaSetFreqHopOn, 8), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_ZM220LoRaSetHopPeriod, 8), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&Pw_ZM220LoRaSetPayLoadLength, 8), 1);
		else if (S_RdWrNo == 4)
			Read_Stm32Data(Address(&Pw_ZM220LoRaSet, 8), 1);
	}
}

void Menu_SZM220Par6(void) // ���� SZM220�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "���ͳ�ʱ      ");
		LCD12864_String(0x87, "ms");
		// �ڶ���
		LCD12864_String(0x90, "���ճ�ʱ      ");
		LCD12864_String(0x97, "ms");
		// ������
		LCD12864_String(0x88, "��������      ");
		LCD12864_String(0x8F, "kb");
		// ������
		LCD12864_String(0x98, "SZM220--LORA����");
		// LCD12864_String(0x9F, "  ");
	}

	ModParNum(2);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x84, 0x05, 0x00, &Pw_ZM220LoRaSetTxPacketTimeOut, 8, F_ModPar1);
	DispMod_Par(0x94, 0x05, 0x00, &Pw_ZM220LoRaSetRxPacketTimeOut, 8, F_ModPar2);
	DispMod_Par(0x8C, 0x04, 0x02, &w_ZM220LoRaDateRate, 8, 0); // ����4��5����DispMod_Par()��û���� ZCL 2018.8.3
	// DispMod_Par(0x9D, 0x03,0x00, &w_LoRaDateRate,4, F_ModPar4);
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	// ReadBPDJPar2();			//ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����

	// ZCL 2019.10.19 �޸�

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&Pw_ZM220LoRaSetTxPacketTimeOut, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&Pw_ZM220LoRaSetRxPacketTimeOut, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 3)
			Write_Stm32Data(Address(&w_ZM220LoRaDateRate, 8), Lw_SaveSetValue);
		// else if(S_RdWrNo==4)
		// Write_Stm32Data( Address(&Pw_ZM220LoRaSetRxSingleOn,8), Lw_SaveSetValue );
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		// if(B_TimeReadBPDJPar==2)		//ZCL 2019.3.19
		// {
		// ReadBPDJPar();
		// return;
		// }

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 3)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			// if(B_TimeReadBPDJPar==1)		//ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			// {
			// B_TimeReadBPDJPar=2;
			// return;										//ZCL 2019.3.19
			// }
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&Pw_ZM220LoRaSetTxPacketTimeOut, 8), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&Pw_ZM220LoRaSetRxPacketTimeOut, 8), 1);
		else if (S_RdWrNo == 3)
			Read_Stm32Data(Address(&w_ZM220LoRaDateRate, 8), 1);
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_ZM220LoRaSetRxSingleOn,8), 1 );
	}
}

void Menu_SZM220Par7(void) // ���� YW310�趨����
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "SZM220����");
		// LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "SZM220�汾      ");
		LCD12864_String(0x97, "  ");
		// ������
		LCD12864_String(0x88, "                ");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "                ");
		LCD12864_String(0x9F, "  ");
	}

	// ZCL 2019.
	ModParNum(0);
	DispMod_Par(0x85, 0x05, 0x00, &w_ZM220Counter, 8, 0); // ZCL 2019.4.2  2019.10.19 ʹ�� 8���ͣ�
	DispMod_Par(0x95, 0x04, 0x02, &w_ZM220SoftVer, 8, 0); // ZCL 2019.4.2
	// DispMod_Par(0x8D, 0x04,0x02, &w_YW310SoftVer,8, 0);
	// DispMod_Par(0x9D, 0x03,0x00, &Pw_YW310EquipmentNo,8, F_ModPar4);
	//
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	// д���� 2015.9.1

	// ������
	// ReadBPDJPar2();			//ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����

	// ZCL 2019.10.19 �޸�

	// д���� 2015.9.1
	if (B_ModPar == 1)
	{
		B_ModPar = 2;
		S_Com1Send = 0;
		C_Com1Send = 0;
		S_Com1SendNoNum = 0;
	}
	if (B_ModPar == 2)
	{
		if (S_RdWrNo == 1)
			Write_Stm32Data(Address(&w_ZM220Counter, 8), Lw_SaveSetValue);
		else if (S_RdWrNo == 2)
			Write_Stm32Data(Address(&w_ZM220SoftVer, 8), Lw_SaveSetValue);
		// else if(S_RdWrNo==3)
		// Write_Stm32Data( Address(&w_YW310SoftVer,8), Lw_SaveSetValue );
		// else if(S_RdWrNo==4)
		// Write_Stm32Data( Address(&Pw_YW310EquipmentNo,8), Lw_SaveSetValue );
	}

	// ������
	else if (B_ModPar == 0) // ZCL 2018.5.15 +if(B_ModPar==0)  û�н�����иĲ�������������
	{
		// if(B_TimeReadBPDJPar==2)		//ZCL 2019.3.19
		// {
		// ReadBPDJPar();
		// return;
		// }

		// ZCL 2015.9.1 ������ͨѶ����
		if (S_RdWrNo > 2)
			S_RdWrNo = 0;
		if (S_RdWrNo == 0)
		{
			S_RdWrNo = 1;
			S_Com1Send = 0;
			C_Com1Send = 0;
			S_Com1SendNoNum = 0;
			// if(B_TimeReadBPDJPar==1)		//ZCL 2019.3.19 =1 Ҫ��ʱ������Ƶ�������������Ǳ����棩
			// {
			// B_TimeReadBPDJPar=2;
			// return;										//ZCL 2019.3.19
			// }
		}
		// ��˳���ȡ����
		if (S_RdWrNo == 1)
			Read_Stm32Data(Address(&w_ZM220Counter, 8), 1);
		else if (S_RdWrNo == 2)
			Read_Stm32Data(Address(&w_ZM220SoftVer, 8), 1);
		// else if(S_RdWrNo==3)
		// Read_Stm32Data( Address(&w_YW310SoftVer,8), 1 );
		// else if(S_RdWrNo==4)
		// Read_Stm32Data(Address(&Pw_YW310EquipmentNo,8), 1 );
	}
}

void Menu_GpsReadPar1(void) // ���� ������  			//ZCL 2016.10.6 ���Ӵ˻���
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		// LCD12864_ClrText();          //����
		// ��һ��
		LCD12864_String(0x80, "γ��:           ");
		LCD12864_String(0x87, "  ");
		// �ڶ���
		LCD12864_String(0x90, "    D   .       ");
		LCD12864_String(0x97, "F ");
		// ������
		LCD12864_String(0x88, "����:           ");
		LCD12864_String(0x8F, "  ");
		// ������
		LCD12864_String(0x98, "     D  .       ");
		LCD12864_String(0x9F, "F "); // ZCL 2018.9.21
	}

	ModParNum(0);
	// ��ʾ���޸Ĳ���
	DispMod_Par(0x83, 0x05, 0x02, &w_ScrGpsLatAllDu1, 5, 0); // ZCL 2019.9.24    ,4,
	DispMod_Par(0x86, 0x04, 0x00, &w_ScrGpsLatAllDu2, 5, 0);

	DispMod_Par(0x91, 0x02, 0x00, &w_ScrGpsLatDu, 5, 0);
	DispMod_Par(0x93, 0x02, 0x00, &w_ScrGpsLatFen1, 5, 0);
	DispMod_Par(0x95, 0x04, 0x00, &w_ScrGpsLatFen2, 5, 0);

	DispMod_Par(0x8B, 0x05, 0x02, &w_ScrGpsLonAllDu1, 5, 0);
	DispMod_Par(0x8E, 0x04, 0x00, &w_ScrGpsLonAllDu2, 5, 0);

	DispMod_Par(0x99, 0x03, 0x00, &w_ScrGpsLonDu, 5, 0);
	DispMod_Par(0x9B, 0x02, 0x00, &w_ScrGpsLonFen1, 5, 0);
	DispMod_Par(0x9D, 0x04, 0x00, &w_ScrGpsLonFen2, 5, 0);

	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	/* #define	w_ScrGpsLatDu			w_ScrParLst[18]		//.0 γ�� �ȣ�	ddmm.mmmm֮dd
	#define	w_ScrGpsLatFen1		w_ScrParLst[19]		//.0 γ�� �֣�		ddmm.mmmm֮mm
	#define	w_ScrGpsLatFen2		w_ScrParLst[20]		//.0 γ�� �֣�		ddmm.mmmm֮mmmm
	#define	w_ScrGpsLonDu			w_ScrParLst[21]		//.0 ���� �ȣ�	dddmm.mmmm֮ddd
	#define	w_ScrGpsLonFen1		w_ScrParLst[22]		//.0 ���� �֣�		dddmm.mmmm֮mm
	#define	w_ScrGpsLonFen2		w_ScrParLst[23]		//.0 ���� �֣�		dddmm.mmmm֮mmmm

	#define	w_ScrGpsLatAllDu1		w_ScrParLst[24]		//.0 γ�� ��1�� 35.44
	#define	w_ScrGpsLatAllDu2		w_ScrParLst[25]		//.0 γ�� ��2��	00.00****
	#define	w_ScrGpsLonAllDu1		w_ScrParLst[26]		//.0 ���� ��1��
	#define	w_ScrGpsLonAllDu2		w_ScrParLst[27]		//.0 ���� ��2�� */

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

void Menu_END(void) // ���� DSP�趨����1
{
	if (Lw_SavePageNo != Lw_PageNo)
	{
		Lw_SavePageNo = Lw_PageNo;
		EnterMenu_InitPar(); // ����˵���ʼ�������� ZCL 2018.5.15

		LCD12864_ClrText(); // ����
		// ��һ��
		LCD12864_String(0x80, "�ൺ�������ܶ���");
		// �ڶ���
		LCD12864_String(0x90, "    ��ӭ����    ");
		// ������
		LCD12864_String(0x88, "                ");
		// ������
		LCD12864_String(0x98, "             End");
	}

	// ZCL 2017.3.21 �����������һ�Σ�������û�в����Ļ��水OK��������ֽ�������޸�״̬����������

	ModParNum(0);
	// ��ʾ���޸Ĳ���
	S_DisplayPar = 0; // ��ʾ��������������S_DisplayPar���ٴ���ʱ

	ReadBPDJPar2(); // ZCL 2019.4.3 ���޲������棬Һ����Ҳ���Զ�DSP,ARM�����
}

// ZCL 2019.3.7 ����Ƶ������������
/* void ReadBPDJPar(void)
{
	if(S_RdWrNo>7)
	{
		S_RdWrNo=0;
		B_TimeReadBPDJPar=0;
	}

	//��˳���ȡ����	//����汾��A���������ʱƵ�ʣ�Ŀ��Ƶ��
	if(S_RdWrNo==1)
		Read_Stm32Data( Address(&w_dspSoftVersion,3), 1 );
	else if(S_RdWrNo==2)
		Read_Stm32Data( Address(&w_dspAPhaseCurrent,3), 1 );
	else if(S_RdWrNo==3)
		Read_Stm32Data( Address(&w_dspTmpTargetHz,3), 6 );		//ZCL 2019.2.18 ���ݵ��ԣ������󣬸ĳɶ��
	else if(S_RdWrNo==4)
		Read_Stm32Data( Address(&w_dspCounter3,3), 5 );		//.0 DCM220 DCM100: dsp����������		//w_dspTargetHz
	else if(S_RdWrNo==5)
		Read_Stm32Data( Address(&w_dspModPar,3), 7 );		//
	else if(S_RdWrNo==6)
		Read_Stm32Data( Address(&w_dspWriteYear,3), 8 );		//


	else if(S_RdWrNo==7)
		Read_Stm32Data( Address(&Pw_dspAddSpeedTime,2), 10 );		// */
// ZCL ���� 2019.3.7 Pw_dsp��д�����Ķ�����������Ķ�OK
/* 	if(S_RdWrNo==1)
		Read_Stm32Data( Address(&Pw_dspAddSpeedTime,2), 6 );
	else if(S_RdWrNo==2)
		Read_Stm32Data( Address(&Pw_dspDeadSupplement,2), 1 );
	else if(S_RdWrNo==3)
		Read_Stm32Data( Address(&Pw_dspOCPCurrent,2), 1 );
	else if(S_RdWrNo==4)
		Read_Stm32Data(Address(&Pw_dspOVPVoltage,2), 1 );		 */
//}

// ZCL 2019.3.7 ����Ƶ������������
//  ��ע���ڵ�ǰ���棬��̨��ȡ���������LORAԶ���ã�
void ReadBPDJPar(void)
{
	if (S_RdWrNo > 12) // ZCL 2019.9.11 7
	{
		S_RdWrNo = 0;
		B_TimeReadBPDJPar = 0;
	}

	// ��˳���ȡ����	//����汾��A���������ʱƵ�ʣ�Ŀ��Ƶ��
	if (S_RdWrNo == 1)
		Read_Stm32Data(Address(&w_dspSoftVersion, 3), 1);
	else if (S_RdWrNo == 2)
		Read_Stm32Data(Address(&w_dspNowHz, 3), 1);
	else if (S_RdWrNo == 3)
		Read_Stm32Data(Address(&w_dspAPhaseCurrent, 3), 1);
	else if (S_RdWrNo == 4)
		Read_Stm32Data(Address(&w_dspOutVoltage, 3), 1); // ZCL 2019.2.18 ���ݵ��ԣ������󣬸ĳɶ��
	else if (S_RdWrNo == 5)
		Read_Stm32Data(Address(&w_dspNowTemperature, 3), 1);
	else if (S_RdWrNo == 6)
		Read_Stm32Data(Address(&w_dspCounter3, 3), 1); // ZCL 2019.2.18 ���ݵ��ԣ������󣬸ĳɶ��
	else if (S_RdWrNo == 7)
		Read_Stm32Data(Address(&w_dspSysErrorCode1, 3), 1); // ZCL 2019.2.18 ���ݵ��ԣ������󣬸ĳɶ��

	// ZCL 2019.9.11 ���ӣ���ˮѹ������ˮѹ�����趨ѹ��
	else if (S_RdWrNo == 8)
		Read_Stm32Data(Address(&w_InPDec, 1), 1);
	else if (S_RdWrNo == 9)
		Read_Stm32Data(Address(&w_OutPDec, 1), 1);
	else if (S_RdWrNo == 10)
		Read_Stm32Data(Address(&w_PIDCalcP, 1), 1);
	else if (S_RdWrNo == 11)
		Read_Stm32Data(Address(&w_EquipOperateStatus, 1), 1);
	else if (S_RdWrNo == 12)
		Read_Stm32Data(Address(&w_EquipAlarmStatus, 1), 1);
	else if (S_RdWrNo == 13)
		Read_Stm32Data(Address(&Pw_ScrEquipPower, 4), 1); // SCR��������Ҫ����

	/* 	else if(S_RdWrNo==4)
			Read_Stm32Data( Address(&w_dspCounter3,3), 5 );		//.0 DCM220 DCM100: dsp����������		//w_dspTargetHz
		else if(S_RdWrNo==5)
			Read_Stm32Data( Address(&w_dspModPar,3), 7 );		//
		else if(S_RdWrNo==6)
			Read_Stm32Data( Address(&w_dspWriteYear,3), 8 );		//


		else if(S_RdWrNo==7)
			Read_Stm32Data( Address(&Pw_dspAddSpeedTime,2), 10 );		// */
	// ZCL ���� 2019.3.7 Pw_dsp��д�����Ķ�����������Ķ�OK
	/* 	if(S_RdWrNo==1)
			Read_Stm32Data( Address(&Pw_dspAddSpeedTime,2), 6 );
		else if(S_RdWrNo==2)
			Read_Stm32Data( Address(&Pw_dspDeadSupplement,2), 1 );
		else if(S_RdWrNo==3)
			Read_Stm32Data( Address(&Pw_dspOCPCurrent,2), 1 );
		else if(S_RdWrNo==4)
			Read_Stm32Data(Address(&Pw_dspOVPVoltage,2), 1 );		 */
}

// ZCL 2019.3.7 ����Ƶ������������ ReadBPDJPar���ڵ�ǰ���棩
// ZCL 2019.4.3 ��ע�����޲������棬Һ����Ҳ���Զ�DSP,ARM�����	 ReadBPDJPar2  LORAԶ���ã�
void ReadBPDJPar2(void)
{
	// ZCL 2019.4.6
	if (((Pw_ScrMenuMode == 0 || Pw_ScrMenuMode == 1) && B_ModPar == 0) // DCM100
		|| ((Pw_ScrMenuMode == 2) && B_PressedStopRunKey == 0))			// DCM220
	{
		if (S_RdWrNo == 0) // ZCL 2019.4.4 ��� ReadBPDJPar2 �²���ȡ��������
			S_RdWrNo = 1;

		// if(B_TimeReadBPDJPar==2)		//ZCL 2019.3.19
		//{
		if (Pw_LoRaMasterSlaveSel == 0) // ZCL 2019.4.3 �ӻ������ӵ�DSP,ARM����
			ReadBPDJPar();
		//}
	}
}

void Menu_Delay(void) // ������ʱ
{
	// ZCL 2018.5.16 ��ʱ�ӵ�����
	if (S_DisplayPar == 0 && T_LcdDisplay != SClk10Ms)
	{
		T_LcdDisplay = SClk10Ms;
		C_LcdDisplay++;
		S_DisplayPar = 1;
		if (C_LcdDisplay > 15)
		{
			C_LcdDisplay = 0;
			S_DisplayPar = 2;
		}
	}
}

void FaultDisplay(u16 FaultValue, u8 Addr) // ������ʾ		����ֵ����ʾ��ַ
{
	if (S_DisplayPar == 2) // ZCL 2018.5.19
	{
		switch (FaultValue)
		{
		case 0:
			LCD12864_String(Addr, "�ռ�¼        ");
			break; //
		case 1:
			LCD12864_String(Addr, "��������      ");
			break; // 1
		case 2:
			LCD12864_String(Addr, "��ѹ����      ");
			break; // 2
		case 3:
			LCD12864_String(Addr, "Ƿѹ����      ");
			break; // 3
		case 4:
			LCD12864_String(Addr, "��Ƶ������    ");
			break; //
		case 5:
			LCD12864_String(Addr, "��Դ����      ");
			break; //
		case 6:
			LCD12864_String(Addr, "Һλ����      ");
			break; //
		case 7:
			LCD12864_String(Addr, "������ѹ      ");
			break; //
		case 8:
			LCD12864_String(Addr, "              ");
			break; //
		case 9:
			LCD12864_String(Addr, "              ");
			break; //
		case 10:
			LCD12864_String(Addr, "ѹ����������  ");
			break; //
		case 11:
			LCD12864_String(Addr, "ȱ�ౣ��      ");
			break; // 11
		case 12:
			LCD12864_String(Addr, "IPM ����      ");
			break; // 12
		case 13:
			LCD12864_String(Addr, "��Ƶ����λʧ��");
			break; //
		case 14:
			LCD12864_String(Addr, "��������������");
			break; // 14
		case 15:
			LCD12864_String(Addr, "��ˮѹ�߱���  ");
			break; //
		case 16:
			LCD12864_String(Addr, "��ʱ����      ");
			break; //
		case 17:
			LCD12864_String(Addr, "IPM ����      ");
			break; // 17
		case 18:
			LCD12864_String(Addr, "              ");
			break; //
		case 19:
			LCD12864_String(Addr, "              ");
			break; //
		case 20:
			LCD12864_String(Addr, "              ");
			break; //
		case 21:
			LCD12864_String(Addr, "              ");
			break; //
		case 22:
			LCD12864_String(Addr, "              ");
			break; //
		case 23:
			LCD12864_String(Addr, "�����ѹ������");
			break; //
		case 24:
			LCD12864_String(Addr, "�������      ");
			break; // 24
		case 25:
			LCD12864_String(Addr, "���ȹ���      ");
			break; // 25
		case 26:
			LCD12864_String(Addr, "EEPROM����    ");
			break; // 26
		case 27:
			LCD12864_String(Addr, "DSP ����������");
			break; // 27
		case 28:
			LCD12864_String(Addr, "STM ����������");
			break; // 28
		case 29:
			LCD12864_String(Addr, "�ӵر���      ");
			break; // 29
		case 30:
			LCD12864_String(Addr, "������Ƿ��ѹ  ");
			break; // 30
		case 31:
			LCD12864_String(Addr, "�����й���ѹ  ");
			break; // 31
		default:
			return;
		}
	}
}

void DspFaultDisplay(u16 FaultValue, u8 Addr) // ZCL 2018.5.18
{
	if (S_DisplayPar == 2) // ZCL 2018.5.19
	{
		switch (FaultValue) // ZCL 2017.6.5 �ĳ�ֻ�б�Ƶ�����ϴ���
		{
		case 0:
			LCD12864_String(Addr, "�޹���- ����  ");
			break; //
		case 1:
			LCD12864_String(Addr, "��������      ");
			break; //
		case 2:
			LCD12864_String(Addr, "��ѹ����      ");
			break; //
		case 3:
			LCD12864_String(Addr, "Ƿѹ����      ");
			break; //
		case 4:
			LCD12864_String(Addr, "ȱ�ౣ��      ");
			break; //
		case 5:
			LCD12864_String(Addr, "IPM ����      ");
			break; //
		case 6:
			LCD12864_String(Addr, "��������������");
			break; //
		case 7:
			LCD12864_String(Addr, "IPM ����      ");
			break; //
		case 8:
			LCD12864_String(Addr, "�������      ");
			break; //
		case 9:
			LCD12864_String(Addr, "���ȹ���      ");
			break; //
		case 10:
			LCD12864_String(Addr, "EEPROM����    ");
			break; //
		case 11:
			LCD12864_String(Addr, "DSP ����������");
			break; //
		case 12:
			LCD12864_String(Addr, "STM ����������");
			break; //
		case 13:
			LCD12864_String(Addr, "�ӵر���      ");
			break; //
		case 14:
			LCD12864_String(Addr, "������Ƿ��ѹ  ");
			break; //
		case 15:
			LCD12864_String(Addr, "�����й���ѹ");
			break; //

		default:
			return;
		}
	}
}

// ��ʾ����.�޸Ĳ���  �ܳ��� 2013.12.6-12.8  �ǳ���Ҫ��һ������������
// p:ҳ  x:��������(h+l)(��͵�ַ��->���λ)  *Address:Ҫ��ʾ���޸ĵĲ���  length:������ʾ����
//	DispMod_Par(2,0x48,&Pw_YeWeiStartFirstPump,4,0);
// LCD12864_Num2(0x96, 3, MainWindow_OutPressure);
// void DispMod_Par(u8 x_pos,u8 length,u16 *Address, u8 ParType, u8 nb_modpar)
// u8 ParType  ZCL 2019.3.8  ����β�û���õ�����Ϊ��ʾ���޸Ķ��õ�����Һ��ARM�Ĳ�����ͨѶ����Ҫ�����������䡣

void DispMod_Par(u8 x_pos, u8 length, u8 XiaoShuBits, u16 *Address, u8 ParType, u8 nb_modpar)
{
	u16 w; // ��Ե�ַ,RelativeAddress

	// u16 *p1;						//ZCL �ۿ� Pw_ScrKeyMode ��ֵַ
	// p1=&Pw_ScrKeyMode;

	u8 nB_Sign; // ZCL 2019.10.18 ���ű�־

	if (ParType >= 10)
	{
		ParType = ParType - 10;
		nB_Sign = 1;
	}
	else
		nB_Sign = 0;

	// ע�⣺RUN . STOP �����Ĳ���ʱ��B_ModPar=3�� ���ⰴ������д������
	if (B_ModPar < 3)
	{
		w = *Address; // ֵ
		//
		// ZCL 2018.5.17 ����10��
		if (Address == &Pw_Time1SetP || Address == &Pw_Time2SetP || Address == &Pw_Time3SetP || Address == &Pw_Time4SetP || Address == &Pw_HaveWaterRunP || Address == &Pw_NoWaterStopP || Address == &Pw_SmallHoldP || Address == &Pw_SetP)
			w = w / 10;

		if (!nb_modpar) // �ֽ���.ǧ.��.ʮ.��λ�������������ʾ���޸ģ��޸���Ҫ�ϲ�
		{
			Value[4] = w / 10000;		   // ��λ
			Value[3] = (w % 10000) / 1000; // ǧλ
			Value[2] = (w % 1000) / 100;   // ��λ
			Value[1] = (w % 100) / 10;	   // ʮλ
			Value[0] = (w % 10);		   // ��λ
		}
		// ZCL 2018.5.14 ע�͵����ŵ����档����Lw_TmpModParValue����ûֵ���ᴮ�и�ֵ����
		/* 	else
				{
					Value[4]=Lw_TmpModParValue/10000;					//��λ
					Value[3]=(Lw_TmpModParValue%10000)/1000;		//ǧλ
					Value[2]=(Lw_TmpModParValue%1000)/100;			//��λ
					Value[1]=(Lw_TmpModParValue%100)/10;				//ʮλ
					Value[0]=(Lw_TmpModParValue%10);				 		//��λ
				}	*/

		// 1.�޸Ĳ���  ����  ------------------------------
		if (nb_modpar)
		{
			// ZCL 2015.9.1 �ս����޸Ĳ���״̬���ȰѲ���ֵ�͵���ʱ����������ʱ�������޸�
			if (S_ModParStatus == 1) // �ս����޸Ĳ�����Ѳ������浽Lw_TmpModParValue���Դ����޸�
			{
				S_ModParStatus = 2;
				Lw_TmpModParValue = w;
				Lw_OldModParValue = w;
				//
			}

			// ZCL 2018.5.14 �Ƶ���������治��
			Value[4] = Lw_TmpModParValue / 10000;		   // ��λ
			Value[3] = (Lw_TmpModParValue % 10000) / 1000; // ǧλ
			Value[2] = (Lw_TmpModParValue % 1000) / 100;   // ��λ
			Value[1] = (Lw_TmpModParValue % 100) / 10;	   // ʮλ
			Value[0] = (Lw_TmpModParValue % 10);		   // ��λ

			if (S_ModParStatus == 2) // �ս����޸Ĳ�����Ѳ������浽Lw_TmpModParValue���Դ����޸�
			{
				if (Sa_ModParKeyData == UPKEY) // ������ 11	ÿ���޸ĺ󣬺ϲ�����ֵ ZCL 2018.5.14
				{
					if (Value[ModParBitPos] < 9)
						Value[ModParBitPos]++;
					else
						Value[ModParBitPos] = 0; // 2015.9.6

					Lw_TmpModParValue = Value[4] * 10000 + Value[3] * 1000 + Value[2] * 100 + Value[1] * 10 + Value[0];
				}
				else if (Sa_ModParKeyData == DOWNKEY) // �½��� 8	ÿ���޸ĺ󣬺ϲ�����ֵ
				{
					if (Value[ModParBitPos] > 0)
						Value[ModParBitPos]--;
					else
						Value[ModParBitPos] = 9; // 2015.9.6
					Lw_TmpModParValue = Value[4] * 10000 + Value[3] * 1000 + Value[2] * 100 + Value[1] * 10 + Value[0];
				}

				else if (Sa_ModParKeyData == LEFTKEY) // ����� 9  �ƶ�����λ��
				{
					if (ModParBitPos + 1 < length) // ֻ���ƶ���length�涨��λ��
						ModParBitPos++;
				}
				else if (Sa_ModParKeyData == RIGHTKEY) // ���Ҽ� 7  �ƶ�����λ��
				{
					if (ModParBitPos > 0) // ֻҪλ�ò���0����1
						ModParBitPos--;
				}

				else if (Sa_ModParKeyData == ESCKEY) // ESCȡ����  10
				{
					S_ModParStatus = 0; // ���޸Ĳ�����ȡ��
					ModParNo = 0;		// �޸Ĳ������=0
					ModParBitPos = 0;	// �޸Ĳ���λλ��=0
				}

				else if (Sa_ModParKeyData == OKKEY) // ENTER�� 6
				{

					// ��ָ���޸Ĳ���
					Lw_TmpModParValue = Value[4] * 10000 + Value[3] * 1000 + Value[2] * 100 + Value[1] * 10 + Value[0];

					// ZCL 2019.4.2 û�в�������������ı䡣 w_GprsModPar ����
					//  ����û�п���Ϳ����޸ĵĲ������£�
					if (Address == &w_ModPar || Address == &w_dspModPar || Address == &w_GprsModPar || Address == &w_TouchRunStop || Address == &w_dspTouchRunStop //) ZCL 2019.9.11
																																								   // ZCL 2019.4.16 Һ��������Ĳ���������Ҫ�����ֹ���Ӳ���ʱ���Ĳ�������
						|| Address == &Pw_ScrKeyMode || Address == &Pw_ScrMenuMode																				   // ZCL 2019.4.16
						|| Address == &Pw_ScrCurrentBits																										   // ZCL 2019.4.16
						|| B_ModYW310SZM220 == 1)																												   // ZCL 2019.10.19  YW310��SZM220�� �޸Ĳ���������Ϊ�����Ĳ������������ֱ���޸ġ�
						;
					// ZCL 2019.4.2 û�в�������������ı�
					else if (w_ModPar != 2000 && w_ModPar != 3000 && w_dspModPar != 2000 && w_dspModPar != 3000 && w_GprsModPar != 2000 && w_GprsModPar != 3000)
					{
						Lw_TmpModParValue = Lw_OldModParValue;
					}

					// ZCL  2018.5.14  ����ֵû�иı�Ļ����Ͳ����޸�
					//  != �����ı�
					if (Lw_OldModParValue != Lw_TmpModParValue) // ZCL 2018.5.14  ������ϻ��������ã����ٰ�OK�������ǻᴮ���޸Ĳ���
					{
						// ZCL 2015.9.1 �д˱�־������ͨѶ�޸�STM32����Ĳ���
						// ZCL 2019.4.6 �¼����ݣ�ֻ��0-3�Ŵ��ڶ�ȡ���ݡ� 4,5���� ����Һ�������������
						// if(ParType>3)  ZCL 2019.10.19
						if (ParType == 4 || ParType == 5) // ZCL 2019.10.19
							B_ModPar = 0;
						else if (ParType == 0 || ParType == 1 || ParType == 2 || ParType == 3 || ParType == 8) // ZCL 2019.10.19
							// ZCL 2019.4.6 �������¼����ݣ�ֻ��0-3�Ŵ��ڶ�ȡ���ݡ� 4,5���� ����Һ�������������
							B_ModPar = 1; //=1�޸Ĳ��������ٶ������� ���޸Ĳ�����һֱ������

						// ZCL 2018.5.17 ����10��
						if (Address == &Pw_Time1SetP || Address == &Pw_Time2SetP || Address == &Pw_Time3SetP || Address == &Pw_Time4SetP || Address == &Pw_HaveWaterRunP || Address == &Pw_NoWaterStopP || Address == &Pw_SmallHoldP || Address == &Pw_SetP)
							Lw_SaveSetValue = Lw_TmpModParValue * 10;
						else
							Lw_SaveSetValue = Lw_TmpModParValue;

						// ZCL 2018.8.4 ����Pw_ScrKeyModeʱ����Ҫw_dspModPar=2000����3000��2018.8.6 ȡ��
						/* 						if(Address==&Pw_ScrKeyMode && w_ModPar!=2000 && w_ModPar!=3000 )
													*Address=w_ScrSaveKeyMode;
												else		//����˳�� */

						*Address = Lw_SaveSetValue;

						EnterMenu_InitPar(); // ZCL 2018.5.17 ����

						// S_RdWrNo = nb_modpar;		//! ����Ҫ�޸���һ������ ZCL 2018.5.14
						S_RdWrNo = ModParNo; // ZCL 2018.5.14
						// S_DisplayPar=2;						//ZCL 2018.5.17 �ı����趨������ˢ����ʾ

						// ZCL 2019.3.22 �������⣺�޸���һ���޸ģ���OK��������һ�£���������޸�λλ�á�����취�����ϴ˾���ˡ�
						// �ֳ������⣺��һ���Զ��˳��޸�״̬������Ϊû������ARM�壬����Write_Stm32Data()�г���3����S_ModParStatus  ����취��ע�͵�S_ModParStatus=0;  ��ע�͵���ModParNo=0; ModParBitPos=0;
						ModParBitPos = 0; // �޸Ĳ���λλ��=0
					}

					//== ����û�иı�
					// ZCL 2018.5.14  ������ϻ��������ã����ٰ�OK�������ǻᴮ���޸Ĳ�����
					// ��ע������ԭ�򣬶�ǰһ�����ڷ���ֵ���͸��˵�ǰ������
					else
					{
						S_ModParStatus = 1; // �޸Ĳ���ȷ�Ϻ�ָ����һ������
						ModParNo++;			// �޸Ĳ������++
						ModParBitPos = 0;	// �޸Ĳ���λλ��=0
					}

					// ZCL 2018.5.14
					//  S_ModParStatus=1;					//�޸Ĳ���ȷ�Ϻ�ָ����һ������
					//  ModParNo++;					//�޸Ĳ������++
					//  ModParBitPos=0;			//�޸Ĳ���λλ��=0
				}
			}

			// ÿ�ΰ������꣬�����㣬�ȴ����°����� �ܳ��� 2013.12.8
			Sa_ModParKeyData = 0;
		}

		// 2.��ʾ���� �ں� -----------------------------------------
		// ZCL 2018.5.17
		Menu_Delay(); // ��ʱ
		if (S_DisplayPar == 2)
		{
			if (nb_modpar) //&& S_ModParStatus>0
				w = Lw_TmpModParValue;

			if (nB_Sign == 1)
				nb_modpar = nb_modpar + 10; // ZCL 2019.10.18  +10 �����и��ţ�

			LCD12864_Num3(x_pos, length, XiaoShuBits, w, nb_modpar);

			// �����ʱ����ӣ����� ������ʾ���ҡ� ZCL 2015.8.31
			// LCD_DLY_ms(1);	 //1MS�Ϳ���
			LCD12864_DelayUS(1000); // 50���У�100��һ�������200������  1000
		}
	}
}

// ZCL 2022.5.21
void DispMod_Par2(u8 x_pos, u8 length, u8 XiaoShuBits, u32 l_value, u8 ParType, u8 nb_modpar)
{
	u32 nl_i; // ��Ե�ַ,RelativeAddress

	// u16 *p1;						//ZCL �ۿ� Pw_ScrKeyMode ��ֵַ
	// p1=&Pw_ScrKeyMode;

	nl_i = l_value; // ֵ

	// ע�⣺RUN . STOP �����Ĳ���ʱ��B_ModPar=3�� ���ⰴ������д������
	if (B_ModPar < 3)
	{

		// 2.��ʾ���� �ں� -----------------------------------------
		// ZCL 2018.5.17
		Menu_Delay(); // ��ʱ
		if (S_DisplayPar == 2)
		{

			// ZCL 2022.3.10 ��������  ��������ʾ�á�w32_PulseCouter ������ֱ�Ӹ�ֵ�ˣ�w_ScrPulseSumH w_ScrPulseSumH û���õ���
			//..

			LCD12864_Num3(x_pos, length, XiaoShuBits, nl_i, nb_modpar);

			// �����ʱ����ӣ����� ������ʾ���ҡ� ZCL 2015.8.31
			// LCD_DLY_ms(1);	 //1MS�Ϳ���
			LCD12864_DelayUS(1000); // 50���У�100��һ�������200������  1000
		}
	}
}

// ZCL 2019.4.5
void DispMod_Par2_String(unsigned int Address, const char *Str)
{
	// 2.��ʾ���� �ں� -----------------------------------------
	// ZCL 2018.5.17
	Menu_Delay(); // ��ʱ
	if (S_DisplayPar == 2)
	{
		LCD12864_String(Address, Str);

		// �����ʱ����ӣ����� ������ʾ���ҡ� ZCL 2015.8.31
		// LCD_DLY_ms(1);	 //1MS�Ϳ���
		LCD12864_DelayUS(1000); // 50���У�100��һ�������200������  1000
	}
}

// ZCL 2019.4.5
void DispMod_Par3_Num(unsigned int Address, long Num)
{
	// 2.��ʾ���� �ں� -----------------------------------------
	// ZCL 2018.5.17
	Menu_Delay(); // ��ʱ
	if (S_DisplayPar == 2)
	{

		LCD12864_Num(Address, Num);

		// �����ʱ����ӣ����� ������ʾ���ҡ� ZCL 2015.8.31
		// LCD_DLY_ms(1);	 //1MS�Ϳ���
		LCD12864_DelayUS(1000); // 50���У�100��һ�������200������  1000
	}
}

void ModParNum(u8 num) // ָ��:�޸Ĳ���������
{
	F_ModPar1 = 0;
	F_ModPar2 = 0;
	F_ModPar3 = 0;
	F_ModPar4 = 0;
	F_ModPar5 = 0;
	F_ModPar6 = 0;
	if (ModParNo > num)
	{
		ModParNo = 0;		// �������޸�
		S_ModParStatus = 0; // ������Ҫ������֤�����޸���ϣ����Ѿ�����ҳ���Ĳ������˳��޸�״̬
	}
	else if (ModParNo == 1)
		F_ModPar1 = 1;
	else if (ModParNo == 2)
		F_ModPar2 = 2;
	else if (ModParNo == 3)
		F_ModPar3 = 3;
	else if (ModParNo == 4)
		F_ModPar4 = 4;
	else if (ModParNo == 5)
		F_ModPar5 = 5;
	else if (ModParNo == 6)
		F_ModPar6 = 6;
}

//********************����Ϊ��������****************************
// ���������ֱ�Ϊ�� ���ϼ�   CANCEL
//								�����		���Ҽ�
//								���¼�		ENTER
/*********************��ֵɨ��************************************/
void GetKey(void) // ZCL �õ������ļ�ֵ 2013.12.07
{
	// 1.��ֵ
	Key_Data = 0;
	if (KEY1 == 0) // KEY==0���¼�
		Key_Data = 1;
	else if (KEY2 == 0) //
		Key_Data = 2;
	// else if( KEY3==0 )		//ZCL 2018.11.13  COM_KEY_3�ߵ���ʱ����ˣ�����M35 STATUS��⡣����ע�͵�
	// Key_Data = 3;
	// else if( KEY4==0 )		//ZCL 2018.10.19
	// Key_Data = 4;
	// else if( KEY5==0 )		//ZCL 2018.12.06
	// Key_Data = 5;
	else if (KEY6 == 0) //
		Key_Data = 6;
	else if (KEY7 == 0) //
		Key_Data = 7;
	else if (KEY8 == 0) //
		Key_Data = 8;
	else if (KEY9 == 0) //
		Key_Data = 9;
	else if (KEY10 == 0) //
		Key_Data = 10;
	else if (KEY11 == 0) //
		Key_Data = 11;

	// 2.����
	if (Sa_OldKeyData != Key_Data) // �����仯�������ֵ�����������
	{
		Sa_OldKeyData = Key_Data;
		Lw_KEYStableCounter = 0;
	}
	else // �������仯�������ﵽ�ȶ���Ŀ
	{
		if (Lw_KEYStableCounter++ > 10) // ��ѭ����������
		{
			Lw_KEYStableCounter = 0;
			if (Key_Data == 0) // ����������
			{
				Key_Pressed = 0;
				F_KeyExec = 0;
			}
			else // ��������		//ZCL 2018.9.29  else ��С��ɾ����V138---V139-2����������ϣ�
			{
				Key_Pressed = 1;
				C_LCDBackLight = 1;

				// ZCL 2018.8.3 �Ӱ���ģʽ���԰�����ͬ���м��ݣ��ı䰴������ֵ��
				if (Pw_ScrKeyMode == 0) //=0��8������������ģʽ
					;
				else if (Pw_ScrKeyMode == 1) //=1��7������
				{
					if (Key_Data == DOWNKEY)
						Key_Data = UPKEY;
					else if (Key_Data == UPKEY)
						Key_Data = DOWNKEY;
					else if (Key_Data == SETKEY)
						Key_Data = LEFTKEY;
				}
				else if (Pw_ScrKeyMode == 2) //=2��8�����������¼�����
				{
					if (Key_Data == DOWNKEY)
						Key_Data = UPKEY;
					else if (Key_Data == UPKEY)
						Key_Data = DOWNKEY;
				}
			}
		}
	}

	if (Pw_EquipmentType == 0) // ˫����
	{
		// 3.�����ı仭��Ĵ���
		if (S_ModParStatus == 0)
		{
			if (Key_Pressed == 1 && Key_Data != 0 && F_KeyExec == 0)
			{
				F_KeyExec = 1;

				// ZCL 2019.3.8 LORA�ӻ�
				if (Pw_LoRaMasterSlaveSel == 0) // ������ԭ�ȵ�����
				{

					// zcl 2014.3.26 >=3����󣬱��������ȷ�ſ�����ʾ && (Lw_PageNo<4 || Pw_ModPar==10000 || Lw_PageNo>4 )
					if (Key_Data == DOWNKEY && Lw_PageNo < 999) // Lw_PageNo<11  ZCL 2018.8.28 308
					{
						Lw_PageNo++;

						if (Pw_ScrMenuMode == 0)
						{
							if (Lw_PageNo == 20 + 1) // ZCL 2018.8.21 18;  2019.8.7 19
								Lw_PageNo = 100;
							else if (Lw_PageNo == 135 + 1) // ZCL 2018.8.21 125;		2019.8.31 133
								Lw_PageNo = 199;		   // ZCL 2018.8.28 200;
							else if (Lw_PageNo == 207 + 1) // ZCL 2018.8.28 =0��������ģʽ
								Lw_PageNo = 300;
							else if (Lw_PageNo == 310) // ZCL 2019.4.15 ��Ƶ���ظ���310����
								Lw_PageNo = 543;

							// ZCL 2018.4.19 ���� 7: Menu_StmReadPar8
							else if (Lw_PageNo == 7)
								Lw_PageNo = 8;
							else if (Lw_PageNo == 544) // ZCL 2019.4.9 �ӻ�����544ҳ
								Lw_PageNo = 546;
							else if (Lw_PageNo == 550 + 1) // ZCL 2019.4.5
								Lw_PageNo = 999;
						}

						else if (Pw_ScrMenuMode == 1)
						{
							if (Lw_PageNo == 221 + 1) // ZCL 2019.3.19 =1����Ƶ��ģʽ
								Lw_PageNo = 300;
							else if (Lw_PageNo == 310 + 1)
								Lw_PageNo = 543; // ZCL 2019.3.23

							else if (Lw_PageNo == 544) // ZCL 2019.4.9 �ӻ�����544ҳ
								Lw_PageNo = 546;
							else if (Lw_PageNo == 550 + 1) // ZCL 2019.4.5
								Lw_PageNo = 999;

							else if (Lw_PageNo == 210) // ZCL 2019.4.15 ����210��Menu_StmReadPar8_NoUse()
								Lw_PageNo = 211;
						}

						else if (Pw_ScrMenuMode == 2) // ZCL 2019.3.19 =11��DCM220��Ƶ��ģʽ
						{
							if (Lw_PageNo == 207 + 1)
								Lw_PageNo = 250;
							else if (Lw_PageNo == 261 + 1)
								Lw_PageNo = 300;
							else if (Lw_PageNo == 310 + 1)
								Lw_PageNo = 330;
							else if (Lw_PageNo == 331 + 1) // ZCL 2019.3.23
								Lw_PageNo = 543;
							//

							else if (Lw_PageNo == 544) // ZCL 2019.4.9 �ӻ�����544ҳ
								Lw_PageNo = 546;
							else if (Lw_PageNo == 550 + 1) // ZCL 2019.4.5
								Lw_PageNo = 999;
						}
					}

					else if (Key_Data == UPKEY && Lw_PageNo > 0)
					{
						Lw_PageNo--;

						if (Pw_ScrMenuMode == 0) // ZCL 2018.8.28 =0��������ģʽ
						{
							if (Lw_PageNo == 543 - 1) // ZCL 2019.4.2  544
								Lw_PageNo = 310 - 1;  // ZCL 2019.4.15 ��Ƶ���ظ���310����
							else if (Lw_PageNo == 300 - 1)
								Lw_PageNo = 207;
							else if (Lw_PageNo == 199 - 1) // ZCL 2018.8.28 =0��������ģʽ
								Lw_PageNo = 135;		   // ZCL 2019.8.31 133
							else if (Lw_PageNo == 100 - 1)
								Lw_PageNo = 20; // ZCL 2019.8.7

							// ZCL 2018.4.19 �������� 7: Menu_StmReadPar8
							// ZCL 2019.4.11 ��ע��������ҳ��ֱ��д���֣�����+1��-1����7ҳ����544ҳ������
							else if (Lw_PageNo == 7)
								Lw_PageNo = 6;
							else if (Lw_PageNo == 544) // ZCL 2019.4.9 �ӻ�����544ҳ
								Lw_PageNo = 543;
							else if (Lw_PageNo == 999 - 1) // ZCL 2019.4.5
								Lw_PageNo = 551 - 1;
						}

						else if (Pw_ScrMenuMode == 1) // ZCL 2019.3.19 =1����Ƶ��ģʽ
						{
							if (Lw_PageNo == 543 - 1) // ZCL 2019.4.2  544
								Lw_PageNo = 310;
							else if (Lw_PageNo == 300 - 1)
								Lw_PageNo = 221;
							else if (Lw_PageNo == 200 - 1)
								Lw_PageNo = 200;

							else if (Lw_PageNo == 544) // ZCL 2019.4.9 �ӻ�����544ҳ
								Lw_PageNo = 543;
							else if (Lw_PageNo == 999 - 1) // ZCL 2019.4.5
								Lw_PageNo = 551 - 1;
							else if (Lw_PageNo == 210) // ZCL 2019.4.15 ����210��Menu_StmReadPar8_NoUse()
								Lw_PageNo = 209;
						}

						else if (Pw_ScrMenuMode == 2) // ZCL 2019.3.19 =11��DCM220��Ƶ��ģʽ
						{
							if (Lw_PageNo == 543 - 1) // ZCL 2019.3.23
								Lw_PageNo = 331;
							else if (Lw_PageNo == 330 - 1)
								Lw_PageNo = 310;
							else if (Lw_PageNo == 300 - 1)
								Lw_PageNo = 261;
							else if (Lw_PageNo == 250 - 1)
								Lw_PageNo = 207;
							else if (Lw_PageNo == 200 - 1)
								Lw_PageNo = 200;

							else if (Lw_PageNo == 544) // ZCL 2019.4.9 �ӻ�����544ҳ
								Lw_PageNo = 543;
							else if (Lw_PageNo == 999 - 1) // ZCL 2019.4.5
								Lw_PageNo = 551 - 1;
						}
					}

					else if (Key_Data == ESCKEY) // CANCEL��������
						Lw_PageNo = 0;
					else if (Key_Data == OKKEY) // ENTER��  ��������޸�״̬
					{
						S_ModParStatus = 1; // ��������޸�״̬
						ModParNo++;			// �޸Ĳ������++
					}
					else if (Key_Data == RIGHTKEY) // �Ҽ�
					{
						B_RightKeyCount++;
					}
					else if (Key_Data == LEFTKEY) // ���
					{
						B_LeftKeyCount++;
					}
				}

				// ZCL 2019.3.8 �¼� LORA����
				else if (Pw_LoRaMasterSlaveSel == 1) // ������ԭ�ȵ�����
				{
					if (Key_Data == UPKEY && Lw_PageNo > 500) // Lw_PageNo>0  ZCL 2016.5.27 �޸�
					{
						// if(Lw_PageNo==300)
						// Lw_PageNo=218;								//2016.5.27 209   			//ZCL 2016.10.6 208�ĳ�209
						/* 				else if(Lw_PageNo==200)			//2016.5.23 ZCL ȡ��
											Lw_PageNo=123;
										else if(Lw_PageNo==100)
											Lw_PageNo=17;	 */
						//
						// else
						Lw_PageNo--;

						// ZCL 2018.4.19 �������� Menu_DspReadPar5 =205
						//  if(Lw_PageNo==205)
						//  Lw_PageNo=204;

						if (Lw_PageNo == 999 - 1) // ZCL 2019.4.5
							Lw_PageNo = 565;

						if (Lw_PageNo == 543 - 1) // ZCL 2019.9.12
							Lw_PageNo = 525;	  // ZCL 2019.10.18  520		540
					}

					// zcl 2014.3.26 >=3����󣬱��������ȷ�ſ�����ʾ && (Lw_PageNo<4 || Pw_ModPar==10000 || Lw_PageNo>4 )
					else if (Key_Data == DOWNKEY && Lw_PageNo < 999) // Lw_PageNo<11
					{
						// if(Lw_PageNo==218)						//2016.5.27 209					//ZCL 2016.10.6 208�ĳ�209
						// Lw_PageNo=300;
						/* 				else if(Lw_PageNo==123)		//2016.5.23 ZCL ȡ��
											Lw_PageNo=200;
										else if(Lw_PageNo==17)
											Lw_PageNo=100;	 */
						//
						// else
						Lw_PageNo++;

						// ZCL 2018.4.19 �������� Menu_DspReadPar5 =205
						//  if(Lw_PageNo==205)
						//  Lw_PageNo=206;

						if (Lw_PageNo == 565 + 1) // ZCL 2019.4.5
							Lw_PageNo = 999;

						if (Lw_PageNo == 525 + 1) // ZCL 2019.9.12 520  540
							Lw_PageNo = 543;
					}
					else if (Key_Data == ESCKEY) // CANCEL��������
						// Lw_PageNo=0;			//2016.5.23 ZCL �޸�
						Lw_PageNo = 500;		// 2016.5.23 ZCL �޸�
					else if (Key_Data == OKKEY) // ENTER��
					{
						S_ModParStatus = 1;
						ModParNo++;
						// w_KeyPressDelay=1805;
					}
					else if (Key_Data == RIGHTKEY) // �Ҽ�
					{
						B_RightKeyCount++;
					}
					else if (Key_Data == LEFTKEY) // ���
					{
						B_LeftKeyCount++;
					}
				}
			}
		}
		// 4.�����޸Ĳ����Ĵ���
		else
		{
			if (Key_Pressed == 1 && Key_Data != 0 && F_KeyExec == 0)
			{
				F_KeyExec = 1;
				Sa_ModParKeyData = Key_Data;
			}
		}

		// 5. RUN . STOP �����Ĳ����� ���ⰴ������д���� ZCL  2015.9.5  2016.12.6
		// ZCL 2018.5.19 ��һ����ֲ V145-3�� ԭ��DSP���ڲ������ϣ�����ARM����������ͣ����ָ��Ҳ�õ�ͨѶ�����ٽ��С�
		if (Key_Pressed == 1 && Key_Data == STOPKEY)
		{
			if (Pw_ScrMenuMode == 2)
				w_dspTouchRunStop = 1;							 // ZCL 2016.12.5 �ĳ�DSP�еĲ���
			else if (Pw_ScrMenuMode == 0 || Pw_ScrMenuMode == 1) // ZCL 2019.4.6 ��DCM100�еİ�����ͣ����
				w_TouchRunStop = 1;
			// B_ModPar=3;
			B_PressedStopRunKey = 1;
		}
		else if (Key_Pressed == 1 && Key_Data == RUNKEY)
		{
			if (Pw_ScrMenuMode == 2)
				w_dspTouchRunStop = 0;							 // ZCL 2016.12.5 �ĳ�DSP�еĲ���
			else if (Pw_ScrMenuMode == 0 || Pw_ScrMenuMode == 1) // ZCL 2019.4.6 ��DCM100�еİ�����ͣ����
				w_TouchRunStop = 0;
			// B_ModPar=3;
			B_PressedStopRunKey = 1;
		}

		// ���ⰴ������д���� 2015.9.5
		if (B_ModPar == 100)
		{
			/* 		B_ModPar=4;
					S_Com1Send=0;
					C_Com1Send=0;
					S_Com1SendNoNum=0;

				}
				if(B_ModPar==4)
				{ */
			if (Pw_ScrMenuMode == 2)
				Write_Stm32Data(Address(&w_dspTouchRunStop, 3), w_dspTouchRunStop); // ZCL 2016.12.5 �ĳ�DSP�еĲ���
			else if (Pw_ScrMenuMode == 0 || Pw_ScrMenuMode == 1)					// ZCL 2019.4.6 ��DCM100�еİ�����ͣ����
				Write_Stm32Data(Address(&w_TouchRunStop, 1), w_TouchRunStop);
		}

		// 6.��ݼ� ZCL 2015.9.6  �찴2�����Ҽ� ���� �������ֻ��һ�γ�ʱ�����ݼ�������
		if (Pw_LoRaMasterSlaveSel == 0) // LORA�ӻ�
		{
			if (Pw_ScrMenuMode == 0) //=0 DCM100 ������ģʽ	 2019.3.19
			{
				// ���Ҽ�
				if (Lw_PageNo == 0 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 5;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2019.3.19
				else if (Lw_PageNo == 5 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 10;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 10 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 17;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 17 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 101;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 101 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 103;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 103 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 108;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 108 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 113;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 113 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 117;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 117 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 121;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 121 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 125;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 125 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 129;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}

				else if (Lw_PageNo == 129 && B_RightKeyCount == 2) // ZCL 2018.8.21
				{
					Lw_PageNo = 135;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2019.8.31 133
				else if (Lw_PageNo == 135 && B_RightKeyCount == 2) // ZCL 2018.8.21 2019.8.31 133
				{
					Lw_PageNo = 200;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}

				else if (Lw_PageNo == 200 && B_RightKeyCount == 2) // ZCL 2018.8.28 =0������
				{
					Lw_PageNo = 205;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 205 && B_RightKeyCount == 2) // ZCL 2018.8.28 =0������
				{
					Lw_PageNo = 301;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 301 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 305;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28
				else if (Lw_PageNo == 305 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 309;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2019.4.15 310
				// ZCL 2019.4.15 2019.9.11
				else if (Lw_PageNo == 309 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 543;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 543 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 548;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 548 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 999;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}

				// �����
				else if (Lw_PageNo == 0 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 200;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 200 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 202;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 202 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 205;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 205 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 207;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 207 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 301;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 301 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 305;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				} // ZCL 2018.8.28 308
				else if (Lw_PageNo == 305 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 309;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2019.4.15 310
				// ZCL 2019.4.15 2019.9.11
				else if (Lw_PageNo == 309 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 543;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 543 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 548;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 548 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 999;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}

				//
				else if (Lw_PageNo == 101 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 301;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				} // ZCL 2018.8.29 ��Ƶ����ʱ�仭��
			}

			else if (Pw_ScrMenuMode == 1) //=1 DCM100 ��Ƶ��ģʽ	2019.3.19
			{
				// ���Ҽ�
				if (Lw_PageNo == 200 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 208;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 208 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 300;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308
				else if (Lw_PageNo == 300 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 301;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308
				else if (Lw_PageNo == 301 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 305;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308
				else if (Lw_PageNo == 305 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 309;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2019.4.15 310
				// ZCL 2019.4.15 2019.9.11
				else if (Lw_PageNo == 309 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 543;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 543 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 548;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 548 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 999;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}

				// ZCL 2018.8.28 ��Ƶ��ģʽ
				// �����
				else if (Lw_PageNo == 200 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 207;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 207 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 213;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 213 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 220;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 220 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 221;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 221 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 301;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 301 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 305;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 305 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 309;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2019.4.15 310
				// ZCL 2019.4.15 2019.9.11
				else if (Lw_PageNo == 309 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 543;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 543 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 548;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 548 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 999;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
			}

			else if (Pw_ScrMenuMode == 2) //=11 DCM220 ��Ƶ��ģʽ
			{
				// ���Ҽ�
				if (Lw_PageNo == 200 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 207;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 207 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 300;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308
				else if (Lw_PageNo == 300 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 301;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308
				else if (Lw_PageNo == 301 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 305;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308
				else if (Lw_PageNo == 305 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 309;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308��2019.9.11 310
				else if (Lw_PageNo == 309 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 310;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2019.4.15 310
				// ZCL 2019.4.15 2019.9.11
				else if (Lw_PageNo == 310 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 330;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308
				// ZCL 2019.3.23
				else if (Lw_PageNo == 330 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 543;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 543 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 548;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 548 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 999;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}

				// ZCL 2018.8.28 ��Ƶ��ģʽ
				// �����
				else if (Lw_PageNo == 200 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 207;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 207 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 250;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 250 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 255;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 255 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 261;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}

				// ZCL 2019.4.16 ���Ӹ����޸Ĳ�������7������Һ������ֻ�����
				else if (Lw_PageNo == 261 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 300;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}

				else if (Lw_PageNo == 300 && B_LeftKeyCount == 2) // 261
				{
					Lw_PageNo = 301;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 301 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 305;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 305 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 309;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308
				else if (Lw_PageNo == 309 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 310;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				} // ZCL 2019.4.15 310
				// ZCL 2019.4.15 2019.9.10
				else if (Lw_PageNo == 310 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 330;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308
				// ZCL 2019.3.23
				else if (Lw_PageNo == 330 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 543;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 543 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 548;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 548 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 999;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
			}
		}

		// ZCL 2019.3.14 ���� LORA������ʱ�򣬿�ݼ�
		else if (Pw_LoRaMasterSlaveSel == 1) // LORA����
		{
			// ���Ҽ� ZCL 2017.3.17
			if (Lw_PageNo == 500 && B_RightKeyCount == 2) // ZCL 2016.10.6  ��ʼ���水��ݼ����� ��Ƶ�趨����
			{
				Lw_PageNo = 543;
				B_RightKeyCount = 0;
				B_LeftKeyCount = 0;
			}
			else if (Lw_PageNo == 543 && B_RightKeyCount == 2)
			{
				Lw_PageNo = 544;
				B_RightKeyCount = 0;
				B_LeftKeyCount = 0;
			}
			else if (Lw_PageNo == 544 && B_RightKeyCount == 2)
			{
				Lw_PageNo = 548;
				B_RightKeyCount = 0;
				B_LeftKeyCount = 0;
			}
			else if (Lw_PageNo == 548 && B_RightKeyCount == 2)
			{
				Lw_PageNo = 551;
				B_RightKeyCount = 0;
				B_LeftKeyCount = 0;
			}

			// ZCL 2019.10.19 YW310 SZM220 ����Ŀ�ݼ�
			else if (Lw_PageNo == 551 && B_RightKeyCount == 2)
			{
				Lw_PageNo = 555;
				B_RightKeyCount = 0;
				B_LeftKeyCount = 0;
			}
			else if (Lw_PageNo == 555 && B_RightKeyCount == 2)
			{
				Lw_PageNo = 559;
				B_RightKeyCount = 0;
				B_LeftKeyCount = 0;
			}
			else if (Lw_PageNo == 559 && B_RightKeyCount == 2)
			{
				Lw_PageNo = 565;
				B_RightKeyCount = 0;
				B_LeftKeyCount = 0;
			}

			else if (Lw_PageNo == 565 && B_RightKeyCount == 2) // ZCL 2019.10.19 551
			{
				Lw_PageNo = 999;
				B_RightKeyCount = 0;
				B_LeftKeyCount = 0;
			}

			// ����� ZCL 2017.3.17
			else if (Lw_PageNo == 500 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 501;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}
			else if (Lw_PageNo == 501 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 506;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}
			else if (Lw_PageNo == 506 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 511;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}
			else if (Lw_PageNo == 511 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 516;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}

			// ZCL 2019.10.18 5-8�ŵ���Ŀ�ݼ�
			else if (Lw_PageNo == 516 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 521;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}
			else if (Lw_PageNo == 521 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 543;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}

			else if (Lw_PageNo == 543 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 544;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}
			else if (Lw_PageNo == 544 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 548;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}
			else if (Lw_PageNo == 548 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 551;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}

			// ZCL 2019.10.19 YW310 SZM220 ����Ŀ�ݼ�
			else if (Lw_PageNo == 551 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 555;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}
			else if (Lw_PageNo == 555 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 559;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}
			else if (Lw_PageNo == 559 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 565;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}

			else if (Lw_PageNo == 565 && B_LeftKeyCount == 2) // ZCL 2019.10.19 551
			{
				Lw_PageNo = 999;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}
		}
	}
	else // ��Ƶ���
	{

		// 3.�����ı仭��Ĵ���
		if (S_ModParStatus == 0)
		{
			if (Key_Pressed == 1 && Key_Data != 0 && F_KeyExec == 0)
			{
				F_KeyExec = 1;

				// ZCL 2019.3.8 LORA�ӻ�
				if (Pw_LoRaMasterSlaveSel == 0) // ������ԭ�ȵ�����
				{

					// zcl 2014.3.26 >=3����󣬱��������ȷ�ſ�����ʾ && (Lw_PageNo<4 || Pw_ModPar==10000 || Lw_PageNo>4 )
					if (Key_Data == DOWNKEY && Lw_PageNo < 999) // Lw_PageNo<11  ZCL 2018.8.28 308
					{
						Lw_PageNo++;

						if (Pw_ScrMenuMode == 0)
						{
							if (Lw_PageNo == 20 + 1) // ZCL 2018.8.21 18;  2019.8.7 19
								Lw_PageNo = 100;
							else if (Lw_PageNo == 135 + 1) // ZCL 2018.8.21 125;		2019.8.31 133
								Lw_PageNo = 199;		   // ZCL 2018.8.28 200;
							else if (Lw_PageNo == 207 + 1) // ZCL 2018.8.28 =0��������ģʽ
								Lw_PageNo = 300;
							else if (Lw_PageNo == 310) // ZCL 2019.4.15 ��Ƶ���ظ���310����
								Lw_PageNo = 523;

							// ZCL 2018.4.19 ���� 7: Menu_StmReadPar8
							else if (Lw_PageNo == 7)
								Lw_PageNo = 8;
							else if (Lw_PageNo == 524) // ZCL 2019.4.9 �ӻ�����524ҳ
								Lw_PageNo = 525;
							else if (Lw_PageNo == 529 + 1) // ZCL 2019.4.5
								Lw_PageNo = 999;
						}

						else if (Pw_ScrMenuMode == 1)
						{
							if (Lw_PageNo == 221 + 1) // ZCL 2019.3.19 =1����Ƶ��ģʽ
								Lw_PageNo = 300;
							else if (Lw_PageNo == 310 + 1)
								Lw_PageNo = 523; // ZCL 2019.3.23

							else if (Lw_PageNo == 524) // ZCL 2019.4.9 �ӻ�����524ҳ
								Lw_PageNo = 525;
							else if (Lw_PageNo == 529 + 1) // ZCL 2019.4.5
								Lw_PageNo = 999;

							else if (Lw_PageNo == 210) // ZCL 2019.4.15 ����210��Menu_StmReadPar8_NoUse()
								Lw_PageNo = 211;
						}

						else if (Pw_ScrMenuMode == 2) // ZCL 2019.3.19 =11��DCM220��Ƶ��ģʽ
						{
							if (Lw_PageNo == 207 + 1)
								Lw_PageNo = 250;
							else if (Lw_PageNo == 261 + 1)
								Lw_PageNo = 300;
							else if (Lw_PageNo == 310 + 1)
								Lw_PageNo = 330;
							else if (Lw_PageNo == 332 + 1) // ZCL 2020.4.8
								Lw_PageNo = 523;
							//

							else if (Lw_PageNo == 524) // ZCL 2019.4.9 �ӻ�����524ҳ
								Lw_PageNo = 525;
							else if (Lw_PageNo == 529 + 1) // ZCL 2019.4.5
								Lw_PageNo = 999;
						}
					}

					else if (Key_Data == UPKEY && Lw_PageNo > 0)
					{
						Lw_PageNo--;

						if (Pw_ScrMenuMode == 0) // ZCL 2018.8.28 =0��������ģʽ
						{
							if (Lw_PageNo == 523 - 1) // ZCL 2019.4.2  524
								Lw_PageNo = 310 - 1;  // ZCL 2019.4.15 ��Ƶ���ظ���310����
							else if (Lw_PageNo == 300 - 1)
								Lw_PageNo = 207;
							else if (Lw_PageNo == 199 - 1) // ZCL 2018.8.28 =0��������ģʽ
								Lw_PageNo = 135;		   // ZCL 2019.8.31 133
							else if (Lw_PageNo == 100 - 1)
								Lw_PageNo = 20; // ZCL 2019.8.7

							// ZCL 2018.4.19 �������� 7: Menu_StmReadPar8
							// ZCL 2019.4.11 ��ע��������ҳ��ֱ��д���֣�����+1��-1����7ҳ����524ҳ������
							else if (Lw_PageNo == 7)
								Lw_PageNo = 6;
							else if (Lw_PageNo == 524) // ZCL 2019.4.9 �ӻ�����524ҳ
								Lw_PageNo = 523;
							else if (Lw_PageNo == 999 - 1) // ZCL 2019.4.5
								Lw_PageNo = 530 - 1;
						}

						else if (Pw_ScrMenuMode == 1) // ZCL 2019.3.19 =1����Ƶ��ģʽ
						{
							if (Lw_PageNo == 523 - 1) // ZCL 2019.4.2  524
								Lw_PageNo = 310;
							else if (Lw_PageNo == 300 - 1)
								Lw_PageNo = 221;
							else if (Lw_PageNo == 200 - 1)
								Lw_PageNo = 200;

							else if (Lw_PageNo == 524) // ZCL 2019.4.9 �ӻ�����524ҳ
								Lw_PageNo = 523;
							else if (Lw_PageNo == 999 - 1) // ZCL 2019.4.5
								Lw_PageNo = 530 - 1;
							else if (Lw_PageNo == 210) // ZCL 2019.4.15 ����210��Menu_StmReadPar8_NoUse()
								Lw_PageNo = 209;
						}

						else if (Pw_ScrMenuMode == 2) // ZCL 2019.3.19 =11��DCM220��Ƶ��ģʽ
						{
							if (Lw_PageNo == 523 - 1) // ZCL 2019.3.23
								Lw_PageNo = 332;
							else if (Lw_PageNo == 330 - 1)
								Lw_PageNo = 310;
							else if (Lw_PageNo == 300 - 1)
								Lw_PageNo = 261;
							else if (Lw_PageNo == 250 - 1)
								Lw_PageNo = 207;
							else if (Lw_PageNo == 200 - 1)
								Lw_PageNo = 200;

							else if (Lw_PageNo == 524) // ZCL 2019.4.9 �ӻ�����524ҳ
								Lw_PageNo = 523;
							else if (Lw_PageNo == 999 - 1) // ZCL 2019.4.5
								Lw_PageNo = 530 - 1;
						}
					}

					else if (Key_Data == ESCKEY) // CANCEL��������
						Lw_PageNo = 0;
					else if (Key_Data == OKKEY) // ENTER��  ��������޸�״̬
					{
						S_ModParStatus = 1; // ��������޸�״̬
						ModParNo++;			// �޸Ĳ������++
					}
					else if (Key_Data == RIGHTKEY) // �Ҽ�
					{
						B_RightKeyCount++;
					}
					else if (Key_Data == LEFTKEY) // ���
					{
						B_LeftKeyCount++;
					}
				}

				// ZCL 2019.3.8 �¼� LORA����
				else if (Pw_LoRaMasterSlaveSel == 1) // ������ԭ�ȵ�����
				{
					if (Key_Data == UPKEY && Lw_PageNo > 500) // Lw_PageNo>0  ZCL 2016.5.27 �޸�
					{
						Lw_PageNo--;

						if (Lw_PageNo == 999 - 1) // ZCL 2019.4.5
							Lw_PageNo = 534;

						if (Lw_PageNo == 523 - 1) // ZCL 2019.9.12
							Lw_PageNo = 520;
					}

					// zcl 2014.3.26 >=3����󣬱��������ȷ�ſ�����ʾ && (Lw_PageNo<4 || Pw_ModPar==10000 || Lw_PageNo>4 )
					else if (Key_Data == DOWNKEY && Lw_PageNo < 999) // Lw_PageNo<11
					{
						Lw_PageNo++;

						if (Lw_PageNo == 534 + 1) // ZCL 2019.4.5
							Lw_PageNo = 999;

						if (Lw_PageNo == 520 + 1) // ZCL 2019.9.12
							Lw_PageNo = 523;
					}
					else if (Key_Data == ESCKEY) // CANCEL��������
						Lw_PageNo = 500;		 // 2016.5.23 ZCL �޸�
					else if (Key_Data == OKKEY)	 // ENTER��
					{
						S_ModParStatus = 1;
						ModParNo++;
						// w_KeyPressDelay=1805;
					}
					else if (Key_Data == RIGHTKEY) // �Ҽ�
					{
						B_RightKeyCount++;
					}
					else if (Key_Data == LEFTKEY) // ���
					{
						B_LeftKeyCount++;
					}
				}
			}
		}
		// 4.�����޸Ĳ����Ĵ���
		else
		{
			if (Key_Pressed == 1 && Key_Data != 0 && F_KeyExec == 0)
			{
				F_KeyExec = 1;
				Sa_ModParKeyData = Key_Data;
			}
		}

		// 5. RUN . STOP �����Ĳ����� ���ⰴ������д���� ZCL  2015.9.5  2016.12.6
		// ZCL 2018.5.19 ��һ����ֲ V145-3�� ԭ��DSP���ڲ������ϣ�����ARM����������ͣ����ָ��Ҳ�õ�ͨѶ�����ٽ��С�
		if (Key_Pressed == 1 && Key_Data == STOPKEY)
		{
			if (Pw_ScrMenuMode == 2)
				w_dspTouchRunStop = 1;							 // ZCL 2016.12.5 �ĳ�DSP�еĲ���
			else if (Pw_ScrMenuMode == 0 || Pw_ScrMenuMode == 1) // ZCL 2019.4.6 ��DCM100�еİ�����ͣ����
				w_TouchRunStop = 1;
			// B_ModPar=3;
			B_PressedStopRunKey = 1;
		}
		else if (Key_Pressed == 1 && Key_Data == RUNKEY)
		{
			if (Pw_ScrMenuMode == 2)
				w_dspTouchRunStop = 0;							 // ZCL 2016.12.5 �ĳ�DSP�еĲ���
			else if (Pw_ScrMenuMode == 0 || Pw_ScrMenuMode == 1) // ZCL 2019.4.6 ��DCM100�еİ�����ͣ����
				w_TouchRunStop = 0;
			// B_ModPar=3;
			B_PressedStopRunKey = 1;
		}

		// ���ⰴ������д���� 2015.9.5
		if (B_ModPar == 100)
		{
			/* 		B_ModPar=4;
					S_Com1Send=0;
					C_Com1Send=0;
					S_Com1SendNoNum=0;

				}
				if(B_ModPar==4)
				{ */
			if (Pw_ScrMenuMode == 2)
				Write_Stm32Data(Address(&w_dspTouchRunStop, 3), w_dspTouchRunStop); // ZCL 2016.12.5 �ĳ�DSP�еĲ���
			else if (Pw_ScrMenuMode == 0 || Pw_ScrMenuMode == 1)					// ZCL 2019.4.6 ��DCM100�еİ�����ͣ����
				Write_Stm32Data(Address(&w_TouchRunStop, 1), w_TouchRunStop);
		}

		// 6.��ݼ� ZCL 2015.9.6  �찴2�����Ҽ� ���� �������ֻ��һ�γ�ʱ�����ݼ�������
		if (Pw_LoRaMasterSlaveSel == 0) // LORA�ӻ�
		{
			if (Pw_ScrMenuMode == 0) //=0 DCM100 ������ģʽ	 2019.3.19
			{
				// ���Ҽ�
				if (Lw_PageNo == 0 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 5;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2019.3.19
				else if (Lw_PageNo == 5 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 10;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 10 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 17;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 17 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 101;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 101 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 103;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 103 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 108;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 108 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 113;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 113 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 117;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 117 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 121;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 121 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 125;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 125 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 129;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}

				else if (Lw_PageNo == 129 && B_RightKeyCount == 2) // ZCL 2018.8.21
				{
					Lw_PageNo = 135;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2019.8.31 133
				else if (Lw_PageNo == 135 && B_RightKeyCount == 2) // ZCL 2018.8.21 2019.8.31 133
				{
					Lw_PageNo = 200;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}

				else if (Lw_PageNo == 200 && B_RightKeyCount == 2) // ZCL 2018.8.28 =0������
				{
					Lw_PageNo = 205;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 205 && B_RightKeyCount == 2) // ZCL 2018.8.28 =0������
				{
					Lw_PageNo = 301;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 301 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 305;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28
				else if (Lw_PageNo == 305 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 309;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2019.4.15 310
				// ZCL 2019.4.15 2019.9.11
				else if (Lw_PageNo == 309 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 523;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 523 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 527;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 527 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 999;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}

				// �����
				else if (Lw_PageNo == 0 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 200;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 200 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 202;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 202 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 205;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 205 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 207;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 207 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 301;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 301 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 305;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				} // ZCL 2018.8.28 308
				else if (Lw_PageNo == 305 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 309;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2019.4.15 310
				// ZCL 2019.4.15 2019.9.11
				else if (Lw_PageNo == 309 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 523;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 523 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 527;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 527 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 999;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}

				//
				else if (Lw_PageNo == 101 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 301;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				} // ZCL 2018.8.29 ��Ƶ����ʱ�仭��
			}

			else if (Pw_ScrMenuMode == 1) //=1 DCM100 ��Ƶ��ģʽ	2019.3.19
			{
				// ���Ҽ�
				if (Lw_PageNo == 200 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 208;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 208 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 300;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308
				else if (Lw_PageNo == 300 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 301;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308
				else if (Lw_PageNo == 301 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 305;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308
				else if (Lw_PageNo == 305 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 309;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2019.4.15 310
				// ZCL 2019.4.15 2019.9.11
				else if (Lw_PageNo == 309 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 523;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 523 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 527;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 527 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 999;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}

				// ZCL 2018.8.28 ��Ƶ��ģʽ
				// �����
				else if (Lw_PageNo == 200 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 207;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 207 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 213;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 213 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 220;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 220 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 221;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 221 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 301;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 301 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 305;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 305 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 309;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2019.4.15 310
				// ZCL 2019.4.15 2019.9.11
				else if (Lw_PageNo == 309 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 523;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 523 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 527;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 527 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 999;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
			}

			else if (Pw_ScrMenuMode == 2) //=11 DCM220 ��Ƶ��ģʽ
			{
				// ���Ҽ�
				if (Lw_PageNo == 200 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 207;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 207 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 300;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308
				else if (Lw_PageNo == 300 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 301;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308
				else if (Lw_PageNo == 301 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 305;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308
				else if (Lw_PageNo == 305 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 309;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308��2019.9.11 310
				else if (Lw_PageNo == 309 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 310;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2019.4.15 310
				// ZCL 2019.4.15 2019.9.11
				else if (Lw_PageNo == 310 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 330;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308
				// ZCL 2019.3.23
				else if (Lw_PageNo == 330 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 523;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 523 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 527;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}
				else if (Lw_PageNo == 527 && B_RightKeyCount == 2)
				{
					Lw_PageNo = 999;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				}

				// ZCL 2018.8.28 ��Ƶ��ģʽ
				// �����
				else if (Lw_PageNo == 200 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 207;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 207 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 250;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 250 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 255;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 255 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 261;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}

				// ZCL 2019.4.16 ���Ӹ����޸Ĳ�������7������Һ������ֻ�����
				else if (Lw_PageNo == 261 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 300;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}

				else if (Lw_PageNo == 300 && B_LeftKeyCount == 2) // 261
				{
					Lw_PageNo = 301;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 301 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 305;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 305 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 309;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308
				else if (Lw_PageNo == 309 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 310;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				} // ZCL 2019.4.15 310
				// ZCL 2019.4.15 2019.9.10
				else if (Lw_PageNo == 310 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 330;
					B_RightKeyCount = 0;
					B_LeftKeyCount = 0;
				} // ZCL 2018.8.28 308
				// ZCL 2019.3.23
				else if (Lw_PageNo == 330 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 523;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 523 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 527;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
				else if (Lw_PageNo == 527 && B_LeftKeyCount == 2)
				{
					Lw_PageNo = 999;
					B_LeftKeyCount = 0;
					B_RightKeyCount = 0;
				}
			}
		}

		// ZCL 2019.3.14 ���� LORA������ʱ�򣬿�ݼ�
		else if (Pw_LoRaMasterSlaveSel == 1) // LORA����
		{
			// ���Ҽ� ZCL 2017.3.17
			if (Lw_PageNo == 500 && B_RightKeyCount == 2) // ZCL 2016.10.6  ��ʼ���水��ݼ����� ��Ƶ�趨����
			{
				Lw_PageNo = 523;
				B_RightKeyCount = 0;
				B_LeftKeyCount = 0;
			}
			else if (Lw_PageNo == 523 && B_RightKeyCount == 2)
			{
				Lw_PageNo = 524;
				B_RightKeyCount = 0;
				B_LeftKeyCount = 0;
			}
			else if (Lw_PageNo == 524 && B_RightKeyCount == 2)
			{
				Lw_PageNo = 527;
				B_RightKeyCount = 0;
				B_LeftKeyCount = 0;
			}
			else if (Lw_PageNo == 527 && B_RightKeyCount == 2)
			{
				Lw_PageNo = 531;
				B_RightKeyCount = 0;
				B_LeftKeyCount = 0;
			}
			else if (Lw_PageNo == 531 && B_RightKeyCount == 2)
			{
				Lw_PageNo = 999;
				B_RightKeyCount = 0;
				B_LeftKeyCount = 0;
			}

			// ����� ZCL 2017.3.17
			else if (Lw_PageNo == 500 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 501;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}
			else if (Lw_PageNo == 501 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 506;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}
			else if (Lw_PageNo == 506 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 511;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}
			else if (Lw_PageNo == 511 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 516;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}
			else if (Lw_PageNo == 516 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 523;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}
			else if (Lw_PageNo == 523 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 524;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}
			else if (Lw_PageNo == 524 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 527;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}
			else if (Lw_PageNo == 527 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 531;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}
			else if (Lw_PageNo == 531 && B_LeftKeyCount == 2)
			{
				Lw_PageNo = 999;
				B_LeftKeyCount = 0;
				B_RightKeyCount = 0;
			}
		}
	}

	// ֻ��һ�γ�ʱ�����ݼ�������
	if (T_KeyPressOverTime != SClk1Ms)
	{
		T_KeyPressOverTime = SClk1Ms;
		if (B_RightKeyCount > 0 || B_LeftKeyCount > 0)
		{
			C_KeyPressOverTime++;
			if (C_KeyPressOverTime > Pw_FastKeyBetweenMS)
			{
				C_KeyPressOverTime = 0;
				B_RightKeyCount = 0;
				B_LeftKeyCount = 0;
			}
		}
		else if (C_KeyPressOverTime > 0)
			C_KeyPressOverTime--;
	}

	// ZCL 2015.9.17 ��ʱˢ��ҳ�棬��ֹ��������
	// ZCL 2018.5.16 �� && S_ModParStatus==0  ���޸Ĳ���ʱ����ˢ��ҳ�棬��ֹ�޸�״̬�¡�>�� �����л�ҳ��
	if (Lw_SavePageNo == Lw_PageNo && S_ModParStatus == 0)
	{
		if (T_TimeUpdatePage != SClk10Ms)
		{
			T_TimeUpdatePage = SClk10Ms; //
			C_TimeUpdatePage++;
			if (C_TimeUpdatePage > 1500) // 15��
			{
				T_TimeUpdatePage = 100;
				C_TimeUpdatePage = 0;
				Lw_SavePageNo = 1000; // ZCL 2018.5.16 ���ԣ�ˢ��ҳ��Ҫ��ʱ������ڽ��ռ�����������ż����������
			}
		}
	}
	else
	{
		C_TimeUpdatePage = 0;
	}

	// ZCL 2019.3.8  =0 �ӻ���Ҫ��RS232�����Ƶ����
	//=2 ��һ���ٲ�=2 ���ϱ÷����������������ӻ�1 ��
	if (Pw_LoRaMasterSlaveSel == 0) //=0�ӻ�
	{
		// ZCL 2019.2.18 ÿ3S����ȡ��Ƶ����������1��
		if (B_TimeReadBPDJPar == 0) //|| B_TimeReadBPDJPar==2
		{
			if (T_TimeReadBPDJPar != SClk10Ms)
			{
				T_TimeReadBPDJPar = SClk10Ms;
				C_TimeReadBPDJPar++;
				if (C_TimeReadBPDJPar > 300)
				{
					C_TimeReadBPDJPar = 0;

					B_TimeReadBPDJPar = 1;
				}
			}
		}
	}
	else
	{
		C_TimeReadBPDJPar = 0;
		B_TimeReadBPDJPar = 0;
	}
}

/*********************����˵��ı�********************************/
void Menu_Change(void) // ����ҳ��ţ���ʾ��Ӧ�Ļ���
{
	if (Lw_PageNo <= 200 && (Pw_ScrMenuMode == 1 || Pw_ScrMenuMode == 2)) // ZCL 2018.8.28 =0����Ƶ��ģʽ
		Lw_PageNo = 200;												  // ZCL 2018.8.28 ��Ƶ��ģʽ�£���ͻ����200

	// 1. ע���޸�GetKey()�л����л��ļ�ֵ ZCL 2015.9.6
	// 2. ע��ʱ�仭��ļ�ֵ���壬TIMEPAGE
	switch (Lw_PageNo)
	{
	// 0. ARM������
	case 0:
		Menu_StmReadPar1();
		break; //
	case 1:
		Menu_StmReadPar2();
		break; //
	case 2:
		Menu_StmReadPar3();
		break; //
	case 3:
		Menu_StmReadPar4();
		break; //
	case 4:
		Menu_StmReadPar5();
		break; //
	case 5:
		Menu_StmReadPar6();
		break; //	��ˮ�汾
	case 6:
		Menu_StmReadPar7();
		break; //
	case 7:
		Menu_StmReadPar8_NoUse();
		break; // ZCL 2018.4.19 ��ʱ����ʾ�˻���
	case 8:
		Menu_StmReadPar9();
		break; //
	case 9:
		Menu_StmReadPar10();
		break; //
	case 10:
		Menu_StmReadPar11();
		break; //
	case 11:
		Menu_StmReadPar12();
		break; //
	case 12:
		Menu_StmReadPar13();
		break; //
	case 13:
		Menu_StmReadPar14();
		break; //
	case 14:
		Menu_StmReadPar15();
		break; //
	case 15:
		Menu_StmReadPar16();
		break; //
	case 16:
		Menu_StmReadPar17();
		break; //
	case 17:
		Menu_StmReadPar18();
		break; //
	case 18:
		Menu_StmReadPar19();
		break; // ZCL 2017.9.12 ����AI1-AI4��ֵ��������ԡ�
	case 19:
		Menu_StmReadPar20();
		break; // ZCL 2019.8.7	DI״̬��DO״̬
	case 20:
		Menu_StmReadPar21();
		break; // ZCL 2018.8.21	�ӵ綯��״̬

	// 1. ARMд����
	case 100:
		Menu_StmSetPar1();
		break; //
	case 101:
		Menu_StmSetPar2();
		break; // �޸Ĳ���
	case 102:
		Menu_StmSetPar3();
		break; //
	case 103:
		Menu_StmSetPar4();
		break; //
	case 104:
		Menu_StmSetPar5();
		break; //
	case 105:
		Menu_StmSetPar6();
		break; //
	case 106:
		Menu_StmSetPar7();
		break; //
	case 107:
		Menu_StmSetPar8();
		break; //
	case 108:
		Menu_StmSetPar9();
		break; //
	case 109:
		Menu_StmSetPar10();
		break; //
	case 110:
		Menu_StmSetPar11();
		break; //
	case 111:
		Menu_StmSetPar12();
		break; //
	case 112:
		Menu_StmSetPar13();
		break; //
	case 113:
		Menu_StmSetPar14();
		break; //
	case 114:
		Menu_StmSetPar15();
		break; //
	case 115:
		Menu_StmSetPar16();
		break; //
	case 116:
		Menu_StmSetPar17();
		break; //
	case 117:
		Menu_StmSetPar18();
		break; //
	case 118:
		Menu_StmSetPar19();
		break; //
	case 119:
		Menu_StmSetPar20();
		break; //
	case 120:
		Menu_StmSetPar21();
		break; //
	case 121:
		Menu_StmSetPar22();
		break; //
	case 122:
		Menu_StmSetPar23();
		break; //
	case 123:
		Menu_StmSetPar24();
		break; //
	case 124:
		Menu_StmSetPar25();
		break; //
	case 125:
		Menu_StmSetPar26();
		break; //

	case 126:
		Menu_StmSetPar27();
		break; //	ZCL 2018.8.21	��˫ˮԴ�͵綯������
	case 127:
		Menu_StmSetPar28();
		break; //
	case 128:
		Menu_StmSetPar29();
		break; //
	case 129:
		Menu_StmSetPar30();
		break; //
	case 130:
		Menu_StmSetPar31();
		break; //
	case 131:
		Menu_StmSetPar32();
		break; //
	case 132:
		Menu_StmSetPar33();
		break; //	ZCL 2018.8.21	��˫ˮԴ�͵綯������
	case 133:
		Menu_StmSetPar34();
		break; //	ZCL 2018.8.22	��˫ˮԴ�͵綯������

	case 134:
		Menu_StmSetPar35();
		break; //	ZCL 2019.8.30 ���ⶨʱͣ��
	case 135:
		Menu_StmSetPar36();
		break; //	ZCL 2019.8.31 �豸���͡���ˮ��Ƶģʽ

	// 2. DSP������
	case 199:
		Menu_DspReadPar0();
		break; //	ZCL 2018.8.28 ������ģʽʱʹ�ã���Ƶģʽʱ����Ҫ����Ƶ�ۿ�������

	case 200:
		Menu_DspReadPar1();
		break; //
	case 201:
		Menu_DspReadPar2();
		break; //
	case 202:
		Menu_DspReadPar3();
		break; //
	case 203:
		Menu_DspReadPar4();
		break; //
	case 204:
		Menu_DspReadPar5();
		break; //

	case 205:
		if (Pw_ScrMenuMode == 0) // ZCL 2018.8.28 =0��������ģʽ
			// Menu_StmReadPar4();
			Menu_DspReadPar6();								 // ZCL 2018.8.28 2019.9.6 ���ɴ˻���
		else if (Pw_ScrMenuMode == 1 || Pw_ScrMenuMode == 2) // ZCL 2018.8.28 =1����Ƶ��ģʽ
			Menu_DspReadPar6();								 // ZCL 2018.8.28
		break;

	case 206:
		Menu_DspReadPar7();
		break; //
	case 207:
		Menu_DspReadPar8();
		break; //

	// ZCL 2018.8.28 DCM100��Ƶģʽʱʹ�á�
	//  ��ע��DCM100������ģʽʱ������Ҫ 208-221���棬��Ϊ��Menu_StmReadPar���С�
	case 208:
		Menu_StmReadPar6();
		break; //	��ˮ�汾
	case 209:
		Menu_StmReadPar7();
		break; //
	case 210:
		Menu_StmReadPar8_NoUse();
		break; //
	case 211:
		Menu_StmReadPar9();
		break; //
	case 212:
		Menu_StmReadPar10();
		break; //

	case 213:
		Menu_StmReadPar11();
		break; // ZCL 2018.8.28 ������ι��ϼ�¼ 1	DCM100��Ƶģʽʱʹ�á�
	case 214:
		Menu_StmReadPar12();
		break; //
	case 215:
		Menu_StmReadPar13();
		break; //
	case 216:
		Menu_StmReadPar14();
		break; //
	case 217:
		Menu_StmReadPar15();
		break; //
	case 218:
		Menu_StmReadPar16();
		break; //
	case 219:
		Menu_StmReadPar17();
		break; // ѡ����ϼ�¼
	case 220:
		Menu_StmReadPar18();
		break; // ʱ��
	case 221:
		Menu_StmReadPar19();
		break; // ZCL 2018.8.28 ����AI1-AI4��ֵ��������ԡ�

	//----------------ZCL 2019.3.18 DCM220 ��Ƶ��ģʽ
	case 250:
		Menu_DspReadPar29();
		break; //
	case 251:
		Menu_DspReadPar30();
		break; //
	case 252:
		Menu_DspReadPar31();
		break; //
	case 253:
		Menu_DspReadPar32();
		break; //
	case 254:
		Menu_DspReadPar33();
		break; //
	case 255:
		Menu_DspReadPar34();
		break; // ZCL 2018.8.28 ������ι��ϼ�¼ 1	DCM220��Ƶģʽʱʹ�á�
	case 256:
		Menu_DspReadPar35();
		break; //
	case 257:
		Menu_DspReadPar36();
		break; //
	case 258:
		Menu_DspReadPar37();
		break; //
	case 259:
		Menu_DspReadPar38();
		break; //
	case 260:
		Menu_DspReadPar39();
		break; //
	case 261:
		Menu_DspReadPar40();
		break; // ѡ����ϼ�¼

	// case 262: Menu_GpsReadPar1();  break;		// 			//ZCL 2017.6.5 ��γ��ҳ

	// 3. DSPд����
	case 300:
		if (Pw_ScrMenuMode == 0)	  // ZCL 2018.8.28 =0��������ģʽ
			Menu_DspSetPar1();		  // ��Ƶ�趨����
		else if (Pw_ScrMenuMode == 1) // ZCL 2018.8.28 =1����Ƶ��ģʽ
			Menu_StmSetPar2();		  // �޸Ĳ���
		else if (Pw_ScrMenuMode == 2) // ZCL 2019.3.18 =11��DCM220��Ƶ��ģʽ
			Menu_DspSetPar50();		  // �޸Ĳ���
		break;						  // �޸Ĳ�������

	case 301:
		Menu_DspSetPar2();
		break; // ����ʱ��
	case 302:
		Menu_DspSetPar3();
		break; //
	case 303:
		Menu_DspSetPar4();
		break; //
	case 304:
		Menu_DspSetPar5();
		break; //
	case 305:
		Menu_DspSetPar6();
		break; //
	case 306:
		Menu_DspSetPar7();
		break; //
	case 307:
		Menu_DspSetPar8();
		break; //
	case 308:
		Menu_DspSetPar9();
		break; //	ZCL 2018.8.28

	case 309:
		Menu_DspSetPar10();
		break; //
	case 310:
		if (Pw_ScrMenuMode == 0)	  // ZCL 2018.8.28 =0��������ģʽ
			Menu_StmSetPar24();		  // ��Ƶģʽ
		else if (Pw_ScrMenuMode == 1) // ZCL 2018.8.28 =1����Ƶ��ģʽ
			Menu_StmSetPar24();		  // ��Ƶģʽ
		else if (Pw_ScrMenuMode == 2) // ZCL 2019.3.18 =11��DCM220��Ƶ��ģʽ
			Menu_DspSetPar30();		  // ��Ƶģʽ
		break;
	// ���� 543-5**����		ZCL 2019.3.23

	//----------------ZCL 2019.3.18 DCM220 ��Ƶ��ģʽ�����2��
	case 330:
		Menu_DspSetPar31();
		break; //
	case 331:
		Menu_DspSetPar32();
		break; //
	case 332:
		Menu_DspSetPar33();
		break; //	ZCL 2020.4.8
	// ���� 523-5**����		ZCL 2019.3.23

	// 5. LORA������ʾ�ӻ��Ĳ���	ZCL 2019.3.8
	case 500:
		Menu_GpsReadPar1(); // ZCL 2017.6.5 ��γ��ҳ
		break;
	case 501:
		Menu_Dsp1ReadPar1();
		break; //
	case 502:
		if (Pw_EquipmentType == 0) // ˫����
		{
			Menu_Dsp1ReadPar2();
		}
		else // ��Ƶ���
		{
			Menu_Dsp1ReadPar2_BK();
		}
		break; //
	case 503:
		if (Pw_EquipmentType == 0) // ˫����
		{
			Menu_Dsp1ReadPar3();
		}
		else // ��Ƶ���
		{
			Menu_Dsp1ReadPar3_BK();
		}
		break; //
	case 504:
		if (Pw_EquipmentType == 0) // ˫����
		{
			Menu_Dsp1ReadPar4();
		}
		else // ��Ƶ���
		{
			Menu_Dsp1ReadPar4_BK();
		}
		break; //	ZCL 2019.9.12
	case 505:
		Menu_Dsp1ReadPar5();
		break; //	ZCL 2019.9.12

	case 506:
		Menu_Dsp2ReadPar1();
		break; //
	case 507:
		Menu_Dsp2ReadPar2();
		break; //
	case 508:
		Menu_Dsp2ReadPar3();
		break; //
	case 509:
		Menu_Dsp2ReadPar4();
		break; //	ZCL 2019.9.12
	case 510:
		Menu_Dsp2ReadPar5();
		break; //	ZCL 2019.9.12

	case 511:
		Menu_Dsp3ReadPar1();
		break; //
	case 512:
		Menu_Dsp3ReadPar2();
		break; //
	case 513:
		Menu_Dsp3ReadPar3();
		break; //
	case 514:
		Menu_Dsp3ReadPar4();
		break; //	ZCL 2019.9.12
	case 515:
		Menu_Dsp3ReadPar5();
		break; //	ZCL 2019.9.12

	case 516:
		Menu_Dsp4ReadPar1();
		break; //
	case 517:
		Menu_Dsp4ReadPar2();
		break; //
	case 518:
		Menu_Dsp4ReadPar3();
		break; //
	case 519:
		Menu_Dsp4ReadPar4();
		break; //	ZCL 2019.9.12
	case 520:
		Menu_Dsp4ReadPar5();
		break; //	ZCL 2019.9.12

	// ZCL 2019.10.18
	case 521:
		Menu_Dsp5ReadPar1();
		break; //
	case 522:
		Menu_Dsp5ReadPar2();
		break; //
	case 523:
		if (Pw_EquipmentType == 0) // ˫����
		{
			Menu_Dsp5ReadPar3();
		}
		else // ��Ƶ���
		{
			Menu_LoRaMasteSetPar1();
		}
		break; //
	case 524:
		if (Pw_EquipmentType == 0) // ˫����
		{
			Menu_Dsp5ReadPar4();
		}
		else // ��Ƶ���
		{
			Menu_LoRaSetPar1();
		}
		break; //	ZCL 2019.9.12
	case 525:
		if (Pw_EquipmentType == 0) // ˫����
		{
			Menu_Dsp5ReadPar5();
		}
		else // ��Ƶ���
		{
			Menu_LoRaSetPar2();
		}
		break; //	ZCL 2019.9.12

	case 526:
		if (Pw_EquipmentType == 0) // ˫����
		{
			Menu_Dsp6ReadPar1();
		}
		else // ��Ƶ���
		{
			Menu_LoRaSetPar3();
		}
		break; //
	case 527:
		if (Pw_EquipmentType == 0) // ˫����
		{
			Menu_Dsp6ReadPar2();
		}
		else // ��Ƶ���
		{
			Menu_LoRaSetPar4();
		}
		break; //
	case 528:
		if (Pw_EquipmentType == 0) // ˫����
		{
			Menu_Dsp6ReadPar3();
		}
		else // ��Ƶ���
		{
			Menu_LoRaSetPar5();
		}
		break; //
	case 529:
		if (Pw_EquipmentType == 0) // ˫����
		{
			Menu_Dsp6ReadPar4();
		}
		else // ��Ƶ���
		{
			Menu_LoRaSetPar6();
		}
		break; //	ZCL 2019.9.12
	case 530:
		if (Pw_EquipmentType == 0) // ˫����
		{
			Menu_Dsp6ReadPar5();
		}
		else // ��Ƶ���
		{
			Menu_LoRaSetPar7();
		}
		break; //	ZCL 2019.9.12

	case 531:
		if (Pw_EquipmentType == 0) // ˫����
		{
			Menu_Dsp7ReadPar1();
		}
		else // ��Ƶ���
		{
			Menu_GprsReadPar1();
		}
		break; //
	case 532:
		if (Pw_EquipmentType == 0) // ˫����
		{
			Menu_Dsp7ReadPar2();
		}
		else // ��Ƶ���
		{
			Menu_GprsReadPar2();
		}
		break; //
	case 533:
		if (Pw_EquipmentType == 0) // ˫����
		{
			Menu_Dsp7ReadPar3();
		}
		else // ��Ƶ���
		{
			Menu_GprsReadPar3();
		}
		break; //
	case 534:
		if (Pw_EquipmentType == 0) // ˫����
		{
			Menu_Dsp7ReadPar4();
		}
		else // ��Ƶ���
		{
			Menu_GprsReadPar4();
		}
		break; //	ZCL 2019.9.12
	case 535:
		Menu_Dsp7ReadPar5();
		break; //	ZCL 2019.9.12

	case 536:
		Menu_Dsp8ReadPar1();
		break; //
	case 537:
		Menu_Dsp8ReadPar2();
		break; //
	case 538:
		Menu_Dsp8ReadPar3();
		break; //
	case 539:
		Menu_Dsp8ReadPar4();
		break; //	ZCL 2019.9.12
	case 540:
		Menu_Dsp8ReadPar5();
		break; //	ZCL 2019.9.12

	case 543:
		Menu_LoRaMasteSetPar1();
		break; //

	case 544:
		Menu_LoRaSetPar1();
		break; // ZCL 2018.11.13 LoRa����
	case 545:
		Menu_LoRaSetPar2();
		break; //
	case 546:
		Menu_LoRaSetPar3();
		break; //
	case 547:
		Menu_LoRaSetPar4();
		break; //
	case 548:
		Menu_LoRaSetPar5();
		break; // ZCL 2019.3.22 LoRa��Ƶ����
	case 549:
		Menu_LoRaSetPar6();
		break; //

	case 550:
		Menu_LoRaSetPar7();
		break; // ZCL 2019.9.24

	case 551:
		Menu_GprsReadPar1();
		break; //
	case 552:
		Menu_GprsReadPar2();
		break; //
	case 553:
		Menu_GprsReadPar3();
		break; //
	case 554:
		Menu_GprsReadPar4();
		break; //

	// ZCL 2019.10.19 ���ӿ��ԸĶ� YW310 SZM220����
	case 555:
		Menu_YW310Par1();
		break; //
	case 556:
		Menu_YW310Par2();
		break; //
	case 557:
		Menu_YW310Par3();
		break; //

	case 558:
		Menu_YW310Par4();
		break; //		ZCL 2019.10.21

	case 559:
		Menu_SZM220Par1();
		break; //
	case 560:
		Menu_SZM220Par2();
		break; //
	case 561:
		Menu_SZM220Par3();
		break; //
	case 562:
		Menu_SZM220Par4();
		break; //
	case 563:
		Menu_SZM220Par5();
		break; //
	case 564:
		Menu_SZM220Par6();
		break; //
	case 565:
		Menu_SZM220Par7();
		break; //

	case 999:
		Menu_END();
		break; //

	default:
		return;
	}
}
