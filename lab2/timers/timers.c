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

#ifdef DEBUG
	void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

void Timer0IntHandler(void) {
	// clear interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	// toggle LED
	GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_1, ~GPIOPinRead(GPIO_PORTL_BASE, GPIO_PIN_1));
}

float count = 3; // global??
void Timer1IntHandler(void) {
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

	count = count ? count - 1 : 3;
	float dutyFraction = (3 - count) / 7;
	int PWMperiod = 250;
	int period = PWMperiod * dutyFraction;

	// cheap hack for no 0
	period = period ? period : 1;
	period = 1;
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, period);
}

int main(void) {
	//// pins
	// config device clock
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_OSC | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN | SYSCTL_SYSDIV_16);
	uint32_t clockRate = 1000000; //SysCtlClockGet();
	int hertz = 2;
	//int dutyRatio = 5; // 1/5 of period?

	// enable Pin N and L for our LEDs
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);

	// Enable PWM
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

	// config all LEDs
	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_4 | GPIO_PIN_1);
	GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_4);

	// config N3 for PWM
	GPIOPinTypePWM(GPIO_PORTN_BASE, GPIO_PIN_3);
	GPIOPinConfigure(GPIO_PN3_M0PWM7);

	//// config PWM
	//SysCtlPWMClockSet(SYSCTL_PWMDIV_4);
	PWMClockSet(PWM0_BASE, SYSCTL_PWMDIV_4);

	PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

	uint32_t PWMperiod = 250 - 1;
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, PWMperiod);
	//PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, PWMperiod / dutyRatio); // duty cycle

	PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
	PWMGenEnable(PWM0_BASE, PWM_GEN_3);

	//// interrupts
	// Enable Timers?
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

	// Config Timers to be full-width period countdown
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

	// load cycle counts to TimerA (b/c fullwidth, B for halfwidth)
	TimerLoadSet(TIMER0_BASE, TIMER_A, clockRate / 2);
	TimerLoadSet(TIMER1_BASE, TIMER_A, clockRate);

	// enable interrupts
	IntEnable(INT_TIMER0A);
	IntEnable(INT_TIMER1A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

	// enable timers
	TimerEnable(TIMER0_BASE, TIMER_A);
	TimerEnable(TIMER1_BASE, TIMER_A);

	// allow processor to respond to interrupts
	IntMasterEnable();

	while(1) {
		GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_4, ~GPIOPinRead(GPIO_PORTL_BASE, GPIO_PIN_4));
		//SysCtlDelay(clockRate / 3 / hertz);
		ROM_SysCtlDelay(clockRate / 3 / hertz);
	}
}

