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

	double hertz = 2;
	double onRatio = 0.5;
	uint32_t clockRate = SysCtlClockGet();

	while(1) {
		GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0x4);

		// SysCtlDelay runs a 3 instruction loop "count" times, count being the parameter
		// You can use this to blink the LED
		SysCtlDelay(clockRate / 3 / hertz * onRatio);

		GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0x0);
		SysCtlDelay(clockRate / 3 / hertz * (1 - onRatio));
	}
}

