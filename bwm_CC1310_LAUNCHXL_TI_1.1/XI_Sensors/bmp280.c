/*
 * bmp280.c
 *
 *  Created on: 03.01.2017
 *      Author: MyDev
 */

/*#include "Board.h"
#include "math.h"

#include <ti/drivers/UART.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPICC26XXDMA.h>
#include <ti/drivers/I2C.h>

#include "bmp280.h"
#include "bios_CC1310.h"*/
#include <XI_Sensors/sensors.h>
#include "XI_Bios/bios.h"



uint8_t read8(uint8_t bmp280_reg){

	uint8_t value = 0;

	i2c_BMP280_Transaction.slaveAddress = BMP280_SLAVE_ADDRESS_1;
	i2c_BMP280_Transaction.writeBuf = txBMP280_Buffer;
	i2c_BMP280_Transaction.writeCount = 1;
	i2c_BMP280_Transaction.readBuf = rxBMP280_Buffer;
	i2c_BMP280_Transaction.readCount = 1;

	txBMP280_Buffer[0] = bmp280_reg;

	I2C_transfer(i2c, &i2c_BMP280_Transaction);

	value = rxBMP280_Buffer[0];

	return value;
}

uint8_t write8(uint8_t bmp280_reg, uint8_t bmp280_value){

	uint8_t value = 0;

	i2c_BMP280_Transaction.slaveAddress = BMP280_SLAVE_ADDRESS_1;
	i2c_BMP280_Transaction.writeBuf = txBMP280_Buffer;
	i2c_BMP280_Transaction.writeCount = 2;
	i2c_BMP280_Transaction.readBuf = rxBMP280_Buffer;
	i2c_BMP280_Transaction.readCount = 2;

	txBMP280_Buffer[0] = bmp280_reg;
	txBMP280_Buffer[1] = bmp280_value;

	I2C_transfer(i2c, &i2c_BMP280_Transaction);

	//value = rxBMP280_Buffer[0];

	return value;
}

void readCoefficients(void){

	i2c_BMP280_Transaction.slaveAddress = BMP280_SLAVE_ADDRESS_1;
	i2c_BMP280_Transaction.writeBuf = txBMP280_Buffer;
	i2c_BMP280_Transaction.writeCount = 1;
	i2c_BMP280_Transaction.readBuf = rxBMP280_Buffer;
	i2c_BMP280_Transaction.readCount = 2;

	txBMP280_Buffer[0] = BMP280_REGISTER_DIG_T1;
	I2C_transfer(i2c, &i2c_BMP280_Transaction);
	_bmp280_calib.dig_T1 = ( (rxBMP280_Buffer[1] << 8) | rxBMP280_Buffer[0] );
	txBMP280_Buffer[0] = BMP280_REGISTER_DIG_T2;
	I2C_transfer(i2c, &i2c_BMP280_Transaction);
	_bmp280_calib.dig_T2 = ( (rxBMP280_Buffer[1] << 8) | rxBMP280_Buffer[0] );
	txBMP280_Buffer[0] = BMP280_REGISTER_DIG_T3;
	I2C_transfer(i2c, &i2c_BMP280_Transaction);
	_bmp280_calib.dig_T3 = ( (rxBMP280_Buffer[1] << 8) | rxBMP280_Buffer[0] );

	txBMP280_Buffer[0] = BMP280_REGISTER_DIG_P1;
	I2C_transfer(i2c, &i2c_BMP280_Transaction);
	_bmp280_calib.dig_P1 = ( (rxBMP280_Buffer[1] << 8) | rxBMP280_Buffer[0] );
	txBMP280_Buffer[0] = BMP280_REGISTER_DIG_P2;
	I2C_transfer(i2c, &i2c_BMP280_Transaction);
	_bmp280_calib.dig_P2 = ( (rxBMP280_Buffer[1] << 8) | rxBMP280_Buffer[0] );
	txBMP280_Buffer[0] = BMP280_REGISTER_DIG_P3;
	I2C_transfer(i2c, &i2c_BMP280_Transaction);
	_bmp280_calib.dig_P3 = ( (rxBMP280_Buffer[1] << 8) | rxBMP280_Buffer[0] );
	txBMP280_Buffer[0] = BMP280_REGISTER_DIG_P4;
	I2C_transfer(i2c, &i2c_BMP280_Transaction);
	_bmp280_calib.dig_P4 = ( (rxBMP280_Buffer[1] << 8) | rxBMP280_Buffer[0] );
	txBMP280_Buffer[0] = BMP280_REGISTER_DIG_P5;
	I2C_transfer(i2c, &i2c_BMP280_Transaction);
	_bmp280_calib.dig_P5 = ( (rxBMP280_Buffer[1] << 8) | rxBMP280_Buffer[0] );
	txBMP280_Buffer[0] = BMP280_REGISTER_DIG_P6;
	I2C_transfer(i2c, &i2c_BMP280_Transaction);
	_bmp280_calib.dig_P6 = ( (rxBMP280_Buffer[1] << 8) | rxBMP280_Buffer[0] );
	txBMP280_Buffer[0] = BMP280_REGISTER_DIG_P7;
	I2C_transfer(i2c, &i2c_BMP280_Transaction);
	_bmp280_calib.dig_P7 = ( (rxBMP280_Buffer[1] << 8) | rxBMP280_Buffer[0] );
	txBMP280_Buffer[0] = BMP280_REGISTER_DIG_P8;
	I2C_transfer(i2c, &i2c_BMP280_Transaction);
	_bmp280_calib.dig_P8 = ( (rxBMP280_Buffer[1] << 8) | rxBMP280_Buffer[0] );
	txBMP280_Buffer[0] = BMP280_REGISTER_DIG_P9;
	I2C_transfer(i2c, &i2c_BMP280_Transaction);
	_bmp280_calib.dig_P9 = ( (rxBMP280_Buffer[1] << 8) | rxBMP280_Buffer[0] );

}

uint8_t bmp280_init(void){
	// Point to the T ambient register and read its 2 bytes
	//txTMP007_Buffer[0] = 0x01;

	if(read8(BMP280_CHIP_ID) != 0x58){
		return 0;
	}

	readCoefficients();

	write8(BMP280_CTRL_MEAS, 0xF3);

	return 1;
	//bmp280_readCoeffiecients();

}

int32_t get_BMP280_Temp(void){

	int32_t adc_T = 0;
	int32_t var1 = 0, var2 = 0;
	int32_t Temp = 0;

	i2c_BMP280_Transaction.slaveAddress = BMP280_SLAVE_ADDRESS_1;
	i2c_BMP280_Transaction.writeBuf = txBMP280_Buffer;
	i2c_BMP280_Transaction.writeCount = 1;
	i2c_BMP280_Transaction.readBuf = rxBMP280_Buffer;
	i2c_BMP280_Transaction.readCount = 3;

	txBMP280_Buffer[0] = BMP280_TEMP_MSB;

	I2C_transfer(i2c, &i2c_BMP280_Transaction);

	adc_T = rxBMP280_Buffer[0];
	adc_T <<= 8;
	adc_T |= rxBMP280_Buffer[1];
	adc_T <<= 8;
	adc_T |= rxBMP280_Buffer[2];
	adc_T >>= 4;

	var1  = ((((adc_T>>3) - ((int32_t)_bmp280_calib.dig_T1 <<1))) *
			((int32_t)_bmp280_calib.dig_T2)) >> 11;

	var2  = (((((adc_T>>4) - ((int32_t)_bmp280_calib.dig_T1)) *
			((adc_T>>4) - ((int32_t)_bmp280_calib.dig_T1))) >> 12) *
			((int32_t)_bmp280_calib.dig_T3)) >> 14;

	t_fine = (var1 + var2);

	Temp = (t_fine * 5 + 128) >> 8;

	return Temp-100;
}

uint32_t get_BMP280_Press(void){

	int32_t adc_P;
	int32_t var1 = 0, var2 = 0;
	uint32_t p = 0;

	get_BMP280_Temp();

	i2c_BMP280_Transaction.slaveAddress = BMP280_SLAVE_ADDRESS_1;
	i2c_BMP280_Transaction.writeBuf = txBMP280_Buffer;
	i2c_BMP280_Transaction.writeCount = 1;
	i2c_BMP280_Transaction.readBuf = rxBMP280_Buffer;
	i2c_BMP280_Transaction.readCount = 3;

	txBMP280_Buffer[0] = BMP280_PRESS_MSB;
	if(I2C_transfer(i2c, &i2c_BMP280_Transaction));

	adc_P = rxBMP280_Buffer[0];
	adc_P <<= 8;
	adc_P |= rxBMP280_Buffer[1];
	adc_P <<= 8;
	adc_P |= rxBMP280_Buffer[2];
	adc_P >>= 4;

	var1 = (((int32_t)t_fine)>>1) - (int32_t)64000;
	var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)_bmp280_calib.dig_P6);
	var2 = var2 + ((var1*((int32_t)_bmp280_calib.dig_P5))<<1);
	var2 = (var2>>2)  + (((int32_t)_bmp280_calib.dig_P4)<<16);
	var1 = ((((int32_t)_bmp280_calib.dig_P3 * (((var1>>2) * (var1>>2)) >> 13)) >> 3) + ((((int32_t)_bmp280_calib.dig_P2) * var1) >> 1)) >> 18;
	var1 = ((((32768+var1))*((uint32_t)_bmp280_calib.dig_P1)) >> 15);

	if (var1 == 0) {
		return 0;  // avoid exception caused by division by zero
	}
	p = (((uint32_t)(((int32_t)1048576)-adc_P)-(var2>>12)))*3125;

	if(p < 0x80000000){
		p = (p << 1) / ((uint32_t)var1);
	}else{
		p = (p / (uint32_t)var1) * 2;
	}
	var1 = (((int32_t)_bmp280_calib.dig_P9) * ((int32_t)(((p>>3) * (p>>3)) >> 13))) >> 12;
	var2 = (((int32_t)(p>>2)) * ((int32_t)_bmp280_calib.dig_P8)) >> 13;

	p = (uint32_t)((int32_t)p + ((var1 + var2 + _bmp280_calib.dig_P7) >> 4));

	return p;
}



/*int32_t get_BMP280_Press(void){

	int32_t adc_P;
	int64_t var1 = 0, var2 = 0, p = 0;

	get_BMP280_Temp();

	i2c_BMP280_Transaction.slaveAddress = BMP280_SLAVE_ADDRESS_1;
	i2c_BMP280_Transaction.writeBuf = txBMP280_Buffer;
	i2c_BMP280_Transaction.writeCount = 1;
	i2c_BMP280_Transaction.readBuf = rxBMP280_Buffer;
	i2c_BMP280_Transaction.readCount = 3;



	txBMP280_Buffer[0] = BMP280_PRESS_MSB;
	if(I2C_transfer(i2c, &i2c_BMP280_Transaction));

	adc_P = rxBMP280_Buffer[0];
	adc_P <<= 8;
	adc_P |= rxBMP280_Buffer[1];
	adc_P <<= 8;
	adc_P |= rxBMP280_Buffer[2];
	adc_P >>= 4;

	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)_bmp280_calib.dig_P6;
	var2 = var2 + ((var1*(int64_t)_bmp280_calib.dig_P5)<<17);
	var2 = var2 + (((int64_t)_bmp280_calib.dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)_bmp280_calib.dig_P3)>>8) +
			((var1 * (int64_t)_bmp280_calib.dig_P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)_bmp280_calib.dig_P1)>>33;

	if (var1 == 0) {
		return 0;  // avoid exception caused by division by zero
	}
	p = 1048576 - adc_P;
	p = (((p<<31) - var2)*3125) / var1;
	var1 = (((int64_t)_bmp280_calib.dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((int64_t)_bmp280_calib.dig_P8) * p) >> 19;

	p = ((p + var1 + var2) >> 8) + (((int64_t)_bmp280_calib.dig_P7)<<4);

	return p;
}*/

int32_t readAltitude(void){

	//Change the "1050.35" to your city current barrometric pressure (https://www.wunderground.com)

	//float_t altitude
	float_t altitude = 0;
	float_t pressure = 0;

	pressure = get_BMP280_Press();
	pressure /= 100;

	altitude = 44330 * (1.0 - pow(pressure / 1050.35, 0.1903)) *1000;

	return altitude;
}





uint8_t get_BMP280_Id(void){

	uint8_t pressure = 0;

	txBMP280_Buffer[0] = BMP280_CHIP_ID;

	if(I2C_transfer(i2c, &i2c_BMP280_Transaction)){

		pressure = rxBMP280_Buffer[0];

	}

	return pressure;
}

uint8_t get_BMP280_Status(void){

	uint8_t pressure = 0;

	txBMP280_Buffer[0] = BMP280_STATUS;

	if(I2C_transfer(i2c, &i2c_BMP280_Transaction)){

		pressure = rxBMP280_Buffer[0];

	}

	return pressure;
}

uint8_t set_BMP280_Reset(void){

	uint8_t pressure = 0;

	txBMP280_Buffer[0] = BMP280_RESET;

	if(I2C_transfer(i2c, &i2c_BMP280_Transaction)){

		pressure = rxBMP280_Buffer[0];

	}

	return pressure;
}
