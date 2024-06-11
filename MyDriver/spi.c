#include "spi.h"

static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle);

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDis) {

	if (EnOrDis == ENABLE){
		if (pSPIx == SPI1){
			SPI1_PCLK_EN();
		}else if (pSPIx == SPI2){
			SPI2_PCLK_EN();
		}else if (pSPIx == SPI3){
			SPI3_PCLK_EN();
		}
	}else {
		if (pSPIx == SPI1){
			SPI1_PCLK_DI();
		}else if (pSPIx == SPI2){
			SPI2_PCLK_DI();
		}else if (pSPIx == SPI3){
			SPI3_PCLK_DI();
		}
	}
}

void SPI_Init(SPI_Handle_t *pSPIHandle) {
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);
	uint32_t tempreg = 0;
	 
	//1. Configure the device mode.
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;
	
	//2. Configure the bus config.
	if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD) {
		tempreg &=~ (1 << SPI_CR1_BIDIMODE);
	}else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD) {
		tempreg |= (1 << SPI_CR1_BIDIMODE);
	}else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY) {
		tempreg &=~ (1 << SPI_CR1_BIDIMODE);
		
		tempreg |= (1 << SPI_CR1_RXONLY);
	}
	//3. Configure the spi serial clock speed (baud rate).
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;
	//4. Configure the data frame format.
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;
	//5. Configure the CPOL.
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;
	//6. configure the CPHA.
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;
	//
	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;
	
	pSPIHandle->pSPIx->CR1 = tempreg;
}
  
void SPI_DeInit(SPI_RegDef_t *pSPIx) {

}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName) {
	if (pSPIx->SR & FlagName) {
		return FLAG_SET;
	}
	return FLAG_RESET; 
}

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t length) {
	while (length > 0) {
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);
		
		if ((pSPIx->CR1 & (1 << SPI_CR1_DFF))) {
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			length--;
			length--;
			(uint16_t*)pTxBuffer++;
		}else {
			pSPIx->DR = *pTxBuffer;
			length--;
			pTxBuffer++;
		}
	}
}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t length) {
	while (length > 0) {
		while (SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == (uint8_t)FLAG_RESET);
		
		if (pSPIx->CR1 & (1 << SPI_CR1_DFF)) {
			*((uint16_t*)pRxBuffer) = pSPIx->DR;
			length--;
			length--;
			(uint16_t*)pRxBuffer++;
		}else {	
			*(pRxBuffer) = pSPIx->DR;
			length--;
			pRxBuffer++;
		}
	}
}

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDis) {
	if (EnOrDis == ENABLE) {
		pSPIx->CR1 |= ( 1<< SPI_CR1_SPE);
	}else {
		pSPIx->CR1 &=~ ( 1 << SPI_CR1_SPE);
	}
}
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDis) {
	if (EnOrDis == ENABLE) {
		pSPIx->CR1 |= ( 1<< SPI_CR1_SSI);
	}else {
		pSPIx->CR1 &=~ ( 1 << SPI_CR1_SSI);
	}
}

void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDis) {
	if (EnOrDis == ENABLE) {
		pSPIx->CR2 |= ( 1<< SPI_CR2_SSOE);
	}else {
		pSPIx->CR2 &=~ ( 1 << SPI_CR2_SSOE);
	}
}
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDis){
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
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority) {
	uint8_t ipr = IRQNumber / 4;
	uint8_t irq = IRQNumber % 4;
	*(NVIC_PR_BASE_ADDR + ipr) |= (IRQPriority << (8*(irq) + 4));
}

void SPI_IRQHandling(SPI_Handle_t *pHandle) {
	uint8_t	 temp1, temp2;
	
	temp1 = pHandle->pSPIx->SR & ( 1 << SPI_SR_TXE);
	temp2 = pHandle->pSPIx->CR2 & ( 1 << SPI_CR2_TXEIE);
	if (temp1 && temp2) {
		spi_txe_interrupt_handle(pHandle);
	}
	
	temp1 = pHandle->pSPIx->SR & ( 1 << SPI_SR_RXNE);
	temp2 = pHandle->pSPIx->CR2 & ( 1 << SPI_CR2_RXNEIE);
	if (temp1 && temp2) {
		spi_rxne_interrupt_handle(pHandle);
	}
	
	temp1 = pHandle->pSPIx->SR & ( 1 << SPI_SR_OVR);
	temp2 = pHandle->pSPIx->CR2 & ( 1 << SPI_CR2_ERRIE);
	if (temp1 && temp2) {
		spi_ovr_err_interrupt_handle(pHandle);
	}
}

static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle) {

		if ((pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))) {
			pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxBuffer);
			pSPIHandle->TxLength--;
			pSPIHandle->TxLength--;
			(uint16_t*)pSPIHandle->pTxBuffer++;
		}else {
			pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
			pSPIHandle->TxLength--;
			pSPIHandle->pTxBuffer++;
		}
		
		if (! pSPIHandle->TxLength) {
			SPI_CloseTransmission(pSPIHandle);
			SPI_ApplicationEventCallBack(pSPIHandle, SPI_EVENT_TX_CMPLT);
		}
}

void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle) {
	pSPIHandle->pSPIx->CR2 &=~ ( 1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLength = 0;
	pSPIHandle->TxState = SPI_READY;
}

static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle) {
	if ((pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))) {
			*((uint16_t*)pSPIHandle->pRxBuffer) = (uint16_t)pSPIHandle->pSPIx->DR;
			pSPIHandle->RxLength--;
			pSPIHandle->RxLength--;
			pSPIHandle->pRxBuffer++;
			pSPIHandle->pRxBuffer++;
		}else {
			*pSPIHandle->pRxBuffer = (uint8_t)pSPIHandle->pSPIx->DR;
			pSPIHandle->RxLength--;
			pSPIHandle->pRxBuffer++;
		}
		
		if (! pSPIHandle->RxLength) {
			SPI_CloseReception(pSPIHandle);
			SPI_ApplicationEventCallBack(pSPIHandle, SPI_EVENT_RX_CMPLT);
		}
}

void SPI_CloseReception(SPI_Handle_t *pSPIHandle) {
	pSPIHandle->pSPIx->CR2 &=~ ( 1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLength = 0;
	pSPIHandle->RxState = SPI_READY;
}

static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle) {
	uint8_t temp;
	if (pSPIHandle->TxState != SPI_BUSY_IN_TX) {
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;
	}
	
	(void)temp;
	SPI_ApplicationEventCallBack(pSPIHandle, SPI_EVENT_OVR_ERR);
}

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx) {
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;
}

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t length) {
	uint8_t state = pSPIHandle->TxState;
	if (state != SPI_BUSY_IN_TX) {
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLength = length;
			
		pSPIHandle->TxState = SPI_BUSY_IN_TX;
			
		pSPIHandle->pSPIx->CR2 |= ( 1 << SPI_CR2_TXEIE);
	}
	return state;
}
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t length) {
	uint8_t state = pSPIHandle->RxState;
	if (state != SPI_BUSY_IN_RX) {
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLength = length;
			
		pSPIHandle->RxState = SPI_BUSY_IN_RX;
			
		pSPIHandle->pSPIx->CR2 |= ( 1 << SPI_CR2_RXNEIE);
	}
	return state;
}

__attribute__((weak)) void SPI_ApplicationEventCallBack(SPI_Handle_t *pSPIHandle, uint8_t AppEv) {


}



