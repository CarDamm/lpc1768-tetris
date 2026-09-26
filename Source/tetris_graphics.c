/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           tetris_graphics.c
** Last modified Date:  2025-12-24
** Project:             Tetris
** Descriptions:        Graphic UI management that translates grid coordinates to LCD pixels.
** Correlated files:    tetris_graphics.h
*********************************************************************************************************/

#include "tetris_graphics.h"
#include "GLCD/GLCD.h" 
#include <stdio.h>

unsigned short piece_colors[8] = {
    COLOR_BACKGROUND,
		COLOR_I,
		COLOR_O,
		COLOR_T,
		COLOR_J,
		COLOR_L,
		COLOR_S,
		COLOR_Z
};

void UI_Init(int high_score) {
    LCD_Clear(COLOR_BACKGROUND);
    UI_DrawBorder(10, 10, 151, 301, COLOR_BORDER); 
    
		GUI_Text(180, 40,  (uint8_t *)"TOP", White, Black);
		GUI_Text(180, 90,  (uint8_t *)"SCORE", White, Black);
		GUI_Text(180, 140, (uint8_t *)"LINES", White, Black);

		
		char s_high_score[10];
		sprintf(s_high_score, "%06d", high_score);
	
		GUI_Text(180, 60,  (uint8_t *)s_high_score, Yellow, Black); // High Score
		GUI_Text(180, 110,  (uint8_t *)"000000", Yellow, Black); // Score, 0 as first value
		GUI_Text(180, 160, (uint8_t *)"000000", Yellow, Black); // Lines, 0 as first value
	
}

void UI_DrawBlock(int col, int row, uint16_t color) {
    
    uint16_t x = OFFSET_X + (col * BLOCK_SIZE);
    uint16_t y = OFFSET_Y + (row * BLOCK_SIZE);
    
    
		if (color==COLOR_BACKGROUND)
			UI_DrawBorder(x, y, BLOCK_SIZE-1, BLOCK_SIZE-1, COLOR_BACKGROUND);
		else
			UI_DrawBorder(x, y, BLOCK_SIZE-1, BLOCK_SIZE-1, COLOR_BORDER);
		
		UI_DrawBorder(x+1, y+1, BLOCK_SIZE-3, BLOCK_SIZE-3, COLOR_BACKGROUND);
    UI_FillRect(x+2, y+2, BLOCK_SIZE-4, BLOCK_SIZE-4, color);
}

void UI_ClearBlock(int col, int row) {
		uint16_t x = OFFSET_X + (col * BLOCK_SIZE);
    uint16_t y = OFFSET_Y + (row * BLOCK_SIZE);
    UI_FillRect(x, y, BLOCK_SIZE, BLOCK_SIZE, COLOR_BACKGROUND);
}

void UI_UpdateScore(int score, int lines) {
    char s_score[10];
		sprintf(s_score, "%06d", score);
    GUI_Text(180, 110, (uint8_t*)s_score, Yellow, Black);
	
		char s_lines[10];
		sprintf(s_lines, "%06d", lines);
    GUI_Text(180, 160, (uint8_t*)s_lines, Yellow, Black);
}


void UI_DrawRow(int row_index, unsigned char row_data[]) {
		int c;
    for(c = 0; c < GRID_COLS; c++) {
        UI_DrawBlock(c, row_index, piece_colors[row_data[c]]);
    }
}



void UI_DrawArea(int col, row, int cols, int rows, uint16_t color) {
		uint16_t x = OFFSET_X + (col * BLOCK_SIZE);
    uint16_t y = OFFSET_Y + (row * BLOCK_SIZE);
		uint16_t width = cols * BLOCK_SIZE;
		uint16_t height = rows * BLOCK_SIZE;
		UI_FillRect(x, y, width, height, color);
}

void UI_ShowPause(void) {
	GUI_Text(180, 190, (uint8_t *)"PAUSE", White, Black);
}

void UI_HidePause(void) {
	GUI_Text(180, 190, (uint8_t *)"PAUSE", Black, Black);
}

void UI_ShowGameOver(void) {
	GUI_Text(43, 146, (uint8_t *)" GAME OVER ", Red, COLOR_BORDER);
}


void UI_DrawBorder(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
    // Top
    LCD_DrawLine(x, y, x + width, y, color);
    // Bottom
    LCD_DrawLine(x, y + height, x + width, y + height, color);
    // Left
    LCD_DrawLine(x, y, x, y + height, color);
    // Right
    LCD_DrawLine(x + width, y, x + width, y + height, color);
}


void UI_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
    uint16_t i;
    for (i = 0; i < height; i++) {
        LCD_DrawLine(x, y + i, x + width - 1, y + i, color);
    }
}










