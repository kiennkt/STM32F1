#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM


// Hien thi dong ho dem giay sd timer voi LED 7 doan CA
// cau hinh chan A7 A6 A5 A4 | A3 A2 A1 A0
// cho moi led   dp g  f  e  | d  c  b  a

//                          s   		ms
// Cau hinh chan cho 4 led  D1 D2 | D3  D4
//													A8 A9 | A10 A11
unsigned char LED[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90}; // Tao Mang LED Khi Khong Su Dung Dot Point 
unsigned char LED_DP[] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10}; // Tao Mang LED Khi Su Dung Dot Point 

void Config_LED();
void Config_Button();
void Config_Timer();
void Delay_ms();
void Delay_n_1ms(unsigned int time);
void Xuly();
void Scanning_LED();


int main(){
	Config_LED();
//	Config_Button();
	Config_Timer();
	while(1){
		Xuly();
	}
}

GPIO_InitTypeDef GPIO_LED;
// cau hinh chan cho led 2 so
void Config_LED(){
//	RCC->APB2ENR |= (1<<2);
//	GPIOA->CRL = 0x33333333;
//	GPIOA->CRH = 0x00003333;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_LED.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_LED.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3|
											GPIO_Pin_4 | GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7|
											GPIO_Pin_8| GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_LED);
}

// cau hinh chan cho 2 button
void Config_Button(){
	// cau hinh chan C13;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_LED.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_LED.GPIO_Pin = GPIO_Pin_13;
	GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_LED);
	// cau hinh chan B0;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_LED.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_LED.GPIO_Pin = GPIO_Pin_0;
	GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_LED);
}
// Ham cau hinh cho timer 1. Vi 1ms = 1s (tuc dem 1000 lan ms = 1s => dem 100 lan can 0.1ms) do chi co 2 so bieu dien dem ms
void Config_Timer(){
	TIM_TimeBaseInitTypeDef Timer;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	Timer.TIM_CounterMode = TIM_CounterMode_Up;
	Timer.TIM_Period = 65535;
	Timer.TIM_Prescaler = 71;
	TIM_TimeBaseInit(TIM1, &Timer);
}

// ham delay 0.1ms => Ft = 10Khz => PR = 100;
void Delay_ms(){
	TIM_Cmd(TIM1, ENABLE);
	TIM_SetCounter(TIM1, 0);
	while(TIM_GetCounter(TIM1) < 100); 
	TIM_Cmd(TIM1, DISABLE); 
}

// ham delay n 0.1ms
void Delay_n_1ms(unsigned int time){
	while(time--){
		Delay_ms();
	}
}

// ham xuly
void Xuly(){
	unsigned int i, j;
	TryAgain:
	for(i = 0; i < 6000; i++){
//		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0){
//			goto TryAgain;
//		}
//		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0){
//			while(1){
//				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 1){
//					break;
//				}
//				Scanning_LED(i);
//			}
//		}
		for(j = 0; j < 24; j++){
			Scanning_LED(i);
		}
	}
}

// Ham quet LED
void Scanning_LED(int i){
				// cho LED 1 sang
		GPIO_Write(GPIOA, LED[i/1000]); // Ghi gia tri cua led vao vxl
		GPIO_SetBits(GPIOA, GPIO_Pin_8); // bat sang led
		Delay_n_1ms(1); // Delay_ms();
		GPIO_ResetBits(GPIOA, GPIO_Pin_8); // tat led
						// Cho LED 2 sang
		GPIO_Write(GPIOA, LED[(i/100)%10]);
		GPIO_SetBits(GPIOA, GPIO_Pin_9);
		Delay_n_1ms(1);  // Delay_ms();
		GPIO_ResetBits(GPIOA, GPIO_Pin_9);
						// Cho LED 3 sang
		GPIO_Write(GPIOA, LED[(i/10)%10]);
		GPIO_SetBits(GPIOA, GPIO_Pin_10);
		Delay_n_1ms(1); // Delay_ms();
		GPIO_ResetBits(GPIOA, GPIO_Pin_10);
						// Cho LED 4 sang
		GPIO_Write(GPIOA, LED[i%10]);
		GPIO_SetBits(GPIOA, GPIO_Pin_11);
		Delay_n_1ms(1); // Delay_ms();
		GPIO_ResetBits(GPIOA, GPIO_Pin_11);
}