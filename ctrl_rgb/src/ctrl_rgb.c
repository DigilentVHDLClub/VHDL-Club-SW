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
 * ctrl_rgb.c: simple application that controls the RGB led on the ZyboZ7 FPGA board
 * using switches and buttons
 *
 * using the switches you are able to select the led color as follows:
 *
 * sw = 00 => red
 * sw = 01 => blue
 * sw = 10 => green
 *
 * for the selected color, you can then control it's PWM parameters using the buttons as follows:
 * btn(3) => decrease frequency divider
 * btn(2) => increase frequency divider
 * btn(1) => decrease duty
 * btn(0) => increase duty
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
#include "platform.h"
#include "platform.h"
#include "xil_printf.h"
#include "xgpio_l.h"
#include "axi_pwm_generator.h"

#define AXI_PWM_GEN_RED_BASEADDR 0x43C00000 // base address of PWM generator for RED LED
#define AXI_PWM_GEN_GRN_BASEADDR 0x43C10000 // base address of PWM generator for GREEN LED
#define AXI_PWM_GEN_BLU_BASEADDR 0x43C20000 // base address of PWM generator for BLUE LED

#define GPIO_OUT_BASEADDR	XPAR_GPIO_1_BASEADDR
#define GPIO_IN_BASEADDR	XPAR_GPIO_0_BASEADDR

#define LED_DELAY	 100000

typedef struct pwm{
	u8 duty;
	u8 freq_divider;
}PWM;

void modify_led(PWM* pwm_led, u8 btn){
	// obtain values for btn(3) and btn(2)
	if(btn >> 2 == 1){ // if btn(3) is off and btn(2) is on, increase division factor
		if(pwm_led->freq_divider < 255) // prevent overflowing
			pwm_led->freq_divider++;
	}else
	if(btn >> 2 == 2){ // if btn(2) is off and btn(3) is on, decrease division factor
		if(pwm_led->freq_divider > 0)
			pwm_led->freq_divider--;
	}

	// obtain values for btn(1) and btn(0)
	if((btn & 0b11) == 1){ // if btn(1) is off and btn(0) is on, increase duty
		if(pwm_led->duty < 255)
			pwm_led->duty++;
	}else
	if((btn & 0b11) == 2){ // if btn(0) is off and btn(1) is on, decrease duty
		if(pwm_led->duty > 0)
			pwm_led->duty--;
	}
}

int main()
{
    init_platform();
	volatile int delay;

	enum pwm_color{red, blue, green};

	u8 btn, sw = 0;

	// initialize pwm freq_divider/duty with 0
	PWM* pwm_red = (PWM*)calloc(1, sizeof(PWM));
	PWM* pwm_blue = (PWM*)calloc(1, sizeof(PWM));
	PWM* pwm_green = (PWM*)calloc(1, sizeof(PWM));


	while(1){

		btn = Xil_In8(GPIO_IN_BASEADDR + XGPIO_DATA_OFFSET); // obtain button values
		sw = Xil_In8(GPIO_IN_BASEADDR + XGPIO_DATA2_OFFSET); // obtain switch values

		switch(sw){
			case red:
				modify_led(pwm_red, btn);
				break;
			case blue:
				modify_led(pwm_blue, btn);
				break;
			case green:
				modify_led(pwm_green, btn);
				break;
		}

		Xil_Out8(AXI_PWM_GEN_RED_BASEADDR + AXI_PWM_GENERATOR_DIVIDER_OFFSET, pwm_red->freq_divider);
		Xil_Out8(AXI_PWM_GEN_RED_BASEADDR + AXI_PWM_GENERATOR_DUTY_OFFSET, pwm_red->duty);

		Xil_Out8(AXI_PWM_GEN_GRN_BASEADDR + AXI_PWM_GENERATOR_DIVIDER_OFFSET, pwm_green->freq_divider);
		Xil_Out8(AXI_PWM_GEN_GRN_BASEADDR + AXI_PWM_GENERATOR_DUTY_OFFSET, pwm_green->duty);

		Xil_Out8(AXI_PWM_GEN_BLU_BASEADDR + AXI_PWM_GENERATOR_DIVIDER_OFFSET, pwm_blue->freq_divider);
		Xil_Out8(AXI_PWM_GEN_BLU_BASEADDR + AXI_PWM_GENERATOR_DUTY_OFFSET, pwm_blue->duty);
		for (delay = 0; delay < LED_DELAY; delay++);

	}

    cleanup_platform();
    return 0;
}
