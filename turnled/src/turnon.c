/*
 * turnon.c
 *
 *  Created on: May 17, 2020
 *      Author: alin
 */

#include "xparameters.h"
#include "xgpio_l.h"
#include "xil_printf.h"
#define LED 0x01
#define GPIO_REG_BASEADDR	XPAR_GPIO_1_BASEADDR
#define LED_DELAY	 1000
//1000000
//#define led_tri_o	1
#define led 1
int main()
{
	u32 Data;
	volatile int Delay;

	/*
	 * Set the direction for all signals to be inputs except the LED output
	 */
	XGpio_WriteReg((GPIO_REG_BASEADDR),
			((led - 1) * XGPIO_CHAN_OFFSET) +
			XGPIO_TRI_OFFSET, (~LED));


	/* Loop forever blinking the LED */

	while (1) {
		/*
		 * Read the state of the data so that only the LED state can be
		 * modified
		 */
		Data = XGpio_ReadReg(GPIO_REG_BASEADDR,
				((led - 1) * XGPIO_CHAN_OFFSET)+
				  XGPIO_DATA_OFFSET);


		/* Set the LED to the opposite state such that it blinks */

		if (Data & LED) {

			XGpio_WriteReg((GPIO_REG_BASEADDR),
				((led- 1) * XGPIO_CHAN_OFFSET) +
				XGPIO_DATA_OFFSET, Data & ~LED);




		} else {

			XGpio_WriteReg((GPIO_REG_BASEADDR),
				((led - 1) * XGPIO_CHAN_OFFSET) +
				XGPIO_DATA_OFFSET, Data | LED);


		}

		/* Wait a small amount of time so that the LED is visible */

		for (Delay = 0; Delay < LED_DELAY; Delay++);
	}
	XGpio_WriteReg((GPIO_REG_BASEADDR),
					((led+2 - 1) * XGPIO_CHAN_OFFSET) +
					XGPIO_TRI_OFFSET, (~LED));
	while (1) {
			/*
			 * Read the state of the data so that only the LED state can be
			 * modified
			 */
			Data = XGpio_ReadReg(GPIO_REG_BASEADDR,
					((led+2 - 1) * XGPIO_CHAN_OFFSET)+
					  XGPIO_DATA_OFFSET);


			/* Set the LED to the opposite state such that it blinks */

			if (Data & LED) {

				XGpio_WriteReg((GPIO_REG_BASEADDR),
					((led+2- 1) * XGPIO_CHAN_OFFSET) +
					XGPIO_DATA_OFFSET, Data & ~LED);




			} else {

				XGpio_WriteReg((GPIO_REG_BASEADDR),
					((led+2 - 1) * XGPIO_CHAN_OFFSET) +
					XGPIO_DATA_OFFSET, Data | LED);


			}

			/* Wait a small amount of time so that the LED is visible */

			for (Delay = 0; Delay < LED_DELAY; Delay++);
		}
	xil_printf("Successfully ran Gpio lowlevel Example\r\n");
	return 0;
}
