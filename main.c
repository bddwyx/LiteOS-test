#include <stdint.h>
#include <stdbool.h>

#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_ints.h"
#include "debug.h"
#include "gpio.h"
#include "pin_map.h"
#include "interrupt.h"

#include "config.h"

#include "los_sys.h"
#include "los_task.ph"
#include "los_memory.ph"

#include "DigitalTube.h"
#include "KeyScan.h"
#include "UART0.h"
#include "Lib_songs.h"
#include "BSP_QEI.h"
#include "StepMotor.h"

#include "Clock.h"
#include "CMDSystem.h"
#include "Operation.h"

int main(){
	//SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |SYSCTL_OSC_MAIN |SYSCTL_USE_OSC), 25000000ul);
	SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |SYSCTL_OSC_MAIN |SYSCTL_USE_PLL |SYSCTL_CFG_VCO_480), SYSTEMCLOCK);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));			//Wait for the GPIO moduleF ready
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0); //Set PF0 as Output pin

    UART0HWInit();
    LOS_KernelInit();
    OperationRRTInit();
    ClockModuleInit();
    DigitalTubeRTTInit();
    KeyScanRTTInit();
    MusicPlayRTTInit();
    StepMotorRTTInit();
    QEIRTTInit();
    //MusicStart(&Astronomia);
    LOS_Start();

	while(1) {	

	}
	
	return 0;
}
