#include "stm32f10x.h"
#include "delay.h"
#include "lcd_i2c.h"

void Config_GPIO();
void Config_I2C();
I2C_InitTypeDef i2c1;

int main(void) {
	Config_GPIO();
	Config_I2C();
	Config_Timer();
	
	LCD_Init();
	LCD_Put_Cursor(0, 1);
	LCD_Send_String("KIM TUAN KIEN");
	LCD_Put_Cursor(1, 5);
	LCD_Send_String("PTIT");

	while (1) {
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		Delay_Ms(500);
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		Delay_Ms(500);
	}
}

/* Configure GPIO */

void Config_GPIO(){
	GPIO_InitTypeDef gpioInit;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInit.GPIO_Pin = GPIO_Pin_13;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpioInit);
	
	/* cau hinh chan PB6-SCL v� PB7-SDA */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	gpioInit.GPIO_Mode = GPIO_Mode_AF_OD;
	gpioInit.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioInit);
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


