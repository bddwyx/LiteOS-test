#ifndef CALLBACK_H
#define CALLBACK_H

/** 跨层文件，供驱动层调用 **/

typedef enum{
    KEY1 = 0,
    KEY2,
    KEYE1,
    KEYE2,
    KEYE3,
    KEYE4,
    KEYE5,
    KEYE6,
    KEYE7,
    KEYE8,
    QEI,
    KEY_TRIG_NUM,
}key_trig_type_e;

void KeyTrigCallback(key_trig_type_e trigger);

#endif //CALLBACK_H
