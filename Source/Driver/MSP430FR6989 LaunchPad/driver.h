/****************************************************************************************************
 * Pragma
 ****************************************************************************************************/

#pragma once

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "gpio.h"

/****************************************************************************************************
 * Define
 ****************************************************************************************************/

/*** Interrupt ***/
/* Disable/Enable */
#define DRIVER_INTERRUPT_DISABLE() __disable_interrupt()
#define DRIVER_INTERRUPT_ENABLE()  __enable_interrupt()

/*** Low-Frequency Crystal Oscillator (LFXT) ***/
/* GPIO */
#define DRIVER_LFXT_GPIO_FUNCTION (GPIO_PRIMARY_MODULE_FUNCTION)
#define DRIVER_LFXT_GPIO_IN_PORT  (GPIO_PORT_PJ)
#define DRIVER_LFXT_GPIO_IN_PIN   (GPIO_PIN4)
#define DRIVER_LFXT_GPIO_OUT_PORT (GPIO_PORT_PJ)
#define DRIVER_LFXT_GPIO_OUT_PIN  (GPIO_PIN5)

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

#ifdef CEEDLING
  extern void __disable_interrupt(void);
  extern void __enable_interrupt(void);
#endif
