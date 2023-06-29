#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC


void Delay_ms(unsigned int time);
void Led_Init();
void Button_Init();
void Press_Button(); // Ham nhan nut thi cac led tat, bo ra lai sang
void On_Off_Button(); // Ham nhan nut thi tat, nhan lai thi sang
void Scan_Button(); // Ham cho nhan n lan thi n led nhap nhay (n<=8)

int main(void){
	Led_Init();
	Button_Init();
	while(1){
		//Press_Button();
		//On_Off_Button();
		Scan_Button();
	}
}
// Ham delay ms
void Delay_ms(unsigned int time){
	int i, j;
	for(i = 0; i < time; i++){
		for(j = 0; j < 0x2aff; j++);
	}
}
// Ham cau hinh chan led
void Led_Init(){
	GPIO_InitTypeDef GPIO_LED;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_LED.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3|
											GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7;
	GPIO_LED.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_LED);
	// Hoac su dung 2 thanh ghi CRL de cau hinh chan( vi su dung cac chan tu 0->7)
//	GPIOA->CRL = 0x33333333;
//	GPIOA->ODR = 0x00ff;
}

// Ham cau hinh chan cho button
void Button_Init(){
	GPIO_InitTypeDef GPIO_BUTTON;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_BUTTON.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
	GPIO_BUTTON.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_BUTTON.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_BUTTON);
	// Hoac su dung thanh ghi CRL va CRH de cau hinh chan
//	GPIOB->CRL = 0x44000000;
//	GPIOB->CRH = 0x00000044;
}

void Press_Button(){
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)){
		GPIOA->ODR = 0x0000; //GPIOA->BRR = 0x00ff;
	}
	else GPIOA->ODR = 0x00ff; //GPIOA->BSRR = 0x00ff;
}

uint8_t sttNew = 1, sttOld = 1;
unsigned int Count = 0;
void On_Off_Button(){
	sttOld = sttNew ;
	sttNew = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
	if(sttOld == 1 && sttNew == 0){
		Count++;
	}
	if(Count%2 == 0){
		GPIOA->ODR = 0x00ff;
	}
	else GPIOA->ODR = 0x0000;
	Delay_ms(1);
}

unsigned int cnt = 0, check_flag = 0;
void Scan_Button(){
	sttOld = sttNew;
	sttNew = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8);
	if(sttOld == 1 && sttNew == 0){
		check_flag++;
	}
	if(check_flag <= 8){
		cnt++; 
		// 200 lan dau tien check_flag bit dau se o trang thai '0'
		if(cnt <= 200){
			GPIOA->ODR = 0x00ff << check_flag;
		}
		else { // 200 lan tiep theo tat ca cac chan deu o trang thai '1'
			GPIOA->ODR = 0x00ff;
			if(cnt >= 400) cnt = 0;
		}
	}
	else check_flag = 0;
	Delay_ms(1);
}