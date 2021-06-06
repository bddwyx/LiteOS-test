#ifndef BEEPER_H
#define BEEPER_H

#include <stdint.h>
#include <stdbool.h>

#include "hw_memmap.h"
#include "gpio.h"
#include "sysctl.h"

void BeepInit(void);
void Beep(void);

#endif //BEEPER_H
