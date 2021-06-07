#include "StepMotor.h"

void StepMotorHWInit(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);						//Enable PortF
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));			//Wait for the GPIO moduleF ready
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);			//Set PF0~3 as Output pin
}

void StepMotorDriver(uint8_t step){
    switch (step) {
        case 0:
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
            break;

        case 1:
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
            break;

        case 2:
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
            break;

        case 3:
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
            break;

        default:;
    }
}

#ifdef RTOS_LOS

static uint32_t stepMotor_thread_handle;

static void stepMotor_thread_entry(void* param){
    while(1) {
        static uint8_t step = 0;
        StepMotorDriver(step++);
        step %= 4;
        LOS_TaskDelay(3); // Delay
    }
}

/**
 * @brief Start step motor thread
 * @ret Status of thread init function
 */
uint32_t StepMotorRTTInit(){

    StepMotorHWInit();

    uint32_t uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 4;
    task_init_param.pcName = "Step Motor";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)stepMotor_thread_entry;
    task_init_param.uwStackSize = 512;
    uwRet = LOS_TaskCreate(&stepMotor_thread_handle, &task_init_param);
    if (uwRet != LOS_OK)
    {
        return uwRet;
    }
    return LOS_OK;
}

#endif
