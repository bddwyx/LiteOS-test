#include "WorkStateFSM.h"

void DisplayOutputLogic(key_trig_type_e input);
void DisplayTransFunc(key_trig_type_e input);
void SettingOutputLogic(key_trig_type_e input);
void SettingTransFunc(key_trig_type_e input);
void StopWatchOutputLogic(key_trig_type_e input);
void StopWatchTransFunc(key_trig_type_e input);
void AlarmOutputLogic(key_trig_type_e input);
void AlarmTransFunc(key_trig_type_e input);

typedef enum{
    DISPLAY = 0,
    SETTING,
    STOPWATCH,
    ALARM,
} FSM_e;

FSM_e currentState = DISPLAY;

FSM_t FSMLogic[] = {
        {DisplayOutputLogic, DisplayTransFunc},
        {SettingOutputLogic, SettingTransFunc},
        {StopWatchOutputLogic, StopWatchTransFunc},
        {AlarmOutputLogic, AlarmTransFunc},
};

void DisplayOutputLogic(key_trig_type_e input){
    /** 0-时间，1-日期，2-节气 **/
    static uint8_t state = 0;

    if(input == KEYE5) {  //显示切换
        state++;
        state %= 3;
    }

    if(input == QEI){   //亮度切换
        if(GetQEIValueChange() > 0) DigitalTubeChangeBrightness(-1);
        else DigitalTubeChangeBrightness(1);
    }

    switch(state){
        case 0:
            OperationChangeAvailability(true, true);
            ClockDisplay(SIXBIT_WITHDASH);
            break;

        case 1:
            OperationChangeAvailability(true, false);
            ClockDateDisplay();
            break;

        case 2:
            ClockSolarDisplay();
            break;

        default:;
    }

    DisplayTransFunc(input);
}

void DisplayTransFunc(key_trig_type_e input){
    switch (input) {
        case KEYE6:
            currentState = STOPWATCH;
            break;

        case KEYE7:
            currentState = ALARM;
            break;

        case KEYE8:
            break;
    }
}

static uint8_t timeSettingBuffer[3];

void SettingOutputLogic(key_trig_type_e input) {
    /** 0：小时，1：分钟，2：秒 **/
    static uint8_t state = 0;

    if(input == KEYE8){
        state++;
        if(state == 4){
            ClockTimeSet(timeSettingBuffer[0], timeSettingBuffer[1], timeSettingBuffer[2]);
            state = 0;
        }
    }

    switch (state) {
        case 0:
            memcpy(timeSettingBuffer, ClockTimeGet(), 3);
            state++;
            break;

        case 1:

            break;

        case 2:

            break;

        case 3:

            break;

        default:;
    }
}

void SettingTransFunc(key_trig_type_e input) {

}

void StopWatchOutputLogic(key_trig_type_e input) {

}

void StopWatchTransFunc(key_trig_type_e input) {

}

void AlarmOutputLogic(key_trig_type_e input) {

}

void AlarmTransFunc(key_trig_type_e input) {

}

void FSMSwitch(key_trig_type_e input){
    FSMLogic[currentState].outputLogic(input);
}
