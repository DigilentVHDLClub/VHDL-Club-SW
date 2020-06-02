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
#define BTN_GPIO_BADDR XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA_OFFSET
#define SWT_GPIO_BADDR XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET
#define R_pwm_BADDR XPAR_axi_pwm_generator_rezerva_0_S00_AXI_BASEADDR
#define G_pwm_BADDR XPAR_axi_pwm_generator_rezerva_1_S00_AXI_BASEADDR
#define B_pwm_BADDR XPAR_axi_pwm_generator_rezerva_2_S00_AXI_BASEADDR
int main()
{
    u8 r,g,b;
    u8 butoane,switchuri;
    Xil_Out8(R_pwm_BADDR + 0x4, 255);
      Xil_Out8(G_pwm_BADDR + 0x4, 255);
      Xil_Out8(B_pwm_BADDR + 0x4, 255);

}







