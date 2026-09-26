/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           tetris_logic.c
** Last modified Date:  2025-12-24
** Project:             Tetris
** Descriptions:        Core game logic
** Correlated files:    tetris_logic.h
*********************************************************************************************************/

#include <stdlib.h>
#include "tetris_logic.h"
#include "tetris_graphics.h"
#include "timer/timer.h"


// Global game state
unsigned char board[GRID_ROWS][GRID_COLS] = {0}; 
unsigned int current_score = 0;
unsigned int high_score = 0;
unsigned int lines_cleared = 0;

GameState current_state = GAME_RUNNING;

//Tetromino shapes [Type][Rotation][Row][Column] 
const unsigned char PIECES[8][4][4][4] = {
    // 0: EMPTY
    {{{0}}}, 

    // 1: I
    {
        {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
        {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}},
        {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}}
    },
    // 2: O
    {
        {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}
    },
    // 3: T
    {
        {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
        {{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}},
        {{0,1,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}}
    },
    // 4: J
    {
        {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,1,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}},
        {{0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}}
    },
    // 5: L
    {
        {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}},
        {{0,0,0,0},{1,1,1,0},{1,0,0,0},{0,0,0,0}},
        {{1,1,0,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}}
    },
    // 6: S
    {
        {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}},
        {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}}
    },
    // 7: Z
    {
        {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
        {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}}
    }
};

// Active piece management
typedef struct {
    int type;
		int rotation, old_rotation;
		int x, y, old_x, old_y;
		const unsigned char (*shape)[4];
} Tetromino;

Tetromino active_piece;


// Active piece rendering: clears old blocks only if they are not occupied by the new position.
void render_active_piece(void) {
   
		int r, c;	
	
		const unsigned char (*oldShape)[4];
		oldShape = PIECES[active_piece.type][active_piece.old_rotation];
		
		//Remove only moved blocks
		for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
						int x = c + active_piece.old_x;
						int	y = r + active_piece.old_y;
						int clear = 1;
						if (oldShape[r][c]) {
						 int newR = y - active_piece.y;
             int newC = x - active_piece.x;
							if (newR >= 0 && newR < 4 && newC >= 0 && newC < 4 && active_piece.shape[newR][newC])
								clear = 0;
						 if (clear && x >= 0 && x < GRID_COLS && y >= 0 && y < GRID_ROWS && !board[y][x]) {
							UI_ClearBlock(x, y);
            }
					} 
						
        }
    }
		
		
    // Render piece in new position
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
						int x = c + active_piece.x;
						int	y = r + active_piece.y;
            if (active_piece.shape[r][c] && x >= 0 && x < GRID_COLS && y >= 0 && y < GRID_ROWS && !board[y][x]) {
							UI_DrawBlock(x, y, piece_colors[active_piece.type]);
            }
        }
    }

    
}


// Collision check against boundaries and locked blocks.
int check_collision(int nextX, int nextY, int nextRot) {
	int r, c;
	const unsigned char (*nextShape)[4];
	nextShape = PIECES[active_piece.type][nextRot];
	for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
						int x = c + nextX;
						int	y = r + nextY;
						if (y < 0  && y > -4 && x >= 0 && x < GRID_COLS)
							continue;
            if (nextShape[r][c]==1 && (x < 0 || x >= GRID_COLS || y < 0 || y >= GRID_ROWS || board[y][x] > 0))
							return 1;
        }
    }
	return 0;
}


// Spawn a new random tetromino at the top of the grid.
void spawn_new_piece(void) {
    active_piece.type = generate_random_piece();
        
    active_piece.x = 3; 
    active_piece.y = -2;
		active_piece.old_x = 3;
		active_piece.old_y = -3;
		active_piece.rotation = 0;
		active_piece.old_rotation = 0;
		active_piece.shape = PIECES[active_piece.type][active_piece.rotation];
		
		if (!check_collision(active_piece.x, active_piece.y+1, active_piece.rotation)) {
			render_active_piece();
		}
		else {
			game_over();
		}
		
}

int generate_random_piece(void) {
    return (int)((rand() % 7) + 1);
}

// Hide active piece
void delete_active_piece(void) {
		int r, c;
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
						int x = c + active_piece.x;
						int	y = r + active_piece.y;
						if (x >= 0 && x < GRID_COLS && y >= 0 && y < GRID_ROWS &&  board[y][x] == 0)  {
							UI_ClearBlock(x, y);
						}
        }
    }

    
}

//Game state transitions

void pause_button_press(void) {
	
	switch (get_game_state()) {
		
            case GAME_RUNNING:
								game_pause();
                break;
            
            case GAME_PAUSED:
                game_resume();
                break;
                
            case GAME_OVER:
                game_start();
								game_resume();
                break;
                
                
            default:
                break;
        }
}

void reset_board(void) {
	int x, y;
    for (y = 0; y < GRID_ROWS; y++) {
        for (x = 0; x < GRID_COLS; x++) {
            board[y][x] = 0;
        }
    }
}


void game_start(void) {
		if (current_score > high_score)
			high_score = current_score;
		current_score = 0;
		lines_cleared = 0;
    
    UI_Init(high_score);
    reset_board();
    
    srand(LPC_TIM0->TC); // Seed initialization with TC
    spawn_new_piece(); 
    
}

void game_pause(void) {
	set_game_state(GAME_PAUSED);
	UI_ShowPause();
}

void game_resume(void) {
	set_game_state(GAME_RUNNING);
	restart_timer(0, NORMAL_SPEED);
	UI_HidePause();
}

void game_over(void) {    
	set_game_state(GAME_OVER);
	UI_ShowGameOver();
}

//Pieces movement management

int move_down(int should_render) {
	if (!check_collision(active_piece.x, active_piece.y+1, active_piece.rotation)) {
		active_piece.old_x = active_piece.x;
		active_piece.old_y = active_piece.y;
		active_piece.old_rotation = active_piece.rotation;
		active_piece.y = active_piece.y + 1;
		if (should_render)
			render_active_piece();
		return 1;
	}
	else {
		lock_piece();
	}
	return 0;
}

void move_left(void) {
	if (!check_collision(active_piece.x-1, active_piece.y, active_piece.rotation)) {
		active_piece.old_x = active_piece.x;
		active_piece.old_y = active_piece.y;
		active_piece.old_rotation = active_piece.rotation;
		active_piece.x = active_piece.x - 1;
		render_active_piece();
	}
}

void move_right(void) {
	if (!check_collision(active_piece.x+1, active_piece.y, active_piece.rotation)) {
		active_piece.old_x = active_piece.x;
		active_piece.old_y = active_piece.y;
		active_piece.old_rotation = active_piece.rotation;
		active_piece.x = active_piece.x + 1;
		render_active_piece();
	}
}


/**
 * Rotates the active piece, if rotation fails at the current position, the function attempts 
 * to shift the piece towards the center of the grid (total 3 attempts).
 * (right if on the left half, left if on the right half).
 */
void rotate(void) {
	int i;
	int nextRot = (active_piece.rotation + 1) % 4;
	
	int direction = (active_piece.x < 5) ? 1 : -1;
	
	for (i=0; i<3; i++) {
		int nextX = active_piece.x + i * direction;
		if (!check_collision(nextX, active_piece.y, nextRot)) {	
			active_piece.old_x = active_piece.x;
			active_piece.old_y = active_piece.y;
			active_piece.old_rotation = active_piece.rotation;
			active_piece.x = nextX;
			active_piece.rotation = nextRot;
			active_piece.shape = PIECES[active_piece.type][active_piece.rotation];
			render_active_piece();
			break;
		}
	}
	
}

void soft_drop(void) {
	if (get_timer_interval(0) == NORMAL_SPEED) 
		restart_timer(0, SOFT_DROP_SPEED);
}

void normal_drop(void) {
	if (get_timer_interval(0) == SOFT_DROP_SPEED) 
		restart_timer(0, NORMAL_SPEED);
}


void hard_drop_press(void) {
    if (current_state != GAME_RUNNING) return;
    disable_timer(0); 

    delete_active_piece(); 
    while (!check_collision(active_piece.x, active_piece.y+1, active_piece.rotation)) {
			move_down(0);
		}
    render_active_piece();
		lock_piece();

    restart_timer(0, NORMAL_SPEED);
}


// Line clearing and scoring

void lock_piece(void) {
	int r, c;
	for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
						int x = c + active_piece.x;
						int	y = r + active_piece.y;
            if (active_piece.shape[r][c]==1 && x >= 0 && x < GRID_COLS && y >= 0 && y < GRID_ROWS) {
							board[y][x] = active_piece.type;
							UI_DrawBlock(x, y, piece_colors[active_piece.type]);
						}
							
        }
    }
	int cleared_lines = check_and_clear_lines();
	calculate_and_store_score(cleared_lines);
	spawn_new_piece();
}


int check_and_clear_lines (void) {
		int r, x, y;
		int cleared_lines = 0;
	
		for (r = 3; r >= 0; r--) {
			int line_cleared = 1;
			for (x = 0; x < GRID_COLS; x++) {
					y = r + active_piece.y;
					if (y >= 0 && y < GRID_ROWS) {
							if (board[y][x] == 0) {
								line_cleared = 0;
								break;
							}
					}
					else {
						line_cleared = 0;
						break;
					}
			}
			cleared_lines += line_cleared;
			if (line_cleared) {
				UI_DrawArea(0, y, 10, 1, COLOR_BORDER);
				UI_DrawArea(0, y, 10, 1, COLOR_BACKGROUND);
				shift_rows_down(y);
				r++;
			} 
	}
	
	return cleared_lines;
}




void shift_rows_down(int cleared_row) {
		int r, c;
    
    for (r = cleared_row; r > 0; r--) {
			for (c = 0; c < 10; c++) {
				board[r][c] = board[r-1][c];
			}
    }
    
	
		for (c = 0; c < GRID_COLS; c++)
			board[0][c] = 0;

		
		UI_DrawArea(0, 0, GRID_COLS, 1, COLOR_BACKGROUND);
		
		for (r = 0; r <= cleared_row; r++) {
        UI_DrawRow(r, board[r]);
    }
		
}



void calculate_and_store_score(int cleared_lines) {
	int score = cleared_lines * 100 + 10;
	if (cleared_lines == 4)
		score += 200;
	current_score += score;
	lines_cleared += cleared_lines;
	UI_UpdateScore(current_score, lines_cleared);
}



// Getter and setter for GameState
GameState get_game_state(void) {
    return current_state;
}

void set_game_state(GameState new_state) {
    current_state = new_state;
}







