/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "clock.h"
#include "cs.h"
#include "driver.h"
#include "gpio.h"

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Initialize ****/
void clock_init(void)
{
    /*** Initialize ****/
    /* Frequencies */
    CS_setExternalClockSource(32768, 0); // 0 (No High-Frequency Crystal Oscillator (HFXT))

    /* Low-Frequency Crystal Oscillator (LFXT) */
    GPIO_setAsPeripheralModuleFunctionInputPin(DRIVER_LFXT_GPIO_IN_PORT, DRIVER_LFXT_GPIO_IN_PIN, DRIVER_LFXT_GPIO_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(DRIVER_LFXT_GPIO_OUT_PORT, DRIVER_LFXT_GPIO_OUT_PIN, DRIVER_LFXT_GPIO_FUNCTION);
    CS_turnOnLFXT(CS_LFXT_DRIVE_3); // Maximum Drive Strength

    /* System Clocks */
    CS_setDCOFreq(CS_DCORSEL_0, CS_DCOFSEL_6); // Digitally Controlled Oscillator (DCO): 8 MHz
    CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1); // Auxiliary Clock (ACLK): 32768 Hz / 1 = 32768 Hz
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1); // Master Clock (MCLK): 8 MHz / 1 = 8 MHz
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1); // Sub-Main CLock (SMCLK): 8 MHz / 1 = 8 MHz
}
