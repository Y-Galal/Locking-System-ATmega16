/*
*   Interfacing Unit - Human-Machine Interface (HMI)
*   Made by: Youssef Galal
*   Date : 2-5-2020
*/
#include "hmi_IU.h"

/*******************Global Variables ****************************/
uint8 tick=0;
MyConfigurations TimerConfigurations,*myConfig_Ptr;
/*************************************************************/

/* Functions declaration*/


/* 
 * Function name: copy
 * Use: to copy the password after writing it 2 times right 
 * to an array of character and puts '#' before '\0' to make it ready to be sent
 * Inputs: 1-Address of a uint8 array "passwordToSend" which will be sent
 *         2-Address of a uint8 array "password1" which has the password
 * outputs: void
 * Made by: Youssef Galal
 */
void copy(uint8 *passwordToSend,uint8 *password1)
{
    uint8 i;
    for(i=0;i<6;i++)
    {
        passwordToSend[i]=password1[i]; /*copying the input password after matching is ok to the array to send*/
    }
    passwordToSend[6]='#';              /*The '#' is used for the UART_sendString to know the end of the string*/
    passwordToSend[7]='\0';
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
        if(password1[i]==password2[i]) /*comparing the two passwords*/
        {
            j++;                    /*flag to tell us how many characters are matched*/
        }
    }
    if (j==6)                   /*if the flag == length of the passowrd then the passwords match*/
    {
        return 0;
    }
    else
    {
    return 1;       /*password not matched*/
    }
}

/* 
 * Function name: LCD_appInit
 * Use: to initiate the LCD
 * Inputs: void
 * outputs: void
 * Made by: Youssef Galal
 */
void LCD_appInit(void) 
{
    LCD_init();         /*initialize the LCD*/
    LCD_clearScreen();  /*clear the screen */
    LCD_goToRowColumn(0,0); /*cursor is on the top left*/
    LCD_displayString("Welcome!");  /*printing Welcome!*/
    _delay_ms(100);             /*delay to be seen by the user*/
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
    UART_init();    /*initialize the UART module*/
}

/* 
 * Function name: SysConfig
 * Use: to handle creating and changing the password, takes it two times then 
 * goes to check them if they are matched returns 1 and the password to send,and if not returns 0
 * Inputs: array of uint8 variables
 * outputs: (1 and the passwordToSend) or 0
 * Made by: Youssef Galal
 */

uint8 SysConfig(uint8 *passwordToSend)  
{
    uint8 password1[6],password2[6],i=0,key;
    LCD_clearScreen(); /*clear the screen to write "enter password"*/
    LCD_displayStringRowColumn(0,0,"Enter Password");
    LCD_goToRowColumn(1,0); /*going to the second line*/

        for(i=0;i<6;i++)
        {
            password1[i]=Keypad_getPressedKey(); /*getting the password form the user*/
            LCD_displayCharacter('*');          /*printing * to tell the user that the character is recorded*/
            _delay_ms(50);                      /*to avoid record one press as 2 characters*/
        }
    _delay_ms(50);
    LCD_clearScreen();                          /*clear the LCD to write "Enter Password Again " */
    LCD_displayStringRowColumn(0,0,"Enter Password");
    LCD_displayStringRowColumn(1,0,"again ");
        for(i=0;i<6;i++)
        {
            password2[i]=Keypad_getPressedKey(); /*getting the password from the user again*/
            LCD_displayCharacter('*');
            _delay_ms(50);
        }
    if(compare(password1,password2) == 0) /*comparing the password*/
    {
        copy(passwordToSend,password1); /* if they are matched we copy it to "passowrdToSend" to be sent */
        return 1;
    }
    else
        {
            LCD_clearScreen();    /*if they are not matched, Lcd prints ERROR NO MATCH*/
            LCD_displayStringRowColumn(0,0,"Error no match"); 
            return 0;
        }
}

/* 
 * Function name: writeByte
 * Use: to write a value in the EEPROM to imply that the system 
 * already has a password after restarting the system
 * Inputs: a number
 * outputs: void
 * Made by: Youssef Galal
 */
void writeByte(uint8 savedNumber)
{
    EEPROM_writeByte(64,savedNumber) ; /*Saving the number in address 64*/
}


/* 
 * Function name: checkFirstTime
 * Use: to check if the system starts for the first time or not
 * If it's the first time, it will ask for a password.
 * If not, the system will go to the idle mode.
 * Inputs: savedNumber to compare with a value saved in EEPROM
 * outputs: 1 if it's the first time, 1 if it's not the first time
 * Made by: Youssef Galal
 */
uint8 checkFirstTime(uint8 savedNumber)
{  
    EEPROM_init();
    uint8 value;
    EEPROM_readByte(64,&value); /*Reading from address 64*/
    if(savedNumber == value)    /*if the value in address 64 == the saved number*/
    {
        return 0;               /*the system isn't starting for the first time (restarting or changing batteries)*/
    }
    else
    { 
        return 1;               /*this is the first time to start the system*/
    }
}

/* 
 * Function name: TIMER_callBackThief
 * Use: if timer1 starts with this function as a callback function,
 * the LCD will print "THIEF THIEF" every time the interrupt fires
 * as the ISR calls it also to count 10 seonds
 * Inputs: void
 * outputs: void
 * Made by: Youssef Galal
 */
void TIMER_callBackThief()
{
    tick++;                 /*counting 5 overflows to get arount 10.5 seconds*/
    LCD_clearScreen();
    LCD_displayStringRowColumn(0,0,"THIEF THIEF");  /*printing "thief thief"*/
}

/* 
 * Function name: TIMER_callBack
 * Use: if timer1 starts with this function as a callback function,
 * every time the interrupt fires, the ISR calls it to count 10 seconds
 * Inputs: void
 * outputs: void
 * Made by: Youssef Galal
 */
void TIMER_callBack()
{
    tick++;  /*we need 5 overflows*/
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
 * Function name: sendPassword
 * Use: to send the password to the processing unit (PU) using USART and 
 *      tell it to check this password or save it
 * Inputs: the password ready to be send, checkOrSave to determine which operation (Checking or Saving)
 * outputs: a confirmation {"SAVE_PASSWORD","CHECK_PASSWORD"}
 * Made by: Youssef Galal
 */
uint8 sendPassword(uint8 *passwordToSend,uint8 checkOrSave) 
{
    uint8 confirmation;
    if(checkOrSave == SAVE_PASSWORD)            /*if we need to save password*/
    {
        UART_sendByte(READY);                   /*send to PU that is waiting for us*/
        UART_sendByte(SAVE_PASSWORD);           /*send him the request SAVE_PASSWORD*/
        while(UART_receiveByte()!= READY){}     /*Waits for him to be ready for the password*/
        UART_sendString(passwordToSend);        /*send him the passwords*/
        while(UART_receiveByte()!=READY){}      /*wait for him to be ready to send me the answer*/
        confirmation = UART_receiveByte();      /*getting the answer from him*/
        if(confirmation == SAVE_PASSWORD)       /*if he replied with SAVE_PASSWORD,this means that he saved the password*/
        {
            LCD_clearScreen();
            LCD_displayStringRowColumn(0,0,"Password Saved");
            _delay_ms(100);
        }
    }
    else if (checkOrSave == CHECK_PASSWORD)     /*if we need to check the input password matches the saved password in PU*/
    {
        UART_sendByte(READY);                   /*send to PU that is waiting for us*/
        UART_sendByte(CHECK_PASSWORD);          /*send him the request CHECK_PASSWORD*/
        while(UART_receiveByte()!= READY){}     /*Waits for him to be ready for the password*/
        UART_sendString(passwordToSend);        /*send him the passwords*/
        while(UART_receiveByte()!=READY){}      /*wait for him to be ready to send me the answer*/
        confirmation = UART_receiveByte();       /*getting the answer from him*/
    }
    return confirmation;
}

/* 
 * Function name: idleMode
 * Use: to wait for a key "+ to open the door" and "- to change password"
 * then asks for a password, if it's right, 
 * the request is granted whether opening the door or changing the password
 * and if it's not right you'll have 1 more attempt. if it's not right again.
 * the system stops for 10 seconds and the LCD prints "THIEF THIEF" and then returns to
 * wait for + or - again.
 * Inputs: void
 * outputs: void
 * Made by: Youssef Galal
 */
void idleMode(void)
{
    uint8 passwordToSend[8],i,goIdleFlag=0,matchFlag=0,attempt=0;

    LCD_clearScreen();
    LCD_displayStringRowColumn(0,0,"+ Open The Door");
    LCD_displayStringRowColumn(1,0,"-Change Password");     /*printing + Open the door - change password*/
    _delay_ms(100);
    uint8 key = Keypad_getPressedKey();                     /*wait for a keypress*/
    if(key == '+') /*open the door*/
    {
        while(goIdleFlag == 0)              
        {
            LCD_clearScreen();
            LCD_displayStringRowColumn(0,0,"Enter Password");
            LCD_goToRowColumn(1,0);
            for(i=0;i<6;i++)        /*getting the password*/
            {
                passwordToSend[i]=Keypad_getPressedKey();
                LCD_displayCharacter('*');
                _delay_ms(50);
            }
            passwordToSend[7]='#';
            passwordToSend[8]='\0';
           matchFlag = sendPassword(passwordToSend,CHECK_PASSWORD); /*sends to PU to check*/
            if(matchFlag == CHECK_PASSWORD) /*Correct password*/
            {
                UART_sendByte(READY);                               /*send PU READY*/
                UART_sendByte(OPEN_THE_DOOR);                       /*send pu to open the door*/
                LCD_clearScreen();                      
                LCD_displayStringRowColumn(0,0,"Opening The Door");     
                TIMER_hmiInit(TIMER_callBack);                      /*starting the timer to count 10 seconds*/
                while(tick !=5){}                                   /*waiting for 10 seconds*/
                tick = 0;
                TIMER1_deInit();                                    /*stopping the timer*/
                LCD_clearScreen();
                LCD_displayStringRowColumn(0,0,"Closing The Door");
                TIMER_hmiInit(TIMER_callBack);                      /*starting the timer againt to count 10 seconds*/
                while(tick !=5){}
                tick = 0;
                TIMER1_deInit();
                goIdleFlag = 1;                                     /*leaving the loop*/
            }
            else /*Wrong password, 2 wrong attempts == thief*/
            {
                LCD_clearScreen();
                LCD_displayStringRowColumn(0,0,"Wrong Password");
                _delay_ms(50);
                attempt++;                  
                if(attempt == 2)            /*if this condition is ture, it's a thief*/
                {
                    UART_sendByte(READY);       /*sends pu ready*/
                    UART_sendByte(THIEF);       /*send pu that this is a thief*/
                    TIMER_hmiInit(TIMER_callBackThief); /*starting the timer for 10 seconds to freeze the system*/
                    while(tick !=5){}
                    tick = 0;
                    TIMER1_deInit();                /*stopping the timer*/
                    goIdleFlag = 1;                 /*getting out of this loop*/
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
            for(i=0;i<6;i++)        /*getting the old password*/
            {
                passwordToSend[i]=Keypad_getPressedKey();
                LCD_displayCharacter('*');
                _delay_ms(50);
            }
            passwordToSend[7]='#';
            passwordToSend[8]='\0';
           matchFlag = sendPassword(passwordToSend,CHECK_PASSWORD); /*send it to check*/ 
            if(matchFlag == CHECK_PASSWORD) /*Correct old password, change request granted */
            {
                while(!( SysConfig(passwordToSend) ) ){}    /*getting the new password*/
                sendPassword(passwordToSend,SAVE_PASSWORD); /*saving it*/
                goIdleFlag = 1;                             /*getting out from the loop*/
            }
            else /*Wrong password, 2 wrong attempts == thief*/
            {
                LCD_clearScreen();
                LCD_displayStringRowColumn(0,0,"Wrong Password");
                _delay_ms(100);
                attempt++;
                if(attempt == 2)        /*if this condition is ture, we have a thief*/
                {
                    UART_sendByte(READY);       /*sends pu ready*/
                    UART_sendByte(THIEF);       /*send pu thief request*/
                    TIMER_hmiInit(TIMER_callBackThief); /*counting 10 seconds of freezing the system*/
                    while(tick !=5){}
                    tick = 0;
                    TIMER1_deInit();        /*Stopping the timer*/
                    goIdleFlag = 1;         /*getting out from the loop*/
                }
            }
        }
    }



}