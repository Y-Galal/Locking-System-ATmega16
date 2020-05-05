 /******************************************************************************
 *
 * Module: Micro - Configuration
 *
 * File Name: Micro_Config.h
 *
 * Description: File include all Microcontroller libraries
 *
 * Made By: Youssef Galal
 *******************************************************************************/

#ifndef MICRO_CONFIG_H_
#define MICRO_CONFIG_H_


#ifndef F_CPU
#define F_CPU 8000000UL /* 8MHz Clock frequency */
#endif

#ifndef  __AVR_ATmega16__
#define __AVR_ATmega16__   /* MMCU Model */
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#endif  /* MICRO_CONFIG_H_ */
