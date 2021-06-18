#include "KeyScan.h"

const uint8_t DETECTMASK = 0x0f;

struct Key_t keyList[2] = {LOW};
#define KEY_NUM (sizeof(keyList) / sizeof(struct Key_t))

const struct Key_t* key1 = keyList;
const struct Key_t* key2 = keyList + 1;

void KeyHWInit(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ));
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1); //Set PJ0-1 as Input pin
    GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

uint32_t Key1ReadFunc(){
    return GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0) >> 0;
}

uint32_t Key2ReadFunc(){
    return GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_1) >> 1;
}

struct KeyReadAction_t keyReadActionList[KEY_NUM] = {
    {Key1ReadFunc},
    {Key2ReadFunc},
};

KeyStatus_e GetKeyStatus(const struct Key_t* key){
    return key->status;
}

uint8_t GetKeyTrigNum(const struct Key_t* key){
    return key->tirgNum;
}

void KeyScan(){
    for(uint8_t i = 0; i < KEY_NUM; i++){
        keyList[i].scanBuf <<= 1;
        keyList[i].scanBuf |= (keyReadActionList[i].readFunc() & 0x01);

        if((keyList[i].scanBuf & DETECTMASK) == DETECTMASK) keyList[i].status = HIGH;
        if((keyList[i].scanBuf & DETECTMASK) == 0x00) {
            keyList[i].status = LOW;
            keyList[i].scanBuf = 0xff;
            keyList[i].tirgNum++;
        }
    }
}

#ifdef RTOS_LOS

static uint32_t key_thread_handle;

static void key_thread_entry(void* param){
    while(1) {
        KeyScan();
        LOS_TaskDelay(10); // Delay
    }
}

/**
 * @brief Start key-scan thread
 * @ret Status of thread init function
 */
uint32_t KeyScanRTTInit(){
	
    KeyHWInit();

    uint32_t uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 4;
    task_init_param.pcName = "Key Scan";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)key_thread_entry;
    task_init_param.uwStackSize = 512;
    uwRet = LOS_TaskCreate(&key_thread_handle, &task_init_param);
    if (uwRet != LOS_OK)
    {
        return uwRet;
    }
    return LOS_OK;
}

#endif
