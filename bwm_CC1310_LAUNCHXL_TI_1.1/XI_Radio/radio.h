/*
 * radio.h
 *
 *  Created on: 04.03.2017
 *      Author: MyDev
 */

#ifndef XI_RADIO_RADIO_H_
#define XI_RADIO_RADIO_H_


//--------------------------------------------------------------------------
// Include for Radio
//-------------------------------------------------------------------------

#include "../XI_Bios/bios.h"


//--------------------------------------------------------------------------
// Sensors Task
//--------------------------------------------------------------------------

#define RADIO_TASKSTACKSIZE   	1024
#define RADIO_PIORITY 			2

/* Packet TX Configuration */
#define PAYLOAD_LENGTH      30
#define PACKET_INTERVAL     (uint32_t)(4000000*0.5f) /* Set packet interval to 500ms */

Task_Struct radioTask0Struct;
UInt8 radioTask0Stack[RADIO_TASKSTACKSIZE];
Task_Handle radioTask;



Semaphore_Params unlockRadioParams;
Semaphore_Struct unlockRadioStruct;
Semaphore_Handle unlockRadioHandle;


#endif /* XI_RADIO_RADIO_H_ */
