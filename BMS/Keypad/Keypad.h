#ifndef _KEYPAD_API_H_
#define _KEYPAD_API_H_

#include "stdint.h"

#define SPACE       ' '
#define ENTER       30
#define SHIFT  		26
#define ESC         1
#define MENU  		2
#define PPR_FD  	3
#define PRINT  		4
#define LEFT_ARROW  6
#define UP_ARROW    7
#define RIGHT_ARROW 8
#define DOWN_ARROW  12
#define FUNCTION  	9
#define DEL         10



#define FUN1  	1
#define FUN2  	2
#define FUN3  	3
#define FUN4  	4
#define FUN5  	5
#define FUN6  	6
#define FUN7  	7


#define FUNCTION1  	'1'
#define FUNCTION2  	'2'
#define FUNCTION3  	'3'
#define FUNCTION4  	'4'
#define FUNCTION5  	'5'
#define FUNCTION6  	'6'
#define FUNCTION7  	'7'
#define FUNCTION8  	'8'
#define FUNCTION9  	'9'


#define KEY_NOT_PRESSED 0xFF

void init_keypad(void);					/* Initialising the keyboard  					      */
uint8_t getkey(void);							/* Get pressed key from keyboard buffer			      */
uint8_t is_valid_ascii(uint8_t key);			/* Checking pressed key is a valid ascii code or not  */
uint8_t is_valid_num(uint8_t key);				/* Checking pressed key is a valid number or not      */
uint8_t is_valid_enter_key(uint8_t key);		/* Checking pressed key is a valid Enter key or not   */
uint8_t is_valid_function_key(uint8_t key);		/* Checking pressed key is a valid Function key or not*/
uint8_t is_valid_delete_key(uint8_t key);		/* Checking pressed key is a valid delete key or not  */
uint8_t is_valid_escape(uint8_t key);			/* Checking pressed key is a valid escape key or not  */
uint8_t is_valid_alphabet(uint8_t key);			/* Checking pressed key is a valid alphabet or not    */
uint8_t is_valid_pprfeed_key(uint8_t key);      /* Checking pressed key is a valid paper feed or not    */
uint8_t is_valid_print(uint8_t key);            /* Checking pressed key is a valid print or not    */
uint8_t is_valid_shift(uint8_t key);            /* Checking pressed key is a valid shift or not    */
uint8_t is_valid_up_arrow_key(uint8_t key);     /* Checking pressed key is a valid up-arrow or not    */
uint8_t is_valid_down_arrow_key(uint8_t key);   /* Checking pressed key is a valid down-arrow or not    */
uint8_t is_valid_space_key(uint8_t key);        /* Checking pressed key is a valid space or not    */
uint8_t is_valid_function_key(uint8_t key);
void Set_shift_key(uint8_t key);
uint8_t is_valid_menu_key(uint8_t key);
void power_off(void);
void power_on(void);
U8 ReadKeyPress1();
U8 ReadKeyPress2();
U8 ReadKeyPress3();
U8 ReadKeyPress4();

#endif
