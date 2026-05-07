#include "exti_cfg.h"
#include "exti.h"
#include "gpio.h"
#include "systick.h"

#define DEBOUNCE_TIME_MS    200U
#define LONG_PRESS_TIME_MS  600U
#define DOUBLE_CLICK_MS     400U

static uint32_t last_press_time   = 0;
static uint32_t last_release_time = 0;
static uint32_t last_click_time   = 0;

static uint8_t  button_pressed    = 0;
static uint8_t  click_pending     = 0;


static void Button_Callback(void)
{
    uint32_t now = SysTick_GetTick();
    uint8_t  pin_state = GPIO_ReadPin(GPIOA, GPIO_PIN_0);

    /* =====================
     * RISING EDGE (PRESS)
     * ===================== */
    if (pin_state)
    {
        if ((now - last_press_time) < DEBOUNCE_TIME_MS)
            return;

        last_press_time = now;
        button_pressed = 1;

        GPIO_TogglePin(GPIOD, GPIO_PIN_12);   // Press LED

        /* Double-click detection */
        if (click_pending && ((now - last_click_time) <= DOUBLE_CLICK_MS))
        {
            GPIO_TogglePin(GPIOD, GPIO_PIN_15);  // Double-click LED
            click_pending = 0;
        }
        else
        {
            click_pending = 1;
            last_click_time = now;
        }
    }

    /* =====================
     * FALLING EDGE (RELEASE)
     * ===================== */
    else
    {
        if ((now - last_release_time) < DEBOUNCE_TIME_MS)
            return;

        last_release_time = now;
        button_pressed = 0;

        GPIO_TogglePin(GPIOD, GPIO_PIN_13);   // Release LED

        uint32_t press_duration = now - last_press_time;

        if (press_duration >= LONG_PRESS_TIME_MS)
        {
            GPIO_TogglePin(GPIOD, GPIO_PIN_14);  // Long press LED
            click_pending = 0;  // long press cancels click
        }
    }
}

void EXTI_ConfigInit(void)
{
    EXTI_Init(0,                 		// Pin 0
    		  EXTI_PORT_A,       		// PD0
			  EXTI_TRIGGER_BOTH,
              Button_Callback);
}



/*#define DEBOUNCE_TIME_MS  200U

static uint32_t last_button_time = 0;*/

/* Button callback */
/*static void Button_Callback(void)
{
    uint32_t current_time = SysTick_GetTick();

    if ((current_time - last_button_time) >= DEBOUNCE_TIME_MS)
    {
        GPIO_TogglePin(GPIOD, GPIO_PIN_12);
        GPIO_TogglePin(GPIOD, GPIO_PIN_13);
        GPIO_TogglePin(GPIOD, GPIO_PIN_14);
        GPIO_TogglePin(GPIOD, GPIO_PIN_15);
        last_button_time = current_time;
    }
}*/
