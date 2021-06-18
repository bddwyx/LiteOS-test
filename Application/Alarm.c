#include "Alarm.h"

alarm_inform_t alarm[] = {
        {12, 0, &Astronomia},
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
    MusicStop(currentAlarm);
    currentAlarm = NULL;
}

void AlarmCheck(const uint8_t* const time){
    for(uint8_t i = 0; i < ALARM_NUM; i++){
        if((time[0] == alarm[i].hour) && (time[1] == alarm[i].minute)){
            if(currentAlarm == NULL) AlarmStart(i);
        }
    }
}

void AlarmSet(uint8_t index, uint8_t _hour, uint8_t _minute){
    alarm[index].hour = _hour;
    alarm[index].minute = _minute;
}

uint8_t AlarmGetHour(uint8_t index){
    return alarm[index].hour;
}

uint8_t AlarmGetMinute(uint8_t index){
    return alarm[index].minute;
}
