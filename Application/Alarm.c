#include "Alarm.h"

alarm_inform_t alarm[] = {
        {12, 0, &Astronomia, false},
        {12, 0, &Windmill, false},
        {12, 0, &Happybirthday, false},
        {12, 0, &Senbonzakura, false},
        {12, 0, &Pureland, false},
        {12, 0, &IronTorrent, false},
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
        if((time[0] == alarm[i].hour) && (time[1] == alarm[i].minute) && (alarm[i].onOff)){
            if(currentAlarm == NULL) AlarmStart(i);
        }
    }
}

void AlarmLoad(){
    for(uint8_t i = 0 ; i < ALARM_NUM; i++){
        uint8_t buffer[4];
        EEPROMReadToRAM(4 * (i + 2), buffer, 4);
        alarm[i].hour = buffer[0];
        alarm[i].minute = buffer[1];
        alarm[i].onOff = buffer[2];
    }
}

void AlarmStore(uint8_t alarmIndex){
    uint8_t buffer[4];
    buffer[0] = alarm[alarmIndex].hour;
    buffer[1] = alarm[alarmIndex].minute;
    buffer[2] = alarm[alarmIndex].onOff;

    EEPROMWriteToDst(4 * (alarmIndex + 2), buffer, 4);
}

void AlarmGet(uint8_t index, uint8_t* buffer){
    buffer[0] = alarm[index].hour;
    buffer[1] = alarm[index].minute;
}

void AlarmSet(uint8_t index, uint8_t _hour, uint8_t _minute){
    alarm[index].hour = _hour;
    alarm[index].minute = _minute;
    AlarmChangeAvailability(index, true);
    AlarmStore(index);
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

void AlarmChangeAvailability(uint8_t index, bool availability) {
    alarm[index].onOff = availability;
    AlarmStore(index);
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

void AlarmShowAvailablity(uint8_t index){
    tubeShowBuffer[0] = 20;
    tubeShowBuffer[1] = 20;
    tubeShowBuffer[2] = 20;
    tubeShowBuffer[3] = 20;
    tubeShowBuffer[4] = 20;
    if(alarm[index].onOff){
        tubeShowBuffer[5] = 20;
        tubeShowBuffer[6] = 0;
        tubeShowBuffer[7] = 21;
    }
    else{
        tubeShowBuffer[5] = 0;
        tubeShowBuffer[6] = 15;
        tubeShowBuffer[7] = 15;
    }
}
