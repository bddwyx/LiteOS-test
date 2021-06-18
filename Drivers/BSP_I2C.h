#ifndef BSP_I2C_H
#define BSP_I2C_H

#include <stdint.h>
#include <stdbool.h>

#include "hw_memmap.h"
#include "pin_map.h"
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

uint8_t I2C0_WriteByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteData);
uint8_t I2C0_ReadByte(uint8_t DevAddr, uint8_t RegAddr);

#endif //BSP_I2C_H
