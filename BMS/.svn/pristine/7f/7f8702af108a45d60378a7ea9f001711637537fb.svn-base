#ifndef _LCD_H
#define _LCD_H

#include "lpc17xx.h"
#include "type.h"

#define DISPLAYUP 		0x00
#define DISPLAYDOWN 	0x01

enum ROW_NUMBERS
{
  LINE1=0,
  LINE2	
};

void lcd_clear(void);
void lcd_init(void);
void lcd_putstring( char *string,unsigned char line);
void lcd_backlight_off(void);
void lcd_backlight_on(void);
void lcd_putchar(int c);
void DisplayDownArrow();
void DisplayUPArrow();
void lcd_buid_char();
void DisableCursor();
void EnableCursor();
void ShiftCursorLeft();
void BringCursorToHome();
void ShiftCursorLeftPos(U8 Pos);

#endif
