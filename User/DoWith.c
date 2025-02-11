/**
  ******************************************************************************
  * @file    DoWith.c
  * @author  ChengLei Zhou  - �ܳ���
  * @version V1.27
  * @date    2014-01-03
  * @brief   �����������⣬���������,ģ���������⣬ģ�������,������������
	******************************************************************************
  * @attention	2014.1.3
	1.UID���������: 2013.10.26
		�ڼ��ܹ�ʽ��,���������������
			uint8_t D[12];//D[12]Ϊ��ʽ����������(ע����ID[12]����).
			uint32_t Fml_Constant; // �û�ָ����32λ����
		���ܹ�ʽ�����Ϊһ������:
			uint8_t Result[4]; // ��ʽ���������,�ĸ��ֽ�
		����Ҫ��оƬ��ȫ��ΨһIDֵ:
			uint8_t* UID=(uint8_t*)ID����ʼ��ַ;
			�Ժ�ֻҪ�������þͿ�����.����UID[0]����оƬ����ֽڵ�ȫ��ΨһID.

		u32 Fml_Constant; 	//���뵽��ʽ�ĳ���
		u8 *C= (u8*)&Fml_Constant;	//�ѳ���ת��Ϊ����
		u16 Fml_CRC16;
	******************************************************************************
	*/

/* Includes ------------------------------------------------------------------*/
#include "GlobalConst.h"
#include "GlobalV_Extern.h" // ȫ�ֱ�������
#include "DoWith.h"
#include "spi_flash.h"
#include "com2_232.h" //ZCL 2019.3.29

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Init Structure definition */

/* Private variables ---------------------------------------------------------*/
//
uchar T_BootParLst; // ��ʼ����������
uint C_BootParLst;
uchar S_BootParLst;
uchar T_SavePar; // �������
uint C_SavePar;
uchar S_SavePar;
uchar T_ForceSavPar; // ǿ�Ʊ������
uchar B_ForceSavPar; // ǿ�Ʊ��������־
uint C_ForceSavPar;	 // ǿ�Ʊ������ ������
//
uint C_PowenOnDelay; // �ϵ���ʱ��,���������жϻ��߶���
uint C_LedLight;	 // LED��˸��ʱ������

uchar T_DataLampAllLight; // ����DATA������ 2016.5.27 ZCL
uint C_DataLampAllLight;  // ����DATA������ ������

uint C_NetLedLightCSQ; // Net LED��˸CSQָʾ������ ZCL 2017.5.25

uchar T_Com2RcvReset; // COM2 ���ո�λ��ʱ�� ZCL 2017.5.26
uint C_Com2RcvReset;  // COM2 ���ո�λ������ ZCL 2017.5.26

uchar T_Com2NoRcv; // COM2 û�н��ռ����� ZCL 2017.5.30
uint C_Com2NoRcv;  // COM2 û�н��ռ����� ZCL 2017.5.30

uchar T_Com3NoRcv; // COM3 û�н����������ݼ����� ZCL 2017.5.30
uint C_Com3NoRcv;  // COM3 û�н����������ݼ����� ZCL 2017.5.30

u8 T_LCDBackLight; // ZCL 2017.9.19 �������  2018.12.10
u16 C_LCDBackLight;
u32 w32_SecCouter;

u16 T_Reboot; // ��ʱ����������
u16 C_Reboot;
union
{
	float f;
	u8 b[4];
} aa; // ZCL 2019.10.18
/* Private variables extern --------------------------------------------------*/
extern uc8 Gprs_ParBootLst;
extern uint Cw_Txd3;
extern uint Cw_Txd2;
extern uchar S_M35;
extern uchar CGD0_LinkConnectOK[]; // ����0���ӱ�־
extern uint Cw_Rcv3;			   // 2014.11.27
extern uint Cw_Rcv2;			   // 2014.11.27

extern uchar B_Com3Cmd8B; // ��ѯDTU�������� 2012.8.17
extern uchar B_Com3Cmd8D; // ����DTU�������� 2012.8.24
extern uchar B_Com3Cmd89; // ��������������ָ�� 2011.8.24
extern uchar B_OtherCmd;  // ��λ��ͨ��DTU������2������ָ��跢�͵�����2��

extern uchar S_SendSms;	   // ���Ͷ��Ų���
extern uchar S_ReadNewSms; // ���¶��Ų���
extern uchar S_SmsFull;	   // ����������

extern uchar B_GprsDataReturn;	 // GPRS ���ݷ��ء������ӣ��յ�����2���ݣ���ͨ��DTU���ͳ�ȥ
extern uchar B_HeartSendFault[]; // �������������ϱ�־
extern uchar B_LinkFault[];		 // �������ӹ��ϱ�־
extern uchar B_ATComStatus;		 // AT����״̬ 2017.5.31
extern uchar ZhuCeOkFLAG[];
extern uchar B_SmsOrFaultHardRestart; // ���Ż��߹���Ӳ������
/* Private function prototypes -----------------------------------------------*/
void Com0_config(void);
void Com1_config(void);
uint CRC16(uchar *pCrcData, uchar CrcDataLen);
void LCD_DLY_ms(u32 Time); // ZCL 2018.12.10

uc16 w_GprsParBootLst[50] = {
	60000, 0, 2, 1, 3,	   // 0-4		0=�������ʱ�� ����,	1=����̨��Ϣ,	2=LORA�豸ͨѶ��ַ
						   //			3=�����ӻ�ѡ��,4=LORAģ��1��ַ
	4, 0, 0, 0, 0,		   // 5-9		5=LORAģ��2��ַ,6=LORAģ��3��ַ
						   //			7=LORAģ��4��ַ 8=�豸����0=˫���ã�1=��Ƶ���	9=�豸���ʣ����ã�
	0, 2, 0, 0, 0,		   // 10-14		10=γ��1 11=γ��2 12=����1
						   //			13=����2 14=LORAģ��5��ַ
	0, 1, 9, 0, 0,		   // 15-19		15=LORAģ��6��ַ 16=LoRa�趨��1=����LoRa 17=Gprs �趨��9=����GPRS
						   //			18=�������2 19=�������3
	6, 0, 0, 0, 0,		   // 20-24		20=Һ��������ģʽ 21=�˵�ģʽ 22=����С��
						   //			23=LORAģ��7��ַ 24=LORAģ��8��ַ
	0, 0, 0, 0, 0,		   // 25-29		25=�� 26=�� 27=ʱ
						   //			28=�� 29=����
	57600, 9600, 3, 0, 0,  // 30-34		30=����3������ 31=����2������ 32=����2 ����λ����
						   //			33=����2 ֹͣλ���� 34=����2 У��λ����
						   //		 	3,  435,   20,    9,    7,	// 35-39	35=����2 �������� 36= 37=		ZCL 2019.9.7
	3, 435, 20, 9, 7,	   // 35-39		35=����2 �������� 36=Lora Ƶ�� 37=  ���书��
						   //			38=�źŴ���BW 	39= ��Ƶ����SF
	2, 1, 0, 0, 0,		   // 40-44		40= ErrorCoding [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] 41=CrcOn 42=ImplicitHeaderOn
						   //			43=RxSingleOn 44=FreqHopOn  	//ZCL 2020.3.27 43��RxSingleOn�ĳ�0
	4, 128, 1000, 1000, 0, // 45-49		45=HopPeriod 46=Pw_LoRaSetPayLoadLength  47=Pw_LoRaSetTxPacketTimeOut
						   //			48=Pw_LoRaSetRxPacketTimeOut 49=����Э��
}; // 0,   1,    2,    3,    4,	//��������
   // 5,   6,    7,    8,    9,	//��������

/* Private functions ---------------------------------------------------------*/

void ADC_DMA_Init(void)
{
	;
	// ZCL 2021.5.4  ��ST��Ļ�����Ҫ��ADC_CMD�����20us���ҵ���ʱ
	//...
	// Delay_US(30);					//ZCL 2021.10.26

#ifdef GD32

#else

#endif
}

u16 FilterAI(u8 SelChannel) // ����ģ����ֵ
{
	u16 i;
	return i;
}

void AnologIn(void)
{
	;
}

void Variable_Init(void) //	������ʼ��
{
	// ZCL 2018.8.3  w_Scr Һ�����Ĳ������飬ParType:4
	w_ScrVERSION = 530; // ����汾�� VERSION	// 2017.9.13 -3 2018.7.17

	w_ScrWriteYear = 2025; // �����д��
	w_ScrWriteDate = 205;  // �����д����

	w_GprsSoftVer = 109;		// ����汾�� VERSION
	C_PowenOnDelay = 0;			// �ϵ���ʱ��,���������жϻ��߶���
	Pw_FastKeyBetweenMS = 1000; // ��������MS
	//
	Lw_SaveEquipOperateStatus = 1000; // ZCL 2015.9.7
	Lw_SaveEquipAlarmStatus = 1000;
	Lw_SaveDspStopStatus = 1000; // ZCL 2015.9.7
	Lw_SaveDspFaultStatus = 1000;
	//
	C_LCDBackLight = 1; // ZCL 2017.9.13 =1��ʼ��ʱ������Ʊ�����
	// LoRaͨѶ����������
	w_Lora1Counter2 = 0;
	w_Lora2Counter2 = 0;
	w_Lora3Counter2 = 0;
	w_Lora4Counter2 = 0;

	F_Reset_GPS = 0; // ����GPS��־

	F_No1_VVVFComm_Fault = 0; // ���Ƶ��ͨѶ����
	F_No2_VVVFComm_Fault = 0; // ���Ƶ��ͨѶ����
	F_No3_VVVFComm_Fault = 0; // ���Ƶ��ͨѶ����
	F_No4_VVVFComm_Fault = 0; // ���Ƶ��ͨѶ����
	F_No5_VVVFComm_Fault = 0; // ���Ƶ��ͨѶ����
	F_No6_VVVFComm_Fault = 0; // ���Ƶ��ͨѶ����
	F_No7_VVVFComm_Fault = 0; // ���Ƶ��ͨѶ����
	F_No8_VVVFComm_Fault = 0; // ���Ƶ��ͨѶ����
}

// ZCL 2018.12.11 ��ע����ǰҺ������Ҫ��������Ҫ��Ϊ��GPRS,LOAR,GPS,���� �Ȳ����ˡ�
void ParLst_Init(void) // RAM�в������� ��ʼ�� (����)
{
	// ZCL 2019.4.4 ����Һ�����趨���� �� FM25L16
	SPI_FLASH_CS_LOW();
	SPI_FMRAM_BufferRead((u8 *)(&w_GprsParLst), 0, SCR_PAR_SIZE * 2); // FMADD:0
	// ZCL 2019.4.4  ����GPRS(DTU)�趨���� �� FM25L16
	SPI_FLASH_CS_LOW();
	SPI_FMRAM_BufferRead(GprsPar, 256, DTU_PAR_SIZE); // FMADD:256

	// 2009.12.4  �Ѳ����ʶ����󱣴棬Ϊ���иı�ʱ���޸Ĳ����ʡ�
	w_GprsSaveBaudRate2 = Pw_GprsBaudRate2;

	// ZCL 2019.3.22 ����LORA��Ƶ����
	LoRaSettings.RFFrequency = (uint32_t)Pw_LoRaSetFreq * 1000000;
	LoRaSettings.Power = Pw_LoRaSetPower;
	LoRaSettings.SignalBw = Pw_LoRaSetSignalBW; // LORA [0: 7.8 kHz, 1: 10.4 kHz, 2: 15.6 kHz,
	// 3: 20.8 kHz, 4: 31.2 kHz,5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved]
	LoRaSettings.SpreadingFactor = Pw_LoRaSetSpreadingFactor;	// LORA [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
	LoRaSettings.ErrorCoding = Pw_LoRaSetErrorCoding;			// LORA [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
	LoRaSettings.CrcOn = Pw_LoRaSetCrcOn;						// [0: OFF, 1: ON]
	LoRaSettings.ImplicitHeaderOn = Pw_LoRaSetImplicitHeaderOn; // [0: OFF, 1: ON]
	LoRaSettings.RxSingleOn = Pw_LoRaSetRxSingleOn;				// [0: Continuous, 1 Single]
	LoRaSettings.FreqHopOn = Pw_LoRaSetFreqHopOn;				// [0: OFF, 1: ON]
	LoRaSettings.HopPeriod = Pw_LoRaSetHopPeriod;				// Hops every frequency hopping period symbols

	LoRaSettings.TxPacketTimeout = Pw_LoRaSetTxPacketTimeOut; // ZCL 2019.9.24 �����¼���
	LoRaSettings.RxPacketTimeout = Pw_LoRaSetRxPacketTimeOut; // ZCL 2019.9.24 �����¼���

	LoRaSettings.PayloadLength = Pw_LoRaSetPayLoadLength;

	// ZCL 2019.9.12 �ϵ�RAM�в������� ��ʼ��ʱ����ֵ����ľ�γ��
	w_ScrGpsLatAllDu1 = Pw_ScrGpsLatAllDu1;
	w_ScrGpsLatAllDu2 = Pw_ScrGpsLatAllDu2;

	w_ScrGpsLonAllDu1 = Pw_ScrGpsLonAllDu1;
	w_ScrGpsLonAllDu2 = Pw_ScrGpsLonAllDu2;
}

// w_GprsWriteSize: �ֵ�����
void ParArrayWrite(u16 *p_Top, u16 *p_Base, uint w_GprsWriteSize)
{
	uint i, j;

	// FLASH_PAGE_SIZE=2048��0x800��		�ͣ����ܶȵ�STM32��1024

	// ���FLASH ��̺Ͳ�������
	FLASH_Unlock();

	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

	// EraseCounter ����������
	FLASH_ErasePage((u32)p_Top);

	// ��ָ����������д��ӿ�ʼ��ַ��������ַ��FLASH
	for (i = 0; i < w_GprsWriteSize; i++) // �ַ�������
	{
		j = *p_Base++;
		FLASH_ProgramHalfWord((u32)p_Top++, j);
	}

	// �ܳ��ڼ��� 2013.1.22 ���Ӧ����Ҫ���� ������̺Ͳ������� ��
	FLASH_Lock();
}

// w_GprsReadSize: �ֵ�����
void ParArrayRead(uint *p_Top, uc16 *p_Base, uint w_GprsReadSize)
{
	uint i;
	for (i = 0; i < w_GprsReadSize; i++)
	{
		*p_Top++ = *p_Base++; // zcl ��ַ�Զ���2��
	}
}

// w_GprsReadSize: �ֵ�����
void GprsArrayRead(uchar *p_Top, uc8 *p_Base, uint w_GprsReadSize)
{
	uint i, j;
	for (i = 0; i < w_GprsReadSize; i++)
	{
		j = *p_Base++; // zcl ��ַ�Զ���2��
		*p_Top++ = j;  // ���ֽ�
					   //*p_Top++ = (j>>8);		//���ֽ�
	}
}

// ZCL 2018.12.11 ��ע����ǰҺ������Ҫ��������Ҫ��Ϊ��GPRS,LOAR,GPS,���� �Ȳ����ˡ�
void Boot_ParLst(void) // ��ʼ���趨����
{
	uchar i = 0;

	u16 nw_i, nw_j, nw_k, nw_l; // ZCL 2019.9.11

	// w_GprsModPar=2000,�����޸Ĳ���	// w_GprsParInitial==4321 ��ʼ������
	if (w_GprsModPar == 2000 && w_GprsParInitial == 4321 && S_BootParLst == 0) // ZCL 2019.3.12
	{
		i = 1;
	}
	else if (Pw_LoRaMasterSlaveSel > 2000 && Pw_LoRaEquipmentNo > 2000 && S_BootParLst == 0)
	{
		i = 2;
		B_ForceSavPar = 1; // 2013.10.1
	}
	else if (Pw_LoRaMasterSlaveSel == 0 && Pw_LoRaEquipmentNo == 0 && // 2013.5.22 ��ʼ��
			 Pw_LoRaSetSpreadingFactor == 0 && Pw_LoRaSetErrorCoding == 0 && S_BootParLst == 0)
	{
		i = 3;
		B_ForceSavPar = 1; // 2013.10.1
	}
	// ZCL 2018.12.22
	else if (Pw_GprsBaudRate2 > 57600 && S_BootParLst == 0)
	{
		i = 4;
		B_ForceSavPar = 1; // 2013.10.1
	}

	if (i > 0)
	{

		// ZCL 2016.5.26 ȡ��Gprs_ParBootLst�����Ķ���Gprs�������ٳ�ʼ��
		// 2016.5.24 dtuid�ţ���ʼ����ʱ���޸�DTU�Ź��ܡ� ����DTU��
		uchar m, DtuID[15];
		for (m = 0; m < 11; m++)
			DtuID[m] = GprsPar[DtuNoBase + m];

		// 2012.7.23 ����GPRS��ʼ������ ZCL 2019.4.4  �ĵ���(uc16 *)(&Gprs_ParBootLst)
		// ZCL 2020.4.5 ȡ����ʼ��GPRS���� OK
		GprsArrayRead(GprsPar, &Gprs_ParBootLst, DTU_PAR_SIZE); // GPRS��ʼ������

		// 2016.5.24 dtuid�ţ���ʼ����ʱ���޸�DTU�š��ָ�DTU��
		for (m = 0; m < 11; m++)
			GprsPar[DtuNoBase + m] = DtuID[m];
		// ZCL 2017.5.27 ���������Ե�ʱ�򣬴򿪣�

		// ZCL 2019.9.11 �⼸�����ڹ�ˮ�����SCR�����������г�ʼ����ֻ���ֶ��޸ġ�
		nw_i = Pw_ScrKeyMode;
		nw_j = Pw_ScrMenuMode;
		nw_k = Pw_ScrCurrentBits;
		nw_l = Pw_ScrEquipPower;

		ParArrayRead(w_GprsParLst, w_GprsParBootLst, SCR_PAR_SIZE); // ������ʼ������

		Pw_ScrKeyMode = nw_i;
		Pw_ScrMenuMode = nw_j;
		Pw_ScrCurrentBits = nw_k;
		Pw_ScrEquipPower = nw_l;

		// ZCL 2019.3.22  ��ֵ�����ã�
		/* 		Pw_LoRaSetFreq=LoRaSettings.RFFrequency/1000000;
				Pw_LoRaSetPower=LoRaSettings.Power;
				Pw_LoRaSetSignalBW=LoRaSettings.SignalBw;     // LORA [0: 7.8 kHz, 1: 10.4 kHz, 2: 15.6 kHz,
				// 3: 20.8 kHz, 4: 31.2 kHz,5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved]
				Pw_LoRaSetSpreadingFactor=LoRaSettings.SpreadingFactor;  // LORA [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
				Pw_LoRaSetErrorCoding=LoRaSettings.ErrorCoding;      // LORA [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
				Pw_LoRaSetCrcOn=LoRaSettings.CrcOn;                         // [0: OFF, 1: ON]
				Pw_LoRaSetImplicitHeaderOn=LoRaSettings.ImplicitHeaderOn;   // [0: OFF, 1: ON]
				Pw_LoRaSetRxSingleOn=LoRaSettings.RxSingleOn;               // [0: Continuous, 1 Single]
				Pw_LoRaSetFreqHopOn=LoRaSettings.FreqHopOn;                 // [0: OFF, 1: ON]
				Pw_LoRaSetHopPeriod=LoRaSettings.HopPeriod;                 // Hops every frequency hopping period symbols

				Pw_LoRaSetTxPacketTimeOut=LoRaSettings.TxPacketTimeout;			//ZCL 2019.9.24 �����¼���
				Pw_LoRaSetRxPacketTimeOut=LoRaSettings.RxPacketTimeout;			//ZCL 2019.9.24 �����¼���

				Pw_LoRaSetPayLoadLength=LoRaSettings.PayloadLength;
		 */

		// ZCL 2016.5.26 �����������ParArrayReadһ�����Ͷ����ָ��ɳ�ʼֵ��
		if (i == 1)
			Pw_GprsSetTest = 991; // ZCL 2019.3.22
		else if (i == 2)
			Pw_GprsSetTest = 992; // ZCL 2016.5.26 ���ϵ���һ����־
		else if (i == 3)
			Pw_GprsSetTest = 993; // ZCL 2016.5.26 ���ϵ���һ����־
		i = 0;

		// �绰���벻��ʼ��
		// �ݲ������������w_GprsModPar==5000�����Ա��������FLASH
		if (B_ForceSavPar == 0)
		{
			w_GprsModPar = 2000;
			w_GprsParInitial = 4321;
			S_BootParLst = 1;
		}
		else
		{
			S_SavePar = 0;
			w_GprsParInitial = 1000; // 2013.10.12 ��1000�����ϵ��ʼ����ǿ�Ʊ���Ĳ���
			SavePar_Prompt();		 // 2013.10.12 �Զ���ʼ������������
		}
	}

	//
	if (T_BootParLst != SClkSecond && S_BootParLst != 0) // ����MD304L��ʾ״̬�����Ի���� ZCL
	{
		T_BootParLst = SClkSecond; // ����w_GprsParInitial=4321
		C_BootParLst++;
		if (C_BootParLst > 2 && S_BootParLst == 1)
		{
			S_BootParLst = 2;
			w_GprsParInitial = 6000;
		}
		else if (C_BootParLst > 4 && S_BootParLst == 2)
		{
			T_BootParLst = 100;
			C_BootParLst = 0;
			S_BootParLst = 0;
			w_GprsParInitial = 0;
		}
	}
}

// ZCL 2018.12.11 ��ע����ǰҺ������Ҫ��������Ҫ��Ϊ��GPRS,LOAR,GPS,���� �Ȳ����ˡ�
void SavePar_Prompt(void) // �������+״̬��ʾ
{
	// uchar i=0;

	if (B_ForceSavPar == 1 && S_SavePar == 0) // ǿ�Ʊ������
	{
		w_GprsModPar = 0; // ��ֹ�� w_GprsModPar==5000 ���浽FMRAM

		SPI_FLASH_CS_LOW();
		// ZCL 2019.4.4  ����Һ�����趨���� �� FM25L16
		SPI_FMRAM_BufferWrite((u8 *)(&w_GprsParLst), 0, SCR_PAR_SIZE * 2);
		// ZCL 2019.4.4  ����GPRS(DTU)�趨���� �� FM25L16
		SPI_FMRAM_BufferWrite(GprsPar, 256, DTU_PAR_SIZE);

		w_GprsModPar = 17;
		// �ټ��޸Ķ������� ZCL
		S_SavePar = 1;
		B_ForceSavPar = 0;
		//

		// ZCL 2019.4.4 �Ժ������ ������ϸ��¼ �� GPRS���ϣ������ȵ�FM25L16
		// SPI_FLASH_Record();
	}

	if ((w_GprsModPar == 5000 || w_ModPar == 5000)
		// ZCL 2019.9.11 ���� ��ˮ���޸Ĳ���w_ModPar=5000��Ҳ���� Һ����SCR�Ĳ�����OK
		&& S_SavePar == 0)
	{
		w_GprsModPar = 0; // ��ֹ�� w_GprsModPar==5000 ���浽FMRAM

		SPI_FLASH_CS_LOW();
		// ZCL 2019.4.4  ����Һ�����趨���� �� FM25L16
		SPI_FMRAM_BufferWrite((u8 *)(&w_GprsParLst), 0, SCR_PAR_SIZE * 2);
		// ZCL 2019.4.4  ����GPRS(DTU)�趨���� �� FM25L16
		SPI_FMRAM_BufferWrite(GprsPar, 256, DTU_PAR_SIZE);

		w_GprsModPar = 5000;
		// �ټ��޸Ķ������� ZCL
		S_SavePar = 1;
		//

		// ZCL 2019.4.4 �Ժ������ ������ϸ��¼ �� GPRS���ϣ������ȵ�FM25L16
		// SPI_FLASH_Record();
	}

	//
	if (T_SavePar != SClk10Ms && S_SavePar != 0) // ����MD304L��ʾ״̬�����Ի���� ZCL
	{
		T_SavePar = SClk10Ms; //
		C_SavePar++;
		if (C_SavePar > 150 && S_SavePar == 1)
		{
			S_SavePar = 2;
			w_GprsModPar = 6000;
		}
		else if (C_SavePar > 300 && S_SavePar == 2)
		{
			T_SavePar = 100;
			C_SavePar = 0;
			S_SavePar = 0;
			w_GprsModPar = 0;
		}
	}
}

// ZCL 2018.12.11 ��ע����ǰҺ������Ҫ��������Ҫ��Ϊ��GPRS,LOAR,GPS,���� �Ȳ����ˡ�
void ForceSavePar(void) // ǿ�Ʊ������
{						// w_GprsModPar=2000,�涨ʱ����û���޸ĺͱ������,ǿ�Ʊ������
	if (w_GprsModPar == 2000 && T_ForceSavPar != SClkSecond)
	{
		T_ForceSavPar = SClkSecond; // �ϵ���ʱʱ�䵽�󣬿��Լ���Ƶ������
		C_ForceSavPar++;
		if (C_ForceSavPar > 900) // 15����
		{
			T_ForceSavPar = 100;
			C_ForceSavPar = 0;
			//
			B_ForceSavPar = 1;
		}
	}
}

void DoWith(void) // һЩ����,��¼�Ĵ���
{
	uint i, j, HeartSendFault, LinkFault;
	// uint nw_Qian,nw_Bai,nw_Shi,nw_Ge;				//ZCL 2019.4.4
	//	u32 m;			//ZCL 2019.10.18 ����
	// ZCL 2019.10.18 �¶ȵı仯���仯ֵ������ʾ
	s16 nw_Wendu;
	u8 ConnectNum;

	float f_m, f_n, f_mul;

	switch (Pw_LoRaSetSignalBW)
	{
	case 0:
		f_m = 7.8;
		break;
	case 1:
		f_m = 10.4;
		break;
	case 2:
		f_m = 15.6;
		break;
	case 3:
		f_m = 20.8;
		break;
	case 4:
		f_m = 31.2;
		break;
	case 5:
		f_m = 41.6;
		break;
	case 6:
		f_m = 62.5;
		break;
	case 7:
		f_m = 125;
		break;
	case 8:
		f_m = 250;
		break;
	case 9:
		f_m = 500;
		break;

	default:
		f_m = 125;
	}

	switch (Pw_LoRaSetErrorCoding)
	{
	case 0:
		f_n = (float)4 / 4;
		break; // ZCL 2019.9.24  �����(float)
	case 1:
		f_n = (float)4 / 5;
		break;
	case 2:
		f_n = (float)4 / 6;
		break;
	case 3:
		f_n = (float)4 / 7;
		break;
	case 4:
		f_n = (float)4 / 8;
		break;

	default:
		f_n = (float)4 / 6;
	}

	// pow(2,20);  //2��20����
	f_mul = (float)Pw_LoRaSetSpreadingFactor;
	f_mul = f_mul * f_m * f_n;
	i = (1 << Pw_LoRaSetSpreadingFactor);
	f_mul = f_mul / i * 100; // *1000/10  ��� *100��  /10 Ϊ����ʾʱ��һ��С���㣬2.280kbps ��ʾ�� 2.28kbps
	w_LoRaDateRate = (uint)f_mul;

	// ZCL 2018.12.20 ����LoRa Gprs����ͬʱ�趨Ϊ͸����COM2���ټ�GPS  Pw_GprsSet.4λ��
	if (F_LoRaToCom) // LoRa���������COM2		2019.1.19
	{
		F_GprsTransToCom = 0; // 1111 1111 1110 1101
							  // F_GpsToCom=0;						// 1111 1111 1110 1111
							  // F_GprsMasterNotToCom=0;	//ZCL 2019.3.11
	}
	// else if(F_GprsTransToCom)	//GPRS����GPS�����COM2
	// {
	// F_GpsToCom=0;						// 1111 1111 1110 1111
	// F_GprsMasterNotToCom=0;	//ZCL 2019.3.11
	// }

	if (F_GprsMasterNotToCom)  // ZCL 2019.3.11  F_GprsMasterNotToCom����������ڣ�
		F_GprsMasterToCom = 0; // ZCL 2019.3.11  F_GprsMasterToCom��������ڣ�

	// ZCL 2019.3.7
	if (Pw_LoRaMasterSlaveSel == 0) // LoRa�ӻ��� LoRa�ӻ���ʱ�򣬰�ֵ�ŵ�ָ�����򣬱���LoRa������ѯ
	{
		// 10����
		w_dsp1SoftVersion = w_dspSoftVersion;
		w_dsp1NowHz = w_dspNowHz;
		w_dsp1APhaseCurrent = w_dspAPhaseCurrent;
		w_dsp1OutVoltage = w_dspOutVoltage;
		w_dsp1NowTemperature = w_dspNowTemperature;
		// w_dsp1BusBarVoltage = w_dspBusBarVoltage;
		// w_dsp1TargetHz 			= w_dspTargetHz;
		w_dsp1Counter3 = w_dspCounter3;
		w_dsp1SysErrorCode1 = w_dspSysErrorCode1;
		w_dsp1SysRunStatus1 = w_dspSysRunStatus1;
		w_dsp1AI1ADCValue = w_dspAI1ADCValue;
		w_dsp1AI2ADCValue = w_dspAI2ADCValue;

		// ZCL 2019.9.11  �ټ�ѹ����״̬ 6����
		w_1InPDec = w_InPDec;
		w_1OutPDec = w_OutPDec;
		w_1PIDCalcP = w_PIDCalcP;
		w_1EquipOperateStatus = w_EquipOperateStatus;
		w_1EquipAlarmStatus = w_EquipAlarmStatus;
		w_1ScrEquipPower = Pw_ScrEquipPower; // �ı��ڵĲ�����4��
	}

	// ZCL 2017.9.19 �������  ZCL 2018.12.10
	if (C_LCDBackLight > 0)
	{
		Qb_LcdBacklight = 1; // ������
		if (T_LCDBackLight != SClk10Ms)
		{
			T_LCDBackLight = SClk10Ms;
			C_LCDBackLight++;
			if (C_LCDBackLight > 12000) // ���65535��
			{
				C_LCDBackLight = 0;
				Qb_LcdBacklight = 0; // ������
			}
		}
	}

	// 2017.5.25  -- 2017.5.31 DoWith��  ������������������
	// ZCL 2017.5.26  HeartSendFault
	HeartSendFault = 0;
	for (i = 0; i < 4; i++)
	{
		j = GprsPar[MaxLoginDSCNum0Base + i];
		if (B_HeartSendFault[i] >= j)
			HeartSendFault++;
	}

	// ZCL 2017.5.26  LinkFault
	LinkFault = 0;
	for (i = 0; i < 4; i++)
	{
		if (B_LinkFault[i] >= 10)
			LinkFault++;
	}

	// 80S��û�лָ����ա�����		ZCL123 2019.3.11 F_GprsMasterToCom
	if (F_GprsTransToCom || F_GprsMasterToCom) // Pw_GprsSet.1=1  GPRS�����COM2
	{
		// ZCL 2017.5.26 COM2��ֹ���պ�80S��û�лָ����ա�����
		// USART2->CR1 .3λ  =0 ��ֹ����
		if ((USART2->CR1 & 0x0004) == 0 //???YLS 2023.01.27�����⣿ ԭ��&&
			|| B_Com3Cmd8B + B_Com3Cmd8D + B_GprsDataReturn + B_ATComStatus + B_Com3Cmd89 + B_OtherCmd + HeartSendFault + LinkFault + S_SendSms + S_ReadNewSms + S_SmsFull > 0)
		{
			if (T_Com2RcvReset != SClkSecond)
			{
				T_Com2RcvReset = SClkSecond;
				C_Com2RcvReset++;
				if (C_Com2RcvReset > 80) // 80S��û�лָ����ա�����
				{
					T_Com2RcvReset = 100;
					C_Com2RcvReset = 0;

					USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
					USART2->CR1 |= 0x0004; //.3λ  ���н���

					B_Com3Cmd8B = 0;
					B_Com3Cmd8D = 0;
					B_GprsDataReturn = 0;

					for (i = 0; i < 4; i++)
					{
						B_HeartSendFault[i] = 0;
					}
					for (i = 0; i < 4; i++)
					{
						B_LinkFault[i] = 0;
					}

					//
					B_SmsOrFaultHardRestart = 0;
					S_M35 = 0x01;	  // ģ��ػ�������
					POWEROFF_4G;	  // 4Gģ��ϵ�
					LCD_DLY_ms(3000); // ��ʱ����û��ι�����豸��λ��
					POWERON_4G;		  // 4Gģ���ϵ�
				}
			}
		}
		else
		{
			C_Com2RcvReset = 0;
		}
	}

	//	// ZCL 2017.5.30  com2���ղ������ݣ�660S��ʱ����
	//	if (S_M35 == 0x1B && T_Com2NoRcv != SClkSecond)
	//	{
	//		T_Com2NoRcv = SClkSecond;
	//		C_Com2NoRcv++;
	//		if (C_Com2NoRcv > 1800)
	//		{
	//			C_Com2NoRcv = 0;
	//			//
	//			S_M35 = 0x01;	  // ģ��ػ�������
	//			LCD_DLY_ms(3000); // ��ʱ����û��ι�����豸��λ��
	//		}
	//	}

	// ZCL 2017.5.30  com3���ղ����������ݣ�660S��ʱ����
	// if (S_M35 == 0x1B && T_Com3NoRcv != SClkSecond)
	if (F_GprsEn && T_Com3NoRcv != SClkSecond) // �ſ������������������κ�״̬��ֻҪ20�����ղ������ݾ�����
	{
		T_Com3NoRcv = SClkSecond;
		C_Com3NoRcv++;
		if (C_Com3NoRcv > 1200)
		{
			C_Com3NoRcv = 0;
			//
			B_SmsOrFaultHardRestart = 0; // ���Ż��߹���Ӳ������
			S_M35 = 0x01;				 // ģ��ػ�������
			POWEROFF_4G;				 // YLS 2023.12.25 �ر�4Gģ���Դ
			LCD_DLY_ms(3000);			 // ��ʱ����û��ι�����豸��λ��
			POWERON_4G;					 // YLS 2023.12.25 ��4Gģ���Դ
		}
	}

	// ǿ�ƶ�ʱ����
	if (Pw_RebootInterval != 60000 && T_Reboot != SClkMinute) // ������ʱ��==60000����ȡ����ʱ��������
	{
		T_Reboot = SClkMinute;
		C_Reboot++;
		if (C_Reboot >= Pw_RebootInterval)
		{
			C_Reboot = 0;
			//
			B_SmsOrFaultHardRestart = 0; // ���Ż��߹���Ӳ������
			S_M35 = 0x01;				 // ģ��ػ�������
			POWEROFF_4G;				 // YLS 2023.12.25 �ر�4Gģ���Դ
			LCD_DLY_ms(3000);			 // ��ʱ����û��ι�����豸��λ��
			POWERON_4G;					 // YLS 2023.12.25 ��4Gģ���Դ
		}
	}

	// ����2���������ó���2013.1.17
	j = GprsPar[BaudRateBase + 2];
	Pw_GprsBaudRate2 = (j << 8) + GprsPar[BaudRateBase + 3];

	Pw_GprsDataBitLen2 = GprsPar[BaudRateBase + 4];	  // 2013.11.15
	Pw_GprsStopBitLen2 = GprsPar[BaudRateBase + 5];	  // 2013.11.15
	Pw_GprsParityBitLen2 = GprsPar[BaudRateBase + 6]; // 2013.11.15
	Pw_GprsFlowControl2 = GprsPar[BaudRateBase + 7];  // 2013.11.15

	// 2013.11.15
	if (Pw_GprsBaudRate2 != w_GprsSaveBaudRate2 || Pw_GprsDataBitLen2 != w_GprsSaveDataBitLen2 || Pw_GprsStopBitLen2 != w_GprsSaveStopBitLen2 || Pw_GprsParityBitLen2 != w_GprsSaveParityBitLen2 || Pw_GprsFlowControl2 != w_GprsSaveFlowControl2)
	{
		w_GprsSaveBaudRate2 = Pw_GprsBaudRate2;
		w_GprsSaveDataBitLen2 = Pw_GprsDataBitLen2;
		w_GprsSaveStopBitLen2 = Pw_GprsStopBitLen2;
		w_GprsSaveParityBitLen2 = Pw_GprsParityBitLen2;
		w_GprsSaveFlowControl2 = Pw_GprsFlowControl2;

		Com2_config();
	}

	// ZCL 2017.5.25 �ź��������õ�ʱ����˸
	if (C_NetLedLightCSQ > 300 && (w_GprsCSQ < 8 || w_GprsCSQ == 99))
	{
		C_NetLedLightCSQ = 0;
		if (NET_STA > 0)
			NET = 0; //=1,��=0,��
		else
			NET = 1; //=1,��=0,��
	}
	//	else if(w_GprsCSQ>=8 && w_GprsCSQ!=99)
	//		NET=0;					//=1,��=0,��

	// GPRSģ��ģ�PWR.DATA.NET ָʾ�ƵĶ��� 2013.9.1
	// S_M35������PWR.DATA.NET�ƣ���Ϊ��ͬ�Ľ׶Σ�ָʾ�Ƶ���˸�в�ͬ�ĺ��壩
	if (S_M35 < 0x11)
	{
		PWR_LIGHT = 1; //=1,��=0,��
		// NET=1;					//=1,��=0,��   ZCL 2017.5.25  ��NETָʾ������˸ָʾ�źŲ��õ�ʱ��

		// 1. DATA����˸��������������
		if (C_LedLight > 300)
		{
			C_LedLight = 0;
			if (COM2_DATASTA == 1)
				COM2_DATA = 0; //=1,��=0,��
			else
				COM2_DATA = 1; //=1,��=0,��
		}
	}
	else if (S_M35 >= 0x011 && S_M35 < 0x01B)
	{
		// 2. NET����
		NET = 0; //=1,��=0,��
		// 2.1 �ҵ����磬DATAֹͣ��˸
		COM2_DATA = 1; //=1,��=0,��
	}

	if (S_M35 >= 0x01B)
	{
		// 3.1 PWR��
		if (CGD0_LinkConnectOK[0] + CGD0_LinkConnectOK[1] + CGD0_LinkConnectOK[2] + CGD0_LinkConnectOK[3] == 0)
		{
			// 3.1.1 PWR  ��˸�Ͽ�  ��ʾδ����GPRS��������
			if (C_LedLight > 50)
			{
				C_LedLight = 0;
				if (PWR_STA > 0)
					PWR_LIGHT = 0; //=1,��=0,��
				else
					PWR_LIGHT = 1; //=1,��=0,��
			}
		}
		else
		{
			ConnectNum = 0;
			for (i = 0; i < 4; i++)
			{
				if (GprsPar[Ip0Base + i * 4] + GprsPar[DomainName0Base + i * 31] != 0)
					ConnectNum++;
			}

			if (ConnectNum == (ZhuCeOkFLAG[0] + ZhuCeOkFLAG[1] + ZhuCeOkFLAG[2] + ZhuCeOkFLAG[3]))
			{
				// 3.1.2 PWR  ��˸����  ��ʾ�ɹ���GPRS��������
				if (C_LedLight > 1000)
				{
					C_LedLight = 0;
					if (PWR_STA > 0)
						PWR_LIGHT = 0; //=1,��=0,��
					else
						PWR_LIGHT = 1; //=1,��=0,��
				}
			}
			else
			{
				// 3.1.2 PWR  ��˸�Ͽ�  ��ʾֻ�в���ͨ���ɹ���GPRS��������
				if (C_LedLight > 300)
				{
					C_LedLight = 0;
					if (PWR_STA > 0)
						PWR_LIGHT = 0; //=1,��=0,��
					else
						PWR_LIGHT = 1; //=1,��=0,��
				}
			}
		}

		// 3.2 COM2_DATA��
		if (Cw_Rcv2 + Cw_Txd2 > 0)
		{
			// COM2_DATA ��ת  2014.11.27�޸�
			// ZCL 2020.3.26			GPIO_WriteBit(GPIOC, GPIO_Pin_12,
			//		               (BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_12))));		//ZCL 2018.12.4
			COM2_DATA_TOGGLE;
		}
		else //=1,��
			COM2_DATA = 1;
	}

	// TXD��RXD���ǵ͵�ƽ��Ч��ƽʱ�ߵ�ƽ 2013.8.30
	// 2014.11.27�޸� COM3_DATA ��ת
	// 2014.12.11 COM3_DATA����S_M35���ƣ��������ݷ�ת
	if (Cw_Rcv3 + Cw_Txd3 > 0)
	{
		// COM3_DATA ��ת 2014.11.27�޸�
		// ZCL 2020.3.26		GPIO_WriteBit(GPIOD, GPIO_Pin_5,
		//  								 (BitAction)((1-GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_5))));		//ZCL 2018.12.4	 �յã�û�������
		COM3_DATA_TOGGLE;
	}
	else //=1,��
		COM3_DATA = 1;

	// 2016.5.27 ����DATA�����һֱ��10�룬���Ź�������2016.5.27
	if (T_DataLampAllLight != SClk10Ms)
	{
		if (COM2_DATASTA == 0)
		{
			T_DataLampAllLight = SClk10Ms;
			C_DataLampAllLight++;
			if (C_DataLampAllLight > 2000)
			{
				C_DataLampAllLight = 0;

				S_M35 = 0x01;	  // ģ��ػ�������
				LCD_DLY_ms(3000); // ��ʱ����û��ι�����豸��λ�� 2013.7.3
			}
		}
		else if (C_DataLampAllLight > 0)
			C_DataLampAllLight--;
	}

	// ZCL 2019.10.18 �¶ȵı仯���仯ֵ������ʾ
	// ZCL 2022.10.5
	// ���1 �¶�ֵ�任
	nw_Wendu = (s16)(w_Lora1WenDu1);
	if (nw_Wendu < 0)
	{
		w_Pump1DispWenDu1DecValue = -nw_Wendu;
		w_Pump1DispWenDu1DecValue = w_Pump1DispWenDu1DecValue + 10000; // 10000�Ǹ���
	}
	else
		w_Pump1DispWenDu1DecValue = w_Lora1WenDu1;

	nw_Wendu = (s16)(w_Lora1WenDu2);
	if (nw_Wendu < 0)
	{
		w_Pump1DispWenDu2DecValue = -nw_Wendu;
		w_Pump1DispWenDu2DecValue = w_Pump1DispWenDu2DecValue + 10000; // 10000�Ǹ���
	}
	else
		w_Pump1DispWenDu2DecValue = w_Lora1WenDu2;

	// ���1 �¶�ֵ�任
	nw_Wendu = (s16)(w_Lora2WenDu1);
	if (nw_Wendu < 0)
	{
		w_Pump2DispWenDu1DecValue = -nw_Wendu;
		w_Pump2DispWenDu1DecValue = w_Pump2DispWenDu1DecValue + 10000; // 10000�Ǹ���
	}
	else
		w_Pump2DispWenDu1DecValue = w_Lora2WenDu1;

	nw_Wendu = (s16)(w_Lora2WenDu2);
	if (nw_Wendu < 0)
	{
		w_Pump2DispWenDu2DecValue = -nw_Wendu;
		w_Pump2DispWenDu2DecValue = w_Pump2DispWenDu2DecValue + 10000; // 10000�Ǹ���
	}
	else
		w_Pump2DispWenDu2DecValue = w_Lora2WenDu2;

	// ���1 �¶�ֵ�任
	nw_Wendu = (s16)(w_Lora3WenDu1);
	if (nw_Wendu < 0)
	{
		w_Pump3DispWenDu1DecValue = -nw_Wendu;
		w_Pump3DispWenDu1DecValue = w_Pump3DispWenDu1DecValue + 10000; // 10000�Ǹ���
	}
	else
		w_Pump3DispWenDu1DecValue = w_Lora3WenDu1;

	nw_Wendu = (s16)(w_Lora3WenDu2);
	if (nw_Wendu < 0)
	{
		w_Pump3DispWenDu2DecValue = -nw_Wendu;
		w_Pump3DispWenDu2DecValue = w_Pump3DispWenDu2DecValue + 10000; // 10000�Ǹ���
	}
	else
		w_Pump3DispWenDu2DecValue = w_Lora3WenDu2;

	// ���1 �¶�ֵ�任
	nw_Wendu = (s16)(w_Lora4WenDu1);
	if (nw_Wendu < 0)
	{
		w_Pump4DispWenDu1DecValue = -nw_Wendu;
		w_Pump4DispWenDu1DecValue = w_Pump4DispWenDu1DecValue + 10000; // 10000�Ǹ���
	}
	else
		w_Pump4DispWenDu1DecValue = w_Lora4WenDu1;

	nw_Wendu = (s16)(w_Lora4WenDu2);
	if (nw_Wendu < 0)
	{
		w_Pump4DispWenDu2DecValue = -nw_Wendu;
		w_Pump4DispWenDu2DecValue = w_Pump4DispWenDu2DecValue + 10000; // 10000�Ǹ���
	}
	else
		w_Pump4DispWenDu2DecValue = w_Lora4WenDu2;
}

void SPI_FLASH_Record(void) // SPI-FLASH ��¼����
{
	//
}

void ParLimit(void) // ��������
{
	// ����������
	if (Pw_GprsBaudRate2 != 2400 && Pw_GprsBaudRate2 != 4800 && Pw_GprsBaudRate2 != 9600 && Pw_GprsBaudRate2 != 19200 && Pw_GprsBaudRate2 != 38400 && Pw_GprsBaudRate2 != 57600)
	{
		Pw_GprsBaudRate2 = 9600; // ����1������
	}

	// ZCL 2018.12.10
	if (Pw_ScrKeyMode >= 3) // ��Һ��������ģʽֵ��������
		Pw_ScrKeyMode = 0;

	// ZCL 2019.3.23
	if (Pw_ScrMenuMode != 0 && Pw_ScrMenuMode != 1 && Pw_ScrMenuMode != 2)
		Pw_ScrMenuMode = 0;

	if (Pw_ScrCurrentBits >= 3 || Pw_ScrCurrentBits < 1) // ��ֵ��������
		Pw_ScrCurrentBits = 2;

	// ZCL 2019.9.11  Pw_ScrEquipPower ��Ƶ����  �Ǹ���ֵ����̫������

	if (Pw_LoRaModule1Add >= 1000)
		Pw_LoRaModule1Add = 0;
	if (Pw_LoRaModule2Add >= 1000)
		Pw_LoRaModule2Add = 0;
	if (Pw_LoRaModule3Add >= 1000)
		Pw_LoRaModule3Add = 0;
	if (Pw_LoRaModule4Add >= 1000)
		Pw_LoRaModule4Add = 0;

	// ZCL 2019.3.22		LORA��ֵ������
	if (Pw_LoRaSetFreq < 137 || Pw_LoRaSetFreq > 1020)
		Pw_LoRaSetFreq = 434;
	if (Pw_LoRaSetPower == 20)
		;
	else if (Pw_LoRaSetPower < 7 || Pw_LoRaSetPower > 14)
		Pw_LoRaSetPower = 20;

	if (Pw_LoRaSetSignalBW > 9) // �źŴ���BW
		Pw_LoRaSetSignalBW = 7; // ZCL 2019.9.24  9

	if (Pw_LoRaSetSpreadingFactor < 6 || Pw_LoRaSetSpreadingFactor > 12) // ��Ƶ���� SF
		Pw_LoRaSetSpreadingFactor = 7;

	if (Pw_LoRaSetErrorCoding < 1 || Pw_LoRaSetErrorCoding > 4) // ������
		Pw_LoRaSetErrorCoding = 2;

	if (Pw_LoRaSetCrcOn > 1)
		Pw_LoRaSetCrcOn = 1;

	if (Pw_LoRaSetImplicitHeaderOn > 1)
		Pw_LoRaSetImplicitHeaderOn = 0;

	if (Pw_LoRaSetRxSingleOn > 1)
		Pw_LoRaSetRxSingleOn = 0; // ZCL 2020.3.26 ������1�ĳ�0

	if (Pw_LoRaSetFreqHopOn > 1)
		Pw_LoRaSetFreqHopOn = 0;

	// ZCL 2019.9.24 ���ͳ�ʱ
	if (Pw_LoRaSetTxPacketTimeOut < 10 || Pw_LoRaSetTxPacketTimeOut > 6000)
		Pw_LoRaSetTxPacketTimeOut = 1000;
	// ZCL 2019.9.24 ���ճ�ʱ
	if (Pw_LoRaSetRxPacketTimeOut < 10 || Pw_LoRaSetRxPacketTimeOut > 6000)
		Pw_LoRaSetRxPacketTimeOut = 1000;

	// ZCL 2020.4.8 ��������
	if (Pw_LoRaMasterSlaveSel > 1)
		Pw_LoRaMasterSlaveSel = 0;

	// ZCL 2019.4.2
	if (Pw_LoRaMasterSlaveSel == 1)
	{
		// Pw_LoRaSet=1;			//ZCL 2019.11.21 ע�͵�
		// Pw_GprsSet=9;			//ZCL 2019.11.21 ע�͵�
		// Pw_LoRaEquipmentNo = 2;
		// if (Pw_LoRaModule1Add < 3)
		// 	Pw_LoRaModule1Add = 3;
	}
	else if (Pw_LoRaMasterSlaveSel == 0)
	{
		// Pw_LoRaSet=1;			//ZCL 2019.11.21 ע�͵�
		// Pw_GprsSet=0;			//ZCL 2019.11.21 ע�͵�
		if (Pw_LoRaEquipmentNo < 3)
			Pw_LoRaEquipmentNo = 3;
	}
	// ǿ������ʱ����
	if (Pw_RebootInterval < 5)
	{
		Pw_RebootInterval = 480;
	}
}

void Time_Output(void) // ���ʱ�����	 2008.10.21
{
	// ���ʱ�ӣ�����ʵʱ��
	if (SClkSecond >= 60) // ��
	{
		SClkSecond = 0;
		SClkMinute++;
		Pw_GprsSetMinute = SClkMinute;
		if (SClkMinute >= 60) // ��
		{
			SClkMinute = 0;
			SClkHour++;
			Pw_GprsSetHour = SClkHour;
			if (SClkHour >= 24) // ʱ
			{
				SClkHour = 0;
				SClkDay++;
				Pw_GprsSetDay = SClkDay;
				if (SClkDay > 30) // ��
				{
					SClkDay = 1;
					SClkMonth++;
					// Pw_GprsSetMonth=Pw_GprsSetMonth;
					if (SClkMonth > 12) // ��
					{
						SClkMonth = 1;
						SClkYear++;
					}
				}
			}
		}
	}

	Pw_GprsSetSecond = SClkSecond;
	Pw_GprsSetMinute = SClkMinute;
	Pw_GprsSetHour = SClkHour;
}

/* static u32 CpuID[3];
static u32 Lock_Code; */

/* void GetLockCode(void)		// 2013.10.26 �õ���������
{
	//��ȡCPUΨһID
	CpuID[0]=*(vu32*)(0x1ffff7e8);
	CpuID[1]=*(vu32*)(0x1ffff7ec);
	CpuID[2]=*(vu32*)(0x1ffff7f0);
	//�����㷨,�ܼ򵥵ļ����㷨
	Lock_Code=(CpuID[0]>>1)+(CpuID[1]>>2)+(CpuID[2]>>3);
}
 */

/* void GetLockCode(void)		// 2013.10.26 �õ���������
{
	//��ȡCPUΨһID
	//ʹ��ָ��ָ��UID����ʼ��ַ��
	//�Ժ�ֻҪ�������þͿ�����.����UID[0]����оƬ����ֽڵ�ȫ��ΨһID.
	u32* UID=(u32*)0x1ffff7e8;		//

	//�����㷨,�ܼ򵥵ļ����㷨
	Lock_Code=(UID[0]>>1)+(UID[1]>>2)+(UID[2]>>3);
}
 */

u8 D[12];						 // D[12]Ϊ��ʽ����������(ע����UID[12]����).
u8 *UID = (u8 *)0x1ffff7e8;		 // ȫ��ΨһID��
u8 *LOCKCODE = (u8 *)0x0801E000; // ��������
u32 Fml_Constant;				 // �û�ָ����32λ���������뵽��ʽ�ĳ���
u8 Result[4];					 // ��ʽ���������,�ĸ��ֽڣ����ܹ�ʽ�����Ϊһ������
u8 *C = (u8 *)&Fml_Constant;	 // �ѳ���ת��Ϊ����
u16 Fml_CRC16;					 // CRC16У��ֵ
u8 B_LockCodeOK;				 // ����������ȷ��־

void GetLockCode(void) // 2013.10.26 �õ���������
{
	// �ڼ��ܹ�ʽ��,��������������� 1���ǳ�����2���Ǳ仯��˳���UIDֵ
	Fml_Constant = 0x1100FF;

	// ����UID[0..11]������ֱ��ȡ�ö�����ΨһID��
	D[0] = UID[2];
	D[1] = UID[3];
	D[2] = UID[6];
	D[3] = UID[7];
	D[4] = UID[4];
	D[5] = UID[5];
	D[6] = UID[10];
	D[7] = UID[11];
	D[8] = UID[0];
	D[9] = UID[1];
	D[10] = UID[8];
	D[11] = UID[9];

	Formula_12(D, Result); // �����Զ����㷨���������
	if (Result[0] == LOCKCODE[0] && Result[1] == LOCKCODE[1] && Result[2] == LOCKCODE[2] && Result[3] == LOCKCODE[3])
		B_LockCodeOK = 1;
	else
		B_LockCodeOK = 0;
}

/********************************************************************
�������ܣ���ʽ
��ڲ�����D
��  ע��
********************************************************************/
void Formula_12(u8 *D, u8 *Result)
{
	Result[0] = C[0] + D[0] - D[4] + D[8];
	Result[1] = C[1] & D[1] ^ D[5] | D[9];
	Result[2] = C[2] | D[2] ^ D[6] ^ D[10];
	Result[3] = C[3] - D[3] + D[7] & D[11];
}

/********************************************************************
�������ܣ���ʽ
��ڲ�����D
��  ע��
********************************************************************/
void Formula_113(u8 *D, u8 *Result)
{
	u8 *crc;
	u16 dat;
	crc = (u8 *)&dat;
	Fml_CRC16 = 0;
	dat = CRC16(D, 12);
	Result[0] = crc[0];
	Result[1] = crc[1];
	Result[2] = C[2] ^ D[2] ^ D[1];
	Result[3] = C[3] ^ D[3] ^ D[0];
}

void Test(void) // ���
{
}
u32 FtoU32(u16 w1, u16 w2) // ���ֽڵĸ�������ת��Ϊ����10�ĳ���������
{
	u32 tmpL;
	aa.b[3] = (w1 >> 8);
	aa.b[2] = w1;
	aa.b[1] = (w2 >> 8);
	aa.b[0] = w2;
	// tmpL=(u32)(aa.f*100.0);		//2λС����
	tmpL = (u32)(aa.f * 10.0); // 1λС����
	return tmpL;
}
