#include "DigitalTube.h"

const uint8_t seg7[] = {
        0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
        0x7f, 0x6f, 0x77, 0x7c, 0x58, 0x5e, 0x79, 0x71, 0x40};

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

void DigitalTubeHWInit(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    I2CMasterInitExpClk(I2C0_BASE, 20000000/*SysCtlClockGet()*/, true);										//config I2C0 400k
    I2CMasterEnable(I2C0_BASE);

    I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_CONFIG_PORT0,0xff);		//config port 0 as input
    I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_CONFIG_PORT1,0x0);			//config port 1 as output
    I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_CONFIG_PORT2,0x0);			//config port 2 as output

    I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_CONFIG,0x00);					//config port as output
    I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT,0xff);				//turn off the LED1-8
}

void DigitalShowNum(uint8_t bit, uint8_t num){
    I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2, 0);		//消隐
    I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT1, seg7[num]);				//write port 1（‘0’的码段值）
    I2C0_WriteByte(TCA6424_I2CADDR,TCA6424_OUTPUT_PORT2, 1 << bit);		//write port 2（点亮第1个数码管，高点亮）
}

void DigitalDisplay(uint8_t mask){
    I2C0_WriteByte(PCA9557_I2CADDR,PCA9557_OUTPUT, ~mask);
}

uint8_t tubeShowBuffer[8] = {
        2, 0, 2, 1, 0, 6, 0, 2,
};

static uint8_t ledShowBuffer[2] = {0};

void DigitalChangeLEDBuffer(uint8_t mask){
    ledShowBuffer[1] = mask;
}

#ifdef RTOS_LOS

static uint32_t tube_thread_handle;

static void tube_thread_entry(void* param){
    while(1) {
        static uint8_t step = 0;
        DigitalShowNum(step, tubeShowBuffer[step]);
        step++;
        step %= 8;

        if(ledShowBuffer[0] != ledShowBuffer[1]){
            ledShowBuffer[0] = ledShowBuffer[1];
            DigitalDisplay(ledShowBuffer[0]);
        }

        LOS_TaskDelay(1); // Delay
    }
}

/**
 * @brief Start digital-tube thread
 * @ret Status of thread init function
 */
uint32_t DigitalTubeRTTInit(){

    DigitalTubeHWInit();

    uint32_t uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 4;
    task_init_param.pcName = "Digital Tube";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)tube_thread_entry;
    task_init_param.uwStackSize = 512;
    uwRet = LOS_TaskCreate(&tube_thread_handle, &task_init_param);
    if (uwRet != LOS_OK)
    {
        return uwRet;
    }
    return LOS_OK;
}

#endif

