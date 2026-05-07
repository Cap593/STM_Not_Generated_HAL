/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Suhas K
 * @brief          : Main program body
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f407xx.h"

volatile uint32_t sysTickMs = 0;

// LED on PD12
static void LED_Init(void)
{
    RCC->AHB1ENR |= (1U << 3); // Enable GPIOD clock

    //PD12
    GPIOD->MODER &= ~(0x3U << (12U * 2)); // clear mode
    GPIOD->MODER |=  (0x1U << (12U * 2)); // output mode

    GPIOD->PUPDR &= ~(0x3U << (12U * 2)); // no pull

    //PD13
    GPIOD->MODER &= ~(0x3U << (13U * 2)); // clear mode
    GPIOD->MODER |=  (0x1U << (13U * 2)); // output mode

    GPIOD->PUPDR &= ~(0x3U << (13U * 2)); // no pull

    //PD14
    GPIOD->MODER &= ~(0x3U << (14U * 2)); // clear mode
    GPIOD->MODER |=  (0x1U << (14U * 2)); // output mode

    GPIOD->PUPDR &= ~(0x3U << (14U * 2)); // no pull

    //PD15
    GPIOD->MODER &= ~(0x3U << (15U * 2)); // clear mode
    GPIOD->MODER |=  (0x1U << (15U * 2)); // output mode

    GPIOD->PUPDR &= ~(0x3U << (15U * 2)); // no pull
}

static void LED_Toggle(void)
{
    GPIOD->ODR ^= (1U << 12);
}

static void SysTick_Init(void)
{
    // Assuming default 16 MHz core clock
    uint32_t reload = 16000U - 1U; // 1 ms

    SysTick->LOAD  = reload;
    SysTick->VAL   = 0;
    SysTick->CTRL  = (1U << 2) | (1U << 1) | (1U << 0);
}

void SysTick_Handler(void)
{
    sysTickMs++;
}

int main(void)
{
    LED_Init();
    SysTick_Init();

    uint32_t lastToggle = 0;
    uint32_t lastToggle13 = 0;
    uint32_t lastToggle14 = 0;
    uint32_t lastToggle15 = 0;

    while (1)
    {
    	// 500ms, 1000ms, 2000ms, 100ms
        if ((sysTickMs - lastToggle) >= 100U)
        {
        	//toggle LED 12
            LED_Toggle();
            lastToggle = sysTickMs;
        }

        if ((sysTickMs - lastToggle13) >= 200U)
        {
        	//toggle LED 13
        	GPIOD->ODR ^= (1U << 13);
            lastToggle13 = sysTickMs;
        }

        if ((sysTickMs - lastToggle14) >= 500U)
        {
        	//toggle LED 14
        	GPIOD->ODR ^= (1U << 14);
            lastToggle14 = sysTickMs;
        }

        if ((sysTickMs - lastToggle15) >= 700U)
        {
        	//toggle LED 15
        	GPIOD->ODR ^= (1U << 15);
            lastToggle15 = sysTickMs;
        }

    }
}
