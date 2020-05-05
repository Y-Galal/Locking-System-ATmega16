/*
*   Processing Unit - Human-Machine Interfacing (HMI)
*   Made by: Youssef Galal
*   Date : 3-5-2020
*/
#include "hmi_PU.h"
/***********Global Variables****************/
static uint8 passwordFromIU[7],i,tick=0;
static MyConfigurations TimerConfigurations,*myConfig_Ptr;


/* 
 * Function name: receiveRequest
 * Use: get a message from IU
 * Inputs: void
 * outputs: the message
 * Made by: Youssef Galal
 */
uint8 receiveRequest()
{
    uint8 message;
    
    while(UART_receiveByte() != READY){}    /*waiting for IU to send READY*/

    message = UART_receiveByte();           /*receiving the request*/
    return message;
}

/* 
 * Function name: compare
 * Use: to compare two passwords after writing the password 2 times 
 * Inputs: 1-Address of a uint8 array "password1" (the first input)
 *         2-Address of a uint8 array "password1" (the 2nd input)
 * outputs: 0 -> if the passwords are matched 
 *          1 -> if the passwords are not matched
 * Made by: Youssef Galal
 */
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

/* 
 * Function name: UART_appInit
 * Use: to initiate the UART module
 * Inputs: void
 * outputs: void
 * Made by: Youssef Galal
 */
void UART_appInit(void)
{
    UART_init();
}

/* 
 * Function name: TIMER_callBackDoor
 * Use: if timer1 starts with this function as a callback function,
 * every time the interrupt fires, the ISR calls it to count 10 seconds
 * Inputs: void
 * outputs: void
 * Made by: Youssef Galal
 */
void TIMER_callBackDoor()
{
    
    tick++;  /*we need 5 overflows*/
}

/* 
 * Function name: TIMER_callBackBuzz
 * Use: if timer1 starts with this function as a callback function,
 * buzzer will toggle every time the interrupt fires
 * as the ISR calls it also to count 10 seonds
 * Inputs: void
 * outputs: void
 * Made by: Youssef Galal
 */
void TIMER_callBackBuzz()
{
    tick++;         /*counting the overflows (5) to get 10.5 seconds*/
    buzzerToggle(); /*Toggle the buzzer*/
}

/* 
 * Function name: TIMER_hmiInit
 * Use: Starts timer1 with configurations 
 * passed from the global pointer to structure declared above
 * Inputs: the call back function to be called in the ISR (TIMER_callBack OR TIMER_callBackThief)
 * outputs: void
 * Made by: Youssef Galal
 */
void TIMER_hmiInit(void (*callback)(void))
{

    static uint8 firstTime =1; /*if this is the first time to call this function,
                                 the if block executes and gives timer1 the configurations needed*/
    if(firstTime == 1)
    {
    myConfig_Ptr = &TimerConfigurations;        /*pointer to configurations structure*/
    myConfig_Ptr->s_clockSelect = CLK_256;      /*prescalar 256*/
    myConfig_Ptr->s_timerMode = NORMAL;         /*Normal mode (overflow)*/
    myConfig_Ptr->s_initialValue = 0x0000;      /*initial value of the counter register=0*/
    firstTime =0;                               /*changing it bc we don't need this anymore 
                                                "the values are already in the structure"*/
    sei();                                      /*enabling the I-bit*/
    }
    TIMER1_init(myConfig_Ptr);                  /*passing the pointer that handles the configurations */
    TIMER1_setCallBack(callback);               /*setting the callback function*/

}

/* 
 * Function name: EEPROM_appInit
 * Use: initiating the EEPROM
 * Inputs: void
 * outputs: void
 * Made by: Youssef Galal
 */
void EEPROM_appInit(void)
{
    EEPROM_init();
}

/* 
 * Function name: savePassword
 * Use: receives the password and writes it in the EEPROM
 * Inputs: void
 * outputs: void
 * Made by: Youssef Galal
 */
void savePassword(void)
{
    UART_sendByte(READY);                   /*sending ready to IU*/
    UART_receiveString(passwordFromIU);     /*receiving the password from IU*/
    for(i=0;i<7;i++)
    {
       
     EEPROM_writeByte(ADDRESS+i, passwordFromIU[i]);        /*storing it in the EEPROM*/
     _delay_ms(5);
    }
    UART_sendByte(READY);                                   /*sending ready to IU*/
    UART_sendByte(SAVE_PASSWORD);                           /*sending the answer to IU*/

}

/* 
 * Function name: checkPassword
 * Use: checking if the password matches the saved one in the EEPROM and sending a reply to IU
 * Inputs: void
 * outputs: void
 * Made by: Youssef Galal
 */
void checkPassword(void)
{
    UART_sendByte(READY);               /*send him ready*/            
    UART_receiveString(passwordFromIU); /*getting the password*/
    uint8 savedPassword[7];
    for(i=0;i<6;i++)
    {
        EEPROM_readByte(ADDRESS+i, &savedPassword[i]);  /*reading the saved password*/
    }
    if( compare(savedPassword,passwordFromIU) == 0 ) /*comparing the received password with the stored one*/
    {
        UART_sendByte(READY);               /*if this condition is true then we will send him ready*/
        UART_sendByte(CHECK_PASSWORD);      /*and CHECK_PASSWORD. this implies that the password is correct*/
    }
    else
    {
        UART_sendByte(READY);               /*password didn't match*/
        UART_sendByte(NO_MATCH);            /*sending ready then NO_MATCH */
    }
}

/* 
 * Function name: motorAndBuzz_appInit
 * Use: initiating the motor and the buzzer
 * Inputs: void
 * outputs: void
 * Made by: Youssef Galal
 */
void motorAndBuzz_appInit()
{
    motorInit();
    buzzerInit();
}

/* 
 * Function name: openTheDoor
 * Use: opening the door for 10 seconds then closing it for 10 seconds
 * Inputs: void
 * outputs: void
 * Made by: Youssef Galal
 */
void openTheDoor(void)
{
   uint8 state = START;
   while(state != STOP)
   {
       TIMER_hmiInit(TIMER_callBackDoor);       /*timer1 counts for 10 seconds*/
       motor(state);                            /*motor(start) to start the motor*/
       while(tick !=5){}                        /*waiting for 10 seconds*/
       tick = 0;                                /*resetting the counter*/
       state++;                                 /*state now is REVERSE, looping again and state becomes*/
                                                /* STOP then we leave this loop*/
   }
   TIMER1_deInit();                             /*stopping the timer*/
   motor(STOP);                                 /*stopping the motor*/

}

/* 
 * Function name: thief
 * Use: toggle the buzzer 10 seconds because we have a thief
 * Inputs: void
 * outputs: void
 * Made by: Youssef Galal
 */
void thief(void)
{
    TIMER_hmiInit(TIMER_callBackBuzz);  /*starting the timer for 10 seconds*/
    buzzerToggle();                     /*starting the buzzer then the isr handles toggling*/
    while(tick!=5){    }                /*wait for 10 seconds*/
    tick=0;                             /*resetting the counter*/
    buzzerStop();                       /*stopping the buzzer in case the last ISR leaves it open */
    TIMER1_deInit();                    /*stopping the timer*/
}