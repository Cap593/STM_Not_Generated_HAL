#include "exti.h"

extern EXTI_Callback_t exti_callbacks[16];

void EXTI0_IRQHandler(void)
{
    if (EXTI->PR & (1U << 0))
    {
        EXTI->PR = (1U << 0);  // Clear pending bit

        if (exti_callbacks[0])
            exti_callbacks[0]();
    }
}
