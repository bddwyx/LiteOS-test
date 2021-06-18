#include "BSP_I2C.h"

void Delay(uint32_t value)
{
    uint32_t ui32Loop;

    for(ui32Loop = 0; ui32Loop < value; ui32Loop++){};
}

uint8_t I2C0_WriteByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteData)
{
    uint8_t rop;

    while(I2CMasterBusy(I2C0_BASE)){}; //忙等待
    I2CMasterSlaveAddrSet(I2C0_BASE, DevAddr, false); //设从机地址，写
    I2CMasterDataPut(I2C0_BASE, RegAddr);	//设数据地址
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START); //启动总线发送
    while(I2CMasterBusy(I2C0_BASE)){};
    rop = (uint8_t)I2CMasterErr(I2C0_BASE);

    I2CMasterDataPut(I2C0_BASE, WriteData); //设数据值
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH); //启动总线发送，发送后停止
    while(I2CMasterBusy(I2C0_BASE)){};
    rop = (uint8_t)I2CMasterErr(I2C0_BASE);

    return rop;
}

uint8_t I2C0_ReadByte(uint8_t DevAddr, uint8_t RegAddr)
{
    uint8_t value;

    while(I2CMasterBusy(I2C0_BASE)){};	//忙等待
    I2CMasterSlaveAddrSet(I2C0_BASE, DevAddr, false); //设从机地址，写
    I2CMasterDataPut(I2C0_BASE, RegAddr); //设数据地址
    I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_SINGLE_SEND); //启动总线发送
    while(I2CMasterBusBusy(I2C0_BASE));
    if (I2CMasterErr(I2C0_BASE) != I2C_MASTER_ERR_NONE)
        return 0; //错误
    Delay(100);

    //receive data
    I2CMasterSlaveAddrSet(I2C0_BASE, DevAddr, true); //设从机地址，读
    I2CMasterControl(I2C0_BASE,I2C_MASTER_CMD_SINGLE_RECEIVE); //启动总线接收
    while(I2CMasterBusBusy(I2C0_BASE));
    if (I2CMasterErr(I2C0_BASE) != I2C_MASTER_ERR_NONE)
        return 0; //错误
    Delay(100);

    value=I2CMasterDataGet(I2C0_BASE);

    return value;
}
