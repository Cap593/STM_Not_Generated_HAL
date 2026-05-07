#include "gpio_cfg.h"

/* ===========================
 * GPIO Configuration Tables
 * =========================== */

/* LEDs on GPIOD */
static GPIO_InitTypeDef GPIOD_ConfigTable[] =
{
    {
        .pin   = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,
        .mode  = GPIO_MODE_OUTPUT,
        .pull  = GPIO_NOPULL,
        .speed = GPIO_SPEED_LOW,
        .otype = GPIO_PUSH_PULL
    }
};

/* User Button on GPIOA (PA0) */
static GPIO_InitTypeDef GPIOA_ConfigTable[] =
{
    {
        .pin   = GPIO_PIN_0,
        .mode  = GPIO_MODE_INPUT,
        .pull  = GPIO_NOPULL,   // external pull-down on board
        .speed = GPIO_SPEED_LOW,
        .otype = GPIO_PUSH_PULL // ignored for input
    }
};

/* ===========================
 * GPIO Configuration Init
 * =========================== */
void GPIO_ConfigInit(void)
{
    /* Init GPIOD */
    for (uint32_t i = 0; i < (sizeof(GPIOD_ConfigTable) / sizeof(GPIOD_ConfigTable[0])); i++)
    {
        GPIO_Init(GPIOD, &GPIOD_ConfigTable[i]);
    }

    /* Init GPIOA */
    for (uint32_t i = 0; i < (sizeof(GPIOA_ConfigTable) / sizeof(GPIOA_ConfigTable[0])); i++)
    {
        GPIO_Init(GPIOA, &GPIOA_ConfigTable[i]);
    }
}
