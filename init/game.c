#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
#include "raylib.h"

#define BLOCK_TYPES 7
#define BLOCK_SIZE 4

Color** allocate_matrix(int Rows, int Cols) {
    Color** matrix = (Color**)malloc(Rows * sizeof(Color*));
    for (int i = 0; i < Rows; i++) {
        matrix[i] = (Color*)malloc(Cols * sizeof(Color));
    }
    return matrix;
}

void free_matrix(int** matrix, int Rows) {
    for (int i = 0; i < Rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

Color*** create_all_blocks() {
    Color colors[8] = {BLACK, GREEN, RED, ORANGE, YELLOW, (Color){21, 204, 209, 255}, PURPLE, BLUE};
    Color*** blocks = (Color***)malloc(BLOCK_TYPES * sizeof(Color**));

    for (int i = 0; i < BLOCK_TYPES; i++) {
        blocks[i] = allocate_matrix(BLOCK_SIZE, BLOCK_SIZE);
        for (int r = 0; r < BLOCK_SIZE; r++) {
            for (int c = 0; c < BLOCK_SIZE; c++) {
                blocks[i][r][c] = BLACK;
            }
        }
    }

    // I Block
    for (int i = 0; i < 4; i++) blocks[0][1][i] = colors[1];

    // O Block
    blocks[1][1][1] = colors[2]; blocks[1][1][2] = colors[2];
    blocks[1][2][1] = colors[2]; blocks[1][2][2] = colors[2];

    // T Block
    blocks[2][0][1] = colors[5];
    blocks[2][1][0] = colors[5]; blocks[2][1][1] = colors[5]; blocks[2][1][2] = colors[5];

    // S Block
    blocks[3][1][1] = colors[3]; blocks[3][1][2] = colors[3];
    blocks[3][2][0] = colors[3]; blocks[3][2][1] = colors[3];

    // Z Block
    blocks[4][1][0] = colors[4]; blocks[4][1][1] = colors[4];
    blocks[4][2][1] = colors[4]; blocks[4][2][2] = colors[4];

    // J Block
    blocks[5][0][0] = colors[6];
    blocks[5][1][0] = colors[6]; blocks[5][1][1] = colors[6]; blocks[5][1][2] = colors[6];

    // L Block
    blocks[6][0][2] = colors[7];
    blocks[6][1][0] = colors[7]; blocks[6][1][1] = colors[7]; blocks[6][1][2] = colors[7];

    return blocks;
}

void draw_block(Color** block, int gridX, int gridY, int cellWidth, int cellHeight) {
    for (int i = 1; i <= BLOCK_SIZE; i++) {
        for (int j = 1; j <= BLOCK_SIZE; j++) {
            if (!ColorIsEqual(block[i-1][j-1], BLACK)) {
                DrawRectangle(gridX + (j * cellWidth), gridY + (i * cellHeight),
                              cellWidth, cellHeight,
                              block[i-1][j-1]);
            }
        }
    }
}

int grid() {
    Color** A = allocate_matrix(numRows, numCols);
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            A[i][j] = BLACK;
        }
    }
    return 0; // Placeholder, replace with actual game logic
}
