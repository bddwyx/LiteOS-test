#ifndef DIGITALTUBE_H
#define DIGITALTUBE_H

#include <stdint.h>
#include <stdbool.h>

#include "los_sys.h"
#include "los_task.ph"
#include "los_memory.ph"

#include "sysctl.h"
#include "hw_memmap.h"
#include "pin_map.h"
#include "gpio.h"
#include "hw_i2c.h"
#include "i2c.h"

#define TCA6424_I2CADDR 				0x22
#define PCA9557_I2CADDR					0x18

#define PCA9557_INPUT					0x00
#define PCA9557_OUTPUT					0x01
#define PCA9557_POLINVERT				0x02
#define PCA9557_CONFIG					0x03

#define TCA6424_CONFIG_PORT0			0x0c
#define TCA6424_CONFIG_PORT1			0x0d
#define TCA6424_CONFIG_PORT2			0x0e

#define TCA6424_INPUT_PORT0				0x00
#define TCA6424_INPUT_PORT1				0x01
#define TCA6424_INPUT_PORT2				0x02

#define TCA6424_OUTPUT_PORT0			0x04
#define TCA6424_OUTPUT_PORT1			0x05
#define TCA6424_OUTPUT_PORT2			0x06

#define LED(_index) (1 << (_index))

extern uint8_t tubeShowBuffer[];

#ifdef RTOS_LOS
uint32_t DigitalTubeRTTInit(void);
void DigitalChangeLEDBuffer(uint8_t mask);
#else
void DigitalTubeHWInit(void);
void DigitalShowNum(uint8_t bit, uint8_t num);
void DigitalDisplay(uint8_t mask);
#endif

#endif //DIGITALTUBE_H
