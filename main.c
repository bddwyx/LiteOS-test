#include <stdint.h>
#include <stdbool.h>

#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_ints.h"
#include "debug.h"
#include "gpio.h"
#include "pin_map.h"
#include "sysctl.h"
#include "interrupt.h"

#include "config.h"

#include "los_sys.h"
#include "los_task.ph"
#include "los_memory.ph"

/**
  \brief   System Tick Configuration
  \details Initializes the System Timer and its interrupt, and starts the System Tick Timer.
           Counter is in free running mode to generate periodic interrupts.
  \param [in]  ticks  Number of ticks between two interrupts.
  \return          0  Function succeeded.
  \return          1  Function failed.
  \note    When the variable <b>__Vendor_SysTickConfig</b> is set to 1, then the
           function <b>SysTick_Config</b> is not included. In this case, the file <b><i>device</i>.h</b>
           must contain a vendor-specific implementation of this function.
 */
uint32_t SysTick_Config(uint32_t ticks)
{
    SysTickPeriodSet(ticks);
    SysTickEnable();
    SysTickIntEnable();     /* Function successful */
    return 0;
}

void Delay(uint32_t value)
{
	uint32_t ui32Loop;
	
	for(ui32Loop = 0; ui32Loop < value; ui32Loop++){};
}

static void LEDTask(){
    while(1) {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);            // Turn on the LED.
        LOS_TaskDelay(500);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x0);                            // Turn off the LED.
        LOS_TaskDelay(500);
    }
}

uint32_t RX_Task_Handle;

static uint32_t AppTaskCreate(void)
{
    uint32_t uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 4;
    task_init_param.pcName = "LEDTask";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)LEDTask;
    task_init_param.uwStackSize = 512;
    uwRet = LOS_TaskCreate(&RX_Task_Handle, &task_init_param);
    if (uwRet != LOS_OK)
    {
        return uwRet;
    }
    return LOS_OK;
}

int main(){
	//SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |SYSCTL_OSC_MAIN |SYSCTL_USE_OSC), 25000000ul);
	SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |SYSCTL_OSC_MAIN |SYSCTL_USE_PLL |SYSCTL_CFG_VCO_480), SYSTEMCLOCK);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));			//Wait for the GPIO moduleF ready
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0); //Set PF0 as Output pin

    LOS_KernelInit();
    AppTaskCreate();
	LOS_Start();

	while(1) {	

	}
	
	return 0;
}
