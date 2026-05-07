#ifndef UART_H
#define UART_H

#include <stdint.h>

#define UART_RX_BUFFER_SIZE 128
typedef struct
{
    volatile uint8_t buffer[UART_RX_BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} UART_RxBuffer_t;

void UART2_Init(void);
void UART2_SendChar(char ch);
void UART2_SendString(const char *str);
void UART2_SendString_IT(const char *str);
char UART2_ReceiveChar(void);
uint8_t UART2_ReadChar(void);
uint8_t UART2_RxAvailable(void);
void UART_ProcessInput(void);
void UART_HandleCommand(char *cmd);


#endif
