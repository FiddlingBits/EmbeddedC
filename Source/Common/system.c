/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "clock.h"
#include "driver.h"
#include "pmm.h"
#include "system.h"
#include "wdt_a.h"

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

static void system_initApplication(void);
static void system_initDriver(void);

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Initialize ****/
void system_init(void)
{
    /*** Initialize ****/
    /* System */
    DRIVER_INTERRUPT_DISABLE();
    WDT_A_hold(WDT_A_BASE); // Disable/Hold Watchdog Timer
    PMM_unlockLPM5(); // Disable GPIO Power-On Default High-Impedance Mode 

    /* Driver */
    system_initDriver();

    /* Application */
    system_initApplication();

    /* System */
    DRIVER_INTERRUPT_ENABLE();
}

/****************************************************************************************************
 * Function Definition (Private)
 ****************************************************************************************************/

/*** Initialize Application ***/
static void system_initApplication(void)
{
    /*** Initialize Application ***/
}

/*** Initialize Driver ****/
static void system_initDriver(void)
{
    /*** Initialize Driver ***/
    clock_init();
}
