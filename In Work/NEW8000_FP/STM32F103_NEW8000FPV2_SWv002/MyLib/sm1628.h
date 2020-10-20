
#ifndef _sm1628_H
#define _sm1628_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"
#include "spi_ST.h"

//-----------------------------------------------------------------------------
//Сигнал CS_SM1628.
#define CsSm1628Hiht 	  (GPIOB->BSRR = GPIO_BSRR_BS12) 
#define CsSm1628Low  	  (GPIOB->BSRR = GPIO_BSRR_BR12) 
#define CsSm1628Toggel 	(GPIOB->ODR  = GPIOB->ODR ^ GPIO_ODR_ODR12)

//Команды SM1628
#define CMD1_7DIGITS_10SEG	0x03  //(0b00000011)
#define CMD2_WRITE			    0x40  //(0b01000000)
#define CMD2_INCREMEN_ADDR	(0b01000100)
#define CMD2_READ			      0x42  //(0b01000010)
#define CMD3_ADDRESS_00		  0xc0  //(0b11000000)
#define CMD4_DISPLAY_ON		  0x88  //(0b10001000)//0b10001111
#define CMD4_DISPLAY_OFF	  0x87  //(0b10000111)

#define CMD3				        (0b11000000)
#define CMD3_ADDRESS_01		  (0b11000001)
#define CMD3_ADDRESS_02		  (0b11000010)
#define CMD3_ADDRESS_03		  (0b11000011)
#define CMD3_ADDRESS_04		  (0b11000100)
#define CMD3_ADDRESS_05		  (0b11000101)
#define CMD3_ADDRESS_06		  (0b11000110)
#define CMD3_ADDRESS_07		  (0b11000111)

//Яркость светодиодов на лицевой панели по умолчанию.
#define DEFAULT_BRIGHT_DISPLAY	2 //4

//Проверочные биты.
#define CHECKBIT1	(1<<2)
#define CHECKBIT2	(1<<5)
//
#define CONTROL_BUTTONS	    2
#define PRESET_BUTTONS	    4
//-----------------------------------------------------------------------------
//Струтура хранения состояния кнопок.
typedef union {
	struct{
		//----------------------------------------
		unsigned char ControlButtonTemporarySost; //Кнопки 
		unsigned char ControlButtonSost;			    //
		//----------------------------------------
		unsigned char PresetButtonTemporarySost;	//Кнопки 
		unsigned char PresetButtonSost;			      //
		//----------------------------------------
	}STR;
	unsigned char BLK[4];
}ButtonStateSTR_TypeDef;
//-----------------------------------------------------------------------------
//Структура состояния кнопок, вычитанная из PT6964.
#define  QUANTITY_READ_BYTE		5 //Количество вычитываемых байт из PT6964.

#pragma pack(push, 1)//размер выравнивания в 1 байт
//typedef volatile union {
typedef volatile union {
	struct{
		//----------------------------------------
		//0-й байт - 
		unsigned char	ReserveBit1:		1;	//0-й бит.
		unsigned char	PRESET8_BUT:	  1;	//
		unsigned char	CheckBit1:		  1;	//Бит используется как проверочный.
		unsigned char	Test_BUT:		    1;	//
		unsigned char	PRESET7_BUT:	  1;	//
		unsigned char	CheckBit2:		  1;	//Бит используется как проверочный.
		unsigned char	not_relevant1:  2;	//
		//----------------------------------------
		//1-й байт.
		unsigned char	ZUMMER_BUT:		  1;	//0-й бит.
		unsigned char	PRESET6_BUT:		1;	//
		unsigned char	CheckBit3:		  1;	//Бит используется как проверочный.
		unsigned char	RESET_FAULT_BUT:1;	//
		unsigned char	PRESET5_BUT:		1;	//
		unsigned char	CheckBit4:		  1;	//Бит используется как проверочный.
		unsigned char	not_relevant2:	2;	//
		//----------------------------------------
		//2-й байт.
		unsigned char	RESET_ALERT_BUT:1;	//0-й бит.
		unsigned char	PRESET4_BUT:		1;	//
		unsigned char	CheckBit5:		  1;	//Бит используется как проверочный.
		unsigned char	STOP_BUT:		    1;	//
		unsigned char	PRESET3_BUT:		1;	//
		unsigned char	CheckBit6:		  1;	//Бит используется как проверочный.
		unsigned char	not_relevant3:	2;	//
		//----------------------------------------
		//3-й байт.
		unsigned char	START_BUT:		  1;	//0-й бит.
		unsigned char	PRESET2_BUT:		1;	//
		unsigned char	CheckBit7:		  1;	//Бит используется как проверочный.
		unsigned char	ReserveBit2:		1;	//
		unsigned char	PRESET1_BUT:		1;	//
		unsigned char	CheckBit8:		  1;	//Бит используется как проверочный.
		unsigned char	not_relevant4:	2;	//
		//----------------------------------------
		//4-й байт.
		unsigned char	ReserveBit3:		1;	//0-й бит.
		unsigned char	ReserveBit4:		1;	//
		unsigned char	CheckBit9:		  1;	//Бит используется как проверочный.
		unsigned char	ReserveBit5:		1;	//
		unsigned char	ReserveBit6:		1;	//
		unsigned char	CheckBit10:		  1;	//Бит используется как проверочный.
		unsigned char	not_relevant5:	2;	//	
		//----------------------------------------	
	}bit;
	unsigned char BLK[QUANTITY_READ_BYTE];//volatile unsigned char BLK[QUANTITY_READ_BYTE];
}ButtonSTR_TypeDef;
#pragma pack(pop)//вернули предыдущую настройку.
//-----------------------------------------------------------------------------
//Cтруктура данных для управления светодиодами на лицевой панеле.
#define  QUANTITY_FACE_PANEL_BYTE	8U //11U 

#pragma pack(push, 1)//размер выравнивания в 1 байт
typedef union {
	struct{
	//----------------------------------------
	//0-й байт - GR1.	
		unsigned char	PRESET8_RED_LED_bit:  	1;	//0-й бит.
		unsigned char	PRESET8_GREEN_LED_bit:	1;	//
		unsigned char	PRESET7_RED_LED_bit:	  1;	//
		unsigned char	PRESET7_GREEN_LED_bit:	1;	//
		unsigned char	PRESET6_RED_LED_bit:	  1;	//
		unsigned char	PRESET6_GREEN_LED_bit:	1;	//
		unsigned char	PRESET5_RED_LED_bit:	  1;	//
		unsigned char	PRESET5_GREEN_LED_bit:	1;	//7-й бит.
	//----------------------------------------
	//1-й байт - GR1.
		unsigned char	not_relevant2:	8;	//
	//----------------------------------------	
	//2-й байт - GR2.
		unsigned char	Communication_GREEN_LED_bit:1;	//16-й бит.
		unsigned char	Communication_RED_LED_bit:	1;	//
		unsigned char	Amplifier_GREEN_LED_bit:	  1;	//
		unsigned char	Amplifier_RED_LED_bit:	    1;	//
		unsigned char	Battery_GREEN_LED_bit:	    1;	//
		unsigned char	Battery_RED_LED_bit:	      1;	//
		unsigned char	Power_GREEN_LED_bit:	      1;	//
		unsigned char	Power_RED_LED_bit:	        1;	//23-й бит.
	//----------------------------------------
	//3-й байт - GR2.
		unsigned char	not_relevant4:	8;	//
	//----------------------------------------
	//4-й байт - GR3.
		unsigned char	PRESET4_GREEN_LED_bit:  1;	//32-й бит.
		unsigned char	PRESET4_RED_LED_bit:	  1;	//
		unsigned char	PRESET3_GREEN_LED_bit:	1;	//
		unsigned char	PRESET3_RED_LED_bit:	  1;	//
		unsigned char	PRESET2_GREEN_LED_bit:	1;	//
		unsigned char	PRESET2_RED_LED_bit:	  1;	//
		unsigned char	PRESET1_GREEN_LED_bit:	1;	//
		unsigned char	PRESET1_RED_LED_bit:	  1;	//39-й бит.
	//----------------------------------------
	//5-й байт - GR3.
		unsigned char	not_relevant6:	8;	//
	//----------------------------------------
	//6-й байт - GR4.		
		unsigned char	TEST_LED_bit:	          1;	//48-й бит.
		unsigned char	ZUMMER_LED_bit:	        1;	//
		unsigned char	RESET_LED_bit:	        1;	//
		unsigned char	START_LED_bit:	        1;	//
		unsigned char	FIRE_LED_bit:	          1;	//
		unsigned char	MIC_GREEN_LED_bit:	    1;	//	
		unsigned char	MIC_RED_LED_bit:	      1;	//
		unsigned char	GENERAL_FAULT_LED_bit:	1;	//55-й бит.	
	//----------------------------------------
	//7-й байт - GR4.
		unsigned char	ALERT_LINES_FAUL_LED_bit:	1;	//56-й бит.
		unsigned char	FIRE_LINES_FAUL_LED_bit:  1;	//
		unsigned char	not_relevant8:            6;	//63-й бит.
	//----------------------------------------
	//8-й байт.
		unsigned char	RIGHT_LED_PANEL_SEG_a:	1;	//
		unsigned char	RIGHT_LED_PANEL_SEG_b:	1;	//
		unsigned char	RIGHT_LED_PANEL_SEG_c:	1;	//
		unsigned char	RIGHT_LED_PANEL_SEG_d:	1;	//
		unsigned char	RIGHT_LED_PANEL_SEG_e:	1;	//
		unsigned char	RIGHT_LED_PANEL_SEG_f:	1;	//
		unsigned char	RIGHT_LED_PANEL_SEG_g:	1;	//
		unsigned char	RS485_LED:	            1;	//
//	//----------------------------------------
//	//9-й байт.
//		unsigned char	STBAY_LED:	1;	//
//		unsigned char	POWER_LED:	1;	//
//		unsigned char	not_relevant10:	6;	//
//	//----------------------------------------
//	//10-й байт.
//		unsigned char	PROT_LED:	1;	//
//		unsigned char	not_relevant11:	7;	//
//	//----------------------------------------
			}bit;
	unsigned char BLK[QUANTITY_FACE_PANEL_BYTE];
}IndicatorSTR_TypeDef;
#pragma pack(pop)//вернули предыдущую настройку.
//---------------------------------------------

//extern IndicatorSTR_TypeDef	  IndicatorsFacePanelStr;
//extern ButtonSTR_TypeDef		  ButtonFacePanelStr;
//extern ButtonStateSTR_TypeDef	ButtonStateStr;

//-----------------------------------------------------------------------------
//static void Sm1628SendByte(uint8_t byte);
//static void Sm1628SendCmd (uint8_t cmd);
//void        Sm1628Init(void);

void    Sm1628SendDataBuf(void);
uint8_t Sm1628ReadButton (void);
uint8_t SortingButtons   (uint8_t button);

//-----------------------------------------------------------------------------
#endif //_sm1628_H


