#include <stdint.h> // Integers
#include "clock.h" // 40 MHz clock
#include "tm4c123gh6pm.h" // Tiva header file

#define SOLENOID (*(volatile uint32_t *)(0x42000000 + ((0x40004000 - 0x40000000 + 0x3FC) * 32 + (4*2)))) // PA2
#define TOPSWITCH (*(volatile uint32_t *)(0x42000000 + ((0x40004000 - 0x40000000 + 0x3FC) * 32 + (4*3)))) // PA3
#define BOTTOMSWITCH (*(volatile uint32_t *)(0x42000000 + ((0x40004000 - 0x40000000 + 0x3FC) * 32 + (4*4)))) // PA4

/**
 * main.c
 */
int main(void)
{
	initSystemClockTo40Mhz(); // Set system clock to 40MHz
	_delay_cycles(3); // Delay for load-up

	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0; // Enable clock to Port A
	_delay_cycles(3); // Delay for load-up
	// 00000001
	// 00011100
	GPIO_PORTA_DIR_R |= 0x04; // Set PA2 (Solenoid) for output
	GPIO_PORTA_DIR_R &= ~0x18; // Set PA3/4 (Switches) for input
	GPIO_PORTA_DEN_R |= 0x1C; // Digitally enable PA2/3/4
	GPIO_PORTA_DR2R_R |= 0x04;
	_delay_cycles(3);

	SOLENOID = 1; // Initially power solenoid

	while (1)
	{
		while (TOPSWITCH == 1) // If top switch triggered
		{
			SOLENOID = 0; // Disable power
		}

		//_delay_cycles(4000);

		while (BOTTOMSWITCH == 1) // If bottom switch triggered
		{
			SOLENOID = 1; // Disable power
		}
	}
}
