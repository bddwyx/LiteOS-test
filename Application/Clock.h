#ifndef CLOCK_H
#define CLOCK_H

#include "DigitalTube.h"
#include "BSP_EEPROM.h"

#include "Alarm.h"

enum display_mode_e {
    FOURBIT_WITHDASH = 0,
    SIXBIT_WITHDASH,
};

void ClockDateSet(uint8_t _year, uint8_t _month, uint8_t _date);
void ClockDateInc(void);
void ClockTimeSet(uint8_t _hour, uint8_t _minute, uint8_t _second);
void ClockTimeInc(uint8_t _hour, uint8_t _minute, uint8_t _second);
const uint8_t * ClockTimeGet(void);
const uint8_t * ClockDateGet(void);

void ClockModuleInit(void);
void ClockTick(void);
void ClockTickChangeAvailability(bool availability);
void ClockDisplay(enum display_mode_e mode);
void ClockDateDisplay(void);
void ClockSolarDisplay(void);
void ClockTimeSetShow(uint8_t state);
void ClockCalendorSetByBit(uint8_t category, int8_t num);

#endif //CLOCK_H
