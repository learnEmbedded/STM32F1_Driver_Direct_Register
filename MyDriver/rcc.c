#include "rcc.h"

uint16_t AHB_PreScaler[8] = {2, 4, 8, 16, 64, 128, 256, 512};
uint16_t APB1_PreScaler[4] = {2, 4, 8, 16};


uint32_t RCC_GetPCLK1Value() {
	uint32_t pclk1, SystemClk;
	uint8_t clksrc, temp, ahbp, apb1p;
	
	clksrc = (( RCC->CFGR >> 2) & 0x03);
	
	if (clksrc == 0) {
		SystemClk = 16000000;
	}else if (clksrc == 1) {
		SystemClk = 8000000;
	}
	
	temp = (( RCC->CFGR >> 4) & 0xF);
	if (temp < 8) {
		ahbp = 1;
	}else {
		ahbp = AHB_PreScaler[temp - 8];
	}
	
	temp = (( RCC->CFGR >> 10) & 0x7);
	if (temp < 4) {
		ahbp = 1;
	}else {
		ahbp = APB1_PreScaler[temp - 4];
	}
	
	pclk1 = (SystemClk / ahbp) / apb1p;
	
	return pclk1;
}

uint32_t RCC_GetPCLK2Value() {
	uint32_t SystemClock = 0, temp, pclk2;
	uint8_t ahbp, apb2p;
	
	uint8_t clk_src = (RCC->CFGR >> 2) & 0x03;
	
	if (clk_src == 0) {
		SystemClock = 16000000;
	}else {
		SystemClock = 8000000;
	}
	
	temp = (RCC->CFGR >> 4) & 0xF;
	if (temp < 0x08) {
		ahbp = 1;
	}else {
		ahbp = AHB_PreScaler[temp-8];
	}
	
	temp = (RCC->CFGR >> 13) & 0x7;
	if (temp < 0x04) {
		apb2p = 1;
	}else {
		apb2p = APB1_PreScaler[temp - 4];
	}
	
	pclk2 = (SystemClock / ahbp) / apb2p;
	return pclk2;
}