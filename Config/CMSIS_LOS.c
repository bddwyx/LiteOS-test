#include "CMSIS_LOS.h"

/**
  \brief   System Tick Configuration
  \details Initializes the System Timer and its interrupt, and starts the System Tick Timer.
           Counter is in free running mode to generate periodic interrupts.
  \param [in]  ticks  Number of ticks between two interrupts.
  \return          0  Function succeeded.
  \return          1  Function failed.
  \note    When the variable <b>__Vendor_SysTickConfig</b> is set to 1, then the
           function <b>SysTick_Config</b> is not included. In this case, the file <b><i>device</i>.h</b>
           must contain a vendor-specific implementation of this function.
 */
uint32_t SysTick_Config(uint32_t ticks)
{
    SysTickPeriodSet(ticks);
    SysTickEnable();
    SysTickIntEnable();     /* Function successful */
    return 0;
}
