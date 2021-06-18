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

#include "BSP_I2C.h"

typedef enum{
    KEY_NORM = 0,
    KEY_TRIGGED,
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
extern const struct Key_t* keyE1;
extern const struct Key_t* keyE2;
extern const struct Key_t* keyE3;
extern const struct Key_t* keyE4;
extern const struct Key_t* keyE5;
extern const struct Key_t* keyE6;
extern const struct Key_t* keyE7;
extern const struct Key_t* keyE8;

KeyStatus_e GetKeyStatus(const struct Key_t* key);
uint8_t GetKeyTrigNum(const struct Key_t* key);

#ifdef RTOS_LOS
uint32_t KeyScanRTTInit(void);
#else
void KeyScan();
void KeyHWInit();
#endif

#endif //KEYSCAN_H
