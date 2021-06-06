#include "UART0.h"

void UARTStringPutNonBlocking(const uint8_t* cMessage)
{
    while(*cMessage!='\0')
        UARTCharPutNonBlocking(UART0_BASE,*(cMessage++));
}

void UART0HWInit(){
    /*SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);						//Enable PortN
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));			//Wait for the GPIO moduleN ready
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);			//Set PN0,PN1 as Output pin
*/
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);			//Set PF0 as Output pin

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);						//Enable PortA
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));			//Wait for the GPIO moduleA ready

    GPIOPinConfigure(GPIO_PA0_U0RX);												// Set GPIO A0 and A1 as UART pins.
    GPIOPinConfigure(GPIO_PA1_U0TX);

    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Configure the UART for 115,200, 8-N-1 operation.
    UARTConfigSetExpClk(UART0_BASE, SYSTEMCLOCK, 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));

    UARTFIFOLevelSet(UART0_BASE,UART_FIFO_TX2_8,UART_FIFO_RX4_8);//set FIFO Level

    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);	//Enable UART0 RX,TX interrupt
    IntEnable(INT_UART0);

    UARTStringPutNonBlocking("\r\nHello, world!\r\n");
}

#define UART0BUFFERSIZE 16
uint8_t UART0Buffer[UART0BUFFERSIZE * 2] = {0};

UART0_buffer_inform_t UART0BufferInformation = {false, 0};

uint8_t* const UART0Buffer1 = UART0Buffer;
uint8_t* const UART0Buffer1End = UART0Buffer + UART0BUFFERSIZE;
uint8_t* const UART0Buffer2 = UART0Buffer + UART0BUFFERSIZE;
uint8_t* const UART0Buffer2End = UART0Buffer + UART0BUFFERSIZE * 2;
uint8_t* UART0BufferPtr = UART0Buffer;

/**
 * @brief 双缓存机制下得到可用缓存区的位置
 * @return A pointer to uint8_t that indicates the location of UART0 buffer
 */
const uint8_t* UART0GetBufferPointer(){
    if(UART0BufferPtr < UART0Buffer2){
        return UART0Buffer2;
    }
    else{
        return UART0Buffer1;
    }
}

void UART0SwitchBuffer(){
    if(UART0BufferPtr <= UART0Buffer2){
        UART0BufferInformation.buffer_content_size = UART0BufferPtr - UART0Buffer1;
        UART0BufferPtr = UART0Buffer2;
    }
    else{
        UART0BufferInformation.buffer_content_size = UART0BufferPtr - UART0Buffer2;
        UART0BufferPtr = UART0Buffer1;
    }
}

void UART0BufferOverflowHandler(){

}

void UART0_RX_RT_Handler(uint8_t data){
    if((data != '\r') && (data != '\n')){
        *UART0BufferPtr = data;
        UART0BufferPtr++;
        if((UART0BufferPtr == UART0Buffer1End) || (UART0BufferPtr == UART0Buffer2End)){
            UART0SwitchBuffer();
            UART0BufferOverflowHandler();
        }
    }

    /**
     * @feature TM4C's FIFO can't be larger than 16
     */
    else if(data == '\r'){
        UART0SwitchBuffer();
        UART0BufferInformation.buffer_availability = true;
    }
}

void UART0_Handler(void)
{
    int32_t uart0_int_status;

    uart0_int_status = UARTIntStatus(UART0_BASE, true);			// Get the interrrupt status.
    UARTIntClear(UART0_BASE, uart0_int_status);							//Clear the asserted interrupts

    if (uart0_int_status & (UART_INT_RX | UART_INT_RT))
    {
        //GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);	//turn on PN1
        while(UARTCharsAvail(UART0_BASE))    										// Loop while there are characters in the receive FIFO.
        {
            UART0_RX_RT_Handler(UARTCharGetNonBlocking(UART0_BASE));
        }
        //GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);		//turn off PN1
    }
}
