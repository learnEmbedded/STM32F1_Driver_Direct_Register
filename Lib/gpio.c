#include "gpio.h"

void GPIO_PeriClockControl(GPIO_RegDef_t* pGPIOx, uint8_t EnOrDis) {
	if(EnOrDis == ENABLE) {
		if(pGPIOx == GPIOA) {
			GPIOA_PCLK_EN();
		}else if(pGPIOx == GPIOB) {
			GPIOB_PCLK_EN();
		}else if(pGPIOx == GPIOC) {
			GPIOC_PCLK_EN();
		}else if(pGPIOx == GPIOD) {
			GPIOD_PCLK_EN();
		}else if(pGPIOx == GPIOE) {
			GPIOE_PCLK_EN();
		}else if(pGPIOx == GPIOF) {
			GPIOF_PCLK_EN();
		}else if(pGPIOx == GPIOG) {
			GPIOG_PCLK_EN();
		}
	}else {
		//
	}
}

void _GPIO_Init(GPIO_Handle_t* pGPIOHandle) {
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);
	uint32_t temp = 0;

	if ((pGPIOHandle->GPIO_PinConfig.GPIO_PinMode & 0x03) < GPIO_MODE_IT_FT) {
			if ((pGPIOHandle->GPIO_PinConfig.GPIO_PinMode & 0x03) == GPIO_MODE_IN) {
					if ((pGPIOHandle->GPIO_PinConfig.GPIO_PinMode & (0x03 << 2)) == GPIO_MODE_IN_PUPD) {
							// C?u hình pull-up/pull-down
							if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl == GPIO_PIN_PU) {
									pGPIOHandle->pGPIOx->ODR |= (1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
							} else {
									pGPIOHandle->pGPIOx->ODR &= ~(1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
							}
					}
			} else if (((pGPIOHandle->GPIO_PinConfig.GPIO_PinMode & 0x03) >= GPIO_MODE_OUT_10MHz) && 
								 ((pGPIOHandle->GPIO_PinConfig.GPIO_PinMode & 0x03) <= GPIO_MODE_OUT_50MHz)) {
					if ((pGPIOHandle->GPIO_PinConfig.GPIO_PinMode & (0x03 << 2)) <= GPIO_MODE_OUT_OD) {
							// C?u hình pull-up/pull-down
							if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl == GPIO_PIN_PU) {
									pGPIOHandle->pGPIOx->ODR |= (1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)); 
							} else {
									pGPIOHandle->pGPIOx->ODR &= ~(1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
							}
					}
			}
			
			if ((pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber) < 8) {
					temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (4 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)));
					pGPIOHandle->pGPIOx->CRL &= ~(0xF << (4 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)));
					pGPIOHandle->pGPIOx->CRL |= temp;
			} else {
					temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (4 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8)));
					pGPIOHandle->pGPIOx->CRH &= ~(0xF << (4 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8)));
					pGPIOHandle->pGPIOx->CRH |= temp;
			}
	} else {
			if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT) {
					EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
					EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			} else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT) {
					EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
					EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			} else {
					EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
					EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			}
			
			uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
			uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
			uint8_t gpioX = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
			AFIO_PCLK_EN();
			AFIO->EXTICR[temp1] = (gpioX << (temp2 * 4));
			
			EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}
}


void _GPIO_DeInit(GPIO_RegDef_t* pGPIOx, uint8_t EnOrDis) {
	if(EnOrDis == DISABLE) {
		if(pGPIOx == GPIOA) {
			GPIOA_REG_RESET();
		}else if(pGPIOx == GPIOB) {
			GPIOB_REG_RESET();
		}else if(pGPIOx == GPIOC) {
			GPIOC_REG_RESET();
		}else if(pGPIOx == GPIOD) {
			GPIOD_REG_RESET();
		}else if(pGPIOx == GPIOE) {
			GPIOE_REG_RESET();
		}else if(pGPIOx == GPIOF) {
			GPIOF_REG_RESET();
		}else if(pGPIOx == GPIOG) {
			GPIOG_REG_RESET();
		}else {
			AFIO_PCLK_EN();
		}
		
	}else {
		//
 }
}

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber){
	uint8_t value;
	value = (uint8_t)(pGPIOx->IDR >> PinNumber)& 0x00000001;
	return value;
}


uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t* pGPIOx) {
	uint16_t value;
	value = (uint16_t)(pGPIOx->IDR);
	return value;
}

void GPIO_WriteToOutputPin(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber, uint8_t Value){
	if (Value == GPIO_PIN_SET) {
		pGPIOx->ODR |= (1 << PinNumber);
	} else {
		pGPIOx->ODR &=~ (1 << PinNumber);
	}
}
void GPIO_WriteToOutputPort(GPIO_RegDef_t* pGPIOx, uint16_t Value){
	pGPIOx->ODR = Value;
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber) {
	pGPIOx->ODR ^= (1 << PinNumber);
}

void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDis) {
	if (EnOrDis == ENABLE) {
		if (IRQNumber <= 31) {
			*NVIC_ISER0 |= (1 << IRQNumber);
		}else if (IRQNumber >= 31 && IRQNumber < 64) {
			*NVIC_ICER1 |= (1 << (IRQNumber % 32));
		}else if (IRQNumber >= 64 && IRQNumber < 96) {
			*NVIC_ICER2 |= (1 << (IRQNumber % 64));
		}
	}else {
		if (IRQNumber <= 31) {
			*NVIC_ICER0 |= (1 << IRQNumber);
		}else if (IRQNumber >= 31 && IRQNumber < 64) {
			*NVIC_ICER1 |= (1 << (IRQNumber % 32));
		}else if (IRQNumber >= 64 && IRQNumber < 96) {
			*NVIC_ICER2 |= (1 << (IRQNumber % 64));
		}
	}
}
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority) {
	uint8_t ipr = IRQNumber / 4;
	uint8_t irq = IRQNumber % 4;
	*(NVIC_PR_BASE_ADDR + ipr) |= (IRQPriority << (8*(irq) + 4));
}
void GPIO_IRQHandling(uint8_t PinNumber) {
	if (EXTI->PR & (1 << PinNumber)) {
		EXTI->PR |= (1 << PinNumber);
	}
}
