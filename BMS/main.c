#include "system_LPC17xx.h"
#include <lpc17xx.h>
#include "lcd.h"
#include "buzzer.h"
#include "GSMTask.h"
#include "Uart.h"
#include "WIFI.h"
#include "adc.h"
#include "string.h"
#include <stdio.h>
#include "utils.h"
#include "Keypad.h"
#include "extint.h"

uint32_t checkvalue;

uint32_t water;

void waterFlow(){
	water++;
}
void EINT0_IRQHandler()
{
	waterFlow();
	GPIO_ClearInt(0,23);
}
void EINT1_IRQHandler()
{
	waterFlow();
	GPIO_ClearInt(0,23);
}
void EINT2_IRQHandler()
{
	waterFlow();
	GPIO_ClearInt(0,23);
}
void EINT3_IRQHandler()
{
	waterFlow();
	lcd_clear_line(LINE2);
	vTaskDelay(1000);
	lcd_putstring("interrupted",LINE2);
	vTaskDelay(2000);
	GPIO_ClearInt(0,0xffffffff);
}
int main(void)
{
	U8 Check = 0,Flag = 0;
	uint32_t ADCValue = 0,Len = 0;
	char Buffer[50],RecBuff[50];
	SystemCoreClockUpdate();
	BuzzerInit();
	water=0;
//	BuzzerOn();
	lcd_init();
	ADCInit(1);
	EnableCursor();
	lcd_clear();
	lcd_putstring("NGX BMS-IOT Kit", LINE1);
	BuzzerOff();
#ifdef  ESP8266_Module
	ConfigureUart3();
	ConfigureWifiInit("NGXPRO2","NGXACT1234");
#endif
#ifdef M66_Module
	ConfigureUart3();
	InitilizeBluetooth();
	GSMSend();
	while(1)
	{
		ProcessBTCommands();
	}
#endif
#ifdef  ADC_OPERATIONS
	while(1){
		ADCValue = ADC0Read(1);
		ADCValue = ADC0Read(1);
		ADCValue = ADC0Read(1);
		ADCValue = ADC0Read(1);
		memset(Buffer,0,sizeof(Buffer));
		sprintf(Buffer,"CHANNEL1- %lu",ADCValue);
		lcd_clear();
		lcd_putstring(Buffer, LINE2);
		vTaskDelay(2000);
	}
#endif
	ConfigureUart1();
	memset(Buffer,0,sizeof(Buffer));
	strcpy(Buffer,"AT\n");
	UART1_Send_Buffer((U8 *)Buffer,strlen(Buffer));
	vTaskDelay(500);
	while(1){
		if((ReadKeyPress1())){
			Flag = 1;
		}else if((ReadKeyPress2())){
			Flag = 2;
		}else if(ReadKeyPress3()){
			Flag = 3;
		}else if(ReadKeyPress4()){
			Flag = 4;
		}else {
			Flag = 0;
		}
		switch(Flag){
		case 1 :/*ConfigureUart3();
				Check = ConfigureWifiInit("NGXPRO2","NGXACT1234");
				if(Check)
				{
					lcd_clear();
					lcd_putstring("WIFI CONN SUCCESSFUL", LINE1);
				}else{
					lcd_clear();
					lcd_putstring("WIFI CONN FAILURE", LINE1);
				}*/

				/****GPIO_IntCmd(0,23,0);
				NVIC_EnableIRQ(EINT0_IRQn);
				while(1){
					sprintf(Buffer,"%d",water);
					lcd_putstring(Buffer,LINE2);
					vTaskDelay(2000);
				}*****/
			  /* SystemClockUpdate() updates the SystemFrequency variable */


			 /* initialize GPIO pins as external interrupts */
				IOSetDir(0, 24, 1);
				lcd_clear();
				lcd_putstring("WIFI CONN FAILURE", LINE1);
				vTaskDelay(3000);
				IOSetPull(0, 24, 1);
				lcd_clear();
				lcd_putstring("interrupt pull", LINE1);
				vTaskDelay(3000);
				IOSetInterrupt(0, 24, 1);
				lcd_clear();
			    lcd_putstring("Interrupt set", LINE1);
			    vTaskDelay(3000);



               for(int i=0; i<7;i++);
               sprintf(Buffer,"%d",water);
               lcd_putstring(Buffer,LINE1);
               vTaskDelay(3000);





				break;
		case 2 :ConfigureUart3();
				InitilizeBluetooth();
				GSMSend();
				while(1)
				{
					ProcessBTCommands();
				}
				break;
		case 3 : ConfigureUart3();
				InitilizeBluetooth();
				TCPSetUpTransparentClientMode();
				vTaskDelay(1000);
				TCPSendData();
				while(1)
				{
					ProcessBTCommands();
				}
				break;
		case 4 :
			while(1){
				Len = uart1_get_byte_count();
				if(Len > 0)
				{
					vTaskDelay(1000);
					Len = uart1_get_byte_count();
					memset(RecBuff,0,sizeof(RecBuff));
					uart1_get_byte((U8 *)RecBuff,Len);
					memset(Buffer,0,sizeof(Buffer));
					strcpy(Buffer,RecBuff);
					vTaskDelay(1000);
					UART1_Send_Buffer((U8 *)Buffer,strlen(Buffer));
				}
			}
			break;
		default :
			break;
		}
	}
}
