 /******************************************************************************
 *
 * Module: Timer0
 *
 * File Name:timer.c
 *
 * Description: Source file for timer0
 *
 * Author: Youssef Galal
 *
 *******************************************************************************/
#include "timer1.h"

void (*callBack_Ptr)(void) = NULL;


/*Interrupt service routine if we are using Overflow mode*/
ISR(TIMER1_OVF_vect)
{
	/*If there is an error setting the callback function,Nothing will happen when the interrupt occurs*/
	if(callBack_Ptr != NULL)
	(*callBack_Ptr)();
}

/*Interrupt service routine if we are using compare mode*/
ISR(TIMER1_COMPA_vect)
{
	/*If there is an error setting the callback function,Nothing will happen when the interrupt occurs*/
	if(callBack_Ptr != NULL)
	(*callBack_Ptr)();
}



/*This Function initiates the timer using configurations given by the user (Mode,Prescalar,Initial Value,Compare Value)*/
void TIMER1_init(MyConfigurations *myConfigs_Ptr)
{
	/*FOC1A bit is set when we use non-pwm mode
	 * WGM10,WGM11 are set to 0 for compare and normal modes
	 */
	TCCR1A = (1<<FOC1A);

	/*WGM12 bit is set when we use compare mode and cleared when we use normal mode*/
	TCCR1B = (TCCR1B & 0xF7 ) | ( (myConfigs_Ptr->s_timerMode) << 3 );

	/*Prescaler selection is done by setting the bits CS12 CS11 CS10 in TCCR1B register*/
	TCCR1B = (TCCR1B & 0xF8) | ( (myConfigs_Ptr->s_clockSelect) & 0x07 );

	/*Setting the initial value in the register*/
	TCNT1 = (uint16) myConfigs_Ptr->s_initialValue;

	/*Setting the compare value in the register.*/
	if( myConfigs_Ptr->s_timerMode )
	OCR1A = (uint16) myConfigs_Ptr->s_compareValue;


	/*Interrupt Enable */
	/*If we are using Compare mode we set OCIE1A*/
	if( myConfigs_Ptr->s_timerMode )
	{
		TIMSK |= (1<<OCIE1A);
	}
	else
		/*If we are using Overflow mode we set TOIE1*/
		TIMSK |= (1<<TOIE1) ;
}

/*This function sets the function to be called when the timer produces interrupt*/

void TIMER1_setCallBack(void (*myFunction_Ptr)(void))
{
	callBack_Ptr= myFunction_Ptr ;
	/*our function address is stored in pointer to function which will be called in ISR*/
}



void TIMER1_deInit(void)
{

	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	OCR1A = 0;
	TIMSK &= ~(1<<OCIE1A);
	TIMSK &= ~(1<<TOIE1);
}




