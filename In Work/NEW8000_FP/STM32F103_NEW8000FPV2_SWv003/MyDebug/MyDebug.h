
#ifndef _MyDebug_H
#define _MyDebug_H
//-----------------------------------------------------------------------------

#include "stm32f10x.h"

//-----------------------------------------------------------------------------
//описание структуры пункта меню:
//typedef struct{
//	//---------
//	void       *Next;
//	void       *Previous;
//	void       *Parent;
//	void       *Child;
//	uint8_t     Select;
//	const char  Text[];
//	//---------	
//} MenuItem_STR;
////-----------------------------------------------------------------------------
////Самое полезное, почерпнутое у MicroMenu – набор дефайнов для быстрого и удобного определения меню.
//#define MAKE_MENU(Name, Next, Previous, Parent, Child, Select, Text) \
//	extern MenuItem_STR Next;     \
//	extern MenuItem_STR Previous; \
//	extern MenuItem_STR Parent;   \
//	extern MenuItem_STR Child;  \
//	MenuItem_STR Name = {(void*)&Next, (void*)&Previous, (void*)&Parent, (void*)&Child, (uint8_t)Select, { Text }}
////-----------------------------------------------------------------------------
//// для начала — пустой элемент. Который NULL на рисунке
//#define NULL_ENTRY Null_Menu
//MenuItem_STR	Null_Menu;// = {(void*)0, (void*)0, (void*)0, (void*)0, 0, {0x00}};
// 
//enum {
//    MENU_CANCEL=1,
//    MENU_RESET,
//    MENU_MODE1,
//    MENU_MODE2,
//    MENU_MODE3,
//    MENU_SENS1,
//    MENU_SENS2,
//};
	
//                 NEXT,      PREVIOUS     PARENT,     CHILD
//MAKE_MENU(m_s1i1,  m_s1i2,    NULL_ENTRY,  NULL_ENTRY, m_s2i1,       0, "Запуск");
//MAKE_MENU(m_s1i2,  m_s1i3,    m_s1i1,      NULL_ENTRY, m_s3i1,       0, "Настройка");
//MAKE_MENU(m_s1i3,  NULL_ENTRY,m_s1i2,      NULL_ENTRY, NULL_ENTRY,   MENU_RESET, "Сброс");	
	

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
#endif /*_MyDebug_H*/

