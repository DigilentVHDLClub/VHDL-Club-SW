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

/***************************** Include Files ********************************/
#include <stdio.h>
#include <signal.h>
#include <gpiod.h>
#include "PWM/pwm.h"
#include <unistd.h>

/************************** Constant Definitions ****************************/

//GPIO chip numbers registered by Linux
#define BTN_GPIO_CHIP_NR 	0
#define SW_GPIO_CHIP_NR 	1
#define LED_GPIO_CHIP_NR 	2
//UIO registered PWM number
#define PWM_UIO_RED_NR		2
#define PWM_UIO_GREEN_NR	1
#define PWM_UIO_BLUE_NR		0
//there is only one map per PWM UIO
#define PWM_UIO_MAP_NR		0

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions ********************/

/************************** Variable Definitions ****************************/

static volatile int run;

/************************** Function Definitions *****************************/

/****************************************************************************/
/**
* Interrupt handler for the CTRL-C key brake. This will set the variable run to
* 0 which will cause the while loop in the main fucntion to exit and destroy
* all the assigned variables
*
* @note		None.
*
*****************************************************************************/
void intHandler()
{
    run = 0;
}

//main
int main()
{
	//pointers to the /dev/gpiochipx
	struct gpiod_chip *sw;
	struct gpiod_chip *btn;
	struct gpiod_chip *led;

	//lines of each /dev/gpiochip
	struct gpiod_line_bulk btn_bulk;
	struct gpiod_line_bulk sw_bulk;
	struct gpiod_line_bulk led_bulk;

	//value arrays of the chips in which value will be
	//read from or written to the gpiochips
    int btn_val[4];
    int sw_val[4];
    int led_val[4]={0,0,0,0};

    //PWM vars
    PWM *red;
    PWM *green;
    PWM *blue;

    int ret, inc, dec;

    printf("Hello World\n");

    //init BTN
    {
		//Open BTN gpiochip0 for use
		btn = gpiod_chip_open_by_number(BTN_GPIO_CHIP_NR);
		if (!btn)
		{
			perror("Open btn chip failed\n");
			goto end;
		}
		//populate btn_buk with all the lines of the BTN gpiochip
		ret = gpiod_chip_get_all_lines(btn, &btn_bulk);
		if (ret < 0)
		{
			perror("Getting all btn lines failed\n");
			goto btn_close;
		}
		//set all lines of the BTN gpiochip to input
		ret = gpiod_line_request_bulk_input(&btn_bulk, "btn");
		if (ret < 0)
		{
			perror("Setting all btn lines to input failed\n");
			goto btn_line_close;
		}
    }
    //init SW
    {
    	//Open SW gpiochip1 for use
		sw = gpiod_chip_open_by_number(SW_GPIO_CHIP_NR);
		if (!sw) {
			perror("Open sw chip failed\n");
			goto btn_line_close;
		}
		//populate sw_buk with all the lines of the SW gpiochip
		ret = gpiod_chip_get_all_lines(sw, &sw_bulk);
		if (ret < 0)
		{
			perror("Getting all sw lines failed\n");
			goto sw_close;
		}
		//set all lines of the SW gpiochip to input
		ret = gpiod_line_request_bulk_input(&sw_bulk, "sw");
		if (ret < 0)
		{
			perror("Setting all sw lines to input failed\n");
			goto sw_line_close;
		}
    }
    //init LED
    {
    	//Open LED gpiochip2 for use
		led = gpiod_chip_open_by_number(LED_GPIO_CHIP_NR);
		if (!led) {
			perror("Open led chip failed\n");
			goto sw_line_close;
		}
		//populate led_buk with all the lines of the LED gpiochip
		ret = gpiod_chip_get_all_lines(led, &led_bulk);
		if (ret < 0)
		{
			perror("Getting all led lines failed\n");
			goto led_close;
		}
		//set all lines of the LED gpiochip to output
		ret = gpiod_line_request_bulk_output(&led_bulk, "btn", led_val);
		if (ret < 0)
		{
			perror("Setting all led lines to output failed\n");
			goto led_line_close;
		}
    }
    //init PWM
    {
    	//get UIO pointer to RGB LEDs on the PWM IP
    	red = PWM_Initialize(PWM_UIO_RED_NR, PWM_UIO_MAP_NR);
    	green = PWM_Initialize(PWM_UIO_GREEN_NR, PWM_UIO_MAP_NR);
    	blue = PWM_Initialize(PWM_UIO_BLUE_NR, PWM_UIO_MAP_NR);

    	//enable the PWM IPs
    	PWM_WriteReg(red, PWM_CTL_REG, (1<<PWM_EN_BIT));
    	PWM_WriteReg(green, PWM_CTL_REG, (1<<PWM_EN_BIT));
    	PWM_WriteReg(blue, PWM_CTL_REG, (1<<PWM_EN_BIT));

    	//set default clock division factor
    	PWM_WriteReg(red, PWM_NFCK_REG, 128);
    	PWM_WriteReg(green, PWM_NFCK_REG, 128);
    	PWM_WriteReg(blue, PWM_NFCK_REG, 128);

    	//set default duty cycle factor
    	PWM_WriteReg(red, PWM_DUTYC_REG, 10);
    	PWM_WriteReg(green, PWM_DUTYC_REG, 10);
    	PWM_WriteReg(blue, PWM_DUTYC_REG, 10);


    }

    //set default values
    inc = 0;
    dec = 0;
    run = 1;

    //set the CLTR-C handler
    signal(SIGINT, intHandler);

    //print the console menu
    printf("GPIO to RBG control\n\n");
    printf("BNT0 increases duty cycle\n");
    printf("BNT3 decreases duty cycle\n");
    printf("SW[2,0] are used to enable the changing of the duty cycle for the Red, Green and Blue\n");
    printf("To exit use CTRL-C\n");

	while(run)
	{
		//output SW values to LEDs
		gpiod_line_get_value_bulk(&sw_bulk, sw_val);
		gpiod_line_set_value_bulk(&led_bulk, sw_val);

		//read BTN values
		gpiod_line_get_value_bulk(&btn_bulk, btn_val);

		//update if increment btn status
		if(btn_val[0] != inc)
			inc = btn_val[0];

		//update if decrement btn status
		if(btn_val[3] != dec)
			dec = btn_val[3];

		//increase duty cycle of the active RGBs
		if (inc)
		{
			if(sw_val[0])
				PWM_IncreaseDuty(red,10);
			if(sw_val[1])
				PWM_IncreaseDuty(green,10);
			if(sw_val[2])
				PWM_IncreaseDuty(blue,10);
		}

		//decrease duty cycle of the active RGBs
		if (dec)
		{
			if(sw_val[0])
				PWM_DecreaseDuty(red,10);
			if(sw_val[1])
				PWM_DecreaseDuty(green,10);
			if(sw_val[2])
				PWM_DecreaseDuty(blue,10);
		}
		usleep(100000);
	}

	PWM_Destroy(red);
	PWM_Destroy(green);
	PWM_Destroy(blue);

led_line_close:
	gpiod_line_release_bulk(&led_bulk);
led_close:
	gpiod_chip_close(led);
sw_line_close:
	gpiod_line_release_bulk(&sw_bulk);
sw_close:
	gpiod_chip_close(sw);
btn_line_close:
	gpiod_line_release_bulk(&btn_bulk);
btn_close:
	gpiod_chip_close(btn);
end:
    return 0;
}
