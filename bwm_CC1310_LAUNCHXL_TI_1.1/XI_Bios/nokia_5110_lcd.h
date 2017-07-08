/*
 * nokia_5110_lcd.h
 *
 *  Created on: 09.12.2016
 *      Author: MyDev
 */

#ifndef NOKIA_5110_LCD_H_
#define NOKIA_5110_LCD_H_

PIN_Handle 	lcdPinHandle;
PIN_State 	lcdPinState;

void init_lcdPinTable(void);
void LCD_init(void);
void LCD_clear(void);
void LCD_set_XY(unsigned char Y, unsigned char X);
void LCD_write_char(unsigned char c);
void LCD_write_english_string(unsigned char Y, unsigned char X, char *s);
void LCD_write_byte(unsigned char dat, unsigned char command);

void nokia_LCD_TEST(void);


#endif /* NOKIA_5110_LCD_H_ */
