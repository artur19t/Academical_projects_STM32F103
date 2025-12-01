#include "button.h"

#define BUTTON_READ()   (GPIOA->IDR & GPIO_IDR_IDR0)

// --- states of button ---
typedef enum {
  BTN_IDLE,
  BTN_DEBOUNCE,
  BTN_PRESSED
} ButtonState;

static volatile uint8_t start_check = 0; // flag from EXTI
static ButtonState state = BTN_IDLE;
static uint8_t debounce_cnt = 0;


// --- interrupt from button ---
void EXTI0_IRQHandler(void)
{
  if (EXTI->PR & EXTI_PR_PR0)
  {
    EXTI->PR = EXTI_PR_PR0;
    // --- turn on flag ---
    start_check = 1; 
  }
}

void Button_OnClick(void)
{
  if (GPIOB->ODR & GPIO_ODR_ODR2)
  {
    GPIOB->BSRR = GPIO_BSRR_BR2;
  }else
  {
    GPIOB->BSRR = GPIO_BSRR_BS2;
  }
}

// --- parsing states of button ---
void Button_Process(void)
{
  switch (state)
  {
    case BTN_IDLE:
      if (start_check) 
      {
        start_check = 0;
        debounce_cnt = 20; // 20ms of debounce
        state = BTN_DEBOUNCE;
      }
      break;

    case BTN_DEBOUNCE:
        
      if (!BUTTON_READ()) 
      {
        state = BTN_IDLE; //button released
      }else if (--debounce_cnt == 0) 
      {
        state = BTN_PRESSED;
        Button_OnClick(); //check complete
      }
      break;

    case BTN_PRESSED:
      if (!BUTTON_READ()) 
      {
        state = BTN_IDLE;
      }
      break;
  }
}
