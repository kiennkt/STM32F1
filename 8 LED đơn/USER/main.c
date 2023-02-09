#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC


// Cau hinh chan cho GPIO(pc13)
GPIO_InitTypeDef GPIO;

void Config(void){
	GPIO.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3|
									GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO);
}

void delay(unsigned int time){
	unsigned int i, j;
	for(i = 0; i < time; ++i){
		for(j = 0; j < 0x2aff; ++j);
	}
}
// mang sang tung led
unsigned int blinkeachled[] = {0xff, 0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};

// mang sang dan cac led
unsigned int sangdanled[] = {0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x00};

int main(void){
	// cap xung Clock cho chan C
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	Config();
	while(1){
		//Cach 1
//		GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3|
//									GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
//		delay(100);
//		GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3|
//									GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
//		delay(100);
		
		// Cach 2
//		GPIO_WriteBit(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3|
//									GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, Bit_RESET);
//		delay(50);
//		GPIO_WriteBit(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3|
//									GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, Bit_SET);
//		delay(50);
		
		
		// cho cac led sang dan
		/*
		1111 1111 = 0xff
		1111 1110 = 0xfe
		1111 1101 = 0xfd
		1111 1011 = 0xfb
		1111 0111 = 0xf7
		1110 1111 = 0xef
		1101 1111 = 0xdf
		1011 1111 = 0xbf
		0111 1111 = 0x7f
		*/
//		int i;
//		for (i = 0; i < 9; i++){
//			GPIOA->ODR = blinkeachled[i];
//			delay(50);
//		}
		
		/*
		sang dan cac led
		1111 1111 = 0xff
		1111 1110 = 0xfe
		1111 1100 = 0xfc
		1111 1000 = 0xf8
		1111 0000 = 0xf0
		1110 0000 = 0xe0
		1100 0000 = 0xc0
		1000 0000 = 0x80
		0000 0000 = 0x00
		*/
//		int i;
//		for (i = 0; i < 9; i++){
//			GPIOA->ODR = sangdanled[i];
//			delay(50);
//		}
		GPIOA->ODR = 0xAA;
		delay(50);
		GPIOA->ODR = 0x55;
		delay(50);
	}
}
