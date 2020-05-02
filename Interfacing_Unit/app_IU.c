/*
*   Interfacing Unit - Main Application
*   Made by: Youssef Galal
*   Date : 2-5-2020
*/
#include "hmi_IU.h"

int main()
{
    uint8 passwordToSend[7];
    LCD_appInit();
    UART_appInit();
    if(checkFirstTime(50) == 1)
    {
        while( SysConfig(passwordToSend) == 0){};
        sendPassword(passwordToSend,SAVE_PASSWORD);
    }    
    
    while(1)
    {
        idleMode();
    }
}