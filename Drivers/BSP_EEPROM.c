#include "BSP_EEPROM.h"

const uint32_t EEPROMADDRBASE = 0x400;

/*EEPROM Initialization*/
void EEPROMHWInit(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_EEPROM0)){}

    uint32_t EEPROMInitResult = EEPROMInit();
    if (EEPROMInitResult != EEPROM_INIT_OK)
    {
        while(1);
    }

    //Disables the EEPROM write and erase interrupt
    EEPROMIntDisable(EEPROM_INT_PROGRAM);

    /*//Reads the memory content into the array, and check if it has been written before
    EEPROMRead(DateTimeArray, EEPROMADDRBASE, sizeof(DateTimeArray));
    for (uint8_t i = 0; i < 6; ++i)
    {
        if(DateTimeArray[i] == 0xFFFFFFFF)
            continue;
        else
        {
            MemoryNotWritten = false;
            break;
        }
    }

    //If EEPROM hasn't been written before, program the initial date and time
    if (MemoryNotWritten)
    {
        DateTimeArray[0] = 2021;
        DateTimeArray[1] = 6;
        DateTimeArray[2] = 13;
        DateTimeArray[3] = 12;
        DateTimeArray[4] = 0;
        DateTimeArray[5] = 0;

        EEPROMProgram(DateTimeArray, EEPROMADDRBASE, sizeof(DateTimeArray));
    }

    //Read the content into the array again
    EEPROMRead(DateTimeArray, EEPROMADDRBASE, sizeof(DateTimeArray));*/
}

void EEPROMWriteToDst(uint8_t start, uint8_t* data, uint8_t size){
    EEPROMProgram((uint32_t *) data, EEPROMADDRBASE + start, size);
}

void EEPROMReadToRAM(uint8_t start, uint8_t* data, uint8_t size){
    EEPROMRead((uint32_t *) data, EEPROMADDRBASE + start, size);
}
