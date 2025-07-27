#include <stdio.h>
#include "raylib.h"
#include "init/menu.h"
#include "init/globals.h" 

#define STAR_COUNT 300

typedef struct {
    float x;
    float y;
    float speed;
    bool active;
    Texture2D *texture;  // Pointer to one of the 4 star textures

} Star;

MenuState currentMenuState = MAIN_MENU;
Star stars[STAR_COUNT];
int screenWidth = 1920;
int screenHeight = 1080;

int activeStars = 0;
float spawnTimer = 0.0f;
float spawnInterval = 0.02f; // seconds between spawns (adjust for faster/slower)

Texture2D starTextures[4];  // Array of star images

int shouldExit = 206; // Global variable
bool block = false;
bool soundOn = true;
bool startGame = false;
bool mode = true;

int main() {
    
    InitWindow(screenWidth, screenHeight, "TeTRo");
    ToggleFullscreen();

    starTextures[0] = LoadTexture("templates/1.png");
    starTextures[1] = LoadTexture("templates/2.png");
    starTextures[2] = LoadTexture("templates/3.png");
    starTextures[3] = LoadTexture("templates/4.png");

    for (int i = 0; i < STAR_COUNT; i++) {
        stars[i].active = false;
    }

    // Main Game Loop!
    while (!WindowShouldClose())
    {

        float deltaTime = GetFrameTime();
        spawnTimer += deltaTime;

        // Spawn logic
        if (spawnTimer >= spawnInterval && activeStars < STAR_COUNT) {
            spawnTimer = 0.0f;

            stars[activeStars].x = GetRandomValue(0, GetScreenWidth() - 32);  // assuming 32px texture width
            stars[activeStars].y = 0;
            stars[activeStars].speed = GetRandomValue(50, 150);
            stars[activeStars].texture = &starTextures[GetRandomValue(0, 3)];
            stars[activeStars].active = true;

            activeStars++;
        }

        BeginDrawing();

        ClearBackground(BLACK);
        for (int i = 0; i < activeStars; i++) {
            if (stars[i].active) {
                stars[i].y += stars[i].speed * deltaTime;

                if (stars[i].y > GetScreenHeight()) {
                    stars[i].y = 0;
                    stars[i].x = GetRandomValue(0, GetScreenWidth() - 32);
                }

                DrawTextureEx(*stars[i].texture, (Vector2){(int)stars[i].x, (int)stars[i].y}, 0.0f, 0.01f, WHITE);
            }
        }

        if (activeStars >= STAR_COUNT) {
            // Call initializer and check if exit is requested
            if (initializer() == 1) {
                break;
            }
        }

        EndDrawing();
    }

    for (int i = 0; i < 4; i++) {
        UnloadTexture(starTextures[i]);
    }

    CloseWindow();

    return 0;
}
