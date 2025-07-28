#ifndef GAME_H
#define GAME_H

#include "raylib.h"

// Constants
#define BLOCK_TYPES 7
#define BLOCK_SIZE 4

// Allocate a 2D matrix of Color
Color** allocate_matrix(int Rows, int Cols);

// Free a 2D matrix of Color
void free_matrix(Color** matrix, int Rows);

// Generate all Tetris blocks
Color*** create_all_blocks(void);

// Draw a block at a grid position
void draw_block(Color** block, int gridX, int gridY, int cellWidth, int cellHeight);

// Game grid setup (placeholder)
int grid(void);

#endif // GAME_H
