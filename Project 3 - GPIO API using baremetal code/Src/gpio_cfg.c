#include "gpio_cfg.h"

GPIO_InitTypeDef LED_Config =
{
    .pin   = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,
    .mode  = GPIO_MODE_OUTPUT,
    .pull  = GPIO_NOPULL,
    .speed = GPIO_SPEED_LOW,
    .otype = GPIO_PUSH_PULL
};

void GPIO_ConfigInit(void)
{
    GPIO_Init(GPIOD, &LED_Config);
}
