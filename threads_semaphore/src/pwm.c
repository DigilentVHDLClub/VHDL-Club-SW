/*
 * pwm.c
 *
 *  Created on: Jun 1, 2020
 *      Author: anubis
 */

/***************************** Include Files ********************************/
#include <libuio.h>
#include "pwm.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions ********************/

/************************** Variable Definitions ****************************/

/************************** Function Definitions *****************************/

/****************************************************************************/
/**
* Initialize the PWM instance.
*
* @param	uioNum is the uio device number -- zero-indexed
* @param	mapNum is the specific map partition inside the UIO device
*			if the UIO device contains only one partition, then the
*			map number is 0
*
* @return
* 		- PWM	which is the uio map number pointer.
*
* @note		None.
*
*****************************************************************************/

PWM PWM_Initialize(uint8_t uioNum, uint8_t mapNum)
{
	PWM pwminst;

	UIO *uio = UIO_MAP(uioNum, mapNum);
	if (uio == NULL)
	{
		fprintf(stderr, "Failed to get uio map\n");
		exit(EXIT_FAILURE);
	}
	pwminst = uio->mapPtr;

	return pwminst;
}

/****************************************************************************/
/**
* Write to a specific offset of the PWM
*
* @param pwm is a pointer to an pwm uio map. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the pwm API must be
*		made with this pointer.
*
* @param RegOffset a unsigned 8 bit variable which represents the
* 		register offset of the PWM IP
*
* @param Data unsigned 32 bit variable of the data sent to the register
*
* @return
* 		None
*
* @note		None.
*
*****************************************************************************/
void PWM_WriteReg(PWM pwminst, uint8_t RegOffset, uint32_t Data)
{
	ACCESS_REG(pwminst, RegOffset) = Data;
}

/****************************************************************************/
/**
* Write to a specific offset of the PWM
*
* @param pwm is a pointer to an pwm uio map. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the pwm API must be
*		made with this pointer.
*
* @param RegOffset a unsigned 8 bit variable which represents the
* 		register offset of the pwm IP
*
* @return
* 		Returns the contents of the register
*
* @note		None.
*
*****************************************************************************/
uint32_t PWM_ReadReg(PWM pwminst, uint8_t RegOffset)
{
	return ACCESS_REG(pwminst, RegOffset);
}

/****************************************************************************/
/**
* Increase the Duty Cycle of the PWM by a set amount. The max value for the
* duty cycle is (2^8)-1 = 255, this function also makes sure that the set value
* is not exceeded.
*
* @param pwm is a pointer to an pwm uio map. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the pwm API must be
*		made with this pointer.
*
* @param step a unsigned 8 bit variable which represents the
* 		size of the increase for the new value
*
* @return
* 		New duty cycle value
*
* @note		None.
*
*****************************************************************************/
uint32_t PWM_IncreaseDuty(PWM pwminst, uint8_t step)
{
	uint32_t read;

	//read current reg value
	read = PWM_ReadReg(pwminst, PWM_DUTYC_REG);
	//increase by step size
	read += step;
	//make sure that you cannot overflow
	//the maximum value
	if (read > 255)
		read = 255;

	//write the new duty cycle value
	PWM_WriteReg(pwminst, PWM_DUTYC_REG, read);

	//return the new value
	return read;
}

/****************************************************************************/
/**
* Decrease the Duty Cycle of the PWM by a set amount. The min value for the
* duty cycle is 0, this function also makes sure that the set value
* is not exceeded.
*
* @param pwm is a pointer to an pwm uio map. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the pwm API must be
*		made with this pointer.
*
* @param step a unsigned 8 bit variable which represents the
* 		size of the decrease for the new value
*
* @return
* 		New duty cycle value
*
* @note		None.
*
*****************************************************************************/
uint32_t PWM_DecreaseDuty(PWM pwminst, uint8_t step)
{
	uint32_t read;

	//read current reg value
	read = PWM_ReadReg(pwminst, PWM_DUTYC_REG);
	//increase by step size
	read -= step;
	//make sure that you cannot overflow
	//the minimum value
	if (read > 255)
		read = 0;

	//write the new duty cycle value
	PWM_WriteReg(pwminst, PWM_DUTYC_REG, read);

	//return the new value
	return read;
}

/****************************************************************************/
/**
* Increase the clock division factor of the PWM by a set amount. The max value
* for the factor is (2^8)-1 = 255, this function also makes sure that the set value
* is not exceeded. This will influence the PWM frequency as given by the equation
* (fpwm=fck/(256*(Nfckpwm_set+1)), Nfckpwm_set>0
*
* @param pwm is a pointer to an pwm uio map. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the pwm API must be
*		made with this pointer.
*
* @param step a unsigned 8 bit variable which represents the
* 		size of the increase for the new value
*
* @return
* 		New clock division factor
*
* @note		None.
*
*****************************************************************************/
uint32_t PWM_IncreaseNfck(PWM pwminst, uint8_t step)
{
	uint32_t read;

	//read current reg value
	read = PWM_ReadReg(pwminst, PWM_NFCK_REG);
	//increase by step size
	read += step;
	//make sure that you cannot overflow
	//the maximum value
	if (read > 255)
		read = 255;

	//write the new duty cycle value
	PWM_WriteReg(pwminst, PWM_NFCK_REG, read);

	//return the new value
	return read;
}

/****************************************************************************/
/**
* Decrease the clock division factor of the PWM by a set amount. The min value
* for the duty cycle is 0, this function also makes sure that the set value
* is not exceeded. This will influence the PWM frequency as given by the equation
* (fpwm=fck/(256*(Nfckpwm_set+1)), Nfckpwm_set>0
*
* @param pwm is a pointer to an pwm uio map. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the pwm API must be
*		made with this pointer.
*
* @param step a unsigned 8 bit variable which represents the
* 		size of the decrease for the new value
*
* @return
* 		New clock division factor
*
* @note		None.
*
*****************************************************************************/
uint32_t PWM_DecreaseNfck(PWM pwminst, uint8_t step)
{
	uint32_t read;

	//read current reg value
	read = PWM_ReadReg(pwminst, PWM_NFCK_REG);
	//increase by step size
	read -= step;
	//make sure that you cannot overflow
	//the minimum value
	if (read > 255)
		read = 0;

	//write the new duty cycle value
	PWM_WriteReg(pwminst, PWM_NFCK_REG, read);

	//return the new value
	return read;
}

/****************************************************************************/
/**
* Closes and frees all memory associated with the the pwm
*
* @param pwm is a pointer to an pwm uio map. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the pwm API must be
*		made with this pointer.
*
* @return
* 		0 if Close was accomplished properly
 		returns -1 otherwise
*
* @note		None.
*
*****************************************************************************/
uint8_t PWM_Destroy(PWM pwminst)
{
	return UIO_UNMAP(pwminst);
}

