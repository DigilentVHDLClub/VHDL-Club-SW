/*
 * turnrgb.c
 *
 *  Created on: May 25, 2020
 *      Author: alin
 */
#include <stdio.h>
//#include "axi_pwm_gen_1.0.h"
#include "xil_printf.h"
#include "xgpio_l.h"
#include "xparameters.h"
#define GPIO_REG_BASEADDR	XPAR_GPIO_1_BASEADDR
#define BTN_REG_BASEADDR   XPAR_GPIO_0_BASEADDR
//#define AXI_PWM_GEN_red_BDDR 0x43C0_0000//for red
//#define AXI_PWM_GEN_green_BDDR 0x43C1_0000//for green
//#define AXI_PWM_GEN_blue_BDDR 0x43C2_0000//for blue
int main()
{
	u8 btn;
	Xil_Out8(GPIO_REG_BASEADDR,0x5);
	Xil_Out8(AXI_PWM_GEN_red_BDDR+0x4,0xFF);
	Xil_Out8(AXI_PWM_GEN_green_BDDR+0x4,0xFF);
	Xil_Out8(AXI_PWM_GEN_blue_BDDR+0x4,0xFF);



}







