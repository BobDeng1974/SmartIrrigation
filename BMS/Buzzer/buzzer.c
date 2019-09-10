#include "lpc17xx_gpio.h"
#include "buzzer.h"
#include "type.h"

#define BUZZER_PORT 	0
#define BUZZER_PIN  	16

void BuzzerInit(void) {
	GPIO_SetDir(BUZZER_PORT, (1 << BUZZER_PIN), 1);
}

void BuzzerOn(void) {
	GPIO_SetValue(BUZZER_PORT, (1 << BUZZER_PIN));
}

void BuzzerOff(void) {
	GPIO_ClearValue(BUZZER_PORT, (1 << BUZZER_PIN));
}

/**********************************************************************************************
 End of File
 *********************************************************************************************/
