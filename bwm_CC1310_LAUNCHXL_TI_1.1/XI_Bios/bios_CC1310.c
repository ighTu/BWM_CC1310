/*
 * bios.c
 *
 *  Created on: 27.12.2016
 *      Author: MyDev
 */

/*#include <stdlib.h>
#include <string.h>

#include <xdc/runtime/System.h>
#include "Board.h"

#include <ti/drivers/UART.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPICC26XXDMA.h>
#include "ti/drivers/I2C.h"
#include "bios_CC1310.h"
#include "nokia_5110_lcd.h"*/
#include <XI_Sensors/sensors.h>
#include "XI_Bios/bios.h"

/********************************************************************************
 *  																System
 ********************************************************************************/
void ________init_System________(void);

void __delay_us(uint16_t usTime){
	// clock 42.000.000 Hz

	uint32_t clock = 42;
	uint32_t count = 0;
	uint32_t i = 0;

	count = clock*usTime/6;

	for(i=0; i < count; i++);
}

void __delay_ms(uint16_t msTime){
	// clock 42.000.000 Hz
	uint32_t clock = 42000;
	uint32_t count = 0;
	uint32_t i = 0;

	count = clock*msTime/6;

	for(i=0; i < count; i++);
}

void System_init(void){

	bwmSystemStatus = 0x0000;	// reset system inits

	if(uart_init()){
		bwmSystemStatus |= UART_INITIALIZED;
	}

	if(i2c_init()){
		bwmSystemStatus |= I2C_INITIALIZED;
	}


}


/*---- Ende System ----------------------------------------------------------------*/


/********************************************************************************
 *  																UART
 ********************************************************************************/
void ________init_UART________(void);

//UART_Params uartParams;
//UART_Handle uart;

uint8_t uart_init(void){

	//const char echoPrompt[] = "\fUART init\r\n";

	UART_Params_init(&uartParams);
	uartParams.writeDataMode = UART_DATA_BINARY;
	uartParams.readDataMode = UART_DATA_BINARY;
	uartParams.readReturnMode = UART_RETURN_FULL;
	uartParams.readEcho = UART_ECHO_OFF;
	uartParams.baudRate = 9600;
	uart = UART_open(Board_UART0, &uartParams);

	if (uart == NULL) {
		return 0;
	}

	return 1;
}

uint16_t getch(char *c){

	return 0;
}

uint16_t putch(char *c){

	UART_write(uart, c, 1);

	return 0;
}

void putstr(char *str){

	char Buffer[128];
	uint16_t length = 0;

	while(*str != '\0'){
		Buffer[length] = *str;
		length++;
		str++;
	}

	UART_write(uart, Buffer, length);
}

void putdata(int32_t data){

	char Buffer[20];
//	uint16_t length = 0;

	ltoa(data, Buffer);
	//while(Buffer[length] != '\0'){ length++; }
	putstr(Buffer);

}

void puthex(int32_t data){

	char hex[] = "0123456789ABCDEF";
	char twistedBuffer[20];
	char Buffer[20];
	uint32_t length = 0;
	int8_t i = 0;

	do{
		twistedBuffer[length] = hex[data % 16];
		length++;
	}while((data /= 16) > 0);

	//data /= 16;

	Buffer[length] = '\0';

	length--;

	for(i=length; i>=0; i--){
		Buffer[i] = twistedBuffer[length-i];
	}

	length = 0;
			//while(Buffer[length] != '\0'){ length++; }
	putstr(Buffer);

}

void putFloatData(int64_t data, uint16_t points){

	char Buffer1[20];
	char Buffer2[20];
	uint32_t data1 = 0;
	uint32_t data2 = 0;

	if(points == 0){
		points = 1;
	}

	data1 = (uint32_t)(data / points);
	data2 = (uint32_t)(data - (data1*points));

	ltoa(data1, Buffer1);
	ltoa(data2, Buffer2);


	//putstr("\r\n");
	putstr(Buffer1);
	putstr(".");
	putstr(Buffer2);
}

/*---- Ende UART ----------------------------------------------------------------*/


/********************************************************************************
 *  																SPI
 ********************************************************************************/
void ________init_SPI________(void);

void spi(uint8_t data){

}


static void transferCallback(SPI_Handle handle, SPI_Transaction *transaction){
	// Start another transfer
	transaction->count = 100;
	SPI_transfer(handle, transaction);
}


void spi_init(void){

	uint16_t txBuf[100], rxBuf[100];

	// Init SPI and specify non-default parameters
	SPI_Params_init(&spiParams);
	spiParams.bitRate = 1000000;
	spiParams.dataSize = 8;
	spiParams.frameFormat = SPI_POL1_PHA1;
	spiParams.mode = SPI_MASTER;
	spiParams.transferMode = SPI_MODE_BLOCKING;
	spiParams.transferCallbackFxn = transferCallback;

	// Configure the transaction
	strcpy((char*)txBuf, "Master SPI");
	spiTransaction.count = 100;
	spiTransaction.txBuf = txBuf;
	spiTransaction.rxBuf = rxBuf;

	// Open the SPI and initiate the first transfer
	spiHandle = SPI_open(Board_SPI0, &spiParams);

	SPI_control(spiHandle, SPICC26XXDMA_RETURN_PARTIAL_ENABLE, NULL);

	SPI_transfer(spiHandle, &spiTransaction);
}

/*---- Ende SPI ----------------------------------------------------------------*/

/********************************************************************************
 *  																I2C
 ********************************************************************************/
void ________init_I2C________(void);

uint8_t i2c_init(void){

	// Create I2C for usage
	I2C_Params_init(&i2cParams);
	i2cParams.bitRate = I2C_400kHz;
	i2c = I2C_open(Board_I2C_TMP, &i2cParams);
	if(i2c == NULL){
		return 0;
	}

	return 1;
}

void close_I2C(void){
	I2C_close(i2c);
	//System_printf("I2C closed!\n");
}


/*---- Ende I2C ----------------------------------------------------------------*/

/********************************************************************************
 *  																Test
 ********************************************************************************/
void ________init_Test________(void);


/*---- Ende Test ----------------------------------------------------------------*/






