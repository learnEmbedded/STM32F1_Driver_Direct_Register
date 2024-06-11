#include "gpio.h"
#include "stm32f1.h"
#include "stdint.h"

void GPIO_Config(void) {
	GPIO_Handle_t GPIO_Test;
	GPIO_Test.pGPIOx = GPIOC;
	GPIO_Test.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Test.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT_2MHz|GPIO_MODE_OUT_PP;
	GPIO_Test.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU ;
	
	_GPIO_Init(&GPIO_Test);
}
void delay(void) {
    for (uint32_t i = 0; i < 500000; i++);
}


int main(void)
{
	GPIO_Config();

	while (1) {
		// GPIO_ToggleOutputPin(GPIOC, GPIO_PIN_NO_13);
		GPIO_WriteToOutputPin(GPIOC, GPIO_PIN_NO_13, 0);
		delay();
		GPIO_WriteToOutputPin(GPIOC, GPIO_PIN_NO_13, 1);
		delay();
	}
	return 0;
}
	