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
    UART2_SendString("\r\nUART CLI Ready\r\n> ");
    UART2_SendString("Following options are available:\r\n");
    UART2_SendString("1) Help\r\n");
    UART2_SendString("2) Status\r\n");
    UART2_SendString("3) LED Blue Light ON\r\n");
    UART2_SendString("4) LED Blue Light OFF\r\n");
    UART2_SendString("Input Command : ");

    while (1)
    {
    	UART_ProcessInput();
    }

   return 0;

}
