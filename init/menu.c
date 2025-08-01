#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "raylib.h"
#include "globals.h"
#include "game.h"
#include <time.h>

#define MAX_INPUT_CHARS 20
char name[MAX_INPUT_CHARS + 1] = "\0"; // +1 for null terminator
char pass[MAX_INPUT_CHARS + 1] = "\0";
int letterCount = 0;
int passCount = 0;

const int numRows = 20;
const int numCols = 10;

const int cellWidth = 400 / numCols;
const int cellHeight = 800 / numRows;

int speed = 75;

int initializer() {

    float deltaTime = GetFrameTime();

    static MenuState currentMenuState = MAIN_MENU;

    // Menu rectangle position and size
    int menuWidth = 800;
    int menuHeight = 600;
    int menuX = (GetScreenWidth() - menuWidth) / 2;
    int menuY = (GetScreenHeight() - menuHeight) / 2;

    if (currentMenuState == MAIN_MENU) {
        DrawRectangle(menuX, menuY, menuWidth, menuHeight, LIGHTGRAY);
        Rectangle rec = {menuX + 40, menuY + 100, 720, 450};
        Rectangle rec1 = {menuX + 257, menuY + 180, 280, 60};
        Rectangle rec2 = {menuX + 277, menuY + 270, 240, 60};
        Rectangle rec3 = {menuX + 260, menuY + 355, 280, 60};
        Rectangle rec4 = {menuX + 260, menuY + 440, 280, 70};
        DrawRectangleLinesEx(rec, 5.0f, RED);
        

        // Add title or button text
        DrawText("Main Menu", menuX + 200, menuY + 10, 80, BLACK);
        DrawText("Start", menuX + 340, menuY + 190, 40, DARKGRAY);
        DrawText("Exit", menuX + 358, menuY + 280, 40, DARKGRAY);
        if (soundOn) {
            DrawText("Sound < On >", menuX + 310, menuY + 370, 30, DARKGRAY);
        } else {
            DrawText("Sound < Off >", menuX + 310, menuY + 370, 30, DARKGRAY);
        }

        if (mode) {
            DrawText("Mode < Easy >", menuX + 300, menuY + 460, 30, DARKGRAY);
        } else {
            DrawText("Mode < Hard >", menuX + 300, menuY + 460, 30, DARKGRAY);
        }

        // Handle menu bar movements
        if (IsKeyPressed(KEY_UP)) {
            shouldExit--;
        } else if (IsKeyPressed(KEY_DOWN)) {
            shouldExit++;
        }

        // Show boxed selected item

        // Starting game handling part
        if (shouldExit % 4 == 2) {
            DrawRectangleRec(rec1, (Color) {80, 80, 80, 130});
            DrawText("Start", menuX + 340, menuY + 190, 40, BLACK);

            if (IsKeyPressed(KEY_ENTER)) {
                currentMenuState = LOGIN_SCREEN; // Switch to login screen
            }

        
        // Exit method handling part
        } else if (shouldExit % 4 == 3) {
            DrawRectangleRec(rec2, (Color) {80, 80, 80, 130});
            DrawText("Exit", menuX + 358, menuY + 280, 40, BLACK);

            if (IsKeyPressed(KEY_ENTER)) {
                return 1;
            }
        
        // Sound control handling part
        } else if (shouldExit % 4 == 0) {
            DrawRectangleRec(rec3, (Color) {80, 80, 80, 130});
            DrawText("Sound < On >", menuX + 310, menuY + 370, 30, BLACK);
            if (IsKeyPressed(KEY_RIGHT) | IsKeyPressed(KEY_LEFT)) {
                soundOn = !soundOn;
            }

            if (soundOn) {
                DrawRectangle(menuX + 260, menuY + 355, 280, 60, LIGHTGRAY);
                DrawRectangleRec(rec3, (Color) {80, 80, 80, 130});
                DrawText("Sound < On >", menuX + 310, menuY + 370, 30, BLACK);
            } else {
                DrawRectangle(menuX + 260, menuY + 355, 280, 60, LIGHTGRAY);
                DrawRectangleRec(rec3, (Color) {80, 80, 80, 130});
                DrawText("Sound < Off >", menuX + 310, menuY + 370, 30, BLACK);
            }

        } else {
            DrawRectangleRec(rec4, (Color) {80, 80, 80, 130});
            DrawText("Mode < Easy >", menuX + 300, menuY + 460, 30, DARKGRAY);
            if (IsKeyPressed(KEY_RIGHT) | IsKeyPressed(KEY_LEFT)) {
                mode = !mode;
            }

            if (mode) {
                DrawRectangle(menuX + 260, menuY + 440, 280, 70, LIGHTGRAY);
                DrawRectangleRec(rec4, (Color) {80, 80, 80, 130});
                DrawText("Mode < Easy >", menuX + 300, menuY + 460, 30, BLACK);
            } else {
                DrawRectangle(menuX + 260, menuY + 440, 280, 70, LIGHTGRAY);
                DrawRectangleRec(rec4, (Color) {80, 80, 80, 130});
                DrawText("Mode < Hard >", menuX + 300, menuY + 460, 30, BLACK);
            }
        }
    }

    else if (currentMenuState == LOGIN_SCREEN) {

        // --- Draw Login UI ---
        DrawRectangle(menuX, menuY, menuWidth, menuHeight, BEIGE);
        DrawRectangleLines(menuX, menuY, menuWidth, menuHeight, DARKGRAY);
        DrawText("Login", menuX + 320, menuY + 40, 60, BLACK);

        DrawText("Username:", menuX + 100, menuY + 150, 30, BLACK);
        DrawRectangle(menuX + 260, menuY + 150, 400, 40, WHITE);

        DrawText("Password:", menuX + 100, menuY + 220, 30, BLACK);
        DrawRectangle(menuX + 260, menuY + 220, 400, 40, WHITE);

        if (!block) {
            // Type in username blank space
            DrawText(strcat(name, "[]"), menuX + 265, menuY + 155, 30, GRAY);
            DrawText("*********", menuX + 265, menuY + 225, 30, GRAY);
            int alpha = GetCharPressed();
            if ((alpha >= 32) && (alpha <= 126) && (letterCount < MAX_INPUT_CHARS))
            {
                name[letterCount] = (char)alpha;
                letterCount++;
            }

            alpha = GetCharPressed(); // Get next character from queue

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0) {
                    letterCount = 0;
                }
            }
            name[letterCount] = '\0'; // Null-terminate the string

            if (IsKeyPressed(KEY_ENTER)) {
                block = !block;
            }

        } else {
            DrawText(name, menuX + 265, menuY + 155, 30, GRAY);
            DrawText(strcat(pass, "[]"), menuX + 265, menuY + 225, 30, GRAY);
            int alpha = GetCharPressed();
            if ((alpha >= 32) && (alpha <= 126) && (passCount < MAX_INPUT_CHARS))
            {
                pass[passCount] = (char)alpha;
                passCount++;
            }

            alpha = GetCharPressed(); // Get next character from queue

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                passCount--;
                if (passCount < 0) {
                    passCount = 0;
                }
            }
            pass[passCount] = '\0'; // Null-terminate the string

            if (IsKeyPressed(KEY_ENTER) && block == true) {
                block = !block;

                if (strlen(name) >= 2 && strlen(pass) >= 2) {
                    currentMenuState = GAME;
                }
            }
        }

        if (!(strlen(name) >= 2 && strlen(pass) >= 2)) {
            DrawText("Fill The Blanks With 2 Or More Characters", menuX + 260, menuY + 280, 20, RED);
        }
        DrawText("Press Q to go back", menuX + 260, menuY + 300, 20, DARKGRAY);

        if (IsKeyPressed(KEY_UP) | IsKeyPressed(KEY_DOWN)) {
            block = !block;
        }

        if (IsKeyPressed(KEY_Q)) {
            currentMenuState = MAIN_MENU;
        }

        if (strlen(name) < 2) {
            DrawText("**", menuX + 665, menuY + 150, 20, RED);
        }

        if (strlen(pass) < 2) {
            DrawText("**", menuX + 665, menuY + 220, 20, RED);
        }

    }

    else if (currentMenuState == GAME) {
        // Menu rectangle position and size
        int menuWidth = 900;
        int menuHeight = 900;
        int menuX = (GetScreenWidth() - menuWidth) / 2;
        int menuY = (GetScreenHeight() - menuHeight) / 2;

        DrawRectangle(menuX, menuY, menuWidth, menuHeight, (Color) {4, 52, 68, 230});
        DrawRectangle(menuX + 250, menuY + 50, 400, 800, BLACK);
        DrawRectangle(menuX + 25, menuY + 50, 200, 150, LIGHTGRAY);
        DrawRectangle(menuX + 25, menuY + 250, 200, 150, LIGHTGRAY);
        DrawRectangle(menuX + 675, menuY + 50, 200, 250, LIGHTGRAY);
        Rectangle rec = {menuX + 680, menuY + 55, 190, 200};
        DrawRectangleLinesEx(rec, 3.0f, RED);
        DrawText("Next Puzzle", menuX + 680, menuY + 260, 19, BLACK);
        DrawText("Highest Score:", menuX + 50, menuY + 75, 21, BLACK);
        DrawText("Score:", menuX + 90, menuY + 275, 21, BLACK);

        srand(time(NULL));
        gridOffsetX = menuX + 250;
        gridOffsetY = menuY + 50;
        DrawNextTetromino(menuX + 675 + 20, menuY + 50 + 20);
        UpdateTetris(deltaTime);

        if (IsKeyPressed(KEY_Q)) {
            passCount = 0;
            letterCount = 0;
            currentMenuState = MAIN_MENU;
        }
    }

    return 0;
}
