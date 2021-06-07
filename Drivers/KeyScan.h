#ifndef KEYSCAN_H
#define KEYSCAN_H

#include <stdint.h>
#include <stdbool.h>

#include "los_sys.h"
#include "los_task.ph"
#include "los_memory.ph"

#include "hw_memmap.h"
#include "gpio.h"
#include "sysctl.h"

typedef enum{
    LOW = 0,
    HIGH,
} KeyStatus_e;

struct Key_t{
    KeyStatus_e status;
    uint8_t scanBuf;
    uint8_t tirgNum;
};

struct KeyReadAction_t{
    uint32_t (*readFunc)(void);
};

extern struct Key_t keyList[];

extern const struct Key_t* key1;
extern const struct Key_t* key2;

KeyStatus_e GetKeyStatus(const struct Key_t* key);
uint8_t GetKeyTrigNum(const struct Key_t* key);

#ifdef RTOS_LOS
uint32_t KeyScanRTTInit(void);
#else
void KeyScan();
void KeyHWInit();
#endif

#endif //KEYSCAN_H
