/**
 ******************************************************************************
 * @file           : main.c
 * @author         : K_Suhas
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "gpio_cfg.h"
#include "systick.h"

extern volatile uint32_t sysTickMs;

int main(void)
{
    uint32_t lastToggle12 = 0;
    uint32_t lastToggle13 = 0;
    uint32_t lastToggle14 = 0;
    uint32_t lastToggle15 = 0;

    __enable_irq();
	SysTick_Init();
    GPIO_ConfigInit();


    while (1)
    {
        if ((sysTickMs - lastToggle12) >= 100U)
        {
        	GPIO_TogglePin(GPIOD, GPIO_PIN_12);
            lastToggle12 = sysTickMs;
        }

        if ((sysTickMs - lastToggle13) >= 200U)
        {
        	GPIO_TogglePin(GPIOD, GPIO_PIN_13);
            lastToggle13 = sysTickMs;
        }

        if ((sysTickMs - lastToggle14) >= 500U)
        {
        	GPIO_TogglePin(GPIOD, GPIO_PIN_14);
            lastToggle14 = sysTickMs;
        }

        if ((sysTickMs - lastToggle15) >= 700U)
        {
        	GPIO_TogglePin(GPIOD, GPIO_PIN_15);
            lastToggle15 = sysTickMs;
        }
    }
}

