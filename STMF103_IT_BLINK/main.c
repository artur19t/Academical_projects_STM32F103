#include "stm32f10x.h"                  // Device header
#include "hw_gpio.h"
#include "hw_IT.h"
#include "button.h"

void SetSysClockTo72(void);

int main()
{
  SetSysClockTo72();
  GPIOB_LED_Init();
  GPIOA_But_Init();
  IT_EXTI_PAO_Init();
  
  // --- SysTick 1 ms ---
  SysTick->LOAD = 72000 - 1; 
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_ENABLE_Msk   |
                  SysTick_CTRL_TICKINT_Msk;
  while(1)
  {
  }
}

void SysTick_Handler(void)
{
    Button_Process();
}

void SetSysClockTo72(void)
{
  // --- Enable HSE ---
  RCC->CR |= RCC_CR_HSEON;

  // --- Wait HSE is ready ---
  while (!READ_BIT(RCC->CR, RCC_CR_HSERDY)) {}

  // --- Setting Flash ---
  FLASH->ACR |= FLASH_ACR_PRFTBE;        // Prefetch enable
  FLASH->ACR &= ~FLASH_ACR_LATENCY;      
  FLASH->ACR |= FLASH_ACR_LATENCY_2;     // 2 wait states 72

  // --- Setting bus ---
  RCC->CFGR &= ~RCC_CFGR_HPRE;  
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;  // AHB setting

  RCC->CFGR &= ~RCC_CFGR_PPRE1;
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // APB1 max = 36MHz

  RCC->CFGR &= ~RCC_CFGR_PPRE2;
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; // APB2 setting

  // --- Setting PLL ---
  RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
  RCC->CFGR |= RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9;  // HSE*9 = 72

  // --- Enable PLL ---
  RCC->CR |= RCC_CR_PLLON;

  // --- Wait PLL ---
  while (!READ_BIT(RCC->CR, RCC_CR_PLLRDY)) {}

  // --- Switch SYSCLK to PLL ---
  RCC->CFGR &= ~RCC_CFGR_SW;
  RCC->CFGR |= RCC_CFGR_SW_PLL;

  // --- Wait for switching ---
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}
}
