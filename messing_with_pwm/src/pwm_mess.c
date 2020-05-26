
/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"

/************************** Constant Definitions *****************************/

#define LED 0x01   /* Assumes bit 0 of GPIO is connected to an LED  */

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define GPIO_EXAMPLE_DEVICE_ID  XPAR_GPIO_0_DEVICE_ID

/*
 * The following constant is used to wait after an LED is turned on to make
 * sure that it is visible to the human eye.  This constant might need to be
 * tuned for faster or slower processor speeds.
 */
#define LED_DELAY     10000000

/*
 * The following constant is used to determine which channel of the GPIO is
 * used for the LED if there are 2 channels supported.
 */
#define LED_CHANNEL 1

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/

#ifdef PRE_2_00A_APPLICATION

/*
 * The following macros are provided to allow an application to compile that
 * uses an older version of the driver (pre 2.00a) which did not have a channel
 * parameter. Note that the channel parameter is fixed as channel 1.
 */
#define XGpio_SetDataDirection(InstancePtr, DirectionMask) \
        XGpio_SetDataDirection(InstancePtr, LED_CHANNEL, DirectionMask)

#define XGpio_DiscreteRead(InstancePtr) \
        XGpio_DiscreteRead(InstancePtr, LED_CHANNEL)

#define XGpio_DiscreteWrite(InstancePtr, Mask) \
        XGpio_DiscreteWrite(InstancePtr, LED_CHANNEL, Mask)

#define XGpio_DiscreteSet(InstancePtr, Mask) \
        XGpio_DiscreteSet(InstancePtr, LED_CHANNEL, Mask)

#endif

/************************** Function Prototypes ******************************/


/************************** Variable Definitions *****************************/

/*
 * The following are declared globally so they are zeroed and so they are
 * easily accessible from a debugger
 */

XGpio btn_sw, led; /* The Instance of the GPIO Driver */

/*****************************************************************************/
/**
*
* The purpose of this function is to illustrate how to use the GPIO
* driver to turn on and off an LED.
*
* @param	None
*
* @return	XST_FAILURE to indicate that the GPIO Initialization had
*		failed.
*
* @note		This function will not return if the test is running.
*
******************************************************************************/
int main(void)
{
	int Status;
	volatile int Delay;
	u32 btn_state, sw_state, led_state, duty_state;

	/* Initialize the GPIO driver */
	Status = XGpio_Initialize(&btn_sw, XPAR_AXI_GPIO_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}
	Status = XGpio_Initialize(&led, XPAR_AXI_GPIO_1_DEVICE_ID);
		if (Status != XST_SUCCESS) {
			xil_printf("Gpio Initialization Failed\r\n");
			return XST_FAILURE;
		}

	/* Set the direction for all signals as inputs except the LED output */
	XGpio_SetDataDirection(&led, 1, 0x00);
	XGpio_SetDataDirection(&btn_sw, 1, 0xff);
	XGpio_SetDataDirection(&btn_sw, 2, 0xff);

	Xil_Out32(XPAR_MYIP_0_S00_AXI_BASEADDR + 4, 0x0f);//writing to the clock divider register
	Xil_Out32(XPAR_MYIP_1_S00_AXI_BASEADDR + 4, 0x0f);//writing to the clock divider register
	Xil_Out32(XPAR_MYIP_2_S00_AXI_BASEADDR + 4, 0x0f);//writing to the clock divider register

	while (1) {

		btn_state = XGpio_DiscreteRead(&btn_sw, 1);
		sw_state = XGpio_DiscreteRead(&btn_sw, 2);

		led_state = (btn_state & sw_state);
		duty_state = (btn_state | sw_state);

		XGpio_DiscreteWrite(&led, 1, led_state);

		Xil_Out32(XPAR_MYIP_0_S00_AXI_BASEADDR + 8, duty_state<<3);
		Xil_Out32(XPAR_MYIP_1_S00_AXI_BASEADDR + 8, duty_state>>2);
		Xil_Out32(XPAR_MYIP_2_S00_AXI_BASEADDR + 8, duty_state<<2);

	}

	xil_printf("Successfully ran Gpio Example\r\n");
	return XST_SUCCESS;
}
