
#ifndef _LineCheck_H
#define _LineCheck_H
//-----------------------------------------------------------------------------

#include "adc_ST.h"
#include "gpio_ST.h"

#include "Relay.h"
//-----------------------------------------------------------------------------
//Структура контекста для одного канала линии Гр.
#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef struct{
	
	uint8_t Action;  //Состояние линии Гр.
	uint8_t LcStatus;//Состояние неисправностей линии Гр.
	//---------
	uint16_t Meas;       //измеренное значение.
  uint16_t OldMeas;    //старое измеренное значение.
	uint16_t RefValue;	 //Опорные значения. 
	uint16_t SteadyValue;//установившееся значения на линиях после переходных процессов.
	//---------
	//флаги
	uint8_t  RefValueFlag:1;//
	uint8_t  LcFlag      :1;//
	uint8_t  :6;						
	//---------
}SpeakerLineContextSTR_Type;
#pragma pack(pop)//вернули предыдущую настройку.
//-----------------------------------------------
#define LcOptoReleyOnTimeOut    2    //Пауза в мС перед включение оптореле какалов LC.
#define LcMeasPeriod            2000 //Период переключения между каналами измерения LC.

#define LcLedBlinkPperiodConst  100  //Интервал в мС изменения состояни сетодиодаво время переходного процесса.
#define LcDeviationDefaultConst 50   //Отклонение в мВ от установившегося значения для фиксации переходного процесс на линии.
#define LcErrorThresholdConst   10   //
#define LineShorThresholdConst  100  //Порого в мВ, ниже которого фиксируется КЗ на линии Гр.
#define LineBreakThresholdConst	2650 //Порого в мВ, выше которого фиксируется обрыв линии Гр.
//--------------------
#define Off			0
#define On		  1
#define Toggel  2
//Постановка линий Гр. на контроль.
#define ControlOff	0
#define ControlOn		1
//Линии Гр.
#define SpLine1       0
#define SpAtten1      1
#define SpLine2       2
#define SpAtten2      3
#define SpAllLines 	  0xFF
#define SpAllLinesOff 0
//Общее количество линий Гр.
#define SpeakerLineNumbers 	4
//Состояния линий Гр.
#define	LineMask		  	 0xf0
#define LineOffControl 	(0<<4)
#define LineNorm     		(1<<4) 
#define LineUndef    		(2<<4)//Состояние не определено - идет переходной процесс.
#define LineBreak    		(3<<4)
#define LineOver     		(4<<4)
#define LineUnder    		(5<<4)
#define LineShort    		(6<<4)
//--------------------
#define FlagClr	0
#define FlagSet	1
//--------------------
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
//-----------------------------------------------------------------------------
void SpeakerLineInit     			 (void);
void SpeakerLineMeasLoop       (void);
void SpeakerLineSteadyStateLoop(void); //Получение установившегося значения после переходного процесса.

volatile uint8_t*           SpeakerLineControl  (void);
volatile uint16_t*          SpeakerLineDeviation(void);
SpeakerLineContextSTR_Type* SpeakerLine(uint8_t speakerLine);

uint8_t SpeakerLineSaveRef     (uint8_t ch);  //Сохранение опорных величин линий Гр. дя LC.
uint8_t SpeakerLineGetDefStatus(uint8_t ch);  //

//Машина состояний для управления линией Гр.
void SpeakerLineFSM(uint8_t speakerLine);
//-----------------------------------------------------------------------------
#endif /*_LineCheck_H*/



