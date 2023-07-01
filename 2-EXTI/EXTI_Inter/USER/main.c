#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

// Bai toan: Dieu khien Led bang nut nhan, bam nut 1 thi led se nhap nhay theo so lan duoc bam
					// Bam nut 2 thi led tat, bam tiep led lai tiep tuc duoc nhay
					
void Delay_Ms(unsigned int time);
void Led_Config();
void Button_Config();
void Exti_Config();
void NVIC_Config();

int main(void){
	Led_Config();
	Button_Config();
	Exti_Config();
	NVIC_Config();
	while(1){
		Delay_Ms(10000);
	}
}

void Delay_Ms(unsigned int time){
	int i, j;
	for(i = 0; i < time; i++){
		for(j = 0; j < 0x2aff; j++);
	}
}

void Led_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIOA->CRL = 0x33333333;
}

void Button_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef Button;
	Button.GPIO_Mode = GPIO_Mode_IPU;
	Button.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	Button.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &Button);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);
}

void Exti_Config(){
	EXTI_InitTypeDef Exti;
	Exti.EXTI_Line = EXTI_Line6 | EXTI_Line7;
	Exti.EXTI_Mode = EXTI_Mode_Interrupt;
	Exti.EXTI_Trigger = EXTI_Trigger_Falling;
	Exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&Exti);
}

void NVIC_Config(){
	NVIC_InitTypeDef nvic;
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	nvic.NVIC_IRQChannel = EXTI9_5_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

uint16_t count = 0;
uint16_t count1 = 0;

void EXTI9_5_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line6) != RESET){
		count++;
		int i;
		for(i = 1; i <= count; i++){
			GPIOA->ODR = 0xff;
			Delay_Ms(100);
			GPIOA->ODR = 0x00;
			Delay_Ms(100);
		}
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	else if(EXTI_GetITStatus(EXTI_Line7) != RESET){
		count1++;
		int i;
		for(i = 1; i <= count1; i++){
			GPIOA->ODR = 0xaa;
			Delay_Ms(100);
			GPIOA->ODR = 0x55;
			Delay_Ms(100);
			GPIOA->ODR = 0x00;
		}
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}
