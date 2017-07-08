/*
 * sensors.c
 *
 *  Created on: 07.02.2017
 *      Author: MyDev
 */

#include <XI_Sensors/sensors.h>
#include "XI_Bios/bios.h"


/*
 *  ======== sensorsTaskFxn ========
 *
 */
void sensorsTaskFxn(UArg arg0, UArg arg1)
{

	float test;
	int16_t dataXYZ[3];
	uint16_t temp = 0;


	// wait for unlock from main
	while(Semaphore_getCount(unlockSensorsHandle) == 0){
		/* We want to sleep for 1000000 microseconds */
		Task_sleep(1000000 / Clock_tickPeriod);
	}


	TMP007_init();
	BME280_init();
	BH1750_init();
	MPU9250_init();
	AK8963_init(&test);

	putstr("\r\nSensor Task running!\r\n");



	/* Loop forever echoing */
	while (1) {

		//putstr("\r\nSensor Task running! :-)\r\n\0");

		save_TMP007_Temp(&TempSensor);

		save_BME280_Press(&PressSensor);

		save_BH1750_Bright(&BrightSensor);

		readAccelData(dataXYZ);
		temp = readTempData();

		// 1 sec. delay
		Task_sleep(1000000 / Clock_tickPeriod);


	}
}

uint8_t init_sensorsSemaphore(void){

	// ---- init semaphore for sensor task
	Semaphore_Params_init(&unlockSensorsParams);
	Semaphore_construct(&unlockSensorsStruct, 1, &unlockSensorsParams);

	unlockSensorsHandle = Semaphore_handle(&unlockSensorsStruct);

	// lock sensor Task to wait for main task
	Semaphore_pend(unlockSensorsHandle, BIOS_WAIT_FOREVER);


	// ---- init semaphore for tempSensor Data handle
	Semaphore_Params_init(&tempDataParams);
	Semaphore_construct(&tempDataStruct, 1, &tempDataParams);

	tempDataHandle = Semaphore_handle(&tempDataStruct);

	// ---- init semaphore for pressSensor Data handle
	Semaphore_Params_init(&pressDataParams);
	Semaphore_construct(&pressDataStruct, 1, &pressDataParams);

	pressDataHandle = Semaphore_handle(&pressDataStruct);

	// ---- init semaphore for brightSensor Data handle
	Semaphore_Params_init(&brightDataParams);
	Semaphore_construct(&brightDataStruct, 1, &brightDataParams);

	brightDataHandle = Semaphore_handle(&brightDataStruct);

	return 0;
}

uint8_t init_sensorsTask(void){

	Task_Params sensorsTaskParams;


	init_sensorsSemaphore();

	/* Construct BIOS objects */
	Task_Params_init(&sensorsTaskParams);
	sensorsTaskParams.stackSize = SENSOR_TASKSTACKSIZE;
	sensorsTaskParams.stack = &sensorTask0Stack;
	sensorsTaskParams.priority = SENSOR_PIORITY;
	Task_construct(&sensorTask0Struct, (Task_FuncPtr)sensorsTaskFxn, &sensorsTaskParams, NULL);


	return 0;

}


