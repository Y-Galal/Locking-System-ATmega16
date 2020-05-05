/*
*   Interfacing Unit - Main Application
*   Made by: Youssef Galal
*   Date : 2-5-2020
*/
#include "hmi_IU.h"

int main()
{
    uint8 passwordToSend[8],firstTimeFlag;
    LCD_appInit();                          /*Initiating the LCD*/
    UART_appInit();                         /*Initiating the UART*/
    firstTimeFlag = checkFirstTime(50);     /*checking if it's the first time to operate the system or not*/
    if( firstTimeFlag == 1)
    {
        while( SysConfig(passwordToSend) == 0){}; /*getting the password 2 times form the user and check matching*/
        sendPassword(passwordToSend,SAVE_PASSWORD); /*sending the password to PU to save it*/
        writeByte(50);                      /*write 50 to a fixed address to imply that 
                                            the system has a password if we restart the system*/
    }   
    while(1)
    {
        idleMode();
    }
}
