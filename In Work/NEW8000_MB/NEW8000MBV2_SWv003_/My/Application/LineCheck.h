
#ifndef _LineCheck_H
#define _LineCheck_H

//*****************************************************************************

#include "adc_ST.h"
#include "gpio_ST.h"

#include "Relay.h"

//*****************************************************************************
//*****************************************************************************
//Настраиваемые параметры для работы контроля линии Гр.
#pragma pack(push, 1)//размер выравнивания в 1 байт.
typedef struct{
	//---------
	uint16_t Deviation;//Отклонение в мВ от установившегося значения для фиксации перех-го проц-са на линии.
	uint8_t	 Check;    //Линии Гр. установленные на контроль.	
	//---------
}SpLineParam_t;
#pragma pack(pop)//вернули предыдущую настройку.
//***********************************************
//Структура контекста для одного канала линии Гр.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	//---------	
	uint8_t IsActivated :4;//Состояние линии Гр.
	uint8_t LcStatus    :4;//Состояние неисправностей линии Гр.	
	//---------
	uint16_t Meas;       //измеренное значение.
  uint16_t OldMeas;    //старое измеренное значение.
	uint16_t RefValue;	 //Опорные значения. 
	uint16_t SteadyValue;//установившееся значения на линиях после переходных процессов.
	//---------
	//флаги
	uint8_t  FirstOnFlag:1;//
	uint8_t  LcFlag     :1;//
	uint8_t  :6;						
	//---------
}SpLineContext_t;
#pragma pack(pop)//вернули предыдущую настройку.
//***********************************************
#define SP_LINE_OPTO_RELAYL_PERIOD  2u    //Пауза в мС перед включение оптореле какалов LC.
#define SP_LINE_MEAS_PERIOD     		2000u //Период переключения между каналами измерения LC.
#define SP_LINE_LED_BLINK_PERIOD	  100u  //Интервал в мС изменения состояни сетодиодаво время переходного процесса.

#define SP_LINE_SHORT_TRESHOLD  	  100u  //Порог в мВ, ниже которого фиксируется КЗ на линии Гр.
#define SP_LINE_BREAK_TRESHOLD		  2650u //Порог в мВ, выше которого фиксируется обрыв линии Гр.

//Отклонение в мВ от установившегося значения для фиксации переходного процесс на линии.
#define SP_LINE_DEVIATION_DEFAULT 50u 
#define SP_LINE_DEVIATION_MIN	    1u
#define SP_LINE_DEVIATION_MAX	    500u
//***********************************************
//#define OFF			0u
//#define ON		  1u
#define TOGGLE  2u
//Линии Гр.
#define SP_LINE_1				0
#define ATTEN_LINE_1		1u
#define SP_LINE_2				2u
#define ATTEN_LINE_2  	3u
//Общее количество линий Гр.
#define SP_LINE_ALL_ON 	0xFFu

#define SP_LINE_ALL_OFF	0
#define SP_LINE_NUMBER	4u
//--------------------
#define FLAG_CLEAR	0
#define FLAG_SET  	1
//***********************************************
//Состояния линий Гр.
#define SP_LINE_CHECK_OFF		0
#define SP_LINE_NORM     		1 
#define SP_LINE_UNDEF    		2 //Состояние не определено - идет переходной процесс.
#define SP_LINE_BREAK				3
#define SP_LINE_OVER 				4
#define SP_LINE_UNDER				5
#define SP_LINE_SHORT			 	6
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

//*****************************************
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
//*****************************************************************************
//*****************************************************************************
void 					   SpeakerLine_Init     			(void);
void 					   SpeakerLine_MeasLoop       (void);
//void 					   SpeakerLine_SteadyStateLoop(void); //Получение установившегося значения после переходного процесса.
SpLineParam_t*   SpeakerLine_Param       (void);
SpLineContext_t* SpeakerLine             (uint8_t speakerLine);
uint8_t 				 SpeakerLine_SaveRef     (uint8_t ch);
uint8_t 				 SpeakerLine_GetDefStatus(uint8_t ch);//
void 						 SpeakerLine_FSMLoop     (void);      //Машина состояний для управления линией Гр.
//*****************************************************************************
//*****************************************************************************
#endif /*_LineCheck_H*/



