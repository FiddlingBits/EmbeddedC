/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "dma.h"
#include "driver.h"
#include "eusci_a_uart.h"
#include "gpio.h"
#include "interrupt.h"
#include "msp430fr6989.h"
#include "project.h"
#include "serial.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/****************************************************************************************************
 * Variable
 ****************************************************************************************************/

PROJECT_STATIC(char serial_txString[SERIAL_TX_STRING_MAX_LENGTH]);
PROJECT_STATIC(size_t serial_txStringIndex);

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

PROJECT_STATIC(void serial_transmit(const uint8_t * const Data, const size_t DataLength));

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Initialize ***/
void serial_init(void)
{
    EUSCI_A_UART_initParam uartInit;

    /*** Initialize ***/
    /* Deinitialize (May Have Dirty Settings After Reset) */
    DMA_disableTransfers(DRIVER_SERIAL_UART_DMA_CHANNEL);

    /* GPIO */
    GPIO_setAsPeripheralModuleFunctionInputPin(DRIVER_SERIAL_UART_GPIO_RX_PORT, DRIVER_SERIAL_UART_GPIO_RX_PIN, DRIVER_SERIAL_UART_GPIO_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(DRIVER_SERIAL_UART_GPIO_TX_PORT, DRIVER_SERIAL_UART_GPIO_TX_PIN, DRIVER_SERIAL_UART_GPIO_FUNCTION);

    /* UART: 115200 8-N-1; See: http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html And MSP430FR6xx User's Guide: 30.3.10 Setting A Baud Rate */
    memset(&uartInit, 0, sizeof(uartInit));
    uartInit.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    uartInit.clockPrescalar = 4;
    uartInit.firstModReg = 5;
    uartInit.secondModReg = 85;
    uartInit.parity = EUSCI_A_UART_NO_PARITY;
    uartInit.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    uartInit.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    uartInit.uartMode = EUSCI_A_UART_MODE;
    uartInit.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;
    (void)EUSCI_A_UART_init(DRIVER_SERIAL_UART_BASE_ADDRESS, &uartInit);
    EUSCI_A_UART_enable(DRIVER_SERIAL_UART_BASE_ADDRESS);
#ifndef CEEDLING
    HWREG16(DRIVER_SERIAL_UART_BASE_ADDRESS + OFS_UCAxCTLW0) &= ~UC7BIT; // 8-Bit Character Length
#endif

    /* UART Interrupt */
    EUSCI_A_UART_clearInterrupt(DRIVER_SERIAL_UART_BASE_ADDRESS, EUSCI_A_UART_RECEIVE_INTERRUPT);
    EUSCI_A_UART_enableInterrupt(DRIVER_SERIAL_UART_BASE_ADDRESS, EUSCI_A_UART_RECEIVE_INTERRUPT);

    /* Variables */
    serial_txString[0] = '\0';
    serial_txStringIndex = 0;
}

/*** Interrupt Handler ***/
interrupt_wake_t serial_interruptHandler(uint16_t InterruptFlag)
{
    /*** Handle Interrupt */
    switch(InterruptFlag)
    {
        case USCI_NONE:
            /*** No Interrupt ***/
            break;
        case USCI_UART_UCRXIFG:
            /*** Receive Interrupt ***/
            break;
        case USCI_UART_UCTXIFG:
            /*** Transmit Interrupt ***/
            break;
        case USCI_UART_UCSTTIFG:
            /*** Start Bit Interrupt ***/
            break;
        case USCI_UART_UCTXCPTIFG:
            /*** Transmit Complete Interrupt ***/
            break;
        default:
            /*** Do Nothing ***/
            break;
    }

    return INTERRUPT_WAKE_LPM_NONE;
}

/*** Print Formatted String ***/
size_t serial_printf(const bool Flush, const char * const Format, ...)
{
    va_list arguments;

    /*** Print Formatted String ***/
    /* Error Check */
    if(Format == NULL)
        return 0;

    /* Wait Until Ready */
    serial_waitUntilReady();

    /* Decode Format String And Optional Arguments And Write To Print String */
    va_start(arguments, Format);
    serial_txStringIndex += (size_t)vsnprintf(&serial_txString[serial_txStringIndex], sizeof(serial_txString) - serial_txStringIndex, Format, arguments);
    va_end(arguments);

    /* Transmit */
    if(Flush && (strlen(serial_txString) > 0))
    {
        serial_transmit((uint8_t *)serial_txString, serial_txStringIndex);
        serial_txStringIndex = 0;
    }

    return serial_txStringIndex;
}

/*** Wait Until Ready ***/
void serial_waitUntilReady(void)
{
    /*** Wait Until Ready: DMA Disabled (Automatically Happens On Completion Of Block Transfer) And UART NOT Busy ***/
#ifndef CEEDLING
    while(((HWREG16(DMA_BASE + DRIVER_SERIAL_UART_DMA_CHANNEL + OFS_DMA0CTL) & DMAEN) == DMAEN) || ((HWREG16(DRIVER_SERIAL_UART_BASE_ADDRESS + OFS_UCAxSTATW) & UCBUSY) == UCBUSY))
        continue;
#endif
}

/****************************************************************************************************
 * Function Definition (Private)
 ****************************************************************************************************/

/*** Transmit ***/
PROJECT_STATIC(void serial_transmit(const uint8_t * const Data, const size_t DataLength))
{
    DMA_initParam dmaInit;

    /*** Transmit ***/
    /* Error Check */
    if((Data == NULL) || (DataLength == 0))
        return;

    /* Initialize DMA */
    (void)memset(&dmaInit, 0, sizeof(dmaInit));
    dmaInit.channelSelect = DRIVER_SERIAL_UART_DMA_CHANNEL;
    dmaInit.transferModeSelect = DMA_TRANSFER_BLOCK;
    dmaInit.transferSize = (uint16_t)DataLength;
    dmaInit.triggerSourceSelect = DRIVER_SERIAL_UART_DMA_TRIGGER_SOURCE;
    dmaInit.transferUnitSelect = DMA_SIZE_SRCBYTE_DSTBYTE;
    dmaInit.triggerTypeSelect = DMA_TRIGGER_HIGH;
    DMA_init(&dmaInit);

    /* Set DMA Addresses And Start Transfer */
    DMA_setSrcAddress(DRIVER_SERIAL_UART_DMA_CHANNEL, (uintptr_t)Data, DMA_DIRECTION_INCREMENT);
    DMA_setDstAddress(DRIVER_SERIAL_UART_DMA_CHANNEL, EUSCI_A_UART_getTransmitBufferAddress(DRIVER_SERIAL_UART_BASE_ADDRESS), DMA_DIRECTION_UNCHANGED);
    DMA_enableTransfers(DRIVER_SERIAL_UART_DMA_CHANNEL);
    DMA_startTransfer(DRIVER_SERIAL_UART_DMA_CHANNEL); // DMA Is Disabled After Completion Of Block Transfer
}
