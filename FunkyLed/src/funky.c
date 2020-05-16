/*
 * funky.c
 *
 *  Created on: May 16, 2020
 *      Author: Nita Eduard
 *
 *  Small program that lights up one LED at a time in a circular manner.
 *  The pattern is as follows: LD0->LD1->LD2->LD3->LD0 and so on.
 *  It can also go in the opposite direction,
 *  as long as any button from BTN{0, 1, 2, 3} is pushed.
 */

#include "platform.h"
#include "xparameters.h"
#include "xgpio_l.h"
#include "xil_printf.h"
#include <stdio.h>

#define GPIO_ONE_BASEADDR	XPAR_GPIO_1_BASEADDR
#define GPIO_ZERO_BASEADDR	XPAR_GPIO_0_BASEADDR

// delay until the power switches from one LED to another
#define LED_DELAY	 10000000


int main(){
	volatile int delay; // delay index
	char leds = 1; // although it is a byte, it is used as a nibble to keep track
				   // of which led is turned on. Possible values: 0001, 0010, 0100, 1000.
				   // The transition between these is achieved by using left/right shift operations.

	char sw = 0;   // flag used to signify if one of the buttons has been pressed

	init_platform();

	while (1) {

		sw = XGpio_ReadReg((GPIO_ZERO_BASEADDR), 0); // obtain value of buttons

		// checks if the button is pressed
	    if(!sw)
	    	leds = leds % 15; // if the button is not pressed, it will
	    					  // apply a modulo 15 operation on the LEDS value.
	    					  // Since the leds only change in this case
	    					  // by shifting left, the first value to be over 15
	    					  // is 16. As such, when it reaches this value,
	    					  // the leds value will be reset back to 1.
	    else
	    	leds = leds + !leds*8; // if the button is pressed, it will
	    						   // add the expression !led*8 to the current value of leds.
	    						   // If leds != 0, the value of !led will be 0, thus no change
	    						   // will occur. If leds = 0, then !leds will evaluate to 1.
	    						   // By multiplying it with 8, we make the transition to the next LED,
	    						   // i.e. from LD0(0b0001) -> LD3(0b1000).

		XGpio_WriteReg((GPIO_ONE_BASEADDR), 0, leds);
		for (delay = 0; delay < LED_DELAY; delay++);

		if(!sw)
			leds = leds << 1;
		else
			leds = leds >> 1;

	}
    print("bye");
    cleanup_platform();
    return 0;
}
