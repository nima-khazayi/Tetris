#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 900
#define GRID_WIDTH 10
#define GRID_HEIGHT 20
#define CELL_SIZE 40
#define BLOCK_SIZE 4
#define FALL_INTERVAL 0.5f

// Define Tetrimino shapes
const int TETROMINOS[7][4][4][4] = {
    // I
    {{{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
     {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}},
     {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
     {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}}},
    // O
    {{{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
     {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
     {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
     {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}}},
    // T
    {{{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}},
     {{0,0,1,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}},
     {{0,0,0,0},{0,1,0,0},{1,1,1,0},{0,0,0,0}},
     {{0,1,0,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}}},
    // S
    {{{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},
     {{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}},
     {{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},
     {{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}}},
    // Z
    {{{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},
     {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
     {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},
     {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}}},
    // J
    {{{0,0,0,0},{1,0,0,0},{1,1,1,0},{0,0,0,0}},
     {{0,1,1,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}},
     {{0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0}},
     {{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}}},
    // L
    {{{0,0,0,0},{0,0,1,0},{1,1,1,0},{0,0,0,0}},
     {{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}},
     {{0,0,0,0},{1,1,1,0},{1,0,0,0},{0,0,0,0}},
     {{1,1,0,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}}}
};

Color TETROMINO_COLORS[7] = { RED, ORANGE, PURPLE, GREEN, MAROON, BLUE, YELLOW };

int grid[GRID_HEIGHT][GRID_WIDTH] = {0};

int currentType = 0, currentRotation = 0, currentX = 3, currentY = 0;
int nextType = 0;
float fallTimer = 0.0f;

int gridOffsetX = 0;
int gridOffsetY = 0;

void DrawTetrisGrid() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x]) {
                DrawRectangle(
                    gridOffsetX + x * CELL_SIZE,
                    gridOffsetY + y * CELL_SIZE,
                    CELL_SIZE - 1, CELL_SIZE - 1,
                    TETROMINO_COLORS[grid[y][x] - 1]
                );
            } else {
                DrawRectangleLines(
                    gridOffsetX + x * CELL_SIZE,
                    gridOffsetY + y * CELL_SIZE,
                    CELL_SIZE, CELL_SIZE,
                    LIGHTGRAY
                );
            }
        }
    }
}

void DrawCurrentTetromino() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (TETROMINOS[currentType][currentRotation][i][j]) {
                DrawRectangle(
                    gridOffsetX + (currentX + j) * CELL_SIZE,
                    gridOffsetY + (currentY + i) * CELL_SIZE,
                    CELL_SIZE - 1, CELL_SIZE - 1,
                    TETROMINO_COLORS[currentType]
                );
            }
        }
    }
}

void DrawNextTetromino(int previewX, int previewY) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (TETROMINOS[nextType][0][i][j]) {
                DrawRectangle(
                    previewX + j * (CELL_SIZE),
                    previewY + i * (CELL_SIZE),
                    (CELL_SIZE) - 1, (CELL_SIZE) - 1,
                    TETROMINO_COLORS[nextType]
                );
            }
        }
    }
}

bool CheckCollision(int offsetX, int offsetY, int rotation) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (TETROMINOS[currentType][rotation][i][j]) {
                int x = currentX + j + offsetX;
                int y = currentY + i + offsetY;

                if (x < 0 || x >= GRID_WIDTH || y >= GRID_HEIGHT) return true;
                if (y >= 0 && grid[y][x]) return true;
            }
        }
    }
    return false;
}

void LockTetromino() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (TETROMINOS[currentType][currentRotation][i][j]) {
                int x = currentX + j;
                int y = currentY + i;
                if (y >= 0) grid[y][x] = currentType + 1;
            }
        }
    }
}

void SpawnTetromino() {
    currentType = nextType;
    nextType = GetRandomValue(0, 6);
    currentRotation = 0;
    currentX = 3;
    currentY = 0;
}

void ClearLines() {
    for (int i = GRID_HEIGHT - 1; i >= 0; i--) {
        bool full = true;
        for (int j = 0; j < GRID_WIDTH; j++) {
            if (!grid[i][j]) {
                full = false;
                break;
            }
        }
        if (full) {
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < GRID_WIDTH; j++) {
                    grid[k][j] = grid[k - 1][j];
                }
            }
            for (int j = 0; j < GRID_WIDTH; j++) grid[0][j] = 0;
            i++; // re-check current row
        }
    }
}

void UpdateTetris(float dt) {
    fallTimer += dt;

    if (IsKeyPressed(KEY_LEFT) && !CheckCollision(-1, 0, currentRotation)) currentX--;
    if (IsKeyPressed(KEY_RIGHT) && !CheckCollision(1, 0, currentRotation)) currentX++;
    if (IsKeyPressed(KEY_UP)) {
        int nextRotation = (currentRotation + 1) % 4;
        if (!CheckCollision(0, 0, nextRotation)) currentRotation = nextRotation;
    }
    if (IsKeyPressed(KEY_DOWN) && !CheckCollision(0, 1, currentRotation)) currentY++;

    if (fallTimer >= FALL_INTERVAL) {
        if (!CheckCollision(0, 1, currentRotation)) {
            currentY++;
        } else {
            LockTetromino();
            ClearLines();
            SpawnTetromino();
        }
        fallTimer = 0;
    }

    DrawTetrisGrid();
    DrawCurrentTetromino();
}
