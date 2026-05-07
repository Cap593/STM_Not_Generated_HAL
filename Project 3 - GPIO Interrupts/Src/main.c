#include "gpio_cfg.h"
#include "exti_cfg.h"
#include "systick.h"

int main(void)
{
    __enable_irq();

    SysTick_Init();
    GPIO_ConfigInit();
    EXTI_ConfigInit();

    while (1)
    {
        /* Application logic */
    }
}
