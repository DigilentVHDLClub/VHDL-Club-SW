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
#include "xgpio_l.h"
#include "xparameters.h"

#define INPUT_GPIO_BADDR XPAR_AXI_GPIO_0_BASEADDR
#define PWM_RED_BADDR XPAR_AXI_PWM_GENERATOR_2_S00_AXI_BASEADDR
#define PWM_BLUE_BADDR XPAR_AXI_PWM_GENERATOR_0_S00_AXI_BASEADDR
#define PWM_GREEN_BADDR XPAR_AXI_PWM_GENERATOR_1_S00_AXI_BASEADDR
int main()
{
    init_platform();
    u8 in1,in2;
    u8 red_int,blue_int,green_int;
    print("Hello World\n\r");
    XGpio_WriteReg(INPUT_GPIO_BADDR,XGPIO_TRI_OFFSET,0xF);//define channel 1 of Input GPIO as all input
    XGpio_WriteReg(INPUT_GPIO_BADDR,XGPIO_TRI2_OFFSET,0xF);// define channel 2 of Input GPIO as all input
    Xil_Out8(PWM_RED_BADDR + 0x4,0xFF); // set Nfckpwm_red
    Xil_Out8(PWM_BLUE_BADDR + 0x4,0xFF); // set Nfckpwm_blue
    Xil_Out8(PWM_GREEN_BADDR + 0x4,0xFF); // set Nfckpwm_green
    while(1){
    	in1 = Xil_In8(INPUT_GPIO_BADDR);// take the input from the switches
    	in2 = Xil_In8(INPUT_GPIO_BADDR + XGPIO_DATA2_OFFSET); // take the input from the buttons
    	red_int = (in1 & 0x8) + (in2 & 0x8 >> 1); // take the 3rd right most button and the left most switch and construct a value for red led
    	blue_int = ((in1 & 0x4) + (in2 & 0x4 >> 1)) << 1; // take the 2rd right most button and the 2nd left switch and construct a value for blue led
    	green_int = ((in1 & 0x2) + (in2 & 0x2 >> 1)) << 2; // take the right most button and the 3rd left switch and construct a value for green led
    	Xil_Out8(PWM_RED_BADDR + 0x8,red_int);// write to the pwm register
    	Xil_Out8(PWM_BLUE_BADDR + 0x8,blue_int);// write to the pwm register
    	Xil_Out8(PWM_GREEN_BADDR + 0x8,green_int);// write to the pwm register
    }
    cleanup_platform();
    return 0;
}
