/*systick.h*/

#include "stm32f407xx.h"
#include <stdint.h>

void SysTick_Init(void);
void SysTick_Handler(void);
uint32_t SysTick_GetTick(void);
