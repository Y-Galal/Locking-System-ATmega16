/*
 * lcd.h
 *
 *  Created on: Sep 15, 2019
 *      Author: Youssef Galal
 */

#ifndef LCD_H_
#define LCD_H_
/********************************************************
 *                    Includes                          *
 ********************************************************/

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
#include <stdlib.h>
/********************************************************
 *                       Macros                         *
 ********************************************************/
#define DATA_BITS_MODE 8
#if (DATA_BITS_MODE == 4)
#define UPPER_PORT_PINS
#endif

/* Hw Pins */
#define RS PB0
#define RW PB1
#define E PB2

#define LCD_CTRL_PORT PORTB
#define LCD_CTRL_PORT_DIR DDRB

#define LCD_DATA_PORT PORTA
#define LCD_DATA_PORT_DIR DDRA

/* LCD Commands */
#define CLEAR_COMMAND 0x01
#define FOUR_BITS_DATA_MODE 0x02
#define TWO_LINE_LCD_FOUR_BIT_MODE 0x28
#define TWO_LINE_LCD_EIGHT_BIT_MODE 0x38
#define CURSOR_ON 0x0E
#define CURSOR_OFF 0x0C
#define SET_CURSOR_LOCATION 0x80



/********************************************************
 *                   Function prototypes                *
 ********************************************************/
void LCD_init(void);
void LCD_sendCommand(uint8 command);
void LCD_displayCharacter(uint8 data);
void LCD_displayString(const char *Str);
void LCD_goToRowColumn(uint8 row,uint8 col);
void LCD_displayStringRowColumn(uint8 row,uint8 col, const char *Str);
void LCD_clearScreen(void);
void LCD_intgerToString(uint8 data);

#endif /* LCD_H_ */
