/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_button.c
** Last modified Date:  2025-12-24
** Project:             Tetris
** Descriptions:        Button interrupt service routines
** Correlated files:    lib_button.c, button.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "button.h"
#include "LPC17xx.h"

void EINT0_IRQHandler (void) //INT0
{
	LPC_SC->EXTINT &= (1 << 0);     // Clear pending interrupt
}



void EINT1_IRQHandler (void) //KEY1
{
	if ((LPC_SC->EXTINT & (1 << 1)) != 0) {
		NVIC_DisableIRQ(EINT1_IRQn);		// Disable button interrupts
		LPC_PINCON->PINSEL4    &= ~(1 << 22);     // GPIO pin selection
		
		LPC_SC->EXTINT |= (1 << 1);     // Clear pending interrupt
	}
	
}

void EINT2_IRQHandler (void) //KEY2
{
	if ((LPC_SC->EXTINT & (1 << 2)) != 0) {
		NVIC_DisableIRQ(EINT2_IRQn);		// Disable button interrupts
		LPC_PINCON->PINSEL4    &= ~(1 << 24);     // GPIO pin selection
		
		LPC_SC->EXTINT |= (1 << 2);     // Clear pending interrupt
	}
	
}


