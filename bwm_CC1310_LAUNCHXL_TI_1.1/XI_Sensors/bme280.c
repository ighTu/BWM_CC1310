/*
 * BME280.c
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

#include "bme280.h"
#include "bios_CC1310.h"*/
#include <XI_Sensors/sensors.h>
#include "XI_Bios/bios.h"



uint8_t BME280_read8(uint8_t BME280_reg){

	uint8_t value = 0;

	i2c_BME280_Transaction.slaveAddress = BME280_SLAVE_ADDRESS_1;
	i2c_BME280_Transaction.writeBuf = txBME280_Buffer;
	i2c_BME280_Transaction.writeCount = 1;
	i2c_BME280_Transaction.readBuf = rxBME280_Buffer;
	i2c_BME280_Transaction.readCount = 1;

	txBME280_Buffer[0] = BME280_reg;

	I2C_transfer(i2c, &i2c_BME280_Transaction);

	value = rxBME280_Buffer[0];

	return value;
}

uint8_t BME280_write8(uint8_t BME280_reg, uint8_t BME280_value){

	uint8_t value = 0;

	i2c_BME280_Transaction.slaveAddress = BME280_SLAVE_ADDRESS_1;
	i2c_BME280_Transaction.writeBuf = txBME280_Buffer;
	i2c_BME280_Transaction.writeCount = 2;
	i2c_BME280_Transaction.readBuf = rxBME280_Buffer;
	i2c_BME280_Transaction.readCount = 2;

	txBME280_Buffer[0] = BME280_reg;
	txBME280_Buffer[1] = BME280_value;

	I2C_transfer(i2c, &i2c_BME280_Transaction);

	//value = rxBME280_Buffer[0];

	return value;
}

void BME280_readCoefficients(void){

	i2c_BME280_Transaction.slaveAddress = BME280_SLAVE_ADDRESS_1;
	i2c_BME280_Transaction.writeBuf = txBME280_Buffer;
	i2c_BME280_Transaction.writeCount = 1;
	i2c_BME280_Transaction.readBuf = rxBME280_Buffer;
	i2c_BME280_Transaction.readCount = 2;

	txBME280_Buffer[0] = BME280_REGISTER_DIG_T1;
	I2C_transfer(i2c, &i2c_BME280_Transaction);
	_BME280_calib.dig_T1 = ( (rxBME280_Buffer[1] << 8) | rxBME280_Buffer[0] );
	txBME280_Buffer[0] = BME280_REGISTER_DIG_T2;
	I2C_transfer(i2c, &i2c_BME280_Transaction);
	_BME280_calib.dig_T2 = ( (rxBME280_Buffer[1] << 8) | rxBME280_Buffer[0] );
	txBME280_Buffer[0] = BME280_REGISTER_DIG_T3;
	I2C_transfer(i2c, &i2c_BME280_Transaction);
	_BME280_calib.dig_T3 = ( (rxBME280_Buffer[1] << 8) | rxBME280_Buffer[0] );

	txBME280_Buffer[0] = BME280_REGISTER_DIG_P1;
	I2C_transfer(i2c, &i2c_BME280_Transaction);
	_BME280_calib.dig_P1 = ( (rxBME280_Buffer[1] << 8) | rxBME280_Buffer[0] );
	txBME280_Buffer[0] = BME280_REGISTER_DIG_P2;
	I2C_transfer(i2c, &i2c_BME280_Transaction);
	_BME280_calib.dig_P2 = ( (rxBME280_Buffer[1] << 8) | rxBME280_Buffer[0] );
	txBME280_Buffer[0] = BME280_REGISTER_DIG_P3;
	I2C_transfer(i2c, &i2c_BME280_Transaction);
	_BME280_calib.dig_P3 = ( (rxBME280_Buffer[1] << 8) | rxBME280_Buffer[0] );
	txBME280_Buffer[0] = BME280_REGISTER_DIG_P4;
	I2C_transfer(i2c, &i2c_BME280_Transaction);
	_BME280_calib.dig_P4 = ( (rxBME280_Buffer[1] << 8) | rxBME280_Buffer[0] );
	txBME280_Buffer[0] = BME280_REGISTER_DIG_P5;
	I2C_transfer(i2c, &i2c_BME280_Transaction);
	_BME280_calib.dig_P5 = ( (rxBME280_Buffer[1] << 8) | rxBME280_Buffer[0] );
	txBME280_Buffer[0] = BME280_REGISTER_DIG_P6;
	I2C_transfer(i2c, &i2c_BME280_Transaction);
	_BME280_calib.dig_P6 = ( (rxBME280_Buffer[1] << 8) | rxBME280_Buffer[0] );
	txBME280_Buffer[0] = BME280_REGISTER_DIG_P7;
	I2C_transfer(i2c, &i2c_BME280_Transaction);
	_BME280_calib.dig_P7 = ( (rxBME280_Buffer[1] << 8) | rxBME280_Buffer[0] );
	txBME280_Buffer[0] = BME280_REGISTER_DIG_P8;
	I2C_transfer(i2c, &i2c_BME280_Transaction);
	_BME280_calib.dig_P8 = ( (rxBME280_Buffer[1] << 8) | rxBME280_Buffer[0] );
	txBME280_Buffer[0] = BME280_REGISTER_DIG_P9;
	I2C_transfer(i2c, &i2c_BME280_Transaction);
	_BME280_calib.dig_P9 = ( (rxBME280_Buffer[1] << 8) | rxBME280_Buffer[0] );

	i2c_BME280_Transaction.readCount = 1;
	txBME280_Buffer[0] = BME280_REGISTER_DIG_H1;
	I2C_transfer(i2c, &i2c_BME280_Transaction);
	_BME280_calib.dig_H1 = ( rxBME280_Buffer[0] );
	i2c_BME280_Transaction.readCount = 2;
	txBME280_Buffer[0] = BME280_REGISTER_DIG_H2;
	I2C_transfer(i2c, &i2c_BME280_Transaction);
	_BME280_calib.dig_H2 = ( (rxBME280_Buffer[1] << 8) | rxBME280_Buffer[0] );
	i2c_BME280_Transaction.readCount = 1;
	txBME280_Buffer[0] = BME280_REGISTER_DIG_H3;
	I2C_transfer(i2c, &i2c_BME280_Transaction);
	_BME280_calib.dig_H3 = ( rxBME280_Buffer[0] );
	i2c_BME280_Transaction.readCount = 2;
	txBME280_Buffer[0] = BME280_REGISTER_DIG_H4;
	I2C_transfer(i2c, &i2c_BME280_Transaction);
	_BME280_calib.dig_H4 = ( (rxBME280_Buffer[0] << 4) | (rxBME280_Buffer[1] & 0x0F) );
	i2c_BME280_Transaction.readCount = 2;
	txBME280_Buffer[0] = BME280_REGISTER_DIG_H5;
	I2C_transfer(i2c, &i2c_BME280_Transaction);
	_BME280_calib.dig_H5 = ( (rxBME280_Buffer[1] << 4) | ((_BME280_calib.dig_H4 & 0x000F)) );
	i2c_BME280_Transaction.readCount = 1;
	txBME280_Buffer[0] = BME280_REGISTER_DIG_H6;
	I2C_transfer(i2c, &i2c_BME280_Transaction);
	_BME280_calib.dig_H6 = ( rxBME280_Buffer[0] );

}

uint8_t BME280_init(void){
	// Point to the T ambient register and read its 2 bytes
	//txTMP007_Buffer[0] = 0x01;

	uint16_t id = 0;
	int8_t bme280_initCount = 1;

	while(id != 0x60 && bme280_initCount < 0 ){
		id = BME280_read8(BME280_CHIP_ID);
		bme280_initCount--;
	}


	/*if(id != 0x60){
		return 0;
	}*/

	BME280_readCoefficients();

	BME280_write8(BME280_CTRL_HUM, 0x05);
	BME280_write8(BME280_CTRL_MEAS, 0xB7);

	return 1;
	//BME280_readCoeffiecients();

}

int32_t get_BME280_Temp(void){

	int32_t adc_T = 0;
	int32_t var1 = 0, var2 = 0;
	int32_t Temp = 0;

	i2c_BME280_Transaction.slaveAddress = BME280_SLAVE_ADDRESS_1;
	i2c_BME280_Transaction.writeBuf = txBME280_Buffer;
	i2c_BME280_Transaction.writeCount = 1;
	i2c_BME280_Transaction.readBuf = rxBME280_Buffer;
	i2c_BME280_Transaction.readCount = 3;

	txBME280_Buffer[0] = BME280_TEMP_MSB;

	I2C_transfer(i2c, &i2c_BME280_Transaction);

	adc_T = rxBME280_Buffer[0];
	adc_T <<= 8;
	adc_T |= rxBME280_Buffer[1];
	adc_T <<= 8;
	adc_T |= rxBME280_Buffer[2];
	adc_T >>= 4;

	var1  = ((((adc_T>>3) - ((int32_t)_BME280_calib.dig_T1 <<1))) *
			((int32_t)_BME280_calib.dig_T2)) >> 11;

	var2  = (((((adc_T>>4) - ((int32_t)_BME280_calib.dig_T1)) *
			((adc_T>>4) - ((int32_t)_BME280_calib.dig_T1))) >> 12) *
			((int32_t)_BME280_calib.dig_T3)) >> 14;

	t_fine = (var1 + var2);

	Temp = (t_fine * 5 + 128) >> 8;

	return Temp;
}

uint32_t get_BME280_Press(void){

	int32_t adc_P;
	int32_t var1 = 0, var2 = 0;
	uint32_t p = 0;

	get_BME280_Temp();

	i2c_BME280_Transaction.slaveAddress = BME280_SLAVE_ADDRESS_1;
	i2c_BME280_Transaction.writeBuf = txBME280_Buffer;
	i2c_BME280_Transaction.writeCount = 1;
	i2c_BME280_Transaction.readBuf = rxBME280_Buffer;
	i2c_BME280_Transaction.readCount = 3;

	txBME280_Buffer[0] = BME280_PRESS_MSB;
	if(I2C_transfer(i2c, &i2c_BME280_Transaction));

	adc_P = rxBME280_Buffer[0];
	adc_P <<= 8;
	adc_P |= rxBME280_Buffer[1];
	adc_P <<= 8;
	adc_P |= rxBME280_Buffer[2];
	adc_P >>= 4;

	var1 = (((int32_t)t_fine)>>1) - (int32_t)64000;
	var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)_BME280_calib.dig_P6);
	var2 = var2 + ((var1*((int32_t)_BME280_calib.dig_P5))<<1);
	var2 = (var2>>2)  + (((int32_t)_BME280_calib.dig_P4)<<16);
	var1 = ((((int32_t)_BME280_calib.dig_P3 * (((var1>>2) * (var1>>2)) >> 13)) >> 3) + ((((int32_t)_BME280_calib.dig_P2) * var1) >> 1)) >> 18;
	var1 = ((((32768+var1))*((uint32_t)_BME280_calib.dig_P1)) >> 15);

	if (var1 == 0) {
		return 0;  // avoid exception caused by division by zero
	}
	p = (((uint32_t)(((int32_t)1048576)-adc_P)-(var2>>12)))*3125;

	if(p < 0x80000000){
		p = (p << 1) / ((uint32_t)var1);
	}else{
		p = (p / (uint32_t)var1) * 2;
	}
	var1 = (((int32_t)_BME280_calib.dig_P9) * ((int32_t)(((p>>3) * (p>>3)) >> 13))) >> 12;
	var2 = (((int32_t)(p>>2)) * ((int32_t)_BME280_calib.dig_P8)) >> 13;

	p = (uint32_t)((int32_t)p + ((var1 + var2 + _BME280_calib.dig_P7) >> 4));

	return p;
}

uint32_t get_BME280_Humidity(void){

	int32_t adc_H;
	int32_t var1 = 0;
	//uint32_t h = 0;

	get_BME280_Temp();

	i2c_BME280_Transaction.slaveAddress = BME280_SLAVE_ADDRESS_1;
	i2c_BME280_Transaction.writeBuf = txBME280_Buffer;
	i2c_BME280_Transaction.writeCount = 1;
	i2c_BME280_Transaction.readBuf = rxBME280_Buffer;
	i2c_BME280_Transaction.readCount = 2;

	txBME280_Buffer[0] = BME280_HUM_MSB;
	if(I2C_transfer(i2c, &i2c_BME280_Transaction));

	adc_H = rxBME280_Buffer[0];
	adc_H <<= 8;
	adc_H |= rxBME280_Buffer[1];

	var1 = ((int32_t)t_fine - (int32_t)76800);
	var1 = (((((adc_H << 14) - (((int32_t) _BME280_calib.dig_H4)	<< 20) - (((int32_t) _BME280_calib.dig_H5) * var1)) + ((int32_t)16384)) >> 15)
			* (((((((var1 * ((int32_t)_BME280_calib.dig_H6)) >> 10) * (((var1 * ((int32_t)_BME280_calib.dig_H3)) >> 11) + ((int32_t)32768))) >> 10)
					+ ((int32_t)2097152)) * ((int32_t)_BME280_calib.dig_H2) + 8192) >> 14));

	var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)_BME280_calib.dig_H1)) >> 4));

	var1 = ((var1 < 0) ? 0 : var1);
	var1 = ((var1 > 419430400) ? 419430400 : var1);


	return (uint32_t)(var1 >> 12);
}


/*int32_t get_BME280_Press(void){

	int32_t adc_P;
	int64_t var1 = 0, var2 = 0, p = 0;

	get_BME280_Temp();

	i2c_BME280_Transaction.slaveAddress = BME280_SLAVE_ADDRESS_1;
	i2c_BME280_Transaction.writeBuf = txBME280_Buffer;
	i2c_BME280_Transaction.writeCount = 1;
	i2c_BME280_Transaction.readBuf = rxBME280_Buffer;
	i2c_BME280_Transaction.readCount = 3;



	txBME280_Buffer[0] = BME280_PRESS_MSB;
	if(I2C_transfer(i2c, &i2c_BME280_Transaction));

	adc_P = rxBME280_Buffer[0];
	adc_P <<= 8;
	adc_P |= rxBME280_Buffer[1];
	adc_P <<= 8;
	adc_P |= rxBME280_Buffer[2];
	adc_P >>= 4;

	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)_BME280_calib.dig_P6;
	var2 = var2 + ((var1*(int64_t)_BME280_calib.dig_P5)<<17);
	var2 = var2 + (((int64_t)_BME280_calib.dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)_BME280_calib.dig_P3)>>8) +
			((var1 * (int64_t)_BME280_calib.dig_P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)_BME280_calib.dig_P1)>>33;

	if (var1 == 0) {
		return 0;  // avoid exception caused by division by zero
	}
	p = 1048576 - adc_P;
	p = (((p<<31) - var2)*3125) / var1;
	var1 = (((int64_t)_BME280_calib.dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((int64_t)_BME280_calib.dig_P8) * p) >> 19;

	p = ((p + var1 + var2) >> 8) + (((int64_t)_BME280_calib.dig_P7)<<4);

	return p;
}*/

int32_t BME280_readAltitude(void){

	//Change the "1050.35" to your city current barrometric pressure (https://www.wunderground.com)

	//float_t altitude
	float_t altitude = 0;
	float_t pressure = 0;

	pressure = get_BME280_Press();
	pressure /= 100;

	altitude = 44330 * (1.0 - pow(pressure / 1050.35, 0.1903)) *1000;

	return altitude;
}





uint8_t get_BME280_Id(void){

	uint8_t pressure = 0;

	txBME280_Buffer[0] = BME280_CHIP_ID;

	if(I2C_transfer(i2c, &i2c_BME280_Transaction)){

		pressure = rxBME280_Buffer[0];

	}

	return pressure;
}

uint8_t get_BME280_Status(void){

	uint8_t pressure = 0;

	txBME280_Buffer[0] = BME280_STATUS;

	if(I2C_transfer(i2c, &i2c_BME280_Transaction)){

		pressure = rxBME280_Buffer[0];

	}

	return pressure;
}

uint8_t set_BME280_Reset(void){

	BME280_write8(BME280_RESET, 0xB6);

	return 0;
}

uint8_t save_BME280_Press(struct bme280 *press){

	if ( Semaphore_getCount(pressDataHandle) == 0 )   {
		//putstr("\r\nWrite Pressure and Humidity Data blocked!\r\n");
	}else{
		Semaphore_pend(pressDataHandle, BIOS_WAIT_FOREVER);
		press->SensTemp = get_BME280_Temp();
		press->SensHumidity = get_BME280_Humidity();
		press->SensPress = get_BME280_Press();
		Semaphore_post(pressDataHandle);
	}

	if(press->SensTemp != 0 || press->SensHumidity != 0 || press->SensPress != 0){
		return 0;
	}else{
		return 1;
	}

}

uint8_t show_BME280_Press(struct bme280 *press){

	if ( Semaphore_getCount(pressDataHandle) == 0 )   {
		//putstr("\r\nRead Pressure and Humidity Data blocked!\r\n");
	}else{
		Semaphore_pend(pressDataHandle, BIOS_WAIT_FOREVER);
		putstr("\r\nTemp: ");
		putFloatData(press->SensTemp, 100);
		putstr(" C* Humidity: ");
		putFloatData(press->SensHumidity, 1000);
		putstr(" % Pressure: ");
		putFloatData(press->SensPress, 100);
		putstr(" hPa                      ");
		Semaphore_post(pressDataHandle);

	}
	return 0;
}
