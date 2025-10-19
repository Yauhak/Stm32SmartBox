#include "stm32f10x.h" 

#define SS_PIN 0
#define SCK_PIN 0
#define MOSI_PIN 0
#define MISO_PIN 0

#define SPI_GPIO_O 0
#define SPI_GPIO_I 0

void MySPI_W_SS(uint8_t BitValue){
	GPIO_WriteBit(GPIOA, SS_PIN, (BitAction)BitValue);
}

void MySPI_W_SCK(uint8_t BitValue){
	GPIO_WriteBit(GPIOA, SCK_PIN, (BitAction)BitValue);
}

void MySPI_W_MOSI(uint8_t BitValue){
	GPIO_WriteBit(GPIOA, MOSI_PIN, (BitAction)BitValue);
}

uint8_t MySPI_R_MISO(){
	return GPIO_ReadInputDataBit(GPIOA, MISO_PIN);
}

void MySPI_Init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = SS_PIN | SCK_PIN | MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO_O, &GPIO_InitStructure);				
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = MISO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO_I, &GPIO_InitStructure);				
	
	MySPI_W_SS(1);										
	MySPI_W_SCK(0);										
}

void MySPI_Start(){
	MySPI_W_SS(0);
}

void MySPI_Stop(){
	MySPI_W_SS(1);
}

uint8_t MySPI_SwapByte(uint8_t ByteSend){
	uint8_t i, ByteReceive = 0x00;
	for (i = 0; i < 8; i ++){
		MySPI_W_MOSI(!!(ByteSend & (0x80 >> i)));	
		MySPI_W_SCK(1);	
		if (MySPI_R_MISO())
			ByteReceive |= (0x80 >> i);
		MySPI_W_SCK(0);
	}
	return ByteReceive;
}
