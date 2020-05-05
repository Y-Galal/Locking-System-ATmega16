/*
*   Interfacing Unit - Human-Machine Interfacing (HMI)
*   Made by: Youssef Galal
*   Date : 2-5-2020
*/
#include "hmi_IU.h"
uint8 tick=0;
MyConfigurations TimerConfigurations,*myConfig_Ptr;


void LCD_appInit(void) 
{
    LCD_init();
    LCD_clearScreen();
    LCD_goToRowColumn(0,0);
    LCD_displayString("Welcome!");
    _delay_ms(100);
}

void TIMER_callBackThief()
{
    tick++;
    LCD_clearScreen();
    LCD_displayStringRowColumn(0,0,"THIEF THIEF");
}

void TIMER_callBack()
{
    tick++;  /*we need 5 overflows*/
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

void UART_appInit(void)
{
    UART_init();
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

void copy(uint8 *passwordToSend,uint8 *password1)
{
    uint8 i;
    for(i=0;i<6;i++)
    {
        passwordToSend[i]=password1[i];
    }
    passwordToSend[6]='#';
    passwordToSend[7]='\0';
}

uint8 SysConfig(uint8 *passwordToSend)  /*writes your password two times and returns a string with the password*/
{
    uint8 password1[6],password2[6],i=0,key;
    LCD_clearScreen();
    LCD_displayStringRowColumn(0,0,"Enter Password");
    LCD_goToRowColumn(1,0);

        for(i=0;i<6;i++)
        {
            password1[i]=Keypad_getPressedKey();
            LCD_displayCharacter('*');
            _delay_ms(50);
        }
    _delay_ms(50);
    LCD_clearScreen();
    LCD_displayStringRowColumn(0,0,"Enter Password");
    LCD_displayStringRowColumn(1,0,"again ");
        for(i=0;i<6;i++)
        {
            password2[i]=Keypad_getPressedKey();
            LCD_displayCharacter('*');
            _delay_ms(50);
        }
    if(compare(password1,password2) == 0)
    {
        copy(passwordToSend,password1);
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
        UART_sendByte(READY);
        UART_sendByte(SAVE_PASSWORD);
        while(UART_receiveByte()!= READY){}
        UART_sendString(passwordToSend);
        while(UART_receiveByte()!=READY){}
        confirmation = UART_receiveByte();
        if(confirmation == SAVE_PASSWORD)
        {
            LCD_clearScreen();
            LCD_displayStringRowColumn(0,0,"Password Saved");
            _delay_ms(100);
        }
    }
    else if (checkOrSave == CHECK_PASSWORD)
    {
        UART_sendByte(READY);
        UART_sendByte(CHECK_PASSWORD);
        while(UART_receiveByte()!= READY){}
        UART_sendString(passwordToSend);
        while(UART_receiveByte()!=READY){}
        confirmation = UART_receiveByte();
    }
    return confirmation;
}

void idleMode(void)
{
    uint8 passwordToSend[8],i,goIdleFlag=0,matchFlag=0,attempt=0;

    LCD_clearScreen();
    LCD_displayStringRowColumn(0,0,"+ Open The Door");
    LCD_displayStringRowColumn(1,0,"-Change Password");
    _delay_ms(100);
    uint8 key = Keypad_getPressedKey();
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
                _delay_ms(50);
            }
            passwordToSend[7]='#';
            passwordToSend[8]='\0';
           matchFlag = sendPassword(passwordToSend,CHECK_PASSWORD);
            if(matchFlag == CHECK_PASSWORD) /*Correct password*/
            {
                UART_sendByte(READY);
                UART_sendByte(OPEN_THE_DOOR);
                LCD_clearScreen();
                LCD_displayStringRowColumn(0,0,"Opening The Door");
                TIMER_hmiInit(TIMER_callBack);
                while(tick !=5){}
                tick = 0;
                TIMER1_deInit();
                LCD_clearScreen();
                LCD_displayStringRowColumn(0,0,"Closing The Door");
                TIMER_hmiInit(TIMER_callBack);
                while(tick !=5){}
                tick = 0;
                TIMER1_deInit();
                goIdleFlag = 1;
            }
            else /*Wrong password, 2 wrong attempts == thief*/
            {
                LCD_clearScreen();
                LCD_displayStringRowColumn(0,0,"Wrong Password");
                _delay_ms(50);
                attempt++;
                if(attempt == 2)
                {
                    UART_sendByte(READY);
                    UART_sendByte(THIEF);
                    TIMER_hmiInit(TIMER_callBackThief);
                    while(tick !=5){}
                    tick = 0;
                    TIMER1_deInit();
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
                _delay_ms(50);
            }
            passwordToSend[7]='#';
            passwordToSend[8]='\0';
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
                _delay_ms(100);
                attempt++;
                if(attempt == 2)
                {
                    UART_sendByte(READY);
                    UART_sendByte(THIEF);
                    TIMER_hmiInit(TIMER_callBackThief);
                    while(tick !=5)
                    {
                        LCD_clearScreen();
                        LCD_displayStringRowColumn(0,0,"THIEF");
                                                
                    }
                    tick = 0;
                    TIMER1_deInit();
                    goIdleFlag = 1;
                }
            }
        }
    }



}