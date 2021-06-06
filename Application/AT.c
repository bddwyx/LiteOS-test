#include "AT.h"

uint8_t ATBuffer[14] = {0};

void ATFuncCLASS(){
    UARTStringPutNonBlocking((const uint8_t *)"F1903202\r\n");
}

void ATFuncSTUDENTCODE(){
    UARTStringPutNonBlocking((const uint8_t *)"519021910609\r\n");
}

bool ATCheckAvailability(const uint8_t* buffer, uint8_t size){
    if((toupper(buffer[0]) == 'A') && (toupper(buffer[1]) == 'T') && (buffer[2] == '+')){
        uint8_t i;
        for(i = 3; i < size; i++){
            ATBuffer[i - 3] = toupper(buffer[i]);
            if((ATBuffer[i - 3] > 'Z') || (ATBuffer[i - 3] < 'A')) return false;
        }
        ATBuffer[size - 3] = '\0';
    }
    return true;
}

void ATOperation(const uint8_t* buffer, uint8_t size){
    if(ATCheckAvailability(buffer, size)){
        if(strcmp((const char*)ATBuffer, "CLASS") == 0){
            ATFuncCLASS();
        }

        if(strcmp((const char*)ATBuffer, "STUDENTCODE") == 0){
            ATFuncSTUDENTCODE();
        }
    }
}

void ATCmdDetect(){
    if(UART0BufferInformation.buffer_availability){
        ATOperation(UART0GetBufferPointer(), UART0BufferInformation.buffer_content_size);
        UART0BufferInformation.buffer_availability = false;
    }
}
