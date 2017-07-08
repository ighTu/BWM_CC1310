/*
 * nokia_5110_lcd.c
 *
 *  Created on: 09.12.2016
 *      Author: MyDev
 */


//#include <stdlib.h>
//#include <stdint.h>

/*#include "Board.h"

#include <ti/drivers/PIN.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/I2C.h>

#include "english_font.h"
#include "nokia_5110_lcd.h"
#include "bios_CC1310.h"*/

#include "XI_Bios/bios.h"
#include <XI_Sensors/sensors.h>
#include "english_font.h"




//#include <ti/drivers/UART.h>
//#include <ti/drivers/I2C.h>

//#include <bios_CC1310.h>

//static PIN_Handle 	lcdPinHandle;
//static PIN_State 	lcdPinState;


#define LCD_RST_set		PIN_setOutputValue(lcdPinHandle, LCD_RST, 1)
#define LCD_RST_clr 	PIN_setOutputValue(lcdPinHandle, LCD_RST, 0)

#define LCD_CE_set 		PIN_setOutputValue(lcdPinHandle, LCD_CE, 1)
#define LCD_CE_clr 		PIN_setOutputValue(lcdPinHandle, LCD_CE, 0)

#define LCD_DC_set 		PIN_setOutputValue(lcdPinHandle, LCD_DC, 1)
#define LCD_DC_clr 		PIN_setOutputValue(lcdPinHandle, LCD_DC, 0)

#define SDIN_set 	PIN_setOutputValue(lcdPinHandle, LCD_SDIN, 1)
#define SDIN_clr 	PIN_setOutputValue(lcdPinHandle, LCD_SDIN, 0)

#define SCLK_set 	PIN_setOutputValue(lcdPinHandle, LCD_SCLK, 1)
#define SCLK_clr 	PIN_setOutputValue(lcdPinHandle, LCD_SCLK, 0)



PIN_Config lcdPinTable[] = {
		LCD_RST | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PULLUP | PIN_PUSHPULL | PIN_DRVSTR_MAX,
		LCD_CE | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PULLUP | PIN_PUSHPULL | PIN_DRVSTR_MAX,
		LCD_DC | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PULLUP | PIN_PUSHPULL | PIN_DRVSTR_MAX,
		LCD_SDIN | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PULLUP | PIN_PUSHPULL | PIN_DRVSTR_MAX,
		LCD_SCLK | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PULLUP | PIN_PUSHPULL | PIN_DRVSTR_MAX,
		PIN_TERMINATE
};

void init_lcdPinTable(void){

	lcdPinHandle = PIN_open(&lcdPinState, lcdPinTable);

	if(!lcdPinHandle) {
		// error
		//System_abort("Error initializing board LCD pins\n");
	}

}

void LCD_writeSDIN(unsigned char state){

	if(state == 0){
		SDIN_clr;
	}else{
		SDIN_set;
	}
}

void LCD_write_byte(unsigned char dat, unsigned char command){

	unsigned char i;

	LCD_CE_clr;


	if(command == 1){
		LCD_DC_set;
	}else{
		LCD_DC_clr;
	}

	for(i=0;i<8;i++){

		LCD_writeSDIN(dat & 0x80);
		dat = dat << 1;
		SCLK_clr;
		//__delay_ms(100);
		SCLK_set;
	}

	LCD_CE_set;
	//__delay_us(1);

}

void LCD_init(void){
	LCD_RST_clr;
	__delay_ms(100);
	LCD_RST_set;

	//__delay_ms(100);

	LCD_write_byte(0x21, 0); 	// Extended instruction set selected
	LCD_write_byte(0xB8, 0);	// set bias voltage
	LCD_write_byte(0x06, 0);	// temperatur correction
	LCD_write_byte(0x14, 0);	// 1:48
	LCD_write_byte(0x20, 0);	// use bias command, vertical

	LCD_clear();				// clear the LCD

	LCD_write_byte(0x0C, 0);	// set LCD mode, display normally

}

void LCD_clear(void){

	unsigned char i, j;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 84; j++) {
			LCD_write_byte(0, 1);
		}
	}

}

void LCD_set_XY( unsigned char Y, unsigned char X){

	unsigned char x;

	x = 6 * X;

	LCD_write_byte(0x40 | Y, 0);
	LCD_write_byte(0x80 | x, 0);
}

void LCD_write_char(unsigned char c){
	unsigned char line;
	unsigned char ch = 0;

	c -= 32;

	for(line=0; line<6; line++){
		ch = font6x8[c][line];
		LCD_write_byte(ch, 1);
	}
}

void LCD_write_english_string(unsigned char Y, unsigned char X, char *s){

	unsigned char ch;

	LCD_set_XY(Y, X);
	__delay_us(1);
	while(*s){
		ch = *s;
		LCD_write_char(ch);
		s++;

		//__delay_ms(100);
	}
}

void nokia_LCD_TEST(void){

	static uint8_t n = 0;

	LCD_write_english_string(0,0,"Hello Sweta ");
	LCD_write_english_string(2,0,"   4  ");

	if(n==0){
		LCD_write_english_string(4,15," ");
		LCD_write_english_string(4,n,">");
	}else if(n==1){
		LCD_write_english_string(4,n-1,"-");
		LCD_write_english_string(4,n,">");
	}else if(n==2){
		LCD_write_english_string(4,n-2,"-");
		LCD_write_english_string(4,n-1,"-");
		LCD_write_english_string(4,n,">");
	}else if(n==14){
		LCD_write_english_string(4,n-3," ");
		LCD_write_english_string(4,n-2,"-");
		LCD_write_english_string(4,n-1,"-");
		LCD_write_english_string(4,0,">");
	}else if(n==15){
		LCD_write_english_string(4,n-3," ");
		LCD_write_english_string(4,n-2,"-");
		LCD_write_english_string(4,0,"-");
		LCD_write_english_string(4,1,">");
	}
	else if(n==16){
		LCD_write_english_string(4,n-3," ");
		LCD_write_english_string(4,0,"-");
		LCD_write_english_string(4,1,"-");
		LCD_write_english_string(4,2,">");
	}else{
		LCD_write_english_string(4,n-3," ");
		LCD_write_english_string(4,n-2,"-");
		LCD_write_english_string(4,n-1,"-");
		LCD_write_english_string(4,n,">");
	}


	n++;

	if(n > 16){
		//LCD_write_english_string(4,n-1,"   ");
		n=3;
	}



}

//PIN_setOutputValue(ledPinHandle, Board_LED0, !currVal);


