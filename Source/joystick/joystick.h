/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2025-12-24
** Project:             Tetris
** Descriptions:        Prototypes of functions included in the lib_joystick, funct_joystick .c files
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include <stdint.h>

#define JOYSTICK_UP     (1 << 29)
#define JOYSTICK_DOWN   (1 << 26)
#define JOYSTICK_LEFT   (1 << 27)
#define JOYSTICK_RIGHT  (1 << 28)

/* lib_joystick */
void joystick_init(void);

/* funct_joystick */
int is_pressed(uint32_t direction);


#endif
