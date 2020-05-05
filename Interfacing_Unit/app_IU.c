/*
*   Interfacing Unit - Main Application
*   Made by: Youssef Galal
*   Date : 2-5-2020
*/
#include "hmi_IU.h"

int main()
{
    uint8 passwordToSend[8];
    LCD_appInit();
    UART_appInit();

    while(1)
    {
        while( SysConfig(passwordToSend) == 0){};
        sendPassword(passwordToSend,SAVE_PASSWORD);
        idleMode();
    }
}