#include "WaterFlow.h"

void DigitalShowNum(uint8_t bit, uint8_t num);
extern const uint8_t seg7[];

void WaterFlow1(){
    static uint8_t state = 0;

    switch (state) {
        case 0:
            DigitalShowNum(0, 1);
            DigitalDisplay(LED(1) | LED(0));
            break;

        case 1:
            DigitalShowNum(1, 2);
            DigitalDisplay(LED(2) | LED(1));
            break;

        case 2:
            DigitalShowNum(2, 3);
            DigitalDisplay(LED(3) | LED(2));
            break;

        case 3:
            DigitalShowNum(3, 4);
            DigitalDisplay(LED(4) | LED(3));
            break;

        case 4:
            DigitalShowNum(4, 5);
            DigitalDisplay(LED(5) | LED(4));
            break;

        case 5:
            DigitalShowNum(5, 6);
            DigitalDisplay(LED(6) | LED(5));
            break;

        case 6:
            DigitalShowNum(6, 7);
            DigitalDisplay(LED(7) | LED(6));
            break;

        case 7:
            DigitalShowNum(7, 8);
            DigitalDisplay(LED(0) | LED(7));
            break;

        default:;
    }

    state++;
    state %= 8;
}
