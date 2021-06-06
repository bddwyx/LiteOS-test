#include "printf.h"

int fputc(int ch, FILE* stream)
{
    UARTCharPutNonBlocking(UART0_BASE, (uint8_t)ch);
    return ch;
}
