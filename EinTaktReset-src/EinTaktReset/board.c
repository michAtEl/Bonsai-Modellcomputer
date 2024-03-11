/*
 * board.c
 *
 * Created: 26.06.2023 21:34:13
 *  Author: Michael
 */ 

#include <avr/io.h>
#include "board.h"

void clk_off()
{
	PORTB &= ~((1 << CLK_PIN) | (1 << CLK_LED_PIN));
}

void clk_on()
{
	PORTB |= (1 << CLK_PIN) | (1 << CLK_LED_PIN);
}

void enable_cu()
{
	PORTD &= ~(1 << _EN_CU);
}

void disable_cu()
{
	PORTD |= (1 << _EN_CU);
}

void enable_auto()
{
	PORTD &= ~(1 << _AUTO);
}

void disable_auto()
{
	PORTD |= (1 << _AUTO);
}