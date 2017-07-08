/*
 * bmp280.h
 *
 *  Created on: 03.01.2017
 *      Author: MyDev
 */

#ifndef BMP280_H_
#define BMP280_H_

#define BMP280_SLAVE_ADDRESS_0	0x76	// SDO Pull Down
#define BMP280_SLAVE_ADDRESS_1	0x77	// SDO Pull Up

// BMP280 REGISTER ADRESS
#define BMP280_CHIP_ID			0xD0	// chip identification number
#define BMP280_RESET			0xE0	// reset
#define BMP280_STATUS			0xF3	// status of the device
#define BMP280_CTRL_MEAS		0xF4	// data acquisition options of device
#define BMP280_CONFIG			0xF5	// Configuration
#define BMP280_PRESS_MSB		0xF7	// pressure measurement MSB [7:0]
#define BMP280_PRESS_LSB		0xF8	// pressure measurement LSB [7:0]
#define BMP280_PRESS_XLSB		0xF9	// pressure measurement XLSB [3:0]
#define BMP280_TEMP_MSB			0xFA	// temperature measurement MSB [7:0]
#define BMP280_TEMP_LSB			0xFB	// temperature measurement LSB [7:0]
#define BMP280_TEMP_XLSB		0xFC	// temperature measurement XLSB [3:0]

// BMP280 CTRL MEAS RESGISTER SETTINGS	0xF4
#define BMP280_CTRL_MEAS_0		0x00	// Skipped (output set to 0x80000)
#define BMP280_CTRL_MEAS_1		0x01	// oversampling x 1		/ 16 bit / 2.62 Pa
#define BMP280_CTRL_MEAS_2		0x02	// oversampling x 2		/ 17 bit / 1.31 Pa
#define BMP280_CTRL_MEAS_4		0x03	// oversampling x 4		/ 18 bit / 0.66 Pa
#define BMP280_CTRL_MEAS_8		0x04	// oversampling x 8		/ 19 bit / 0.33 Pa
#define BMP280_CTRL_MEAS_16		0x07	// oversampling x 16	/ 20 bit / 0.16 Pa

// BMP280 CONFIG RESGISTER SETTINGS		0xF5
#define BMP280_CONFIG_SPI3W_ON		0x01	// Enables 3-wire SPI interface when set to '1'
#define BMP280_CONFIG_SPI3W_OFF		0x00	// Enables 3-wire SPI interface when set to '1'

#define BMP280_CONFIG_FILTER_2		0x08	// Filter coefficient 2
#define BMP280_CONFIG_FILTER_4		0x0C	// Filter coefficient 4
#define BMP280_CONFIG_FILTER_8		0x10	// Filter coefficient 8
#define BMP280_CONFIG_FILTER_16		0x1C	// Filter coefficient 16

#define BMP280_CONFIG_SB_0_5		0x00	// standby time 0.5 ms
#define BMP280_CONFIG_SB_62_5		0x20	// standby time 62.5 ms
#define BMP280_CONFIG_SB_125		0x40	// standby time 125 ms
#define BMP280_CONFIG_SB_250		0x60	// standby time 250 ms
#define BMP280_CONFIG_SB_500		0x80	// standby time 500 ms
#define BMP280_CONFIG_SB_1000		0xA0	// standby time 1000 ms
#define BMP280_CONFIG_SB_2000		0xC0	// standby time 2000 ms
#define BMP280_CONFIG_SB_4000		0xE0	// standby time 4000 ms

// BMP280 Sample trimming values
#define BMP280_REGISTER_DIG_T1		0x88
#define BMP280_REGISTER_DIG_T2		0x8A
#define BMP280_REGISTER_DIG_T3		0x8C

#define BMP280_REGISTER_DIG_P1		0x8E
#define BMP280_REGISTER_DIG_P2		0x90
#define BMP280_REGISTER_DIG_P3		0x92
#define BMP280_REGISTER_DIG_P4		0x94
#define BMP280_REGISTER_DIG_P5		0x96
#define BMP280_REGISTER_DIG_P6		0x98
#define BMP280_REGISTER_DIG_P7		0x9A
#define BMP280_REGISTER_DIG_P8		0x9C
#define BMP280_REGISTER_DIG_P9		0x9E




// Callibration Data
typedef struct{

	uint16_t	dig_T1;
	int16_t		dig_T2;
	int16_t		dig_T3;

	uint16_t	dig_P1;
	int16_t		dig_P2;
	int16_t		dig_P3;
	int16_t		dig_P4;
	int16_t		dig_P5;
	int16_t		dig_P6;
	int16_t		dig_P7;
	int16_t		dig_P8;
	int16_t		dig_P9;

	uint16_t	dig_H1;
	int16_t		dig_H2;
	int8_t		dig_H3;
	int16_t		dig_H4;
	int16_t		dig_H5;
	int8_t		dig_H6;

}bmp280_calib_data;


I2C_Transaction i2c_BMP280_Transaction;

uint8_t txBMP280_Buffer[3];
uint8_t rxBMP280_Buffer[3];

int32_t t_fine;

uint8_t bmp280_init(void);
int32_t get_BMP280_Temp(void);
uint32_t get_BMP280_Press(void);
int32_t readAltitude(void);
uint8_t get_BMP280_Id(void);
uint8_t get_BMP280_Status(void);
uint8_t set_BMP280_Reset(void);

bmp280_calib_data _bmp280_calib;


#endif /* BMP280_H_ */
