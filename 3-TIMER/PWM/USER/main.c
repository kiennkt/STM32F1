#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

void Led_Config();
void Timer();
void Delay_1Ms();
void Delay_nMs(unsigned int time);


int main(void){
	Led_Config();
	Timer();
	while(1){

	}
}

void Led_Config(){
	GPIO_InitTypeDef gpio;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC->APB2ENR |= 1<<2;
	GPIOA->CRL = 0x0000bbbb;
//	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
//	gpio.GPIO_Speed = GPIO_Speed_50MHz;
//	gpio.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3;
//	GPIO_Init(GPIOA, &gpio);
}
void Timer(void){
	TIM_TimeBaseInitTypeDef Timer;
	TIM_OCInitTypeDef Pwm;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	Timer.TIM_Period = 100-1;
	Timer.TIM_Prescaler = 71;
	Timer.TIM_RepetitionCounter = 0;
	Timer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&Timer);
	TIM_Cmd(TIM2,ENABLE);
	// chanel 1;
	Pwm.TIM_OCMode = TIM_OCMode_PWM1;
	Pwm.TIM_OCPolarity = TIM_OCPolarity_High;
	Pwm.TIM_Pulse = 70;
	Pwm.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC1Init(TIM2, &Pwm);
	// chanel 2
	Pwm.TIM_OCMode = TIM_OCMode_PWM1;
	Pwm.TIM_OCPolarity = TIM_OCPolarity_High;
	Pwm.TIM_Pulse = 40;
	Pwm.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC2Init(TIM2, &Pwm);
	// chanel 3
	Pwm.TIM_OCMode = TIM_OCMode_PWM1;
	Pwm.TIM_OCPolarity = TIM_OCPolarity_High;
	Pwm.TIM_Pulse = 50;
	Pwm.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC3Init(TIM2, &Pwm);
	// chanel 4
	Pwm.TIM_OCMode = TIM_OCMode_PWM1;
	Pwm.TIM_OCPolarity = TIM_OCPolarity_High;
	Pwm.TIM_Pulse = 75;
	Pwm.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC4Init(TIM2, &Pwm);
	
}


