# Communication between STM32F103C8T6 and PCF8574 module of LCD 16x02 using I2C protocol
## Introduction
### This project uses Timer 2 to create delay miniseconds and uses I2C1 that means PB6 is SCL and PB7 is SDA 
### Address of PCF8574 module is 0x4E (8 bit or 0x27 for 7 bit)
## Note: Don't connect VCC and GND of PCF8574 module to STM32. Just connect VCC and GND to ST-Link
