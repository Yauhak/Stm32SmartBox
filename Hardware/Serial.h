#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

#define STM32_TX GPIO_Pin_9
#define STM32_RX GPIO_Pin_10

extern uint8_t RecvArray[256];
extern uint8_t Preserved;//当前一共接收了多少字节字符

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);

uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);

#endif
