#include "stm32f0xx.h"
#include "main.h"

typedef struct polinom;
typedef struct PlnmToSend;

struct PlnmToSend *poliSend;
struct polinom *poli;

extern unsigned short LoadPlnmFromMsg (PlnmToSend * mko_send, polinom * plnm, unsigned short l_str);

float ff = 87.9;
/*unsigned*/ char acceptData[4];
unsigned char result;
void port_ini(void)
{
  GPIO_InitTypeDef GPIO_LEDS_ini;
  GPIO_InitTypeDef InitA0;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  
  GPIO_LEDS_ini.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
  GPIO_LEDS_ini.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_LEDS_ini.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_LEDS_ini.GPIO_OType = GPIO_OType_PP;
  GPIO_LEDS_ini.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
  GPIO_Init(GPIOC,&GPIO_LEDS_ini);
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
  
  InitA0.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
  InitA0.GPIO_Mode = GPIO_Mode_OUT;
  InitA0.GPIO_Speed = GPIO_Speed_Level_3;
  InitA0.GPIO_OType = GPIO_OType_PP;
  InitA0.GPIO_PuPd = GPIO_PuPd_NOPULL;
            
  GPIO_Init(GPIOA,&InitA0);
  
}
//-------------------------------------------
//
//-------------------------------------------
void recieveData(uint8_t *count_p,uint8_t bytes)
{
  uint8_t i=0;
  while(i<bytes)
  {
    while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET)
    {}
    USART_ReceiveData(USART2);
          i++;
          
  }
}
//-------------------------------------------
//
//-------------------------------------------
void delay(int n)
{
  while(n>0)
  {
    n--;
  }
}
//-------------------------------------------
//
//-------------------------------------------
    int i = 0;

void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
  {
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    while(i<5003)
    {
      //poliSend = USART_ReceiveData(USART2);
    }
    LoadPlnmFromMsg(poliSend,poli,16);
  }

}

//-------------------------------------------
//
//-------------------------------------------
void USART_ini()
{
  GPIO_InitTypeDef GPIO_Init_USART;
  USART_InitTypeDef USART_InitUser;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
  
  GPIO_Init_USART.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
  GPIO_Init_USART.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init_USART.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init_USART.GPIO_OType = GPIO_OType_PP;
  GPIO_Init_USART.GPIO_PuPd = GPIO_PuPd_UP;
  
  GPIO_Init(GPIOA,&GPIO_Init_USART);
  
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_1);
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
  
  USART_InitUser.USART_BaudRate = 9600;
  USART_InitUser.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitUser.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
  USART_InitUser.USART_Parity = USART_Parity_No;
  USART_InitUser.USART_StopBits = USART_StopBits_1;
  USART_InitUser.USART_WordLength = USART_WordLength_8b;
  
  USART_Init(USART2, &USART_InitUser);
  
  NVIC_EnableIRQ(USART2_IRQn);
  
  USART_ITConfig(USART2,USART_IT_RXNE, ENABLE);

  USART_Cmd(USART2,ENABLE);
}

int main()
{
  
  port_ini();
  USART_ini();
  while(1)
  {
    //if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1)
    //{
      delay(5000);
      if(acceptData!=0)
      {
        GPIO_SetBits(GPIOC,GPIO_Pin_9);
      }
      else
      {
        GPIO_SetBits(GPIOC,GPIO_Pin_8);
      }
      
   // }
   // else
   // {
//GPIO_SetBits(GPIOC,GPIO_Pin_8);
    //  delay(1000000);
   //   GPIO_ResetBits(GPIOC,GPIO_Pin_8);
      
    //  GPIO_SetBits(GPIOC,GPIO_Pin_9);
    //  delay(1000000);
    //  GPIO_ResetBits(GPIOC,GPIO_Pin_9);    
    //}
  }
  
  
  
      
  
}
