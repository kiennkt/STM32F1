#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

// Hien thi dong ho dem phut sd timer voi LED 7 doan CA
// cau hinh chan A7 A6 A5 A4 | A3 A2 A1 A0
// cho moi led   dp g  f  e  | d  c  b  a

//                          phut    giay
// Cau hinh chan cho 4 led  D1 D2 | D3  D4
//													A8 A9 | A10 A11
unsigned char LED[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90}; // Tao Mang LED Khi Khong Su Dung Dot Point 
unsigned char LED_DP[] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10}; // Tao Mang LED Khi Su Dung Dot Point 
void Led_Config();
void Button_Config();
void Timer_Config();
void Led_7_Display();
void Delay_Ms(unsigned int time);
int main(void){
	Led_Config();
	Timer_Config();
	while(1){
		Led_7_Display();
	}
}
void Delay_Ms(unsigned int time){
	int i, j;
	for(i = 0; i < time; i++){
		for(j = 0; j < 0x2aff; j++);
	}
}

void Led_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // Or Use Register RCC->APB2ENR |= (1<<2);
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
									GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |
									GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
//	GPIOA->CRL = 0x33333333;
//	GPIOA->CRH = 0x00003333;
	GPIOA->ODR = 0xff;
}

unsigned int cnt = 0, second = 0, minute = 0;

void Led_7_Display(){
	if(cnt == 60){
		minute++;
		if(minute == 60){
			minute = 0;
		}
		cnt = 0;
	}
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
	GPIO_Write(GPIOA, LED[cnt%10]);
	Delay_Ms(100);
	GPIO_SetBits(GPIOA, GPIO_Pin_11);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_10);
	GPIO_Write(GPIOA, LED[cnt/10]);
	Delay_Ms(100);
	GPIO_SetBits(GPIOA, GPIO_Pin_10);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_9);
	GPIO_Write(GPIOA, LED[minute%10]);
	Delay_Ms(100);
	GPIO_SetBits(GPIOA, GPIO_Pin_9);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	GPIO_Write(GPIOA, LED[minute/10]);
	Delay_Ms(100);
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
}

// Cau hinh Timer dinh thoi 1s
void Timer_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_TimeBaseInitTypeDef timer;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_ClockDivision = 1; // giu nguyen 72Mhz
	timer.TIM_Period = 9999;
	timer.TIM_Prescaler = 7199;
	timer.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &timer);
	TIM_Cmd(TIM1, ENABLE); // cho phep TIM1 hd
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update); // TIM_ClearFlag(TIM1, TIM_IT_Update);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = TIM1_UP_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

void TIM1_IRQHandler(){
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET){
		cnt++;
	}
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
} 
