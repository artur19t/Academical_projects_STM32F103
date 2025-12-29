#include "it.h"

uint16_t old_pos = 0;
uint8_t len = 0;
void USART1_IRQHandler(void)
{
  if(USART1->SR & USART_SR_IDLE)
  {
    (void)USART1->SR;
    (void)USART1->DR;
    uint16_t received = RX_SIZE - DMA1_Channel5->CNDTR;
    if (old_pos < received)
    {
      collect_data(&RxBuf[old_pos], received-old_pos, false);
    }else
    {
      collect_data(&RxBuf[old_pos], RX_SIZE-old_pos, true);
      collect_data(&RxBuf[0], received, false);
    }
    old_pos = received;
  }
}
