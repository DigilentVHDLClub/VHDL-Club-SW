/*
 * Copyright (c) 2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <stdio.h>
#include <gpiod.h>
#include "pwm.h"
int main()
{
	struct gpiod_chip *led;
	struct gpiod_line_bulk led_bulk;

	PWM red,green,blue;

	int led_value[4]={1,0,1,0};
    printf("Hello World\n");

    led = gpiod_chip_open_by_number(2);
    if(!led){
    	perror("Unable to open gpio chip");
    	return 1;
    }
    gpiod_chip_get_all_lines(led,&led_bulk);
    gpiod_line_request_bulk_output(&led_bulk,"leds",led_value);

    red = PWM_Initialize(2,0);
    blue = PWM_Initialize(0,0);
    green = PWM_Initialize(1,0);
    PWM_WriteReg(red,PWM_NFCK_REG,128);
    PWM_WriteReg(red,PWM_DUTYC_REG,20);
    PWM_WriteReg(blue,PWM_NFCK_REG,128);
    PWM_WriteReg(blue,PWM_DUTYC_REG,20);
    PWM_WriteReg(green,PWM_NFCK_REG,128);
    PWM_WriteReg(green,PWM_DUTYC_REG,100);


    gpiod_line_release_bulk(&led_bulk);
    gpiod_chip_close(led);
    return 0;
}
