/*
 * tmp007.c
 *
 *  Created on: 30.12.2016
 *      Author: MyDev
 */

//#include <stdlib.h>
/*#include "Board.h"

#include <ti/drivers/UART.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPICC26XXDMA.h>
#include <ti/drivers/I2C.h>

#include "tmp007.h"
#include "bios_CC1310.h"*/
#include "../XI_Sensors/sensors.h"
#include "../XI_Bios/bios.h"


void TMP007_init(void){
	// Point to the T ambient register and read its 2 bytes
	//txTMP007_Buffer[0] = 0x01;
	i2c_TMP007_Transaction.slaveAddress = TMP007_SLAVE_ADDRESS;
	i2c_TMP007_Transaction.writeBuf = txTMP007_Buffer;
	i2c_TMP007_Transaction.writeCount = 1;
	i2c_TMP007_Transaction.readBuf = rxTMP007_Buffer;
	i2c_TMP007_Transaction.readCount = 2;

	TempSensor.i2c_address = TMP007_SLAVE_ADDRESS;
}


/*for(i=0; i<20; i++){
	if(I2C_transfer(i2c, &i2cTransaction)){
		temperature = (rxBuffer[0] << 6) | (rxBuffer[1] >> 2);

		// If the MSB is set '1' then we have a 2's complement
		// negative value which needs to be sign extended
		if(rxBuffer[0] & 0x80){
			temperature |= 0xF000;
		}
		// For simplicity divide the temperature value by 32 to get rid of
		// the decimal precision; see TI´s TMP006 datasheet
		temperature /= 32;

		putstr("Sample ");
		putdata(i);
		putstr(" - ");
		putdata(temperature);
		putstr(" *C                 \r\n");
		//System_printf("Sample %u: %d (C)\n", i, temperature);
	}else{
		putstr("I2C Bus fault\r\n");
		//System_printf("I2C Bus fault\n");
	}
}*/

uint32_t get_TMP007_SensTemp(void){

	uint32_t temperature = 0;

	txTMP007_Buffer[0] = TMP007_ATRR;

	if(I2C_transfer(i2c, &i2c_TMP007_Transaction)){


		temperature = (rxTMP007_Buffer[0] << 6) | (rxTMP007_Buffer[1] >> 2);

		// If the MSB is set '1' then we have a 2's complement
		// negative value which needs to be sign extended
		if(rxTMP007_Buffer[0] & 0x80){
			temperature |= 0xF000;
		}
		// For simplicity divide the temperature value by 32 to get rid of
		// the decimal precision; see TI´s TMP006 datasheet
		temperature *= 100;
		temperature /= 32;

	}

	return temperature;
}

uint32_t get_TMP007_ObjTemp(void){

	uint32_t temperature = 0;

	txTMP007_Buffer[0] = TMP007_OTRR;

	if(I2C_transfer(i2c, &i2c_TMP007_Transaction)){

		temperature = (rxTMP007_Buffer[0] << 6) | (rxTMP007_Buffer[1] >> 2);

		// If the MSB is set '1' then we have a 2's complement
		// negative value which needs to be sign extended
		if(rxTMP007_Buffer[0] & 0x80){
			temperature |= 0xF000;
		}
		// For simplicity divide the temperature value by 32 to get rid of
		// the decimal precision; see TI´s TMP006 datasheet
		temperature *= 100;
		temperature /= 32;

	}

	return temperature;
}

uint8_t save_TMP007_Temp(struct tmp007 *temp){

	if ( Semaphore_getCount(tempDataHandle) == 0 )   {
		//putstr("\r\nWrite Temperature Data blocked!\r\n");
	}else{
		Semaphore_pend(tempDataHandle, BIOS_WAIT_FOREVER);
		temp->ObjTemp = get_TMP007_ObjTemp();
		temp->SensTemp = get_TMP007_SensTemp();
		Semaphore_post(tempDataHandle);
	}

	if( temp->ObjTemp != 0 || temp->SensTemp != 0 ){
		return 0;
	}else{
		return 1;
	}

}

uint8_t show_TMP007_Temp(struct tmp007 *temp){

	if ( Semaphore_getCount(tempDataHandle) == 0 )   {
		//putstr("\r\nRead Temperature Data blocked!\r\n");
	}else{
		Semaphore_pend(tempDataHandle, BIOS_WAIT_FOREVER);
		putstr("\r\nObjTemp: ");
		putFloatData(temp->ObjTemp, 100);
		putstr(" C* SensTemp: ");
		putFloatData(temp->SensTemp, 100);
		putstr(" C*                     ");
		Semaphore_post(tempDataHandle);
	}

	return 0;

}

