
#ifndef _MB_MotherBoard_H
#define _MB_MotherBoard_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//-----------------------------------------------------------------------------
//���������� ����� ��. �� ��������.
#define ControlOff	0
#define ControlOn		1

#define SP_CHECK_ON  1
#define SP_CHECK_OFF 0
//����� ��.
#define	Line1  			0	
#define	Att1   			1	
#define	Line2  			2	
#define	Att2   			3
#define AllLines 		0xFF
//��������� ����� ��.
#define	LineOffControl	0	
#define LineNorm     		1 
#define LineUndef    		2 //��������� �� ���������� - ���� ���������� �������.
#define LineBreak    		3
#define LineOver     		4
#define LineUnder    		5
#define LineShort    		6
//----------------------------------------
//������ � ������.
#define	Zone1   0 //(1<<0)
#define	Zone2   1 //(1<<1)		  
#define	Zone3   2 //(1<<2) 
#define	Zone4   3 //(1<<3)
#define	Zone5   4 //(1<<4) 
#define	Zone6   5 //(1<<5)
#define	ZoneAll 6 //(Zone1 | Zone2 | Zone3 | Zone4 | Zone5 | Zone6)

#define	On   1 
#define	Off  0
//�������� � ������ ��.
#define	ActionOff		   			0 //���� ���������.
#define	ActionManualOn 			1 //���� ������������ � ������, �������� ������ ������1-������8.
#define	ActionConsolOn 			2 //���� ������������ ��������.
#define	ActionMicOn    			3 //���� ������������ ������������ ���������� ����.
#define	ActionPuskOn   			4 //���� ������������ ������� ����.
#define	ActionFireLine1On		5 //���� ������������ �������� ������� ���.1.
#define	ActionFireLine2On	  6 //���� ������������ �������� ������� ���.2.
#define	ActionFireLine3On	  7 //���� ������������ �������� ������� ���.3. 
#define	ActionFireLineChSOn	8 //���� ������������ �������� ������� �����.
#define	ActionIPOn	   			9 //���� ������������ ������ IP.
#define	ActionAutoOn	 			10//���� ������������ ����������. - ����� ��������, �������� ��� ��������� �� �����.
#define	ActionAutoOff	 			11//���������� ��������� - ������������ �������� ������ ����� ������.  
#define	ActionDefect   			12//

//��� ��� ����������� ����� ��.
typedef enum{
	OUT_OFF = 0,
	OUT_MANUAL_ACT,
	OUT_CONSOL_ACT,
	OUT_MIC_ACT,
	OUT_FIRE_LINE_1_ACT,
	OUT_FIRE_LINE_2_ACT,
	OUT_FIRE_LINE_3_ACT,
	OUT_FIRE_LINE_ChS_ACT,
	OUT_IP_ACT,
	OUT_AUTO_ACT,
	OUT_AUTO_OFF,
	OUT_DEFECT,
}SpOutEnum_t;
//----------------------------------------
//��������� ��������� ������� 220V � ���������.
#define PowerMask        0x0F
#define PowerBlink     	(0<<0) //��� �������.
#define PowerControlOff	(1<<0) //�������� ��������.
#define PowerACOk      	(2<<0) //���� ������� 220V
#define PowerDCOk      	(3<<0) //�������� ��������.
#define PowerACNo       (4<<0) //������� 220V �����������
#define PowerDCFault    (5<<0) //�������� ����������.
#define PowerFault      (6<<0) //���������� ��� ����� �������.
//��������� �������.
#define BatMask          0xF0
#define BatBlink        (0<<4) //��� �������.
#define BatControlOff   (1<<4) //�������� ��������.
#define BatOk           (2<<4) //���������� �� ��� �� 21� �� 27,2�.
#define BatCharge       (3<<4) //���� ����� ���
#define BatChargeEnd    (4<<4) //����� �������, ���������� ��� 27,2�.
#define BatAttention    (5<<4) //���������� �� ��� <= 20,5�.
#define BatDeep         (6<<4) //���������� �� ��� <= 18,5� - �������� ������ ���.
#define BatNo           (7<<4) //��� �����������.
//��������� ����.
#define AmpBlink        (0<<0) //��� �������.
#define AmpDisactive    (1<<0) //���� ��������.
#define AmpActive       (2<<0) //���� �������.
#define AmpProt         (3<<0) //���� � ������.
//*****************************************************************************
//*****************************************************************************
#pragma pack(push, 1)//������ ������������ � 1 ����

typedef struct{
	//--------------------
	uint8_t OutState  :4;
	uint8_t LineState :4;	
	//--------------------
}Line_t;
//**************************************************
//��������� ������ ��.
//#pragma pack(push, 1)//������ ������������ � 1 ����
typedef struct{
	//--------------------
	uint32_t TimeUTC;
	//--------------------	
	//��������� ������� �������.
	uint16_t TotalEvents;
	uint16_t NotReadEvents;
	//--------------------
	//��������� ������������.
	uint16_t SirenPower;	 
	Line_t   Siren1;
	Line_t   Siren2;
	Line_t   Siren3;
	//--------------------	
	uint16_t DebugData1;
	uint16_t DebugData2;
	//--------------------
	//����� ��.
	uint16_t  SpLineDeviation;
	Line_t 		Sp1;
	Line_t 		Sp2;
	Line_t 		Sp3;
	Line_t 		Sp4;
	Line_t 		Sp5;
	Line_t 		Sp6;	
	//--------------------
	//����� ������� ������������.
	Line_t 		Fr1;
	Line_t 		Fr2;
	Line_t 		Fr3;
	Line_t 		FrChS;
	//--------------------	
	uint8_t  Addres;         
	uint8_t  Group;
	uint8_t  MB_State;
	uint8_t  StatusPOWER;
	uint8_t  StatusPA;
	//--------------------
	//��������� ����� IP.		

	//--------------------
	//��������� ����� ZE.		

	//--------------------
}MBData_t;
#pragma pack(pop)//������� ���������� ���������.
#define MOTHER_BOARD_DATA_SIZE ((uint8_t)sizeof(MBData_t))
//**************************************************
#define MB_TYPE             0x01 //��� ������ - ����������� �����.
//������� MB.
#define MB_CMD_SEND_STATUS  0xA0 //������� "������ ������� �� MB".
//********************************
#define MB_WORK_STATE	0
#define MB_TEST_STATE	1

//********************************
#define BUTTONS_ALG_ACT  6 //PRIORITY_6	    6
#define BUTTON_START_ACT 5 //PRIORITY_5	    5
#define ChS_ACT				   4 //PRIORITY_4	    4
#define FIRE_LINE_ACT 	 3 //PRIORITY_3	    3
#define BUTTON_PUSK_ACT  2 //PRIORITY_2	    2
#define MIC_ACT 		 		 1 //PRIORITY_1	    1
#define NO_ACT 		 		   0 //��� ���������
//*****************************************************************************************
//*****************************************************************************************
//��������� ��������� ������� 220V, ��������� � ����.
typedef struct{
	uint8_t MainPower;
	uint8_t Bat;
	uint8_t Amp;
	//----------
	union {
		struct 
		{	
			uint8_t MainPower :1;
			uint8_t Bat       :1;
			uint8_t :6;
		}bit;
		uint8_t Byte;
	}CheckFromMB;	
	//----------
	union {
		struct 
		{	
			uint8_t MainPower :1;
			uint8_t Bat       :1;
			uint8_t :6;
		}bit;
		uint8_t Byte;
	}Check;
	//----------
}PowerSTR_t;
//*********************************
#pragma pack(push, 1)//������ ������������ � 1 ����
typedef struct{
	//---------
	uint8_t  LineRef;
	uint8_t  Control;
	uint16_t Deviation;	
	//---------	
	uint16_t ConfigFlag :1;
	uint16_t :15;
	//---------	
}SpParam_t;
#pragma pack(pop)//������� ���������� ���������.
//*****************************************************************************
//*****************************************************************************
uint8_t 	 SpLine_GetOutStateForTxt(uint8_t out);
uint8_t 	 SpLine_GetOutStateForLed(uint8_t out);
uint8_t 	 SpLine_GetOutState(uint8_t out);
uint8_t    SpLine_CompareAllLinesWith(uint8_t state);
SpParam_t* SpLine_Param(void);
//*********************************

uint8_t FireLine_GetLineState(uint8_t in);
uint8_t FireLine_GetInState(uint8_t in);
uint8_t FireLine_CompareAllLinesWith(uint8_t state);

//*********************************
void 			MotherBoard_SaveData(uint8_t *dataBuf);
MBData_t* MotherBoard(void);
//---------

PowerSTR_t* PowerDevice(void);
//*****************************************************************************
//*****************************************************************************
//������ � �������� �������.
//��������� ������ ������� �������
#pragma pack(push, 1)//������ ������������ � 1 ����
typedef struct{
	//--------
	uint32_t TimeUtc;//����� �������� �������.
	uint16_t Number; //����� �������.
	uint8_t  ID;     //��� �������.
	uint8_t  Param;  //�������� �������(����� ����, ����� ������ � �.�.).
	//--------
}Event_t;
#pragma pack(pop)//������� ���������� ���������.
//-------------------------------------------------
#pragma pack(push, 1)//������ ������������ � 1 ����
typedef struct{
	//--------
	Event_t  Event;
	uint16_t TotalEvents;  //����� ���������� ������� �������.
	uint16_t NotReadEvents;//���������� ������������ �������.
	uint16_t EventIndex;
	uint16_t EventOldIndex;	
	//--------
}Log_t;
#pragma pack(pop)//������� ���������� ���������.
//-------------------------------------------------
#define MaxEvent 	1024
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void 	 LogSaveEvent(uint8_t* dataBuf);
Log_t* Log(void);
//************************************************************************************************
//************************************************************************************************
//������ � ����������������� ����� �������� �������.
//���������� �������� ������ �������� ������.
#pragma pack(push, 2)//������ ������������ � 2 �����
typedef	struct{
	//-------------
	uint16_t Number;
	uint16_t FaultState;
	uint16_t LogicalState;  
	uint16_t Type;
	uint16_t TimeOut;
	uint16_t PhysicalState;
	uint16_t MeasValue;
	//-------------
}FireLine_t;
#pragma pack(pop)//������� ���������� ���������.
//-------------------------------------------------
//����� ������� �������� ������������
#define Poj1   								0
#define Poj2   								1
#define Poj3   								2
#define ChS    								3
//����� � ���. �������� �� �������� � ����
#define	InputTimeOutMin 			0
#define	InputTimeOutMax 			99
//����� ������ �����.���� Type ����������� ��������� �����.
#define	InputOff              0 // ���� ����. � �� ��������������; 
#define	InputOnControlOnClose	1 // ���� ���. � ��������������, �������� ��������� ��;
#define	InputOnControlOnOpen  2 // ���� ���. � ��������������, �������� ���������� ��;

//#define	InputOnControlOff     1 // ���� ���. � �� ��������������;
//#define	InputOnControlOnClose	2 // ���� ���. � ��������������, �������� ��������� ��;
//#define	InputOnControlOnOpen  3 // ���� ���. � ��������������, �������� ���������� ��;
//#define	InputOnControlOnClOp  4 // ���� ���. � ��������������, �������� ��������� � ���������� ��;
//C�������� �������� �������.
//���������� ��������� �������� �������.
#define	FR_IN_CONTROL_OFF    0u //
#define	FR_IN_CONTROL_ON     1u //
#define	FR_IN_ATTENTION 		 2u //��������Ż � ������������� ������������ ������ ����������
#define	FR_IN_FIRE      		 3u //�����л    � ���� ��������� � ��� ��������� ����� � ������ ��������� �������� � ������� TimeOut. TimeOut �������� � ���������� �����.
//���������� ��������� �������� �������.
#define FR_LINE_NORM 			   0u
#define FR_LINE_UNDEF 			 1u
#define FR_LINE_NORM_OPEN  	 2u
#define FR_LINE_NORM_CLOSE 	 3u
#define FR_LINE_SHORT 			 4u
#define FR_LINE_BREAK     	 5u
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void 				FireLine_Save(uint8_t *dataBuf);
FireLine_t* FireLine(uint32_t line);
//************************************************************************************************
//************************************************************************************************
#endif /*_MB_MotherBoard_H*/






















