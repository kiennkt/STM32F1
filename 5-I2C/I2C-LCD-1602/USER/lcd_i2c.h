#include "stm32f10x.h"                  // Device header
#include "stm32f10x_i2c.h"              // Keil::Device:StdPeriph Drivers:I2C


void LCD_Init();		// Initialize LCD

void LCD_Send_Command(char cmd);		// Send command to LCD

void LCD_Send_Data(char data);				// Send data to LCD

void LCD_Send_String(char *str);		// Send string to LCD

void LCD_Clear();		// Clear display on LCD

void LCD_Put_Cursor(uint8_t row, uint8_t column);			// Put cursor at row 0 to 2, column 0 to 15

void I2C_Master_Transmit(uint16_t Slave_Address, uint8_t *pData, uint16_t size);