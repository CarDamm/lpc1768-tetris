/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2025-12-24
** Project:             Tetris
** Descriptions:        Atomic joystick init functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "joystick.h"

/*----------------------------------------------------------------------------
  Function that initializes joysticks and switch them off
 *----------------------------------------------------------------------------*
 
  Joystick pin map (Port 1):
 * P1.26 -> DOWN
 * P1.27 -> LEFT
 * P1.28 -> RIGHT
 * P1.29 -> UP
 */
void joystick_init(void) {
    // 1. Clear bits to set pins as GPIO
    LPC_PINCON->PINSEL3 &= ~( (3<<18) | (3<<20) | (3<<22) | (3<<24) | (3<<26) );

    // 2. Set pins direction as INPUT(0)
    LPC_GPIO1->FIODIR &= ~( (1<<25) | (1<<26) | (1<<27) | (1<<28) | (1<<29) );
}
