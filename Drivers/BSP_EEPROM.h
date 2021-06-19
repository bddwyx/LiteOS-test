#ifndef BSP_EEPROM_H
#define BSP_EEPROM_H

#include <stdint.h>
#include <stdbool.h>

#include "hw_memmap.h"
#include "gpio.h"
#include "hw_types.h"
#include "pin_map.h"
#include "sysctl.h"
#include "interrupt.h"
#include "hw_ints.h"
#include "eeprom.h"

void EEPROMHWInit(void);
void EEPROMWriteToDst(uint8_t start, uint8_t* data, uint8_t size);
void EEPROMReadToRAM(uint8_t start, uint8_t* data, uint8_t size);

#endif //BSP_EEPROM_H
