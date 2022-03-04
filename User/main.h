
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"


void LEDS_ini(void);
void USART3_ini(void);


void sysTickSet(uint32_t nTime);
void sysTickDalay(uint32_t nTime);
void sysTickDalayInit(void);

uint32_t sysTickGet();

void delay(int x);


#endif /* __MAIN_H */

