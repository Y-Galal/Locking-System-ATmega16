/*
*   Processing Unit - Main Application
*   Made by: Youssef Galal
*   Date : 3-5-2020
*/
#include "hmi_PU.h"

int main()
{
    uint8 incoming;
    UART_appInit();
    EEPROM_appInit();
    motorAndBuzz_appInit();
    while(1)
    {
        incoming = receiveRequest();
        if(incoming == SAVE_PASSWORD)
        {
            savePassword();
        }
        else if (incoming == CHECK_PASSWORD)
        {
            checkPassword();
        }
        else if (incoming == OPEN_THE_DOOR)
        {
            openTheDoor();
        }
        else if (incoming == THIEF)
        {
            thief();
        }
        
    }
}