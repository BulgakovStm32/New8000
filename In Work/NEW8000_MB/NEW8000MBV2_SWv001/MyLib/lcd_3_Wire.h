#include <stm32f10x.h>
//-----------------------------------------------------------------------------
// ���������� ����� � ��� ������� ��� ������ � �� �������� �� 3-� ��������� ����.
#define PORT_LCD  GPIOA	  // ����, � �������� ��������� LCD.
	//-------------------
#define E_LCD  	  (1<<8)  // ����� � ������� ����������� ����� E ���.
#define DAT_LCD   (1<<9)  // ����� � ������� ����������� ����� CLK ���.
#define CLK_LCD   (1<<10) // ����� � ������� ����������� ����� D_In ���.
//-----------------------------------------------------------------------------
// ��������� ������� ������ � LCD.
void LCD_Init (void);
void Delay_LCD(uint32_t del);
void STROB_CLK (void);
void LCD_CMD_SEND (unsigned char cmd);
void LCD_DATA_SEND (unsigned char  data);
void LCD_OUT(char *txt);
void LCD_OUT_VARIABLE(uint32_t var);
void LCD_USER_SUMBOL(void);
void LCD_OUT_STRING(char *txt);
//-----------------------------------------------------------------------------
