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

static uint8_t timeSettingBuffer[3];
static uint8_t alarmSettingBuffer[3];

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
            StopWatchModeSwitch(true);
            OperationChangeAvailability(false, false);
            break;

        case KEYE7:
            OperationChangeAvailability(false, false);
            currentState = ALARM;
            AlarmOutputLogic(KEY1); //刷新显示，KEY1无意义
            break;

        case KEYE8:
            break;
    }
}

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
    switch (input) {
        case KEYE2:
            StopWatchInc(1);
            break;
        case KEYE3:
            StopWatchInc(-1);
            break;
        case KEYE4:
            StopWatchOn();
            break;
        case QEI:
            StopWatchInc(GetQEIValueChange());
            break;
    }

    StopWatchTransFunc(input);
}

void StopWatchTransFunc(key_trig_type_e input) {
    switch (input) {
        case KEYE8:
            currentState = DISPLAY;
            StopWatchModeSwitch(false);
            OperationChangeAvailability(true, true);
            break;
    }
}

void AlarmOutputLogic(key_trig_type_e input) {
    /** 0:：闹钟选择，1：小时设定，2：分钟设定，3：使能 **/
    static uint8_t state = 0;
    static uint8_t alarmIndex = 0;

    switch (state) {
        case 0:
            switch(input){
                case KEYE2:
                    if(alarmIndex < ALARM_NUM - 1) alarmIndex++;
                    break;
                case KEYE3:
                    if(alarmIndex > 0) alarmIndex--;
                    break;
                case KEYE4:
                    state = 1;
                    AlarmGet(alarmIndex, alarmSettingBuffer);
                    AlarmShow(alarmIndex, true);
                    return;
                    break;
            }
            for(uint8_t i = 0; i < 6; i++) tubeShowBuffer[i] = 20;
            tubeShowBuffer[6] = alarmIndex / 10;
            tubeShowBuffer[7] = alarmIndex % 10;
            break;

        case 1:
            switch (input) {
                case KEYE2:
                    AlarmInc(alarmIndex, 1, 0);
                    AlarmShow(alarmIndex, true);
                    break;
                case KEYE3:
                    AlarmInc(alarmIndex, 23, 0);
                    AlarmShow(alarmIndex, true);
                    break;
                case QEI:
                    if(GetQEIValueChange() > 0) AlarmInc(alarmIndex, 1, 0);
                    else AlarmInc(alarmIndex, 23, 0);
                    AlarmShow(alarmIndex, true);
                    break;
                case KEYE4:
                    state = 2;
                    AlarmShow(alarmIndex, false);
                    break;
            }
            break;

        case 2:
            switch (input) {
                case KEYE2:
                    AlarmInc(alarmIndex, 0, 1);
                    AlarmShow(alarmIndex, false);
                    break;
                case KEYE3:
                    AlarmInc(alarmIndex, 23, 59);
                    AlarmShow(alarmIndex, false);
                    break;
                case QEI:
                    if(GetQEIValueChange() > 0) AlarmInc(alarmIndex, 0, 1);
                    else AlarmInc(alarmIndex, 23, 59);
                    AlarmShow(alarmIndex, false);
                    break;
                case KEYE4:
                    state = 3;
                    AlarmShowAvailablity(alarmIndex);
                    break;
            }
            break;

        case 3:
            switch (input) {
                case KEYE2:
                    AlarmChangeAvailability(alarmIndex, true);
                    AlarmShowAvailablity(alarmIndex);
                    break;
                case KEYE3:
                    AlarmChangeAvailability(alarmIndex, false);
                    AlarmShowAvailablity(alarmIndex);
                    break;
                case KEYE4:
                    state = 0;
                    AlarmTransFunc(KEYE8);  //切换回正常状态
                    return;
                    break;
            }
            break;

    }

    AlarmTransFunc(input);
}

void AlarmTransFunc(key_trig_type_e input) {
    switch (input) {
        case KEYE8:
            currentState = DISPLAY;
            OperationChangeAvailability(true, true);
            break;
    }
}

void FSMSwitch(key_trig_type_e input){
    FSMLogic[currentState].outputLogic(input);
}
