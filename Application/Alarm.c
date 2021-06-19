#include "Alarm.h"

alarm_inform_t alarm[] = {
        {03, 12, &Astronomia},
        {24, 0, &Windmill},
};

const uint8_t ALARM_NUM = sizeof(alarm) / sizeof(alarm[0]);

T_Song* currentAlarm = NULL;

void AlarmStart(uint8_t alarmIndex){
    if(alarm[alarmIndex].song != NULL){
        MusicStart(alarm[alarmIndex].song);
        currentAlarm = alarm[alarmIndex].song;
    }
    else{
        MusicStart(&Astronomia);
        currentAlarm = &Astronomia;
    }
}

void AlarmStop(){
    if(currentAlarm != NULL){
        MusicStop(currentAlarm);
        currentAlarm = NULL;
    }
}

void AlarmCheck(const uint8_t* const time){
    for(uint8_t i = 0; i < ALARM_NUM; i++){
        if((time[0] == alarm[i].hour) && (time[1] == alarm[i].minute)){
            if(currentAlarm == NULL) AlarmStart(i);
        }
    }
}

void AlarmGet(uint8_t index, uint8_t* buffer){
    buffer[0] = alarm[index].hour;
    buffer[1] = alarm[index].minute;
}

void AlarmSet(uint8_t index, uint8_t _hour, uint8_t _minute){
    alarm[index].hour = _hour;
    alarm[index].minute = _minute;
}

void AlarmInc(uint8_t index, uint8_t _hour, uint8_t _minute){
    alarm[index].minute += _minute;
    if(alarm[index].minute > 59){
        alarm[index].minute %= 60;
        alarm[index].hour++;
    }
    alarm[index].hour += _hour;
    alarm[index].hour %= 24;
}

void AlarmShow(uint8_t index, bool isHourMode){
    tubeShowBuffer[0] = 20;
    tubeShowBuffer[1] = 20;
    tubeShowBuffer[2] = 20;
    tubeShowBuffer[3] = alarm[index].hour / 10;
    tubeShowBuffer[4] = alarm[index].hour % 10;
    tubeShowBuffer[5] = 19;
    if(isHourMode){
        tubeShowBuffer[6] = 20;
        tubeShowBuffer[7] = 20;
    }
    else{
        tubeShowBuffer[6] = alarm[index].minute / 10;
        tubeShowBuffer[7] = alarm[index].minute % 10;
    }
}
