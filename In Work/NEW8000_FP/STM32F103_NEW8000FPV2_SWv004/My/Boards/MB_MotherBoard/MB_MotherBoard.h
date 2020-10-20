
#ifndef _MB_MotherBoard_H
#define _MB_MotherBoard_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//-----------------------------------------------------------------------------
//Постановка линий Гр. на контроль.
#define ControlOff	0
#define ControlOn		1

#define SP_CHECK_ON  1
#define SP_CHECK_OFF 0
//Линий Гр.
#define	Line1  			0	
#define	Att1   			1	
#define	Line2  			2	
#define	Att2   			3
#define AllLines 		0xFF
//Состояния линий Гр.
#define	LineOffControl	0	
#define LineNorm     		1 
#define LineUndef    		2 //Состояние не определено - идет переходной процесс.
#define LineBreak    		3
#define LineOver     		4
#define LineUnder    		5
#define LineShort    		6
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
#define	ActionOff		   			0 //зона отключена.
#define	ActionManualOn 			1 //зона активирована в ручную, нажатием кнопки ПРЕСЕТ1-ПРЕСЕТ8.
#define	ActionConsolOn 			2 //зона активирована консолью.
#define	ActionMicOn    			3 //зона активирована приоритетным микрофоном МИКР.
#define	ActionPuskOn   			4 //зона активирована кнопкой ПУСК.
#define	ActionFireLine1On		5 //зона активирована пожарным шлейфом Пож.1.
#define	ActionFireLine2On	  6 //зона активирована пожарным шлейфом Пож.2.
#define	ActionFireLine3On	  7 //зона активирована пожарным шлейфом Пож.3. 
#define	ActionFireLineChSOn	8 //зона активирована пожарным шлейфом ГОиЧС.
#define	ActionIPOn	   			9 //зона активирована платой IP.
#define	ActionAutoOn	 			10//зона активирована атоматикой. - нужно подумать, возможно это состояние не нужно.
#define	ActionAutoOff	 			11//Автоматика отключена - производится нажатием кнопки СБРОС ОПОВЕЩ.  
#define	ActionDefect   			12//

//Кем был активирован выход Гр.
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
#pragma pack(push, 1)//размер выравнивания в 1 байт

typedef struct{
	//--------------------
	uint8_t OutState  :4;
	uint8_t LineState :4;	
	//--------------------
}Line_t;
//**************************************************
//Структура данных ЦП.
//#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	//--------------------
	uint32_t TimeUTC;
	//--------------------	
	//Состояние журнала событий.
	uint16_t TotalEvents;
	uint16_t NotReadEvents;
	//--------------------
	//Состояния оповещателей.
	uint16_t SirenPower;	 
	Line_t   Siren1;
	Line_t   Siren2;
	Line_t   Siren3;
	//--------------------	
	uint16_t DebugData1;
	uint16_t DebugData2;
	//--------------------
	//Линии Гр.
	uint16_t  SpLineDeviation;
	Line_t 		Sp1;
	Line_t 		Sp2;
	Line_t 		Sp3;
	Line_t 		Sp4;
	Line_t 		Sp5;
	Line_t 		Sp6;	
	//--------------------
	//Линии шлейфов сигнализайии.
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
	//Состояния платы IP.		

	//--------------------
	//Состояния платы ZE.		

	//--------------------
}MBData_t;
#pragma pack(pop)//вернули предыдущую настройку.
#define MOTHER_BOARD_DATA_SIZE ((uint8_t)sizeof(MBData_t))
//**************************************************
#define MB_TYPE             0x01 //Тип модуля - материнская плата.
//Команды MB.
#define MB_CMD_SEND_STATUS  0xA0 //Команда "Выдача статуса от MB".
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
#define NO_ACT 		 		   0 //Нет активации
//*****************************************************************************************
//*****************************************************************************************
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
//*********************************
#pragma pack(push, 1)//размер выравнивания в 1 байт
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
#pragma pack(pop)//вернули предыдущую настройку.
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
//Работа с журналом событий.
//Структура одного события системы
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	//--------
	uint32_t TimeUtc;//Время фиксации события.
	uint16_t Number; //Номер события.
	uint8_t  ID;     //Код события.
	uint8_t  Param;  //Параметр события(номер зоны, номер шлейфа и т.п.).
	//--------
}Event_t;
#pragma pack(pop)//вернули предыдущую настройку.
//-------------------------------------------------
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	//--------
	Event_t  Event;
	uint16_t TotalEvents;  //Общее количество записей событий.
	uint16_t NotReadEvents;//Количество непрочитаных записей.
	uint16_t EventIndex;
	uint16_t EventOldIndex;	
	//--------
}Log_t;
#pragma pack(pop)//вернули предыдущую настройку.
//-------------------------------------------------
#define MaxEvent 	1024
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void 	 LogSaveEvent(uint8_t* dataBuf);
Log_t* Log(void);
//************************************************************************************************
//************************************************************************************************
//Работа с конфигурированием линий пожарных шлейфов.
//Логический контекст одного входного шлейфа.
#pragma pack(push, 2)//размер выравнивания в 2 байта
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
#pragma pack(pop)//вернули предыдущую настройку.
//-------------------------------------------------
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
#define	FR_IN_CONTROL_OFF    0u //
#define	FR_IN_CONTROL_ON     1u //
#define	FR_IN_ATTENTION 		 2u //«ВНИМАНИЕ» – зафиксировано срабатывание одного извещателя
#define	FR_IN_FIRE      		 3u //«ПОЖАР»    – вход переходит в это состояние через в случае удержания сработки в течении TimeOut. TimeOut задается в настройках входа.
//Физические состояния Пожарных шлейфов.
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






















