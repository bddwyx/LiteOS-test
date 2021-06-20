#include "StopWatch.h"

static uint8_t stopWatchTime = 10;
static uint8_t stopWatch10ms = 0;
bool stopWatchOn = false;
bool stopWatchDisplayOn = false;

void StopWatchOn(){
    stopWatchOn = true;
    DigitalChangeLEDBuffer(0x20);
}

void StopWatchPend(){
    stopWatchOn = false;
    DigitalChangeLEDBuffer(0x10);
}

void StopWatchModeSwitch(bool state) {
    stopWatchDisplayOn = state;
}

void StopWatchSet(uint8_t seconds){
    stopWatchTime = seconds;
}

void StopWatchInc(int8_t seconds){
    stopWatchOn = false;
    if((seconds > 0) && (stopWatchTime < 255)) stopWatchTime++;
    if((seconds < 0) && (stopWatchTime > 0)) stopWatchTime--;
    stopWatch10ms = 0;
}

void StopWatchTick(){
    if(stopWatch10ms > 0) stopWatch10ms--;
    else{
        if(stopWatchTime > 0){
            stopWatchTime--;
            stopWatch10ms = 99;
        }
        else{
            stopWatchOn = false;
            DigitalChangeLEDBuffer(0x10);
            MusicStart(&SuperMario);
        }
    }
}

void StopWatchDisplay(){
    tubeShowBuffer[0] = 20;
    tubeShowBuffer[1] = 20;
    tubeShowBuffer[2] = stopWatchTime / 100;
    tubeShowBuffer[3] = stopWatchTime / 10 % 10;
    tubeShowBuffer[4] = stopWatchTime % 10;
    tubeShowBuffer[5] = 19;
    tubeShowBuffer[6] = stopWatch10ms / 10;
    tubeShowBuffer[7] = stopWatch10ms % 10;
}

#ifdef RTOS_LOS

static uint32_t stopWatch_thread_handle;

static void tube_thread_entry(void* param){
    while(1) {
        if(stopWatchOn) StopWatchTick();
        if(stopWatchDisplayOn) StopWatchDisplay();
        LOS_TaskDelay(10); // Delay
    }
}

/**
 * @brief Start digital-tube thread
 * @ret Status of thread init function
 */
uint32_t StopWatchRTTInit(){
    uint32_t uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 4;
    task_init_param.pcName = "Stop Watch";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)tube_thread_entry;
    task_init_param.uwStackSize = 512;
    uwRet = LOS_TaskCreate(&stopWatch_thread_handle, &task_init_param);
    if (uwRet != LOS_OK)
    {
        return uwRet;
    }
    return LOS_OK;
}

#endif