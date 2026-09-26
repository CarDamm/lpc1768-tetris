/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           tetris_graphics.h
** Last modified Date:  2025-12-24
** Project:             Tetris
** Descriptions:        Prototypes of functions included in the tetris_graphics.c file
** Correlated files:    tetris_graphics.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#ifndef __TETRIS_GRAPHICS_H
#define __TETRIS_GRAPHICS_H

#include "LPC17xx.h"
#include "../GLCD/GLCD.h"


#define GRID_ROWS 20
#define GRID_COLS 10

#define BLOCK_SIZE 15
#define OFFSET_X 11
#define OFFSET_Y 11

#define COLOR_BACKGROUND Black
#define COLOR_BORDER     White
#define COLOR_I  0x07FF
#define COLOR_O  0xFFE0
#define COLOR_T  0x801F
#define COLOR_J  0x001F
#define COLOR_L  0xFD20
#define COLOR_S  0x07E0
#define COLOR_Z  0xF800


extern unsigned short piece_colors[8];


void UI_Init(int high_score);
void UI_DrawBlock(int col, int row, uint16_t color);
void UI_ClearBlock(int col, int row);
void UI_UpdateScore(int score, int lines);
void UI_DrawRow(int row_index, unsigned char row_data[]);
void UI_DrawArea(int col, row, int cols, int rows, uint16_t color);
void UI_ShowPause(void);
void UI_HidePause(void);
void UI_ShowGameOver(void);
void UI_DrawBorder(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
void UI_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);

#endif
