#ifndef _GSMTASK_H
#define _GSMTASK_H

#define WEAK __attribute__ ((weak))

//portTASK_FUNCTION(vGSMTask, pvParameters);

void GSMSend();
U8 GSMWaitForOk(char *Ptr,U8 TimeOut);
U8 GSMWaitForCloseOk(char *Ptr,U8 TimeOut);

U8 SendReqdIndexMsg(U8 Index);
U8 SendReqdMsg(char *Msg);
U8 GSMSendCmdForSelectingMobileNo();
U8 GSMSendCmdForSelectingTextMode(U8 Index);
U8 GSMATReadSMSCmd(U8 Index);
U8 GSMATDeleteSMSCmd(U8 Index);
void GetGSMReciveData(char *Ptr,U16 Len,U8 TimeOut);
U8 GSMATSendSMSCmd();
U8 GSMATResetCmd();
U8 GSMATCmd();
U8 SendMSGToMobile(char BufferTest[]);
U8 GetIndexRecdMSGFromMobile(U8 Index);
U8 MSGContentMatches();
U8 SaveMobileNoToFile();
U8 GetLastMSGContentFromBuffer(char *Msg);
U8 GetReceivedMobileNo();
U8 DeleteAllSmsFromMemory();
void ProcessBTCommands(void);
U8 InitilizeBluetooth();
void BTDeviceUnpair(U8 ConnectedID);
void SubscriberNumber();
U8 ReadMOBBINFile();
U8 DeleteAllMsg();
void ClearDataInBuffer(U16 Len);

U8 TCPSetContextCmd();
U8 TCPSetAPNCmd();
U8 TCPDisableMUXCmd();
U8 TCPTransperantModeCmd();
U8 TCPConfigureContextCmd();
U8 TCPUseIPforTCPCmd();
U8 TCPSetUpTransparentClientMode();

U8 TCPCloseSocketCmd();
U8 TCPOpenSocketCmd();
U8 TCPEnableDataEchoCmd();
U8 TCPSendDataToServer();
void TCPTerminatingChar();
U8 TCPSendData();


#define MAXRETRYCONNECTION  5
#define TEXTMODE	1
#define MOBILENOBINFILE	"1://HB//GSM.bin"
//#define SENDMSG          1
//#define NEWDAYDAILYMSG	 0
typedef struct
{
	char MobileNo[15];
	char Password[15];
}MOBILENO;

typedef enum {
	pair_bt						= 0x01,
	connect_bt					= 0x02,
	msg_received				= 0x03,
	ring						= 0x04,
}bluetooth_or_gsm_commands_t;

typedef struct
{
	bluetooth_or_gsm_commands_t Cmd;
}__attribute__((__packed__)) m66_commands_t;


#endif
