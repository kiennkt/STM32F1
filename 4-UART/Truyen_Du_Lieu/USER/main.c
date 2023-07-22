#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "string.h"


void Config_UART(){
	USART_InitTypeDef UART;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	UART.USART_BaudRate = 9600; // toc do bit/s
	UART.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // Luong dieu khien
	UART.USART_Mode = USART_Mode_Tx; // Che do truyen di
	UART.USART_Parity = USART_Parity_No; // duoc them vao de xac dinh tong so bit trong du lieu truyen
																			// + parity bit la 1 so chan(even) or le(odd)
																			// VD: lap trinh data chan: 10111011 thi parity bit duoc them vao de
																			// tong so bit 1 la chan. TH parity bit le ngc lai
	UART.USART_StopBits = USART_StopBits_1;	// Bit cuoi cung cua Frame, bao hieu Data da dc gui xong
	UART.USART_WordLength = USART_WordLength_8b; // do dai bit truyen
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
char A[] = "Kim Tuan Kien\n";

int main(void){
	Config_UART();
	Config_GPIO();	
	delay(100);
	while(1){
		int i;
		for(i = 0; i < strlen(A); i++){
			USART_SendData(USART1, A[i]);
			delay(50);
		}
	}
}
