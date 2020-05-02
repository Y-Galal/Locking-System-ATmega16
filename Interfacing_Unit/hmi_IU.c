/*
*   Interfacing Unit - Human-Machine Interfacing (HMI)
*   Made by: Youssef Galal
*   Date : 2-5-2020
*/
#include "hmi_IU.h"


void LCD_appInit(void) 
{
    LCD_init();
    LCD_clearScreen();
    LCD_goToRowColumn(0,0);
    LCD_displayString("Welcome!");
}

void UART_appInit(void)
{
    UART_init();
}

uint8 checkFirstTime(uint8 savedNumber)
{  
    uint8 value = eeprom_read_byte ((const uint8*)64); /*Reading from address 64*/
    if(savedNumber == value)
    {
        return 0;
    }
    else
    { 
      eeprom_write_byte (( uint8 *) 64, savedNumber ); /*Saving the number in address 64*/
      return 1;
    }
}

uint8 SysConfig(uint8 *passwordToSend)  /*writes your password two times and returns a string with the password*/
{
    uint8 password1[7],password2[7],i;
    LCD_clearScreen();
    LCD_displayStringRowColumn(0,0,"Enter Password");
    LCD_goToRowColumn(1,0);

    for(i=0;i<6;i++)
    {
        password1[i] = Keypad_getPressedKey();
        LCD_displayCharacter('*');
        _delay_ms(500);
    }  
    password1[i]= '\0';
    LCD_clearScreen();
    LCD_displayStringRowColumn(0,0,"Enter Password");
    LCD_displayStringRowColumn(1,0,"again");
    for(i=0;i<6;i++)
    {
        password2[i] = Keypad_getPressedKey();
        LCD_displayCharacter('*');
        _delay_ms(500);
    }
    password2[i]='\0';
    if(strcmp(password1,password2) == 0)
    {
        strcpy(passwordToSend,password1);
        return 1;
    }
    else
        {
            LCD_clearScreen();
            LCD_displayStringRowColumn(0,0,"Error no match");
            return 0;
        }
}

uint8 sendPassword(uint8 *passwordToSend,uint8 checkOrSave) /*takes the password and sends it to Processing unit to save*/
{
    uint8 confirmation;
    if(checkOrSave == SAVE_PASSWORD)
    {
        UART_sendByte(SAVE_PASSWORD);
        _delay_ms(20);
        UART_sendString(passwordToSend);
        _delay_ms(20);
        confirmation = UART_receiveByte();
        if(confirmation == SAVE_PASSWORD)
        {
            LCD_clearScreen();
            LCD_displayStringRowColumn(0,0,"Password Saved");
        }
    }
    else if (checkOrSave == CHECK_PASSWORD)
    {
        UART_sendByte(CHECK_PASSWORD);
        _delay_ms(20);
        UART_sendString(passwordToSend);
        _delay_ms(20);
        confirmation = UART_receiveByte();
    }
    return confirmation;
}

void idleMode(void)
{
    LCD_clearScreen();
    LCD_displayStringRowColumn(0,0,"+ Open The Door");
    LCD_displayStringRowColumn(1,0,"-Change Password");
    uint8 key = Keypad_getPressedKey();
    uint8 passwordToSend[7],i,goIdleFlag=0,matchFlag=0,attempt=0;
    if(key == '+') /*open the door*/
    {
        while(goIdleFlag == 0)
        {
            LCD_clearScreen();
            LCD_displayStringRowColumn(0,0,"Enter Password");
            LCD_goToRowColumn(1,0);
            for(i=0;i<6;i++)
            {
                passwordToSend[i]=Keypad_getPressedKey();
                LCD_displayCharacter('*');
                _delay_ms(500);
            }
           matchFlag = sendPassword(passwordToSend,CHECK_PASSWORD);
            if(matchFlag == CHECK_PASSWORD) /*Correct password*/
            {
                UART_sendByte(OPEN_THE_DOOR);
                LCD_clearScreen();
                LCD_displayStringRowColumn(0,0,"Opening The Door");
                _delay_ms(10000);
                LCD_clearScreen();
                LCD_displayStringRowColumn(0,0,"Closing The Door");
                _delay_ms(10000);
                goIdleFlag = 1;
            }
            else /*Wrong password, 2 wrong attempts == thief*/
            {
                LCD_clearScreen();
                LCD_displayStringRowColumn(0,0,"Wrong Password");
                _delay_ms(1000);
                attempt++;
                if(attempt == 2)
                {
                    UART_sendByte(THIEF);
                    LCD_clearScreen();
                    LCD_displayStringRowColumn(0,0,"THIEF THIEF");
                    _delay_ms(10000);
                    goIdleFlag = 1;
                }
            }
        }

    }
    else if(key == '-') /*change password*/
    {
        goIdleFlag=0;
        attempt=0;
        while(goIdleFlag == 0)
        {
            LCD_clearScreen();
            LCD_displayStringRowColumn(0,0,"EnterOldPassword");
            LCD_goToRowColumn(1,0);
            for(i=0;i<6;i++)
            {
                passwordToSend[i]=Keypad_getPressedKey();
                LCD_displayCharacter('*');
                _delay_ms(500);
            }
           matchFlag = sendPassword(passwordToSend,CHECK_PASSWORD);
            if(matchFlag == CHECK_PASSWORD) /*Correct old password, change request granted */
            {
                while(!( SysConfig(passwordToSend) ) ){}
                sendPassword(passwordToSend,SAVE_PASSWORD);
                goIdleFlag = 1;
            }
            else /*Wrong password, 2 wrong attempts == thief*/
            {
                LCD_clearScreen();
                LCD_displayStringRowColumn(0,0,"Wrong Password");
                _delay_ms(1000);
                attempt++;
                if(attempt == 2)
                {
                    UART_sendByte(THIEF);
                    LCD_clearScreen();
                    LCD_displayStringRowColumn(0,0,"THIEF THIEF");
                    _delay_ms(10000);
                    goIdleFlag = 1;
                }
            }
        }
    }



}