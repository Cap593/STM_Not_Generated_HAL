/*Systick.c*/

#include "systick.h"


volatile uint32_t sysTickMs = 0;

void SysTick_Handler(void)
{
    sysTickMs++;
}

void SysTick_Init(void)
{
    // Assuming default 16 MHz core clock
    uint32_t reload = 16000U - 1U; // 1 ms

    SysTick->LOAD  = reload;
    SysTick->VAL   = 0;
    SysTick->CTRL  = (1U << 2) | (1U << 1) | (1U << 0);
}
