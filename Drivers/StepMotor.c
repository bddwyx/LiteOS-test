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

#ifdef RT_VERSION

ALIGN(RT_ALIGN_SIZE);
static rt_uint8_t stepMotor_thread_stack[128];

static struct rt_thread stepMotor_thread;

static void stepMotor_thread_entry(void* param){
    while(1) {
				static uint8_t step = 0;
				StepMotorDriver(step++);
				step %= 4;	
				rt_thread_delay(3); // Delay
    }
}

/**
 * @brief Start step motor thread
 * @ret Status of thread init function
 */
rt_err_t StepMotorRTTInit(){

    StepMotorHWInit();

    rt_err_t status =
            rt_thread_init(&stepMotor_thread,
                "Step Motor",
                stepMotor_thread_entry,
                RT_NULL,
                stepMotor_thread_stack,
                sizeof(stepMotor_thread_stack),
                2,
                1);

    if(RT_EOK == status){
        //rt_thread_startup(&stepMotor_thread);
    }

    return status;
}

#endif
