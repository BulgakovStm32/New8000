
#ifndef _LineCheck_H
#define _LineCheck_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//-----------------------------------------------------------------------------
//Состояния линий Гр.
#define	Line1  1	
#define	Att1   2	
#define	Line2  3	
#define	Att2   4	

#define	LineMask		  0xf0
#define LineNorm     (1<<4) 
#define LineUndef    (2<<4)//Состояние не определено - идет переходной процесс.
#define LineBreak    (3<<4)
#define LineOver     (4<<4)
#define LineUnder    (5<<4)
#define LineShort    (6<<4)

//Состояния линий пожарных шлейфов.
#define Poj1   1    
#define Poj2   2     
#define Poj3   3   
#define ChS    4 

#define NormSost          1//(1<<0)
#define NormOpenSost      2//(1<<1)
#define NormCloseSost     3//(1<<2)
#define ShortLineSost     4//(1<<3)
#define BreakLineSost     5//(1<<4)

//Состояния основного питания 220V и инвертора.
#define PowerMask       0x0F
#define PowerBlink     (0<<0)//Для мигания.
#define PowerACOk      (1<<0)//Есть питание 220V
#define PowerDCOk      (2<<0)//Инвертор исправен.
#define PowerACNo      (3<<0)//Питание 220V отсутствует
#define PowerDCFault   (4<<0)//Инвертор неисправен.
#define PowerFault     (5<<0)//Неисправны оба блока питания.

//Состояние батареи.
#define BatMask         0xF0
#define BatBlink       (0<<4)//Для мигания.
#define BatOk          (1<<4)//Напряжение на АКБ от 21В до 27,2В.
#define BatCharge      (2<<4)//Идет заряд АКБ
#define BatChargeEnd   (3<<4)//Заряд окончен, напряжение АКБ 27,2В.
#define BatAttention   (4<<4)//Напряжение на АКБ <= 20,5В.
#define BatDeep        (5<<4)//Напряжение на АКБ <= 18,5В - глубокий разряд АКБ.
#define BatNo          (6<<4)//АКБ отсутствует.

//Состояние УМЗЧ.
#define AmpBlink       (0<<0)//Для мигания.
#define AmpDisactive   (1<<0)//УМЗЧ отключен.
#define AmpActive      (2<<0)//УМЗЧ активен.
#define AmpProt        (3<<0)//УМЗЧ в защите.
//-----------------------------------------------------------------------------
void     LcFpSet       (uint8_t* p);
uint8_t  LcFpGetForTxt (uint8_t ch);
uint8_t  LcFpGetForLed (uint8_t ch);
void     LcDeviationSet(uint16_t* p);
uint16_t LcDeviationGet(void);

void     PojLineFpSet(uint8_t* p);
uint8_t  PojLineFpGet(uint8_t ch);

void     PowerAndBatFpSet(uint8_t* p);
uint8_t  PowerFpGet      (void);
uint8_t  BatFpGet        (void);

void     AmpFpSet(uint8_t* p);
uint8_t  AmpFpGet(void);
//-----------------------------------------------------------------------------
#endif /*_LineCheck_H*/



