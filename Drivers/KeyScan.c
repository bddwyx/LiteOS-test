#include "KeyScan.h"

const uint8_t DETECTMASK = 0x0f;

uint32_t Key1ReadFunc(void);
uint32_t Key2ReadFunc(void);
uint32_t KeyE1ReadFunc(void);
uint32_t KeyE2ReadFunc(void);
uint32_t KeyE3ReadFunc(void);
uint32_t KeyE4ReadFunc(void);
uint32_t KeyE5ReadFunc(void);
uint32_t KeyE6ReadFunc(void);
uint32_t KeyE7ReadFunc(void);
uint32_t KeyE8ReadFunc(void);

struct KeyReadAction_t keyReadActionList[] = {
        {Key1ReadFunc},
        {Key2ReadFunc},
        {KeyE1ReadFunc},
        {KeyE2ReadFunc},
        {KeyE3ReadFunc},
        {KeyE4ReadFunc},
        {KeyE5ReadFunc},
        {KeyE6ReadFunc},
        {KeyE7ReadFunc},
        {KeyE8ReadFunc},
};

#define KEY_NUM (sizeof(keyReadActionList) / sizeof(keyReadActionList[0]))
struct Key_t keyList[KEY_NUM] = {KEY_NORM};

const struct Key_t* key1 = keyList;
const struct Key_t* key2 = keyList + 1;
const struct Key_t* keyE1 = keyList + 2;
const struct Key_t* keyE2 = keyList + 3;
const struct Key_t* keyE3 = keyList + 4;
const struct Key_t* keyE4 = keyList + 5;
const struct Key_t* keyE5 = keyList + 6;
const struct Key_t* keyE6 = keyList + 7;
const struct Key_t* keyE7 = keyList + 8;
const struct Key_t* keyE8 = keyList + 9;

void KeyHWInit(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ));
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1); //Set PJ0-1 as Input pin
    GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

static uint8_t keyExAuxBuf;

uint32_t Key1ReadFunc(){
    return (~GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0)) >> 0;
}

uint32_t Key2ReadFunc(){
    return (~GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_1)) >> 1;
}

uint32_t KeyE1ReadFunc(){
    return keyExAuxBuf >> 0;
}

uint32_t KeyE2ReadFunc(){
    return keyExAuxBuf >> 1;
}

uint32_t KeyE3ReadFunc(){
    return keyExAuxBuf >> 2;
}

uint32_t KeyE4ReadFunc(){
    return keyExAuxBuf >> 3;
}

uint32_t KeyE5ReadFunc(){
    return keyExAuxBuf >> 4;
}

uint32_t KeyE6ReadFunc(){
    return keyExAuxBuf >> 5;
}

uint32_t KeyE7ReadFunc(){
    return keyExAuxBuf >> 6;
}

uint32_t KeyE8ReadFunc(){
    return keyExAuxBuf >> 7;
}

KeyStatus_e GetKeyStatus(const struct Key_t* key){
    return key->status;
}

uint8_t GetKeyTrigNum(const struct Key_t* key){
    return key->tirgNum;
}

void KeyScan(){
    keyExAuxBuf = ~I2C0_ReadByte(TCA6424_I2CADDR, TCA6424_INPUT_PORT0);

    for(uint8_t i = 0; i < KEY_NUM; i++){
        keyList[i].scanBuf <<= 1;
        keyList[i].scanBuf |= (keyReadActionList[i].readFunc() & 0x01);

        if((keyList[i].scanBuf & DETECTMASK) == DETECTMASK) {
            keyList[i].status = KEY_TRIGGED;
            keyList[i].tirgNum++;
            keyList[i].scanBuf = DETECTMASK + 1;
        }
        else if((keyList[i].scanBuf & DETECTMASK) == 0x00) {
            keyList[i].status = KEY_NORM;
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
