#ifndef ALARM_H
#define ALARM_H

#include "Lib_songs.h"

typedef struct{
    uint8_t hour;
    uint8_t minute;
    T_Song* song;
} alarm_inform_t;

void AlarmCheck(const uint8_t* const time);
void AlarmStop(void);
void AlarmSet(uint8_t index, uint8_t _hour, uint8_t _minute);

#endif //ALARM_H
