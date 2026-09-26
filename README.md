# Tetris for NXP LPC1768 / LandTiger

Tetris implemented in C for the NXP LPC1768 (ARM Cortex-M3) LandTiger platform. This is an academic embedded programming project preserved and published after the course for which it was developed.

## Features

- 20 x 10 game board and the seven tetromino types
- Horizontal movement and clockwise rotation with limited horizontal adjustment
- Collision detection, piece locking, and completed-line removal
- Soft drop, hard drop, pause, restart, and game-over states
- Score, cleared-line count, and a high score retained in RAM until reset
- Incremental LCD updates during piece movement and affected-row redraws after line removal

## Controls

- Joystick left/right: move the active piece
- Joystick up: rotate clockwise
- Joystick down: use the faster falling interval while held
- KEY1: pause or resume; start a new game after game over
- KEY2: hard drop

The application initializes the board and starts in the paused state.

## Implementation

The firmware uses a bare-metal superloop with no RTOS. Timer0 generates falling-piece events. The Repetitive Interrupt Timer (RIT) periodically samples the joystick and participates in external-button debouncing; interrupt handlers publish flags that the foreground loop consumes.

The board is stored separately from the active piece. Collision checks test the active tetromino against board boundaries and locked cells. After a piece locks, only the four rows intersecting that piece are checked for completed lines. Scoring adds 10 points for each locked piece, 100 points per cleared line, and an additional 200 points when four lines are cleared at once.

Normal movement clears obsolete active-piece blocks and draws the new blocks instead of redrawing the complete screen. Line removal redraws the shifted portion of the board. Rendering uses the bundled GLCD interface.

## Hardware And Tools

- NXP LPC1768, ARM Cortex-M3
- LandTiger board model with 240 x 320 LCD
- On-board joystick, KEY1, and KEY2
- Keil MDK / uVision project: `sample.uvprojx`
- Arm Compiler 6.24 and `Keil.LPC1700_DFP` 2.7.2 in the recorded project configuration

The project defines `LandTiger_LPC1768 (release)` and `SW_DEBUG` targets. `SW_DEBUG` defines `SIMULATOR`. The repository does not contain a command-line build, automated tests, or enough verified information to document a physical-board flashing procedure. The listed tool versions describe the historical project configuration and have not been independently reproduced as part of this publication cleanup.

## Repository Structure

- `Source/main.c`: hardware initialization and foreground event loop
- `Source/tetris_logic.*`: board, pieces, movement, collision, states, line removal, and scoring
- `Source/tetris_graphics.*`: game-specific LCD rendering
- `Source/joystick/`, `Source/button_EXINT/`, `Source/RIT/`, `Source/timer/`: input and timing support used by the game
- `Source/GLCD/`: bundled LCD and font support
- `Source/TouchPanel/`: bundled legacy support compiled by the project but unused by the game
- `Source/CMSIS_core/`, `Source/startup_LPC17xx.s`, `Source/system_LPC17xx.c`: CMSIS, startup, and device initialization support
- `sample.uvprojx`: Keil project and target configuration

## Provenance

The Tetris-specific logic and presentation are in `main.c`, `tetris_logic.*`, and `tetris_graphics.*`, with project-specific integration in the input and timer modules.

The repository also contains hardware-support and template code supplied or adapted through the original course/toolchain environment. This includes ARM CMSIS and startup material, PowerMCU/AVRman-derived GLCD and touch-panel code, and low-level timer, RIT, button, and joystick support. Existing third-party attribution and license notices are preserved in those files.

The project was developed for a Computer Architectures course at Politecnico di Torino. It is retained as a historical academic implementation, including its original architecture and design decisions.
