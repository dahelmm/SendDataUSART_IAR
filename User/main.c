
#include "main.h"
#define BLUE_ON GPIO_SetBits(GPIOB,GPIO_Pin_6);
#define BLUE_OFF GPIO_ResetBits(GPIOB,GPIO_Pin_6);
#define GREEN_ON GPIO_SetBits(GPIOB,GPIO_Pin_7);
#define GREEN_OFF GPIO_ResetBits(GPIOB,GPIO_Pin_7);

#define ON_ALL GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);
#define OFF_ALL GPIO_ResetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);

#define DELAY 5000

static volatile uint32_t sysTickCount = 0;

void sysTickDalayInit()
{
    SysTick_Config(SystemCoreClock/1000); //1 ms
}

void sysTickDalay(uint32_t nTime)
{
  sysTickCount = nTime;
  while(sysTickCount!=0);
}

void SysTick_Handler(void)
{
  if(sysTickCount!=0)
  {
    sysTickCount--;
  }
}

void USART3_ini(void)
{
  GPIO_InitTypeDef GPIO_init_USART;
  USART_InitTypeDef USART_init_User;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);

  GPIO_init_USART.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_init_USART.GPIO_Mode = GPIO_Mode_AF;
  GPIO_init_USART.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_init_USART.GPIO_OType = GPIO_OType_PP;
  GPIO_init_USART.GPIO_PuPd = GPIO_PuPd_UP; 
  
  GPIO_Init(GPIOB,&GPIO_init_USART);
  
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource10, GPIO_AF_USART3);
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource11, GPIO_AF_USART3);
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

  USART_init_User.USART_BaudRate = 9600;
  USART_init_User.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_init_User.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
  USART_init_User.USART_Parity = USART_Parity_No;
  USART_init_User.USART_StopBits = USART_StopBits_1;
  USART_init_User.USART_WordLength = USART_WordLength_8b;

  USART_Init(USART3,&USART_init_User);
  
  USART_Cmd(USART3, ENABLE);

}
void LEDS_ini(void)
{
  
  GPIO_InitTypeDef GPIO_LEDs_ini;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
  
  
  GPIO_LEDs_ini.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_LEDs_ini.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_LEDs_ini.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_LEDs_ini.GPIO_OType = GPIO_OType_PP;
  GPIO_LEDs_ini.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
  GPIO_Init(GPIOB,&GPIO_LEDs_ini);
  
}




int main(void)
{
  
  uint8_t count = 0;
  
  LEDS_ini();
  USART3_ini();
  
  sysTickDalayInit();
  
  while (1)
  {
    //OFF_ALL;
    sysTickDalay(30);   
    USART_SendData(USART3, count++);

    //ON_ALL;
    //sysTickDalay(15);

  }
}






