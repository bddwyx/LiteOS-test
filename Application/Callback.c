#include "Callback.h"

#include "KeyScan.h"
#include "DigitalTube.h"

#include "Alarm.h"
#include "WorkStateFSM.h"

void KeyTrigCallback(key_trig_type_e trigger){
    AlarmStop();
    FSMSwitch(trigger);

    switch (trigger) {
        case KEY1:
            SysCtlReset();
            break;
        case KEY2:
            break;
        case KEYE1:
            DigitalTubeBrightnessMax();
            break;
        case KEYE2:
            DigitalTubeBrightnessMax();
            break;
        case KEYE3:
            DigitalTubeBrightnessMax();
            break;
        case KEYE4:
            DigitalTubeBrightnessMax();
            break;
        case KEYE5:
            DigitalTubeBrightnessMax();
            break;
        case KEYE6:
            DigitalTubeBrightnessMax();
            break;
        case KEYE7:
            DigitalTubeBrightnessMax();
            break;
        case KEYE8:
            DigitalTubeBrightnessMax();
            break;
        case QEI:
            break;
        default:;
    }
}
