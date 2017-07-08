/*
 * radio.c
 *
 *  Created on: 04.03.2017
 *      Author: MyDev
 */

#include <XI_Radio/radio.h>
#include <XI_Bios/bios.h>

static RF_Object rfObject;
static RF_Handle rfHandle;

//uint32_t time;
static uint8_t packet[PAYLOAD_LENGTH];
static uint16_t seqNumber;

void radioTaskFxn(UArg arg0, UArg arg1){

	uint32_t time;
	RF_Params rfParams;
	RF_Params_init(&rfParams);

	RF_cmdPropTx.pktLen = PAYLOAD_LENGTH;
	RF_cmdPropTx.pPkt = packet;
	RF_cmdPropTx.startTrigger.triggerType = TRIG_ABSTIME;
	RF_cmdPropTx.startTrigger.pastTrig = 1;
	RF_cmdPropTx.startTime = 0;

	// Request access to the radio
	rfHandle = RF_open(&rfObject, &RF_prop, (RF_RadioSetup*)&RF_cmdPropRadioDivSetup, &rfParams);

	// Set the frequency
	RF_postCmd(rfHandle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);

	// Get current time
	time = RF_getCurrentTime();

	while(1){

		// Create packet with incrementing sequence number and random payload
		packet[0] = (uint8_t)(seqNumber >> 8);
		packet[1] = (uint8_t)(seqNumber++);
		uint8_t i;
		for(i = 2; i < PAYLOAD_LENGTH; i++){
			packet[i] = rand();
		}

		// Set absolute TX time to utilize automatic power management
		time += PACKET_INTERVAL;
		RF_cmdPropTx.startTime = time;

		// Send packet
		RF_EventMask result = RF_runCmd(rfHandle, (RF_Op*)&RF_cmdPropTx, RF_PriorityNormal, NULL, 0);
		if(!(result & RF_EventLastCmdDone)){
			// Error
			while(1);
		}

		// fertig

	}

}


uint8_t init_radioSemaphore(void){

	return 0;
}

uint8_t init_radioTask(void){

	Task_Params radioTaskParams;

	Task_Params_init(&radioTaskParams);
	radioTaskParams.stackSize = RADIO_TASKSTACKSIZE;
	radioTaskParams.priority = RADIO_PIORITY;
	radioTaskParams.stack = &radioTask0Stack;
	radioTaskParams.arg0 = (UInt)1000000;

	Task_construct(&radioTask0Struct, radioTaskFxn, &radioTaskParams, NULL);

	return 0;
}
