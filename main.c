#include "stm32f0xx.h"
#include "main.h"
int main()
{
  port_ini();
  USART_ini();
  while(1)
  {
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1)
    {
      delay(5000);
      USART_SendData(USART2,0xff);
    }
    else
    {
      GPIO_SetBits(GPIOC,GPIO_Pin_8);
      delay(1000000);
      GPIO_ResetBits(GPIOC,GPIO_Pin_8);
      
      GPIO_SetBits(GPIOC,GPIO_Pin_9);
      delay(1000000);
      GPIO_ResetBits(GPIOC,GPIO_Pin_9);    }
  }
  
  
  
      
  
}
