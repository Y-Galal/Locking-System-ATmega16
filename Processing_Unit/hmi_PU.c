/*
*   Processing Unit - Human-Machine Interfacing (HMI)
*   Made by: Youssef Galal
*   Date : 3-5-2020
*/
#include "hmi_PU.h"
static uint8 passwordFromIU[7],i,tick=0;
static MyConfigurations TimerConfigurations,*myConfig_Ptr;

uint8 receiveRequest()
{
    uint8 message;
    
    while(UART_receiveByte() != READY){}

    message = UART_receiveByte();
    return message;
}

uint8 compare(uint8 *password1,uint8 *password2)
{
    uint8 i,j=0;
    for(i=0;i<6;i++)
    {
        if(password1[i]==password2[i])
        {
            j++;
        }
    }
    if (j==6)
    {
        return 0;
    }
    else
    {
    return 1;
    }
}

void UART_appInit(void)
{
    UART_init();
}

void TIMER_callBackDoor()
{
    
    tick++;  /*we need 5 overflows*/
}
void TIMER_callBackBuzz()
{
    tick++;
    buzzerToggle();
}

void TIMER_hmiInit(void (*callback)(void))
{

    static uint8 firstTime =1;
    if(firstTime == 1)
    {
    myConfig_Ptr = &TimerConfigurations;
    myConfig_Ptr->s_clockSelect = CLK_256;
    myConfig_Ptr->s_timerMode = NORMAL;
    myConfig_Ptr->s_initialValue = 0x0000;
    firstTime =0;
    sei();
    }
    TIMER1_init(myConfig_Ptr);
    TIMER1_setCallBack(callback);

}

void EEPROM_appInit(void)
{
    EEPROM_init();
}

void savePassword(void)
{
    UART_sendByte(READY);
    UART_receiveString(passwordFromIU);
    for(i=0;i<7;i++)
    {
       
     EEPROM_writeByte(ADDRESS+i, passwordFromIU[i]);
     _delay_ms(5);
    }
    UART_sendByte(READY);
    UART_sendByte(SAVE_PASSWORD);

}

void checkPassword(void)
{
    UART_sendByte(READY);
    UART_receiveString(passwordFromIU);
    uint8 savedPassword[7];
    for(i=0;i<6;i++)
    {
        EEPROM_readByte(ADDRESS+i, &savedPassword[i]);
    }
    if( compare(savedPassword,passwordFromIU) == 0 ) /*matched*/
    {
        UART_sendByte(READY);
        UART_sendByte(CHECK_PASSWORD);
    }
    else
    {
        UART_sendByte(READY);
        UART_sendByte(NO_MATCH);
    }
}

void motorAndBuzz_appInit()
{
    motorInit();
    buzzerInit();
}
void openTheDoor(void)
{
   uint8 state = START;
   while(state != STOP)
   {
       TIMER_hmiInit(TIMER_callBackDoor);
       motor(state);
       while(tick !=5){}
       tick = 0;
       state++;
   }
   TIMER1_deInit();
   motor(STOP);

}

void thief(void)
{
    TIMER_hmiInit(TIMER_callBackBuzz);
    buzzerToggle();
    while(tick!=5){    }
    tick=0;
    buzzerStop();
    TIMER1_deInit();
}