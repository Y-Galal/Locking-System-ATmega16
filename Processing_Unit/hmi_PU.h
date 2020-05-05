/*
*   Processing Unit - Human-Machine Interfacing (HMI)
*   Headerfile
*   Made by: Youssef Galal
*   Date : 3-5-2020
*/
#ifndef HMI_PU_H_
#define HMI_PU_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
#include <string.h>
#include "external_eeprom.h"
#include "timer1.h"
#include "uart.h"
#include "buzzer.h"
#include "motor.h"


#define SAVE_PASSWORD 1
#define CHECK_PASSWORD 2
#define OPEN_THE_DOOR 3
#define THIEF 4
#define ADDRESS 10
#define NO_MATCH 0
#define START 11
#define REVERSE 12
#define STOP 13
#define READY 55

uint8 compare(uint8 *password1,uint8 *password2);
void motorAndBuzz_appInit(void);
void TIMER_hmiInit(void (*callback)(void));
void UART_appInit(void);
void EEPROM_appInit(void);
void savePassword(void);
void checkPassword(void);
void openTheDoor(void);
void thief(void);
uint8 receiveRequest(void);

#endif