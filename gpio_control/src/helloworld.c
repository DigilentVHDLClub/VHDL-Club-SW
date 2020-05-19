/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xgpio.h"

#define GPIO_SW_ID XPAR_AXI_GPIO_0_DEVICE_ID
#define GPIO_LED_ID XPAR_AXI_GPIO_1_DEVICE_ID

#define SW_CHANNEL 2
#define BTN_CHANNEL	1
#define LED_CHANNEL 1

#define BTNS_MASK 	(0xF)
#define SWS_MASK 	(0xF)
#define LEDS_MASK 	(0xF)


int main()
{
    init_platform();

    print("Hello World\n\r");

    XGpio GpioBtnSw, GpioLed;
    XStatus Status;

    Status = XGpio_Initialize(&GpioBtnSw, GPIO_SW_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}

    Status = XGpio_Initialize(&GpioLed, GPIO_LED_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}

	/* Set the direction for LED output */
	XGpio_SetDataDirection(&GpioLed, LED_CHANNEL, 0x0);

	/* Set the direction for LED output */
	XGpio_SetDataDirection(&GpioBtnSw, BTN_CHANNEL, 0xFFFFFFFF);
	XGpio_SetDataDirection(&GpioBtnSw, SW_CHANNEL, 0xFFFFFFFF);

	while (1)
	{
		u32 sw = XGpio_DiscreteRead(&GpioBtnSw, SW_CHANNEL);
		sw &= SWS_MASK;
		u32 leds = XGpio_DiscreteRead(&GpioLed, LED_CHANNEL);
		leds &= ~LEDS_MASK;
		leds |= (sw & LEDS_MASK);
		XGpio_DiscreteWrite(&GpioLed, LED_CHANNEL, leds);
	}

    cleanup_platform();
    return 0;
}
