/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "driver.h"
#include "mock_dma.h"
#include "mock_eusci_a_uart.h"
#include "mock_gpio.h"
#include "serial.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "unity.h"

/****************************************************************************************************
 * Variable
 ****************************************************************************************************/

extern char serial_txString[SERIAL_TX_STRING_MAX_LENGTH];
extern size_t serial_txStringIndex;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

extern void serial_transmit(const uint8_t * const Data, const size_t DataLength);

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
 * Helper
 ****************************************************************************************************/

/*** Transmit ***/
static void mock_transmit(const uint8_t * const Data, const size_t DataLength)
{
    /*** Transmit ***/
    /* Constant */
    const uint32_t BufferAddress = 0xDEADBEEF;

    /* Variable */
    DMA_initParam expectedDmaInit;

    /* Set Up */
    (void)memset(&expectedDmaInit, 0, sizeof(expectedDmaInit));
    expectedDmaInit.channelSelect = DRIVER_SERIAL_UART_DMA_CHANNEL;
    expectedDmaInit.transferModeSelect = DMA_TRANSFER_BLOCK;
    expectedDmaInit.transferSize = (uint16_t)DataLength;
    expectedDmaInit.triggerSourceSelect = DRIVER_SERIAL_UART_DMA_TRIGGER_SOURCE;
    expectedDmaInit.transferUnitSelect = DMA_SIZE_SRCBYTE_DSTBYTE;
    expectedDmaInit.triggerTypeSelect = DMA_TRIGGER_HIGH;

    /* Mock */
    DMA_init_Expect(&expectedDmaInit);
    DMA_setSrcAddress_Expect(DRIVER_SERIAL_UART_DMA_CHANNEL, (uintptr_t)Data, DMA_DIRECTION_INCREMENT);
    EUSCI_A_UART_getTransmitBufferAddress_ExpectAndReturn(DRIVER_SERIAL_UART_BASE_ADDRESS, BufferAddress);
    DMA_setDstAddress_Expect(DRIVER_SERIAL_UART_DMA_CHANNEL, BufferAddress, DMA_DIRECTION_UNCHANGED);
    DMA_enableTransfers_Expect(DRIVER_SERIAL_UART_DMA_CHANNEL);
    DMA_startTransfer_Expect(DRIVER_SERIAL_UART_DMA_CHANNEL);
}

/****************************************************************************************************
 * Test (Public)
 ****************************************************************************************************/

/*** Initialize ***/
void test_init(void)
{
    /*** Initialize ***/
    /* Variable */
    EUSCI_A_UART_initParam expectedUartInit;

    /* Set Up */
    (void)memset(&expectedUartInit, 0, sizeof(expectedUartInit));
    expectedUartInit.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    expectedUartInit.clockPrescalar = 4;
    expectedUartInit.firstModReg = 5;
    expectedUartInit.secondModReg = 85;
    expectedUartInit.parity = EUSCI_A_UART_NO_PARITY;
    expectedUartInit.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    expectedUartInit.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    expectedUartInit.uartMode = EUSCI_A_UART_MODE;
    expectedUartInit.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

    /* Mock */
    DMA_disableTransfers_Expect(DRIVER_SERIAL_UART_DMA_CHANNEL);
    GPIO_setAsPeripheralModuleFunctionInputPin_Expect(DRIVER_SERIAL_UART_GPIO_RX_PORT, DRIVER_SERIAL_UART_GPIO_RX_PIN, DRIVER_SERIAL_UART_GPIO_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin_Expect(DRIVER_SERIAL_UART_GPIO_TX_PORT, DRIVER_SERIAL_UART_GPIO_TX_PIN, DRIVER_SERIAL_UART_GPIO_FUNCTION);
    EUSCI_A_UART_init_ExpectAndReturn(DRIVER_SERIAL_UART_BASE_ADDRESS, &expectedUartInit, STATUS_SUCCESS);
    EUSCI_A_UART_enable_Expect(DRIVER_SERIAL_UART_BASE_ADDRESS);
    EUSCI_A_UART_clearInterrupt_Expect(DRIVER_SERIAL_UART_BASE_ADDRESS, EUSCI_A_UART_RECEIVE_INTERRUPT);
    EUSCI_A_UART_enableInterrupt_Expect(DRIVER_SERIAL_UART_BASE_ADDRESS, EUSCI_A_UART_RECEIVE_INTERRUPT);

    /* Initialize */
    serial_init();

    /* Verify */
    TEST_ASSERT_EQUAL_CHAR('\0', serial_txString[0]);
    TEST_ASSERT_EQUAL_UINT32(0, serial_txStringIndex);
}

/*** Interrupt Handler ***/
void test_interruptHandler(void)
{
    /*** Interrupt Handler ***/
    /* Constant */
    const uint16_t Flag[] = {USCI_NONE, USCI_UART_UCRXIFG, USCI_UART_UCTXIFG, USCI_UART_UCSTTIFG, USCI_UART_UCTXCPTIFG, 0xFFFF};
    const size_t FlagCount = sizeof(Flag) / sizeof(Flag[0]);

    /* Variable */
    size_t i;
    interrupt_wake_t wake;

    /* Interrupt Handler */
    for(i = 0; i < FlagCount; i++)
    {
        /* Interrupt Handler */
        wake = serial_interruptHandler(Flag[i]);

        /* Verify */
        TEST_ASSERT_EQUAL_INT(INTERRUPT_WAKE_LPM_NONE, wake);
    }
}

/*** Print Formatted String ***/
void test_printf_1(void)
{
    /*** Print Formatted String ***/
    /* Variable */
    size_t txStringIndex;

    /* Set Up */
    serial_txString[0] = '\0';
    serial_txStringIndex = 0;

    /* Print Formatted String */
    txStringIndex = serial_printf(false, NULL);
    txStringIndex = serial_printf(false, "");
    txStringIndex = serial_printf(true, "");

    /* Verify */
    TEST_ASSERT_EQUAL_STRING("", serial_txString);
    TEST_ASSERT_EQUAL_UINT32(0, txStringIndex);
}

void test_printf_2(void)
{
    /*** Print Formatted String ***/
    /* Constant */
    const char * const ExpectedString = "Test\r\n";

    /* Variable */
    size_t txStringIndex;

    /* Set Up */
    serial_txString[0] = '\0';
    serial_txStringIndex = 0;

    /* Print Formatted String */
    txStringIndex = serial_printf(false, "%s\r\n", "Test");

    /* Verify */
    TEST_ASSERT_EQUAL_STRING(ExpectedString, serial_txString);
    TEST_ASSERT_EQUAL_UINT32(strlen(serial_txString), txStringIndex);
}

void test_printf_3(void)
{
    /*** Print Formatted String ***/
    /* Constant */
    const char * const ExpectedString = "Test";

    /* Variable */
    size_t txStringIndex;

    /* Set Up */
    serial_txString[0] = '\0';
    serial_txStringIndex = 0;

    /* Mock */
    mock_transmit((uint8_t *)serial_txString, strlen(ExpectedString));

    /* Print Formatted String */
    txStringIndex = serial_printf(true, "%s", "Test");

    /* Verify */
    TEST_ASSERT_EQUAL_STRING(ExpectedString, serial_txString);
    TEST_ASSERT_EQUAL_UINT32(0, txStringIndex);
}

/****************************************************************************************************
 * Test (Private)
 ****************************************************************************************************/

/*** Transmit ***/
void test_transmit_1(void)
{
    /*** Transmit ***/
    /* Variable */
    char data;

    /* Transmit */
    serial_transmit(NULL, sizeof(data));
    serial_transmit(&data, 0);
}

void test_transmit_2(void)
{
    /*** Transmit ***/
    /* Variable */
    char data;

    /* Mock */
    mock_transmit(&data, sizeof(data));

    /* Transmit */
    serial_transmit(&data, sizeof(data));
}
