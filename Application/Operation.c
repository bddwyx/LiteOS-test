#include "Operation.h"

bool motorEnable = true, tubeEnable = true;

void OperationChangeAvailability(bool motor, bool tube){
    motorEnable = motor;
    tubeEnable = tube;
}

void OperationModuleInit(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)){}

    TimerClockSourceSet(TIMER0_BASE, TIMER_CLOCK_SYSTEM);

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);         //Periodic mode
    TimerLoadSet(TIMER0_BASE, TIMER_A, 12 * SYSTEMCLOCK /12 / 1000 * 10 - 1);  //100Hz timer, initiates an interrupt every 0.01s

    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    TimerEnable(TIMER0_BASE, TIMER_A);
}

void TIMER0A_Handler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT); //Clear int flags

    static uint8_t cnt = 0;
    cnt++;
    if(cnt > 99){
        cnt = 0;
        if(motorEnable || tubeEnable) ClockTick();
        if(tubeEnable) ClockDisplay(SIXBIT_WITHDASH);
        if(motorEnable) StepMotorTick(false);
    }
}

static void operation_thread_entry(){
    while(1) {
        CMDDetect();
        LOS_TaskDelay(200);
    }
}

uint32_t operation_thread_handle;

uint32_t OperationRRTInit(void)
{
    OperationModuleInit();

    uint32_t uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 4;
    task_init_param.pcName = "Operation";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)operation_thread_entry;
    task_init_param.uwStackSize = 512;
    uwRet = LOS_TaskCreate(&operation_thread_handle, &task_init_param);
    if (uwRet != LOS_OK)
    {
        return uwRet;
    }
    return LOS_OK;
}
