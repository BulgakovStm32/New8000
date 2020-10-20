
#include "MenuLcd12864.h"
#include "ascii_RusText.h"  //надписи на русском языке.
//-----------------------------------------------------------------------------
extern unsigned char        bmp6[];
extern unsigned char        mybmp[];
extern const unsigned char  LogoRoxton[];
extern const unsigned char  Spicker[];
//-----------------------------------------------------------------------------
MenuPageSTR_Type NoPage;

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

    RusText_PageMenuPoj, //"Menu>Page4", 
    RusText_StatusPoj1,  //"Item41 - ", 
    RusText_StatusPoj2,  //"Item41 - ", "Item42 - ", 
    RusText_StatusPoj3,  //"Item41 - ", "Item43 - ", 
    RusText_StatusGOChS, //"Item44 - ", 
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
    RusText_LcDeviation,   //"Item55 - ", 
    RusText_Zone4,
};                             
//-----------------------------------------------------------------------------
//4-я страница меню - питание блока.
MenuPageSTR_Type MenuPage4 = {

    RusText_PowerPage,
    RusText_AcPower, 
    RusText_BatLevel, 
    RusText_StatusPA,   
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
    0, 
};   
//-----------------------------------------------------------------------------
//6-я страница меню.
MenuPageSTR_Type MenuPage6 = {

    "Menu>Page6", 
    "Item1 - ", 
    "Item2 - ", 
    "Item3 - ", 
    "Item4 - ", 
    "Item5 - ", 
    0, 
};    
//-----------------------------------------------------------------------------
//Фоновая страница. - эта страница используется в режиме "Блокировка Управления"
//и "Управление".
MenuPageSTR_Type BackgroundPage;
//MenuPage_STR BackgroundPage = { RusText_BlockControlMode,//"_NEW8000_", 
//                                "Address:", 
//                                "  Group:", 
//                                "Mode:", 
//                                0, 
//                                0, 
//                                0, 
//                                0, 
//                                0, 
//                                0, 
//                                0};   
//-----------------------------------------------------------------------------
MenuPageSTR_Type       *MenuPageStack[MaxMenuPages];//Массив указателей на страницы 
IndexRegMenuSTR_Type    IndexRegMenuSTR;            //Структура переменных для работы с меню. 

//----------------------------------
//Указатели для работы меню.                  
//static uint16_t PointerMenuPage = 0;
//static uint16_t PointerPageItem = 0;

//-----------------------------------------------------------------------------
//Инициализация пунктов меню.
void MenuInit(void){

  MenuPageStack[0] = &MenuPage1;
  MenuPageStack[1] = &MenuPage2;
  MenuPageStack[2] = &MenuPage3;
  MenuPageStack[3] = &MenuPage4;
  MenuPageStack[4] = &MenuPage5;
  MenuPageStack[5] = &MenuPage6;	
	MenuPageStack[6] = &NoPage;

  IndexRegMenuSTR.IndexMenuPage  = 0;
  IndexRegMenuSTR.IndexPageItem  = 0;

  MenuPage3.PageItems.StringVar[String5] = LcDeviationGet();//отклонения от установившегося значения для фиксации перех-го проц-са на линии.   
}
//-----------------------------------------------------------------------------
//Вывод на дисплей логотопа.
void MenuLogo(void){
  
  uint16_t i;
  //--------------------
	LcdClear();
 	//LcdImage(LogoRoxton);
	LcdImage(Spicker);
	
  for(i=0; i<16; i++) LcdUpdate();
}
//-----------------------------------------------------------------------------
//Горизонтальный прогресбар.
void MenuProgressBar(void){

  static uint16_t i = 0;
  static uint16_t y = 0;
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
  if(++i >= 25)
    {
      i = 0;
      y++;
      if(y >= 28) y = 0;
    }
  LcdCircle(63, 36, y-1, PIXEL_ON);
  LcdCircle(63, 36,   y, PIXEL_ON);
  //Рисуем диагонали.
  LcdLine  (0,  8, 127, 63, PIXEL_ON);
  LcdLine  (0, 63, 127,  8, PIXEL_ON);

  
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
  if(pointer == RusText_SaveParam_Pointer)       return (char*)&RusText_SaveParam[0];
  if(pointer == RusText_ErrorConnectMB_Pointer)  return (char*)&RusText_ErrorConnectMB[0];
  if(pointer == RusText_PageMenuZone_Pointer)    return (char*)&RusText_PageMenuZone[0];
  if(pointer == RusText_PageMenuPoj_Pointer)     return (char*)&RusText_PageMenuPoj[0];
  if(pointer == RusText_Zone1_Pointer)           return (char*)&RusText_Zone1[0];
  if(pointer == RusText_Zone2_Pointer)           return (char*)&RusText_Zone2[0];
  if(pointer == RusText_Zone3_Pointer)           return (char*)&RusText_Zone3[0];
  if(pointer == RusText_Zone4_Pointer)           return (char*)&RusText_Zone4[0];
  if(pointer == RusText_Zone5_Pointer)           return (char*)&RusText_Zone5[0];
  if(pointer == RusText_Zone6_Pointer)           return (char*)&RusText_Zone6[0];
  if(pointer == RusText_StatusPoj1_Pointer)      return (char*)&RusText_StatusPoj1[0];
  if(pointer == RusText_StatusPoj2_Pointer)      return (char*)&RusText_StatusPoj2[0];
  if(pointer == RusText_StatusPoj3_Pointer)      return (char*)&RusText_StatusPoj3[0];
  if(pointer == RusText_StatusGOChS_Pointer)     return (char*)&RusText_StatusGOChS[0];
  if(pointer == RusText_StatusPOWER_Pointer)     return (char*)&RusText_StatusPOWER[0];
  if(pointer == RusText_StatusPA_Pointer)        return (char*)&RusText_StatusPA[0];
  if(pointer == RusText_Active_Pointer)          return (char*)&RusText_Active[0];
  if(pointer == RusText_Disactive_Pointer)       return (char*)&RusText_Disactive[0];
  if(pointer == RusText_StateNorm_Pointer)       return (char*)&RusText_StateNorm[0];
  if(pointer == RusText_StateAboveNorm_Pointer)  return (char*)&RusText_StateAbove[0];
  if(pointer == RusText_StateBelowNorm_Pointer)  return (char*)&RusText_StateBelow[0];
  if(pointer == RusText_StateBreak_Pointer)      return (char*)&RusText_StateBreak[0];
  if(pointer == RusText_StateShort_Pointer)      return (char*)&RusText_StateShort[0];
  if(pointer == RusText_StateUndef_Pointer)      return (char*)&RusText_StateUndef[0];
  if(pointer == RusText_LineNotWork_Pointer)     return (char*)&RusText_LineNotWork[0];
  if(pointer == RusText_Mic_Pointer)             return (char*)&RusText_Mic[0];
  if(pointer == RusText_ManualControl_Pointer)   return (char*)&RusText_ManualControl[0];
  if(pointer == RusText_BatLevel_Pointer)        return (char*)&RusText_BatLevel[0];  
  if(pointer == RusText_AcPower_Pointer)         return (char*)&RusText_AcPower[0];  
  if(pointer == RusText_PowerPage_Pointer)       return (char*)&RusText_PowerPage[0];  
  if(pointer == RusText_PAProt_Pointer)          return (char*)&RusText_PAProt[0];
  if(pointer == RusText_Preset1_Pointer)         return (char*)&RusText_Preset1[0];
  if(pointer == RusText_LcDeviation_Pointer)     return (char*)&RusText_LcDeviation[0];
	
	if(pointer == RusText_PageMenuOpoveshatel_Pointer)	return (char*)&RusText_PageMenuOpoveshatel[0];
	if(pointer == RusText_Opoveshatel1_Pointer)     		return (char*)&RusText_Opoveshatel1[0];
	if(pointer == RusText_Opoveshatel2_Pointer)     		return (char*)&RusText_Opoveshatel2[0];
	if(pointer == RusText_Opoveshatel3_Pointer)     		return (char*)&RusText_Opoveshatel3[0];
	if(pointer == RusText_PitanieOpovesh_Pointer)    		return (char*)&RusText_PitanieOpovesh[0];
	
  return 0;
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
//-----------------------------------------------------------------------------
//Шапка страницы.
void MenuHeaderPage(const char* NamePage){
  
  //static uint8_t SecTemp = 0;
  //--------------------
  LcdString(1, 1);
  //Имя страницы меню.
  LcdOutStr((char*)NamePage);
  //Вывод времени в правый верхний угол.
  LcdString(17, 1);
  LcdBinToDec(RtcGetTime()->hour, 2);
  if(RtcGetTime()->sec & 0x0001) LcdChr(':');//Мигающий разделитель ":"
  else                           LcdChr(' ');//
  LcdBinToDec(RtcGetTime()->min, 2); 
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

  LcdString(1, 4);
  LcdOutStr((char*)menuPage->PageItems.StringName[String3]);
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
void MenuTimeUpdate(uint8_t position, uint8_t data, uint8_t blink){

  //static uint8_t CursorPosition = 0;
  //--------------------
  //LcdString(18, 5);
  //LcdOutStr("<=");

  LcdString(10, 5);
  LcdChr(':');
  LcdString(13, 5);
  LcdChr(':');
  //--------------------
  //Часы.
  if(position == 0)//Редактирование.
    {
      LcdString(8 +(0 * 3), 5);//Редактирумый разряд
      RtcGetTime()->hour = data;
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(RtcGetTime()->hour, 2); 
      else     LcdOutStr("  ");
    }
  else//Отображение.
    {
      LcdString(8 +(0 * 3), 5);
      LcdBinToDec(RtcGetTime()->hour, 2); 
    }
  //--------------------
  //Минуты.
  if(position == 1)//Редактирование.
    {
      LcdString(8 +(1 * 3), 5);//Редактирумый разряд
      RtcGetTime()->min = data;
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(RtcGetTime()->min, 2); 
      else     LcdOutStr("  ");
    }
  else//Отображение.
    {
      LcdString(8 +(1 * 3), 5);
      LcdBinToDec(RtcGetTime()->min, 2); 
    }
  //--------------------
  //Секунды.
  if(position == 2)//Редактирование.
    {
      LcdString(8 +(2 * 3), 5);//Редактирумый разряд
      RtcGetTime()->sec = data;
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(RtcGetTime()->sec, 2); 
      else     LcdOutStr("  ");
    }
  else//Отображение.
    {
      LcdString(8 +(2 * 3), 5);
      LcdBinToDec(RtcGetTime()->sec, 2); 
    }
  //--------------------
  //Вывод времени без редактирования.
  if(position == 3)
    {
      LcdString(8 +(0 * 3), 5);
      LcdBinToDec(RtcGetTime()->hour, 2); 
      LcdString(8 +(1 * 3), 5);
      LcdBinToDec(RtcGetTime()->min, 2); 
      LcdString(8 +(2 * 3), 5);
      LcdBinToDec(RtcGetTime()->sec, 2); 
    }
  //--------------------
}
//-----------------------------------------------------------------------------
//mode = 1 - редактирование параметров меню
//mode = 0 - 
void MenuUpdate(MenuPageSTR_Type *menuPage, uint8_t mode, uint8_t pointStr){
  
  static uint8_t cursorPosition[7];// = {0, 0, 0, 0, 0};
  //--------------------
  LcdClear();                               //Очистка видеопамяти.
  MenuHeaderPage((char*)menuPage->NamePage);//Вывод шапки страницы.
  //--------------------
  //При редактировании параметров меню на против редактируемого параметра
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
      MenuTimeUpdate(3, 0, 0);

      //Дата
      LcdString(1, 6);
      cursorPosition[3]  = LcdOutStr((char*)menuPage->PageItems.StringName[String4]);
      cursorPosition[3] += LcdBinToDec(11, 2);
      LcdChr('.');
      cursorPosition[3] += LcdBinToDec(10, 2);
      LcdChr('.');
      cursorPosition[3] += LcdBinToDec(2019, 4);
      //
      LcdString(1, 7);
      cursorPosition[4]  = LcdOutStr("Debug1-");//((char*)menuPage->PageItems.StringName[String5]);
      cursorPosition[4] += LcdBinToDec(menuPage->PageItems.StringVar[String5], 4);
      //
      LcdString(1, 8);
      cursorPosition[5]  = LcdOutStr("Debug2-");//((char*)menuPage->PageItems.StringName[String6]);
      cursorPosition[5] += LcdBinToDec(menuPage->PageItems.StringVar[String6], 4);
    } 
  //----------------------------------------
  //2-я страница меню - Состояние пожарных шлейфов.
  if(menuPage == MenuPage(Page2))
    {
      //--------------------
      //
      LcdString(1, 3);
      LcdOutStr((char*)menuPage->PageItems.StringName[String1]);
      LcdOutStr((char*)menuPage->PageItems.StringText[String1]);
      //
      LcdString(1, 4);
      LcdOutStr((char*)menuPage->PageItems.StringName[String2]);
      LcdOutStr((char*)menuPage->PageItems.StringText[String2]);
      //
      LcdString(1, 5);
      LcdOutStr((char*)menuPage->PageItems.StringName[String3]);
      LcdOutStr((char*)menuPage->PageItems.StringText[String3]);
      //
      LcdString(1, 6);
      LcdOutStr((char*)menuPage->PageItems.StringName[String4]);
      LcdOutStr((char*)menuPage->PageItems.StringText[String4]);
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
      cursorPosition[4] += LcdBinToDec(menuPage->PageItems.StringVar[String5], 3);
      cursorPosition[4] += 2;
      LcdString(15, 7);
      LcdOutStr("mB");
      
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
      //
      LcdString(1, 3);
      cursorPosition[String1]  = LcdOutStr((char*)menuPage->PageItems.StringName[String1]);
      cursorPosition[String1] += LcdBinToDec(menuPage->PageItems.StringVar[String1], 2);
      //
      LcdString(1, 4);
      cursorPosition[String2]  = LcdOutStr((char*)menuPage->PageItems.StringName[String2]);
      cursorPosition[String2] += LcdBinToDec(menuPage->PageItems.StringVar[String2], 4);
      //
      LcdString(1, 5);
      cursorPosition[String3]  = LcdOutStr((char*)menuPage->PageItems.StringName[String3]);
      cursorPosition[String3] += LcdOutStr((char*)menuPage->PageItems.StringText[String3]);
      //cursorPosition[2] += LcdBinToDec(menuPage->PageItems.StringVar[2], 2);
      //
//      LcdString(1, 6);
//      cursorPosition[3]  = LcdOutStr((char*)menuPage->PageItems.StringName[3]);
//      cursorPosition[3] += LcdBinToDec(menuPage->PageItems.StringVar[3], 2);
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
  //5-я стриницы меню. - Состояние оповещателей.
  if(menuPage == MenuPage(Page5))
    {
      //
      LcdString(1, 3);
      cursorPosition[2]  = LcdOutStr((char*)menuPage->PageItems.StringName[String1]);
      cursorPosition[2] += LcdBinToDec(menuPage->PageItems.StringVar[String1], 4);
      //
      LcdString(1, 4);
      cursorPosition[2]  = LcdOutStr((char*)menuPage->PageItems.StringName[1]);
      cursorPosition[2] += LcdBinToDec(menuPage->PageItems.StringVar[1], 4);
      //
      LcdString(1, 5);
      cursorPosition[2]  = LcdOutStr((char*)menuPage->PageItems.StringName[2]);
      cursorPosition[2] += LcdBinToDec(menuPage->PageItems.StringVar[2], 4);
      //
      LcdString(1, 6);
      cursorPosition[3]  = LcdOutStr((char*)menuPage->PageItems.StringName[3]);
      cursorPosition[3] += LcdBinToDec(menuPage->PageItems.StringVar[3], 4);
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
  //6-я стриницы меню. 
  if(menuPage == MenuPage(Page6))
    {
      //
      LcdString(1, 3);
      cursorPosition[2]  = LcdOutStr((char*)menuPage->PageItems.StringName[0]);
      cursorPosition[2] += LcdBinToDec(menuPage->PageItems.StringVar[0], 2);
      //
      LcdString(1, 4);
      cursorPosition[2]  = LcdOutStr((char*)menuPage->PageItems.StringName[1]);
      cursorPosition[2] += LcdBinToDec(menuPage->PageItems.StringVar[1], 2);
      //
      LcdString(1, 5);
      cursorPosition[2]  = LcdOutStr((char*)menuPage->PageItems.StringName[2]);
      cursorPosition[2] += LcdBinToDec(menuPage->PageItems.StringVar[2], 2);
      //
      LcdString(1, 6);
      cursorPosition[3]  = LcdOutStr((char*)menuPage->PageItems.StringName[3]);
      cursorPosition[3] += LcdBinToDec(menuPage->PageItems.StringVar[3], 2);
      //
      LcdString(1, 7);
      cursorPosition[4]  = LcdOutStr((char*)menuPage->PageItems.StringName[4]);
      cursorPosition[4] += LcdBinToDec(menuPage->PageItems.StringVar[4], 2);
      //
      LcdString(1, 8);
      cursorPosition[5]  = LcdOutStr((char*)menuPage->PageItems.StringName[5]);
      cursorPosition[5] += LcdBinToDec(menuPage->PageItems.StringVar[5], 2);
    }
	//----------------------------------------
  //7-я стриницы меню.
  if(menuPage == MenuPage(Page7))
    {
      LcdClear();
      LcdString(1, 1);
      LcdOutStr("Menu>Page7: LCD TEST");      
      MenuProgressBar();
    }
  //----------------------------------------  
}
//-----------------------------------------------------------------------------
//Вывод на дисплей надписи "Нет связи с ЦП!!!" при отсутствии связи с материнской платой.
void MenuTextErrorConnect(void){
  
  LcdClear(); 
  LcdOutStr((char*)RusText_ErrorConnectMB);
  LcdString(3, 4);

  //LcdUpdate();
}
//-----------------------------------------------------------------------------
char* MenuGetTextStatLc(uint8_t line){

  if(line == LineNorm)  return (char*)&RusText_StateNorm[0];
  if(line == LineUndef) return (char*)&RusText_StateUndef[0];
  if(line == LineBreak) return (char*)&RusText_StateBreak[0];
  if(line == LineOver)  return (char*)&RusText_StateAbove[0];
  if(line == LineUnder) return (char*)&RusText_StateBelow[0];
                        return (char*)&RusText_StateShort[0];
}
//-----------------------------------------------------------------------------
char* MenuGetTextStatPojLine(uint8_t line){

  if(line == NormOpenSost)  return (char*)&RusText_Disactive[0];
  if(line == NormCloseSost) return (char*)&RusText_Active[0];
  if(line == ShortLineSost) return (char*)&RusText_StateShort[0];
  if(line == BreakLineSost) return (char*)&RusText_StateBreak[0];
                            return 0;
}
//-----------------------------------------------------------------------------
void MenuGetMicStatus(MenuPageSTR_Type *page){

  //--------------------
  switch(GetMicState()){
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














