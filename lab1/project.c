#include <stdbool.h>
#include <stdint.h>
#include "driverlib/sysctl.h"

#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"

int main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

	// See Page26 of the guide for how the LED is connected to PG2

	// enable Port G
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);

	// config Pin2 of Port G as an output
	GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);

	while(1) {
		// Write 1 to Pin2 of Port G
		// NB1: the second argument is treated as a bitmask to figure out which pin(s) to set
		// NB2: GPIO_PIN_2 is also literally just a macro for 0x4, because that gives the (2+1)th least significant bit as 1
		//
		// ie: we could also literally write 0xF instead of 0x4 as the third argument
		// but, we couldn't do eg: 0x1 or 0xA
		GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0x4);

		// Switch off by writing 0x0
		//GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0x0);
	}
}

