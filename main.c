#include "stm32f0xx.h"
#include "main.h"
float ff = 87.9;



int main()
{
  uint8_t array[sizeof(float)];
  uint8_t *arrData;
  arrData = (uint8_t*)&ff;
  for(int i =0;i<sizeof(float);i++)
  {
    array[i]=*(arrData++);
  }
  port_ini();
  USART_ini();
  while(1)
  {
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1)
    {
      delay(5000);
      sendData(array,4);
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
