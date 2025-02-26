/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "msp430fr6989.h"
#include "serial.h"
#include <stdint.h>

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

static inline uint16_t interrupt_getWakeMask(const interrupt_wake_t Wake);

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** USCI A1 Vector Handler ***/
#pragma vector=USCI_A1_VECTOR
__interrupt void interrupt_usciA1VectorHandler(void)
{
    uint16_t mask;
    interrupt_wake_t wake;

    /*** USCI A1 Vector Handler ***/
    wake = serial_interruptHandler(__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG));
    if((mask = interrupt_getWakeMask(wake)) > 0)
        __bic_SR_register_on_exit(mask);
}

/****************************************************************************************************
 * Function Definition (Private)
 ****************************************************************************************************/

/*** Get Wake Mask ***/
static inline uint16_t interrupt_getWakeMask(const interrupt_wake_t Wake)
{
    uint16_t mask;

    /*** Get Wake Mask ***/
    switch(Wake)
    {
        case INTERRUPT_WAKE_LPM_0:
            /* Low Power Mode 0 */
            mask = LPM0_bits;
            break;
        case INTERRUPT_WAKE_LPM_1:
            /* Low Power Mode 1 */
            mask = LPM1_bits;
            break;
        case INTERRUPT_WAKE_LPM_2:
            /* Low Power Mode 2 */
            mask = LPM2_bits;
            break;
        case INTERRUPT_WAKE_LPM_3:
            /* Low Power Mode 3 */
            mask = LPM3_bits;
            break;
        case INTERRUPT_WAKE_LPM_4:
            /* Low Power Mode 4 */
            mask = LPM4_bits;
            break;
        case INTERRUPT_WAKE_LPM_NONE:
        default:
            /* Do Nothing */
            mask = 0;
            break;
    }

    return mask;
}
