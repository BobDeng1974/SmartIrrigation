#include"lpc17xx_uart.h"
#include"lpc17xx_pinsel.h"
#include"string.h"
#include "circular_buf.h"
#include"UART.h"
#include "Utils.h"

#define UART0PORT 0
#define UART0PINTX 2
#define UART0PINRX 3

#define UART3PORT 4
#define UART3PINTX 28
#define UART3PINRX 29

#define UART1PORT 2
#define UART1PINTX 0
#define UART1PINRX 1

CIR_BUFFER UART0CirBuf,UART3CirBuf,UART1CirBuf;
U8 Uart0BufRx[100];
U8 Uart3BufRx[100];
U8 Uart1BufRx[100];
LPC_UART_TypeDef *UART0 = (LPC_UART_TypeDef *)LPC_UART0;
LPC_UART_TypeDef *UART3 = (LPC_UART_TypeDef *)LPC_UART3;
LPC_UART_TypeDef *UART1 = (LPC_UART_TypeDef *)LPC_UART1;

void ConfigureUart0Pins()
{
	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 2;
	PinCfg.Pinnum = UART0PINTX;
	PinCfg.Portnum = UART0PORT;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = UART0PINRX;
	PINSEL_ConfigPin(&PinCfg);
}
void ConfigureUart3Pins()
{
	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 3;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = UART3PINTX;
	PinCfg.Portnum =UART3PORT;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = UART3PINRX;
	PINSEL_ConfigPin(&PinCfg);
}

void ConfigureUart1Pins()
{
	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 2;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = UART1PINTX;
	PinCfg.Portnum =UART1PORT;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = UART1PINRX;
	PINSEL_ConfigPin(&PinCfg);
}

void ConfigureUart0()
{

			        UART_CFG_Type UARTConfigStruct;
			        CirBufInit(&UART0CirBuf,sizeof(Uart0BufRx),Uart0BufRx);
			        ConfigureUart0Pins();
			        UART_ConfigStructInit(&UARTConfigStruct);
    		        UARTConfigStruct.Baud_rate = 9600;
			        UART_Init(UART0, &UARTConfigStruct);
			        UART_TxCmd(UART0, ENABLE);
			        UART_IntConfig(UART0, UART_INTCFG_RBR, ENABLE);
			        NVIC_EnableIRQ(UART0_IRQn);
			        NVIC_SetPriority (UART0_IRQn, 8);			/* highest priority */
			    //    UART_Send(UART0, (uint8_t *)"UART0 \r\n", sizeof("UART0 \r\n"), BLOCKING);

}

void ConfigureUart3()
{

			        UART_CFG_Type UARTConfigStruct;
			        CirBufInit(&UART3CirBuf,sizeof(Uart3BufRx),Uart3BufRx);
			        ConfigureUart3Pins();
			        UART_ConfigStructInit(&UARTConfigStruct);
    		        UARTConfigStruct.Baud_rate = 115200;
			        UART_Init(UART3, &UARTConfigStruct);
			        UART_TxCmd(UART3, ENABLE);
			        UART_IntConfig(UART3, UART_INTCFG_RBR, ENABLE);
			        NVIC_EnableIRQ(UART3_IRQn);
			        NVIC_SetPriority (UART3_IRQn, 8);
			     ///   UART_Send(UART3, (uint8_t *)"UART3 \r\n", sizeof("UART3 \r\n"), BLOCKING);

}

void ConfigureUart1()
{

			        UART_CFG_Type UARTConfigStruct;
			        CirBufInit(&UART1CirBuf,sizeof(Uart1BufRx),Uart1BufRx);
			        ConfigureUart1Pins();
			        UART_ConfigStructInit(&UARTConfigStruct);
    		        UARTConfigStruct.Baud_rate = 9600;
			        UART_Init(UART1, &UARTConfigStruct);
			        UART_TxCmd(UART1, ENABLE);
			        UART_IntConfig(UART1, UART_INTCFG_RBR, ENABLE);
			        NVIC_EnableIRQ(UART1_IRQn);
			        NVIC_SetPriority (UART1_IRQn, 8);
			     ///   UART_Send(UART3, (uint8_t *)"UART3 \r\n", sizeof("UART3 \r\n"), BLOCKING);

}
void ConfigureUarts()
{
	ConfigureUart0();
	ConfigureUart1();
	ConfigureUart3();
}
void InitUARTs()
{
	ConfigureUarts();
}

U8 GsmReadUart0(U8* Data, U8 Len, U32 TimeOut) {
	S32 ReturnLen = 0;
	S32 i;

	for (i = 0; i < TimeOut; i++) {
		ReturnLen = getCirBufSize(&UART0CirBuf);
		if (ReturnLen == Len) {
			return popFromCirBuf(&UART0CirBuf, (uint8_t *) Data, ReturnLen);
		}
		vTaskDelay(1);
	}
	if (ReturnLen > 0) {
		return popFromCirBuf(&UART0CirBuf, (uint8_t *) Data, ReturnLen);
	} else
		return 0;
}

U8 GsmReadUart3(U8* Data, U8 Len, U32 TimeOut) {
	S32 ReturnLen = 0;
	S32 i;

	for (i = 0; i < TimeOut; i++) {
		ReturnLen = getCirBufSize(&UART3CirBuf);
		if (ReturnLen == Len) {
			return popFromCirBuf(&UART3CirBuf, (uint8_t *) Data, ReturnLen);
		}
		vTaskDelay(1);
	}
	if (ReturnLen > 0) {
		return popFromCirBuf(&UART3CirBuf, (uint8_t *) Data, ReturnLen);
	} else
		return 0;
}

U8 ReadUart0(U8 *Buf,U8 size)
{
	return popFromCirBuf(&UART0CirBuf,Buf,size);
}


U8 ReadUart3(U8 *Buf,U8 size)
{
	return popFromCirBuf(&UART3CirBuf,Buf,size);
}


U8 WriteUart3(U8 *Buf,U8 size)
{
	return UART_Send(UART3,Buf, size, BLOCKING);
}

U8 WriteUart0(U8 *Buf,U8 size)
{
	return UART_Send(UART0,Buf, size, BLOCKING);
}

void UART0Puts(U8 *Buf)
{
	U8 Len = strlen((char *)Buf);
	WriteUart0(Buf,Len);
}

void UART3Puts(U8 *Buf)
{
	U8 Len = strlen((char *)Buf);
	WriteUart3(Buf,Len);
}

void FlushUARTBuffer()
{
	flushCirBuf(&UART3CirBuf);
	flushCirBuf(&UART1CirBuf);
	flushCirBuf(&UART0CirBuf);
}

void UART3_Send_Buffer(U8 *Buf,U8 size){
	UART_Send(UART3,Buf, size, BLOCKING);
}

void uart3_get_byte(U8 *Buf,U8 size){
	popFromCirBuf(&UART3CirBuf,Buf,size);
}

U16 uart3_get_byte_count(){
	U16 Len = 0;
	Len = getCirBufSize(&UART3CirBuf);
	return Len;
}

U32 uart1_get_byte_count(){
	U32 Len = 0;
	Len = getCirBufSize(&UART1CirBuf);
	return Len;
}

void UART1_Send_Buffer(U8 *Buf,U8 size){
	UART_Send(UART1,Buf, size, BLOCKING);
}

void uart1_get_byte(U8 *Buf,U8 size){
	popFromCirBuf(&UART1CirBuf,Buf,size);
}


