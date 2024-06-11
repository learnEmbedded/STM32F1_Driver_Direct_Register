#ifndef RCC_H_
#define RCC_H_

#include "stm32f1.h"

//This returns the APB1 clock value
uint32_t RCC_GetPCLK1Value(void);
//This returns the APB2 clock value
uint32_t RCC_GetPCLK2Value(void);
#endif