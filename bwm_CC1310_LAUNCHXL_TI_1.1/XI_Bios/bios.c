/*
 * bios.c
 *
 *  Created on: 08.02.2017
 *      Author: MyDev
 */

#include <XI_Sensors/sensors.h>
#include "XI_Bios/bios.h"

uint8_t init_Hardware(void){

	/* Call board init functions */
	Board_initGeneral();
	Board_initGPIO();
	Board_initUART();
	Board_initI2C();
	Board_initPWM();

	return 0;

}

//--------------------------------------------------------------------------
// for PWM
//--------------------------------------------------------------------------

void pwmTaskFxn(UArg arg0, UArg arg1)
{
	uint16_t   pwmDuty = 1500;

	PWM_Params params;
	uint16_t   pwmPeriod = 19000;      // Period and duty in microseconds
	//uint16_t   duty = 0;
	//uint16_t   dutyInc = 100;

	PWM_Params_init(&params);
	params.dutyUnits = PWM_DUTY_US;
	params.dutyValue = 0;
	params.periodUnits = PWM_PERIOD_US;
	params.periodValue = pwmPeriod;

	pwm1 = PWM_open(Board_PWM1, &params);

	if (pwm1 == NULL) {
		System_abort("Board_PWM0 did not open");
	}else{
		PWM_start(pwm1);
	}

	/* Loop forever incrementing the PWM duty */
	while (1) {

		PWM_setDuty(pwm1, pwmDuty);

		if(pwmDuty > 2090){		//test for servo
			pwmDuty = 2090;
		}

		if(pwmDuty < 820){
			pwmDuty = 820;
		}

		Task_sleep((UInt) arg0);
	}
}

uint8_t init_pwmTask(void){

	Task_Params pwmTaskParams;

	/* Construct PWM Task thread */
	Task_Params_init(&pwmTaskParams);
	pwmTaskParams.stackSize = PWM_TASKSTACKSIZE;
	pwmTaskParams.stack = &pwmTask0Stack;
	pwmTaskParams.priority = 3;
	pwmTaskParams.arg0 = 1;
	Task_construct(&pwmTask0Struct, (Task_FuncPtr)pwmTaskFxn, &pwmTaskParams, NULL);

	/* Obtain instance handle */
	pwmTask = Task_handle(&pwmTask0Struct);

	return 0;
}

//--------------------------------------------------------------------------
// for Hardware Interrupt
//--------------------------------------------------------------------------

/* Pin driver handles */
static PIN_Handle buttonPinHandle;
/* Global memory storage for a PIN_Config table */
static PIN_State buttonPinState;

/*
 * Application button pin configuration table:
 *   - Buttons interrupts are configured to trigger on falling edge.
 */
PIN_Config buttonPinTable[] = {
		Board_BUTTON0  | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
		Board_BUTTON1  | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
		PIN_TERMINATE
};


/*  ======== buttonCallbackFxn ========
 *  Pin interrupt Callback function board buttons configured in the pinTable.
 *  If Board_LED3 and Board_LED4 are defined, then we'll add them to the PIN
 *  callback function.
 */
void buttonCallbackFxn(PIN_Handle handle, PIN_Id pinId) {
	//uint32_t currVal = 0;

	/* Debounce logic, only toggle if the button is still pushed (low) */
	//CPUdelay(50);
	if (!PIN_getInputValue(pinId)) {
		/* Toggle LED based on the button pressed */
		switch (pinId) {
		case Board_BUTTON0:
			//currVal =  PIN_getOutputValue(Board_LED0);
			//PIN_setOutputValue(ledPinHandle, Board_LED0, !currVal);

			while(PIN_getInputValue(pinId) == 0){
//				pwmDuty -= 10;
//				PWM_setDuty(pwm1, pwmDuty);

//				if(pwmDuty < 820){
//					pwmDuty = 820;
//				}
				__delay_ms(5);
			}



			break;

		case Board_BUTTON1:
			//currVal =  PIN_getOutputValue(Board_LED1);
			//PIN_setOutputValue(ledPinHandle, Board_LED1, !currVal);

			while(PIN_getInputValue(pinId) == 0){
//				pwmDuty += 10;
//				PWM_setDuty(pwm1, pwmDuty);

//				if(pwmDuty > 2090){
//					pwmDuty = 2090;
//				}
				__delay_ms(5);
			}



			break;

		default:
			/* Do nothing */
			break;
		}
	}
}


uint8_t init_Interrupt(void){

	buttonPinHandle = PIN_open(&buttonPinState, buttonPinTable);
		if(!buttonPinHandle) {
			System_abort("Error initializing button pins\n");
		}

		/* Setup callback for button pins */
		if (PIN_registerIntCb(buttonPinHandle, &buttonCallbackFxn) != 0) {
			System_abort("Error registering button callback function");
		}

	return 0;
}

