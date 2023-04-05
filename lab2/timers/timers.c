// A.1
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"

// A.2
#include "driverlib/rom.h"

// B
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

// C
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Timer (timers)</h1>
//!
//! This example application demonstrates the use of the timers to generate
//! periodic interrupts.  One timer is set up to interrupt once per second and
//! the other to interrupt twice per second; each interrupt handler will toggle
//! its own indicator on the display.
//
//*****************************************************************************

//*****************************************************************************
//
// Flags that contain the current value of the interrupt indicator as displayed
// on the CSTN display.
//
//*****************************************************************************
uint32_t g_ui32Flags;

//*****************************************************************************
//
// Graphics context used to show text on the CSTN display.
//
//*****************************************************************************
tContext g_sContext;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//*****************************************************************************
//
// The interrupt handler for the first timer interrupt.
//
//*****************************************************************************
void Timer0IntHandler(void) {
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, ~GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_0));
}

//*****************************************************************************
//
// The interrupt handler for the second timer interrupt.
//
//*****************************************************************************
void Timer1IntHandler(void) {
}

//*****************************************************************************
//
// This example application demonstrates the use of the timers to generate
// periodic interrupts.
//
//*****************************************************************************
int main(void) {
	// config device clock
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

	//// Pin N? config

	// enable Pin N? to read some voltages
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

	// config first 4 PN?* as inputs
	GPIOPinTypeGPIOInput(GPIO_PORTN_BASE, 0xF);

	// connect first 4 PN* to internal pull-down resistors and set 2 mA as current strength.
	// so we want our disconnected pins to default to 0s
	GPIOPadConfigSet(GPIO_PORTN_BASE, 0xFF, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

	//// Interrupt config?

	// allow processor to respond to interrupts
	IntMasterEnable();

	// Enable Timer0?
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

	// Config Timer0 to be full-width period countdown?
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

	// load cycle count to TimerA (b/c fullwidth, B for halfwidth)
	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / 2);

	// enable interrupt?
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	// enable timer
	TimerEnable(TIMER0_BASE, TIMER_A);

	//uint32_t clockRate = SysCtlClockGet();
	//int hertz = 2;
    while(1) {
		//SysCtlDelay(clockRate / 3 / hertz);
		//ROM_SysCtlDelay(clockRate / 3 / hertz);
    }
}
