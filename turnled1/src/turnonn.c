/*
 * turnonn.c
 *
 *  Created on: May 18, 2020
 *      Author: alin
 */
#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"
#define LED 0x01
#define GPIO_EXAMPLE_DEVICE_ID  XPAR_GPIO_1_DEVICE_ID
#define LED_DELAY1     10000000
#define led_tri_o 1
#ifdef PRE_2_00A_APPLICATION
#define XGpio_SetDataDirection(InstancePtr, DirectionMask) \
        XGpio_SetDataDirection(InstancePtr, led_tri_o, DirectionMask)

#define XGpio_DiscreteRead(InstancePtr) \
        XGpio_DiscreteRead(InstancePtr, led_tri_o)

#define XGpio_DiscreteWrite(InstancePtr, Mask) \
        XGpio_DiscreteWrite(InstancePtr, led_tri_o, Mask)

#define XGpio_DiscreteSet(InstancePtr, Mask) \
        XGpio_DiscreteSet(InstancePtr, led_tri_o, Mask)

#endif
#ifdef PRE_2_00B_APPLICATION
#define XGpio_SetDataDirection(InstancePtr, DirectionMask) \
        XGpio_SetDataDirection(InstancePtr, led_tri_o+2, DirectionMask)

#define XGpio_DiscreteRead(InstancePtr) \
        XGpio_DiscreteRead(InstancePtr, led_tri_o+2)

#define XGpio_DiscreteWrite(InstancePtr, Mask) \
        XGpio_DiscreteWrite(InstancePtr, led_tri_o+2, Mask)

#define XGpio_DiscreteSet(InstancePtr, Mask) \
        XGpio_DiscreteSet(InstancePtr, led_tri_o+2, Mask)

#endif
XGpio Gpio;
int main(void)
{
	int Status;
	volatile int Delay;

	/* Initialize the GPIO driver */
	Status = XGpio_Initialize(&Gpio, GPIO_EXAMPLE_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}
	/* Set the direction for all signals as inputs except the LED output */
	XGpio_SetDataDirection(&Gpio, led_tri_o, ~LED);
	XGpio_SetDataDirection(&Gpio, led_tri_o+2, ~LED);

	/* Loop forever blinking the LED */

	while (1) {
		/* Set the LED to High */
		XGpio_DiscreteWrite(&Gpio, led_tri_o, LED);

		/* Wait a small amount of time so the LED is visible */
		for (Delay = 0; Delay < LED_DELAY1; Delay++);

		/* Clear the LED bit */
		XGpio_DiscreteClear(&Gpio, led_tri_o, LED);

		/* Wait a small amount of time so the LED is visible */



	XGpio_DiscreteWrite(&Gpio, led_tri_o+2, LED);

			/* Wait a small amount of time so the LED is visible */
			for (Delay = 0; Delay < LED_DELAY1; Delay++);

			/* Clear the LED bit */
			XGpio_DiscreteClear(&Gpio, led_tri_o+2, LED);

			/* Wait a small amount of time so the LED is visible */
			for (Delay = 0; Delay < LED_DELAY1; Delay++);
	}
	xil_printf("Successfully ran Gpio Example\r\n");
	return XST_SUCCESS;
}

