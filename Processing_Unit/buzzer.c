
#include "buzzer.h"

void buzzerInit()
{
    SET_BIT(DDRD,PD6); /*PD6 output pin*/
    CLEAR_BIT(PORTD,PD6); /*buzzer is off*/
}
void buzzerToggle()
{
    TOGGLE_BIT(PORTD,PD6);
}
void buzzerStop()
{
    CLEAR_BIT(PORTD,PD6);
}