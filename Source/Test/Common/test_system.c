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
#include "mock_serial.h"
#include "mock_wdt_a.h"
#include <stdlib.h>
#include "system.h"
#include "unity.h"

/****************************************************************************************************
 * Variable
 ****************************************************************************************************/

extern system_freeCallback_t system_freeCallback;
extern system_mallocCallback_t system_mallocCallback;

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

/*** Free ***/
void test_free_1(void)
{
    /*** Free ***/
    /* Variable */
    void *memory;

    /* Set Up */
    memory = malloc(1);
    system_freeCallback = NULL;

    /* Free */
    system_free(&memory);

    /* Clean Up */
    free(memory);
}

void test_free_2(void)
{
    /*** Free ***/
    /* Set Up */
    system_freeCallback = free;

    /* Free */
    system_free(NULL);
}

void test_free_3(void)
{
    /*** Free ***/
    /* Variable */
    void *memory;

    /* Set Up */
    memory = malloc(1);
    system_freeCallback = free;

    /* Free */
    system_free(&memory);

    /* Verify */
    TEST_ASSERT_NULL(memory);
}

/*** Initialize ***/
void test_init(void)
{
    /*** Initialize ***/
    /* Mock */
    DRIVER_INTERRUPT_DISABLE_EXPECT();
    WDT_A_hold_Expect(WDT_A_BASE);
    PMM_unlockLPM5_Expect();
    clock_init_Expect();
    serial_init_Expect();
    DRIVER_INTERRUPT_ENABLE_EXPECT();

    /* Initialize */
    system_init(free, malloc);

    /* Verify */
    TEST_ASSERT_EQUAL_PTR(free, system_freeCallback);
    TEST_ASSERT_EQUAL_PTR(malloc, system_mallocCallback);
}

/*** Malloc ***/
void test_malloc_1(void)
{
    /*** Malloc ***/
    /* Variable */
    void *memory;

    /* Set Up */
    system_mallocCallback = NULL;

    /* Malloc */
    memory = system_malloc(1);

    /* Verify */
    TEST_ASSERT_NULL(memory);
}

void test_malloc_2(void)
{
    /*** Malloc ***/
    /* Variable */
    void *memory;

    /* Set Up */
    system_mallocCallback = malloc;

    /* Malloc */
    memory = system_malloc(1);

    /* Verify */
    TEST_ASSERT_NOT_NULL(memory);

    /* Clean Up */
    free(memory);
}
