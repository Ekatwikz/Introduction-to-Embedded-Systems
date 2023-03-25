#include <stdbool.h>
#include <stdint.h>
#include "driverlib/sysctl.h"

#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"

int main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);

	// assuming we're trying PM4 now
	int buttonPin = GPIO_PIN_4;

	// gotta set this to the "distance" between the switch pin and LED pin
	int shiftAmount = 2;

	// enable Port M
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);

	// config PM? as an input
	GPIOPinTypeGPIOInput(GPIO_PORTM_BASE, buttonPin);

	// connect PM? to internal pull-up resistors (ie: 1 is default) and set 2 mA as current strength.
	GPIOPadConfigSet(GPIO_PORTM_BASE, buttonPin, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	while(1) {
		// gotta bit shift the value or the mask won't work!
		// if you try eg: PM0, you should shift in the other direction
		GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, ~GPIOPinRead(GPIO_PORTM_BASE, buttonPin) >> shiftAmount);
	}
}

