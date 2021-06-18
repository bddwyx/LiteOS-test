#ifndef BSP_QEI_H
#define BSP_QEI_H

#include <stdint.h>
#include <stdbool.h>

#include "los_sys.h"
#include "los_task.ph"
#include "los_memory.ph"

#include "hw_memmap.h"
#include "pin_map.h"
#include "gpio.h"
#include "sysctl.h"
#include "hw_qei.h"
#include "qei.h"

int32_t GetQEIValueChange(void);
int32_t GetQEIDir(void);

#ifdef RTOS_LOS
uint32_t QEIRTTInit(void);
#endif

#endif //BSP_QEI_H
