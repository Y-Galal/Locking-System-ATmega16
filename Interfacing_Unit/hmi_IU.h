/*
*   Interfacing Unit - Human-Machine Interfacing (HMI)
*   Made by: Youssef Galal
*   Date : 2-5-2020
*/

#include "micro_config.h"
#include "lcd.h"
#include "keypad.h"
#include "std_types.h"
#include "common_macros.h"
#include "uart.h"
#include <avr/eeprom.h>
#include <string.h>

#define SAVE_PASSWORD 1
#define CHECK_PASSWORD 2
#define OPEN_THE_DOOR 3
#define THIEF 4

void LCD_appInit(void);
void UART_appInit(void);
uint8 checkFirstTime(uint8 savedNumber);
uint8 SysConfig(uint8 *passwordToSend);
void idleMode(void);
void  sendPasswordToSave(uint8 *passwordToSend,uint8 checkOrSave,uint8 *checkOrSaveFlag);
