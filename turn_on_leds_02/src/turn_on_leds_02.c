
#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"


#define LED 0x05   /* Assumes bits 0 and 2 of GPIO are connected to LEDs  */

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define GPIO_LEDS_DEVICE_ID  XPAR_GPIO_1_DEVICE_ID

/*
 * The following constant is used to determine which channel of the GPIO is
 * used for the LED if there are 2 channels supported.
 */
#define LED_CHANNEL 1


XGpio Gpio; /* The Instance of the GPIO Driver */


int main(void)
{
	int Status;

	/* Initialize the GPIO driver */
	Status = XGpio_Initialize(&Gpio, GPIO_LEDS_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}

	/* Set the LED to High */
	XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, LED);

	while (1) {}

	xil_printf("Successfully turned on LEDs 0 and 2\r\n");
	return XST_SUCCESS;
}
