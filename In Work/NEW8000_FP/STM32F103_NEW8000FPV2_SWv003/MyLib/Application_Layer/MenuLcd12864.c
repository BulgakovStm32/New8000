
#include "MenuLcd12864.h"
#include "ascii_RusText.h"  //надписи на русском языке.
//-----------------------------------------------------------------------------
//extern unsigned char        bmp6[];
//extern unsigned char        mybmp[];
extern const unsigned char  LogoRoxton[];
//extern const unsigned char  Spicker[];
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

RusTextSTR_Type RusTextSTR;

//-----------------------------------------------------------------------------

MenuPageSTR_Type NoPage= {"Empty Page"};

//-----------------------------------------------------------------------------
//1-я страница меню.
MenuPageSTR_Type MenuPage1 = { 

    RusText_CofigModeUnit,//"Configuration Mode", 
    RusText_Address,      //"Adress - ", 
    RusText_Group,        //"Group  - ", 
    RusText_Time,         //"Time   - ", 
    RusText_Date,         //"Item4  - ", 
    RusText_ItemMenu,     //"Item5  - ", 
    RusText_ItemMenu,
};
//-----------------------------------------------------------------------------
//2-я страница меню - состояние пожарных шлейфов.
MenuPageSTR_Type MenuPage2 = {

    RusText_PojLine, //"Menu>Page4", 
    RusText_Poj1,  //"Item41 - ", 
    RusText_Poj2,  //"Item41 - ", "Item42 - ", 
    RusText_Poj3,  //"Item41 - ", "Item43 - ", 
    RusText_GOChS, //"Item44 - ", 
    0,                   
    0
};   
//-----------------------------------------------------------------------------
//3-я страница меню - состояние линий Гр.
MenuPageSTR_Type MenuPage3 = {

    RusText_PageMenuZone,  //"Menu>Page5", 
    RusText_Zone1,         //"Item51 - ", 
    RusText_Att1,
    RusText_Zone2,         //"Item51 - ", 
    RusText_Att2,
    RusText_Sensitivity,   //"Item55 - ", 
    RusText_Zone4,
};                             
//-----------------------------------------------------------------------------
//4-я страница меню - питание блока.
MenuPageSTR_Type MenuPage4 = {

    RusText_PowerPage,
    RusText_MainPower,
    RusText_ReservePower,	
    RusText_BatLevel, 
    RusText_PA,   
    0,                //"Item44 - ", 
    0,                //"Item45 - ", 
    0 
}; 
//-----------------------------------------------------------------------------
//5-я страница меню.
MenuPageSTR_Type MenuPage5 = {

    RusText_PageMenuOpoveshatel, 
		RusText_PitanieOpovesh,
    RusText_Opoveshatel1, 
		RusText_Opoveshatel2,
		RusText_Opoveshatel3,
		0,
    0 
};   
//-----------------------------------------------------------------------------
//6-я страница меню - ЖУРНАЛ СОБЫТИЙ.
MenuPageSTR_Type MenuPage6 = {

    RusText_EventLog, //Имя страницы меню.
		//----------
		//Названия пунктов на странице меню.
    0,//
		0,//
    0,//
    0,//
    0//
};
//-----------------------------------------------------------------------------
//Пустая страница-заглушка.
//MenuPageSTR_Type NoPage= {"Empty Page"};
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
MenuPageSTR_Type 		 BackgroundPage; 						 //Фоновая страница. - эта страница используется в режиме "Блокировка Управления" и "Управление".
MenuPageSTR_Type    *MenuPageStack[MaxMenuPages];//Массив указателей на страницы 
IndexRegMenuSTR_Type IndexRegMenuSTR;            //Структура переменных для работы с меню. 
//*****************************************************************************************
//*****************************************************************************************
//Инициализация пунктов меню.
void MenuInit(void){

  MenuPageStack[Page1] = &MenuPage1;
  MenuPageStack[Page2] = &MenuPage2;
  MenuPageStack[Page3] = &MenuPage3;
  MenuPageStack[Page4] = &MenuPage4;
  MenuPageStack[Page5] = &MenuPage5;
  MenuPageStack[Page6] = &MenuPage6;
	MenuPageStack[Page7] = &NoPage;
	//--------------------
  IndexRegMenuSTR.IndexMenuPage  = 0;
  IndexRegMenuSTR.IndexPageItem  = 0;
	//--------------------
	//отклонения от установившегося значения для фиксации перех-го проц-са на линии.
  MenuPage3.PageItems.StringVar[String5] = LcDeviationGet();   
	//--------------------
	RusTextSTR.EventID      = &RusText_EventID[0];
	RusTextSTR.LcControlOn	= &RusText_LcOn[0];
	RusTextSTR.LcControlOff = &RusText_LcOff[0];
	//--------------------
}
//-----------------------------------------------------------------------------
//Вывод на дисплей логотопа.
void MenuLogo(void){
  
	LcdClear();
 	LcdImage(LogoRoxton);
	//LcdImage(Spicker);
  LcdUpdateAll();
}
//-----------------------------------------------------------------------------
//Горизонтальный прогресбар.
void MenuProgressBar(void){

  static uint16_t i = 0;
  static uint16_t y = 0;
	static uint16_t z = 0;
  //--------------------
//  LcdClear();
//  LcdString(8, 1);
//  LcdOutStr("LCD TEST");
  //Рисование рамки. 0,0 - это верхний левый угол экрана.
  LcdLine(  0, 8,   0, 63, PIXEL_ON);//Левая вертикальная линия.
  LcdLine(  0, 8, 127,  8, PIXEL_ON);//Верхняя горизонтальная линия
  LcdLine(127, 8, 127, 63, PIXEL_ON);//Правая вертикальная линия.
  LcdLine(127,63,   0, 63, PIXEL_ON);//Нижняя горизонтальная линия
  //Рисуем окружность.
  if(++i >= 5)
    {
      i = 0;
      y++;
			z++;
			if(z >= 127) z = 0;
      if(y >=  28) y = 0;
    }
  LcdCircle(63, 36, y-1, PIXEL_ON);
  LcdCircle(63, 36,   y, PIXEL_ON);
  //Рисуем диагонали.
  //LcdLine  (0,  8, 127, 63, PIXEL_ON);
  //LcdLine  (0, 63, 127,  8, PIXEL_ON);
	
	//Вращающийся ромб.
	LcdLine  (63, 63,   z, 35, PIXEL_ON);		
	LcdLine  (62, 63, z-1, 35, PIXEL_ON);	
	LcdLine  (61, 63, z-2, 35, PIXEL_ON);	
		
	LcdLine  (63, 63, 127-z, 35, PIXEL_ON);	
	LcdLine  (62, 63, 126-z, 35, PIXEL_ON);	
	LcdLine  (61, 63, 125-z, 35, PIXEL_ON);	
		
	LcdLine  (63,  9, 127-z, 35, PIXEL_ON);	
	LcdLine  (62,  9, 126-z, 35, PIXEL_ON);	
	LcdLine  (61,  9, 125-z, 35, PIXEL_ON);	
		
	LcdLine  (63,  9,     z, 35, PIXEL_ON);
	LcdLine  (62,  9,   z-1, 35, PIXEL_ON);
	LcdLine  (61,  9,   z-2, 35, PIXEL_ON);
		

  
//  LcdStringBig(3, 4);
//  LcdOutStr((char*)RusText_SelfTest);
//  //Горизонтальный прогресбар.
//  for (i=0;i<128;i++)
//    {
//       LcdLine (i, 44, i, 61, PIXEL_ON);
//       //LcdUpdate(); 
//        
//       //De lay_mSec(10); 
//    }  
}
//-----------------------------------------------------------------------------
//Получение указателя на русские надписи.
char* MenuGetRusText(uint16_t pointer){

  if(pointer == RusText_SelfTest_Pointer)        return (char*)&RusText_SelfTest[0];
  if(pointer == RusText_TESTMode_Pointer)        return (char*)&RusText_TESTMode[0];
  if(pointer == RusText_LockModeUnit_Pointer)    return (char*)&RusText_LockModeUnit[0];
  if(pointer == RusText_ControlModeUnit_Pointer) return (char*)&RusText_ControlModeUnit[0];
  if(pointer == RusText_CofigModeUnit_Pointer)   return (char*)&RusText_CofigModeUnit[0];
  if(pointer == RusText_MicNotConnect_Pointer)   return (char*)&RusText_MicNotConnect[0];
  if(pointer == RusText_MicConnect_Pointer)      return (char*)&RusText_MicConnect[0];
  if(pointer == RusText_MicFault_Pointer)        return (char*)&RusText_MicFault[0];
  if(pointer == RusText_MicActive_Pointer)       return (char*)&RusText_MicActive[0];
  if(pointer == RusText_Address_Pointer)         return (char*)&RusText_Address[0];
  if(pointer == RusText_Group_Pointer)           return (char*)&RusText_Group[0];
  if(pointer == RusText_Time_Pointer)            return (char*)&RusText_Time[0];
  if(pointer == RusText_Date_Pointer)            return (char*)&RusText_Date[0];
  if(pointer == RusText_ItemMenu_Pointer)        return (char*)&RusText_ItemMenu[0];
  if(pointer == RusText_SaveParam_Pointer)       return (char*)&RusText_Saveing[0];
  if(pointer == RusText_ErrorConnectMB_Pointer)  return (char*)&RusText_ErrorConnectMB[0];
  if(pointer == RusText_PageMenuZone_Pointer)    return (char*)&RusText_PageMenuZone[0];
  if(pointer == RusText_PojLine_Pointer)     		 return (char*)&RusText_PojLine[0];
  if(pointer == RusText_Zone1_Pointer)           return (char*)&RusText_Zone1[0];
  if(pointer == RusText_Zone2_Pointer)           return (char*)&RusText_Zone2[0];
  if(pointer == RusText_Zone3_Pointer)           return (char*)&RusText_Zone3[0];
  if(pointer == RusText_Zone4_Pointer)           return (char*)&RusText_Zone4[0];
  if(pointer == RusText_Zone5_Pointer)           return (char*)&RusText_Zone5[0];
  if(pointer == RusText_Zone6_Pointer)           return (char*)&RusText_Zone6[0];
  if(pointer == RusText_StatusPoj1_Pointer)      return (char*)&RusText_Poj1[0];
  if(pointer == RusText_StatusPoj2_Pointer)      return (char*)&RusText_Poj2[0];
  if(pointer == RusText_StatusPoj3_Pointer)      return (char*)&RusText_Poj3[0];
  if(pointer == RusText_StatusGOChS_Pointer)     return (char*)&RusText_GOChS[0];
//  if(pointer == RusText_StatusPOWER_Pointer)     return (char*)&RusText_POWER[0];
  if(pointer == RusText_StatusPA_Pointer)        return (char*)&RusText_PA[0];
  if(pointer == RusText_Active_Pointer)          return (char*)&RusText_Active[0];
  if(pointer == RusText_Disactive_Pointer)       return (char*)&RusText_Disactive[0];
  if(pointer == RusText_StateNorm_Pointer)       return (char*)&RusText_Norm[0];
  if(pointer == RusText_StateAboveNorm_Pointer)  return (char*)&RusText_AboveNorm[0];
  if(pointer == RusText_StateBelowNorm_Pointer)  return (char*)&RusText_BelowNorm[0];
  if(pointer == RusText_StateBreak_Pointer)      return (char*)&RusText_Break[0];
  if(pointer == RusText_StateShort_Pointer)      return (char*)&RusText_Short[0];
  if(pointer == RusText_StateUndef_Pointer)      return (char*)&RusText_Analyze[0];
  if(pointer == RusText_LineNotWork_Pointer)     return (char*)&RusText_LineNotWork[0];
  if(pointer == RusText_Mic_Pointer)             return (char*)&RusText_Mic[0];
  if(pointer == RusText_ManualControl_Pointer)   return (char*)&RusText_ManualControl[0];
  if(pointer == RusText_BatLevel_Pointer)        return (char*)&RusText_BatLevel[0];  
  if(pointer == RusText_MainPower_Pointer)       return (char*)&RusText_MainPower[0];  
  if(pointer == RusText_PowerPage_Pointer)       return (char*)&RusText_PowerPage[0];  
  if(pointer == RusText_PAProt_Pointer)          return (char*)&RusText_PAProt[0];
  if(pointer == RusText_Preset1_Pointer)         return (char*)&RusText_Alg1[0];
  if(pointer == RusText_LcDeviation_Pointer)     return (char*)&RusText_Sensitivity[0];
	
	if(pointer == RusText_PageMenuOpoveshatel_Pointer)	return (char*)&RusText_PageMenuOpoveshatel[0];
	if(pointer == RusText_Opoveshatel1_Pointer)     		return (char*)&RusText_Opoveshatel1[0];
	if(pointer == RusText_Opoveshatel2_Pointer)     		return (char*)&RusText_Opoveshatel2[0];
	if(pointer == RusText_Opoveshatel3_Pointer)     		return (char*)&RusText_Opoveshatel3[0];
	if(pointer == RusText_PitanieOpovesh_Pointer)    		return (char*)&RusText_PitanieOpovesh[0];
	if(pointer == RusText_NoConnectionSB_Pointer)				return (char*)&RusText_NoConnectionSB[0];
	
  return 0;
}
//-----------------------------------------------------------------------------
RusTextSTR_Type* RusText(void){

	return &RusTextSTR;
}
//-----------------------------------------------------------------------------
//Массив указателей для работы с меню.
MenuPageSTR_Type* MenuPage(uint8_t page){

  return MenuPageStack[page];
}
//-----------------------------------------------------------------------------
//Получение переменных для работы с меню.
IndexRegMenuSTR_Type* MenuIndexReg(void){

  return &IndexRegMenuSTR;
}
//-----------------------------------------------------------------------------
//Сброс переменных для работы с меню.
void MenuIndexRegCleare(void){

  IndexRegMenuSTR.IndexMenuPage  = 0;
  IndexRegMenuSTR.IndexPageItem  = 0;
}
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//Коды событий.
typedef enum{
	//----------------
	NoFault = 0,
	DeviceOn,
	LogACFault,
	LogACNorm,
	LogDCFault, 
	LogDCNorm,
	LogBatFault,
	LogBatNorm,			 //7
	//----------------
	LogFireLine1Fault, 
	LogFireLine1Norm,
	LogFireLine2Fault,
	LogFireLine2Norm,
	LogFireLine3Fault,
	LogFireLine3Norm,
	LogChSLineFault, 
	LogChSLineNorm,  //15
	//----------------
	LogSpLine1Fault,
	LogSpLine1Norm,
	LogSpAtten1Fault,
	LogSpAtten1Norm,
	LogSpLine2Fault, 
	LogSpLine2Norm, 
	LogSpAtten2Fault,
	LogSpAtten2Norm, //23
	//----------------
	LogSiren1Fault,
	LogSiren1Norm,
	LogSiren2Fault,
	LogSiren2Norm,
	LogSiren3Fault,
	LogSiren3Norm,   //29
	//----------------
	LogPAFault,
	LogPANorm,
	LogMicFault,
	LogMicNorm,
	LogConnectFault,
	LogConnectNorm,
	//----------------
}EventIDEnum_t;
//-----------------------------------------------------------------------------
//преобразование кода события в текст.
//вход: eventId - код события. от 1 до 1024.
//выход: указатель на текст.
char* LogGetRusText(uint8_t eventID){

	switch(eventID){
		case(DeviceOn)   : return (char*)&RusText_PowerOn[0];    
		//Основное питание.
		case(LogACNorm)  : return (char*)&RusText_PowerAcNorm[0];
		case(LogACFault) : return (char*)&RusText_PowerAcFault[0];     
		//Инвертор.
		case(LogDCNorm)  : return (char*)&RusText_PowerDcNorm[0];   
		case(LogDCFault) : return (char*)&RusText_PowerDcFault[0];   
		//АКБ.
		case(LogBatNorm) : return (char*)&RusText_BatNorm[0];   		
		case(LogBatFault): return (char*)&RusText_BatFault[0];   
		//шлейфы пожарные.
		case(LogFireLine1Norm) : return (char*)&RusText_FireLine1Norm[0];   		
		case(LogFireLine1Fault): return (char*)&RusText_FireLine1Fault[0];   		
		case(LogFireLine2Norm) : return (char*)&RusText_FireLine2Norm[0];   		
		case(LogFireLine2Fault): return (char*)&RusText_FireLine2Fault[0]; 
		case(LogFireLine3Norm) : return (char*)&RusText_FireLine3Norm[0];   		
		case(LogFireLine3Fault): return (char*)&RusText_FireLine3Fault[0]; 
		case(LogChSLineNorm)   : return (char*)&RusText_ChSLineNorm[0];   		
		case(LogChSLineFault)  : return (char*)&RusText_ChSLineFault[0]; 
		//линии связи с Гр.
		case(LogSpLine1Norm)   : return (char*)&RusText_SpLine1Norm[0];   		
		case(LogSpLine1Fault)  : return (char*)&RusText_SpLine1Fault[0]; 
		case(LogSpAtten1Norm)  : return (char*)&RusText_SpAtten1Norm[0];   		
		case(LogSpAtten1Fault) : return (char*)&RusText_SpAtten1Fault[0]; 
		case(LogSpLine2Norm)   : return (char*)&RusText_SpLine2Norm[0];   		
		case(LogSpLine2Fault)  : return (char*)&RusText_SpLine2Fault[0];
		case(LogSpAtten2Norm)  : return (char*)&RusText_SpAtten2Norm[0];   		
		case(LogSpAtten2Fault) : return (char*)&RusText_SpAtten2Fault[0]; 
		//
		case(LogSiren1Norm) : return (char*)&RusText_Siren1Norm[0];   		
		case(LogSiren1Fault): return (char*)&RusText_Siren1Fault[0]; 		
		case(LogSiren2Norm) : return (char*)&RusText_Siren2Norm[0];   		
		case(LogSiren2Fault): return (char*)&RusText_Siren2Fault[0]; 		
		case(LogSiren3Norm) : return (char*)&RusText_Siren3Norm[0];   		
		case(LogSiren3Fault): return (char*)&RusText_Siren3Fault[0]; 
		//
		case(LogConnectNorm) : return (char*)&RusText_ConnectNorm[0];   		
		case(LogConnectFault): return (char*)&RusText_ConnectFault[0]; 		
		//--------------------
		default: 
		  LcdOutStr((char*)&RusText_EventID[0]);//надпись "Код события"
		  LcdBinToDec(eventID, 4);							//собственно код события
		return 0;
		//--------------------
	}
}
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//Шапка страницы.
void MenuHeaderPage(const char* NamePage){
  
  //static uint8_t SecTemp = 0;
  //--------------------
	//Имя страницы меню.
  LcdString(1, 1);
  LcdOutStr((char*)NamePage);
  //Вывод времени в правый верхний угол.
  LcdString(17, 1);
	LcdBinToDec(DateAhdTimeGet()->hour, 2);
  if(DateAhdTimeGet()->sec & 0x0001) LcdChr(':');//Мигающий разделитель ":"
  else                               LcdChr(' ');//
	LcdBinToDec(DateAhdTimeGet()->min, 2);
  //Разделяющая полоса.
  LcdLine( 0, 10, 127, 10, PIXEL_ON);
  LcdLine( 0, 11, 127, 11, PIXEL_ON);
  LcdLine( 0, 12, 127, 12, PIXEL_ON);
}
//-----------------------------------------------------------------------------
MenuPageSTR_Type* MenuBackgroundPage(void){
  
  return &BackgroundPage;
}
//-----------------------------------------------------------------------------
void MenuBackgroundPageDisplay(MenuPageSTR_Type *menuPage){
  
  //Шапка страницы.
  MenuHeaderPage(menuPage->NamePage);
  //--------------------
  //Адрес
  LcdString(1, 3);
  LcdOutStr((char*)menuPage->PageItems.StringName[String1]);
  LcdBinToDec(menuPage->PageItems.StringVar[String1], 2);
  //Группа
  LcdString(13, 3);
  LcdOutStr((char*)menuPage->PageItems.StringName[String2]);
  LcdBinToDec(menuPage->PageItems.StringVar[String2], 2);
	//Состояние микрофона
  LcdString(1, 4);
  LcdOutStr((char*)menuPage->PageItems.StringName[String3]);
	//
	

	
//	LcdString(1, 5);
//  LcdOutStr((char*)menuPage->PageItems.StringName[String4]);
	
	
//  LcdOutStr((char*)menuPage->PageItems.StringText[String3]);

//  LcdOutStr((char*)menuPage->PageItems.StringName1);
//  LcdStringBold(1, 3);
//  LcdBinToDecBold(menuPage->PageItems.Param1, 2);

//  LcdOutStr((char*)menuPage->PageItems.StringName2);
//  LcdStringBold(1, 5);
//  LcdBinToDecBold(menuPage->PageItems.Param2, 2);

//  LcdOutStr((char*)menuPage->PageItems.StringName3);
//  LcdStringBig(1, 7);

//  LcdOutStr((char*)menuPage->PageItems.StringName4);
//  LcdStringBold(5, 7);

//  LcdOutStr((char*)menuPage->PageItems.StringName5);
//  LcdString(1, 7);
//  LcdBinToDec(menuPage->PageItems.Param1);
  
  //LcdUpdate();
}
//-----------------------------------------------------------------------------
void MenuTextDisplay(uint8_t ptr1, char* txt, uint8_t str){

  LcdString(1, str+1);
  LcdOutStr(MenuGetRusText(ptr1));
  LcdOutStr(txt);
}
//-----------------------------------------------------------------------------
void MenuAddressUpdate(uint8_t position, uint8_t data, uint8_t blink){

  //Адрес.
  if(position == 0)//Редактирование.
    {
      LcdString(8, 3);//Редактирумый разряд
      MenuPage1.PageItems.StringVar[String1] = data;
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(MenuPage1.PageItems.StringVar[String1], 2);
      else     LcdOutStr("  ");
    }
  else//Отображение.
    {
      LcdString(8, 3);
      LcdBinToDec(MenuPage1.PageItems.StringVar[String1], 2);
    }
}
//-----------------------------------------------------------------------------
void MenuGroupUpdate(uint8_t position, uint8_t data, uint8_t blink){

  //Группа.
  if(position == 0)//Редактирование.
    {
      LcdString(8, 4);//Редактирумый разряд
      MenuPage1.PageItems.StringVar[String2] = data;
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(MenuPage1.PageItems.StringVar[String2], 2);
      else     LcdOutStr("  ");
    }
  else//Отображение.
    {
      LcdString(8, 4);
      LcdBinToDec(MenuPage1.PageItems.StringVar[String2], 2);
    }
}
//-----------------------------------------------------------------------------
//Вывод времени и даты на дисплей в формате дд.мм.гггг/чч:мм:сс
//Вход: numString   - номер строки куда выводить время (считается сверху, от 1 до 8);
//			posInString - позиция в строке куда выводить время.
//			timeUTC - время в формате UTC.		
void DisplayDateAndTimeEvent(uint8_t posInString, uint8_t numString, uint32_t timeUtc){

	CounterToTimeAndDate(timeUtc);
	//--------------------
	numString += 1;
	//--------------------
	//Вывод даты.
	LcdString(posInString, numString);
  LcdBinToDec(DateAhdTimeGet()->day, 2); 
	LcdChr('.');
	LcdBinToDec(DateAhdTimeGet()->month, 2); 
	LcdChr('.');
	LcdBinToDec(DateAhdTimeGet()->year, 4);
	LcdChr('/');
	//Вывод времени.
	posInString +=11;
  LcdString(posInString+2, numString);
  LcdChr(':');
  LcdString(posInString+5, numString);
  LcdChr(':');
  
	LcdString(posInString +(0 * 3), numString);
	LcdBinToDec(DateAhdTimeGet()->hour, 2); 
	LcdString(posInString +(1 * 3), numString);
	LcdBinToDec(DateAhdTimeGet()->min, 2); 
	LcdString(posInString +(2 * 3), numString);
	LcdBinToDec(DateAhdTimeGet()->sec, 2); 
}
//-----------------------------------------------------------------------------
void MenuTimeUpdate(uint8_t position, uint16_t data, uint8_t blink, TimeSTR_Type *time){

  //static uint8_t CursorPosition = 0;
	//--------------------
  LcdString(10, 5);
  LcdChr(':');
  LcdString(13, 5);
  LcdChr(':');
  //--------------------
  //Часы.
  if(position == 0)//Редактирование.
    {
      LcdString(8 +(0 * 3), 5);//Редактирумый разряд
      time->hour = data;
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(time->hour, 2); 
      else     LcdOutStr("  ");
    }
  else//Отображение.
    {
      LcdString(8 +(0 * 3), 5);
      LcdBinToDec(time->hour, 2); 
    }
  //--------------------
  //Минуты.
  if(position == 1)//Редактирование.
    {
      LcdString(8 +(1 * 3), 5);//Редактирумый разряд
      time->min = data;
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(time->min, 2); 
      else     LcdOutStr("  ");
    }
  else//Отображение.
    {
      LcdString(8 +(1 * 3), 5);
      LcdBinToDec(time->min, 2); 
    }
  //--------------------
  //Секунды.
  if(position == 2)//Редактирование.
    {
      LcdString(8 +(2 * 3), 5);//Редактируемый разряд
      time->sec = data;
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(time->sec, 2); 
      else     LcdOutStr("  ");
    }
  else//Отображение.
    {
      LcdString(8 +(2 * 3), 5);
      LcdBinToDec(time->sec, 2); 
    }
//  //--------------------
//  //Вывод времени без редактирования.
//  if(position == 3)
//    {
//      LcdString(8 +(0 * 3), 5);
//      LcdBinToDec(time->hour, 2); 
//      LcdString(8 +(1 * 3), 5);
//      LcdBinToDec(time->min, 2); 
//      LcdString(8 +(2 * 3), 5);
//      LcdBinToDec(time->sec, 2); 
//		}
//  //--------------------
}
//-----------------------------------------------------------------------------
void MenuCalendarUpdate(uint8_t position, uint16_t date, uint8_t blink, TimeSTR_Type* Data){

  //static uint8_t CursorPosition = 0;
	//--------------------
  LcdString(10, 6);
  LcdChr('.');
  LcdString(13, 6);
  LcdChr('.');
  //--------------------
  //День.
  if(position == 0)//Редактирование.
    {
      LcdString(8 +(0 * 3), 6);//Редактирумый разряд
      Data->day = date;
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(Data->day, 2); 
      else     LcdOutStr("  ");
    }
  else//Отображение.
    {
      LcdString(8 +(0 * 3), 6);
      LcdBinToDec(Data->day, 2); 
    }
  //--------------------
  //Месяц.
  if(position == 1)//Редактирование.
    {
      LcdString(8 +(1 * 3), 6);//Редактирумый разряд
      Data->month = date;
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(Data->month, 2); 
      else     LcdOutStr("  ");
    }
  else//Отображение.
    {
      LcdString(8 +(1 * 3), 6);
      LcdBinToDec(Data->month, 2); 
    }
  //--------------------
  //Год.
	if(position == 2)//Редактирование.
    {
      LcdString(14 +(0 * 3), 6);//Редактирумый разряд
      Data->year = date;
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(Data->year, 4); 
      else     LcdOutStr("    ");
    }
  else//Отображение.
    {
      LcdString(14 +(0 * 3), 6);
      LcdBinToDec(Data->year, 4); 
    }
  //--------------------
  //Вывод времени без редактирования.
//  if(position == 3)
//    {
//      LcdString(14 +(0 * 3), 6);
//      LcdBinToDec(Data->year, 4); 
//			
//      LcdString(8 +(1 * 3), 6);
//      LcdBinToDec(Data->month, 2);
//			
//      LcdString(8 +(2 * 3), 6);
//      LcdBinToDec(Data->day, 2); 
//    }
  //--------------------
}
//-----------------------------------------------------------------------------
//Мигающий редактируемый разряд при настройке праметров входов пожарной сигнализации.
void MenuInputConfigUpdate(uint8_t numStr, uint8_t digit, FireLine_t* ptr){
	
	//--------------------
  //Тип входа.
	LcdString(18, numStr + 3);
	if(digit == 1)//Мигаем редактируемым символом.
    {
      if(Blink(BlinkInterval250mS))LcdBinToDec(ptr->Type, 1); 
      else     LcdOutStr(" ");
    }
  else LcdBinToDec(ptr->Type, 1);//Отображение.
  //--------------------
	//Таймаут перехода из режима ВНИМАНИЕ в режим ПУСК. От 0 до 99 сек.
	LcdString(13, numStr + 3);
	if(digit == 0)//Мигаем редактируемым символом.
    {
      if(Blink(BlinkInterval250mS))LcdBinToDec(ptr->TimeOut, 2); 
      else     LcdOutStr("  ");
    }
  else LcdBinToDec(ptr->TimeOut, 2); //Отображение.
  //--------------------
}
//-----------------------------------------------------------------------------
//mode = 1 - редактирование параметров меню
//mode = 0 - 
void MenuUpdate(MenuPageSTR_Type *menuPage, uint8_t mode, uint8_t pointStr){
  
  static uint8_t cursorPosition[8];// = {0, 0, 0, 0, 0};
  //--------------------
  LcdClear();                               //Очистка видеопамяти.
  MenuHeaderPage((char*)menuPage->NamePage);//Вывод шапки страницы.
  //--------------------
  //При редактировании параметров меню справа от редактируемого параметра
  //выводится "<=".
  if(mode)
    {
      LcdString(cursorPosition[pointStr], (3 + pointStr));
      LcdOutStr("<=");
    }
  //----------------------------------------
  //1-я страница меню - Настриваемые параметры, время, дата.
  if(menuPage == MenuPage(Page1))
    {
      //--------------------
      //Адрес
      LcdString(1, 3);
      LcdOutStr((char*)menuPage->PageItems.StringName[String1]);
      cursorPosition[0] = 10; 
      MenuAddressUpdate(3, 0, 0);

      //Группа
      LcdString(1, 4);
      LcdOutStr((char*)menuPage->PageItems.StringName[String2]);
      cursorPosition[1] = 10;
      MenuGroupUpdate(3, 0, 0);

      //Время
      LcdString(1, 5);
      LcdOutStr((char*)menuPage->PageItems.StringName[String3]);
      cursorPosition[2] = 16;
      MenuTimeUpdate(3, 0, 0, DateAhdTimeGet());
			
      //Дата
      LcdString(1, 6);
      cursorPosition[3]  = LcdOutStr((char*)menuPage->PageItems.StringName[String4]);
      cursorPosition[3] += LcdBinToDec(DateAhdTimeGet()->day, 2);
      LcdChr('.');
      cursorPosition[3] += LcdBinToDec(DateAhdTimeGet()->month, 2);
      LcdChr('.');
      cursorPosition[3] += LcdBinToDec(DateAhdTimeGet()->year, 4);
			
      //Отладочная информация.
//      LcdString(1, 7);
//      cursorPosition[4]  = LcdOutStr("Debug1-");//((char*)menuPage->PageItems.StringName[String5]);
//      cursorPosition[4] += LcdBinToDec(menuPage->PageItems.StringVar[String5], 4);
//      //
//      LcdString(1, 8);
//      cursorPosition[5]  = LcdOutStr("Debug2-");//((char*)menuPage->PageItems.StringName[String6]);
//      cursorPosition[5] += LcdBinToDec(menuPage->PageItems.StringVar[String6], 4);
    } 
  //----------------------------------------
  //2-я страница меню - Состояние пожарных шлейфов.
  if(menuPage == MenuPage(Page2))
    {
      //--------------------
      cursorPosition[String2] = 20;
			cursorPosition[String3] = 20;
			cursorPosition[String4] = 20;
			cursorPosition[String5] = 20;
			
			//"Шл.|Сост.|Сек.|Тип"
			LcdString(1, 3);
			LcdOutStr((char*)&RusText_PojLineTable[0]);
			//--------------------
			//шлейф Пож.1
      LcdString(1, 4);
      LcdOutStr((char*)menuPage->PageItems.StringName[String1]);
			//физическое состояние шлейфа
			LcdOutStr("-");
      LcdOutStr((char*)menuPage->PageItems.StringText[String1]);
			//Время перехода из ВНИМАНИЕ в ПУСК в сек.
			LcdString(11, 4);
			LcdOutStr("- ");
			LcdBinToDec(FireLine(Poj1)->TimeOut, 2); 
			//Тип входа
			LcdOutStr(" - ");
			LcdBinToDec(FireLine(Poj1)->Type, 1);  		
			//--------------------
      //шлейф Пож.2
      LcdString(1, 5);
      LcdOutStr((char*)menuPage->PageItems.StringName[String2]);
			//физическое состояние шлейфа
			LcdOutStr("-");
      LcdOutStr((char*)menuPage->PageItems.StringText[String2]);
			//Время перехода из ВНИМАНИЕ в ПУСК в сек.
			LcdString(11, 5);
			LcdOutStr("- ");
			LcdBinToDec(FireLine(Poj2)->TimeOut, 2); 
			//Тип входа
			LcdOutStr(" - ");
			LcdBinToDec(FireLine(Poj2)->Type, 1);  		
			//--------------------
      //шлейф Пож.3
      LcdString(1, 6);
      LcdOutStr((char*)menuPage->PageItems.StringName[String3]);
			//физическое состояние шлейфа
			LcdOutStr("-");
      LcdOutStr((char*)menuPage->PageItems.StringText[String3]);
			//Время перехода из ВНИМАНИЕ в ПУСК в сек.
			LcdString(11, 6);
			LcdOutStr("- ");
			LcdBinToDec(FireLine(Poj3)->TimeOut, 2); 
			//Тип входа
			LcdOutStr(" - ");
			LcdBinToDec(FireLine(Poj3)->Type, 1);  		
			//--------------------
      //шлейф ГОиЧС
      LcdString(1, 7);
      LcdOutStr((char*)menuPage->PageItems.StringName[String4]);
			//физическое состояние шлейфа
			LcdOutStr("-");
      LcdOutStr((char*)menuPage->PageItems.StringText[String4]);
			//Время перехода из ВНИМАНИЕ в ПУСК в сек.
			LcdString(11, 7);
			LcdOutStr("- ");
			LcdBinToDec(FireLine(ChS)->TimeOut, 2); 
			//Тип входа
			LcdOutStr(" - ");
			LcdBinToDec(FireLine(ChS)->Type, 1);  		
      //--------------------
     }
  //----------------------------------------
  //3-я страница меню - Состояние линий Гр.
  if(menuPage == MenuPage(Page3))
    {
      //--------------------
      //Зона 1
      LcdString(1, 3);
      cursorPosition[0]  = LcdOutStr((char*)menuPage->PageItems.StringName[String1]);
      cursorPosition[0] += LcdOutStr((char*)menuPage->PageItems.StringText[String1])+1;
      //LcdBinToDec(menuPage->PageItems.StringVar[String1], 2);
      //Аттенюатер 1
      LcdString(1, 4);
      cursorPosition[1]  = LcdOutStr((char*)menuPage->PageItems.StringName[String2]);
      cursorPosition[1] += LcdOutStr((char*)menuPage->PageItems.StringText[String2])+1;
      //LcdBinToDec(menuPage->PageItems.StringVar[String2], 2);
      //Зона 2
      LcdString(1, 5);
      cursorPosition[2]  = LcdOutStr((char*)menuPage->PageItems.StringName[String3]);
      cursorPosition[2] += LcdOutStr((char*)menuPage->PageItems.StringText[String3])+1;
      //LcdBinToDec(menuPage->PageItems.StringVar[String3], 2);
      //Аттенюатер 2
      LcdString(1, 6);
      cursorPosition[3]  = LcdOutStr((char*)menuPage->PageItems.StringName[String4]);
      cursorPosition[3] += LcdOutStr((char*)menuPage->PageItems.StringText[String4])+1;
      //LcdBinToDec(menuPage->PageItems.StringVar[String4], 2);
      //Отклонение для фиксации ПП на линии Гр.
      LcdString(1, 7);
      cursorPosition[4]  = LcdOutStr((char*)menuPage->PageItems.StringName[String5]);
			cursorPosition[4] += LcdBinToDecWithDot(menuPage->PageItems.StringVar[String5], 0, "mB");
//      cursorPosition[4] += LcdBinToDec(menuPage->PageItems.StringVar[String5], 3);
//      cursorPosition[4] += 2;
//      LcdString(15, 7);
//      LcdOutStr("mB");
      
//      //
//      LcdString(1, 8);
//      cursorPosition[5]  = LcdOutStr((char*)menuPage->PageItems.StringName[String6]);
//      cursorPosition[5] += LcdBinToDec(menuPage->PageItems.StringVar[String6], 2);
      //--------------------
     }
  //----------------------------------------
  //4-я страница меню - Питание блока.
  if(menuPage == MenuPage(Page4))
    {
      //Основное питание
      LcdString(1, 3);
      cursorPosition[String1]  = LcdOutStr((char*)menuPage->PageItems.StringName[String1]);
      cursorPosition[String1] += LcdOutStr((char*)menuPage->PageItems.StringText[String1])+1;
			//Резервное питание
			LcdString(1, 4);
			cursorPosition[String2]  = LcdOutStr((char*)menuPage->PageItems.StringName[String2]);
      cursorPosition[String2] += LcdOutStr((char*)menuPage->PageItems.StringText[String2])+1;
      //Напряжение на АКБ.
      LcdString(1, 5);
      cursorPosition[String3]  = LcdOutStr((char*)menuPage->PageItems.StringName[String3]);
			
			if(PowerDevice()->Bat != BatControlOff) cursorPosition[String3] += LcdBinToDecWithDot(menuPage->PageItems.StringVar[String3], 2, "B");
      else                             				cursorPosition[String3] += LcdOutStr((char*)RusText_Undef);
      //Состояние УМЗЧ.
      LcdString(1, 6);
      cursorPosition[String4]  = LcdOutStr((char*)menuPage->PageItems.StringName[String4]);
      cursorPosition[String4] += LcdOutStr((char*)TextGetAmpState(PowerDevice()->Amp));
      //
			
//      LcdString(1, 7);
//      cursorPosition[4]  = LcdOutStr((char*)menuPage->PageItems.StringName[4]);
//      cursorPosition[4] += LcdBinToDec(menuPage->PageItems.StringVar[4], 2);
//      //
//      LcdString(1, 8);
//      cursorPosition[5]  = LcdOutStr((char*)menuPage->PageItems.StringName[5]);
//      cursorPosition[5] += LcdBinToDec(menuPage->PageItems.StringVar[5], 2);
    }
  //----------------------------------------
  //5-я стриницы меню. - Состояние оповещателей.
  if(menuPage == MenuPage(Page5))
    {
			if(menuPage->PageItems.StringVar[String1] == 0xFFFF)
				{
					//Нет связи с платой.
					LcdString(1, 3);
					LcdOutStr((char*)&RusText_NoConnectionSB[0]);
				}
			else
				{
					//Питание табло.
					LcdString(1, 3);
					cursorPosition[2]  = LcdOutStr((char*)menuPage->PageItems.StringName[String1]);
					cursorPosition[2] += LcdBinToDecWithDot(menuPage->PageItems.StringVar[String1], 2, "B");
					//Табло1.
					LcdString(1, 4);
					cursorPosition[3]  = LcdOutStr((char*)menuPage->PageItems.StringName[String2]);
					cursorPosition[3]  = LcdOutStr((char*)menuPage->PageItems.StringText[String2]);
					//cursorPosition[2] += LcdBinToDec(menuPage->PageItems.StringVar[String2], 4);
					//Табло2.
					LcdString(1, 5);
					cursorPosition[4]  = LcdOutStr((char*)menuPage->PageItems.StringName[String3]);
					cursorPosition[4]  = LcdOutStr((char*)menuPage->PageItems.StringText[String3]);
					//cursorPosition[2] += LcdBinToDec(menuPage->PageItems.StringVar[String3], 4);
					//Табло3.
					LcdString(1, 6);
					cursorPosition[5]  = LcdOutStr((char*)menuPage->PageItems.StringName[String4]);
					cursorPosition[5]  = LcdOutStr((char*)menuPage->PageItems.StringText[String4]);
					//cursorPosition[3] += LcdBinToDec(menuPage->PageItems.StringVar[String4], 4);
				}
//      //
//      LcdString(1, 7);
//      cursorPosition[4]  = LcdOutStr((char*)menuPage->PageItems.StringName[4]);
//      cursorPosition[4] += LcdBinToDec(menuPage->PageItems.StringVar[4], 2);
//      //
//      LcdString(1, 8);
//      cursorPosition[5]  = LcdOutStr((char*)menuPage->PageItems.StringName[5]);
//      cursorPosition[5] += LcdBinToDec(menuPage->PageItems.StringVar[5], 2);
    }
  //----------------------------------------
  //6-я стриницы меню - Журнал событий.
  if(menuPage == MenuPage(Page6))
    {
			cursorPosition[String1] = 20;
      LcdString(1, 3);
      LcdBinToDec(MenuPage(Page6)->PageItems.StringVar[String1], 4);//Общее количество событий
			LcdOutStr("/");																				        //разделитель
      LcdBinToDec(MenuPage(Page6)->PageItems.StringVar[String2], 4);//Количество непросмотренных событий
      
			if(LogWorkReg()->NotReadEvents) //Вывод надписи "Есть н.с." если есть новые события
				{
					//LcdString(1, 4);
					LcdOutStr((char*)&RusText_NotRead[0]) ;		
				}
			//Номер события.
			if(LogEvent()->EventNumber == 0) return;
      LcdString(1, 5);
			LcdOutStr((char*)&RusText_Event[0]);		
			LcdBinToDec(menuPage->PageItems.StringVar[String3], 4);
      //Дата и время события.		
      DisplayDateAndTimeEvent(Char1, String6, LogEvent()->EventTimeUtc);
      //Код события
      LcdString(1, 7);
			if(LogGetRusText(menuPage->PageItems.StringVar[String4]) != 0)
				{
					LcdOutStr(LogGetRusText(menuPage->PageItems.StringVar[String4]));
				}				
      //Параметр события.
			//LcdString(8, 7);
			//LcdBinToDec(menuPage->PageItems.StringVar[String5], 2);
    }
	//----------------------------------------
  //7-я стриницы меню.
  if(menuPage == MenuPage(Page7))
    {
//      LcdClear();
//      LcdString(1, 1);
//      LcdOutStr("Menu>Page7: LCD TEST");      
//      MenuProgressBar();
			
			//Отладка. Вывод конфигурации пожарного входа.
			//шапка страницы
			LcdString(1, String1 + 1);
			LcdOutStr("EEPROM Info:");   
			
			LcdString(1, String3 + 1);
			LcdOutStr("BedSectors- "); 
			LcdBinToDec(Eeprom()->BadSectorsCount, 4);  		
			
			LcdString(1, String4 + 1);
			LcdOutStr("WriteCount- ");  	
			LcdBinToDec(Eeprom()->WriteCount, 4);  			

			LcdString(1, String5 + 1);
			LcdOutStr("ReadCount - ");  
			LcdBinToDec(Eeprom()->ReadCount, 4);  
			
			LcdString(1, String6 + 1);
			LcdOutStr("StatReg   - "); 
			LcdBinToDec(Eeprom()->StatReg, 3);  		
//			
//			//Физисеское состояние линии.
//			LcdString(1, String6 + 1);
//			LcdOutStr("PhysState- "); 
//			LcdBinToDec(FireLineGetInputConfig(Poj1)->PhysState, 2);  			

//			//Измеренное значение в мв. на входе.
//			LcdString(1, String7 + 1);
//			LcdOutStr("MeasValue- ");  
//			LcdBinToDec(FireLineGetInputConfig(Poj1)->MeasValue, 4);  
    }
  //----------------------------------------  
}
//-----------------------------------------------------------------------------
//Вывод на дисплей надписи "РЕЖИМ ТЕСТИРОВАНИЯ."
void PrintTestmode(void){

	LcdClear();
	LcdStringBig(3, 4);
	LcdOutStr((char*)RusText_TESTMode); 
}
//-----------------------------------------------------------------------------
//Вывод на дисплей надписи "Нет связи с ЦП!!!" при отсутствии связи с материнской платой.
void PrintErrConnect(void){
  
  LcdClear(); 
	LcdString(3, 4);
  LcdOutStr((char*)RusText_ErrorConnectMB);
}
//-----------------------------------------------------------------------------
void PrintMicState(MenuPageSTR_Type *page){

  //--------------------
  switch(MicGetState()){
      //--------------------
      case(MicConnectedConst):
        page->PageItems.StringName[String3] = RusText_MicConnect;
        LedControl(MIC_LED, GreenColor);
      break;
      //--------------------
      case(MicNotConnectedConst):
        page->PageItems.StringName[String3] = RusText_MicNotConnect;
        LedControl(MIC_LED, YellowColor);
      break;
      //--------------------
      case(MicFaultConst):
        page->PageItems.StringName[String3] = RusText_MicFault;
        LedControl(MIC_LED, YellowColor);
      break;
      //--------------------
      case(MicActiveConst):
        page->PageItems.StringName[String3] = RusText_MicActive;
        LedControl(MIC_LED, RedColor);
      break;
      //--------------------
    }
  //--------------------
}
//-----------------------------------------------------------------------------
void PrintPowerState(void){
	
	//Вывод сотсояния основного питания.
	if(PowerDevice()->Check.bit.MainPower) MenuPage(Page4)->PageItems.StringText[String1] = TextGetPowerState(PowerDevice()->MainPower);
	else																	 MenuPage(Page4)->PageItems.StringText[String1] = (char*)&RusText_LcOff;
	//Вывод состояния АКБ
	if(PowerDevice()->Check.bit.Bat) MenuPage(Page4)->PageItems.StringText[String2] = TextGetBatState(PowerDevice()->Bat);
	else														 MenuPage(Page4)->PageItems.StringText[String2] = (char*)&RusText_LcOff;	
}
//-----------------------------------------------------------------------------
char* TextGetBatState(uint8_t batState){

	if(batState == BatControlOff) return (char*)&RusText_LcOn[0];
	if(batState == BatChargeEnd) 	return (char*)&RusText_Norm[0];
	if(batState == BatAttention) 	return (char*)&RusText_BelowNorm[0];
	if(batState == BatDeep)      	return (char*)&RusText_BatDeepCharge[0];	
	if(batState == BatNo)        	return (char*)&RusText_BatNo[0];
																return (char*)&RusText_BatCharge[0];
}
//-----------------------------------------------------------------------------
char* TextGetPowerState(uint8_t mainPowerState){

	if(mainPowerState == PowerControlOff) return (char*)&RusText_LcOn[0];
	if(mainPowerState == PowerACOk)    		return (char*)&RusText_Norm[0];
	if(mainPowerState == PowerDCOk)    		return (char*)&RusText_Norm[0];
	if(mainPowerState == PowerACNo)    		return (char*)&RusText_NotWork[0];
	if(mainPowerState == PowerDCFault) 		return (char*)&RusText_NotWorkDcDc[0];
	if(mainPowerState == PowerFault)   		return (char*)&RusText_NotWork[0];	
	return 0;
}
//-----------------------------------------------------------------------------
char* TextGetAmpState(uint8_t ampState){
  
	if(ampState == AmpDisactive) return (char*)&RusText_Disactive[0];
	if(ampState == AmpActive)    return (char*)&RusText_On[0];
	if(ampState == AmpProt)      return (char*)&RusText_PAProt[0];
	return 0;
}
//-----------------------------------------------------------------------------
char* TextGetSirenSate(uint16_t sirenState){
  
	if(sirenState == SirenLineNorm)  return (char*)&RusText_Norm[0];
	if(sirenState == SirenLineBreak) return (char*)&RusText_Break[0];
	if(sirenState == SirenLineShort) return (char*)&RusText_Short[0];
																	 return (char*)&RusText_Analyze[0];
}
//-----------------------------------------------------------------------------
char* MenuGetTextStatLc(uint8_t line){

	if(line == LineOffControl) return (char*)&RusText_LcOn[0];
  if(line == LineNorm)  		 return (char*)&RusText_Norm[0];
  if(line == LineUndef) 		 return (char*)&RusText_Analyze[0];
  if(line == LineBreak) 		 return (char*)&RusText_Break[0];
  if(line == LineOver)  		 return (char*)&RusText_AboveNorm[0];
  if(line == LineUnder) 		 return (char*)&RusText_BelowNorm[0];
														 return (char*)&RusText_Short[0];
}
//-----------------------------------------------------------------------------
char* MenuGetTextStatPojLine(uint8_t line){
	
  if(line == FireLineCONTROLOFF)   return (char*)&RusText_Disactive[0]; 
//	if(line == FireLineUNDERCONTROL) return (char*); 
	if(line == FireLineATTENTION)    return (char*)&RusText_Attention[0]; 
	if(line == FireLineFIRE)         return (char*)&RusText_Fire[0]; 	
	
	if(line == FireLineUndef )    return (char*)&RusText_Undef[0];
  if(line == FireLineNormOpen)  return (char*)&RusText_Open[0]; //RusText_Disactive[0];
  if(line == FireLineNormClose) return (char*)&RusText_Close[0];//RusText_Active[0];
  if(line == FireLineShort) 		return (char*)&RusText_Short[0];
  if(line == FireLineBreak) 		return (char*)&RusText_Break[0];
																return 0;
}
//-----------------------------------------------------------------------------














