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
 * disco.c: simple application for the RGB LED
 *
 * The frequency divider/duty factor of each PWM Generator get random values
 * to obtain random colors. The program uses the standard library rand() function
 * to generate both the seed and the random results.
 *
 * Side-note: an alternative for the rand() function might be a
 * pseudo-random number generator embedded directly into the hardware,
 * such as: https://github.com/snoobism/LFSR-Counter
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

#include <stdlib.h>
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xgpio_l.h"
#include "axi_pwm_generator.h"

#define AXI_PWM_GEN_RED_BASEADDR 0x43C00000 // base address of PWM generator for RED LED
#define AXI_PWM_GEN_GRN_BASEADDR 0x43C10000 // base address of PWM generator for GREEN LED
#define AXI_PWM_GEN_BLU_BASEADDR 0x43C20000 // base address of PWM generator for BLUE LED

#define GPIO_OUT_BASEADDR	XPAR_GPIO_1_BASEADDR
#define GPIO_IN_BASEADDR	XPAR_GPIO_0_BASEADDR

#define LED_DELAY	 15000000

int main()
{
    init_platform();
	volatile int delay;

	srand (1); // seed of random generator

	while(1){
			// frequency divider can take any 8 bit number
			// duty factor can take any 6 bit number (so that the color intensity is not too harsh)
			Xil_Out8(AXI_PWM_GEN_RED_BASEADDR + AXI_PWM_GENERATOR_DIVIDER_OFFSET, rand() % 256);
			Xil_Out8(AXI_PWM_GEN_RED_BASEADDR + AXI_PWM_GENERATOR_DUTY_OFFSET, rand() % 64);

			Xil_Out8(AXI_PWM_GEN_GRN_BASEADDR + AXI_PWM_GENERATOR_DIVIDER_OFFSET, rand() % 256);
			Xil_Out8(AXI_PWM_GEN_GRN_BASEADDR + AXI_PWM_GENERATOR_DUTY_OFFSET, rand() % 64);

			Xil_Out8(AXI_PWM_GEN_BLU_BASEADDR + AXI_PWM_GENERATOR_DIVIDER_OFFSET, rand() % 256);
			Xil_Out8(AXI_PWM_GEN_BLU_BASEADDR + AXI_PWM_GENERATOR_DUTY_OFFSET, rand() % 64);

			// delay so that you don't have a seizure looking at these
			for (delay = 0; delay < LED_DELAY; delay++);

    	}
    cleanup_platform();
    return 0;
}
