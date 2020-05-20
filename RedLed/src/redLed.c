/*
 * @Author Nita Eduard
 * @file redLed.c
 *
 *	This is a simple application that turns on a basic LED if
 *	the switch and button corresponding to it are both on. (LD(i) = SW(i) and BTN(i))
 *
 *	This program also turns on the red led using a PWM signal with the
 *	duty factor and clock factor of an arbitrary value.
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
#include "xparameters.h"
#include "platform.h"
#include "xil_printf.h"
#include "xil_io.h"
#include "xgpio_l.h"
#include "axi_pwm_generator.h"

#define AXI_PWM_GENERATOR_BASEADDR 0x43C00000 // base address of PWM generator

#define GPIO_OUT_BASEADDR	XPAR_GPIO_1_BASEADDR
#define GPIO_IN_BASEADDR	XPAR_GPIO_0_BASEADDR


int main()
{
    init_platform();
	u8 btn, sw = 0;

	while(1){

		btn = Xil_In8(GPIO_IN_BASEADDR + XGPIO_DATA_OFFSET); // obtain button values
		sw = Xil_In8(GPIO_IN_BASEADDR + XGPIO_DATA2_OFFSET); // obtain switch values

		Xil_Out8(GPIO_OUT_BASEADDR, sw & btn); // Each LDi is powered on if both SWi and BTNi are on

		// Uncomment code below if you want to control the PWM Signal using switches and buttons
//		Xil_Out8(AXI_PWM_GENERATOR_BASEADDR + AXI_PWM_GENERATOR_DIVIDER_OFFSET, sw);
//		Xil_Out8(AXI_PWM_GENERATOR_BASEADDR + AXI_PWM_GENERATOR_DUTY_OFFSET, btn);
	}

	Xil_Out8(AXI_PWM_GENERATOR_BASEADDR + AXI_PWM_GENERATOR_DIVIDER_OFFSET, 0x5);
	Xil_Out8(AXI_PWM_GENERATOR_BASEADDR + AXI_PWM_GENERATOR_DUTY_OFFSET, 0x5);


    cleanup_platform();
    return 0;
}
