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
#define INPUT_GPIO_BADDR XPAR_AXI_GPIO_0_BASEADDR
#define AXI_PWM_RED_BADDR 0x43C0_0000
#define AXI_PWM_BLUE_BADDR 0x43C1_0000
#define AXI_PWM_GREEN_BADDR 0x43C2_0000
int main()
{
    u8 r,g,b;
   Xil_Out8(AXI_PWM_RED_BADDR + 0x4,0xFF);
    Xil_Out8(AXI_PWM_BLUE_BADDR + 0x4,0xFF);
    Xil_Out8(AXI_PWM_GREEN_BADDR + 0x4,0xFF);

}







