#include "usart.h"
#include "stm32f1.h"
#include "rcc.h"
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

void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate) {
	uint32_t PCLKx;
	uint32_t usartdiv;
	uint32_t M_part, F_part;
	uint32_t tempreg = 0;

	// 1. Get the clock value
	if (pUSARTx == USART1) {
			PCLKx = RCC_GetPCLK2Value();
	} else {
			PCLKx = RCC_GetPCLK1Value();
	}

	// 2. Calculate the USARTDIV value
	usartdiv = (PCLKx * 25) / (4 * BaudRate);
	
	// 3. Extract the mantissa part
	M_part = usartdiv / 100;
	
	// 4. Extract the fractional part
	F_part = usartdiv - (M_part * 100);
	
	// 5. Calculate the final fractional part
	F_part = ((F_part * 16) + 50) / 100;
	
	// 6. Combine the mantissa and fractional parts
	tempreg |= (M_part << 4);
	tempreg |= (F_part & 0xF);
	
	// 7. Write to the BRR register
	pUSARTx->BRR = tempreg;
}

void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDis) {
	if (EnOrDis == ENABLE) {
		pUSARTx->CR1 |= ( 1 << 13);
	}else {
		pUSARTx->CR1 &=~ ( 1 << 13);
	}
}

uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint8_t StatusFlagName) {
	uint8_t status = RESET;
	if (pUSARTx->SR & StatusFlagName) {
		status = SET;
	}
	return status;
}

void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Length) {
	uint16_t *pdata;
	
	for (uint32_t i = 0; i < Length; i++) {
		// Wait until TXE flag is set in SR
		while (! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TXE))
		
		// Check the USART_WordLength item for 9BI OR 8BTI in a frame
		if (pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLENGTH_9BITS) {
			// Load the DR with 2 bytes
			pdata = (uint16_t*)pTxBuffer;
			pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);
			
			// Check for USART_ParityControl
			if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE) {
				// Bits off user data will be sent
				pTxBuffer++;
				pTxBuffer++;
			}else {
				pTxBuffer++;
			}
			
		}else { // USART_WordLength
			// This is 8bit data transfer
			pUSARTHandle->pUSARTx->DR = (*pTxBuffer & (uint8_t)0xFF);
			
			// Increament the buffer address
			pTxBuffer++;
		}		
	}
	
	// While till TC flag is set in the SR
	while (!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TC));
}