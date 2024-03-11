/*
 * board.h
 *
 * Created: 26.06.2023 21:30:32
 *  Author: Michael
 */ 


#ifndef BOARD_H_
#define BOARD_H_

#include "util.h"

// Port B
#define CLK_PIN				1		// Ausgang
#define CLK_LED_PIN			2		// Ausgang

// Port C
#define CLK_Poti			0		// analoger Eingang

#define CLK_Speed			1		// digitaler Eingang
#define CLK_auto_ON_OFF		2		// digitaler Eingang
#define CLK_manual			3		// digitaler Eingang
#define CU_ON_OFF			4		// digitaler Eingang
#define RESET				5		// digitaler Eingang

// Port D - Ausgänge
#define _EN_DEC				0
#define _EN_CU				1
#define _CLR_MPC			2
#define _WRITE_ACCU			3
#define _READ_IR			4
#define CLR_PC				5
#define CLR_AKKU			6
#define _AUTO				7

void clk_off();
void clk_on();

void enable_cu();
void disable_cu();

void enable_auto();
void disable_auto();

#endif /* BOARD_H_ */