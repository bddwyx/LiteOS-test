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
            DigitalTubeChangeBrightness(1);
            break;
        case KEYE2:
            DigitalTubeChangeBrightness(-1);
            break;
        case KEYE3:
            break;
        case KEYE4:
            break;
        case KEYE5:
            break;
        case KEYE6:
            break;
        case KEYE7:
            break;
        case KEYE8:
            break;
        case QEI:
            break;
        default:;
    }
}
