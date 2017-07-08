/*
 * bh1750.h
 *
 *  Created on: 19.01.2017
 *      Author: MyDev
 */

#ifndef BH1750_H_
#define BH1750_H_


#define BH1750_SLAVE_ADDRESS_0	0x23	// SDO Pull Down
#define BH1750_SLAVE_ADDRESS_1	0x5C	// SDO Pull Up

// No active state
#define BH1750_POWER_DOWN 0x00
// Wating for measurment command
#define BH1750_POWER_ON 0x01
// Reset data register value - not accepted in POWER_DOWN mode
#define BH1750_RESET 0x07

// Start measurement at 1lx resolution. Measurement time is approx 120ms.
#define BH1750_CONTINUOUS_HIGH_RES_MODE 0x10
// Start measurement at 0.5lx resolution. Measurement time is approx 120ms.
#define BH1750_CONTINUOUS_HIGH_RES_MODE_2 0x11
// Start measurement at 4lx resolution. Measurement time is approx 16ms.
#define BH1750_CONTINUOUS_LOW_RES_MODE 0x13
// Start measurement at 1lx resolution. Measurement time is approx 120ms.
// Device is automatically set to Power Down after measurement.
#define BH1750_ONE_TIME_HIGH_RES_MODE 0x20
// Start measurement at 0.5lx resolution. Measurement time is approx 120ms.
// Device is automatically set to Power Down after measurement.
#define BH1750_ONE_TIME_HIGH_RES_MODE_2 0x21
// Start measurement at 1lx resolution. Measurement time is approx 120ms.
// Device is automatically set to Power Down after measurement.
#define BH1750_ONE_TIME_LOW_RES_MODE 0x23

struct bhl1750{

	uint8_t i2c_address;
	uint8_t reserve1;		// reserve
	uint16_t SensBrightness;

};

struct bhl1750 BrightSensor;

Semaphore_Params brightDataParams;
Semaphore_Struct brightDataStruct;
Semaphore_Handle brightDataHandle;

I2C_Transaction i2c_BH1750_Transaction;

uint8_t txBH1750_Buffer[2];
uint8_t rxBH1750_Buffer[4];

uint8_t BH1750_init(void);
uint16_t get_BH1750_LightLevel(void);
uint8_t save_BH1750_Bright(struct bhl1750 *bright);
uint8_t show_BH1750_Bright(struct bhl1750 *bright);


#endif /* BH1750_H_ */
