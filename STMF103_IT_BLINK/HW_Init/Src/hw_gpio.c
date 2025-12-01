#include "hw_gpio.h"

// --- Init LED pin ---
void GPIOB_LED_Init(void)
{
  RCC -> APB2ENR |= RCC_APB2ENR_IOPBEN;
  // --- CLear Bits of PB2 ---
  GPIOB -> CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
  // --- Set conf MODE2 = 10, CNF2 = 00 ---
  GPIOB -> CRL |= GPIO_CRL_MODE2_1;
}

// --- Init button pin ---
void GPIOA_But_Init(void)
{
  RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN;
  // --- Clear bits of PA0 ---
  GPIOA -> CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
  // --- Set conf MODE0 = 00, CNF0 = 10 ---
  GPIOA -> CRL |= GPIO_CRL_CNF0_1;
}