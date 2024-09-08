#ifndef __SERIAL2_H
#define __SERIAL2_H

#include <stdio.h>

extern char Serial2_RxPacket[];
extern uint8_t Serial2_RxFlag;

void Serial2_Init(void);
void Serial2_Power_OFF(void);

void Serial2_SendByte(uint8_t Byte);
void Serial2_SendArray(uint8_t *Array, uint16_t Length);
void Serial2_SendString(char *String);



#endif
