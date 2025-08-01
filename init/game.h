#ifndef GAME_H
#define GAME_H

#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 900
#define GRID_WIDTH 10
#define GRID_HEIGHT 20
#define CELL_SIZE 40
#define BLOCK_SIZE 4
#define FALL_INTERVAL 0.5f

extern const int TETROMINOS[7][4][4][4];
extern Color TETROMINO_COLORS[7];
extern int grid[GRID_HEIGHT][GRID_WIDTH];
extern int gridOffsetX;
extern int gridOffsetY; 

void DrawTetrisGrid();
void DrawCurrentTetromino();
void DrawNextTetromino(int previewX, int previewY);
bool CheckCollision(int offsetX, int offsetY, int rotation);
void LockTetromino();
void SpawnTetromino();
void ClearLines();
void UpdateTetris(float dt);

#endif // GAME_H
