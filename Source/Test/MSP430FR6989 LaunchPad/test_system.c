/****************************************************************************************************
 * Define
 ****************************************************************************************************/

#define DRIVER_INTERRUPT_DISABLE_EXPECT() __disable_interrupt_Expect()
#define DRIVER_INTERRUPT_ENABLE_EXPECT()  __enable_interrupt_Expect()

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "mock_clock.h"
#include "mock_driver.h"
#include "mock_pmm.h"
#include "mock_wdt_a.h"
#include "system.h"
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
    /* Mock (System) */
    DRIVER_INTERRUPT_DISABLE_EXPECT();
    WDT_A_hold_Expect(WDT_A_BASE);
    PMM_unlockLPM5_Expect();

    /* Mock (Driver) */
    clock_init_Expect();

    /* Mock (Application) */

    /* Mock (System) */
    DRIVER_INTERRUPT_ENABLE_EXPECT();

    /* Initialize */
    system_init();
}
