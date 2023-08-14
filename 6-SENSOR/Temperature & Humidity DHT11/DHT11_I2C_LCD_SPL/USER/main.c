#include "stm32f10x.h"
#include "delay.h"
#include "lcd_i2c.h"
#include "stdio.h"

static void Config_GPIO();
static void Config_I2C();

I2C_InitTypeDef i2c1;
TIM_TimeBaseInitTypeDef timerInit;
GPIO_InitTypeDef gpioInit;
/********************* DHT11 Fuction ***************************/

#define DHT11_Data_Port GPIOA
#define DHT11_Data_Pin  GPIO_Pin_0

uint8_t Check_Response;
uint8_t Data[5] = {0};
int Temperature = 0, Humidity = 0;
/* MCU send a start signal */

void Send_Start_Signal_To_DHT11(){
	GPIO_ResetBits(DHT11_Data_Port, DHT11_Data_Pin);
	Delay_Ms(20); // delay > 18ms
	GPIO_SetBits(DHT11_Data_Port, DHT11_Data_Pin);
	Delay_Us(50); // delay 20-40 us to wait for DHT11 response
}

/* Response signal of DHT11 */

uint8_t Check_DHT11_Response(){
	uint8_t Check = 0;
	if(!GPIO_ReadInputDataBit(DHT11_Data_Port, DHT11_Data_Pin)){
		Delay_Us(100);
		// if after delay 100 us, response signal at high level indicate that DHT11 ready to send data
		if(GPIO_ReadInputDataBit(DHT11_Data_Port, DHT11_Data_Pin)) Check = 1;
		// if after delay 100 us, response signal at low level indicate that DHT11 not ready to send data
		else Check = 2;
	}
	while(GPIO_ReadInputDataBit(DHT11_Data_Port, DHT11_Data_Pin)); // wait DHT11 response signal at low
	return Check;  // if Check = 0, DHT11 not response
}
 
/* Read data after DHT11 response */

uint8_t Read_Data_From_DHT11(){
	uint8_t i,j;
	for(i = 0; i < 8; i++){
		while(!GPIO_ReadInputDataBit(DHT11_Data_Port, DHT11_Data_Pin)); // wait about 50 us at pin low
		Delay_Us(50); // wait about 50us
		// after 50us sinal at low -> bit '0'
		if(!GPIO_ReadInputDataBit(DHT11_Data_Port, DHT11_Data_Pin)){
			j &= ~(1 << (7-i));
		}
		// bit '1'
		else {
			j |= (1 << (7-i));
		}
		while(GPIO_ReadInputDataBit(DHT11_Data_Port, DHT11_Data_Pin)); // wait signal at low
	}
	return j;
}

/* Display temperature on LCD */

void Display_Temperature(int temperature){
	char str[20] = {0};
	LCD_Put_Cursor(0, 1);
	sprintf (str, "NHIET DO - %d C", temperature);
	LCD_Send_String(str);
}

/* Display humidity on LCD */

void Display_Humidity(int humidity){
	char str[20] = {0};
	LCD_Put_Cursor(1, 1);
	sprintf (str, "DO AM - %d ", humidity);
	LCD_Send_String(str);
	LCD_Send_String("%");
}

int main(void) {
	Config_GPIO();
	Config_I2C();
	Config_Timer();
	
	LCD_Init();
	LCD_Put_Cursor(1, 1);
	LCD_Send_String("KIM TUAN KIEN");
	LCD_Put_Cursor(0, 5);
	LCD_Send_String("PTIT");
	Delay_Ms(5000);
	LCD_Clear();
	
	while (1) {
		
		Display_Temperature(Temperature);
		Display_Humidity(Humidity);
		
		Send_Start_Signal_To_DHT11();
		Check_Response = Check_DHT11_Response();
		/* Check DHT11 response ? */
		if(Check_Response == 0) {
			while(1){
				LCD_Clear();
				LCD_Put_Cursor(0, 2);
				LCD_Send_String("NOT RESPONSE");
				Delay_Ms(5000);
			}
		}
		else if(Check_Response == 2) {
			while(1){
				LCD_Clear();
				LCD_Put_Cursor(0, 2);
				LCD_Send_String("NOT READY");
				Delay_Ms(5000);
			}
		}
		
		/* Read 5 byte data from DHT11*/
		Data[0] = Read_Data_From_DHT11(); // Byte 1 RH - intergral
		Data[1] = Read_Data_From_DHT11(); // Byte 2 RH - decimal
		Data[2] = Read_Data_From_DHT11();	// Byte 2 Temp - intergral
		Data[3] = Read_Data_From_DHT11(); // Byte 2 Temp - decimal
		Data[4] = Read_Data_From_DHT11(); // Byte check sum
		
		/*Check if byte1 + byte 2 + byte 3 + byte 4 != byte 5 ?*/
		if(Data[4] != Data[0] + Data[1] + Data[2] + Data[3]){
			while(1){
				LCD_Clear();
				LCD_Put_Cursor(0, 1);
				LCD_Send_String("WRONG DATA");
				Delay_Ms(5000);
			}			
		}
		
		Temperature = Data[2];
		Humidity = Data[0];
		
		Delay_Ms(3000);  

	}
}

/* Configure GPIO */

void Config_GPIO(){
	
	/* cau hinh chan PB6-SCL và PB7-SDA */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	gpioInit.GPIO_Mode = GPIO_Mode_AF_OD;
	gpioInit.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioInit);
	
	/* Cau hinh chan PA0- Data DHT11 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	gpioInit.GPIO_Mode = GPIO_Mode_Out_OD;
	gpioInit.GPIO_Pin = GPIO_Pin_0;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioInit);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

/*  Configure i2C1 */

void Config_I2C(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	i2c1.I2C_Ack = I2C_Ack_Enable;
	i2c1.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	i2c1.I2C_ClockSpeed = 100000;
	i2c1.I2C_DutyCycle = I2C_DutyCycle_2;
	i2c1.I2C_Mode = I2C_Mode_I2C;
	I2C_Init(I2C1, &i2c1);
	I2C_Cmd(I2C1, ENABLE);
}
