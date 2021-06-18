#ifndef CLOCK_H
#define CLOCK_H

#include "DigitalTube.h"

enum display_mode_e {
    FOURBIT_WITHDASH = 0,
    SIXBIT_WITHDASH,
};

void ClockDateSet(uint8_t _year, uint8_t _month, uint8_t _date);
void ClockDateInc(void);
void ClockTimeSet(uint8_t _hour, uint8_t _minute, uint8_t _second);
void ClockTimeInc(uint8_t _hour, uint8_t _minute, uint8_t _second);
const uint8_t *const ClockTimeGet(void);
const uint8_t *const ClockDateGet(void);

void ClockTick(void);
void ClockDisplay(enum display_mode_e mode);

#endif //CLOCK_H
