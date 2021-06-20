#ifndef STOPWATCH_H
#define STOPWATCH_H

#include "DigitalTube.h"

#include "Lib_songs.h"

void StopWatchOn(void);
void StopWatchPend(void);
void StopWatchModeSwitch(bool state);
void StopWatchSet(uint8_t seconds);
void StopWatchInc(int8_t seconds);

#ifdef RTOS_LOS
uint32_t StopWatchRTTInit(void);
#endif

#endif //STOPWATCH_H
