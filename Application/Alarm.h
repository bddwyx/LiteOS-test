#ifndef ALARM_H
#define ALARM_H

#include "DigitalTube.h"

#include "Lib_songs.h"

typedef struct{
    uint8_t hour;
    uint8_t minute;
    T_Song* song;
    bool onOff;
} alarm_inform_t;

extern const uint8_t ALARM_NUM;

void AlarmCheck(const uint8_t* const time);
void AlarmStop(void);
void AlarmGet(uint8_t index, uint8_t* buffer);
void AlarmSet(uint8_t index, uint8_t _hour, uint8_t _minute);
void AlarmInc(uint8_t index, uint8_t _hour, uint8_t _minute);
void AlarmChangeAvailability(uint8_t index, bool availability);
void AlarmShow(uint8_t index, bool isHourMode);
void AlarmShowAvailablity(uint8_t index);

#endif //ALARM_H
