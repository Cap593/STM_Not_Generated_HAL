#ifndef EXTI_H
#define EXTI_H

#include "stm32f407xx.h"
#include <stdint.h>

/* ===========================
 * EXTI Trigger Selection
 * =========================== */
typedef enum
{
    EXTI_TRIGGER_RISING = 0,
    EXTI_TRIGGER_FALLING,
    EXTI_TRIGGER_BOTH
} EXTI_Trigger_t;

/* ===========================
 * EXTI Port Encoding
 * ===========================
 * These values match SYSCFG EXTICR encoding
 * =========================== */
typedef enum
{
    EXTI_PORT_A = 0x0,
    EXTI_PORT_B = 0x1,
    EXTI_PORT_C = 0x2,
    EXTI_PORT_D = 0x3,
    EXTI_PORT_E = 0x4,
    EXTI_PORT_F = 0x5,
    EXTI_PORT_G = 0x6,
    EXTI_PORT_H = 0x7
} EXTI_Port_t;

/* ===========================
 * EXTI Callback Type
 * =========================== */
typedef void (*EXTI_Callback_t)(void);

/* ===========================
 * EXTI Driver API
 * =========================== */
void EXTI_Init(uint8_t pin,
               EXTI_Port_t port,
               EXTI_Trigger_t trigger,
               EXTI_Callback_t callback);

#endif /* EXTI_H */
