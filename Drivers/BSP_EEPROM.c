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
}

void EEPROMWriteToDst(uint8_t start, uint8_t* data, uint8_t size){
    EEPROMProgram((uint32_t *) data, EEPROMADDRBASE + start, size);
}

void EEPROMReadToRAM(uint8_t start, uint8_t* data, uint8_t size){
    EEPROMRead((uint32_t *) data, EEPROMADDRBASE + start, size);
}
