
#ifndef _RusText_H
#define _RusText_H
//-----------------------------------------------------------------------------
//Положение ключа.
const char RusText_LockModeUnit[]    = "БЛОКИРОВКА УПР.";
const char RusText_ControlModeUnit[] = "УПРАВЛЕНИЕ";

//-----------------------------------------------------------------------------
//страница "Общие настройки" режима "НАСТРОЙКА".
const char RusText_CofigModeUnit[] = "Общие настройки";
const char RusText_Address[]       = "Адрес";//"Адрес -";
const char RusText_Group[]         = "Группа-";
const char RusText_Time[]          = "Время -";
const char RusText_Date[]          = "Дата  -";

//-----------------------------------------------------------------------------
//страница "Входные клеммы" режима "НАСТРОЙКА".
const char RusText_PojLine[]      =	"Входные клеммы";
const char RusText_PojLineTable[] =	"Вход|Сост.|Сек.|Ржм|";
const char RusText_Poj1[]         = "Пож1";
const char RusText_Poj2[]         = "Пож2";
const char RusText_Poj3[]         = "Трвг";
const char RusText_GOChS[]        = " ЧС ";

//-----------------------------------------------------------------------------
//страница "Линии Гр." режима "НАСТРОЙКА".
const char RusText_PageMenuZone[] = "Линии Гр.";
const char RusText_SpLine[]       =	"Лин.Гр";
const char RusText_SpLine1[] 			= "Лин.Гр1-";
const char RusText_SpLine2[] 			= "Лин.Гр2-";
const char RusText_SpLine3[] 			= "Лин.Гр3-";
const char RusText_SpLine4[] 			=	"Лин.Гр4-";
const char RusText_SpLine5[] 			= "Лин.Гр5-";
const char RusText_SpLine6[] 			= "Лин.Гр6-";

const char RusText_AttLine[] 			= "Лин.Ат";
const char RusText_AttLine1[] 		= "Лин.Ат1-";
const char RusText_AttLine2[] 		= "Лин.Ат2-";
const char RusText_AttLine3[]		  = "Лин.Ат3-";
const char RusText_AttLine4[] 		= "Лин.Ат4-";
const char RusText_AttLine5[] 		= "Лин.Ат5-";
const char RusText_AttLine6[] 		= "Лин.Ат6-";
const char RusText_Sensitivity[]  = "Чувств.-";

//-----------------------------------------------------------------------------
//страница "Питание блока" режима "НАСТРОЙКА".
const char RusText_PowerPage[]    = "Питание блока";
const char RusText_MainPower[]    = "Пит.1-";
const char RusText_ReservePower[] = "Пит.2-";
const char RusText_BatLevel[]     = "Uакб -";
const char RusText_PA[]           = "УМЗЧ -";

//-----------------------------------------------------------------------------
//страница "Световые табло" режима "НАСТРОЙКА".
const char RusText_PageMenuTablo[]  = "Световые табло";
const char RusText_PitanieTablo[]   = "Пит.табло=";
const char RusText_Tablo1[] =         "Табло1";
const char RusText_Tablo2[] =         "Табло2";
const char RusText_Tablo3[] =         "Табло3";

//-----------------------------------------------------------------------------
//страница "Журнал событий" режима "НАСТРОЙКА".
const char RusText_EventLog[] = "Журнал событий";
const char RusText_Event[]    = "Событие ";
const char RusText_NotRead[]  = " Есть н.с.";

//-----------------------------------------------------------------------------
//Обмен данными.
const char RusText_ErrorConnectMB[] = "Нет связи с ЦП!!!";
const char RusText_NoConnectionSB[] = "Нет связи с модулем!!";

//-----------------------------------------------------------------------------
//МИКР.
const char RusText_MicNotConnect[] = "Микрофон отсутствует";
const char RusText_MicConnect[]    = "Микрофон подключен";
const char RusText_MicActive[]     = "АКТИВИРОВАН МИКР.";

//-----------------------------------------------------------------------------
//Алгоритмы.
const char RusText_Alg[]  = "Алг.";
const char RusText_Alg1[] = "Алг.1";
const char RusText_Alg2[] = "Алг.2";
const char RusText_Alg3[] = "Алг.3";
const char RusText_Alg4[] = "Алг.4";
const char RusText_Alg5[] = "Алг.5";
const char RusText_Alg6[] = "Алг.6";
const char RusText_Alg7[] = "Алг.7";
const char RusText_Alg8[] = "Алг.8";

//-----------------------------------------------------------------------------
//Пожарные шлейфа 
const char RusText_PLine[] = "ШС ";//"Шлейф ";

//-----------------------------------------------------------------------------
//Состояния линий связи.
const char RusText_Close[]     = "Змкн.";
const char RusText_Open[]      = "Рзмк.";
const char RusText_On[]        = "Вкл.";
const char RusText_Active[]    = "Акт.";
const char RusText_Activated[] = "Aктивирован ";
const char RusText_Disactive[] = "Откл.";
const char RusText_Norm[]      = "Норма";
const char RusText_AboveNorm[] = "Выше нормы";
const char RusText_BelowNorm[] = "Ниже нормы";
const char RusText_Break[]     = "Обрыв";
const char RusText_Short[]     = "КЗ";
const char RusText_Undef[]     = "Неоп.";
const char RusText_Analyze[]   = "Опрос";
const char RusText_LcOn[]    	 = "Контр.Вкл.";
const char RusText_LcOff[]   	 = "Контр.Откл.";
//-------------------------------------------------------
//Состояния "Питание блока"
const char RusText_PAProt[]        = "Защита!";
const char RusText_NotWork[]       = "Неиспр.";
const char RusText_NotWorkDcDc[]   = "Неиспр.Инверт.";

const char RusText_BatCharge[]     = "Заряд";
const char RusText_BatChargeEnd[]  = "Заряд окончен";
const char RusText_BatDeepCharge[] = "Гл.разряд";
const char RusText_BatNo[]         = "Нет АКБ";//"Не подкл.";

//-------------------------------------------------------
//Логирование.
const char RusText_EventID[] 				= "Код события-";

const char RusText_PowerOn[]       	= "Подано питание.";
const char RusText_PowerAcNorm[]    = "Пит.1 в норме";
const char RusText_PowerAcFault[]   = "Пит.1 неиспр.";
const char RusText_PowerDcNorm[]   	= "DC/DC в норме";
const char RusText_PowerDcFault[]  	= "DC/DC неисправен";
const char RusText_BatNorm[]       	= "Пит.2 в норме";
const char RusText_BatFault[]      	= "Пит.2 неисправен";
//
const char RusText_FireLine1Norm[] 	= "ШС Пож.1 в норме";
const char RusText_FireLine1Fault[]	= "ШС Пож.1 неисправен";
const char RusText_FireLine2Norm[] 	= "ШС Пож.2 в норме";
const char RusText_FireLine2Fault[]	= "ШС Пож.2 неисправен";
const char RusText_FireLine3Norm[] 	= "ШС Пож.3 в норме";
const char RusText_FireLine3Fault[]	= "ШС Пож.3 неисправен";
const char RusText_ChSLineNorm[]   	= "ШС ЧС в норме";
const char RusText_ChSLineFault[] 	= "ШС ЧС неисправен";
//
const char RusText_SpLine1Norm[]    = "Лин.Гр1 в норме";
const char RusText_SpLine1Fault[] 	= "Лин.Гр1 неисправнa";
const char RusText_SpAtten1Norm[]   = "Лин.Ат1 в норме";
const char RusText_SpAtten1Fault[] 	= "Лин.Ат1 неисправнa";
const char RusText_SpLine2Norm[]    = "Лин.Гр2 в норме";
const char RusText_SpLine2Fault[] 	= "Лин.Гр2 неисправнa";
const char RusText_SpAtten2Norm[]   = "Лин.Ат2 в норме";
const char RusText_SpAtten2Fault[] 	= "Лин.Ат2 неисправнa";
//
const char RusText_Siren1Norm[]   	= "Табло1 в норме";
const char RusText_Siren1Fault[] 		= "Табло1 неисправно";
const char RusText_Siren2Norm[]   	= "Табло2 в норме";
const char RusText_Siren2Fault[] 		= "Табло2 неисправно";
const char RusText_Siren3Norm[]   	= "Табло3 в норме";
const char RusText_Siren3Fault[] 		= "Табло3 неисправно";
//
const char RusText_MicNorm[]        = "Микрофон в норме";
const char RusText_MicFault[]       = "Микрофон неисправен";
//
const char RusText_ConnectNorm[]   	= "Обмен данными норм.";
const char RusText_ConnectFault[] 	= "Сбой обмена данными";
//-------------------------------------------------------

const char RusText_PuskVoiceMessageNum[] = "АКТ.СООБЩЕНИЕ"; //"Акт. сообщение ";
const char RusText_PuskAuto[]   				 = "ПУСК АВТОМАТИКИ";
const char RusText_Defect[]     			   = "НЕИСПРАВНОСТЬ";
const char RusText_ManualActivation[]    = "РУЧНАЯ АКТИВАЦИЯ"; //"Ручная активация";
const char RusText_TESTMode[] 			 		 = "РЕЖИМ ТЕСТИРОВАНИЯ.";
//-----------------------------------------------------------------------------
#endif 

