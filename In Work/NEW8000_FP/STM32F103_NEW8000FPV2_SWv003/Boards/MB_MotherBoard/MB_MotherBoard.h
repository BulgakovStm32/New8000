
#ifndef _MB_MotherBoard_H
#define _MB_MotherBoard_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//-----------------------------------------------------------------------------
//���������� ����� ��. �� ��������.
#define ControlOff	0
#define ControlOn		1
//����� ��.
#define	Line1  			0	
#define	Att1   			1	
#define	Line2  			2	
#define	Att2   			3
#define AllLines 		0xFF
//��������� ����� ��.
#define	LineMask		  	 0xF0
#define	LineOffControl	(0<<4)	
#define LineNorm     		(1<<4) 
#define LineUndef    		(2<<4)//��������� �� ���������� - ���� ���������� �������.
#define LineBreak    		(3<<4)
#define LineOver     		(4<<4)
#define LineUnder    		(5<<4)
#define LineShort    		(6<<4)
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
#define	ActionMask		  	 	 0x0f
#define	ActionOff		   			(0<<0) //���� ���������.
#define	ActionManualOn 			(1<<0) //���� ������������ � ������, �������� ������ ������1-������8.
#define	ActionConsolOn 			(2<<0) //���� ������������ ��������.
#define	ActionMicOn    			(3<<0) //���� ������������ ������������ ���������� ����.
#define	ActionPuskOn   			(4<<0) //���� ������������ ������� ����.
#define	ActionFireLine1On		(5<<0) //���� ������������ �������� ������� ���.1.
#define	ActionFireLine2On	  (6<<0) //���� ������������ �������� ������� ���.2.
#define	ActionFireLine3On	  (7<<0) //���� ������������ �������� ������� ���.3. 
#define	ActionFireLineChSOn	(8<<0) //���� ������������ �������� ������� �����.
#define	ActionIPOn	   			(9<<0) //���� ������������ ������ IP.
#define	ActionAutoOn	 			(10<<0)//���� ������������ ����������. - ����� ��������, �������� ��� ��������� �� �����.
#define	ActionAutoOff	 			(11<<0)//���������� ��������� - ������������ �������� ������ ����� ������.  
#define	ActionDefect   			(12<<0)//
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
//��������� ������ ��.
#pragma pack(push, 1)//������ ������������ � 1 ����
typedef struct{
	//--------------------
	uint8_t  Addres;         
	uint8_t  Group;
	uint8_t  Priority;
	uint8_t  StatusZone1;
	uint8_t  StatusZone2;
	uint8_t  StatusZone3;
	uint8_t  StatusZone4;
	uint8_t  StatusZone5;
	uint8_t  StatusZone6;
	uint8_t  StatusPoj1;
	uint8_t  StatusPoj2;
	uint8_t  StatusPoj3;
	uint8_t  StatusGOChS;
	uint8_t  StatusPOWER;
	uint8_t  StatusPA;
	uint32_t TimeUTC;
	uint16_t DebugData1;
	uint16_t DebugData2;
	uint16_t LcDeviationVolue;
	//--------------------	
	//��������� ������� �������.
	uint16_t TotalEvents;
	uint16_t NotReadEvents;
	//--------------------
	//��������� ������������.
	uint16_t SirenPower;	 
	uint16_t Siren1Status; 
	uint16_t Siren2Status; 
	uint16_t Siren3Status; 
	//--------------------
	//��������� ����� IP.		

	//--------------------
	//��������� ����� ZE.		

	//--------------------
	//uint16_t CRC16;
	//--------------------
}MBData_t;
#pragma pack(pop)//������� ���������� ���������.
//*****************************************************************************
//*****************************************************************************
//��������� ������ FP.
#pragma pack(push, 1)//������ ������������ � 1 ����
typedef struct{
	//--------------------
	uint8_t  MicState;			 //????????? ?????????
	uint8_t  KeyState;			 //????????? ?????.
	uint8_t  Address;        //6-? ????  - ?????? ?????.        
	uint8_t  Group;          //7-? ????  - ?????? ?????.  
	uint8_t  Control_Buttons;//8-? ????  - ????????? ?????? ??????????.
	uint8_t  Alg_Buttons;    //9-? ????  - ????????? ?????? ????????? ??????????.
	uint32_t TimeUTC;        //10-? ???? - ????? ? ??????? UTC.	
	//--------------------
}FPData_t;
#pragma pack(pop)//������� ���������� ���������.
//*****************************************************************************
//*****************************************************************************
///������ � ����������������� ����� �������� �������.
//���������� �������� ������ �������� ������.
#pragma pack(push, 1)//������ ������������ � 1 ����
typedef	struct{
	//-------------
	uint8_t  Number;
	uint8_t  Type;  
	uint8_t  TimeOut;
	uint8_t  PhysState;
	uint16_t MeasValue;	
	//-------------
}FireLine_t;
#pragma pack(pop)//������� ���������� ���������.
//*****************************************************************************
//*****************************************************************************
//������ � ����������� EEPROM, ������������ �� ��.
#pragma pack(push, 1)//������ ������������ � 1 ����
typedef	struct{
	//-------------
	uint32_t BadSectorsCount;//
	uint32_t WriteCount;     //���������� �������
	uint32_t ReadCount;	     //���������� ������.	
	uint8_t  StatReg;        //������� ���������.
	//-------------
}EepInfo_t;
#pragma pack(pop)//������� ���������� ���������.
//*****************************************************************************
//*****************************************************************************
#define RS485_BUF_SIZE         64
#define RS485_BUF_HEADER_SIZE  5
//����� ��� ������ ���������.
#pragma pack(push, 1)//������ ������������ � 1 ����
typedef struct{
	//��������� ������
	uint8_t SinhroByte1;// ����������.
	uint8_t SinhroByte2;// ����������.
	uint8_t Type;       // ��� ������.
	uint8_t Size;       // ������ ������.
	uint8_t CmdCode;    // ��� �������.	
	//������.
	union
		{
			uint8_t Buf[RS485_BUF_SIZE];//�����.
			//----------
			FireLine_t FireLine;//������ SB.
			EepInfo_t  EepInfo; //
			//----------
			struct 
				{
					MBData_t Status;//������ ��.
					uint16_t CRC16;
				}MB;
			//----------
			struct
				{
					FPData_t Status;//������ FP.
					uint16_t CRC16;
				}FP;
			//----------
		}Data;
	//-----------
}RS485BufUnion_t;
#pragma pack(pop)//������� ���������� ���������.
//*****************************************************************************************
//*****************************************************************************************
//������� MB.
#define MB_DefultAddressConst    0x01 //�������� ������ ����������� ����� �� ���������.
#define MB_TypeConst             0x01 //��� ������ - ����������� �����.

#define CmdMBStatusConst         0xA0 //������� "������ ������� �� MB".
#define CmdBroadcastRequestConst 0xFF //����������������� ������.

#define BUTTONS_ALG_ACT  6 //PRIORITY_6	    6
#define BUTTON_START_ACT 5 //PRIORITY_5	    5
#define ChS_ACT				   4 //PRIORITY_4	    4
#define FIRE_LINE_ACT 	 3 //PRIORITY_3	    3
#define BUTTON_PUSK_ACT  2 //PRIORITY_2	    2
#define MIC_ACT 		 		 1 //PRIORITY_1	    1
#define NO_ACT 		 		   0 //��� ���������
//-----------------------------------------------------------------------------
//�����.
typedef struct{
	
	uint8_t FireLineAlert :1;
	uint8_t MBNewDataFlag :1;
	uint8_t MBErrorFlag   :1;
	uint8_t :5;
	
}MBFlagsSTR_Type;

#define FlagClr	0
#define FlagSet	1
//-----------------------------------------------------------------------------
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
//*****************************************************************************
//*****************************************************************************
MBFlagsSTR_Type* MBFlags(void); 

void		 LcSetControl(uint8_t line, uint8_t control);
uint8_t  LcGetControl(uint8_t line);
uint8_t  LcGetControlFromMB(void);

void     LcSave         (uint8_t* p);
uint8_t  LcGetForTxt    (uint8_t ch);
uint8_t  LcGetForLed    (uint8_t ch);
void     LcDeviationSave(uint16_t* p);
uint16_t LcDeviationGet (void);

void     ZoneSave(uint8_t* p);
uint8_t  ZoneGet (uint8_t ch);

void     FireLineSave(uint8_t* p);
uint8_t  FireLineGetPhysState (uint8_t ch);
uint8_t  FireLineGetLogicState(uint8_t ch);

PowerSTR_t* PowerDevice(void);
//*****************************************************************************
//*****************************************************************************
//������ � �������� �������.
//��������� ������ ������� �������
#pragma pack(push, 1)//������ ������������ � 1 ����
typedef struct{
	//--------
	uint32_t EventTimeUtc;//����� �������� �������.
	uint16_t EventNumber; //����� �������.
	uint8_t  EventID;     //��� �������.
	uint8_t  EventParam;  //�������� �������(����� ����, ����� ������ � �.�.).
	//--------
}Event_t;
#pragma pack(pop)//������� ���������� ���������.
//-------------------------------------------------
//��������� ������ ������� �������
#pragma pack(push, 1)//������ ������������ � 1 ����
typedef struct{
	//--------
	uint16_t TotalEvents;  //����� ���������� ������� �������.
	uint16_t NotReadEvents;//���������� ������������ �������.
	uint16_t EventIndex;
	uint16_t EventOldIndex;	
	//--------
}LogWorkReg_t;
#pragma pack(pop)//������� ���������� ���������.
//-------------------------------------------------
#define MaxEvent 	1024
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void 		      LogSaveEvent(uint8_t* dataBuf);
Event_t*      LogEvent(void);
LogWorkReg_t* LogWorkReg(void);
//*****************************************************************************
//*****************************************************************************
//������ � ����������������� ����� �������� �������.
////���������� �������� ������ �������� ������.
//#pragma pack(push, 1)//������ ������������ � 1 ����
//typedef	struct{
//	//-------------
//	uint16_t Number;		//����� �����
//	uint16_t Type;			//����� ������ �����.
//	uint16_t TimeOut;		//����� � ���. �������� �� ������� � ����
//	uint8_t  PhysState; //���������� ��������� �������� �������.
//	uint16_t MeasValue;	//�������� � ��. �� �����.
//	//-------------
//}FireLineInputConfigSTR_Type;
//#pragma pack(pop)//������� ���������� ���������.
//-------------------------------------------------------
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
#define FireLineLogicMask			 0x0F
#define	FireLineCONTROLOFF  	 0 //
#define	FireLineUNDERCONTROL   1 //
#define	FireLineATTENTION 		 2 //��������Ż � ������������� ������������ ������ ����������
#define	FireLineFIRE      		 3 //�����л    � ���� ��������� � ��� ��������� ����� � ������ ��������� �������� � ������� TimeOut. TimeOut �������� � ���������� �����.
//���������� ��������� �������� �������.
#define FireLinePhysMask			0xF0
#define FireLineNorm 			   (0<<4)
#define FireLineUndef 			 (1<<4)
#define FireLineNormOpen  	 (2<<4)
#define FireLineNormClose 	 (3<<4)
#define FireLineShort 			 (4<<4)
#define FireLineBreak     	 (5<<4)
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
FireLine_t* FireLine(uint8_t line);

//************************************************************************************************
//************************************************************************************************
//������ � ����������� EEPROM, ������������ �� ��.

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
EepInfo_t* Eeprom(void);

//************************************************************************************************
//************************************************************************************************
#endif /*_MB_MotherBoard_H*/






















