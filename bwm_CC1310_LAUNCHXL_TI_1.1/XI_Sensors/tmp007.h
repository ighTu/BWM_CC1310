/*
 * tmp007.h
 *
 *  Created on: 30.12.2016
 *      Author: 5igi
 */

#ifndef TMP007_H_
#define TMP007_H_


/*
 * Register Name from RMP007EVM User Guide (Rev. A)
 */

#define TMP007_SLAVE_ADDRESS	0x40

#define TMP007_SVRR		0x00 	// Sensor Voltage Result Register
#define TMP007_ATRR		0x01	// Ambient Temperature Result Register
#define TMP007_CR		0c02	// Configuration Resgister
#define TMP007_OTRR		0x03	// Object Temperature Result Register
#define TMP007_SR		0x04	// Status Register
#define TMP007_MER		0x05	// Mask/En Register
#define TMP007_OTHLR	0x06	// Object Temperature High Limit Register
#define TMO007_OTLLR	0x07	// Object Temperature Low Limit Register
#define TMP007_LTHLR	0x08	// Local Temperature High Limit Register
#define TMP007_LTLLR	0x09	// Local Temperature Low Limit Register

struct tmp007{

	uint8_t i2c_address;
	uint8_t reserve1;
	uint8_t reserve2;
	uint8_t reserve3;
	uint32_t SensTemp;
	uint32_t ObjTemp;

};

struct tmp007 TempSensor;

Semaphore_Params tempDataParams;
Semaphore_Struct tempDataStruct;
Semaphore_Handle tempDataHandle;



I2C_Transaction i2c_TMP007_Transaction;

uint8_t txTMP007_Buffer[1];
uint8_t rxTMP007_Buffer[2];

void TMP007_init(void);
uint32_t get_TMP007_SensTemp(void);
uint32_t get_TMP007_ObjTemp(void);

uint8_t save_TMP007_Temp(struct tmp007 *temp);
uint8_t show_TMP007_Temp(struct tmp007 *temp);


#endif /* TMP007_H_ */
