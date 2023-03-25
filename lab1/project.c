#include <stdbool.h>
#include <stdint.h>
#include "driverlib/sysctl.h"

#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"

int main(void) {
	// clock
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

	// LED
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);

	// See Page26 of the guide to confirm where Pin N is

	// enable Pin N to read some voltages
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

	// config PN* as inputs
	GPIOPinTypeGPIOInput(GPIO_PORTN_BASE, 0xFF);

	// connect PN* to internal pull-down resistors and set 2 mA as current strength.
	// NB: we don't use pull-ups like last time, b/c our "strong voltage" is high now,
	// so we want our disconnected pins to default to 0s
	GPIOPadConfigSet(GPIO_PORTN_BASE, 0xFF, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

	double onRatio = 0.5;
	uint32_t clockRate = SysCtlClockGet();
	double baseHertz = 1 / 16; // frequency of the end pin, others get multiplied

	while(1) {
		// multiply base frequency by value of pin N (which will be a power of 2)
		// you have to modify this if frequency is decreasing with higher pins instead
		double hertz = baseHertz * GPIOPinRead(GPIO_PORTN_BASE, 0xFF);

		// cheap hack to make sure we don't accidentally divide by zero
		hertz = hertz ? hertz : 1;

		GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0x4);
		SysCtlDelay(clockRate / 3 / hertz * onRatio);
		GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0x0);
		SysCtlDelay(clockRate / 3 / hertz * (1 - onRatio));
	}
}

