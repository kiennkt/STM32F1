#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

void Led_Config();
void Timer();

int main(void){
	Led_Config();
	Timer();
	while(1){
		// Delay_Ms(100); Khong phu thuoc vao ham delay
	}
}

void Led_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIOA->CRL = 0x33333333;
}

// Timing Chu ky:0.5s => Ft = 2 (voi Fs = 72Mhz, PSC < 65525)
// Ft = Fs/(PSC + 1)(PR + 1)
void Timer(void){
	TIM_TimeBaseInitTypeDef Timer;
	NVIC_InitTypeDef NVIC_Config;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	Timer.TIM_Period = 9999;
	Timer.TIM_Prescaler = 3599;
	Timer.TIM_RepetitionCounter = 0;
	Timer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&Timer);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_Config.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_Config.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Config.NVIC_IRQChannelSubPriority = 0;
	NVIC_Config.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Config);
//	NVIC_EnableIRQ(TIM2_IRQn); // kich hoat ngat cho kenh ngat TIM2
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQHandler(){
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)!=RESET){		
		GPIOA->ODR ^= 0xff; // Dao trang thai
	}	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}
