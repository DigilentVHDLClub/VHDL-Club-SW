
#include "xparameters.h"
#include "xgpio_l.h"
#include "xil_printf.h"

#define LED 0x05 /* Assumes bits 0 and 2 of GPIO are connected to LEDs */

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define GPIO_REG_BASEADDR	XPAR_GPIO_1_BASEADDR

/*
 * The following constant is used to determine which channel of the GPIO is
 * used for the LED if there are 2 channels supported.
 */
#define LED_CHANNEL	1


int main()
{
	u32 Data;


	/* Turn on LEDs 0 and 2 */

	/*
	 * Read the state of the data so that only the LED state can be
	 * modified
	 */
	Data = XGpio_ReadReg(GPIO_REG_BASEADDR,
			((LED_CHANNEL - 1) * XGPIO_CHAN_OFFSET) +
			  XGPIO_DATA_OFFSET);

	XGpio_WriteReg((GPIO_REG_BASEADDR),
		((LED_CHANNEL - 1) * XGPIO_CHAN_OFFSET) +
		XGPIO_DATA_OFFSET, Data | LED);
	xil_printf("Successfully turned on LEDs 0 and 2.\r\n");

	while (1) {}

	return 0;
}
