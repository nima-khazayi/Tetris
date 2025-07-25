#include <stdlib.h>
#include "raylib.h"

void initializer() {

    // Menu rectangle position and size
    int menuWidth = 800;
    int menuHeight = 600;
    int menuX = (GetScreenWidth() - menuWidth) / 2;
    int menuY = (GetScreenHeight() - menuHeight) / 2;

    DrawRectangle(menuX, menuY, menuWidth, menuHeight, LIGHTGRAY);

    // Add title or button text
    DrawText("Main Menu", menuX + 200, menuY + 10, 80, BLACK);
    DrawText("Start",     menuX + 340, menuY + 210, 40, DARKGRAY);
    DrawText("Exit",      menuX + 358, menuY + 270, 40, DARKGRAY);
}