#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART2_Init(void);
void UART2_SendChar(char ch);
void UART2_SendString(const char *str);
void UART2_SendString_IT(const char *str);
char UART2_ReceiveChar(void);


#endif
