#ifndef STEPMOTOR_H
#define STEPMOTOR_H

#include <stdint.h>
#include <stdbool.h>
#include <rtthread.h>

#include "hw_memmap.h"
#include "gpio.h"
#include "sysctl.h"

#ifdef RT_VERSION
rt_err_t StepMotorRTTInit(void);
#else
void StepMotorHWInit();
void StepMotorDriver(uint8_t step);
#endif

#endif //STEPMOTOR_H
