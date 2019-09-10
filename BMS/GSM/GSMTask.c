
#include "lpc17xx_wdt.h"
#include "type.h"
#include "GSMTask.h"
#include"string.h"
#include"stdio.h"
#include"stdlib.h"
#include"UART.h"
#include "Utils.h"
#include "lcd.h"

char SendCmdBuffer[256];// __attribute__ ((section(".usb_buffer")));
char RecBuffer[256];// __attribute__ ((section(".usb_buffer")));
MOBILENO Number;

U32 br,bw;


void GSMSend(){
	char BufferTest[512];

	DeleteAllMsg();
	strcpy(BufferTest,"NO ENTRY FOR THIS DATE");
	 SendMSGToMobile(BufferTest);
}

U8 SendMSGToMobile(char BufferTest[]) {
	U8 Check;
//	char Msg[50];
	/*if(MODE == NEWDAYDAILYMSG)
		ReadMOBBINFile();*/
	vTaskDelay(100);
	memset(SendCmdBuffer, 0, sizeof(SendCmdBuffer));
	Check = GSMSendCmdForSelectingTextMode(TEXTMODE);
	if (Check == 0) {
/*		GLCDClearAppArea();
		GLCDPuts("ERROR IN MSG FORMAT", 1);*/
		vTaskDelay(500);
		return 0;
	}
	Check = GSMSendCmdForSelectingMobileNo();
	if (Check == 0) {
/*		GLCDClearAppArea();
		GLCDPuts("ERROR IN SELECTING MOB NO", 1);*/
		vTaskDelay(500);
		return 0;
	}
	Check = SendReqdMsg(BufferTest);
	if (Check == 0) {
/*		GLCDClearAppArea();
		GLCDPuts("ERROR IN SENDING COMMAND", 1);*/
		vTaskDelay(500);
		return 0;
	}
	return 1;
}

U8 GSMATCmd() {
	U8 Check;

	while (1) {
		WDT_Feed();
		sprintf(SendCmdBuffer, "AT\r\n");
		UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
		vTaskDelay(100);
		memset(RecBuffer,0,sizeof(RecBuffer));
		Check = GSMWaitForOk(RecBuffer,10);
		if (Check == 1)
			return 1;
		/*if (Check == 2)
			SendTerminatingChar();*/
	}
	return Check;
}

void SendTerminatingChar()
{
	memset(SendCmdBuffer,0,sizeof(SendCmdBuffer));
	SendCmdBuffer[0] = 0x1A;
	SendCmdBuffer[1] = 0x0A;
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	vTaskDelay(100);
	memset(RecBuffer,0,sizeof(RecBuffer));
	GetGSMReciveData(RecBuffer,1,10);
}

U8 GSMATResetCmd() {
	U8 Check;

	sprintf(SendCmdBuffer, "AT+RESET\r\n");
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	vTaskDelay(100);
	memset(RecBuffer,0,sizeof(RecBuffer));
	Check = GSMWaitForOk(RecBuffer,10);
	return Check;
}

// Response Checking after closing Socket
U8 GSMWaitForCloseOk(char *Ptr,U8 TimeOut) {
	U16 i = 0,Len = 0;

	for (i = 0; i < TimeOut; i++)
	{
		WDT_Feed();
		if (uart3_get_byte_count() >= 2)
		{
			Len = uart3_get_byte_count();
			break;
		}
		vTaskDelay(1);
	}
	if (i == TimeOut)
	{
		return 0;//Data not received as there is too much delay in data receiving
	}
	uart3_get_byte((U8 *)Ptr, Len);
	if (strstr(Ptr, "CLOSE OK") != '\0')
		return 1;
	/*if (strchr(Ptr,'>')!='\0')
		return 2;*/
	return 0;
}
// Response Checking after opening socket
U8 GSMWaitForConnectOk(char *Ptr,U8 TimeOut) {
	U16 i = 0,Len = 0;

	for (i = 0; i < TimeOut; i++)
	{
		WDT_Feed();
		if (uart3_get_byte_count() >= 2)
		{
			Len = uart3_get_byte_count();
			break;
		}
		vTaskDelay(1);
	}
	if (i == TimeOut)
	{
		return 0;//Data not received as there is too much delay in data receiving
	}
	uart3_get_byte((U8 *)Ptr, Len);
	if (strstr(Ptr, "CONNECT OK") != '\0')
		return 1;
	/*if (strchr(Ptr,'>')!='\0')
		return 2;*/
	return 0;
}

U8 GSMWaitForOk(char *Ptr,U8 TimeOut) {
	U16 i = 0,Len = 0;

	for (i = 0; i < TimeOut; i++)
	{
		WDT_Feed();
		if (uart3_get_byte_count() >= 2)
		{
			Len = uart3_get_byte_count();
			break;
		}
		vTaskDelay(1);
	}
	if (i == TimeOut)
	{
		return 0;//Data not received as there is too much delay in data receiving
	}
	uart3_get_byte((U8 *)Ptr, Len);
	if (strstr(Ptr, "OK") != '\0')
		return 1;
	/*if (strchr(Ptr,'>')!='\0')
		return 2;*/
	return 0;
}

U8 GSMATSendSMSCmd() {
	U8 Check;

	sprintf(SendCmdBuffer, "AT+CMGS\r\n");
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	memset(RecBuffer,0,sizeof(RecBuffer));
	Check = GSMWaitForOk(RecBuffer,10);
	return Check;
}

U8 GSMATDeleteSMSCmd(U8 Index) {
	U8 Check;

	sprintf(SendCmdBuffer, "AT+CMGD=%d\r\n", Index);
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	memset(RecBuffer,0,sizeof(RecBuffer));
	Check = GSMWaitForOk(RecBuffer,10);
	return Check;
}

U8 DeleteAllSmsFromMemory() {
	U8 Check;

	sprintf(SendCmdBuffer, "AT+QMGDA=\"DEL ALL\"\r\n");
	UART3_Send_Buffer((U8 *)SendCmdBuffer, strlen(SendCmdBuffer));
	vTaskDelay(100);
	memset(RecBuffer,0,sizeof(RecBuffer));
	Check = GSMWaitForOk(RecBuffer,100);
	return Check;
}

U8 DeleteAllMsg(){
	U8 Check;

	Check = GSMSendCmdForSelectingTextMode(TEXTMODE);
	if (Check == 0) {
		return 0;
	}
	sprintf(SendCmdBuffer, "AT+QMGDA=\"DEL ALL\"\r\n");
	UART3_Send_Buffer((U8 *)SendCmdBuffer, strlen(SendCmdBuffer));
	vTaskDelay(100);
	memset(RecBuffer,0,sizeof(RecBuffer));
	Check = GSMWaitForOk(RecBuffer,100);
	return Check;
}

U8 GSMSendCmdForSelectingTextMode(U8 Index) {
	U8 Check;

	memset(SendCmdBuffer,0,sizeof(SendCmdBuffer));
	sprintf(SendCmdBuffer, "AT+CMGF=%d\r\n", Index); //select msg format to text mode
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	vTaskDelay(100);
	memset(RecBuffer,0,sizeof(RecBuffer));
	Check = GSMWaitForOk(RecBuffer,100);
	return Check;
}

U8 GSMSendCmdForSelectingMobileNo() {

	memset(SendCmdBuffer,0,sizeof(SendCmdBuffer));
	sprintf(SendCmdBuffer, "AT+CMGW=\"+919916416500\"\r\n");
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	memset(RecBuffer,0,sizeof(RecBuffer));
	vTaskDelay(100);
	GetGSMReciveData(RecBuffer,1,100);
	if (strstr(RecBuffer, ">") != '\0')
		return 1;
	if (strstr(RecBuffer,"CMS ERROR: 322")!='\0' || strstr(RecBuffer,"CMS ERROR: 321")!='\0')
	{
		DeleteAllSmsFromMemory();
		return 1;
	}
	return 0;
}

void GetGSMReciveData(char *Ptr,U16 Len,U8 TimeOut) {
	U8 i = 0;

	for (i = 0; i < TimeOut; i++)
	{
		WDT_Feed();
		if (uart3_get_byte_count() >= Len)
		{
			Len = uart3_get_byte_count();
			break;
		}
		vTaskDelay(1);
	}
	if (i == TimeOut)
	{
		return;//Data not received as there is too much delay in data receiving
	}
	uart3_get_byte((U8 *)Ptr, Len);
}

U8 SendReqdMsg(char BufferTest[]) {
	U8 Index;
	char *Temp;
	char Buffer[5];
//	U16 Len = 0;

	memset(Buffer,0,sizeof(Buffer));
	memset(SendCmdBuffer,0,sizeof(SendCmdBuffer));
	memset(RecBuffer,0,sizeof(RecBuffer));
	sprintf(SendCmdBuffer, "%s\r\n", BufferTest);
//	SendCmdBuffer[strlen(SendCmdBuffer)] = 0x1A;
	//SendCmdBuffer[strlen(SendCmdBuffer)+1] = 0x0a;
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	vTaskDelay(1000);
	GetGSMReciveData(RecBuffer,10,100);
	memset(SendCmdBuffer,0,sizeof(SendCmdBuffer));
	memset(RecBuffer,0,sizeof(RecBuffer));
	GetGSMReciveData(RecBuffer,90,100);
	memset(SendCmdBuffer,0,sizeof(SendCmdBuffer));
	memset(RecBuffer,0,sizeof(RecBuffer));
	Buffer[0] = 0x1a;
	Buffer[1] = 0x0d;
	sprintf(SendCmdBuffer, "%s\r\n", Buffer);
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	vTaskDelay(500);
	GetGSMReciveData(RecBuffer,5,100);
	if ((Temp = strstr(RecBuffer, "+CMGW: ")) != '\0') {
		Temp += strlen("+CMGW: ");
		Index = atoi(Temp);
	}
	else if (strstr(RecBuffer,"CMS ERROR: 322")!='\0')
	{
		DeleteAllSmsFromMemory();
		return 0;
	}
	else
		return 0;
	return SendReqdIndexMsg(Index);
}

U8 SendReqdIndexMsg(U8 Index) {
	U8 Check;

	sprintf(SendCmdBuffer, "AT+CMSS=%d\r\n", Index);
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	vTaskDelay(5000);
	Check = GSMWaitForOk(RecBuffer,10);
	return Check;
}

U8 GetUnreadRecdMSGFromMobile()
{
	char Msg[30];

	sprintf(SendCmdBuffer, "AT+CMGL=\"REC UNREAD\"\r\n");//Recd Unread msg
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	vTaskDelay(50);
	GetGSMReciveData(RecBuffer,25,25);
	GetLastMSGContentFromBuffer(Msg);
	return 1;
}

U8 GetLastMSGContentFromBuffer(char *Msg)
{
	char *Index;

	if ((Index = strrchr(RecBuffer,'\"'))!='\0')
	{
		Index++;
		strncpy(Msg, Index,29);
		Msg[29]='\0';
		return 1;
	}
	return 0;
}

U8 GetIndexRecdMSGFromMobile(U8 Index)
{
	char Msg[30];

	sprintf(SendCmdBuffer, "AT+CMGR=%d\r\n", Index);//Receive Particular index msg from msg memory storage
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	GetGSMReciveData(RecBuffer,10,50);
	GetLastMSGContentFromBuffer(Msg);
	if (MSGContentMatches(Msg))
	{
		if (GetReceivedMobileNo())
		{
			SaveMobileNoToFile();
			return 1;
		}
		else
			return 0;
	}
	return 0;
}

U8 MSGContentMatches(char *Msg)
{
	if (strstr(Msg,"Change Number")!= '\0')
		return 1;
	return 0;
}

U8 GetReceivedMobileNo()
{
	char *Ptr;

	if ((Ptr = strstr(RecBuffer,"\", \"")) != '\0')
	{
		Ptr += 4;
		strncpy(Number.MobileNo,Ptr,13);
		return 1;
	}
	return 0;
}

void SubscriberNumber(){

	memset(SendCmdBuffer,0,sizeof(SendCmdBuffer));
	sprintf(SendCmdBuffer, "AT+CNUM\r\n");
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	vTaskDelay(1000);
	memset(RecBuffer,0,sizeof(RecBuffer));
	GetGSMReciveData(RecBuffer,10,10);
}

U8 ReadRecievedMsg(U8 Index){

	char *Temp;
	char CompareMsg[50];

	/*check = ReadMOBBINFile();
	if(check == 0)
		return 0;*/
	GSMSendCmdForSelectingTextMode(TEXTMODE);
	memset(SendCmdBuffer,0,sizeof(SendCmdBuffer));
	sprintf(SendCmdBuffer, "AT+CMGR=%d\r\n",Index);
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	sprintf((char *)CompareMsg, "%s", Number.MobileNo);
	memset(RecBuffer,0,sizeof(RecBuffer));
	vTaskDelay(300);
	GetGSMReciveData(RecBuffer,90,250);
	if(strstr(RecBuffer,CompareMsg) != '\0')
	{
		sprintf((char *)CompareMsg, "SEND SUMMARY REPORT");
		if(strstr(RecBuffer, CompareMsg) != '\0')
		{
	//		DeleteAllMsg();
			GSMSend();
			return 1;
		}
	}
	sprintf((char *)CompareMsg, "SEND SUMMARY REPORT %s", Number.Password);
	if(strstr(RecBuffer, CompareMsg) != '\0')
	{
		if ((Temp = strstr(RecBuffer, "+91")) != '\0') {
			memset(Number.MobileNo,0,sizeof(Number.MobileNo));
			strncpy(Number.MobileNo,Temp,13);
//			DeleteAllMsg();
			GSMSend();
		}
	}
	return 1;
}

void ProcessBTCommands(void) {
	U8 Index = 0;
/*	U8 Check = 0;
	U8 CompareMsg[50];
	U16 Len = 0;*/

	m66_commands_t m66_commands_s;
	char *Temp;
	U8 ConnectedFlag = 0,PairingFlag = 0;

	memset(&m66_commands_s,0,sizeof(m66_commands_t));
	memset(RecBuffer,0,sizeof(RecBuffer));
	vTaskDelay(1000);
	GetGSMReciveData(RecBuffer,5,100);
	if (strstr(RecBuffer, "pair") != '\0')
		m66_commands_s.Cmd = pair_bt;
	else if (strstr(RecBuffer, "conn") != '\0')
		m66_commands_s.Cmd = connect_bt;
	else if ((Temp = strstr(RecBuffer, "+CMTI: ")) != '\0')
		m66_commands_s.Cmd = msg_received;
	else if (strstr(RecBuffer, "RING") != '\0')
		m66_commands_s.Cmd = ring;
/*	else{
		Len = uart3_get_byte_count();
		if(Len > 0)
			ClearDataInBuffer(Len);
	}*/
	switch (m66_commands_s.Cmd) {
		case pair_bt :
			sprintf(SendCmdBuffer, "AT+QBTPAIRCNF=1\r\n");
			UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
			vTaskDelay(2000);
			GetGSMReciveData(RecBuffer,10,10);
			if ((Temp = strstr(RecBuffer, "+QBTPAIRCNF: 1,")) != '\0')
			{
				Temp += strlen("+QBTPAIRCNF: 1,");
				Index = atoi(Temp);
				if(Index == 1)
					Index += 1;
				else
					Index -= 1;
				BTDeviceUnpair(Index);
			}
			PairingFlag = 1;
			ConnectedFlag = 0;
			break;
		case connect_bt :
			sprintf(SendCmdBuffer, "AT+QBTACPT=1,2\r\n");
			UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
			ConnectedFlag = 1;
			break;
/*		case msg_received :
			Temp += strlen("+CMTI: \"SM\",");
			Index = atoi(Temp);
			ReadRecievedMsg(Index);
			break;
		case ring :
			vTaskDelay(100);
			Check = ReadMOBBINFile();
			if(Check == 0)
				return;
			sprintf(SendCmdBuffer, "AT+CLIP=1\r\n");
			UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
			vTaskDelay(500);
			GetGSMReciveData(RecBuffer,25,100);
			sprintf((char *)CompareMsg, "%s", Number.MobileNo);
			if(strstr(RecBuffer,CompareMsg) != '\0')
			{
				vTaskDelay(100);
				sprintf(SendCmdBuffer, "ATH0\r\n");
				UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
				vTaskDelay(500);
				Check = GSMWaitForOk(RecBuffer,10);
				if(Check == 1)
					GSMSend();
			}
			break;*/
		default:
				break;
	}
	if(PairingFlag)
	{
		lcd_clear();
		lcd_putstring("PAIRING SUCCESSFUL", 0);
	}
	if(ConnectedFlag)
	{
		lcd_clear();
		lcd_putstring("CONNECTION SUCCESSFUL", 1);
	}
	return;
}

U8 InitilizeBluetooth(){
	U8 check = 0, i = 0;

	memset(SendCmdBuffer,0,sizeof(SendCmdBuffer));
	sprintf(SendCmdBuffer, "AT+QBTPWR?\r\n");
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	memset(RecBuffer,0,sizeof(RecBuffer));
	vTaskDelay(100);
	GetGSMReciveData(RecBuffer,5,10);
	if (strstr(RecBuffer, "+QBTPWR: 1") != '\0')
	{
		check = 1;
	}else
	{
		vTaskDelay(100);
		GetGSMReciveData(RecBuffer,5,10);
		if (strstr(RecBuffer, "+QBTPWR: 1") != '\0')
		{
			check = 1;
		}
		else
		{
			for(i=0;i<MAXRETRYCONNECTION;i++)
			{
				WDT_Feed();
				memset(SendCmdBuffer,0,sizeof(SendCmdBuffer));
				memset(RecBuffer,0,sizeof(RecBuffer));
				sprintf(SendCmdBuffer, "AT+QBTPWR=1\r\n");
				UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
				vTaskDelay(1000);
				check = GSMWaitForOk(RecBuffer,100);
				if(check)
				  break;
			}
			if(i == MAXRETRYCONNECTION)
				return 0;
		}
	}
	if(check)
	  return 1;
	return 0;
}

/*void BTDeviceUnpair(){
	U8 i= 0;
	for(i=1;i<=10;i++)
	{
		memset(SendCmdBuffer,0,sizeof(SendCmdBuffer));
		memset(RecBuffer,0,sizeof(RecBuffer));
		vTaskDelay(1000);
		sprintf(SendCmdBuffer, "AT+QBTUNPAIR=%d\r\n",i);
		UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
		vTaskDelay(100);
		GetGSMReciveData(RecBuffer,10);//+CME ERROR: 3518
		if (strstr(RecBuffer, "OK") != '\0')
			continue;
		if (strstr(RecBuffer, "+CME ERROR: 3518") != '\0')
			break;
		else if (strstr(RecBuffer, "+CME ERROR: 8016") != '\0')
			i--;
	}
	return;
}*/

void BTDeviceUnpair(U8 ConnectedID){
	U8 i= 0;

	vTaskDelay(1000);
	for(i =0;i<5;i++)
	{
		WDT_Feed();
		memset(SendCmdBuffer,0,sizeof(SendCmdBuffer));
		memset(RecBuffer,0,sizeof(RecBuffer));
		sprintf(SendCmdBuffer, "AT+QBTUNPAIR=%d\r\n",ConnectedID);
		UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
		vTaskDelay(100);
		GetGSMReciveData(RecBuffer,10,10);//+CME ERROR: 3518
		if ((strstr(RecBuffer, "OK") != '\0') || (strstr(RecBuffer, "+CME ERROR: 3518") != '\0'))
			break;
	}
/*	else if (strstr(RecBuffer, "+CME ERROR: 8016") != '\0')     // NOT REQUIRED
		continue;*/
}

void SendATCommand(){
	U8 i=0,check;
	for(i=0;i<MAXRETRYCONNECTION;i++)
	{
		memset(SendCmdBuffer,0,sizeof(SendCmdBuffer));
		memset(RecBuffer,0,sizeof(RecBuffer));
		sprintf(SendCmdBuffer, "AT\r\n");
		UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
		vTaskDelay(5000);
		check = GSMWaitForOk(RecBuffer,100);
		if(check == 0)
			break;
	}
}

/*
void ClearDataInBuffer(U16 Len){
	GetGSMReciveData(RecBuffer,Len,10);
}
*/

/* ------------------------------------------------------------------------------------------*/



//Methods required for TCP/IP communication with server

//Setting up M66 in Transparent Mode
U8 TCPSetUpTransparentClientMode() {
	U8 Check;

	vTaskDelay(100);
	Check = TCPSetContextCmd();
	if (Check == 0) {
		vTaskDelay(500);
		return 0;
	}
	Check = TCPSetAPNCmd();
	if (Check == 0) {
		vTaskDelay(500);
		return 0;
	}
	Check = TCPDisableMUXCmd();
	if (Check == 0) {
		vTaskDelay(500);
		return 0;
	}
	Check = TCPTransperantModeCmd();
	if (Check == 0) {
		vTaskDelay(500);
		return 0;
	}
	Check = TCPConfigureContextCmd();
	if (Check == 0) {
		vTaskDelay(500);
		return 0;
	}
	Check = TCPUseIPforTCPCmd();
	if (Check == 0) {
		vTaskDelay(500);
		return 0;
	}
	return 1;
}

U8 TCPSetContextCmd() {
	U8 Check;

	sprintf(SendCmdBuffer, "AT+QIFGCNT=0\r\n");
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	vTaskDelay(100);
	memset(RecBuffer,0,sizeof(RecBuffer));
	Check = GSMWaitForOk(RecBuffer,10);
	return Check;
}
U8 TCPSetAPNCmd() {
	U8 Check;

	sprintf(SendCmdBuffer, "AT+QICSGP=1\r\n");
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	vTaskDelay(100);
	memset(RecBuffer,0,sizeof(RecBuffer));
	Check = GSMWaitForOk(RecBuffer,10);
	return Check;
}
U8 TCPDisableMUXCmd() {
	U8 Check;

	sprintf(SendCmdBuffer, "AT+QIMUX=0\r\n");
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	vTaskDelay(100);
	memset(RecBuffer,0,sizeof(RecBuffer));
	Check = GSMWaitForOk(RecBuffer,10);
	return Check;
}
U8 TCPTransperantModeCmd() {
	U8 Check;

	sprintf(SendCmdBuffer, "AT+QIMODE=1\r\n");
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	vTaskDelay(100);
	memset(RecBuffer,0,sizeof(RecBuffer));
	Check = GSMWaitForOk(RecBuffer,10);
	return Check;
}
U8 TCPConfigureContextCmd() {
	U8 Check;

	sprintf(SendCmdBuffer, "AT+QITCGF=3,2,512,1\r\n");
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	vTaskDelay(100);
	memset(RecBuffer,0,sizeof(RecBuffer));
	Check = GSMWaitForOk(RecBuffer,10);
	return Check;
}
U8 TCPUseIPforTCPCmd() {
	U8 Check;

	sprintf(SendCmdBuffer, "AT+QIDNSIP=0\r\n");
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	vTaskDelay(100);
	memset(RecBuffer,0,sizeof(RecBuffer));
	Check = GSMWaitForOk(RecBuffer,10);
	return Check;
}

//Opening TCP connection and Sending data
U8 TCPSendData() {
	U8 Check;

	vTaskDelay(100);
		Check = TCPSetContextCmd();

		Check = TCPCloseSocketCmd();
		if (Check == 0) {
			vTaskDelay(500);
			return 0;
		}
		Check = TCPOpenSocketCmd();
		if (Check == 0) {
			vTaskDelay(500);
			return 0;
		}
		Check = TCPEnableDataEchoCmd();
		if (Check == 0) {
			vTaskDelay(500);
			return 0;
		}
		Check = TCPSendDataToServer();
		if (Check == 0) {
			vTaskDelay(500);
			return 0;
		}
	return 1;
}

//Close Socket
U8 TCPCloseSocketCmd() {
	U8 Check;

	sprintf(SendCmdBuffer, "AT+QICLOSE\r\n");
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	vTaskDelay(100);
	memset(RecBuffer,0,sizeof(RecBuffer));
	Check = GSMWaitForCloseOk(RecBuffer,10);
	return Check;
}

//Open Socket
U8 TCPOpenSocketCmd() {
	U8 Check;

	sprintf(SendCmdBuffer, "AT+QIOPEN=\"TCP\",\"166.62.27.185\",\"22\"\r\n");
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	vTaskDelay(100);
	memset(RecBuffer,0,sizeof(RecBuffer));
	Check = GSMWaitForConnectOk(RecBuffer,10);
	return Check;
}

//Enable data echo
U8 TCPEnableDataEchoCmd() {
	U8 Check;

	sprintf(SendCmdBuffer, "AT+QISDE=0\r\n");
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	vTaskDelay(100);
	memset(RecBuffer,0,sizeof(RecBuffer));
	Check = GSMWaitForOk(RecBuffer,10);
	return Check;
}

//Send data
U8 TCPSendDataToServer() {
	U8 Check;

	memset(SendCmdBuffer,0,sizeof(SendCmdBuffer));
	sprintf(SendCmdBuffer, "AT+QISEND\r\n");
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	memset(RecBuffer,0,sizeof(RecBuffer));
	vTaskDelay(100);
	GetGSMReciveData(RecBuffer,1,100);
	if (strstr(RecBuffer, ">") != '\0')
	{
		memset(SendCmdBuffer,0,sizeof(SendCmdBuffer));
		sprintf(SendCmdBuffer, "Connection is successful");
		UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
		TCPTerminatingChar();
		Check = GSMWaitForOk(RecBuffer,10);
		if(Check == 1)
		{
			lcd_putstring("Data Sending Success",LINE2);
			return 1;
		}
		lcd_putstring("Data Sending Fail",LINE2);
	}
	return 0;
}
void TCPTerminatingChar(){
	memset(SendCmdBuffer,0,sizeof(SendCmdBuffer));
	SendCmdBuffer[0] = 0x1A;
	UART3_Send_Buffer((U8 *)SendCmdBuffer,strlen(SendCmdBuffer));
	vTaskDelay(100);
}




