#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

void DelayMs(unsigned int time);
void Led_Init();
void Button_Init();
void Exti_Config();
void NVIC_Config();

int main(void){
	Led_Init();
	Button_Init();
	Exti_Config();
	NVIC_Config();
	while(1){
		DelayMs(100000);
	}
}

void DelayMs(unsigned int time){
	int i, j;
	for(i = 0; i < time; i++){
		for(j = 0; j < 0x2aff; j++);
	}
}

void Led_Init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIOA->CRL = 0x33333333;
}

void Button_Init(){
	GPIO_InitTypeDef Button;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	Button.GPIO_Mode = GPIO_Mode_IPU;
	Button.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_14;
	Button.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&Button);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
}

void Exti_Config(){
	EXTI_InitTypeDef exti;
	exti.EXTI_Line = EXTI_Line14 | EXTI_Line12;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);
}
void NVIC_Config(){
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

void EXTI15_10_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line12) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line12);
		GPIOA->ODR = 0xff;
	}
	else if(EXTI_GetITStatus(EXTI_Line14) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line14);
		GPIOA->ODR = 0x00;
	}
}