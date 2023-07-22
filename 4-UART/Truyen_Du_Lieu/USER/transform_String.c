#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "string.h"

void Config_UART(){
	USART_InitTypeDef UART;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	UART.USART_BaudRate = 9600;
	UART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UART.USART_Mode = USART_Mode_Tx;
	UART.USART_Parity = USART_Parity_No;
	UART.USART_StopBits = USART_StopBits_1;
	UART.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &UART);
	USART_Cmd(USART1, ENABLE);
}

void Config_GPIO(){
	GPIO_InitTypeDef GPIO;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// Tx
	GPIO.GPIO_Pin = GPIO_Pin_9;
	GPIO.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO);
	
	// Rx
	GPIO.GPIO_Pin = GPIO_Pin_10;
	GPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO);
}
void delay(unsigned int time){
	unsigned int i, j;
	for(i = 0; i < time; i++){
		for(j = 0; j < 0x2AFF; j++);
	}
}

char A[] = "Kim Tuan Kien DZ\n";
int main(void){
	Config_UART();
	Config_GPIO();
	while(1){
		int i;
		for(i = 0; i < srtlen(A); i++){
			USART_SendData(USART1, A[i]);
			delay(100);
		}
	}
}
