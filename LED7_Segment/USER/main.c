#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC


void Config_LED();
void Config_Button();
void Config_Timer();
void Delay_n_1ms();
void Delay_1ms();
void Xuly();
void Scanning_LED();

// Tao 1 mang luu cac gia tri so tu 0-> o dang he 16
unsigned char LED[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};

int main(){
	Config_LED();
	Config_Button();
	while(1){
		Xuly();
	}
}

GPIO_InitTypeDef GPIO_LED;
// cau hinh chan cho led 2 so
void Config_LED(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_LED.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_LED.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2|
									GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5| GPIO_Pin_6|
									GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11| GPIO_Pin_12;
	GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_LED);
}

// cau hinh chan cho 2 button
void Config_Button(){
	// cau hinh chan C13;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_LED.GPIO_Mode = GPIO_Mode_IPU;
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

// Ham cau hinh cho timer 1
void Config_Timer(){
	TIM_TimeBaseInitTypeDef Timer;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	Timer.TIM_CounterMode = TIM_CounterMode_Up;
	Timer.TIM_Period = 65535;
	Timer.TIM_Prescaler = 1;
	TIM_TimeBaseInit(TIM1, &Timer);
}

// ham delay 1 ms
void Delay_1ms(){
	Config_Timer();
	TIM_Cmd(TIM1, ENABLE);
	TIM_SetCounter(TIM1, 0);
	while(TIM_GetCounter(TIM1) < 72);
	TIM_Cmd(TIM1, DISABLE);
}

// ham delay n ms
void Delay_n_1ms(unsigned int time){
	while(time--){
		Delay_1ms();
	}
}

// ham xuly
void Xuly(){
	unsigned int i, j;
	unsigned int check = 1;
	TryAgain:
	for(i = 0; i < 6000; i++){
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0){
			goto TryAgain;
		}
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0){
			while(1){
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 1){
					break;
				}
				Scanning_LED(i);
			}
		}
		for(j = 0; j < 25; j++){
			Scanning_LED(i);
		}
	}
}

// Ham quet LED
void Scanning_LED(int i){
				// cho LED 1 sang
		GPIO_Write(GPIOA, LED[i/1000]); // Ghi gia tri cua led vao vxl
		GPIO_SetBits(GPIOA, GPIO_Pin_9); // bat sang led
		Delay_n_1ms(10);
		GPIO_ResetBits(GPIOA, GPIO_Pin_9); // tat led
						// Cho LED 2 sang
		GPIO_Write(GPIOA, LED[(i/100)%10]);
		GPIO_SetBits(GPIOA, GPIO_Pin_10);
		Delay_n_1ms(10);
		GPIO_ResetBits(GPIOA, GPIO_Pin_10);
						// Cho LED 3 sang
		GPIO_Write(GPIOA, LED[(i/10)%10]);
		GPIO_SetBits(GPIOA, GPIO_Pin_11);
		Delay_n_1ms(10);
		GPIO_ResetBits(GPIOA, GPIO_Pin_11);
						// Cho LED 4 sang
		GPIO_Write(GPIOA, LED[i%10]);
		GPIO_SetBits(GPIOA, GPIO_Pin_12);
		Delay_n_1ms(10);
		GPIO_ResetBits(GPIOA, GPIO_Pin_12);
}
