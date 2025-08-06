
// globals.h

#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>

// Enum for menu state
typedef enum {
    MAIN_MENU,
    LOGIN_SCREEN,
    GAME
} MenuState;

// Declare global variables
extern MenuState currentMenuState;
extern bool soundOn;
extern bool mode;
extern int shouldExit;
extern int speed;
extern int posX;
extern bool startGame;
extern bool block;
extern bool init;
extern int point;
extern const int numRows;
extern const int numCols;
extern const int cellWidth;
extern const int cellHeight;

#endif