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
        if (UART2_RxAvailable())
        {
            char ch = UART2_ReadChar();

            // Echo
            UART2_SendChar(ch);

            // Example: newline handling
            if (ch == '\r')
            {
                UART2_SendString("\r\n");
            }
        }
    }
   return 0;
}
