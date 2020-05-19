/*
 * main.c
 *
 *  Created on: 19 mai 2020
 *      Author: thinkthinkthink
 */


#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio_l.h"
#include "xil_io.h"

#define GPIO_REG_BASEADDR	XPAR_GPIO_1_BASEADDR
#define LED_CHANNEL	1
#define LED 0x05

int main()
{


    XGpio_WriteReg((GPIO_REG_BASEADDR),
    				((LED_CHANNEL - 1) * XGPIO_CHAN_OFFSET) +
    				XGPIO_DATA_OFFSET, LED);

    return 0;
}
