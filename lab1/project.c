#include <stdbool.h>
#include <stdint.h>
#include "driverlib/sysctl.h"

int main(void) {
	// set clock using the given library call
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

	while(1) { }
}

