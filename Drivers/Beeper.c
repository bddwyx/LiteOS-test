#include "Beeper.h"

void BeepInit(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);						//Enable PortK
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOK));			//Wait for the GPIO moduleK ready
    GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_5);			//Set PK5 as Output pin*/
	
	    SysCtlPWMClockSet(SYSCTL_PWMDIV_32);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0));
	
				volatile uint32_t* tmp = (uint32_t*)(0x40028FC8);
	*tmp = 0x01;
	
    GPIOPinConfigure(GPIO_PK5_M0PWM7);
    GPIOPinTypePWM(GPIO_PORTK_BASE, GPIO_PIN_5);
	

    PWMGenConfigure(PWM0_BASE, PWM_GEN_3,
                    PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 40000);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, 20000);

    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);

}

void Beep(){
    int k;
    for (k = 0; k < 600; k++) { //音长
        GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_5,
                     ~GPIOPinRead(GPIO_PORTK_BASE, GPIO_PIN_5)
        );
        //SysCtlDelay(120000000 / (1048 * 3));
    }
}
