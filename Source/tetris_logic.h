/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           tetris_logic.h
** Last modified Date:  2025-12-24
** Project:             Tetris
** Descriptions:        Prototypes of functions included in the tetris_logic.c file
** Correlated files:    tetris_logic.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#ifndef __TETRIS_LOGIC_H
#define __TETRIS_LOGIC_H

#define GRID_ROWS 20
#define GRID_COLS 10

#define NORMAL_SPEED ((uint32_t)(0x17D7840)) // 1.0 s
#define SOFT_DROP_SPEED ((uint32_t)(NORMAL_SPEED/2)) // 0.5 s

extern const unsigned char PIECES[8][4][4][4];

typedef enum {
    GAME_RUNNING,
    GAME_PAUSED,
    GAME_OVER
} GameState;


// Active piece rendering
void render_active_piece(void);

// Collision check against boundaries and locked blocks.
int check_collision(int nextX, int nextY, int nextRot);

// Spawn a new random tetromino at the top of the grid.
void spawn_new_piece(void);
int generate_random_piece(void);

// Hide active piece
void delete_active_piece(void);

//Game state transitions
void pause_button_press(void);
void reset_board(void);
void game_start(void);
void game_pause(void);
void game_resume(void);
void game_over(void);

//Pieces movement management
int move_down(int should_render);
void move_left(void);
void move_right(void);
void rotate(void);
void soft_drop(void);
void normal_drop(void);
void hard_drop_press(void);

// Line clearing and scoring
void lock_piece(void);
int check_and_clear_lines (void);
void shift_rows_down(int start_row);
void calculate_and_store_score(int cleared_lines);

// Getter and setter for GameState
void set_game_state(GameState new_state);
GameState get_game_state(void);


#endif
