#include "lcd.h"
#include <string.h>


#define MAX_CHAR_IN_ONE_LINE 16

#define LCD_BACK_LIGHT_DIR LPC_GPIO1->FIODIR
#define LCD_BACK_LIGHT_SET LPC_GPIO1->FIOSET
#define LCD_BACK_LIGHT_CLR LPC_GPIO1->FIOCLR

#define LCD_DATA_DIR		LPC_GPIO2->FIODIR
#define LCD_DATA_SET		LPC_GPIO2->FIOSET
#define LCD_DATA_CLR		LPC_GPIO2->FIOCLR
#define LCD_DATA_PIN		LPC_GPIO2->FIOPIN

#define LCD_CTRL_CS_DIR		LPC_GPIO2->FIODIR
#define LCD_CTRL_CS_SET    	LPC_GPIO2->FIOSET
#define LCD_CTRL_CS_CLR    	LPC_GPIO2->FIOCLR

#define LCD_CTRL_RST_DIR	LPC_GPIO1->FIODIR
#define LCD_CTRL_RST_SET    LPC_GPIO1->FIOSET
#define LCD_CTRL_RST_CLR    LPC_GPIO1->FIOCLR

#define LCD_CTRL_BL_DIR	LPC_GPIO0->FIODIR
#define LCD_CTRL_BL_SET    LPC_GPIO0->FIOSET
#define LCD_CTRL_BL_CLR1    LPC_GPIO0->FIOCLR

#define LCD_CTRL_EN_DIR		LPC_GPIO0->FIODIR
#define LCD_CTRL_EN_SET     LPC_GPIO0->FIOSET
#define LCD_CTRL_EN_CLR    	LPC_GPIO0->FIOCLR

#define LCD_CTRL_RW_DIR		LPC_GPIO0->FIODIR
#define LCD_CTRL_RW_SET     LPC_GPIO0->FIOSET
#define LCD_CTRL_RW_CLR    	LPC_GPIO0->FIOCLR

#define LCD_CTRL_RS_DIR		LPC_GPIO0->FIODIR
#define LCD_CTRL_RS_SET     LPC_GPIO0->FIOSET
#define LCD_CTRL_RS_CLR    	LPC_GPIO0->FIOCLR


#define LCD_D0_POSTION1 2
#define LCD_D0 		(1<<2)
#define LCD_D1 		(1<<3)
#define LCD_D2 		(1<<4)
#define LCD_D3 		(1<<5)
#define LCD_D4 		(1<<6)
#define LCD_D5 		(1<<7)
#define LCD_D6 		(1<<8)
#define LCD_D7 		(1<<9)

#define LCD_BUSY_FLAG           LCD_D7

#define LCDRS	(1<<21)
#define LCDRW	(1<<27)
#define LCDEN 	(1<<22)
#define LCDBL	(1<<19)

#define LCD_DATA_MASK (0xFF)



const unsigned char DownArrow[] = { 0x04, 0x04, 0x04, 0x04, 0x15, 0x0E, 0x04,
		0x00 };
const unsigned char UpArrow[] =
		{ 0x04, 0x0E, 0x15, 0x04, 0x04, 0x04, 0x04, 0x00 };

#define DATA_POS 	LCD_D0_POSTION1
#define DELAY_COUNT 100

static int lcd_gotoxy(unsigned int x, unsigned int y);
static void LCD_buildCharacter(unsigned char location, const unsigned char *ptr);

void delay(int count) {
	int j = 0, i = 0;

	for (j = 0; j < count; j++) {
		/* At 60Mhz, the below loop introduces
		 delay of 10 us */
		for (i = 0; i < 35; i++);
	}
}

static void wait_lcd(void) {
	LCD_DATA_DIR &= ~(LCD_DATA_MASK << LCD_D0_POSTION1);
	LCD_CTRL_RS_CLR |= LCDRS;
	LCD_CTRL_RW_SET |= LCDRW;
	LCD_CTRL_EN_SET |= LCDEN;
	delay(20);

	while ((LCD_DATA_PIN & LCD_BUSY_FLAG))
		; /* wait for busy flag to become low */
	LCD_CTRL_EN_CLR |= LCDEN;
	LCD_CTRL_RW_CLR |= LCDRW;
	LCD_DATA_DIR |= (LCD_DATA_MASK << LCD_D0_POSTION1);
}

static void lcd_write_4bit(char c) {
	LCD_DATA_DIR |= (LCD_DATA_MASK << LCD_D0_POSTION1);

	LCD_CTRL_RW_CLR = LCDRW;
	LCD_DATA_CLR = (LCD_DATA_MASK << LCD_D0_POSTION1);
	LCD_DATA_SET = ((LCD_DATA_MASK & c) << LCD_D0_POSTION1);

	LCD_CTRL_EN_SET = LCDEN;
	delay(100);
	LCD_CTRL_EN_CLR = LCDEN;
	delay(100);
}

static void lcd_command_write(unsigned char command) {
	unsigned char temp = 0;
	unsigned int temp1 = 0;

	temp = command;
	temp1 = temp;
	LCD_CTRL_RS_CLR = LCDRS;
	lcd_write_4bit(temp1);
	wait_lcd();
}

static void lcd_data_write(unsigned char data) {
	unsigned char temp = 0;
	unsigned int temp1 = 0;

	temp = data;
	temp1 = temp;
	LCD_CTRL_RS_SET = LCDRS;
	lcd_write_4bit(temp1);
	wait_lcd();
}

static void set_lcd_port_output(void) {
	LCD_DATA_DIR |= (LCD_DATA_MASK << LCD_D0_POSTION1);

	LCD_CTRL_RS_DIR |= LCDRS;
	LCD_CTRL_EN_DIR |= LCDEN;
	LCD_CTRL_RW_DIR |= LCDRW;
	LCD_CTRL_BL_DIR |= LCDBL;

	LCD_DATA_CLR |= 0xF0;
	LCD_CTRL_EN_CLR = LCDEN;
	LCD_CTRL_BL_SET = LCDBL;
	LCD_DATA_CLR = 0x0F;
}

void lcd_clear(void) {
	lcd_command_write(0x01);
	lcd_gotoxy(0, 0);
}

static int lcd_gotoxy(unsigned int x, unsigned int y) {
	int retval = 0;

	if ((x > 1) && (y > 15)) {
		retval = -1;
	} else {
		if (x == 0) {
			lcd_command_write(0x80 + y); /* command - position cursor at 0x00 (0x80 + 0x00 ) */
		} else if (x == 1) {
			lcd_command_write(0xC0 + y); /* command - position cursor at 0x40 (0x80 + 0x00 ) */
		}
	}
	return retval;
}

void DisplayUPArrow(void) {
	lcd_data_write(DISPLAYUP);
}

void DisplayDownArrow(void) {
	lcd_data_write(DISPLAYDOWN);
}

void lcd_putchar(int c) {
	lcd_data_write(c);
}

void lcd_putstring(char *string, unsigned char line) {
	unsigned char len = MAX_CHAR_IN_ONE_LINE;

	lcd_gotoxy(line, 0);
	while (*string != '\0' && len--) {
		lcd_putchar(*string);
		string++;
	}
}

void lcd_clear_line(unsigned char line) {
	lcd_putstring("                  ", line); //16 spaces
}

void lcd_backlight_on(void) {
	LCD_BACK_LIGHT_DIR |= LCDBL;
	LCD_BACK_LIGHT_SET |= LCDBL;
}

void lcd_backlight_off(void) {
	LCD_BACK_LIGHT_DIR |= LCDBL;
	LCD_BACK_LIGHT_CLR |= LCDBL;
}

void lcd_init(void) {
	set_lcd_port_output();
	delay(10000);
	lcd_command_write(0x38); /*   8-bit interface, two line, 5X7 dots.        */
	delay(10000);
	lcd_command_write(0x0c); /*   cursor move direction                       */
	delay(10000);
	lcd_command_write(0x06); /*   display on      */
	delay(10000);
	lcd_gotoxy(0, 0);
	lcd_clear();
}

void DisableCursor(void) {
	lcd_command_write(0x0c);
}

void EnableCursor(void) {
	lcd_command_write(0x0f);
}

void ShiftCursorLeft(void) {
	lcd_command_write(0x10);
}

void BringCursorToHome(void) {
	lcd_command_write(0x02);
}

void ShiftCursorLeftPos(U8 Pos) {
	U8 i;

	for (i = 0; i < Pos; i++) {
		lcd_command_write(0x10);
	}
}
void lcd_buid_char(void) {
	LCD_buildCharacter(0, UpArrow);
	LCD_buildCharacter(1, DownArrow);
}

static void LCD_buildCharacter(unsigned char location, const unsigned char *ptr) {
	unsigned char i;

	if (location < 8) {
		lcd_command_write(0x40 + (location * 8));
		for (i = 0; i < 8; i++)
			lcd_data_write(ptr[i]);
	}
}
