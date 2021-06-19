#include "Clock.h"

/**
 * @feature Year must be within 2021-2255.
 */
uint8_t calender[4] = {21, 6, 20, 0};
uint8_t* const year = calender;
uint8_t* const month = calender + 1;
uint8_t* const date = calender + 2;
uint8_t* const solarTerm = calender + 3;
uint8_t time[3] = {12, 0, 0};
uint8_t* const hour = time;
uint8_t* const minute = time + 1;
uint8_t* const second = time + 2;
uint8_t showBuffer[6] = {0};

void ClockModuleInit(){
    EEPROMHWInit();
    EEPROMReadToRAM(0, time, 4);
    EEPROMReadToRAM(4, calender, 4);
}

void ClockDataStore(){
    EEPROMWriteToDst(0, time, 4);
}

void ClockCalendorStore(){
    EEPROMWriteToDst(4, calender, 4);
}

void ClockDateSet(uint8_t _year, uint8_t _month, uint8_t _date){
    *year = _year;
    *month = _month;
    *date = _date;

    ClockCalendorStore();
}

void ClockDateInc(){
    (*date)++;
    uint8_t daysInMonth;
    switch (*month) {
        case 2:
            if(((*year) % 4) == 0) daysInMonth = 29;
            else daysInMonth = 28;
            break;
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            daysInMonth = 31;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            daysInMonth = 30;
            break;
        default:;
    }

    if((*date) > daysInMonth){
        *date -= daysInMonth;
        *month += 1;
        if(*month > 12){
            *month -= 12;
            *year += 1;
        }
    }

    ClockCalendorStore();
}

void ClockTimeSet(uint8_t _hour, uint8_t _minute, uint8_t _second){
    *hour = _hour;
    *minute = _minute;
    *second = _second;
}

void ClockTimeInc(uint8_t _hour, uint8_t _minute, uint8_t _second){
    *hour += _hour;
    *minute += _minute;
    *second += _second;

    if(*second > 59){
        *minute += *second / 60;
        *second %= 60;
    }
    if(*minute > 59){
        *hour += *minute / 60;
        *minute %= 60;
    }
    *hour %= 24;
}

const uint8_t * ClockDateGet(){
    return calender;
}

const uint8_t * ClockTimeGet(){
    return time;
}

void ClockTick(){
    if(++*second > 59){
        *second = 0;
        if(++*minute > 59){
            *minute = 0;
            if(++*hour > 23){
                *hour = 0;
                ClockDateInc();
            }
        }

        AlarmCheck(time);
    }

    ClockDataStore();
}

void ClockBufferGenerate(){
    showBuffer[4] = *second / 10;
    showBuffer[5] = *second % 10;
    showBuffer[2] = *minute / 10;
    showBuffer[3] = *minute % 10;
    showBuffer[0] = *hour / 10;
    showBuffer[1] = *hour % 10;
}

void ClockDisplay(enum display_mode_e mode) {
    ClockBufferGenerate();

    switch (mode) {
        case FOURBIT_WITHDASH:
            tubeShowBuffer[0] = showBuffer[2];
            tubeShowBuffer[1] = showBuffer[3];
            tubeShowBuffer[2] = 16;
            tubeShowBuffer[3] = showBuffer[4];
            tubeShowBuffer[4] = showBuffer[5];
            tubeShowBuffer[5] = 17;
            tubeShowBuffer[6] = 17;
            tubeShowBuffer[7] = 17;
            break;
        case SIXBIT_WITHDASH:
            tubeShowBuffer[0] = showBuffer[0];
            tubeShowBuffer[1] = showBuffer[1];
            tubeShowBuffer[2] = 16;
            tubeShowBuffer[3] = showBuffer[2];
            tubeShowBuffer[4] = showBuffer[3];
            tubeShowBuffer[5] = 16;
            tubeShowBuffer[6] = showBuffer[4];
            tubeShowBuffer[7] = showBuffer[5];
            break;
        default:;
    }
}

void ClockDateDisplay(){
    tubeShowBuffer[0] = 2;
    tubeShowBuffer[1] = 0;
    tubeShowBuffer[2] = (*year) / 10;
    tubeShowBuffer[3] = (*year) % 10;
    tubeShowBuffer[4] = (*month) / 10;
    tubeShowBuffer[5] = (*month) % 10;
    tubeShowBuffer[6] = (*date) / 10;
    tubeShowBuffer[7] = (*date) % 10;
}

const uint8_t solarTermDate[24][2] = {
        {2, 3},
        {2, 18},
        {3, 5},
        {3, 20},
        {4, 4},
        {4, 20},

        {5, 5},
        {5, 21},
        {6, 5},
        {6, 21},
        {7, 7},
        {7, 22},

        {8, 7},
        {8, 23},
        {9, 7},
        {9, 23},
        {10, 8},
        {10, 23},

        {11, 7},
        {11, 22},
        {12, 7},
        {12, 21},
        {1, 5},
        {1, 20},
};

const uint8_t solarTermSeasonName[4][2] = {
        {5, 17},
        {5, 18},
        {10, 18},
        {8, 1},
};

void ClockSolarDisplay(){
    for(uint8_t i = 0; i < 24; i++){
        if((solarTermDate[i][0] == (*month)) && (solarTermDate[i][1] == (*date))){
            tubeShowBuffer[0] = 20;
            tubeShowBuffer[1] = 20;
            tubeShowBuffer[2] = 20;
            tubeShowBuffer[3] = 20;
            tubeShowBuffer[4] = 20;
            tubeShowBuffer[5] = solarTermSeasonName[i / 6][0];
            tubeShowBuffer[6] = solarTermSeasonName[i / 6][1];
            tubeShowBuffer[7] = i % 6 + 1;

            return;
        }
    }

    for(uint8_t i = 0; i < 5; i++) tubeShowBuffer[i] = 20;
    for(uint8_t i = 5; i < 8; i++) tubeShowBuffer[i] = 19;
}
