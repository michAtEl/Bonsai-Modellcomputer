/*
 * util.h
 *
 * Created: 26.06.2023 21:32:58
 *  Author: Michael
 */ 


#ifndef UTIL_H_
#define UTIL_H_

// Makros für Zugriffe auf einzelne Pins
#define GET_PIN(register, pin)		(register & (1 << pin)) >> pin;
#define SET_PIN(register, pin)		register |= (1 << pin)
#define CLEAR_PIN(register, pin)	register &= ~(1 << pin)

#endif /* UTIL_H_ */