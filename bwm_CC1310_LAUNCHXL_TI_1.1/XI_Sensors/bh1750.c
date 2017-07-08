/*
 * bh1750.c
 *
 *  Created on: 19.01.2017
 *      Author: MyDev
 */

//#include "Board.h"
//#include "math.h"

#include <XI_Sensors/sensors.h>
#include "XI_Bios/bios.h"



//#include <ti/drivers/UART.h>
//#include <ti/drivers/SPI.h>
//#include <ti/drivers/spi/SPICC26XXDMA.h>
//#include <ti/drivers/I2C.h>

//#include "bh1750.h"
//#include "bios_CC1310.h"

uint8_t BH1750_init(void){

	uint8_t mode = BH1750_CONTINUOUS_HIGH_RES_MODE;

	i2c_BH1750_Transaction.slaveAddress = BH1750_SLAVE_ADDRESS_1;
	i2c_BH1750_Transaction.writeBuf = txBH1750_Buffer;
	i2c_BH1750_Transaction.writeCount = 1;
	i2c_BH1750_Transaction.readBuf = rxBH1750_Buffer;
	i2c_BH1750_Transaction.readCount = 0;

	//txBH1750_Buffer[0] = BH1750_POWER_ON;
	//I2C_transfer(i2c, &i2c_BH1750_Transaction);

	//__delay_ms(100);

	switch (mode) {
	case BH1750_CONTINUOUS_HIGH_RES_MODE:
	case BH1750_CONTINUOUS_HIGH_RES_MODE_2:
	case BH1750_CONTINUOUS_LOW_RES_MODE:
	case BH1750_ONE_TIME_HIGH_RES_MODE:
	case BH1750_ONE_TIME_HIGH_RES_MODE_2:
	case BH1750_ONE_TIME_LOW_RES_MODE:

		txBH1750_Buffer[0] = mode;
		I2C_transfer(i2c, &i2c_BH1750_Transaction);

		//__delay_ms(120);

		break;

	default:
		break;
	}


	return 1;
}

uint16_t get_BH1750_LightLevel(void){

	uint16_t level;

	//BH1750_init();

	i2c_BH1750_Transaction.slaveAddress = BH1750_SLAVE_ADDRESS_1;
	i2c_BH1750_Transaction.writeBuf = txBH1750_Buffer;
	i2c_BH1750_Transaction.writeCount = 1;
	i2c_BH1750_Transaction.readBuf = rxBH1750_Buffer;
	i2c_BH1750_Transaction.readCount = 2;

	txBH1750_Buffer[0] = BH1750_CONTINUOUS_HIGH_RES_MODE;
	I2C_transfer(i2c, &i2c_BH1750_Transaction);

	level = rxBH1750_Buffer[0];
	level <<= 8;
	level |= rxBH1750_Buffer[1];

	level /= 1.2;


	return level;
}

uint8_t save_BH1750_Bright(struct bhl1750 *bright){


	if ( Semaphore_getCount(brightDataHandle) == 0 )   {
		//putstr("\r\nWrite Brightness Data blocked!\r\n");
	}else{
		Semaphore_pend(brightDataHandle, BIOS_WAIT_FOREVER);
		bright->SensBrightness = get_BH1750_LightLevel();
		Semaphore_post(brightDataHandle);
	}

	if( bright->SensBrightness != 0 ){
		return 0;
	}else{
		return 1;
	}

}

uint8_t show_BH1750_Bright(struct bhl1750 *bright){


	if ( Semaphore_getCount(brightDataHandle) == 0 )   {
		//putstr("\r\nRead Brightness Data blocked!\r\n");
	}else{
		Semaphore_pend(brightDataHandle, BIOS_WAIT_FOREVER);
		putstr("\r\nBrightness: ");
		putdata(bright->SensBrightness);
		putstr(" Lux                            ");
		Semaphore_post(brightDataHandle);
	}

		return 0;
}

