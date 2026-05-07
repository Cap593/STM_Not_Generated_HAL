#include "exti.h"

/* ===========================
 * Static Callback Table
 * =========================== */
EXTI_Callback_t exti_callbacks[16] = {0};

/* ===========================
 * Enable SYSCFG Clock
 * =========================== */
static void EXTI_EnableSYSCFGClock(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}

/* ===========================
 * Configure SYSCFG EXTICR
 * =========================== */
static void EXTI_ConfigPort(uint8_t pin, EXTI_Port_t port)
{
    uint8_t index = pin / 4;          // EXTICR index (0–3)
    uint8_t shift = (pin % 4) * 4;    // 4 bits per EXTI

    SYSCFG->EXTICR[index] &= ~(0xFU << shift);
    SYSCFG->EXTICR[index] |=  ((uint32_t)port << shift);
}

/* ===========================
 * EXTI Initialization
 * =========================== */
void EXTI_Init(uint8_t pin,
               EXTI_Port_t port,
               EXTI_Trigger_t trigger,
               EXTI_Callback_t callback)
{
    /* 1. Enable SYSCFG clock */
    EXTI_EnableSYSCFGClock();

    /* 2. Map GPIO port to EXTI line */
    EXTI_ConfigPort(pin, port);

    /* 3. Configure trigger */
    EXTI->RTSR &= ~(1U << pin);
    EXTI->FTSR &= ~(1U << pin);

    if (trigger == EXTI_TRIGGER_RISING)
        EXTI->RTSR |= (1U << pin);
    else if (trigger == EXTI_TRIGGER_FALLING)
        EXTI->FTSR |= (1U << pin);
    else
    {
        EXTI->RTSR |= (1U << pin);
        EXTI->FTSR |= (1U << pin);
    }

    /* 4. Enable interrupt */
    EXTI->IMR |= (1U << pin);

    /* 5. Register callback */
    exti_callbacks[pin] = callback;

    /* 6. Enable NVIC IRQ */
    if (pin <= 4)
    {
        NVIC_EnableIRQ(EXTI0_IRQn + pin);
    }
    else if (pin <= 9)
    {
        NVIC_EnableIRQ(EXTI9_5_IRQn);
    }
    else
    {
        NVIC_EnableIRQ(EXTI15_10_IRQn);
    }
}
