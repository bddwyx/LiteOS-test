#ifndef WORKSTATEFSM_H
#define WORKSTATEFSM_H

#include <stdint.h>

#include "DigitalTube.h"

#include "Clock.h"
#include "Callback.h"

typedef void (*output_logic_t)(key_trig_type_e input);
typedef void (*state_trans_t)(key_trig_type_e input);

typedef struct{
    output_logic_t outputLogic;
    state_trans_t stateTrans;
} FSM_t;

#endif //WORKSTATEFSM_H
