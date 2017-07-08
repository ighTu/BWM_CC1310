/*
 * sensors.h
 *
 *  Created on: 06.02.2017
 *      Author: MyDev
 */

#ifndef XI_SENSORS_SENSORS_H_
#define XI_SENSORS_SENSORS_H_



//--------------------------------------------------------------------------
// Include for Sensors
//-------------------------------------------------------------------------

#include "../XI_Bios/bios.h"
#include "../XI_Sensors/bh1750.h"
#include "../XI_Sensors/bme280.h"
#include "../XI_Sensors/bmp280.h"
#include "../XI_Sensors/tmp007.h"
#include "../XI_Sensors/mpu9250.h"

//--------------------------------------------------------------------------
// Sensors Task
//--------------------------------------------------------------------------

struct sSensors{


};

struct sSensors sensors;

#define SENSOR_TASKSTACKSIZE   	1024
#define SENSOR_PIORITY 			2

Task_Struct sensorTask0Struct;
UInt8 sensorTask0Stack[SENSOR_TASKSTACKSIZE];
Task_Handle sensorTask;

Semaphore_Params unlockSensorsParams;
Semaphore_Struct unlockSensorsStruct;
Semaphore_Handle unlockSensorsHandle;

uint8_t init_sensorsTask(void);


#endif /* XI_SENSORS_SENSORS_H_ */
