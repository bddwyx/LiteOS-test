#include "Callback.h"

#include "KeyScan.h"

#include "Alarm.h"

void KeyTrigCallback(key_trig_type_e tirgger){
    AlarmStop();

    switch (tirgger) {
        case KEY1:
            break;
        case KEY2:
            break;
        case KEYE1:
            break;
        case KEYE2:
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
