#include <stdio.h>
#include <math.h>
#include "init/root.h"
#include "raylib.h"


int main() {
    
    InitWindow(1800, 900, "TeTRo");
    Texture2D star_1;
    Texture2D star_2;
    Texture2D star_3;
    Texture2D star_4;
    star_1 = LoadTexture("templates/1.png");
    star_2 = LoadTexture("templates/2.png");
    star_3 = LoadTexture("templates/3.png");
    star_4 = LoadTexture("templates/4.png");
    Texture2D stars[4] = {star_1, star_2, star_3, star_4};

    // Main Game Loop!
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);
        initializer(stars);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
