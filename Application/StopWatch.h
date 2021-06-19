#ifndef STOPWATCH_H
#define STOPWATCH_H

#include "DigitalTube.h"

#include "Lib_songs.h"

void StopWatchOn(void);
void StopWatchModeSwitch(void);

#ifdef RTOS_LOS
uint32_t StopWatchRTTInit(void);
#endif

#endif //STOPWATCH_H
