/*
 * twoLeds.c
 *
 *  Created on: May 16, 2020
 *      Author: Nita Eduard
 *
 * Basic program that turns on LD1 and LD3 on the Zybo-Z7-10 FPGA.
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
#include "platform.h"
#include "xparameters.h"
#include "xgpio_l.h"
#include "xil_printf.h"

#define GPIO_BASEADDR	XPAR_GPIO_1_BASEADDR

int main(){
    init_platform();
    /*
      	since data register address coincides with the GPIO base address,
    	we use an offset of 0 in the WriteReg function and pass into
    	the data field the value 10, which in binary is 1010.
    	Thus, since our led ports are of width 4, they will be powered by this value,
    	i.e. LD3 will be powered by the MSB and L1 by the second-LSB.
	*/
    XGpio_WriteReg((GPIO_BASEADDR), 0, 10); // turn on LEDs

    print("yay!");
    cleanup_platform();
    return 0;
}
