
#ifndef _MB_MotherBoard_H
#define _MB_MotherBoard_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//-----------------------------------------------------------------------------
//Постановка линий Гр. на контроль.
#define ControlOff	0
#define ControlOn		1
//Линий Гр.
#define	Line1  			0	
#define	Att1   			1	
#define	Line2  			2	
#define	Att2   			3
#define AllLines 		0xFF
//Состояния линий Гр.
#define	LineMask		  	 0xF0
#define	LineOffControl	(0<<4)	
#define LineNorm     		(1<<4) 
#define LineUndef    		(2<<4)//Состояние не определено - идет переходной процесс.
#define LineBreak    		(3<<4)
#define LineOver     		(4<<4)
#define LineUnder    		(5<<4)
#define LineShort    		(6<<4)
//----------------------------------------
//Работа с зонами.
#define	Zone1   0 //(1<<0)
#define	Zone2   1 //(1<<1)		  
#define	Zone3   2 //(1<<2) 
#define	Zone4   3 //(1<<3)
#define	Zone5   4 //(1<<4) 
#define	Zone6   5 //(1<<5)
#define	ZoneAll 6 //(Zone1 | Zone2 | Zone3 | Zone4 | Zone5 | Zone6)

#define	On   1 
#define	Off  0
//Действия с линией Гр.
#define	ActionMask		  	 	 0x0f
#define	ActionOff		   			(0<<0) //зона отключена.
#define	ActionManualOn 			(1<<0) //зона активирована в ручную, нажатием кнопки ПРЕСЕТ1-ПРЕСЕТ8.
#define	ActionConsolOn 			(2<<0) //зона активирована консолью.
#define	ActionMicOn    			(3<<0) //зона активирована приоритетным микрофоном МИКР.
#define	ActionPuskOn   			(4<<0) //зона активирована кнопкой ПУСК.
#define	ActionFireLine1On		(5<<0) //зона активирована пожарным шлейфом Пож.1.
#define	ActionFireLine2On	  (6<<0) //зона активирована пожарным шлейфом Пож.2.
#define	ActionFireLine3On	  (7<<0) //зона активирована пожарным шлейфом Пож.3. 
#define	ActionFireLineChSOn	(8<<0) //зона активирована пожарным шлейфом ГОиЧС.
#define	ActionIPOn	   			(9<<0) //зона активирована платой IP.
#define	ActionAutoOn	 			(10<<0)//зона активирована атоматикой. - нужно подумать, возможно это состояние не нужно.
#define	ActionAutoOff	 			(11<<0)//Автоматика отключена - производится нажатием кнопки СБРОС ОПОВЕЩ.  
#define	ActionDefect   			(12<<0)//
//----------------------------------------
//Состояния основного питания 220V и инвертора.
#define PowerMask        0x0F
#define PowerBlink     	(0<<0) //Для мигания.
#define PowerControlOff	(1<<0) //контроль отключен.
#define PowerACOk      	(2<<0) //Есть питание 220V
#define PowerDCOk      	(3<<0) //Инвертор исправен.
#define PowerACNo       (4<<0) //Питание 220V отсутствует
#define PowerDCFault    (5<<0) //Инвертор неисправен.
#define PowerFault      (6<<0) //Неисправны оба блока питания.
//Состояние батареи.
#define BatMask          0xF0
#define BatBlink        (0<<4) //Для мигания.
#define BatControlOff   (1<<4) //контроль отключен.
#define BatOk           (2<<4) //Напряжение на АКБ от 21В до 27,2В.
#define BatCharge       (3<<4) //Идет заряд АКБ
#define BatChargeEnd    (4<<4) //Заряд окончен, напряжение АКБ 27,2В.
#define BatAttention    (5<<4) //Напряжение на АКБ <= 20,5В.
#define BatDeep         (6<<4) //Напряжение на АКБ <= 18,5В - глубокий разряд АКБ.
#define BatNo           (7<<4) //АКБ отсутствует.
//Состояние УМЗЧ.
#define AmpBlink        (0<<0) //Для мигания.
#define AmpDisactive    (1<<0) //УМЗЧ отключен.
#define AmpActive       (2<<0) //УМЗЧ активен.
#define AmpProt         (3<<0) //УМЗЧ в защите.
//*****************************************************************************
//*****************************************************************************
//Структура данных ЦП.
#pragma pack(push, 1)//размер выравнивания в 1 байт
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
	//Состояние журнала событий.
	uint16_t TotalEvents;
	uint16_t NotReadEvents;
	//--------------------
	//Состояния оповещателей.
	uint16_t SirenPower;	 
	uint16_t Siren1Status; 
	uint16_t Siren2Status; 
	uint16_t Siren3Status; 
	//--------------------
	//Состояния платы IP.		

	//--------------------
	//Состояния платы ZE.		

	//--------------------
	//uint16_t CRC16;
	//--------------------
}MBData_t;
#pragma pack(pop)//вернули предыдущую настройку.
//*****************************************************************************
//*****************************************************************************
//Структура данных FP.
#pragma pack(push, 1)//размер выравнивания в 1 байт
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
#pragma pack(pop)//вернули предыдущую настройку.
//*****************************************************************************
//*****************************************************************************
///Работа с конфигурированием линий пожарных шлейфов.
//Логический контекст одного входного шлейфа.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef	struct{
	//-------------
	uint8_t  Number;
	uint8_t  Type;  
	uint8_t  TimeOut;
	uint8_t  PhysState;
	uint16_t MeasValue;	
	//-------------
}FireLine_t;
#pragma pack(pop)//вернули предыдущую настройку.
//*****************************************************************************
//*****************************************************************************
//Работа с микросхемой EEPROM, устанвленной на ЦП.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef	struct{
	//-------------
	uint32_t BadSectorsCount;//
	uint32_t WriteCount;     //Количество записей
	uint32_t ReadCount;	     //Количество чтений.	
	uint8_t  StatReg;        //Регистр состояния.
	//-------------
}EepInfo_t;
#pragma pack(pop)//вернули предыдущую настройку.
//*****************************************************************************
//*****************************************************************************
#define RS485_BUF_SIZE         64
#define RS485_BUF_HEADER_SIZE  5
//буфер для работы протокола.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	//Заголовок пакета
	uint8_t SinhroByte1;// Синхробайт.
	uint8_t SinhroByte2;// Синхробайт.
	uint8_t Type;       // Тип модуля.
	uint8_t Size;       // Размер пакета.
	uint8_t CmdCode;    // Код команды.	
	//Данные.
	union
		{
			uint8_t Buf[RS485_BUF_SIZE];//буфер.
			//----------
			FireLine_t FireLine;//данные SB.
			EepInfo_t  EepInfo; //
			//----------
			struct 
				{
					MBData_t Status;//данные ЦП.
					uint16_t CRC16;
				}MB;
			//----------
			struct
				{
					FPData_t Status;//данные FP.
					uint16_t CRC16;
				}FP;
			//----------
		}Data;
	//-----------
}RS485BufUnion_t;
#pragma pack(pop)//вернули предыдущую настройку.
//*****************************************************************************************
//*****************************************************************************************
//Команды MB.
#define MB_DefultAddressConst    0x01 //Значение адреса материнской платы по умолчанию.
#define MB_TypeConst             0x01 //Тип модуля - материнская плата.

#define CmdMBStatusConst         0xA0 //Команда "Выдача статуса от MB".
#define CmdBroadcastRequestConst 0xFF //Широковещательный запрос.

#define BUTTONS_ALG_ACT  6 //PRIORITY_6	    6
#define BUTTON_START_ACT 5 //PRIORITY_5	    5
#define ChS_ACT				   4 //PRIORITY_4	    4
#define FIRE_LINE_ACT 	 3 //PRIORITY_3	    3
#define BUTTON_PUSK_ACT  2 //PRIORITY_2	    2
#define MIC_ACT 		 		 1 //PRIORITY_1	    1
#define NO_ACT 		 		   0 //Нет активации
//-----------------------------------------------------------------------------
//Флаги.
typedef struct{
	
	uint8_t FireLineAlert :1;
	uint8_t MBNewDataFlag :1;
	uint8_t MBErrorFlag   :1;
	uint8_t :5;
	
}MBFlagsSTR_Type;

#define FlagClr	0
#define FlagSet	1
//-----------------------------------------------------------------------------
//Состояния основного питания 220V, инвертора И УМЗЧ.
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
//Работа с журналом событий.
//Структура одного события системы
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	//--------
	uint32_t EventTimeUtc;//Время фиксации события.
	uint16_t EventNumber; //Номер события.
	uint8_t  EventID;     //Код события.
	uint8_t  EventParam;  //Параметр события(номер зоны, номер шлейфа и т.п.).
	//--------
}Event_t;
#pragma pack(pop)//вернули предыдущую настройку.
//-------------------------------------------------
//Структура одного события системы
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	//--------
	uint16_t TotalEvents;  //Общее количество записей событий.
	uint16_t NotReadEvents;//Количество непрочитаных записей.
	uint16_t EventIndex;
	uint16_t EventOldIndex;	
	//--------
}LogWorkReg_t;
#pragma pack(pop)//вернули предыдущую настройку.
//-------------------------------------------------
#define MaxEvent 	1024
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void 		      LogSaveEvent(uint8_t* dataBuf);
Event_t*      LogEvent(void);
LogWorkReg_t* LogWorkReg(void);
//*****************************************************************************
//*****************************************************************************
//Работа с конфигурированием линий пожарных шлейфов.
////Логический контекст одного входного шлейфа.
//#pragma pack(push, 1)//размер выравнивания в 1 байт
//typedef	struct{
//	//-------------
//	uint16_t Number;		//Номер входа
//	uint16_t Type;			//Режим работы входа.
//	uint16_t TimeOut;		//Время в сек. перехода из НИМАНИЕ в ПУСК
//	uint8_t  PhysState; //Физическое состояния Пожарных шлейфов.
//	uint16_t MeasValue;	//Значение в мв. на входе.
//	//-------------
//}FireLineInputConfigSTR_Type;
//#pragma pack(pop)//вернули предыдущую настройку.
//-------------------------------------------------------
//Линии шлейфов пожарной сигнализации
#define Poj1   								0
#define Poj2   								1
#define Poj3   								2
#define ChS    								3
//Время в сек. перехода из ВНИМАНИЕ в ПУСК
#define	InputTimeOutMin 			0
#define	InputTimeOutMax 			99
//Режим работы входа.Поле Type логического контекста входа.
#define	InputOff              0 // вход откл. и не контролируется; 
#define	InputOnControlOnClose	1 // вход вкл. и контролируется, фиксация замыкания СК;
#define	InputOnControlOnOpen  2 // вход вкл. и контролируется, фиксация размыкания СК;

//#define	InputOnControlOff     1 // вход вкл. и не контролируется;
//#define	InputOnControlOnClose	2 // вход вкл. и контролируется, фиксация замыкания СК;
//#define	InputOnControlOnOpen  3 // вход вкл. и контролируется, фиксация размыкания СК;
//#define	InputOnControlOnClOp  4 // вход вкл. и контролируется, фиксация замыкания и размыкания СК;
//Cостояния Пожарных шлейфов.
//Логические состояния Пожарных шлейфов.
#define FireLineLogicMask			 0x0F
#define	FireLineCONTROLOFF  	 0 //
#define	FireLineUNDERCONTROL   1 //
#define	FireLineATTENTION 		 2 //«ВНИМАНИЕ» – зафиксировано срабатывание одного извещателя
#define	FireLineFIRE      		 3 //«ПОЖАР»    – вход переходит в это состояние через в случае удержания сработки в течении TimeOut. TimeOut задается в настройках входа.
//Физические состояния Пожарных шлейфов.
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
//Работа с микросхемой EEPROM, устанвленной на ЦП.

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
EepInfo_t* Eeprom(void);

//************************************************************************************************
//************************************************************************************************
#endif /*_MB_MotherBoard_H*/






















