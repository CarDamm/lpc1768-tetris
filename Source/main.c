/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified for:            Tetris application
** Modified date:           24/12/2025
** Version:                 v2.0
** Project:             		Tetris
** Descriptions:        		Main game loop for Tetris implementation.
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
#include "joystick/joystick.h"
#include "button_EXINT/button.h"
#include "RIT/RIT.h"
#include "tetris_graphics.h"
#include "tetris_logic.h"



#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

//Global flags
volatile int timer_tick = 0;
volatile int pause_requested = 0;
volatile int hard_drop_requested = 0;
volatile int soft_drop_requested = 0;
volatile int move_left_requested = 0;
volatile int move_right_requested = 0;
volatile int rotation_requested = 0;




int main(void)
{
	// System Initialization (i.e., PLL)
  SystemInit();
	
	// LCD Initialization
  LCD_Initialization();
	LCD_Clear(Black);
	
	// BUTTON and joystick Initialization
	BUTTON_init();
	joystick_init();
	
	// RIT Initialization 16.67 ms
	init_RIT(0x001E8480/3);
	enable_RIT();
	
	// Timer0 Initialization
	init_timer(0, NORMAL_SPEED);
	enable_timer(0);
	
	// power-down	mode
	LPC_SC->PCON |= 0x1;
	LPC_SC->PCON &= 0xFFFFFFFFD;	

	//Game start sequence
	game_start();
	game_pause();

	
	
	//Game loop
  while (1)	
  {
		//Handle PAUSE/RESUME
		if (pause_requested) {
				hard_drop_requested = 0;
				timer_tick = 0;
        pause_requested = 0;
        pause_button_press();
    }

		//Movement allowed only when the game is running
    if (get_game_state() == GAME_RUNNING) {
			
				//LEFT / RIGHT movement
				if (move_left_requested) {
					move_left_requested = 0;
					move_left();
				}
				else if (move_right_requested) {
					move_right_requested = 0;
					move_right();
				}
				
				//ROTATION
				if (rotation_requested) {
					rotation_requested = 0;
					rotate();
				}
				
				//SOFT DROP
				if (soft_drop_requested == 1) {
					soft_drop();
				}
				else if (soft_drop_requested == 2) {
					soft_drop_requested = 0;
					normal_drop();
				}
				
				//HARD and NORMAL DROP
        if (hard_drop_requested) {
            hard_drop_requested = 0;
            timer_tick = 0;
            hard_drop_press();
        }
        else if (timer_tick) {
            timer_tick = 0;
            move_down(1);
        }
		}
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
