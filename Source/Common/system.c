/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "clock.h"
#include "driver.h"
#include "pmm.h"
#include "project.h"
#include "serial.h"
#include <stddef.h>
#include "system.h"
#include "wdt_a.h"

/****************************************************************************************************
 * Variable
 ****************************************************************************************************/

PROJECT_STATIC(system_freeCallback_t system_freeCallback);
PROJECT_STATIC(system_mallocCallback_t system_mallocCallback);

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

static void system_initApplication(void);
static void system_initDriver(void);

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Free ***/
void system_free(void **memory)
{
    /*** Free ***/
    if((system_freeCallback != NULL) && (memory != NULL))
    {
        system_freeCallback(*memory);
        *memory = NULL;
    }
}

/*** Initialize ***/
void system_init(system_freeCallback_t freeCallback, system_mallocCallback_t mallocCallback)
{
    /*** Initialize ***/
    /* Variable */
    system_freeCallback = freeCallback;
    system_mallocCallback = mallocCallback;

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

/*** Malloc ***/
void *system_malloc(size_t size)
{
    /*** Malloc ***/
    if(system_mallocCallback != NULL)
        return system_mallocCallback(size);
    else
        return NULL;
}

/****************************************************************************************************
 * Function Definition (Private)
 ****************************************************************************************************/

/*** Initialize Application ***/
static void system_initApplication(void)
{
    /*** Initialize Application ***/
}

/*** Initialize Driver ***/
static void system_initDriver(void)
{
    /*** Initialize Driver ***/
    clock_init();
    serial_init();
}
