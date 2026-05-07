#include "gpio.h"

/* ===========================
 * Internal Helper Functions
 * =========================== */
static void GPIO_EnableClock(GPIO_TypeDef *port)
{
    if (port == GPIOA) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    else if (port == GPIOB) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    else if (port == GPIOC) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    else if (port == GPIOD) RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    else if (port == GPIOE) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    else if (port == GPIOF) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
    else if (port == GPIOG) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
    else if (port == GPIOH) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
}

/* ===========================
 * GPIO Initialization
 * =========================== */
void GPIO_Init(GPIO_TypeDef *port, GPIO_InitTypeDef *cfg)
{
    uint32_t pin;

    /* 1. Enable clock */
    GPIO_EnableClock(port);

    /* 2. Configure each selected pin */
    for (pin = 0; pin < 16; pin++)
    {
        if (cfg->pin & (1U << pin))
        {
            uint32_t pos = pin * 2;

            /* MODER */
            port->MODER &= ~(0x3U << pos);
            port->MODER |=  ((uint32_t)cfg->mode << pos);

            /* OTYPER (only for output or AF) */
            if (cfg->mode == GPIO_MODE_OUTPUT || cfg->mode == GPIO_MODE_AF)
            {
                port->OTYPER &= ~(1U << pin);
                port->OTYPER |=  ((uint32_t)cfg->otype << pin);
            }

            /* OSPEEDR */
            port->OSPEEDR &= ~(0x3U << pos);
            port->OSPEEDR |=  ((uint32_t)cfg->speed << pos);

            /* PUPDR */
            port->PUPDR &= ~(0x3U << pos);
            port->PUPDR |=  ((uint32_t)cfg->pull << pos);
        }
    }
}

/* ===========================
 * GPIO Write Pin
 * =========================== */
void GPIO_WritePin(GPIO_TypeDef *port, uint16_t pin, uint8_t state)
{
    if (state)
        port->BSRR = pin;             /* Set pin */
    else
        port->BSRR = (uint32_t)pin << 16U; /* Reset pin */
}

/* ===========================
 * GPIO Read Pin
 * =========================== */
uint8_t GPIO_ReadPin(GPIO_TypeDef *port, uint16_t pin)
{
    return ((port->IDR & pin) ? 1U : 0U);
}

/* ===========================
 * GPIO Toggle Pin
 * =========================== */
void GPIO_TogglePin(GPIO_TypeDef *port, uint16_t pin)
{
    port->ODR ^= pin;
}
