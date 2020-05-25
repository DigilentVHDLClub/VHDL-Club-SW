/*
 * rgb_pwm.c: Light up RGB LED on Genesys ZU using three PWM signals, controlled using buttons and switches.
 */

#include <stdio.h>
#include "xil_printf.h"
#include "xgpio_l.h"
#include "xparameters.h"

#define BTN_GPIO_BADDR XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA_OFFSET
#define SWT_GPIO_BADDR XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET
#define R_PWM_BADDR XPAR_AXI_PWM_GENERATOR_R_S00_AXI_BASEADDR
#define G_PWM_BADDR XPAR_AXI_PWM_GENERATOR_G_S00_AXI_BASEADDR
#define B_PWM_BADDR XPAR_AXI_PWM_GENERATOR_B_S00_AXI_BASEADDR

const u8 minPwm = 0;
const u8 maxPwm = 255;
const u8 pwmStep = 8;

// Decode whether we increase or decrease duty cycle for the desired RGB channel
u8 CheckButtonUpDown(u8 crntBtn, u8 crntPwm) {
	u8 pwm = crntPwm;

	// If BTN Up is pressed
	if (crntBtn == 0x10) {
		if (pwm < maxPwm) {
			pwm += pwmStep;
		}
	}
	else {
		// If BTN Down is pressed
		if (crntBtn == 0x08) {
			if (pwm > minPwm) {
				pwm -= pwmStep;
			}
		}
	}

	return pwm;
}


int main()
{
    u8 btn;
    u8 swt;
    u8 duty_r = 0;
    u8 duty_g = 0;
    u8 duty_b = 0;

    // Set division constant for each PWM channel to maximum
    Xil_Out8(R_PWM_BADDR + 0x4, 255);
    Xil_Out8(G_PWM_BADDR + 0x4, 255);
    Xil_Out8(B_PWM_BADDR + 0x4, 255);

    // Set initial duty cycle for each PWM channel
    Xil_Out8(R_PWM_BADDR + 0x8, duty_r);
    Xil_Out8(G_PWM_BADDR + 0x8, duty_g);
    Xil_Out8(B_PWM_BADDR + 0x8, duty_b);

    // Enable each PWM channel
    Xil_Out8(R_PWM_BADDR, 2);
    Xil_Out8(G_PWM_BADDR, 2);
    Xil_Out8(B_PWM_BADDR, 2);

    // Now the RGB LED should light up in white...

    while(1) {
    	// Read current state of switches
    	swt=Xil_In8(SWT_GPIO_BADDR);

    	// Read current state of buttons
    	btn=Xil_In8(BTN_GPIO_BADDR);

    	// Decode which RGB channel is currently selected with switches
    	// R channel
    	if (swt == 4) {
    		// Update duty cycle based on pressed button (if any)
    		duty_r = CheckButtonUpDown(btn, duty_r);
    		// Change RGB channel duty cycle accordingly
            Xil_Out8(R_PWM_BADDR + 0x8, duty_r);
    	}
    	else {
    		// G channel
    		if (swt == 2) {
    			// Update duty cycle based on pressed button (if any)
    			duty_g = CheckButtonUpDown(btn, duty_g);
				// Change RGB channel duty cycle accordingly
				Xil_Out8(G_PWM_BADDR + 0x8, duty_g);
    		}
    		else {
    			// B channel
    			if (swt == 1) {
    				// Update duty cycle based on pressed button (if any)
					duty_b = CheckButtonUpDown(btn, duty_b);
					// Change RGB channel duty cycle accordingly
					Xil_Out8(B_PWM_BADDR + 0x8, duty_b);
    			}
    		}
    	}

    	// Wait until no button is pressed
    	while (btn != 0) {
    		btn=Xil_In8(BTN_GPIO_BADDR);
    	}

    	// Add a delay before adjusting the duty cycle again
    	for (int i=0;i<10000;i++) {
    	}

    }

    return 0;
}
