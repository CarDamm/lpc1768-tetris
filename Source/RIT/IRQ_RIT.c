/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2025-12-24
** Project:             Tetris
** Descriptions:        RIT service routine: handles debouncing for buttons and joystick
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "tetris_logic.h"
#include "../joystick/joystick.h"


extern volatile int pause_requested;
extern volatile int hard_drop_requested;
extern volatile int soft_drop_requested;
extern volatile int move_left_requested;
extern volatile int move_right_requested;
extern volatile int rotation_requested;



void RIT_IRQHandler (void)
{			
	
	// INT0 (P2.10) - not used
	if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
		reset_RIT();
	}
	else {	/* button released */
		reset_RIT();
		NVIC_EnableIRQ(EINT0_IRQn);							 /* Enable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
	}
	
	static int key1_count = 0;
	// KEY1 (P2.11 -> EINT1) - PAUSE button
	if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){
		key1_count = 0;
	}
	else {	/* button released */
		key1_count++;
		
		if (key1_count > 2) {	// Stable for at least 50 ms (if RIT is 16.67 ms)
			key1_count = 0;
			pause_requested = 1;
			NVIC_EnableIRQ(EINT1_IRQn);							 // Re-enable Button interrupts
			LPC_PINCON->PINSEL4    |= (1 << 22);     // Restore EINT1 function on pin
		}
		
	}
	
	
	static int key2_count = 0;
	// KEY2 (P2.12 -> EINT2)  - HARD DROP button
	if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){
		key2_count = 0;
	}
	else {	/* button released */
		key2_count++;
		
		if (key2_count > 2) {		// Stable for at least 50 ms (if RIT is 16.67 ms)
			key2_count = 0;
			hard_drop_requested = 1;
			NVIC_EnableIRQ(EINT2_IRQn);							 // Re-enable Button interrupts
			LPC_PINCON->PINSEL4    |= (1 << 24);     // Restore EINT1 function on pin
		}
	}
	
	
	// JOYSTICK  - MOVEMENT, SOFT DROP and ROTATION
	
	
	//LEFT and RIGHT
	if (is_pressed(JOYSTICK_LEFT)) {
		move_left_requested = 1; 
	}
	if (is_pressed(JOYSTICK_RIGHT)) {
		move_right_requested = 1;
	}
	
	//UP - Prevented continous rotation
	static int joy_up_last_state = 0;
	int joy_up_current_state = is_pressed(JOYSTICK_UP);
	if (joy_up_current_state && !joy_up_last_state) {
		rotation_requested = 1;
	}
	joy_up_last_state = joy_up_current_state;
	

	//DOWN
	static int joy_down_last_state = 0;
	int joy_down_current_state = is_pressed(JOYSTICK_DOWN);
	if (joy_down_current_state) {
		soft_drop_requested = 1; // Trigger speed increase
	}
	else if (joy_down_last_state) {
		soft_drop_requested = 2;	// Trigger speed reset to normal
	}
	joy_down_last_state = joy_down_current_state;
		
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
