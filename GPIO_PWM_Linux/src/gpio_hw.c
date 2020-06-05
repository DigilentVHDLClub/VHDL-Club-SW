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
#include "pwm.h"
#include <gpiod.h>

#define PWM_PERIOD 128
int main() {
	struct gpiod_chip *led, *btns, *sw;
	struct gpiod_line_bulk led_bulk, btns_bulk, sw_bulk;

	PWM red, green, blue;

	int led_value[4] = { 0, 0, 0, 0 };
	int sw_values[4];
	int btns_values[4];
	int r_val = 0, g_val = 0, b_val = 0;

	printf("Hello World\n");
	setbuf(stdout, NULL);
	led = gpiod_chip_open_by_number(2);
	btns = gpiod_chip_open_by_number(0);
	sw = gpiod_chip_open_by_number(1);
	if (!led || !sw || !btns) {
		perror("Unable to open gpio chip");
		return 1;
	}
	gpiod_chip_get_all_lines(led, &led_bulk);
	gpiod_chip_get_all_lines(btns, &btns_bulk);
	gpiod_chip_get_all_lines(sw, &sw_bulk);
	gpiod_line_request_bulk_output(&led_bulk, "leds", led_value);
	gpiod_line_request_bulk_input(&sw_bulk, "sw");
	gpiod_line_request_bulk_input(&btns_bulk, "btns");
	red = PWM_Initialize(2, 0);
	blue = PWM_Initialize(0, 0);
	green = PWM_Initialize(1, 0);
	PWM_WriteReg(red, PWM_NFCK_REG, PWM_PERIOD);
	PWM_WriteReg(blue, PWM_NFCK_REG, PWM_PERIOD);
	PWM_WriteReg(green, PWM_NFCK_REG, PWM_PERIOD);
	PWM_WriteReg(blue, PWM_DUTYC_REG, b_val);
	PWM_WriteReg(red, PWM_DUTYC_REG, r_val);
	PWM_WriteReg(green, PWM_DUTYC_REG, g_val);

	gpiod_line_get_value_bulk(&btns_bulk, btns_values);
	while (btns_values[0] == 0) {//exit on left_most_button press
		gpiod_line_get_value_bulk(&btns_bulk, btns_values);
		gpiod_line_get_value_bulk(&sw_bulk, sw_values);
		if (btns_values[1] == 1) {
			if (sw_values[3] == 1) {
				r_val = 0;
			}
			if (sw_values[2] == 1) {
				g_val = 0;
			}
			if (sw_values[1] == 1) {
				b_val = 0;
			}
		}
		if (btns_values[2] == 1) {
			if (sw_values[3] == 1) {
				r_val = 100;
			}
			if (sw_values[2] == 1) {
				g_val = 100;
			}
			if (sw_values[1] == 1) {
				b_val = 100;
			}
		}
		PWM_WriteReg(blue, PWM_DUTYC_REG, b_val);
		PWM_WriteReg(red, PWM_DUTYC_REG, r_val);
		PWM_WriteReg(green, PWM_DUTYC_REG, g_val);
		gpiod_line_set_value_bulk(&led_bulk,sw_values);
	}
	printf("Exiting");
	PWM_WriteReg(blue, PWM_DUTYC_REG, 0);
	PWM_WriteReg(red, PWM_DUTYC_REG, 0);
	PWM_WriteReg(green, PWM_DUTYC_REG, 0);
	gpiod_line_set_value_bulk(&led_bulk,led_value);
	gpiod_line_release_bulk(&sw_bulk);
	gpiod_line_release_bulk(&btns_bulk);
	gpiod_line_release_bulk(&led_bulk);
	gpiod_chip_close(led);
	gpiod_chip_close(btns);
	gpiod_chip_close(sw);
	return 0;
}
