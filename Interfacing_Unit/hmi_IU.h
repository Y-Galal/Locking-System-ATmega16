/*
*   Interfacing Unit - Human-Machine Interfacing (HMI)
*   Made by: Youssef Galal
*   Date : 2-5-2020
*/
#ifndef _HMI_IU_H
#define _HMI_IU_H

#include "micro_config.h"
#include "lcd.h"
#include "keypad.h"
#include "std_types.h"
#include "common_macros.h"
#include "uart.h"
#include "external_eeprom.h"
#include <string.h>
#include "timer1.h"

/***************Important definitions************/
/*All of these are considered requests and cofirmations between IU and PU to do.
*If the unit sends them, it's a request 
*and if the unit wants to receive anyone of the, it's a confirmation from the other unit.
*/
#define SAVE_PASSWORD 1     /*Request/confrimation to save the password*/      
#define CHECK_PASSWORD 2    /*Request/confirmation to check the input password is == the password in the PU*/
#define OPEN_THE_DOOR 3     /*Request to open the door*/
#define THIEF 4             /*Request not to open the door, switch on an alarm due to inputting the password wrong 2 times*/
#define READY 55            /*Any micro wants to receive from the other, it must send READY to avoid corrupting the data*/
/***********************************************/
/************Functions prototype***************/


void copy(uint8 *passwordToSend,uint8 *password1); 

uint8 compare(uint8 *password1,uint8 *password2);

void LCD_appInit(void);

void UART_appInit(void);

uint8 SysConfig(uint8 *passwordToSend);

void idleMode(void);

uint8  sendPassword(uint8 *passwordToSend,uint8 checkOrSave);

void TIMER_hmiInit(void (*callback)(void));

void writeByte(uint8 savedNumber);

uint8 checkFirstTime(uint8 savedNumber);


#endif 
