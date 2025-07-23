#include "raylib.h"


int initializer() {

    InitWindow(800, 600, "TeTRo");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
