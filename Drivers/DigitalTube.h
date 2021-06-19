#ifndef DIGITALTUBE_H
#define DIGITALTUBE_H

#include <stdint.h>
#include <stdbool.h>

#include "los_sys.h"
#include "los_task.ph"
#include "los_memory.ph"

#include "sysctl.h"
#include "gpio.h"
#include "BSP_I2C.h"

#define LED(_index) (1 << (_index))

extern uint8_t tubeShowBuffer[];

#ifdef RTOS_LOS
uint32_t DigitalTubeRTTInit(void);
void DigitalChangeLEDBuffer(uint8_t mask);
void DigitalTubeChangeBrightness(int8_t input);
void DigitalTubeBrightnessMax(void);
#else
void DigitalTubeHWInit(void);
void DigitalShowNum(uint8_t bit, uint8_t num);
void DigitalDisplay(uint8_t mask);
#endif

#endif //DIGITALTUBE_H
