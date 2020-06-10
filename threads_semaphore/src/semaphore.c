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

/*
 * semaphore.c: simple test application using threads and System V semaphores in petalinux
 *
 * This application generates NO_OF_THREADS threads, which all execute a simple barrier function,
 * which lets only BARRIER_MAX threads running at the same time in the critical area. The function also controls
 * the red and green leds using a PWM as follows:
 *
 * If the barrier is full, i.e. there are BARRIER_MAX threads in the critical area,
 * the color of the RGB leds will be RED.
 *
 * If the barrier is not full, but there are still threads running in the critical area,
 * the color of the RGB leds will be YELLOW.
 *
 * If the barrier is empty, i.e. there are no threads running in the critical area,
 * the color of the RGB leds will be GREEN.
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
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <pthread.h>
#include "pwm.h"

#define BARRIER_MAX 10 // max number of threads that can run in the critical area at the same time
#define NO_OF_THREADS 100 // total number of threads

PWM red, green; // red and green LED structs
int semId; // semaphore id
int th_count; // number of threads in the critical area

typedef struct {
	int waitTime; // the number of seconds that the thread spends in the critical area
    int id; // thread id
}thArg;

void create_threads(pthread_t* th, int th_size, void* start_func){
    thArg* args = malloc(th_size*sizeof(thArg));
    for(int i = 1; i <= th_size; i++){
        args[i-1].id = i;
        args[i-1].waitTime = rand() % 5;
        pthread_create(&th[i-1], NULL, start_func, (void*)&args[i-1]);
    }
}

void end_threads(pthread_t* th, int th_size){
    for(int i = th_size; i > 0; i--){
        pthread_join(th[i-1], NULL);
    }
}

void P(int semId, int semNr)
{
    struct sembuf op = {semNr, -1, 0};
    semop(semId, &op, 1);
}

void V(int semId, int semNr)
{
    struct sembuf op = {semNr, 1, 0};
    semop(semId, &op, 1);
}

void* waitAtBarrier(void* arg){
    thArg* args = arg;
    int id = args->id;
    int waitTime = args->waitTime;

    P(semId, 0);
    // entering critical area
    PWM_WriteReg(red,PWM_DUTYC_REG,120);
    th_count++;
    if(th_count == BARRIER_MAX){
        PWM_WriteReg(green,PWM_DUTYC_REG, 0); // remove green => only red
    }
    sleep(waitTime);
    // leaving critical area
    V(semId, 0);
    PWM_WriteReg(green, PWM_DUTYC_REG, 40);
    th_count--;
    if(th_count == 0){
        PWM_WriteReg(red, PWM_DUTYC_REG, 0); // remove red => only green
    }

    return NULL;
}

int main()
{
    pthread_t th[NO_OF_THREADS];

	semId = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    semctl(semId, 0, SETVAL, BARRIER_MAX); // set semaphore to allow only BARRIER_MAX threads at the same time

    // initialize PWM generator for the red LED
    red = PWM_Initialize(0,0);
    PWM_WriteReg(red,PWM_NFCK_REG, 2);
    PWM_WriteReg(red,PWM_DUTYC_REG, 0);

    // initialize PWM generator for the green LED
    green = PWM_Initialize(1,0);
    PWM_WriteReg(green,PWM_NFCK_REG,2);
    PWM_WriteReg(green,PWM_DUTYC_REG,110);

    // create NO_OF_THREADS threads
    create_threads(th, NO_OF_THREADS, waitAtBarrier);

    end_threads(th, NO_OF_THREADS);

    PWM_WriteReg(red,PWM_DUTYC_REG,0);

    PWM_Destroy(red);
    PWM_Destroy(green);
    return 0;
}
