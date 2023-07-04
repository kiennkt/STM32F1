#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

void Led_Config();
void Timer();

int main(void){
	Led_Config();
	Timer();
	while(1){
		
	}
}

void Led_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIOA->CRL = 0x33333333;
}


void Timer(void){
	TIM_TimeBaseInitTypeDef Timer;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	Timer.TIM_Period = 9999;
	Timer.TIM_Prescaler = 3599;
	Timer.TIM_RepetitionCounter = 0;
	Timer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&Timer);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQHandler(){
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)!=RESET){		
		GPIOA->ODR ^= 0xff;
	}	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}
