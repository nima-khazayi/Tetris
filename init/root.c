#include <stdlib.h>
#include "raylib.h"
#include "menu.h"

void initializer(Texture2D stars[4]) {

    // Menu Screen!        
    float posY = 0;
    int magik = GetRandomValue(0, 3);
    int posX = GetRandomValue(0, 1800);
    Vector2 ImagePos = {posX, posY};

    DrawTextureEx(stars[magik], ImagePos, 0, .01f, WHITE);
}
