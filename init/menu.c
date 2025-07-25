#include <stdlib.h>
#include "raylib.h"
#include "globals.h"

int initializer() {

    // Menu rectangle position and size
    int menuWidth = 800;
    int menuHeight = 600;
    int menuX = (GetScreenWidth() - menuWidth) / 2;
    int menuY = (GetScreenHeight() - menuHeight) / 2;
    bool exit = false;

    DrawRectangle(menuX, menuY, menuWidth, menuHeight, LIGHTGRAY);
    Rectangle rec = {menuX + 40, menuY + 100, 720, 450};
    Rectangle rec1 = {menuX + 257, menuY + 200, 280, 60};
    Rectangle rec2 = {menuX + 277, menuY + 290, 240, 60};
    DrawRectangleLinesEx(rec, 5.0f, RED);
    

    // Add title or button text
    DrawText("Main Menu", menuX + 200, menuY + 10, 80, BLACK);
    DrawText("Start", menuX + 340, menuY + 210, 40, DARKGRAY);
    DrawText("Exit", menuX + 358, menuY + 300, 40, DARKGRAY);

    if (IsKeyPressed(KEY_UP) | IsKeyPressed(KEY_DOWN)) {
        if (!shouldExit) {
            shouldExit = true; // Set exit to true if it wasn't already
        } else {
            shouldExit = false;
        }
    }

    if (!shouldExit) {
        DrawRectangleRec(rec1, (Color) {80, 80, 80, 130});
        DrawText("Start", menuX + 340, menuY + 210, 40, BLACK);
        
    } else {
        DrawRectangleRec(rec2, (Color) {80, 80, 80, 130});
        DrawText("Exit", menuX + 358, menuY + 300, 40, BLACK);

        if (IsKeyPressed(KEY_ENTER)) {
            return 1;
        }
    }
    return 0;
}
