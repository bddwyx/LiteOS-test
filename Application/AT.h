#ifndef AT_H
#define AT_H

#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <rtthread.h>

#include "UART0.h"

void ATCmdDetect(void);

#endif //AT_H
