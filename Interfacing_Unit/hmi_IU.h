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

#define SAVE_PASSWORD 1
#define CHECK_PASSWORD 2
#define OPEN_THE_DOOR 3
#define THIEF 4
#define READY 55

void copy(uint8 *passwordToSend,uint8 *password1);
uint8 compare(uint8 *password1,uint8 *password2);
void LCD_appInit(void);
void UART_appInit(void);
uint8 SysConfig(uint8 *passwordToSend);
void idleMode(void);
uint8  sendPassword(uint8 *passwordToSend,uint8 checkOrSave);
void TIMER_hmiInit(void (*callback)(void));
#endif
