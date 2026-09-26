/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           funct_joystick.h
** Last modified Date:  2025-12-24
** Project:             Tetris
** Descriptions:        High level joystick management functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "joystick.h"



int is_pressed(uint32_t direction) {
    if ((LPC_GPIO1->FIOPIN & direction) == 0) 
        return 1;
    return 0;
}
