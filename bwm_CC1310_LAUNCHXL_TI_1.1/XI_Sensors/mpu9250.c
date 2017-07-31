/*
 * mpu9250.c
 *
 *  Created on: 24.06.2017
 *      Author: MyDev
 */

//===================================================================================================================
//====== Set of useful function to access acceleration. gyroscope, magnetometer, and temperature data
//===================================================================================================================

#include <XI_Sensors/sensors.h>
#include "XI_Bios/bios.h"
#include "mpu9250.h"


// Specify sensor full scale
uint8_t Gscale = GFS_250DPS;
uint8_t Ascale = AFS_2G;
uint8_t Mscale = MFS_16BITS; // Choose either 14-bit or 16-bit magnetometer resolution
uint8_t Mmode = 0x02;        // 2 for 8 Hz, 6 for 100 Hz continuous magnetometer data read
float aRes, gRes, mRes;      // scale resolutions per LSB for the sensors


uint8_t MPU9250_read8(uint8_t MPU9250_address, uint8_t MPU9250_reg, uint8_t MPU9250_buffer_size){

	i2c_MPU9250_Transaction.slaveAddress = 0x69;
	i2c_MPU9250_Transaction.writeBuf = txMPU9250_Buffer;
	i2c_MPU9250_Transaction.writeCount = 1;
	i2c_MPU9250_Transaction.readBuf = rxMPU9250_Buffer;
	i2c_MPU9250_Transaction.readCount = 1;

	txMPU9250_Buffer[0] = 0x75;
	//__delay_us(400);

	I2C_transfer(i2c, &i2c_MPU9250_Transaction);

	//__delay_us(400);

	/*if(MPU9250_buffer_size == 1){
		return rxMPU9250_Buffer[0];
	}else{*/
		return 0;
	//}
}

uint8_t MPU9250_write8(uint8_t MPU9250_address, uint8_t MPU9250_reg, uint8_t MPU9250_value){

	uint8_t value = 0;

	i2c_MPU9250_Transaction.slaveAddress = 0x68;
	i2c_MPU9250_Transaction.writeBuf = txMPU9250_Buffer;
	i2c_MPU9250_Transaction.writeCount = 2;
	i2c_MPU9250_Transaction.readBuf = rxMPU9250_Buffer;
	i2c_MPU9250_Transaction.readCount = 0;

	txMPU9250_Buffer[0] = MPU9250_reg;
	txMPU9250_Buffer[1] = MPU9250_value;

	I2C_transfer(i2c, &i2c_MPU9250_Transaction);

	return value;
}

void getMres(void) {
	switch (Mscale)
	{
	// Possible magnetometer scales (and their register bit settings) are:
	// 14 bit resolution (0) and 16 bit resolution (1)
	case MFS_14BITS:
		mRes = 10.*4912./8190.; // Proper scale to return milliGauss
		break;
	case MFS_16BITS:
		mRes = 10.*4912./32760.0; // Proper scale to return milliGauss
		break;
	}
}

void getGres(void) {
	switch (Gscale)
	{
	// Possible gyro scales (and their register bit settings) are:
	// 250 DPS (00), 500 DPS (01), 1000 DPS (10), and 2000 DPS  (11).
	// Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value:
	case GFS_250DPS:
		gRes = 250.0/32768.0;
		break;
	case GFS_500DPS:
		gRes = 500.0/32768.0;
		break;
	case GFS_1000DPS:
		gRes = 1000.0/32768.0;
		break;
	case GFS_2000DPS:
		gRes = 2000.0/32768.0;
		break;
	}
}

void getAres(void) {
	switch (Ascale)
	{
	// Possible accelerometer scales (and their register bit settings) are:
	// 2 Gs (00), 4 Gs (01), 8 Gs (10), and 16 Gs  (11).
	// Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value:
	case AFS_2G:
		aRes = 2.0/32768.0;
		break;
	case AFS_4G:
		aRes = 4.0/32768.0;
		break;
	case AFS_8G:
		aRes = 8.0/32768.0;
		break;
	case AFS_16G:
		aRes = 16.0/32768.0;
		break;
	}
}

void readAccelData(int16_t *destination)
{
	//uint8_t rawData[6];  // x/y/z accel register data stored here
	MPU9250_read8(MPU9250_ADDRESS, ACCEL_XOUT_H, 6);  // Read the six raw data registers into data array
	destination[0] = ((int16_t)rxMPU9250_Buffer[0] << 8) | rxMPU9250_Buffer[1] ;  // Turn the MSB and LSB into a signed 16-bit value
	destination[1] = ((int16_t)rxMPU9250_Buffer[2] << 8) | rxMPU9250_Buffer[3] ;
	destination[2] = ((int16_t)rxMPU9250_Buffer[4] << 8) | rxMPU9250_Buffer[5] ;
}

void readGyroData(int16_t *destination)
{
	//uint8_t rawData[6];  // x/y/z gyro register data stored here
	MPU9250_read8(MPU9250_ADDRESS, GYRO_XOUT_H, 6);	 // Read the six raw data registers sequentially into data array
	destination[0] = ((uint16_t) rxMPU9250_Buffer[0] << 8) | rxMPU9250_Buffer[1];	// Turn the MSB and LSB into a signed 16-bit value
	destination[1] = ((uint16_t) rxMPU9250_Buffer[2] << 8) | rxMPU9250_Buffer[3];
	destination[2] = ((uint16_t) rxMPU9250_Buffer[4] << 8) | rxMPU9250_Buffer[5];
}

void readMagData(int16_t *destination)
{
	//uint8_t c = 0;
	//uint8_t rawData[7];  // x/y/z gyro register data, ST2 register stored here, must read ST2 at end of data acquisition
	MPU9250_read8(AK8963_ADDRESS, AK8963_ST1, 1);	 // Read the six raw data registers sequentially into data array
	if(rxMPU9250_Buffer[0] & 0x01){
		MPU9250_read8(AK8963_ADDRESS, AK8963_XOUT_L, 7);	// Read the six raw data and ST2 registers sequentially into data array
		if(!(rxMPU9250_Buffer[6] & 0x08)){	// Check if magnetic sensor overflow set, if not then report data
			destination[0] = ((int16_t)rxMPU9250_Buffer[1] << 8) | rxMPU9250_Buffer[0] ;  // Turn the MSB and LSB into a signed 16-bit value
			destination[1] = ((int16_t)rxMPU9250_Buffer[3] << 8) | rxMPU9250_Buffer[2] ;  // Data stored as little Endian
			destination[2] = ((int16_t)rxMPU9250_Buffer[5] << 8) | rxMPU9250_Buffer[4] ;
		}
	}
}

int16_t readTempData(void)
{
  //uint8_t rawData[2];  // x/y/z gyro register data stored here
	MPU9250_read8(MPU9250_ADDRESS, TEMP_OUT_H, 2);  // Read the two raw data registers sequentially into data array
  return ((int16_t)rxMPU9250_Buffer[0] << 8) | rxMPU9250_Buffer[1] ;  // Turn the MSB and LSB into a 16-bit value
}

// Accelerometer and gyroscope self test; check calibration wrt factory settings
void MPU9250_self_test(float *destination){	// Should return percent deviation from factory trim values, +/- 14 or less deviation is a pas

	uint8_t rawData[6] = {0, 0, 0, 0, 0, 0};
	uint8_t selfTest[6];
	int32_t gAvg[3] = {0}, aAvg[3] = {0}, aSTAvg[3] = {0}, gSTAvg[3] = {0};
	float factoryTrim[6];
	uint8_t FS = 0;
	int i = 0;

	MPU9250_write8(MPU9250_ADDRESS, SMPLRT_DIV, 0x00);		// Set gyro sample rate to 1 kHz
	MPU9250_write8(MPU9250_ADDRESS, CONFIG, 0x02);			// Set gyro sample rate to 1 kHz and DLPF to 92 Hz
	MPU9250_write8(MPU9250_ADDRESS, GYRO_CONFIG, FS<<3);		// Set full scale range for the gyro to 250 dps
	MPU9250_write8(MPU9250_ADDRESS, ACCEL_CONFIG2, 0x02);	// Set accelerometer rate to 1 kHz and bandwidth to 92 Hz
	MPU9250_write8(MPU9250_ADDRESS, ACCEL_CONFIG, FS<<3);	// Set full scale range for the accelerometer to 2 g


	for(i = 0; i < 200; i++){		// get average current values of gyro and acclerometer

		MPU9250_read8(MPU9250_ADDRESS, ACCEL_XOUT_H, 6);	// Read the six raw data registers into data array
		aAvg[0] += (int16_t)((int16_t)rxMPU9250_Buffer[0] << 8 | rxMPU9250_Buffer[1]);	// Turn the MSB and LSB into a signed 16-bit value
		aAvg[1] += (int16_t)((int16_t)rxMPU9250_Buffer[2] << 8 | rxMPU9250_Buffer[3]);
		aAvg[2] += (int16_t)((int16_t)rxMPU9250_Buffer[4] << 8 | rxMPU9250_Buffer[5]);

		MPU9250_read8(MPU9250_ADDRESS, GYRO_XOUT_H, 6);	// Read the six raw data registers sequentially into data array
		aAvg[0] += (int16_t)((int16_t)rxMPU9250_Buffer[0] << 8 | rxMPU9250_Buffer[1]);	// Turn the MSB and LSB into a signed 16-bit value
		aAvg[1] += (int16_t)((int16_t)rxMPU9250_Buffer[2] << 8 | rxMPU9250_Buffer[3]);
		aAvg[2] += (int16_t)((int16_t)rxMPU9250_Buffer[4] << 8 | rxMPU9250_Buffer[5]);

	}

	for(i = 0; i < 3; i++){
		aAvg[i] /= 200;
		gAvg[i] /= 200;
	}

	// Configure the accelerometer for self-test
	MPU9250_write8(MPU9250_ADDRESS, ACCEL_CONFIG, 0xE0);	// Enable self test on all three axes and set accelerometer range to +/- 2 g
	MPU9250_write8(MPU9250_ADDRESS, GYRO_CONFIG, 0xE0);		// Enable self test on all three axes and set gyro range to +/- 250 degrees/s
	__delay_ms(25);

	for(i = 0; i < 200; i++){

		MPU9250_read8(MPU9250_ADDRESS, ACCEL_XOUT_H, 6);	// Read the six raw data registers into data array
		aSTAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
		aSTAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
		aSTAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;

		MPU9250_read8(MPU9250_ADDRESS, GYRO_XOUT_H, 6);	 // Read the six raw data registers sequentially into data array
		gSTAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
		gSTAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
		gSTAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;

	}

	for (i =0; i < 3; i++) {  // Get average of 200 values and store as average self-test readings
		aSTAvg[i] /= 200;
		gSTAvg[i] /= 200;
	}

	// Configure the gyro and accelerometer for normal operation

	MPU9250_write8(MPU9250_ADDRESS, ACCEL_CONFIG, 0x00);
	MPU9250_write8(MPU9250_ADDRESS, GYRO_CONFIG, 0x00);
	__delay_ms(25);	// Delay a while to let the device stabilize

	MPU9250_read8(MPU9250_ADDRESS, SELF_TEST_X_ACCEL, 1);	// X-axis accel self-test results
	selfTest[0] = rxMPU9250_Buffer[0];
	MPU9250_read8(MPU9250_ADDRESS, SELF_TEST_Y_ACCEL, 1);	// Y-axis accel self-test results
	selfTest[1] = rxMPU9250_Buffer[0];
	MPU9250_read8(MPU9250_ADDRESS, SELF_TEST_Z_ACCEL, 1);	// Z-axis accel self-test results
	selfTest[2] = rxMPU9250_Buffer[0];
	MPU9250_read8(MPU9250_ADDRESS, SELF_TEST_X_GYRO, 1);	// X-axis gyro self-test results
	selfTest[3] = rxMPU9250_Buffer[0];
	MPU9250_read8(MPU9250_ADDRESS, SELF_TEST_Y_GYRO, 1);	// Y-axis gyro self-test results
	selfTest[4] = rxMPU9250_Buffer[0];
	MPU9250_read8(MPU9250_ADDRESS, SELF_TEST_Z_GYRO, 1);	// Z-axis gyro self-test results
	selfTest[5] = rxMPU9250_Buffer[0];

	// Retrieve factory self-test value from self-test code reads
	factoryTrim[0] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[0] - 1.0) )); // FT[Xa] factory trim calculation
	factoryTrim[1] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[1] - 1.0) )); // FT[Ya] factory trim calculation
	factoryTrim[2] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[2] - 1.0) )); // FT[Za] factory trim calculation
	factoryTrim[3] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[3] - 1.0) )); // FT[Xg] factory trim calculation
	factoryTrim[4] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[4] - 1.0) )); // FT[Yg] factory trim calculation
	factoryTrim[5] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[5] - 1.0) )); // FT[Zg] factory trim calculation

	// Report results as a ratio of (STR - FT)/FT; the change from Factory Trim of the Self-Test Response
	// To get percent, must multiply by 100
	for (i = 0; i < 3; i++) {
		destination[i]   = 100.0*((float)(aSTAvg[i] - aAvg[i]))/factoryTrim[i] - 100.;   // Report percent differences
		destination[i+3] = 100.0*((float)(gSTAvg[i] - gAvg[i]))/factoryTrim[i+3] - 100.; // Report percent differences
	}

}

// Function which accumulates gyro and accelerometer data after device initialization. It calculates the average
// of the at-rest readings and then loads the resulting offsets into accelerometer and gyro bias registers.

uint8_t MPU9250_calibrate(float *dest1, float *dest2){

	uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
	uint16_t i, packet_count, fifo_count;
	int32_t gyro_bias[3]  = {0, 0, 0}, accel_bias[3] = {0, 0, 0};

	// reset device
	MPU9250_write8(MPU9250_ADDRESS, PWR_MGMT_1, 0x80);	// Write a one to bit 7 reset bit; toggle reset device
	__delay_ms(100);

	// get stable time source; Auto select clock source to be PLL gyroscope reference if ready
	// else use the internal oscillator, bits 2:0 = 001
	MPU9250_write8(MPU9250_ADDRESS, PWR_MGMT_1, 0x01);
	MPU9250_write8(MPU9250_ADDRESS, PWR_MGMT_2, 0x00);
	__delay_ms(200);

	// Configure device for bias calculation
	MPU9250_write8(MPU9250_ADDRESS, INT_ENABLE, 0x00);		// Disable all interrupts
	MPU9250_write8(MPU9250_ADDRESS, FIFO_EN, 0x00);			// Disable FIFO
	MPU9250_write8(MPU9250_ADDRESS, PWR_MGMT_1, 0x00);		// Turn on internal clock source
	MPU9250_write8(MPU9250_ADDRESS, I2C_MST_CTRL, 0x00);	// Disable I2C master
	MPU9250_write8(MPU9250_ADDRESS, USER_CTRL, 0x00);		// Disable FIFO and I2C master modes
	MPU9250_write8(MPU9250_ADDRESS, USER_CTRL, 0x0C);		// Reset FIFO and DMP
	__delay_ms(15);

	// Configure MPU6050 gyro and accelerometer for bias calculation
	MPU9250_write8(MPU9250_ADDRESS, CONFIG, 0x01);			// Set low-pass filter to 188 Hz
	MPU9250_write8(MPU9250_ADDRESS, SMPLRT_DIV, 0x00);		// Set sample rate to 1 kHz
	MPU9250_write8(MPU9250_ADDRESS, GYRO_CONFIG, 0x00);		// Set gyro full-scale to 250 degrees per second, maximum sensitivity
	MPU9250_write8(MPU9250_ADDRESS, ACCEL_CONFIG, 0x00);	// Set accelerometer full-scale to 2 g, maximum sensitivity

	uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
	uint16_t  accelsensitivity = 16384;  // = 16384 LSB/g

	// Configure FIFO to capture accelerometer and gyro data for bias calculation
	MPU9250_write8(MPU9250_ADDRESS, USER_CTRL, 0x40);		// Enable FIFO
	MPU9250_write8(MPU9250_ADDRESS, FIFO_EN, 0x78);			// Enable gyro and accelerometer sensors for FIFO  (max size 512 bytes in MPU-9150)
	__delay_ms(40);		// accumulate 40 samples in 40 milliseconds = 480 bytes

	// At end of sample accumulation, turn off FIFO sensor read
	MPU9250_write8(MPU9250_ADDRESS, FIFO_EN, 0x00);
	MPU9250_read8(MPU9250_ADDRESS, FIFO_COUNTH, 2);
	fifo_count = ((uint16_t)rxMPU9250_Buffer[0] << 8) | rxMPU9250_Buffer[1];
	packet_count = fifo_count/12;// How many sets of full gyro and accelerometer data for averaging

	for(i = 0; i < packet_count; i++){

		int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
		MPU9250_read8(MPU9250_ADDRESS, FIFO_R_W, 12);	// read data for averaging
		accel_temp[0] = (int16_t) ( ((int16_t)rxMPU9250_Buffer[0] << 8) | rxMPU9250_Buffer[1] );	// Form signed 16-bit integer for each sample in FIFO
		accel_temp[1] = (int16_t) ( ((int16_t)rxMPU9250_Buffer[2] << 8) | rxMPU9250_Buffer[3] );
		accel_temp[2] = (int16_t) ( ((int16_t)rxMPU9250_Buffer[4] << 8) | rxMPU9250_Buffer[5] );
		gyro_temp[0] = (int16_t) ( ((int16_t)rxMPU9250_Buffer[6] << 8) | rxMPU9250_Buffer[7] );
		gyro_temp[1] = (int16_t) ( ((int16_t)rxMPU9250_Buffer[8] << 8) | rxMPU9250_Buffer[9] );
		gyro_temp[2] = (int16_t) ( ((int16_t)rxMPU9250_Buffer[10] << 8) | rxMPU9250_Buffer[11] );

		accel_bias[0] += (int32_t) accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
		accel_bias[1] += (int32_t) accel_temp[1];
		accel_bias[2] += (int32_t) accel_temp[2];
		gyro_bias[0]  += (int32_t) gyro_temp[0];
		gyro_bias[1]  += (int32_t) gyro_temp[1];
		gyro_bias[2]  += (int32_t) gyro_temp[2];

	}

	accel_bias[0] /= (int32_t) packet_count; // Normalize sums to get average count biases
	accel_bias[1] /= (int32_t) packet_count;
	accel_bias[2] /= (int32_t) packet_count;
	gyro_bias[0]  /= (int32_t) packet_count;
	gyro_bias[1]  /= (int32_t) packet_count;
	gyro_bias[2]  /= (int32_t) packet_count;

	if(accel_bias[2] > 0L) {accel_bias[2] -= (int32_t) accelsensitivity;}  // Remove gravity from the z-axis accelerometer bias calculation
	else {accel_bias[2] += (int32_t) accelsensitivity;}

	// Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
	data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
	data[1] = (-gyro_bias[0]/4)       & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
	data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
	data[3] = (-gyro_bias[1]/4)       & 0xFF;
	data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
	data[5] = (-gyro_bias[2]/4)       & 0xFF;

	MPU9250_write8(MPU9250_ADDRESS, XG_OFFSET_H, data[0]);
	MPU9250_write8(MPU9250_ADDRESS, XG_OFFSET_L, data[1]);
	MPU9250_write8(MPU9250_ADDRESS, YG_OFFSET_H, data[2]);
	MPU9250_write8(MPU9250_ADDRESS, YG_OFFSET_L, data[3]);
	MPU9250_write8(MPU9250_ADDRESS, ZG_OFFSET_H, data[4]);
	MPU9250_write8(MPU9250_ADDRESS, ZG_OFFSET_L, data[5]);

	// Output scaled gyro biases for display in the main program
	dest1[0] = (float) gyro_bias[0]/(float) gyrosensitivity;
	dest1[1] = (float) gyro_bias[1]/(float) gyrosensitivity;
	dest1[2] = (float) gyro_bias[2]/(float) gyrosensitivity;

	// Construct the accelerometer biases for push to the hardware accelerometer bias registers. These registers contain
	// factory trim values which must be added to the calculated accelerometer biases; on boot up these registers will hold
	// non-zero values. In addition, bit 0 of the lower byte must be preserved since it is used for temperature
	// compensation calculations. Accelerometer bias registers expect bias input as 2048 LSB per g, so that
	// the accelerometer biases calculated above must be divided by 8.

	int32_t accel_bias_reg[3] = {0, 0, 0}; // A place to hold the factory accelerometer trim biases
	MPU9250_read8(MPU9250_ADDRESS, XA_OFFSET_H, 2);
	accel_bias_reg[0] = (int32_t) (((uint16_t)txMPU9250_Buffer[0] << 8) | txMPU9250_Buffer[1]);
	MPU9250_read8(MPU9250_ADDRESS, YA_OFFSET_H, 2);
	accel_bias_reg[1] = (int32_t) (((uint16_t)txMPU9250_Buffer[0] << 8) | txMPU9250_Buffer[1]);
	MPU9250_read8(MPU9250_ADDRESS, ZA_OFFSET_H, 2);
	accel_bias_reg[2] = (int32_t) (((uint16_t)txMPU9250_Buffer[0] << 8) | txMPU9250_Buffer[1]);

	uint32_t mask = 1uL; // Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers
	uint8_t mask_bit[3] = {0, 0, 0}; // Define array to hold mask bit for each accelerometer bias axis

	for(i = 0; i < 3; i++) {
		if((accel_bias_reg[i] & mask)) mask_bit[i] = 0x01; // If temperature compensation bit is set, record that fact in mask_bit
	}

	// Construct total accelerometer bias, including calculated average accelerometer bias from above
	accel_bias_reg[0] -= (accel_bias[0]/8); // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
	accel_bias_reg[1] -= (accel_bias[1]/8);
	accel_bias_reg[2] -= (accel_bias[2]/8);

	data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
	data[1] = (accel_bias_reg[0])      & 0xFF;
	data[1] = data[1] | mask_bit[0]; // preserve temperature compensation bit when writing back to accelerometer bias registers
	data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
	data[3] = (accel_bias_reg[1])      & 0xFF;
	data[3] = data[3] | mask_bit[1]; // preserve temperature compensation bit when writing back to accelerometer bias registers
	data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
	data[5] = (accel_bias_reg[2])      & 0xFF;
	data[5] = data[5] | mask_bit[2]; // preserve temperature compensation bit when writing back to accelerometer bias registers

	// Apparently this is not working for the acceleration biases in the MPU-9250
	// Are we handling the temperature correction bit properly?
	// Push accelerometer biases to hardware registers
	MPU9250_write8(MPU9250_ADDRESS, XA_OFFSET_H, data[0]);
	MPU9250_write8(MPU9250_ADDRESS, XA_OFFSET_L, data[1]);
	MPU9250_write8(MPU9250_ADDRESS, YA_OFFSET_H, data[2]);
	MPU9250_write8(MPU9250_ADDRESS, YA_OFFSET_L, data[3]);
	MPU9250_write8(MPU9250_ADDRESS, ZA_OFFSET_H, data[4]);
	MPU9250_write8(MPU9250_ADDRESS, ZA_OFFSET_L, data[5]);

	// Output scaled accelerometer biases for display in the main program
	dest2[0] = (float)accel_bias[0]/(float)accelsensitivity;
	dest2[1] = (float)accel_bias[1]/(float)accelsensitivity;
	dest2[2] = (float)accel_bias[2]/(float)accelsensitivity;

	return 1;
}

uint8_t AK8963_init(float *destinations){

	// First extract the factory calibration for each magnetometer axis
	//int8_t rawData[3];  // x/y/z gyro calibration data stored here
	MPU9250_write8(AK8963_ADDRESS, AK8963_CNTL, 0x00);	// Power down magnetometer
	__delay_ms(10);
	MPU9250_write8(AK8963_ADDRESS, AK8963_CNTL, 0x0F);	// Enter Fuse ROM access mode
	__delay_ms(10);
	MPU9250_read8(AK8963_ADDRESS, AK8963_ASAX, 3);	// Read the x-, y-, and z-axis calibration values
	destinations[0] = (float)(rxMPU9250_Buffer[0] - 128)/256. + 1.;	// Return x-axis sensitivity adjustment values, etc.
	destinations[1] = (float)(rxMPU9250_Buffer[1] - 128)/256. + 1.;
	destinations[1] = (float)(rxMPU9250_Buffer[2] - 128)/256. + 1.;
	MPU9250_write8(AK8963_ADDRESS, AK8963_CNTL, 0x0F);	// Power down magnetometer
	__delay_ms(10);
	// Configure the magnetometer for continuous read and highest resolution
	// set Mscale bit 4 to 1 (0) to enable 16 (14) bit resolution in CNTL register,
	// and enable continuous mode data acquisition Mmode (bits [3:0]), 0010 for 8 Hz and 0110 for 100 Hz sample rates
	MPU9250_write8(AK8963_ADDRESS, AK8963_CNTL, Mscale << 4 | Mmode);
	__delay_ms(10);

	return 0;
}

uint8_t MPU9250_init(void){

	uint8_t c = 0;

	// wake up device
	MPU9250_write8(MPU9250_ADDRESS, PWR_MGMT_1, 0x00);
	__delay_ms(100);

	// get stable time source
	MPU9250_write8(MPU9250_ADDRESS, PWR_MGMT_1, 0x01);
	__delay_ms(200);

	// Configure Gyro and Thermometer
	// Disable FSYNC and set thermometer and gyro bandwidth to 41 and 42 Hz, respectively;
	// minimum delay time for this setting is 5.9 ms, which means sensor fusion update rates cannot
	// be higher than 1 / 0.0059 = 170 Hz
	// DLPF_CFG = bits 2:0 = 011; this limits the sample rate to 1000 Hz for both
	// With the MPU9250, it is possible to get gyro sample rates of 32 kHz (!), 8 kHz, or 1 kHz
	MPU9250_write8(MPU9250_ADDRESS, CONFIG, 0x03);

	// Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
	MPU9250_write8(MPU9250_ADDRESS, SMPLRT_DIV, 0x04);	// Use a 200 Hz rate; a rate consistent with the filter update rate
	// determined inset in CONFIG above

	// Set gyroscope full scale range
	// Range selects FS_SEL and GFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3
	MPU9250_read8(MPU9250_ADDRESS, GYRO_CONFIG, 1);	// get current GYRO_CONFIG register value
	c = rxMPU9250_Buffer[0];
	// c = c & ~0xE0; // Clear self-test bits [7:5]
	c = c & ~0x03; // Clear Fchoice bits [1:0]
	c = c & ~0x18; // Clear GFS bits [4:3]
	c = c | Gscale << 3; // Set full scale range for the gyro
	// c =| 0x00; // Set Fchoice for the gyro to 11 by writing its inverse to bits 1:0 of GYRO_CONFIG
	MPU9250_write8(MPU9250_ADDRESS, GYRO_CONFIG, c);	// Write new GYRO_CONFIG value to register

	// Set accelerometer full-scale range configuration
	MPU9250_read8(MPU9250_ADDRESS, ACCEL_CONFIG, 1);	// get current ACCEL_CONFIG register value
	c = rxMPU9250_Buffer[0];
	// c = c & ~0xE0; // Clear self-test bits [7:5]
	c = c & ~0x18;  // Clear AFS bits [4:3]
	c = c | Ascale << 3; // Set full scale range for the accelerometer
	MPU9250_write8(MPU9250_ADDRESS, ACCEL_CONFIG, c);	// Write new ACCEL_CONFIG register value

	// Set accelerometer sample rate configuration
	// It is possible to get a 4 kHz sample rate from the accelerometer by choosing 1 for
	// accel_fchoice_b bit [3]; in this case the bandwidth is 1.13 kHz
	MPU9250_read8(MPU9250_ADDRESS, ACCEL_CONFIG2, 1);
	c = rxMPU9250_Buffer[0];
	c = c & ~0x0F; // Clear accel_fchoice_b (bit 3) and A_DLPFG (bits [2:0])
	c = c | 0x03;  // Set accelerometer rate to 1 kHz and bandwidth to 41 Hz
	MPU9250_write8(MPU9250_ADDRESS, ACCEL_CONFIG2, c);	// Write new ACCEL_CONFIG2 register value
	// The accelerometer, gyro, and thermometer are set to 1 kHz sample rates,
	// but all these rates are further reduced by a factor of 5 to 200 Hz because of the SMPLRT_DIV setting

	// Configure Interrupts and Bypass Enable
	// Set interrupt pin active high, push-pull, hold interrupt pin level HIGH until interrupt cleared,
	// clear on read of INT_STATUS, and enable I2C_BYPASS_EN so additional chips
	// can join the I2C bus and all can be controlled by the Arduino as master
	MPU9250_write8(MPU9250_ADDRESS, INT_PIN_CFG, 0x22);
	MPU9250_write8(MPU9250_ADDRESS, INT_ENABLE, 0x01);	// Enable data ready (bit 0) interrupt
	__delay_ms(100);


	return 1;
}

