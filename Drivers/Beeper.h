#ifndef BEEPER_H
#define BEEPER_H

#include <stdint.h>
#include <stdbool.h>

#include "hw_memmap.h"
#include "pin_map.h"
#include "gpio.h"
#include "sysctl.h"
#include "hw_pwm.h"
#include "pwm.h"

void BeepInit(void);
void Beep(void);

#endif //BEEPER_H
