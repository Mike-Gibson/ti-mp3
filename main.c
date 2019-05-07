

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "drivers/buttons.h"

volatile uint32_t ui32Buttons;


//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
void
UARTIntHandler(void)
{
    uint32_t ui32Status;

    //
    // Get the interrupt status.
    //
    ui32Status = ROM_UARTIntStatus(UART0_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    ROM_UARTIntClear(UART0_BASE, ui32Status);

    //
    // Loop while there are characters in the receive FIFO.
    //
    while(ROM_UARTCharsAvail(UART0_BASE))
    {
        //
        // Read the next character from the UART.
        //
        uint8_t a = ROM_UARTCharGetNonBlocking(UART0_BASE);

        switch(a) {

            case 'i'  :
                UARTSend((uint8_t *)"\r\nInitialising...", 17);

                // http://www.electronicoscaldas.com/datasheet/DFR0299-DFPlayer-Mini-Manual.pdf
                // $S VER Len CMD Feedback para1 para2 checksum $O
                // 7E FF  06  09  00       00    02    FF DD    EF
                UARTSend_1(0x7E);
                UARTSend_1(0xFF);
                UARTSend_1(0x06);
                UARTSend_1(0x09);
                UARTSend_1(0x01); // TEMP - get feedback?
                UARTSend_1(0x00);
                UARTSend_1(0x02);
                // 0 - VER - Len - CMD - Feedback - para1 - para2
//                UARTSend_1(0xFE);  // NOT NEEDED?
//                UARTSend_1(0xFF);  // NOT NEEDED?
                UARTSend_1(0xEF);

                break;

            case 'p'  :
                UARTSend((uint8_t *)"\r\nPlaying...", 12);

//                // http://www.electronicoscaldas.com/datasheet/DFR0299-DFPlayer-Mini-Manual.pdf
//                // $S VER Len CMD Feedback para1 para2 checksum $O
//                // 7E FF  06  0D  00       00    00    ?? ??    EF
                UARTSend_1(0x7E);
                UARTSend_1(0xFF);
                UARTSend_1(0x06);
                UARTSend_1(0x0D);
                UARTSend_1(0x01); // TEMP - get feedback?
                UARTSend_1(0x00);
                UARTSend_1(0x00);
                // UARTSend_1(0x????, 2); NOT NEEDED?
                UARTSend_1(0xEF);

                // Play 01/001.mp3
                //
                // http://www.electronicoscaldas.com/datasheet/DFR0299-DFPlayer-Mini-Manual.pdf
                // $S VER Len CMD Feedback para1 para2 checksum $O
                // 7E FF  06  0F  00       01    01    xx xx    EF
//                UARTSend_1(0x7E);
//                UARTSend_1(0xFF);
//                UARTSend_1(0x06);
//                UARTSend_1(0x0F);
//                UARTSend_1(0x01); // TEMP - get feedback?
//                UARTSend_1(0x01);
//                UARTSend_1(0x01);
//                UARTSend_1(0xFE);  // NOT NEEDED?
//                UARTSend_1(0xE9);  // NOT NEEDED?
//                UARTSend_1(0xEF);

                break;

            case 'n'  :
               UARTSend((uint8_t *)"\r\nNext...", 9);

               UARTSend_1(0x7E);
               UARTSend_1(0xFF);
               UARTSend_1(0x06);
               UARTSend_1(0x01);
               UARTSend_1(0x01); // TEMP - get feedback?
               UARTSend_1(0x00);
               UARTSend_1(0x00);
               UARTSend_1(0xEF);

               break;

            case 's'  :
               UARTSend((uint8_t *)"\r\nStopping...", 13);

               // http://www.electronicoscaldas.com/datasheet/DFR0299-DFPlayer-Mini-Manual.pdf
               // $S VER Len CMD Feedback para1 para2 checksum $O
               // 7E FF  06  0E  00       00    00    xx xx    EF
               UARTSend_1(0x7E);
               UARTSend_1(0xFF);
               UARTSend_1(0x06);
               UARTSend_1(0x0E);
               UARTSend_1(0x01); // TEMP - get feedback?
               UARTSend_1(0x00);
               UARTSend_1(0x00);
               UARTSend_1(0xFE);  // NOT NEEDED?
               UARTSend_1(0xEC);  // NOT NEEDED?
               UARTSend_1(0xEF);

               break;

            case 'v'  :
               UARTSend((uint8_t *)"\r\nVolume...", 11);

               UARTSend_1(0x7E);
               UARTSend_1(0xFF);
               UARTSend_1(0x06);
               UARTSend_1(0x06);
               UARTSend_1(0x01); // TEMP - get feedback?
               UARTSend_1(0x00);
               UARTSend_1(0x1E);
               UARTSend_1(0xEF);

               break;

            case 'q'  :
               UARTSend((uint8_t *)"\r\nQuery...", 10);

               UARTSend_1(0x7E);
               UARTSend_1(0xFF);
               UARTSend_1(0x06);
               UARTSend_1(0x47);
               UARTSend_1(0x00); // TEMP - get feedback?
               UARTSend_1(0x00);
               UARTSend_1(0x00);
               UARTSend_1(0xEF);

               break;

            case 'z'  :
               UARTSend((uint8_t *)"\r\nTESTING...", 12);

//               UARTSend_1(0x7E);
               UARTSend_1(0x05);
//               UARTSend_1(0xEF);

               break;

            default:
                UARTSend((uint8_t *)"\r\nUnrecognised command...", 25);
                break;
        }

        UARTSend((uint8_t *)"\r\nDone", 6);

        //
        // Blink the LED to show a character transfer is occuring.
        //
        //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

        //
        // Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.
        //
        // SysCtlDelay(SysCtlClockGet() / (1000 * 3));

        //
        // Turn off the LED
        //
        // GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);

    }
}


//*****************************************************************************
//
// The UART1 interrupt handler.
//
//*****************************************************************************
void
UART1IntHandler(void)
{
    uint32_t ui32Status;

    //
    // Get the interrupt status.
    //
    ui32Status = ROM_UARTIntStatus(UART1_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    ROM_UARTIntClear(UART1_BASE, ui32Status);

    bool haveWrittenHeader = false;

    while(ROM_UARTCharsAvail(UART1_BASE))
    {
        if (!haveWrittenHeader) {
            UARTSend((uint8_t *)"\r\nReceived: ", 12);
            haveWrittenHeader = true;
        }

        //
        // Read the next character from the UART.
        //
        uint8_t a = ROM_UARTCharGetNonBlocking(UART1_BASE);

        char asHexString[2];
        ConvertByteToHexString(a, asHexString);

        // Echo back to UART 0
        UARTSend(asHexString, 2);
        UARTSend((uint8_t *)" ", 1);
    }
}

void ConvertByteToHexString(uint8_t input, uint8_t *output) {
    output[0] = 0;
    output[1] = 1;
    int i;
    for (i = 0; i < 2; i++) {
        output[1 - i] = input % 16;
        input /= 16;

        if (input == 0)
            break;
    }

    for (i = 0; i < 2; i++) {
        if (output[i] < 10)
            output[i] += '0';
        else
            output[i] += 'A' - 10;
    }
}

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void
UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        ROM_UARTCharPut(UART0_BASE, *pui8Buffer++);
        // ROM_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++); -- this chops off chars if buffer gets full
    }
}


//*****************************************************************************
//
// Send a string to the UART1.
//
//*****************************************************************************
void
UARTSend_1(const uint8_t c)
{
    //
    // Loop while there are more characters to send.
    //
//    while(ui32Count--)
    {
        //
        // Write the character to the UART.
        //
        ROM_UARTCharPut(UART1_BASE, c);
        // ROM_UARTCharPutNonBlocking(UART1_BASE, *pui8Buffer++); -- this chops off chars if buffer gets full
    }
}

//*****************************************************************************
//
// Handler to manage the button press events and state machine transitions
// that result from those button events.
//
// This function is called by the SysTickIntHandler if a button event is
// detected. Function will determine which button was pressed and tweak various
// elements of the global state structure accordingly.
//
//*****************************************************************************
void
AppButtonHandler(void)
{
    static uint32_t ui32TickCounter;

    ui32TickCounter++;

    switch(ui32Buttons & ALL_BUTTONS)
    {

    case LEFT_BUTTON:
        //
        // Turn on the LED.
        //
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); // TODO: Why last arg?
        break;

    case RIGHT_BUTTON:
        //
        // Turn off the LED.
        //
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);
        break;

    case ALL_BUTTONS:

        break;

    default:

        break;
    }
}

//*****************************************************************************
//
// Called by the NVIC as a result of SysTick Timer rollover interrupt flag
//
// Checks buttons and calls AppButtonHandler to manage button events.
// Tracks time and auto mode color stepping.  Calls AppRainbow to implement
// RGB color changes.
//
//*****************************************************************************
void SysTickIntHandler(void)
{
    ui32Buttons = ButtonsPoll(0,0);
    AppButtonHandler();
}

int main(void)
{
    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();


    //
    // Set the clocking to run directly from the crystal.
    //
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);



    //
    // Enable the peripherals.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();

    //
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Set GPIO PB0 and PB1 as UART pins.
    //
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Configure the UART0 for 115,200, 8-N-1 operation.
    //
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    //
    // Configure the UART1 for 9600,  8-N-1 operation.
    //
    ROM_UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(), 9600,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));


    //
    // Enable the UART interrupt.
    //
    ROM_IntEnable(INT_UART0);
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    ROM_IntEnable(INT_UART1);
    ROM_UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);

    //
    // Prompt for text to be entered.
    //
    UARTSend((uint8_t *)"\033[2JEnter text: \r\n", 18);
    while(1)
        {
        }
    //
    // Initialize the buttons
    //
    //ButtonsInit();

    //
    // Enable the GPIO port that is used for the on-board LED.
    //
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Check if the peripheral access is enabled.
    //
//    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
//    {
//    }

    //
    // Enable the GPIO pin for the LED (PF3).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
//    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

    //
    // Initialize the SysTick interrupt
    //
//    SysTickPeriodSet(SysCtlClockGet() / 32);
//    SysTickEnable();
//    SysTickIntEnable();
//    IntMasterEnable();

    //
    // spin forever
    //
//    while(1)
//    }
//    {
}
