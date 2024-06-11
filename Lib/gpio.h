#ifndef GPIO_H
#define GPIO_H

#include "stm32f1.h"

typedef union {
	struct {
		uint8_t MODE : 4;
		uint8_t CNF : 4;
	} CR;
	
	uint8_t PinMode;
} GPIO_PinMode_t;

typedef struct {
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;
	// uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PinPuPdControl;
}	GPIO_PinConfig_t;


typedef struct {
	GPIO_RegDef_t *pGPIOx;
	GPIO_PinConfig_t GPIO_PinConfig;
}	GPIO_Handle_t;

#define GPIO_PIN_NO_0			0
#define GPIO_PIN_NO_1			1
#define GPIO_PIN_NO_2			2
#define GPIO_PIN_NO_3			3
#define GPIO_PIN_NO_4			4
#define GPIO_PIN_NO_5			5
#define GPIO_PIN_NO_6			6
#define GPIO_PIN_NO_7			7
#define GPIO_PIN_NO_8			8
#define GPIO_PIN_NO_9			9
#define GPIO_PIN_NO_10		10
#define GPIO_PIN_NO_11		11
#define GPIO_PIN_NO_12		12
#define GPIO_PIN_NO_13		13
#define GPIO_PIN_NO_14		14
#define GPIO_PIN_NO_15		15

// GPIO mode settings
#define GPIO_MODE_IN            0x00

#define GPIO_MODE_OUT_10MHz     0x01
#define GPIO_MODE_OUT_2MHz      0x02
#define GPIO_MODE_OUT_50MHz     0x03

#define GPIO_MODE_IT_FT					(0x04)
#define GPIO_MODE_IT_RT					(0x05)
#define GPIO_MODE_IT_RFT				(0x06)

// GPIO configuration settings for input mode
#define GPIO_MODE_IN_ANALOG     (0x00 << 2)
#define GPIO_MODE_IN_FLOAT      (0x01 << 2)
#define GPIO_MODE_IN_PUPD       (0x02 << 2)


// GPIO configuration settings for output mode
#define GPIO_MODE_OUT_PP        (0x00 << 2)
#define GPIO_MODE_OUT_OD        (0x01 << 2)
#define GPIO_MODE_AF_OUT_PP     (0x02 << 2)
#define GPIO_MODE_AF_OUT_OD     (0x03 << 2)

// Pull-up/Pull-down configuration
#define GPIO_PIN_PU             0x01
#define GPIO_PIN_PD             0x00

void GPIO_PeriClockControl(GPIO_RegDef_t* pGPIOx, uint8_t EnOrDis);
void _GPIO_Init(GPIO_Handle_t* pGPIOHandle);
void _GPIO_DeInit(GPIO_RegDef_t* pGPIOx, uint8_t EnOrDis);

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t* pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t* pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber);

void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDis);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);









#endif