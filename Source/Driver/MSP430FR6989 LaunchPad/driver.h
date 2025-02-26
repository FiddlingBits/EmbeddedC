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
/* General-Purpose Input/Output (GPIO) */
#define DRIVER_LFXT_GPIO_FUNCTION (GPIO_PRIMARY_MODULE_FUNCTION) // LFXIN, LFXOUT
#define DRIVER_LFXT_GPIO_IN_PORT  (GPIO_PORT_PJ)
#define DRIVER_LFXT_GPIO_IN_PIN   (GPIO_PIN4)
#define DRIVER_LFXT_GPIO_OUT_PORT (GPIO_PORT_PJ)
#define DRIVER_LFXT_GPIO_OUT_PIN  (GPIO_PIN5)

/*** Serial ***/
/* Base Address */
#define DRIVER_SERIAL_UART_BASE_ADDRESS (EUSCI_A1_BASE)

/* Direct Memory Access (DMA) */
#define DRIVER_SERIAL_UART_DMA_CHANNEL        (DMA_CHANNEL_0)
#define DRIVER_SERIAL_UART_DMA_TRIGGER_SOURCE (DMA_TRIGGERSOURCE_17) // UCA1TXIFG

/* General-Purpose Input/Output (GPIO) */
#define DRIVER_SERIAL_UART_GPIO_FUNCTION (GPIO_PRIMARY_MODULE_FUNCTION) // UCA1RXD, UCA1TXD
#define DRIVER_SERIAL_UART_GPIO_RX_PORT  (GPIO_PORT_P3)
#define DRIVER_SERIAL_UART_GPIO_RX_PIN   (GPIO_PIN5)
#define DRIVER_SERIAL_UART_GPIO_TX_PORT  (GPIO_PORT_P3)
#define DRIVER_SERIAL_UART_GPIO_TX_PIN   (GPIO_PIN4)

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

#ifdef CEEDLING
  extern void __disable_interrupt(void);
  extern void __enable_interrupt(void);
#endif
