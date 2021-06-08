#ifndef STEPMOTOR_H
#define STEPMOTOR_H

#include <stdint.h>
#include <stdbool.h>

#include "los_sys.h"
#include "los_task.ph"
#include "los_memory.ph"

#include "hw_memmap.h"
#include "gpio.h"
#include "sysctl.h"

#ifdef RTOS_LOS
uint32_t StepMotorRTTInit(void);
#else
void StepMotorHWInit();
void StepMotorDriver(uint8_t step);
#endif

int32_t StepMotorCMD(int32_t cmd);
void StepMotorTick(void);

#endif //STEPMOTOR_H
