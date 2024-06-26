#ifndef USART_H_
#define USART_H_

#include "stm32f1.h"

// structure for USART peripheral
typedef struct {
	uint8_t	USART_Mode;
	uint32_t USART_Baud;
	uint8_t	USART_NoOfStopBits;
	uint8_t	USART_WordLength;
	uint8_t USART_ParityControl;
	uint8_t USART_HWFloUControl;
} USART_Config_t;

typedef struct {
	USART_RegDef_t *pUSARTx;
	USART_Config_t USART_Config;
	uint8_t	*pTxBuffer;
	uint8_t	*pRxBuffer;
	uint32_t TxLength;
	uint32_t RxLength;
	uint8_t	TxBusyState;
	uint8_t	RxBusyState;
}	USART_Handle_t;

// Possible option for USART_Mode
#define USART_MODE_ONLY_TX		0
#define USART_MODE_ONLY_RX		1
#define USART_MODE_TXRX				2

// USART_Baud
#define USART_STD_BAUD_1200                1200
#define USART_STD_BAUD_2400                2400
#define USART_STD_BAUD_4800                4800
#define USART_STD_BAUD_9600                9600
#define USART_STD_BAUD_14400               14400
#define USART_STD_BAUD_19200               19200
#define USART_STD_BAUD_38400               38400
#define USART_STD_BAUD_57600               57600
#define USART_STD_BAUD_115200              115200
#define USART_STD_BAUD_230400              230400
#define USART_STD_BAUD_460800              460800
#define USART_STD_BAUD_921600              921600
#define USART_STD_BAUD_2M				           2000000
#define USART_STD_BAUD_3M				           3000000

// USART_ParityControl 
#define USART_PARITY_EN_ODD		2
#define USART_PARITY_EN_EVEN	1
#define USART_PARITY_DISABLE	0

//	WordLength
#define USART_WORDLENGTH_8BITS		0
#define USART_WORDLENGTH_9BITS		1

// NoOfStopB�t
#define USART_STOPBITS_1		0
#define USART_STOPBITS_0_5	1
#define USART_STOPBITS_2		2
#define USART_STOPBITS_1_5	3

// HWFlowControl
#define USART_HW_FLOW_CTRL_NONE			0
#define USART_HW_FLOW_CTRL_CTS			1
#define USART_HW_FLOW_CTRL_RTS			2
#define USART_HW_FLOW_CTRL_CTS_RTS	3

// USART flag
#define USART_FLAG_TXE			( 1 << USART_SR_TXE)
#define USART_FLAG_RXNE			( 1 << USART_SR_RXNE)
#define USART_FLAG_TC				( 1 << USART_SR_TC)

// Application states
#define USART_BUSY_IN_RX		1
#define USART_BUSY_IN_TX		2
#define USART_READY					0

#define USART_EVENT_TX_COMPLT			0
#define USART_EVENT_RX_COMPLT			1
#define USART_EVENT_IDLE					2
#define USART_EVENT_CTS						3
#define USART_EVENT_PE						4
#define USART_ERR_FE							5
#define USART_ERR_NE							6
#define USART_ERR_ORE							7

/*
 *	Peripheral Clock setup
 */
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDis);

/*
 *	Init and De-init
 */
void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_Handle_t *pUSARTHandle);

/*
 *	Data Send and Receive 
 */
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Length);
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Length);
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Length);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Length);

/*
 *	IRQ Configuration and IRS Handling
 */
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDis);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void USART_IRQHandling(USART_Handle_t *pUSARTHande);

/*
 *	Other Peripheral Control APIs
 */
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint8_t StatusFlagName);
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName);
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDis);
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate);

/*
 *	Application Callbacks
 */
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle, uint8_t ApEv);







#endif