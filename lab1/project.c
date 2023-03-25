#include <stdbool.h>
#include <stdint.h>
#include "driverlib/sysctl.h"

#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"

int main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);

	// See Page26 of the guide for how Switch 3 is connected to PM2

	// enable Port M
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);

	// config PM2 as an input
	GPIOPinTypeGPIOInput(GPIO_PORTM_BASE, GPIO_PIN_2);

	// connect PM2 to internal pull-up resistors (ie: 1 is default) and set 2 mA as current strength.
	GPIOPadConfigSet(GPIO_PORTM_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	while(1) {
		// Read PM2 and write the inverse of that to PG2
		// NB1: Why inverse? because when we close the switch, it connects us straight to 0V (Page 26 of guide again)
		// (This is also why we configured it with a weak pull-up resistor, so that it's 1 when the switch is open)
		//
		// NB2: Reading PM2 gives us 0x0 or 0x4, both of which we can write directly to PG2
		// what if we configured and tried to read PM4 though? can we write the 0x10 that we would get?
		GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, ~GPIOPinRead(GPIO_PORTM_BASE, GPIO_PIN_2));
	}
}

