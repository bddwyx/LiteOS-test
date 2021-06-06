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
            keyList[i].tirgNum++;
        }
    }
}

#ifdef RT_VERSION

ALIGN(RT_ALIGN_SIZE);
static rt_uint8_t key_thread_stack[128];

static struct rt_thread key_thread;

static void key_thread_entry(void* param){
    while(1) {
        KeyScan();
        rt_thread_delay(10); // Delay
    }
}

/**
 * @brief Start key-scan thread
 * @ret Status of thread init function
 */
rt_err_t KeyScanRTTInit(){
	
    KeyHWInit();

    rt_err_t status =
            rt_thread_init(&key_thread,
                "Key Scan",
                key_thread_entry,
                RT_NULL,
                key_thread_stack,
                sizeof(key_thread_stack),
                3,
                1);

    if(RT_EOK == status){
        rt_thread_startup(&key_thread);
    }

    return status;
}

#endif
