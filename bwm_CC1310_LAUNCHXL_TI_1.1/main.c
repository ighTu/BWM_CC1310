/*
 *  ======== bwm test igor ========
 *  kleiner test
 */


#include <XI_Sensors/sensors.h>
#include "XI_Bios/bios.h"



//#define TASKSTACKSIZE     768

Task_Struct task0Struct;
Char task0Stack[MAIN_TASKSTACKSIZE];


/* Pin driver handles */
static PIN_Handle ledPinHandle;

/* Global memory storage for a PIN_Config table */
static PIN_State ledPinState;

/*
 * Application LED pin configuration table:
 *   - All LEDs board LEDs are off.
 */
PIN_Config ledPinTable[] = {
		Board_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
		Board_LED2 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
		PIN_TERMINATE
};


/*
 *  ======== echoFxn ========
 *  Task for this function is created statically. See the project's .cfg file.
 */
void mainTask(UArg arg0, UArg arg1)
{

	const char echoPrompt[] = "\fEchoing characters:\r\n";

	System_init();

	UART_write(uart, echoPrompt, sizeof(echoPrompt));

	putstr("\r\nTest BWM V1.1 :-)\r\n\0");

	Semaphore_post(unlockSensorsHandle);

	/* Loop forever echoing */
	while (1) {
		//putstr("\r\nMain Task running! :-)\r\n\0");


		// unlock sensors task

		show_TMP007_Temp(&TempSensor);
		show_BME280_Press(&PressSensor);
		show_BH1750_Bright(&BrightSensor);
		/*if ( Semaphore_getCount(tempDataHandle) == 0 )   {
			putstr("\r\nRead Temperature Data blocked!\r\n");
		}else{
			Semaphore_pend(tempDataHandle, BIOS_WAIT_FOREVER);
			putFloatData(TempSensor.ObjTemp, 100);
			putFloatData(TempSensor.SensTemp, 100);
			Semaphore_post(tempDataHandle);
		}

		if ( Semaphore_getCount(pressDataHandle) == 0 )   {
			putstr("\r\nRead Pressure and Humidity Data blocked!\r\n");
		}else{
			Semaphore_pend(pressDataHandle, BIOS_WAIT_FOREVER);
			putFloatData(PressSensor.SensTemp, 100);
			putFloatData(PressSensor.SensHumidity, 100);
			putFloatData(PressSensor.SensPress, 100);
			Semaphore_post(pressDataHandle);

		}

		if ( Semaphore_getCount(brightDataHandle) == 0 )   {
			putstr("\r\nRead Brightness Data blocked!\r\n");
		}else{
			Semaphore_pend(brightDataHandle, BIOS_WAIT_FOREVER);
			putFloatData(BrightSensor.SensBrightness, 100);
			Semaphore_post(brightDataHandle);

		}*/



		//__delay_ms(500);
		Task_sleep(2000000 / Clock_tickPeriod);

	}
}

uint8_t init_mainTask(void){

	Task_Params taskParams;

	/* Construct BIOS objects */
	Task_Params_init(&taskParams);
	taskParams.stackSize = MAIN_TASKSTACKSIZE;
	taskParams.stack = &task0Stack;
	taskParams.priority = MAIN_PIORITY;
	Task_construct(&task0Struct, (Task_FuncPtr)mainTask, &taskParams, NULL);

	return 0;
}

/*
 *  ======== main ========
 */
int main(void)
{
	//PIN_Handle ledPinHandle;
	//Task_Params taskParams;
	//Task_Params pwmTaskParams;

	init_Hardware();
	init_sensorsTask();
	init_mainTask();
	//init_radioTask();



	/* Open LED pins */
	ledPinHandle = PIN_open(&ledPinState, ledPinTable);
	if(!ledPinHandle) {
		// for Error!
		//System_abort("Error initializing board LED pins\n");
	}

	//PIN_setOutputValue(ledPinHandle, Board_LED1, 1);

	/* This example has logging and many other debug capabilities enabled */
	/*System_printf("This example does not attempt to minimize code or data "
			"footprint\n");
	System_flush();*/

	/*System_printf("Starting the UART Echo example\nSystem provider is set to "
			"SysMin. Halt the target to view any SysMin contents in "
			"ROV.\n");*/
	/* SysMin will only print to the console when you call flush or exit */
	System_flush();

	/* Start BIOS */
	BIOS_start();

	return (0);
}
