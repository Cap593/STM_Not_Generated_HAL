/*Author - Suhas K*/

#ifndef GPIO_H
#define GPIO_H

#include "stm32f407xx.h"
#include <stdint.h>

/* ===========================
 * GPIO Pin Definitions
 * =========================== */
#define GPIO_PIN_0   (1U << 0)
#define GPIO_PIN_1   (1U << 1)
#define GPIO_PIN_2   (1U << 2)
#define GPIO_PIN_3   (1U << 3)
#define GPIO_PIN_4   (1U << 4)
#define GPIO_PIN_5   (1U << 5)
#define GPIO_PIN_6   (1U << 6)
#define GPIO_PIN_7   (1U << 7)
#define GPIO_PIN_8   (1U << 8)
#define GPIO_PIN_9   (1U << 9)
#define GPIO_PIN_10  (1U << 10)
#define GPIO_PIN_11  (1U << 11)
#define GPIO_PIN_12  (1U << 12)
#define GPIO_PIN_13  (1U << 13)
#define GPIO_PIN_14  (1U << 14)
#define GPIO_PIN_15  (1U << 15)

/* ===========================
 * GPIO Mode Enumeration
 * =========================== */
typedef enum
{
    GPIO_MODE_INPUT  = 0x0,
    GPIO_MODE_OUTPUT = 0x1,
    GPIO_MODE_AF     = 0x2,
    GPIO_MODE_ANALOG = 0x3
} GPIO_Mode_t;

/* ===========================
 * GPIO Pull-up / Pull-down
 * =========================== */
typedef enum
{
    GPIO_NOPULL   = 0x0,
    GPIO_PULLUP   = 0x1,
    GPIO_PULLDOWN = 0x2
} GPIO_Pull_t;

/* ===========================
 * GPIO Output Speed
 * =========================== */
typedef enum
{
    GPIO_SPEED_LOW = 0x0,
    GPIO_SPEED_MEDIUM,
    GPIO_SPEED_HIGH,
    GPIO_SPEED_VERY_HIGH
} GPIO_Speed_t;

/* ===========================
 * GPIO Output Type
 * =========================== */
typedef enum
{
    GPIO_PUSH_PULL  = 0x0,
    GPIO_OPEN_DRAIN = 0x1
} GPIO_OType_t;

/* ===========================
 * GPIO Init Structure
 * =========================== */
typedef struct
{
    uint16_t       pin;
    GPIO_Mode_t    mode;
    GPIO_Pull_t    pull;
    GPIO_Speed_t   speed;
    GPIO_OType_t   otype;
} GPIO_InitTypeDef;

/* ===========================
 * GPIO Driver API
 * =========================== */
void GPIO_Init(GPIO_TypeDef *port, GPIO_InitTypeDef *cfg);
void GPIO_WritePin(GPIO_TypeDef *port, uint16_t pin, uint8_t state);
uint8_t GPIO_ReadPin(GPIO_TypeDef *port, uint16_t pin);
void GPIO_TogglePin(GPIO_TypeDef *port, uint16_t pin);

#endif /* GPIO_H */
