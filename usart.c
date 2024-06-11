#include "usart.h"
#include "stm32f1.h"

void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDis) {
	if (EnOrDis == ENABLE) {
		if (pUSARTx == USART1) {
			USART1_PCLK_EN();
		}else if (pUSARTx == USART2) {
			USART2_PCLK_EN();
		}else if (pUSARTx == USART3) {
			USART3_PCLK_EN();
		}else if (pUSARTx == UART4) {
			UART4_PCLK_EN();
		}else if (pUSARTx == UART5) {
			UART5_PCLK_EN();
		}
	}
}

void USART_Init(USART_Handle_t *pUSARTHandle) {
	uint32_t tempreg = 0;
	
	USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);
	
	// Mode
	if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX) {
		tempreg |= ( 1 << USART_CR1_RE);
	}else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX) {
		tempreg |= ( 1 << USART_CR1_TE);
	}else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX) {
		tempreg |= (( 1 << USART_CR1_RE) | ( 1 << USART_CR1_TE));
	}
	
	// Word Length
	tempreg |= pUSARTHandle->USART_Config.USART_WordLength << USART_CR1_M;
	
	// Parity
	if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN) {
		tempreg |= ( 1 << USART_CR1_PCE);
	}else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD) {
		tempreg |= ( 1 << USART_CR1_PCE);
		tempreg |= ( 1 << USART_CR1_PS);
	}
	
	pUSARTHandle->pUSARTx->CR1 = tempreg;
	
	tempreg = 0;
	
	tempreg |= pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP;
	pUSARTHandle->pUSARTx->CR2 = tempreg;
	
	tempreg = 0;
	
	if (pUSARTHandle->USART_Config.USART_HWFloUControl == USART_HW_FLOW_CTRL_CTS) {
		tempreg |= ( 1 << USART_CR3_CTSE);
	}else if (pUSARTHandle->USART_Config.USART_HWFloUControl == USART_HW_FLOW_CTRL_RTS) {
		tempreg |= ( 1 << USART_CR3_RTSE);
	}else if (pUSARTHandle->USART_Config.USART_HWFloUControl == USART_HW_FLOW_CTRL_CTS_RTS) {
		tempreg |= (( 1 << USART_CR3_CTSE) | ( 1 << USART_CR3_RTSE));
	}
	
	pUSARTHandle->pUSARTx->CR3 = tempreg;
	
	USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->USART_Config.USART_Baud);
}