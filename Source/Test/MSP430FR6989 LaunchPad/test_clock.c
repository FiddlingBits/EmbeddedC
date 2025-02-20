/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "clock.h"
#include "driver.h"
#include "mock_cs.h"
#include "mock_gpio.h"
#include "unity.h"

/****************************************************************************************************
 * Set Up/Tear Down
 ****************************************************************************************************/

void setUp(void)
{
}

void tearDown(void)
{
}

/****************************************************************************************************
 * Test (Public)
 ****************************************************************************************************/

/*** Initialize ****/
void test_init(void)
{
    /*** Initialize ***/
    /* Mock */
    CS_setExternalClockSource_Expect(32768, 0);
    GPIO_setAsPeripheralModuleFunctionInputPin_Expect(DRIVER_LFXT_GPIO_IN_PORT, DRIVER_LFXT_GPIO_IN_PIN, DRIVER_LFXT_GPIO_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin_Expect(DRIVER_LFXT_GPIO_OUT_PORT, DRIVER_LFXT_GPIO_OUT_PIN, DRIVER_LFXT_GPIO_FUNCTION);
    CS_turnOnLFXT_Expect(CS_LFXT_DRIVE_3);
    CS_setDCOFreq_Expect(CS_DCORSEL_0, CS_DCOFSEL_6);
    CS_initClockSignal_Expect(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal_Expect(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal_Expect(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* Initialize */
    clock_init();
}
