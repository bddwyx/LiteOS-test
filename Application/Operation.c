#include "Operation.h"

bool motorEnable = true, tubeEnable = true;

void OperationChangeAvailability(bool motor, bool tube){
    motorEnable = motor;
    tubeEnable = tube;
}

static void operation_thread_entry(){
    while(1) {
        CMDDetect();
        LOS_TaskDelay(200);
        CMDDetect();
        LOS_TaskDelay(200);
        CMDDetect();
        LOS_TaskDelay(200);
        CMDDetect();
        LOS_TaskDelay(200);
        CMDDetect();
        LOS_TaskDelay(200);

        ClockTick();

        if(motorEnable) StepMotorTick();
        if(tubeEnable) ClockDisplay(SIXBIT_WITHDASH);
    }
}

uint32_t operation_thread_handle;

uint32_t OperationRRTInit(void)
{
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
