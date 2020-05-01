 /******************************************************************************
 *
 * Module: Timer0
 *
 * File Name: timer.h
 *
 * Description: Header file for timer0
 *
 * Author: Youssef Galal
 *
 *******************************************************************************/
#ifndef TIMER1_H_
#define TIMER1_H_
#include "std_types.h"
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#define NULL 0
typedef enum TimerMode {NORMAL,COMPARE} Mode;
typedef enum ClockSelect {NOCLK,CLK_1,CLK_8,CLK_64,CLK_256,CLK_1024}Prescalar;
typedef struct TimerConfigurations
{
	Mode s_timerMode;
	Prescalar s_clockSelect;
	uint8 s_initialValue;
	uint8 s_compareValue;
} MyConfigurations;



void TIMER1_init(MyConfigurations *myconfigs_Ptr);

void TIMER1_setCallBack(void (*myFunction_Ptr)(void));

void TIMER1_deInit(void);


#endif /* TIMER1_H_ */
