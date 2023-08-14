
#include "lcd_i2c.h"
#include "delay.h"

#define Slave_Address_LCD 0x4E
extern I2C_InitTypeDef i2c1;

void I2C_Master_Transmit(uint16_t Slave_Address, uint8_t *pData, uint16_t size){
	uint8_t i;
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, Slave_Address, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	for(i = 0; i < size; i++){
		I2C_SendData(I2C1, pData[i]);
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}
	I2C_GenerateSTOP(I2C1, ENABLE);
}

void LCD_Send_Command(char cmd) {
	
	char bits_high, bits_low;
	uint8_t Buff_Cmd[4];
	bits_high = (cmd & 0xF0);
	bits_low = ((cmd << 4) & 0xF0);
	/* D7 D6 D5 D4 BLA E 	RW RS      LCD hd44780
		 P7	P6 P5 P4 P3  P2 P1 P0			 module i2c PCF8574
		 0  0  0  0  1   x  0  0
	*/
	Buff_Cmd[0] = bits_high | 0x0C;   // E = 1
	Buff_Cmd[1] = bits_high | 0x08;		// E = 0
	Buff_Cmd[2] = bits_low | 0x0C;		// E = 1
	Buff_Cmd[3] = bits_low | 0x08;		// E = 0
	
	I2C_Master_Transmit(Slave_Address_LCD, Buff_Cmd, 4);
}

void LCD_Send_Data(char data) {
	
	char bits_high, bits_low;
	uint8_t Buff_Data[4];
	bits_high = (data & 0xF0);
	bits_low = ((data << 4) & 0xF0);
	/* D7 D6 D5 D4 BLA E 	RW RS      LCD hd44780
		 P7	P6 P5 P4 P3  P2 P1 P0			 module i2c PCF8574
		 0  0  0  0  1   x  0  1
	*/
	Buff_Data[0] = bits_high | 0x0D;   // E = 1
	Buff_Data[1] = bits_high | 0x09;		// E = 0
	Buff_Data[2] = bits_low | 0x0D;		// E = 1
	Buff_Data[3] = bits_low | 0x09;		// E = 0
	
	I2C_Master_Transmit(Slave_Address_LCD, Buff_Data, 4);
}

void LCD_Init(){
	// Start init
	Delay_Ms(50);	// > 15 ms to VCC up to 4.5V
	LCD_Send_Command(0x30);
	Delay_Ms(5);	// > 4.1 ms
	LCD_Send_Command(0x30);
	Delay_Ms(1);  // > 100 us;
	LCD_Send_Command(0x30);
	Delay_Ms(10);
	LCD_Send_Command(0x20);
	Delay_Ms(10);
	LCD_Send_Command(0x28); // function set, N = 1, F = 0
	Delay_Ms(1);
	LCD_Send_Command(0x08); // display on, D = C = B = 0 (D: display on/off, C: cursor on/off, B: blink on/ off)
	Delay_Ms(1);
	LCD_Send_Command(0x01); // clear display
	Delay_Ms(1);
	LCD_Send_Command(0x06); // Entry mode set, I/D = 0, S = 0    	// End init
	Delay_Ms(1);
	LCD_Send_Command(0x0C); // D = C = B = 1
}

void LCD_Send_String(char *str){
	while(*str){
		LCD_Send_Data(*str++);
	}
}

void LCD_Clear (void) {
	uint8_t i;
	LCD_Send_Command (0x80); // put cursor at first line
	for (i=0; i < 60; i++) {
		LCD_Send_Data (' ');
	}
}
/* DDRAM ADDRESS LCD 1602 
		80 81 82 83 84 85 86 87 88 89 8A 8B 8C 8D 8E 8F
		C0 C1 C2 C3 C4 C5 C6 C7 C8 C9 CA CB CC CD CE CF
*/
void LCD_Put_Cursor(uint8_t row, uint8_t column){
	if(row == 0){
		column |= 0x80;
	}
	else if(row == 1){
		column |= 0xC0;
	}
	LCD_Send_Command(column);
}

void LCD_Right_Shift_Display(uint8_t n){
	uint8_t i;
	for(i = 0; i < n; i++){
		LCD_Send_Command(0x1C);
		Delay_Ms(400);
	}	
}

void LCD_Left_Shift_Display(uint8_t n){
	uint8_t i;
	for(i = 0; i < n; i++){
		LCD_Send_Command(0x18);
		Delay_Ms(400);
	}	
}