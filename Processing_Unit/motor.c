

#include "motor.h"

void motorInit()
{
    SET_BIT(DDRB,PB6);
    SET_BIT(DDRB,PB7);      /*PB6&PB7  are output pins */
	CLEAR_BIT(PORTB,PB6);
    CLEAR_BIT(PORTB,PB7);   /*Motor is off*/
}

void motor(uint8 condition)
{
    if(condition == START)
    {
        SET_BIT(PORTB,PB7);
	    CLEAR_BIT(PORTB,PB6);        
    }
    else if (condition == REVERSE)
    {
        SET_BIT(PORTB,PB6);
	    CLEAR_BIT(PORTB,PB7);        
    }
    else if (condition == STOP)
    {
     	CLEAR_BIT(PORTB,PB6);
        CLEAR_BIT(PORTB,PB7);   
    }
}