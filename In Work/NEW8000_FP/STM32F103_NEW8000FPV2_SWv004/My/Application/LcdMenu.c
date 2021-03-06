
#include "LcdMenu.h"
#include "ascii_RusText.h"  //надписи на русском языке.

//******************************************************************************************
//extern unsigned char        bmp6[];
//extern unsigned char        mybmp[];
extern const unsigned char  LogoRoxton[];
//extern const unsigned char  Spicker[];
//*****************************************************
//*****************************************************
Menu_t MenuStr = {
	//---------------------
	.Index.Page = 0,
	.Index.Item = 0,	
	//---------------------
	//Страница меню 1 - "Общие настройки"
	.Page[Page1].Name = RusText_CofigModeUnit,
	.Page[Page1].Item1.Name = RusText_Address,
	.Page[Page1].Item2.Name = RusText_Group,
	.Page[Page1].Item3.Name = RusText_Time,
	.Page[Page1].Item4.Name = RusText_Date,
	//---------------------
	//Страница меню 2 - "Входные клеммы"
	.Page[Page2].Name = RusText_PojLine,
//	.Page[Page2].Item1.Name = RusText_Poj1,
//	.Page[Page2].Item2.Name = RusText_Poj2,
//	.Page[Page2].Item3.Name = RusText_Poj3,
//	.Page[Page2].Item4.Name = RusText_GOChS,
	//---------------------
	//Страница меню 3 - "Линии Гр."
	.Page[Page3].Name = RusText_PageMenuZone,
	.Page[Page3].Item1.Name = RusText_SpLine1,
	.Page[Page3].Item2.Name = RusText_AttLine1,
	.Page[Page3].Item3.Name = RusText_SpLine2,
	.Page[Page3].Item4.Name = RusText_AttLine2,
	.Page[Page3].Item5.Name = RusText_Sensitivity,	
	//---------------------
	//Страница меню 4 - "Питание блока"
	.Page[Page4].Name = RusText_PowerPage,
	.Page[Page4].Item1.Name = RusText_MainPower,
	.Page[Page4].Item2.Name = RusText_ReservePower,
	.Page[Page4].Item3.Name = RusText_BatLevel,
	.Page[Page4].Item4.Name = RusText_PA,
	//---------------------
	//Страница меню 5 - "Световые табло"
	.Page[Page5].Name = RusText_PageMenuTablo,
//	.Page[Page5].Item1.Name = RusText_PitanieTablo,
//	.Page[Page5].Item2.Name = RusText_Tablo1,
//	.Page[Page5].Item3.Name = RusText_Tablo2,
//	.Page[Page5].Item4.Name = RusText_Tablo3,
	//---------------------
	//Страница меню 6 - "Журнал событий"
	.Page[Page6].Name = RusText_EventLog,
	//---------------------
};
//*********************************************************************************************
//*********************************************************************************************
//Шапка окна.
static void Page_Header(const char* NamePage){
  
	//Имя страницы меню.
  LcdSetCursor(1, 1);
  LcdOutStr((char*)NamePage);
  //Вывод времени в правый верхний угол.
  LcdSetCursor(17, 1);
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



//*********************************************************************************************
//*********************************************************************************************
//Инициализация пунктов меню.
void Menu_Init(void){

}
//-----------------------------------------------------------------------------
Page_t* Menu(uint8_t page){

  return &MenuStr.Page[page];
}
//-----------------------------------------------------------------------------
//Получение переменных для работы с меню.
Index_t* Menu_IndexReg(void){

  return &MenuStr.Index;
}
//-----------------------------------------------------------------------------
//Сброс переменных для работы с меню.
void Menu_IndexRegCleare(void){

	MenuStr.Index.Page = 0;
  MenuStr.Index.Item = 0;
}
//-----------------------------------------------------------------------------
//Вывод на дисплей логотопа.
void Menu_Logo(void){
  
	LcdClear();
 	LcdImage(LogoRoxton);
	//LcdImage(Spicker);
  LcdUpdateAll();
}
//-----------------------------------------------------------------------------
//Горизонтальный прогресбар.
void Menu_ProgressBar(void){

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
		case(DeviceOn)   : return (char*)RusText_PowerOn;   
		//Основное питание.
		case(LogACNorm)  : return (char*)RusText_PowerAcNorm;
		case(LogACFault) : return (char*)RusText_PowerAcFault;     
		//Инвертор.
		case(LogDCNorm)  : return (char*)RusText_PowerDcNorm;   
		case(LogDCFault) : return (char*)RusText_PowerDcFault;  
		//АКБ.
		case(LogBatNorm) : return (char*)RusText_BatNorm;   		
		case(LogBatFault): return (char*)RusText_BatFault;   
		//шлейфы пожарные.
		case(LogFireLine1Norm) : return (char*)RusText_FireLine1Norm; 		
		case(LogFireLine1Fault): return (char*)RusText_FireLine1Fault;   		
		case(LogFireLine2Norm) : return (char*)RusText_FireLine2Norm;   		
		case(LogFireLine2Fault): return (char*)RusText_FireLine2Fault;
		case(LogFireLine3Norm) : return (char*)RusText_FireLine3Norm;   		
		case(LogFireLine3Fault): return (char*)RusText_FireLine3Fault;
		case(LogChSLineNorm)   : return (char*)RusText_ChSLineNorm;  		
		case(LogChSLineFault)  : return (char*)RusText_ChSLineFault; 
		//линии связи с Гр.
		case(LogSpLine1Norm)   : return (char*)RusText_SpLine1Norm;   		
		case(LogSpLine1Fault)  : return (char*)RusText_SpLine1Fault;
		case(LogSpAtten1Norm)  : return (char*)RusText_SpAtten1Norm;   		
		case(LogSpAtten1Fault) : return (char*)RusText_SpAtten1Fault;
		case(LogSpLine2Norm)   : return (char*)RusText_SpLine2Norm;		
		case(LogSpLine2Fault)  : return (char*)RusText_SpLine2Fault;
		case(LogSpAtten2Norm)  : return (char*)RusText_SpAtten2Norm; 		
		case(LogSpAtten2Fault) : return (char*)RusText_SpAtten2Fault;
		//линии связи с табличками ВЫХОД
		case(LogSiren1Norm) : return (char*)RusText_Siren1Norm;   		
		case(LogSiren1Fault): return (char*)RusText_Siren1Fault; 		
		case(LogSiren2Norm) : return (char*)RusText_Siren2Norm;   		
		case(LogSiren2Fault): return (char*)RusText_Siren2Fault;	
		case(LogSiren3Norm) : return (char*)RusText_Siren3Norm;  		
		case(LogSiren3Fault): return (char*)RusText_Siren3Fault;
		//Внешний микрофон.
		case(LogMicNorm) : return (char*)RusText_MicNorm;   		
		case(LogMicFault): return (char*)RusText_MicFault; 			
		//Обмен данными с лицевой пнелью.
		case(LogConnectNorm) : return (char*)RusText_ConnectNorm;   		
		case(LogConnectFault): return (char*)RusText_ConnectFault; 		
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
void Menu_AddressUpdate(uint8_t position, uint8_t data, uint8_t blink){

  //Адрес.
  if(position == 0)//Редактирование.
    {
      LcdSetCursor(8, 3);//Редактирумый разряд
      Menu(Page1)->Item1.Var = data;
			
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(Menu(Page1)->Item1.Var, 2);
      else     LcdOutStr("  ");
    }
  else//Отображение.
    {
      LcdSetCursor(8, 3);
      LcdBinToDec(Menu(Page1)->Item1.Var, 2);
    }
}
//-----------------------------------------------------------------------------
void Menu_GroupUpdate(uint8_t position, uint8_t data, uint8_t blink){

  //Группа.
  if(position == 0)//Редактирование.
    {
      LcdSetCursor(8, 4);//Редактирумый разряд
			Menu(Page1)->Item2.Var = data;
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(Menu(Page1)->Item2.Var, 2);
      else     LcdOutStr("  ");
    }
  else//Отображение.
    {
      LcdSetCursor(8, 4);
      LcdBinToDec(Menu(Page1)->Item2.Var, 2);
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
	LcdSetCursor(posInString, numString);
  LcdBinToDec(DateAhdTimeGet()->day, 2); 
	LcdChr('.');
	LcdBinToDec(DateAhdTimeGet()->month, 2); 
	LcdChr('.');
	LcdBinToDec(DateAhdTimeGet()->year, 4);
	LcdChr('/');
	//Вывод времени.
	posInString +=11;
  LcdSetCursor(posInString+2, numString);
  LcdChr(':');
  LcdSetCursor(posInString+5, numString);
  LcdChr(':');
  
	LcdSetCursor(posInString +(0 * 3), numString);
	LcdBinToDec(DateAhdTimeGet()->hour, 2); 
	LcdSetCursor(posInString +(1 * 3), numString);
	LcdBinToDec(DateAhdTimeGet()->min, 2); 
	LcdSetCursor(posInString +(2 * 3), numString);
	LcdBinToDec(DateAhdTimeGet()->sec, 2); 
}
//-----------------------------------------------------------------------------
void Menu_TimeUpdate(uint8_t position, uint16_t data, uint8_t blink, TimeSTR_Type *time){

  //static uint8_t CursorPosition = 0;
	//--------------------
  LcdSetCursor(10, 5);
  LcdChr(':');
  LcdSetCursor(13, 5);
  LcdChr(':');
  //--------------------
  //Часы.
  if(position == 0)//Редактирование.
    {
      LcdSetCursor(8 +(0 * 3), 5);//Редактирумый разряд
      time->hour = data;
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(time->hour, 2); 
      else     LcdOutStr("  ");
    }
  else//Отображение.
    {
      LcdSetCursor(8 +(0 * 3), 5);
      LcdBinToDec(time->hour, 2); 
    }
  //--------------------
  //Минуты.
  if(position == 1)//Редактирование.
    {
      LcdSetCursor(8 +(1 * 3), 5);//Редактирумый разряд
      time->min = data;
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(time->min, 2); 
      else     LcdOutStr("  ");
    }
  else//Отображение.
    {
      LcdSetCursor(8 +(1 * 3), 5);
      LcdBinToDec(time->min, 2); 
    }
  //--------------------
  //Секунды.
  if(position == 2)//Редактирование.
    {
      LcdSetCursor(8 +(2 * 3), 5);//Редактируемый разряд
      time->sec = data;
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(time->sec, 2); 
      else     LcdOutStr("  ");
    }
  else//Отображение.
    {
      LcdSetCursor(8 +(2 * 3), 5);
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
void Menu_CalendarUpdate(uint8_t position, uint16_t date, uint8_t blink, TimeSTR_Type* Data){

  //static uint8_t CursorPosition = 0;
	//--------------------
  LcdSetCursor(10, 6);
  LcdChr('.');
  LcdSetCursor(13, 6);
  LcdChr('.');
  //--------------------
  //День.
  if(position == 0)//Редактирование.
    {
      LcdSetCursor(8 +(0 * 3), 6);//Редактирумый разряд
      Data->day = date;
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(Data->day, 2); 
      else     LcdOutStr("  ");
    }
  else//Отображение.
    {
      LcdSetCursor(8 +(0 * 3), 6);
      LcdBinToDec(Data->day, 2); 
    }
  //--------------------
  //Месяц.
  if(position == 1)//Редактирование.
    {
      LcdSetCursor(8 +(1 * 3), 6);//Редактирумый разряд
      Data->month = date;
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(Data->month, 2); 
      else     LcdOutStr("  ");
    }
  else//Отображение.
    {
      LcdSetCursor(8 +(1 * 3), 6);
      LcdBinToDec(Data->month, 2); 
    }
  //--------------------
  //Год.
	if(position == 2)//Редактирование.
    {
      LcdSetCursor(14 +(0 * 3), 6);//Редактирумый разряд
      Data->year = date;
      //Мигаем редактируемым символом.
      if(blink)LcdBinToDec(Data->year, 4); 
      else     LcdOutStr("    ");
    }
  else//Отображение.
    {
      LcdSetCursor(14 +(0 * 3), 6);
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
void Menu_InputConfigUpdate(uint8_t numStr, uint8_t digit, FireLine_t *ptr){
	
	//--------------------
  //Тип входа.
	LcdSetCursor(18, numStr + 3);
	if(digit == 1)//Мигаем редактируемым символом.
    {
      if(Blink(INTERVAL_250_mS))LcdBinToDec(ptr->Type, 1); 
      else     LcdOutStr(" ");
    }
  else LcdBinToDec(ptr->Type, 1);//Отображение.
  //--------------------
	//Таймаут перехода из режима ВНИМАНИЕ в режим ПУСК. От 0 до 99 сек.
	LcdSetCursor(13, numStr + 3);
	if(digit == 0)//Мигаем редактируемым символом.
    {
      if(Blink(INTERVAL_250_mS))LcdBinToDec(ptr->TimeOut, 2); 
      else     LcdOutStr("  ");
    }
  else LcdBinToDec(ptr->TimeOut, 2); //Отображение.
  //--------------------
}
//-----------------------------------------------------------------------------
//mode = 1 - редактирование параметров меню
//mode = 0 - 
void Menu_Update(Page_t *menuPage, uint8_t mode, uint8_t pointStr){
  
  LcdClear();                        //Очистка видеопамяти.
  Page_Header((char*)menuPage->Name);//Вывод шапки страницы.
  //--------------------
  //При редактировании параметров меню справа от редактируемого
	//параметра выводится "<=".
  if(mode)
    {
      LcdSetCursor(20, (3 + pointStr));
      LcdOutStr("<=");
    }
  //----------------------------------------
  //1-я страница меню - Настриваемые параметры, время, дата.
  if(menuPage == Menu(Page1))
    {
      //Адрес
      LcdSetCursor(1, 3);
			LcdOutStr((char*)menuPage->Item1.Name);
			LcdSetCursor(7, 3);
			LcdChr('-');
      Menu_AddressUpdate(3, 0, 0);
      //Группа
      LcdSetCursor(1, 4);
      LcdOutStr((char*)menuPage->Item2.Name);
      Menu_GroupUpdate(3, 0, 0);
      //Время
      LcdSetCursor(1, 5);
      LcdOutStr((char*)menuPage->Item3.Name);
      Menu_TimeUpdate(3, 0, 0, DateAhdTimeGet());		
      //Дата
      LcdSetCursor(1, 6);
      LcdOutStr((char*)menuPage->Item4.Name);
      LcdBinToDec(DateAhdTimeGet()->day, 2);
      LcdChr('.');
      LcdBinToDec(DateAhdTimeGet()->month, 2);
      LcdChr('.');
      LcdBinToDec(DateAhdTimeGet()->year, 4);
			//-------------
			return;
    } 
  //----------------------------------------
  //2-я страница меню - Состояние пожарных шлейфов.
  if(menuPage == Menu(Page2))
    {
			//"Шл.|Сост.|Сек.|Тип"
			LcdSetCursor(1, 3);
			LcdOutStr((char*)&RusText_PojLineTable[0]);
			//--------------------
			//шлейф Пож.1
      LcdSetCursor(1, 4);
      LcdOutStr((char*)RusText_Poj1);
			//физическое состояние шлейфа
			LcdOutStr("-");
			Print_FireLineState(Poj1);
			//Время перехода из ВНИМАНИЕ в ПУСК в сек.
			LcdSetCursor(11, 4);
			LcdOutStr("- ");
			LcdBinToDec(FireLine(Poj1)->TimeOut, 2); 
			//Тип входа
			LcdOutStr(" - ");
			LcdBinToDec(FireLine(Poj1)->Type, 1);  		
			//--------------------
      //шлейф Пож.2
      LcdSetCursor(1, 5);
      LcdOutStr((char*)RusText_Poj2);
			//физическое состояние шлейфа
			LcdOutStr("-");
      Print_FireLineState(Poj2);
			//Время перехода из ВНИМАНИЕ в ПУСК в сек.
			LcdSetCursor(11, 5);
			LcdOutStr("- ");
			LcdBinToDec(FireLine(Poj2)->TimeOut, 2); 
			//Тип входа
			LcdOutStr(" - ");
			LcdBinToDec(FireLine(Poj2)->Type, 1);  		
			//--------------------
      //шлейф Пож.3
      LcdSetCursor(1, 6);
      LcdOutStr((char*)RusText_Poj3);
			//физическое состояние шлейфа
			LcdOutStr("-");
      Print_FireLineState(Poj3);
			//Время перехода из ВНИМАНИЕ в ПУСК в сек.
			LcdSetCursor(11, 6);
			LcdOutStr("- ");
			LcdBinToDec(FireLine(Poj3)->TimeOut, 2); 
			//Тип входа
			LcdOutStr(" - ");
			LcdBinToDec(FireLine(Poj3)->Type, 1);  		
			//--------------------
      //шлейф ГОиЧС
      LcdSetCursor(1, 7);
      LcdOutStr((char*)RusText_GOChS);
			//физическое состояние шлейфа
			LcdOutStr("-");
      Print_FireLineState(ChS);
			//Время перехода из ВНИМАНИЕ в ПУСК в сек.
			LcdSetCursor(11, 7);
			LcdOutStr("- ");
			LcdBinToDec(FireLine(ChS)->TimeOut, 2); 
			//Тип входа
			LcdOutStr(" - ");
			LcdBinToDec(FireLine(ChS)->Type, 1);  		
			//-------------
			return;
    }
  //----------------------------------------
  //3-я страница меню - Состояние линий Гр.
  if(menuPage == Menu(Page3))
    {
      //Зона 1
      LcdSetCursor(1, 3);
      LcdOutStr((char*)menuPage->Item1.Name);
      LcdOutStr((char*)menuPage->Item1.Text);
      //Аттенюатер 1
      LcdSetCursor(1, 4);
      LcdOutStr((char*)menuPage->Item2.Name);
      LcdOutStr((char*)menuPage->Item2.Text);
      //Зона 2
      LcdSetCursor(1, 5);
      LcdOutStr((char*)menuPage->Item3.Name);
      LcdOutStr((char*)menuPage->Item3.Text);
      //Аттенюатер 2
      LcdSetCursor(1, 6);
      LcdOutStr((char*)menuPage->Item4.Name);
      LcdOutStr((char*)menuPage->Item4.Text);
      //Отклонение для фиксации ПП на линии Гр.
      LcdSetCursor(1, 7);
      LcdOutStr((char*)menuPage->Item5.Name);
			LcdBinToDecWithDot(menuPage->Item5.Var, 0, "mB");
 			//-------------
			return;
     }
  //----------------------------------------
  //4-я страница меню - Питание блока.
  if(menuPage == Menu(Page4))
    {
      //Основное питание
      LcdSetCursor(1, 3);
      LcdOutStr((char*)menuPage->Item1.Name);
      LcdOutStr((char*)menuPage->Item1.Text);
			//Резервное питание
			LcdSetCursor(1, 4);
			LcdOutStr((char*)menuPage->Item2.Name);
      LcdOutStr((char*)menuPage->Item2.Text);
      //Напряжение на АКБ.
      LcdSetCursor(1, 5);
      LcdOutStr((char*)menuPage->Item3.Name);
			
			if(Power()->State.bits.Bat != BAT_CHECK_OFF) LcdBinToDecWithDot(menuPage->Item3.Var, 2, "B");
      else                             				     LcdOutStr((char*)RusText_Undef);
      //Состояние УМЗЧ.
      LcdSetCursor(1, 6);
      LcdOutStr((char*)menuPage->Item4.Name);
      LcdOutStr((char*)Text_GetAmpState(Amp_GetState()));
			//-------------
			return;
    }
  //----------------------------------------
  //5-я стриницы меню. - Состояние оповещателей.
  if(menuPage == Menu(Page5))
    {
			//Нет связи с платой.
			if(Siren()->PowerVol == 0xFFFF)
				{
					LcdSetCursor(1, 3);
					LcdOutStr((char*)&RusText_NoConnectionSB[0]);
				}
			//Отображение данных.
			else
				{
					//Питание табло.
					LcdSetCursor(1, 3);
					LcdOutStr((char*)RusText_PitanieTablo);
					LcdBinToDecWithDot(Siren()->PowerVol, 2, "B");
					//Табло1.
					LcdSetCursor(1, 4);
					LcdOutStr((char*)RusText_Tablo1);
					LcdChr('-');
					Print_SirenSate(Siren()->Siren1.LineState);
					//Табло2.
					LcdSetCursor(1, 5);
					LcdOutStr((char*)RusText_Tablo2);
					LcdChr('-');
					Print_SirenSate(Siren()->Siren2.LineState);
					//Табло3.
					LcdSetCursor(1, 6);
					LcdOutStr((char*)RusText_Tablo3);
					LcdChr('-');
					Print_SirenSate(Siren()->Siren3.LineState);
				}
			//-------------
			return;				
    }
  //----------------------------------------
  //6-я стриницы меню - Журнал событий.
  if(menuPage == Menu(Page6))
    {
      LcdSetCursor(1, 3);
      LcdBinToDec(Menu(Page6)->Item1.Var, 4);//Общее количество событий
			LcdOutStr("/");												 //разделитель
      LcdBinToDec(Menu(Page6)->Item2.Var, 4);//Количество непросмотренных событий
      
			if(Log()->NotReadEvents) //Вывод надписи "Есть н.с." если есть новые события
				{
					//LcdString(1, 4);
					LcdOutStr((char*)&RusText_NotRead[0]) ;		
				}
			//Номер события.
			if(Log()->Event.Number == 0) return;
      LcdSetCursor(1, 5);
			LcdOutStr((char*)&RusText_Event[0]);		
			LcdBinToDec(menuPage->Item3.Var, 4);
      //Дата и время события.		
      DisplayDateAndTimeEvent(Char1, String6, Log()->Event.TimeUtc);
      //Код события
      LcdSetCursor(1, 7);
			if(LogGetRusText(menuPage->Item4.Var) != 0)
				{
					LcdOutStr(LogGetRusText(menuPage->Item4.Var));
				}				
      //Параметр события.
			//LcdString(8, 7);
			//LcdBinToDec(menuPage->PageItems.StringVar[String5], 2);
			//-------------
			return;
    }
	//----------------------------------------
  //7-я стриницы меню.
  if(menuPage == Menu(Page7))
    {
			//Отладка. Вывод конфигурации пожарного входа.
			//шапка страницы
			LcdSetCursor(1, String1 + 1);
			LcdOutStr("EEPROM Info:");   
  
			//-------------
    }
  //----------------------------------------  
}
//-----------------------------------------------------------------------------
//Вывод на дисплей надписи "Нет связи с ЦП!!!" при отсутствии связи с материнской платой.
void Print_ErrConnect(void){
  
  LcdClear(); 
	LcdSetCursor(3, 4);
  LcdOutStr((char*)RusText_ErrorConnectMB);
}
//-----------------------------------------------------------------------------
void Print_MicState(void){

	LcdSetCursor(1, 4);
  //--------------------
  switch(MicState()){
		//--------------------
		case(MIC_CONNECTED):
			LcdOutStr((char*)RusText_MicConnect);
		break;
		//--------------------
		case(MIC_NOT_CONNECT):
			LcdOutStr((char*)RusText_MicNotConnect);
		break;
		//--------------------
		case(MIC_FAULT):
			LcdOutStr((char*)RusText_MicFault);
		break;
		//--------------------
		case(MIC_ACTIVE):
			LcdOutStr((char*)RusText_MicActive);
		break;
		//--------------------
  }
  //--------------------
}
//-----------------------------------------------------------------------------
void Print_PowerStateFromMB(void){
	
	//Вывод состояния основного питания.
	if(Power()->StateFromMB.bits.AC == POWER_AC_CHECK_OFF) Menu(Page4)->Item1.Text = (char*)&RusText_LcOff;
	else	                                                 Menu(Page4)->Item1.Text = Text_GetAcPowerState(Power()->StateFromMB.bits.AC);
	//Вывод состояния АКБ
	if(Power()->StateFromMB.bits.Bat == BAT_CHECK_OFF) Menu(Page4)->Item2.Text = (char*)&RusText_LcOff;
	else																							 Menu(Page4)->Item2.Text = Text_GetBatState(Power()->StateFromMB.bits.Bat);
}
//-----------------------------------------------------------------------------
void Print_PowerState(void){
	
	//Вывод сотсояния основного питания.
	if(Power()->State.bits.AC == POWER_AC_CHECK_OFF) Menu(Page4)->Item1.Text = (char*)&RusText_LcOff;
	else												  	                 Menu(Page4)->Item1.Text = Text_GetAcPowerState(Power()->StateFromMB.bits.AC);
	//Вывод состояния АКБ
	if(Power()->State.bits.Bat == BAT_CHECK_OFF) Menu(Page4)->Item2.Text = (char*)&RusText_LcOff;	
	else											                   Menu(Page4)->Item2.Text = Text_GetBatState(Power()->StateFromMB.bits.Bat);
}
//-----------------------------------------------------------------------------
//Вывод на дисплей состояния линий Гр. поставленных на контроль.
void Print_SpLineState(uint8_t line){
	
	if(line & (1 << Line1))Menu(Page3)->Item1.Text = Text_GetSpLineState(Line1);
	else									 Menu(Page3)->Item1.Text = RusText_LcOff;
	
	if(line & (1 << Att1)) Menu(Page3)->Item2.Text = Text_GetSpLineState(Att1);
	else									 Menu(Page3)->Item2.Text = RusText_LcOff;
	
	if(line & (1 << Line2))Menu(Page3)->Item3.Text = Text_GetSpLineState(Line2);
	else									 Menu(Page3)->Item3.Text = RusText_LcOff;
	
	if(line & (1 << Att2)) Menu(Page3)->Item4.Text = Text_GetSpLineState(Att2);
	else									 Menu(Page3)->Item4.Text = RusText_LcOff;			
}
//-----------------------------------------------------------------------------
void Print_SirenSate(uint8_t line){

	     if(line == SIREN_LINE_NORM)	LcdOutStr((char*)&RusText_Norm);
	else if(line == SIREN_LINE_BREAK) LcdOutStr((char*)&RusText_Break);
	else if(line == SIREN_LINE_SHORT) LcdOutStr((char*)&RusText_Short);
	else 															LcdOutStr((char*)&RusText_Analyze);
}
//-----------------------------------------------------------------------------
void Print_FireLineState(uint8_t line){
	
	uint8_t frState = FireLine_GetLineState(line);	
	//----------------------
	if(FireLine_GetInState(line) == FR_IN_CONTROL_OFF) LcdOutStr((char*)&RusText_Disactive);	
	else if(frState == FR_LINE_UNDEF )     LcdOutStr((char*)&RusText_Analyze);//LcdOutStr((char*)&RusText_Undef);
  else if(frState == FR_LINE_NORM_OPEN)  LcdOutStr((char*)&RusText_Open);
  else if(frState == FR_LINE_NORM_CLOSE) LcdOutStr((char*)&RusText_Close);
  else if(frState == FR_LINE_SHORT) 		 LcdOutStr((char*)&RusText_Short);
  else if(frState == FR_LINE_BREAK) 		 LcdOutStr((char*)&RusText_Break);
	else 										               LcdOutStr((char*)&RusText_Short);
}
//******************************************************************************************
//******************************************************************************************
char* Text_GetBatState(uint8_t state){

	if(state == BAT_CHECK_OFF)   return (char*)RusText_LcOn;
	if(state == BAT_OK)	         return (char*)RusText_Norm;
	if(state == BAT_CHARGE)	     return (char*)RusText_BatCharge;       	
	if(state == BAT_CHARGE_END)  return (char*)RusText_BatChargeEnd;
	if(state == BAT_ATTENTION) 	 return (char*)RusText_BelowNorm;
	if(state == BAT_DEEP)      	 return (char*)RusText_BatDeepCharge;	
															 return (char*)RusText_BatNo;
}
//-----------------------------------------------------------------------------
char* Text_GetAcPowerState(uint8_t state){
	
	if(state == POWER_AC_CHECK_OFF) return (char*)RusText_LcOn;
	if(state == POWER_AC_OK)    		return (char*)RusText_Norm;
	if(state == POWER_AC_FAULT)  		return (char*)RusText_NotWork;
	return 0;
}	
//-----------------------------------------------------------------------------
char* Text_GetDcPowerState(uint8_t state){

	if(state == POWER_DC_CHECK_OFF) return (char*)RusText_LcOn;
	if(state == POWER_DC_OK)    		return (char*)RusText_Norm;
	if(state == POWER_DC_FAULT)  		return (char*)RusText_NotWorkDcDc;
	return 0;
}	
//-----------------------------------------------------------------------------
char* Text_GetAmpState(uint8_t state){
  
	if(state == PAMP_OFF)  return (char*)RusText_Disactive;
	if(state == PAMP_ON)   return (char*)RusText_On;
	if(state == PAMP_PROT) return (char*)RusText_PAProt;
	return 0;
}
//-----------------------------------------------------------------------------
char* Text_GetSpLineState(uint8_t line){
	
	uint8_t outState = SpLine_GetOutStateForTxt(line);
	//----------------------
	if(outState == LineOffControl) return (char*)RusText_LcOn;
  if(outState == LineNorm)  		 return (char*)RusText_Norm;
  if(outState == LineUndef) 		 return (char*)RusText_Analyze;
  if(outState == LineBreak) 		 return (char*)RusText_Break;
  if(outState == LineOver)  		 return (char*)RusText_AboveNorm;
  if(outState == LineUnder) 		 return (char*)RusText_BelowNorm;
	return (char*)&RusText_Short[0];
}
//******************************************************************************************
//******************************************************************************************
static void PrintActive(uint8_t string){
	
	LcdSetCursor(1, string+1);
	LcdOutStr((char*)RusText_Active);
	LcdOutStr((char*)RusText_PLine);
}
//***************************************************************
static void PrintFaults(uint8_t string){
	
	LcdSetCursor(1, string+1);
	//LcdOutStr((char*)RusText_NotWork);
	LcdOutStr((char*)RusText_SpLine);
}
//***************************************************************
void Display_Header(const char* txt){

	Page_Header(txt);
}
//***************************************************************
void Display_Mic(void){

	LcdSetCursor(1, 3);
	LcdOutStr((char*)RusText_MicActive);
	//Линия Гр1.
	LcdSetCursor(1, 4);
	LcdOutStr((char*)RusText_Active);
	LcdOutStr((char*)RusText_SpLine);
	LcdOutStr("1");
	//Линия Гр2.
	if(SpLine_GetOutState(Zone3) == ActionMicOn)
		{
			LcdSetCursor(1, 5);
			LcdOutStr((char*)RusText_Active);
			LcdOutStr((char*)RusText_SpLine);
			LcdOutStr("2");
		}
}
//***************************************************************
void Display_PuskButtonActivation(void){

	LcdSetCursor(1, String4);
	LcdOutStr((char*)RusText_PuskVoiceMessageNum);
	//Линия Гр1.	
	LcdSetCursor(1, 4);
	LcdOutStr((char*)RusText_Active);
	LcdOutStr((char*)RusText_SpLine);
	LcdOutStr("1");
	//Линия Гр2.	
	LcdSetCursor(1, 5);
	LcdOutStr((char*)RusText_Active);
	LcdOutStr((char*)RusText_SpLine);
	LcdOutStr("2");
}
//***************************************************************
void Display_Fire(void){

	LcdSetCursor(1, String4);
	LcdOutStr((char*)RusText_PuskAuto);
	//Шлейф ПОЖ1
	if(FireLine_GetInState(Poj1) == FR_IN_FIRE)
		{
			PrintActive(String4);
			LcdOutStr((char*)RusText_Poj1);
		}
	//Шлейф ПОЖ2
	if(FireLine_GetInState(Poj2) == FR_IN_FIRE)
		{
			PrintActive(String5);
			LcdOutStr((char*)RusText_Poj2);
		}
	//Шлейф ПОЖ3
	if(FireLine_GetInState(Poj3) == FR_IN_FIRE)
		{
			PrintActive(String6);
			LcdOutStr((char*)RusText_Poj3);
		}				
}
//***************************************************************
void Display_Faults(uint32_t faults){

	LcdSetCursor(1, String4);
	LcdOutStr((char*)RusText_Defect);	
	//--------------------
	//Линии пожарных шлейфов.
  //Шлейф ПОЖ1.
	if(faults & FaultPoj1_Flag)
		{
			LcdSetCursor(1, String5);
			LcdOutStr((char*)RusText_Poj1);
		}		
	//Шлейф ПОЖ2.
	if(faults & FaultPoj2_Flag)
		{
			LcdSetCursor(1, String6);
			LcdOutStr((char*)RusText_Poj2);
		}	
	//Шлейф ПОЖ3.
	if(faults & FaultPoj3_Flag)
		{
			LcdSetCursor(1, String7);
			LcdOutStr((char*)RusText_Poj3);
		}	
	//Шлейф ГОиЧС.
	if(faults & FaultChS_Flag)
		{
			LcdSetCursor(1, String8);
			LcdOutStr((char*)RusText_GOChS);
		}		
	//--------------------
	//Линии связи с громкоговорителями.	
	//Линия Гр1.
	if(faults & FaultLc1Line_Flag)
		{
			PrintFaults(String5);
			LcdOutStr("1");		
		}			
	//Линия Аттенюатора1.
	if(faults & FaultLc1Att_Flag)
		{
			PrintFaults(String6);
			LcdOutStr("2");			
		}		
	//Линия Гр2.
	if(faults & FaultLc2Line_Flag)
		{
			PrintFaults(String7);
			LcdOutStr("3");		
		}			
	//Линия Аттенюатора2.
	if(faults & FaultLc2Att_Flag)
		{
			PrintFaults(String8);
			LcdOutStr("4");			
		}
	//--------------------
	//Линии связи со световыми табличками.			
  //Табло1.
	if(faults & FaultSiren1_Flag)
		{
			LcdSetCursor(1, String5);
			LcdOutStr((char*)RusText_Tablo1);	
		}		
	//Табло2.
	if(faults & FaultSiren2_Flag)
		{
			LcdSetCursor(1, String6);
			LcdOutStr((char*)RusText_Tablo2);	
		}	
	//Табло3.
	if(faults & FaultSiren3_Flag)
		{
			LcdSetCursor(1, String7);
			LcdOutStr((char*)RusText_Tablo3);	
		}			
	//--------------------
}
//***************************************************************
void Display_Manual(void){

	LcdSetCursor(1, String4);
	LcdOutStr((char*)RusText_ManualActivation);
	//Линия Гр1.
	if(SpLine_GetOutState(Zone1) == ActionManualOn)
		{
			LcdSetCursor(1, 4);
			LcdOutStr((char*)RusText_Active);
			LcdOutStr((char*)RusText_SpLine);
			LcdOutStr("1");
		}	
	//Линия Гр2.
	if(SpLine_GetOutState(Zone3) == ActionManualOn)
		{
			LcdSetCursor(1, 5);
			LcdOutStr((char*)RusText_Active);
			LcdOutStr((char*)RusText_SpLine);
			LcdOutStr("2");
		}	
}
//***************************************************************
void Display_AddressGroupAndMicState(void){

  //Адрес
  LcdSetCursor(1, String4);
  LcdOutStr((char*)RusText_Address);
	LcdChr('-');
  LcdBinToDec(MotherBoard()->Addres, 2);
  //Группа
  LcdSetCursor(13, String4);
  LcdOutStr((char*)RusText_Group);
  LcdBinToDec(MotherBoard()->Group, 2);	
	//Cостояние микрофона.  
	Print_MicState();
}
//***************************************************************
void Display_Test(void){

	LcdClear();
	LcdStringBig(3, 4);
	LcdOutStr((char*)RusText_TESTMode); 
}
//******************************************************************************************
//******************************************************************************************













