#ifndef OPERATION_H
#define OPERATION_H

#include <stdint.h>
#include <stdbool.h>

#include "hw_ints.h"
#include "timer.h"

#include "StepMotor.h"

#include "Clock.h"
#include "CMDSystem.h"

void OperationChangeAvailability(bool motor, bool tube);
uint32_t OperationRRTInit(void);

#endif //OPERATION_H
