
#include "delay.h"

/* Configure Timer 2 */

extern 	TIM_TimeBaseInitTypeDef timerInit;

void Config_Timer(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	timerInit.TIM_CounterMode = TIM_CounterMode_Up;
	timerInit.TIM_Period = 0xFFFF;
	timerInit.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInit(TIM2, &timerInit);
	TIM_Cmd(TIM2, ENABLE);
}

/* Initialization delay miniseconds function */

void Delay_Ms(unsigned int time){
	while(time--){
			TIM_SetCounter(TIM2, 0);
			while(TIM_GetCounter(TIM2) < 1000);
	}
}

/* Initialization delay microseconds function */

void Delay_Us(unsigned int time){
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < time);
}
