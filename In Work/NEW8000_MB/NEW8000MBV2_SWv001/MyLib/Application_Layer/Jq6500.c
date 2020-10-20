
#include "Jq6500.h"

//-----------------------------------------------------------------------------
const unsigned char JqCmdChipResetArray[] = {0x7E, 0x02, 0x0C, 0xEF};
const unsigned char JqCmdPlayArray[]      = {0x7E, 0x02, 0x0D, 0xEF};

const unsigned char JqCmdPauseArray[]     = {0x7E, 0x02, 0x0E, 0xEF};
const unsigned char JqCmdSleepModeArray[] = {0x7E, 0x02, 0x0A, 0xEF};//Использовать вместо Pause

const unsigned char JqCmdNextArray[]      = {0x7E, 0x02, 0x01, 0xEF};
const unsigned char JqCmdPreArray[]       = {0x7E, 0x02, 0x02, 0xEF};

const unsigned char JqCmdVolMaxArray[]    = {0x7E, 0x03, 0x06, 0x15, 0xEF};
const unsigned char JqCmdVolUpArray[]     = {0x7E, 0x02, 0x04, 0xEF};
const unsigned char JqCmdVolDownArray[]   = {0x7E, 0x02, 0x05, 0xEF};

const unsigned char JqCmdReservArray[]    = {0x7E, 0x02, 0x10, 0xEF};//Не использовать.

unsigned char JqCmdLoopArray[]            = {0x7E, 0x03, 0x11, 0x00, 0xEF};
unsigned char JqCmdFolderSwArray[]        = {0x7E, 0x03, 0x0F, 0x00, 0xEF};

volatile uint8_t JqRespReg = 0;
//*********************************************************************************
//*************************Прикладной уровень. JQ6500.*****************************
void JQInit(void){

  Uart1Init();
  JQReset();
}
//-----------------------------------------------------------------------------
void JQTxCmd(uint8_t cmd, uint16_t value){

  uint8_t        i;
  uint8_t        *Buf;
  uint8_t        size  = 0;
  static uint8_t state = 0;
  //--------------------
  if(state == cmd)return;//Это нужно чтобы не слать одну и туже команду.
  state = cmd;
  //Смотрим, от кого пришли данные.
  switch(cmd){
      //-----------
      case(JqCmdChipReset):
        Buf  = (uint8_t*)JqCmdChipResetArray;
        size = 4;
      break;
      //-----------
      case(JqCmdPlay):
        Buf  = (uint8_t*)JqCmdPlayArray;
        size = 4;        
      break;
      //-----------
      case(JqCmdPause):
        Buf  = (uint8_t*)JqCmdPauseArray;
        size = 4;        
      break;
      //-----------
      case(JqCmdSleepMode):
        Buf  = (uint8_t*)JqCmdSleepModeArray;
        size = 4;        
      break;
      //-----------
      case(JqCmdNext):
        Buf  = (uint8_t*)JqCmdNextArray;
        size = 4;        
      break;
      //-----------
      case(JqCmdPre):
        Buf  = (uint8_t*)JqCmdPreArray;
        size = 4;        
      break;
      //-----------
      case(JqCmdVolMax):
        Buf  = (uint8_t*)JqCmdVolMaxArray;
        size = 5;        
      break;
      //-----------
      case(JqCmdVolUp):
        Buf  = (uint8_t*)JqCmdVolUpArray;
        size = 4;        
      break;
      //-----------
      case(JqCmdVolDown):
        Buf  = (uint8_t*)JqCmdVolDownArray;
        size = 4;        
      break;
     //-----------
     //Не использовать. После этой каманды модуль не реагирует на другие команды.
     //после сброса питания модуль функционирует нормально.
//      case(JqCmdReserv):
//        Buf  = (uint8_t*)JqCmdReservArray;
//        size = 4;        
//      break;
      //-----------
      case(JqCmdLoop):
        JqCmdLoopArray[3] = 1;//Single Cycle
        Buf  = (uint8_t*)JqCmdLoopArray;
        size = 5;        
      break;
     //-----------
      case(JqCmdFolderSw):
        JqCmdFolderSwArray[3] = 1;
        Buf  = (uint8_t*)JqCmdFolderSwArray;
        size = 5;        
      break;
      //-----------
      case(JqCmdNop):
      return;
      //-----------
      default: //Неизвестные данные.
      break;
      //-----------
    }
  //--------------------------
  USART1->SR;//Такая последовательность сбрасывает флаги ошибки 
  USART1->DR;//
  //Передача команды.
  for(i=0; i < size; i++)
    {
      //while(!(USART1->SR & USART_SR_TC));//Ждем пока бит TC в регистре SR станет 1
      while(!(USART1->SR & USART_SR_TXE));
      USART1->DR = *(Buf + i);           //Передача первого байта.
    }

//  Uart1StarTx(Buf, size);
  //--------------------------
  //Прием ответа.
//  size = 0; 
//  while ((USART1->SR & USART_SR_RXNE) == 0) {}
//  Buf[0] = USART1->DR;
//  while ((USART1->SR & USART_SR_RXNE) == 0) {}
//  Buf[1] = USART1->DR;

//  if(Buf[0] == 'O' && 
//     Buf[1] == 'K') 
//    {
//      Buf[0] = 0;
//      Buf[1] = 0;
//      JqRespReg  = JqRespOK;
//    }
  //--------------------------
}
//-----------------------------------------------------------------------------
void JQReset(void){

  //Эта последовательность необходима для корректного сброса.
  JQTxCmd(JqCmdPlay,      0);//
  JQTxCmd(JqCmdVolMax,    0);
  JQTxCmd(JqCmdLoop,      0);
  JQTxCmd(JqCmdSleepMode, 0);
  JQTxCmd(JqCmdChipReset, 0);
}
//-----------------------------------------------------------------------------
//Прием ответа на команду. - не отлажено!!!!
void JQRxResp(uint8_t data){

  static uint8_t JqRxCounterByte = 0;
  static uint8_t JqRxBuf[5] = {0, 0, 0, 0, 0};
  //--------------------
  JqRxBuf[JqRxCounterByte++] = data;//Прнятый байт в приемный буфер.
  //Ответ "OK".
  if(JqRxBuf[0] == 'O' && 
     JqRxBuf[1] == 'K') 
    {
      JqRxCounterByte = 0;
      JqRxBuf[0] = 0;
      JqRxBuf[1] = 0;
      JqRespReg  = JqRespOK;
      return;
    }
  //--------------------
  JqRxBuf[0] = 0;
  JqRxBuf[1] = 0;
  JqRxBuf[2] = 0;
  JqRxBuf[3] = 0;
  JqRxBuf[4] = 0;
  JqRxCounterByte = 0;
  JqRespReg = JqNoResp;
}
//-----------------------------------------------------------------------------
//Управление сообщениями.
void VoiceMessage(uint8_t cmd){
  
  //--------------------  
  switch(cmd){
      //-----------
      case(PlayMessage):
        JQTxCmd(JqCmdPlay, 0);//Запуск сообщения.
        JQTxCmd(JqCmdNop,  0);//
      break;
      //-----------
      case(StopMessage):
        JQTxCmd(JqCmdSleepMode, 0);//Сброс модуля JQ.
        JQTxCmd(JqCmdChipReset, 0);//
      break;
      //-----------
      case(ReplayMessage):

      break;    
      //-----------
      case(PlayMessage1):

      break; 
      //-----------
      case(PlayMessage2):

      break;     
      //-----------
      default:
      break;
      //-----------
    }    
  //--------------------
}
//*********************************************************************************
//*********************************************************************************

