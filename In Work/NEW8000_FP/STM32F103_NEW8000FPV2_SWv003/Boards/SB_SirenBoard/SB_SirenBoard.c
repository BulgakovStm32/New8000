
#include "SB_SirenBoard.h"

//-----------------------------------------------------------------------------
//Описание 
//Работа с оповещателями.
//-----------------------------------------------------------------------------
static SirenData_STR	SirenData;//Структура данных для хранения данных 
//-----------------------------------------------------------------------------
SirenData_STR* Siren(void){

	return &SirenData;
}
//-----------------------------------------------------------------------------
