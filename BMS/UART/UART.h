#ifndef __UART_H
#define  __UART_H

#include "type.h"

typedef struct
{
	char WeighScaleName[25];
	U8 StartData;
	U8 EndData;
	U32 BaudRate;
}WEIGHCONFIG;

void InitECRUARTs();
U8 ReadUart0(U8 *Buf,U8 size);
U8 WriteUart0(U8 *Buf,U8 size);
U8 ReadUart3(U8 *Buf,U8 size);
U8 WriteUart3(U8 *Buf,U8 size);
void UART0Puts(U8 *Buf);
void UART3Puts(U8 *Buf);
U8 GsmReadUart0(U8* Data, U8 Len, U32 TimeOut);
void FlushUARTBuffer();
void ConfigureUart3();
U16 uart3_get_byte_count();
void uart3_get_byte(U8 *Buf,U8 size);
void UART3_Send_Buffer(U8 *Buf,U8 size);
void ConfigureUart1();
void UART1_Send_Buffer(U8 *Buf,U8 size);
void uart1_get_byte(U8 *Buf,U8 size);
U32 uart1_get_byte_count();
#endif
