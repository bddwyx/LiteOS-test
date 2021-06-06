#include "Beeper.h"

void BeepInit(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);						//Enable PortK
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOK));			//Wait for the GPIO moduleK ready
    GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_5);			//Set PK5 as Output pin
}

void Beep(){
    int k;
    for (k = 0; k < 600; k++) { //音长
        GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_5,
                     ~GPIOPinRead(GPIO_PORTK_BASE, GPIO_PIN_5)
        );
        SysCtlDelay(120000000 / (1048 * 3));
    }
}
