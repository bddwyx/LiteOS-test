#include "StepMotor.h"

static int32_t stepMotorTodo = 0;

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

int32_t StepMotorCMD(int32_t cmd){
    stepMotorTodo += cmd;
    return stepMotorTodo;
}

void StepMotorTick(){
    static uint8_t cnt = 0;
    cnt++;
    if(cnt > 59){
        StepMotorCMD(-42);
        cnt %= 60;
    }
    else{
        StepMotorCMD(-34);
    }
}

#ifdef RTOS_LOS

static uint32_t stepMotor_thread_handle;

static void StepMotor_thread_entry(void* param){
    while(1) {
        static uint8_t step = 0;
        if(stepMotorTodo > 0){
            StepMotorDriver(step++);
            step %= 4;
            stepMotorTodo--;
        }
        else if(stepMotorTodo < 0){
            StepMotorDriver(step);
            step += 4;
            step--;
            step %= 4;
            stepMotorTodo++;
        }
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
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)StepMotor_thread_entry;
    task_init_param.uwStackSize = 512;
    uwRet = LOS_TaskCreate(&stepMotor_thread_handle, &task_init_param);
    if (uwRet != LOS_OK)
    {
        return uwRet;
    }
    return LOS_OK;
}

#endif
