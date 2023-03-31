#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "stdio.h"

// Truyen Chuoi: "387,192,186,610,666,342,307,419,272,996,951,709,641,424,72,413,384,335,993,23,450,443,506,328,667,650,732,16,506,622,290,32,370,322,933,73,231,251,33,689,834,14,450,758,257,689,915,483,507,623";
// Sap xep tang hoac giam dan, su dung 1 Menu de chon

void Config_UART(){
	USART_InitTypeDef UART;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	UART.USART_BaudRate = 9600; // toc do bit/s
	UART.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // Luong dieu khien
	UART.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // Che do truyen di
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
	
	// Button
	GPIO.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO.GPIO_Mode = GPIO_Mode_IPU;
	GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO);
}

void delay(unsigned int time){
	unsigned int i, j;
	for(i = 0; i < time; i++){
		for(j = 0; j < 0x2AFF; j++);
	}
}
char a[] = "387,192,186,610,666,342,307,419,272,996,951,709,641,424,72,413,384,335,993,23,450,443,506,328,667,650,732,16,506,622,290,32,370,322,933,73,231,251,33,689,834,14,450,758,257,689,915,483,507,623";
char b[1001][4]; // mang 2 chieu de luu cac so sau khi da tach sau phay va Sort

void Sort1(int n){
		int  i, j;
    for(i = 0; i < n; i++){
        for(j = n-1; j >= i; j--){
            if(strlen(b[j]) != strlen(b[i])){
                if(strlen(b[j]) < strlen(b[i])){
                    char tmp[100];
                    strcpy(tmp, b[j]);
                    strcpy(b[j], b[i]);
                    strcpy(b[i], tmp);
                }
            }
            else {
                if(strcmp(b[j], b[i]) < 0){
                char tmp[100];
                strcpy(tmp, b[j]);
                strcpy(b[j], b[i]);
                strcpy(b[i], tmp);
                }
            }
        }
    }
}

void Sort2(int n){
		int  i, j;
    for(i = 0; i < n; i++){
       for(j = n-1; j >= i; j--){
          if(strlen(b[j]) != strlen(b[i])){
             if(strlen(b[j]) > strlen(b[i])){
                char tmp[100];
                strcpy(tmp, b[j]);
                strcpy(b[j], b[i]);
                strcpy(b[i], tmp);
             }
          }
          else {
            if(strcmp(b[j], b[i]) > 0){
								char tmp[100];
                strcpy(tmp, b[j]);
                strcpy(b[j], b[i]);
                strcpy(b[i], tmp);
            }
          }
      }
  }
}

void Send_char(char c){
	USART_SendData(USART1, c);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void Send_String(char *str){
	while(*str != NULL){
		Send_char(*str++);
	}
}

int main(void){
	Config_UART();
	Config_GPIO();
	char c[1001]; // Mang 1 chieu de them cac phan tu tu mang b va cac dau ','
	char d[1001]; // Mang 1 chieu de them cac phan tu tu mang b va cac dau ','
	char MENU[] = "MENU\n";
	char sortIncre[] = "1. Sap Xep Tang Dan\n";
	char sortDecre[] = "2. Sap Xep Giam Dan\n";
	char Again[] = "Hay nhap lai lua chon 1 hoac 2\n";
	unsigned int n = 0, k = 0, p = 0, i, j;
	
	char *token = strtok(a, ",");
  while(token != NULL){
     strcpy(b[n++], token);
     token = strtok(NULL, ",");
 }
	Sort1(n);
	for(i = 0; i < n; i++){
     for(j = 0; j < strlen(b[i]); j++){
				c[k++] = b[i][j];
     }
        if(i != n-1) c[k++] = ',';
	}
	c[strlen(c)] = '\n';
	
	Sort2(n);
	for(i = 0; i < n; i++){
    for(j = 0; j < strlen(b[i]); j++){
       d[p++] = b[i][j];
    }
       if(i != n-1) d[p++] = ',';
	}
	d[strlen(d)] = '\n'; 
	
	while(1){
		Send_String(MENU);
		delay(100);
		Send_String(sortIncre);
		delay(100);
		Send_String(sortDecre);
		delay(1000);
		
		while(1){
			if(USART_ReceiveData(USART1) == '1'){
			for(i = 0; i < strlen(c); i++){
				if(USART_ReceiveData(USART1) == '2'){
					break;
					}
				USART_SendData(USART1, c[i]);
				delay(10);
				}
				break;
			}
			else if(USART_ReceiveData(USART1) == '2'){
			for(i = 0; i < strlen(d); i++){
				if(USART_ReceiveData(USART1) == '1'){
					break;
					}
				USART_SendData(USART1, d[i]);
				delay(10);
				}
				break;
			}
		}
	}
}
