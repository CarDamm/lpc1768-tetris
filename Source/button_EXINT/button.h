/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           button.h
** Last modified Date:  2025-12-24
** Descriptions:        Prototypes of functions included in the lib_button, IRQ_button .c files
** Project:             Tetris
** Correlated files:    lib_button.c, IRQ_button.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#ifndef __BUTTON_H
#define __BUTTON_H

/* lib_button */
void BUTTON_init(void);

/* IRQ_button */
void EINT1_IRQHandler(void);
void EINT2_IRQHandler(void);
void EINT3_IRQHandler(void);

#endif
