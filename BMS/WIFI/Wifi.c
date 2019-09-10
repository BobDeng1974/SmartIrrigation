
#include "LPC17xx.h"
#include "type.h"
#include "uart.h"
#include"string.h"
#include"Wifi.h"
#include "Utils.h"

char Buffer1[50];
char BufRecd[100];


U8 ResetWifiModem()
{
	U8 Len, ch, i;

	memset(BufRecd,0,sizeof(BufRecd));
	strcpy(Buffer1,"AT+RST\r\n");
	Len = strlen(Buffer1);
	UART3_Send_Buffer((U8 *)Buffer1,Len);
	vTaskDelay(500);
	for (i = 0; i < 75; i++) {
		vTaskDelay(1);
		ch = 0;
		uart3_get_byte(&ch,1);
		BufRecd[i] = ch;
	}
	BufRecd[74] = '\0';
	if (strstr(BufRecd, "OK\r\n") != '\0') {
		//if (strstr(BufRecd,"ready")!= '\0')
			return 1;
	}
	return 0;
}

/*U8 SendATCommand()
{
	U8 Len, ch, i;

	memset(BufRecd,0,sizeof(BufRecd));
	strcpy(Buffer1,"AT+RST\r\n");
	Len = strlen(Buffer1);
	UART3_Send_Buffer(Buffer1,Len);
	vTaskDelay(500);
	for (i = 0; i < 10; i++) {
		vTaskDelay(1);
		ch = 0;
		uart3_get_byte(&ch,1);
		BufRecd[i] = ch;
	}
	BufRecd[10] = '\0';
	if (strstr(BufRecd, "OK\r\n") != '\0') {
		//if (strstr(BufRecd,"ready")!= '\0')
			return 1;
	}
	return 0;
}*/


U8 SetWifiMode() {
	U8 Len, ch, i;

	memset(BufRecd,0,sizeof(BufRecd));
	strcpy(Buffer1, "AT+CWMODE=3\r\n");
	Len = strlen(Buffer1);
	UART3_Send_Buffer((U8 *)Buffer1, Len);
	vTaskDelay(500);
	for (i = 0; i < 50; i++) {
		vTaskDelay(1);
		ch = 0;
		uart3_get_byte(&ch,1);
		BufRecd[i] = ch;
	}
	BufRecd[49] = '\0';
	if (strstr(BufRecd, "OK\r\n") != '\0' || strstr(BufRecd,"no change")!= '\0') {
		return 1;
	}
	return 0;
}

U8 ConnectWifiSSIDAndPassword(char *SSID, char *PSWD) {
	U8 Len, ch, i;

	memset(BufRecd,0,sizeof(BufRecd));
	strcpy(Buffer1, "AT+CWJAP=");
	strcat(Buffer1,"\"");
	strcat(Buffer1,SSID);
	strcat(Buffer1,"\",");
	strcat(Buffer1,"\"");
	strcat(Buffer1,PSWD);
	strcat(Buffer1,"\"\r\n");
	Len = strlen(Buffer1);
	UART3_Send_Buffer((U8 *)Buffer1, Len);
	vTaskDelay(5000);
	for (i = 0; i < 50; i++) {
		vTaskDelay(1);
		ch = 0;
		uart3_get_byte(&ch,1);
		BufRecd[i] = ch;
	}
	BufRecd[49] = '\0';
	if (strstr(BufRecd, "OK\r\n") != '\0') {
		return 1;
	}
	return 0;
}

U8 EnableMultipleConnection() {
	U8 Len, ch, i;

	memset(BufRecd,0,sizeof(BufRecd));
	strcpy(Buffer1, "AT+CIPMUX=1\r\n");
	Len = strlen(Buffer1);
	UART3_Send_Buffer((U8 *)Buffer1, Len);
	vTaskDelay(1000);
	for (i = 0; i < 50; i++) {
		vTaskDelay(1);
		ch = 0;
		uart3_get_byte(&ch,1);
		BufRecd[i] = ch;
	}
	BufRecd[49] = '\0';
	if (strstr(BufRecd, "OK\r\n") != '\0') {
		return 1;
	}
	return 0;
}

U8 SetPortAndConnectAsServer() {
	U8 Len, ch, i;

	memset(BufRecd,0,sizeof(BufRecd));
	strcpy(Buffer1, "AT+CIPSERVER=1,100\r\n");//1 is for enable to work as server
	Len = strlen(Buffer1);
	UART3_Send_Buffer((U8 *)Buffer1, Len);
	vTaskDelay(500);
	for (i = 0; i < 50; i++) {
		vTaskDelay(1);
		ch = 0;
		uart3_get_byte(&ch,1);
		BufRecd[i] = ch;
	}
	BufRecd[49] = '\0';
	if (strstr(BufRecd, "OK\r\n") != '\0') {
		return 1;
	}
	return 0;
}

U8 CheckIPAddress() {
	U8 Len, ch, i;

	memset(BufRecd,0,sizeof(BufRecd));
	strcpy(Buffer1, "AT+CIFSR\r\n");//1 is for enable to work as server
	Len = strlen(Buffer1);
	UART3_Send_Buffer((U8 *)Buffer1, Len);
	vTaskDelay(500);
	for (i = 0; i < 80; i++) {
		vTaskDelay(1);
		ch = 0;
		uart3_get_byte(&ch,1);
		BufRecd[i] = ch;
	}
	BufRecd[80] = '\0';
	if (strstr(BufRecd, "OK\r\n") != '\0') {
		return 1;
	}
	return 0;
}

U8 ConfigureWifiInit(char *SSID, char *PSWD)
{
	U8 check = 0;
	ResetWifiModem();
	SetWifiMode();
	ConnectWifiSSIDAndPassword(SSID,PSWD);
    EnableMultipleConnection();
	check = SetPortAndConnectAsServer();
	CheckIPAddress();
	return check;

}
