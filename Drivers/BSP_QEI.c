#include "BSP_QEI.h"

int32_t qeiValue, qeiDir;

void QEIHWInit(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL));
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_QEI0));

    GPIOPinConfigure(GPIO_PL1_PHA0);
    GPIOPinConfigure(GPIO_PL2_PHB0);
    //software patch to force the PL3 to low voltage
    GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_3,0);

    GPIOPinTypeQEI(GPIO_PORTL_BASE, GPIO_PIN_1);
    GPIOPinTypeQEI(GPIO_PORTL_BASE, GPIO_PIN_2);

    QEIConfigure(QEI0_BASE, (QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET |	QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP), 100);
    QEIEnable(QEI0_BASE);
}

int32_t GetQEIValue(){
    return qeiValue;
}

int32_t GetQEIDir(){
    return qeiDir;
}

#ifdef RTOS_LOS

static uint32_t qei_thread_handle;

static void QEI_thread_entry(void* param){
    while(1) {
        qeiValue = QEIPositionGet(QEI0_BASE);
        qeiDir = QEIDirectionGet(QEI0_BASE);
        LOS_TaskDelay(10); // Delay
    }
}

/**
 * @brief Start key-scan thread
 * @ret Status of thread init function
 */
uint32_t QEIRTTInit(){

    QEIHWInit();

    uint32_t uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 4;
    task_init_param.pcName = "QEI";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)QEI_thread_entry;
    task_init_param.uwStackSize = 512;
    uwRet = LOS_TaskCreate(&qei_thread_handle, &task_init_param);
    if (uwRet != LOS_OK)
    {
        return uwRet;
    }
    return LOS_OK;
}

#endif
