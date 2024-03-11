/*
 * EinTaktReset.c
 *
 * Created: 23.06.2023 16:48:30
 * Author : Michael
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "board.h"

#define F_CPU 1000000UL  // 1 MHz
#include <util/delay.h>

struct debounce_state
{
	uint8_t debounced_state;
	uint8_t counter;
	uint8_t changeDetected;
};


typedef struct debounce_state Debounce_state;

volatile Debounce_state CLK_Speed_debounce_state;
volatile Debounce_state CLK_auto_ON_OFF_debounce_state;
volatile Debounce_state CLK_manual_debounce_state;
volatile Debounce_state CU_ON_OFF_debounce_state;
volatile Debounce_state RESET_debounce_state;



void debounce(volatile Debounce_state *state, uint8_t pin)
{
	uint8_t valueNow = GET_PIN(PINC, pin);

	if (valueNow != state->debounced_state)
	{
		state->counter ++;
		if (state->counter >= 1)
		{
			state->counter = 0;
			state->changeDetected = 1;
			state->debounced_state = valueNow;
		}
		} else {
		state->counter = 0;
	}
}

void debounce_all()
{
	debounce(&CLK_Speed_debounce_state, CLK_Speed);
	debounce(&CLK_auto_ON_OFF_debounce_state, CLK_auto_ON_OFF);
	debounce(&CLK_manual_debounce_state, CLK_manual);
	debounce(&CU_ON_OFF_debounce_state, CU_ON_OFF);
	debounce(&RESET_debounce_state, RESET);
}


void init_bonsai()
{
	
}

void starte_automatischen_takt()
{
	if (TCCR1B & ((1 << CS12) | (1 << CS11) | (1 << CS10))) {
		// Der Timer läuft bereits
	} else {
		// Der Timer läuft nicht
		// Timer starten
		if (CLK_Speed_debounce_state.debounced_state) {
			// schnell
			OCR1A = 5;
			} else {
			// langsam
			OCR1A = 1000;
		}
		
		TCNT1 = 0;
		TCCR1B |= (1 << CS12) | (1 << CS10);
	}
}

void stoppe_automatischen_takt()
{
	TCCR1B &= ~(1 << CS12 |1 << CS11 | 1 << CS10);
	//PORTB &= ~((1 << CLK_PIN) | (1 << CLK_LED_PIN));
}

// ca. f = 1kHz
// ISR (TIMER0_COMPA_vect)
void tick_loop()
{
	//debounce(&CLK_Speed_debounce_state, CLK_Speed);
	//debounce(&CLK_auto_ON_OFF_debounce_state, CLK_auto_ON_OFF);
	//debounce(&CLK_manual_debounce_state, CLK_manual);
	//debounce(&CU_ON_OFF_debounce_state, CU_ON_OFF);
	//debounce(&RESET_debounce_state, RESET);
	debounce_all();
	
	if (RESET_debounce_state.debounced_state == 0)
	{
		// Reset-Button ist im Moment gedrückt
		stoppe_automatischen_takt();
		
		// Signale in den Reset-Zustand bringen
		// Pins konfigurieren
		// DDR: 0: Eingang
		//      1: Ausgang


				
		PORTD &= ~( (1 << _CLR_MPC) | (1 << _WRITE_ACCU) | (1 << _READ_IR) );

		PORTD |= (1 << CLR_PC) | (1 << CLR_AKKU) | (1 << _EN_DEC) | (1 << _EN_CU);	

		DDRD |= (1 << _CLR_MPC) | (1 << _WRITE_ACCU) | (1 << _READ_IR) | (1 << CLR_PC) | (1 << CLR_AKKU);
		
		// Takt einschalten, damit die FlipFlops im Steuerwerk zurückgesetzt werden
		PORTB |= (1 << CLK_PIN) | (1 << CLK_LED_PIN);
				
		// Abarbeitung der ISR abbrechen
		return;
	}
	
	// der Reset-Button ist im Moment nicht gedrückt
	// Signale aus dem Reset-Zustand herausnehmen
	DDRD &= ~((1 << _CLR_MPC) | (1 << _WRITE_ACCU) | (1 << _READ_IR) | (1 << CLR_PC) | (1 << CLR_AKKU));
	// Pull-Ups abschalten
	PORTD &= ~( (1 << _CLR_MPC) | (1 << _WRITE_ACCU) | (1 << _READ_IR) | (1 << CLR_PC) | (1 << CLR_AKKU) );
	
	if (CU_ON_OFF_debounce_state.debounced_state == 0) {
		// Steuerwerk
		PORTD &= ~((1 << _EN_DEC) | (1 << _EN_CU) | (1 << _AUTO));
	} else {
		// Hand
		PORTD |= (1 << _EN_DEC) | (1 << _EN_CU) | (1 << _AUTO);
		
	}
	
	if (CLK_auto_ON_OFF_debounce_state.debounced_state == 0) {
		// automatischer Timer aus
		stoppe_automatischen_takt();
		if (CLK_manual_debounce_state.debounced_state == 1)
		{
			PORTB &= ~((1 << CLK_PIN) | (1 << CLK_LED_PIN));
		}
		else
		{
			PORTB |= (1 << CLK_PIN) | (1 << CLK_LED_PIN);
		}
	} else {
		starte_automatischen_takt();
	}
	
	//if (debounce_changeDetected(&RESET_debounce_state))
	//{
		//// der Reset-Button wurde gerade losgelassen
		//
		//// Signale aus dem Reset-Zustand herausnehmen
		//PIND &= ~((1 << _CLR_MPC) | (1 << _WRITE_ACCU) | (1 << _READ_IR) | (1 << CLR_PC) | (1 << CLR_AKKU));
//
		//if (CU_ON_OFF_debounce_state.debounced_state == 1) {
			//
		//}
		//
		//if (CLK_auto_ON_OFF_debounce_state.debounced_state == 1) {
			//// der automatische Takt ist an
			//starte_automatischen_takt();
		//}
		//
		//return;
	//}

	//if (debounce_changeDetected(&CLK_auto_ON_OFF_debounce_state))
	//{
		//if (CLK_auto_ON_OFF_debounce_state.debounced_state)
		//{
			//// der automatische Takt wurde gerade eingeschaltet
			//starte_automatischen_takt();
		//}
		//else
		//{
			//// der automatische Takt wurde gerade ausgeschaltet
			//stoppe_automatischen_takt();
		//}
	//}
	//
	//if (CLK_auto_ON_OFF_debounce_state.debounced_state == 1)
	//{
		//// der automatische Takt ist an
		//} else {
		//// der automatische Takt ist aus
		////
		//if (CLK_manual_debounce_state.debounced_state == 1)
		//{
			//PORTB = 0xff;
		//}
		//else
		//{
			//PORTB = 0;
		//}
	//}
}

int main(void)
{
	// Pins konfigurieren
	// DDR: 0: Eingang
	//      1: Ausgang

	// Pins für den Bonsai-Systamtakt als Ausgänge
	DDRB |= (1 << CLK_PIN) | (1 << CLK_LED_PIN);
	
	// Pins für die Schalter und Taster als Eingänge
	DDRC &= ~((1 < CLK_Speed) | (1 < CLK_auto_ON_OFF) | (1 < CLK_manual) | (1 < CU_ON_OFF) | (1 < RESET));
	
	// PORTC: Pullups aktivieren
	PORTC = 0xff;
	
	// Pins für _Auto, _EN_CU und _EN_DEC als Ausgänge
	DDRD |= (1 << _AUTO) | (1 << _EN_CU) | (1 << _EN_DEC);

	// Structs für Entprellung initialiseren
	CLK_Speed_debounce_state.debounced_state = GET_PIN(PINC, CLK_Speed);
	CLK_Speed_debounce_state.counter = 0;
	CLK_Speed_debounce_state.changeDetected = 0;
		
	CLK_auto_ON_OFF_debounce_state.debounced_state = GET_PIN(PINC, CLK_auto_ON_OFF);
	CLK_auto_ON_OFF_debounce_state.counter = 0;
	CLK_auto_ON_OFF_debounce_state.changeDetected = 0;
	
	CLK_manual_debounce_state.debounced_state = GET_PIN(PINC, CLK_manual);
	CLK_manual_debounce_state.counter = 0;
	CLK_manual_debounce_state.changeDetected = 0;
	
	CU_ON_OFF_debounce_state.debounced_state = GET_PIN(PINC, CU_ON_OFF);
	CU_ON_OFF_debounce_state.counter = 0;
	CU_ON_OFF_debounce_state.changeDetected = 0;
	
	RESET_debounce_state.debounced_state = GET_PIN(PINC, RESET)
	RESET_debounce_state.counter = 0;
	RESET_debounce_state.changeDetected = 0;

	// Timer für den automatischen Takt des Bonsai konfigurieren
	// CTC, Top OCR1A, normal Port Operation
	// WGM1 = 0100
	// Clock Select: clk/1024 (from prescaler)
	// Interrupt OutputCompare A aktivieren
	TCCR1B |= (1 << WGM12);	
	TIMSK1 |= (1 << OCIE1A);

	OCR1A = 1000;
	
	//if (CLK_auto_ON_OFF_debounce_state.debounced_state) {
		//// automatischen Takt einschalten
		//starte_automatischen_takt();
	//}
	
	
	// Timer für den Systemtakt des ATmega konfigurieren
	// CTC, TOP OCRA , normal Port Operation
	// WGM = 010
	// Clock Select: clk/256 (from prescaler)
	//TCCR0A |= (1 << WGM01);
	//TCCR0B |= (1 << CS02);
	//TIMSK0 |= (1 << OCIE0A);	
	//OCR0A = 4;
	
	// ------------------------
	// Ausgänge initialisieren
	// ------------------------
	
	if (CLK_auto_ON_OFF_debounce_state.debounced_state == 1)
	{
		// der automatische Takt ist an
		starte_automatischen_takt();
	}
	else
	{
		// der automatische Takt ist aus
		if (CLK_manual_debounce_state.debounced_state == 1) {
			clk_off();
		} else {
			clk_on();
		}
	}
	
	if (CU_ON_OFF_debounce_state.debounced_state) {
		enable_cu();
		enable_auto();
	} else {
		disable_cu();
		disable_auto();
	}
	
	// ------------------------
	
	
	sei();
	
    while (1)
    {	
		tick_loop();	
		_delay_ms(1);
    }
}



uint8_t debounce_changeDetected(volatile Debounce_state *state) 
{
	if (state->changeDetected)
	{
		state->changeDetected = 0;
		return true;
	}
	else
	{
		return false;
	}
}

ISR (TIMER1_COMPA_vect)
{
	if (PINB & (1 << CLK_PIN)) {
		// Der Takt ist 1
		// Takt auf 0 setzen
		//PORTB &= ~((1 << CLK_PIN) | (1 << CLK_LED_PIN));
		clk_off();		
	} else {
		// Der Takt ist 0
		// Takt auf 1 setzen
		clk_on();
		//PORTB |= (1 << CLK_PIN) | (1 << CLK_LED_PIN);
	}	
			
	if (CLK_Speed_debounce_state.debounced_state) {
		// schnell		
		OCR1A = 5;
	} else {
		// langsam
		OCR1A = 1000;
	}
}