#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

void delay(unsigned int time);
void Led_Init();
void Led_Blink();
void Led_Increase();
void Led_Increase_1();
void Led_Decrease();
void Led_Decrease_1();

int main(void){
	Led_Init();
	while(1){
//		Led_Blink();
		Led_Decrease();
		Led_Increase();
//		Led_Increase_1();
//		Led_Decrease_1();
	}
}

void delay(unsigned int time){
	unsigned int i, j;
	for(i = 0; i < time; ++i){
		for(j = 0; j < 0x2aff; ++j);
	}
}

void Led_Init(){
	//	RCC->APB2ENR |= (1<<2);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// Su dung thanh ghi
	GPIOA->CRL = 0x33333333;
	// Su dung thu vien
//	GPIO_InitTypeDef LED;
//	LED.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3|
//							   GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//	LED.GPIO_Mode = GPIO_Mode_Out_PP;
//	LED.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &LED);
}

void Led_Blink(){
		// Su dung thanh ghi
		GPIOA->ODR = 0x00ff;
		delay(100);
		GPIOA->ODR = 0x0000;
		delay(100);
			// hoac
//		GPIOA->BSRR = 0x000000ff;
//		delay(100);
//		GPIOA->BRR = 0x000000ff;
//		delay(100);
		// Su dung thu vien
//		GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3|
//												GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
//		delay(100);
//		GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3|
//													GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
//		delay(100);
			// hoac sd bit action
//		GPIO_WriteBit(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3|
//												 GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, Bit_SET);
//		delay(100);
//		GPIO_WriteBit(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3|
//												 GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, Bit_RESET);
//		delay(100);
}

unsigned int LED[9] = {0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x00};
void Led_Increase(){
	int i;
	for(i = 8; i >= 0; i--){
		GPIOA->ODR = LED[i];
		delay(100);
	}
}

void Led_Decrease(){
	int i;
	for(i = 0; i < 8; i++){
		GPIOA->ODR = LED[i];
		delay(100);
	}
}

void Led_Increase_1(){
	int i = 0;
	GPIOA->ODR = 0x00;
	for(i = 0; i < 8; i++){
		delay(100);
		GPIOA->ODR <<= 1;
		GPIOA->ODR |= 1;
	}
	delay(100);
}

void Led_Decrease_1(){
	int i = 0;
	GPIOA->ODR = 0xff;
	for(i = 0; i < 8; i++){
		GPIOA->ODR >>= 1;
		GPIOA->ODR |= 1;
		delay(100);
	}
}
