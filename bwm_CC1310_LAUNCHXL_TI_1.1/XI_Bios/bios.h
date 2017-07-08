/*
 * bios.h
 *
 *  Created on: 06.02.2017
 *      Author: MyDev
 */

#ifndef XI_BIOS_BIOS_H_
#define XI_BIOS_BIOS_H_





//--------------------------------------------------------------------------
// Include for Bios
//-------------------------------------------------------------------------

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/rf/RF.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPICC26XXDMA.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/PWM.h>

#include "Board.h"

#include "smartrf_settings/smartrf_settings.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#include "bios_CC1310.h"
#include "nokia_5110_lcd.h"
#include "math.h"
//#include "english_font.h"


#define MAIN_TASKSTACKSIZE   	1024
#define MAIN_PIORITY 			2

uint8_t init_Hardware(void);

//--------------------------------------------------------------------------
// for PWM
//--------------------------------------------------------------------------

#define PWM_TASKSTACKSIZE   512

PWM_Handle pwm1;


Task_Struct pwmTask0Struct;
UInt8 pwmTask0Stack[PWM_TASKSTACKSIZE];
Task_Handle pwmTask;




#endif /* XI_BIOS_BIOS_H_ */
