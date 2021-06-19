#include "CMDSystem.h"

uint8_t CMDBuffer[17] = {0};

void CMDFuncSET(){
    uint8_t hour, minute, second;
    hour = (CMDBuffer[3] - '0') * 10 + (CMDBuffer[4] - '0');
    minute = (CMDBuffer[6] - '0') * 10 + (CMDBuffer[7] - '0');
    second = (CMDBuffer[9] - '0') * 10 + (CMDBuffer[10] - '0');
    ClockTimeSet(hour, minute, second);
}

void CMDFuncINC(){
    uint8_t hour, minute, second;
    hour = (CMDBuffer[3] - '0') * 10 + (CMDBuffer[4] - '0');
    minute = (CMDBuffer[6] - '0') * 10 + (CMDBuffer[7] - '0');
    second = (CMDBuffer[9] - '0') * 10 + (CMDBuffer[10] - '0');
    ClockTimeInc(hour, minute, second);
}

void CMDFuncGetTime(){
    uint8_t showBuffer[15];
    sprintf((char*)showBuffer, "TIME%02d:%02d:%02d\r\n", *ClockTimeGet(), *(ClockTimeGet() + 1), *(ClockTimeGet() + 2));
    UARTStringPutNonBlocking(showBuffer);
}

void CMDFuncDateSet(){
    uint8_t year, month, date;
    year = (CMDBuffer[6] - '0') * 10 + (CMDBuffer[7] - '0');
    month = (CMDBuffer[8] - '0') * 10 + (CMDBuffer[9] - '0');
    date = (CMDBuffer[10] - '0') * 10 + (CMDBuffer[11] - '0');
    ClockDateSet(year, month, date);
    UARTStringPutNonBlocking("Success\r\n");
}

bool CMDCheckAvailability(const uint8_t* buffer, uint8_t size){
    uint8_t i;
    for(i = 0; i < size; i++){
        CMDBuffer[i] = toupper(buffer[i]);
    }
    CMDBuffer[size] = '\0';
    return true;
}

void CMDOperation(const uint8_t* buffer, uint8_t size){
    if(CMDCheckAvailability(buffer, size)){
        if(strncmp((const char*)(CMDBuffer), "SET", 3) == 0){
            CMDFuncSET();
        }

        if(strncmp((const char*)CMDBuffer, "INC", 3) == 0){
            CMDFuncINC();
        }

        if(strncmp((const char*)CMDBuffer, "GETTIME", 7) == 0){
            CMDFuncGetTime();
        }

        if(strncmp((const char*)CMDBuffer, "DATE", 4) == 0){
            CMDFuncDateSet();
        }
    }
}

void CMDDetect(){
    if(UART0BufferInformation.buffer_availability){
        CMDOperation(UART0GetBufferPointer(), UART0BufferInformation.buffer_content_size);
        UART0BufferInformation.buffer_availability = false;
    }
}
