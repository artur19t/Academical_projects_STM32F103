#include "usart_logic.h"

enum comand {LED_ON, LED_OFF, RETRANSLATE};
static char cmd_buf[RX_SIZE];


typedef struct {
  const char *cmd;
  void (*handler)(void);
} usart_cmd_entry_t;

const usart_cmd_entry_t usart_commands[] = {
  [LED_ON]       = { "led_on",       cmd_led_on },
  [LED_OFF]      = { "led_off",      cmd_led_off },
  [RETRANSLATE]  = { "retranslate",  cmd_retranslate }
};

void collect_data(uint8_t *bufAddr, uint16_t dSize, bool need_s)
{
  prepare(bufAddr, dSize, need_s);
  if (!need_s)
  {
    process_command();
    len = 0;
  }
}

void prepare(uint8_t *bufAddr, uint16_t dSize, bool need_s)
{
  for(uint16_t i = 0; i < dSize; i++)
  {
    char c = bufAddr[i];
    if(c == '\r' || c == '\n')
    {
      break;
    }
    cmd_buf[len] = c;
    len++;
  }
  if (!need_s)
  {
    cmd_buf[len] = '\0';
  }
}

void process_command(void)
{
  for(uint8_t i = 0; i < 3; i++)
  {
    if(strcmp(cmd_buf, usart_commands[i].cmd) == 0)
    {
      usart_commands[i].handler();
      return;
    }
  }
  uint8_t f_ans[] = {"FALSE"};
  USART_UstTranslate(f_ans, 5);
}
void cmd_led_on(void)
{
  GPIOB->BSRR |= GPIO_BSRR_BS2;
}

void cmd_led_off(void)
{
  GPIOB->BSRR |= GPIO_BSRR_BR2;
}

void cmd_retranslate(void)
{
}

void USART_UstTranslate(uint8_t *tx_buf, uint16_t dSize)
{
  DMA1_USART1TX_UsrConfig(tx_buf, dSize);
}
