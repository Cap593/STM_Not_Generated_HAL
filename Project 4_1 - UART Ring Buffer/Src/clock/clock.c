#include "stm32f407xx.h"
#include "system_stm32f4xx.h"
#include "clock.h"

void Clock_Init(void)
{
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY));

    RCC->CFGR = 0; // AHB=1, APB1=1, APB2=1

    SystemCoreClockUpdate();
}


