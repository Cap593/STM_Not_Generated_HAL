#include "gpio_cfg.h"
#include "exti_cfg.h"
#include "systick.h"
#include "uart.h"
#include "clock.h"

#define ZERO	0

uint32_t current_time_from_systick;
uint32_t lastTrigger;
char rx;
extern char uart_rx_char;
extern uint8_t uart_rx_ready;

int main(void)
{

    __enable_irq();

    Clock_Init();
    UART2_Init();
    SysTick_Init();
    GPIO_ConfigInit();
    EXTI_ConfigInit();


    // Using polling based
    UART2_SendString("UART Initialized!\r\n");

    while (1)
    {
    	/*1. Transmit String
    	current_time_from_systick = SysTick_GetTick();

        if ((current_time_from_systick - lastTrigger) >= 2000U)
        {
        	// Using interrupt based
        	UART2_SendString_IT("Hello from STM32 using UART Interrupt\r\n");
        	lastTrigger = current_time_from_systick;
        }*/


    	/* 2. Echo test
    	rx = UART2_ReceiveChar();   // Wait for key
    	UART2_SendChar(rx);         // Echo back */

    	/* 3. Echo test using RxInterrupt*/
    	if (uart_rx_ready)
    	{
    		uart_rx_ready = 0;
    	    UART2_SendChar(uart_rx_char);  // echo from ISR data
    	}

    }
}
