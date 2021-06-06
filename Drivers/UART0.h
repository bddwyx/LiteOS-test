#ifndef UART0_H
#define UART0_H

#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <rtthread.h>

#include "sysctl.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "pin_map.h"
#include "interrupt.h"
#include "gpio.h"
#include "uart.h"

#include "config.h"

typedef struct {
    bool buffer_availability;
    uint8_t buffer_content_size;
} UART0_buffer_inform_t;

extern UART0_buffer_inform_t UART0BufferInformation;

void UART0HWInit(void);
void UARTStringPutNonBlocking(const uint8_t* cMessage);
const uint8_t* UART0GetBufferPointer(void);

#endif //UART0_H
