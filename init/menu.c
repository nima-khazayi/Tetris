#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "raylib.h"
#include "globals.h"
#include "game.h"
#include <time.h>
#include <sqlite3.h>
#include <ctype.h>

#define MAX_INPUT_CHARS 20
char name[MAX_INPUT_CHARS + 1] = "\0"; // +1 for null terminator
char pass[MAX_INPUT_CHARS + 1] = "\0";
int letterCount = 0;
int passCount = 0;

const int numRows = 20;
const int numCols = 10;

const int cellWidth = 400 / numCols;
const int cellHeight = 800 / numRows;

int speed = 1;
int point = 0;

// Global variables for authentication state
int authError = 0; // 0: no error, 1: wrong password, 2: database error
int userHighestScore = 0;
int currentUserID = -1; // Store current user's database ID

// Function to trim whitespace from strings
void trim(char* str) {
    char* end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return; // All spaces?

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Null terminate
    *(end + 1) = 0;
}

// Initialize database and create users table if it doesn't exist
int initializeDatabase() {
    sqlite3* db;
    char* errMsg = 0;
    const char* sql = "CREATE TABLE IF NOT EXISTS users ("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "username TEXT UNIQUE NOT NULL,"
                     "password TEXT NOT NULL,"
                     "highest_score INTEGER DEFAULT 0,"
                     "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                     "last_login DATETIME DEFAULT CURRENT_TIMESTAMP);";
    
    const char* index_sql = "CREATE INDEX IF NOT EXISTS idx_username ON users(username);";
    
    int rc = sqlite3_open("database/users.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    // Execute SQL statements
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }

    rc = sqlite3_exec(db, index_sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }

    sqlite3_close(db);
    return 0;
}

// Authenticate or register user in database
int authenticateUser(const char* username, const char* password) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    const char* select_sql = "SELECT id, password, highest_score FROM users WHERE username = ?";
    const char* insert_sql = "INSERT INTO users (username, password, highest_score) VALUES (?, ?, 0)";
    const char* update_login_sql = "UPDATE users SET last_login = CURRENT_TIMESTAMP WHERE id = ?";
    
    // Trim input
    char trimmed_username[MAX_INPUT_CHARS + 1];
    char trimmed_password[MAX_INPUT_CHARS + 1];
    strcpy(trimmed_username, username);
    strcpy(trimmed_password, password);
    trim(trimmed_username);
    trim(trimmed_password);

    if (initializeDatabase() != 0) {
        return -2; // Database initialization error
    }

    int rc = sqlite3_open("database/users.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return -2; // Database connection error
    }
    
    // Prepare SQL statement to find existing user
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -2; // SQL preparation error
    }

    sqlite3_bind_text(stmt, 1, trimmed_username, -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        // User exists, check password
        currentUserID = sqlite3_column_int(stmt, 0);
        const char* stored_password = (const char*)sqlite3_column_text(stmt, 1);
        userHighestScore = sqlite3_column_int(stmt, 2);

        // Debug output
        printf("Stored password: %s\n", stored_password);
        printf("Input password: %s\n", trimmed_password);

        if (strcmp(trimmed_password, stored_password) == 0) {
            // Update last login timestamp
            sqlite3_finalize(stmt);
            rc = sqlite3_prepare_v2(db, update_login_sql, -1, &stmt, NULL);
            if (rc == SQLITE_OK) {
                sqlite3_bind_int(stmt, 1, currentUserID);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
            sqlite3_close(db);
            return 0; // Successful login
        } else {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return -1; // Wrong password
        }
    } else if (rc == SQLITE_DONE) {
        // User doesn't exist, create new user
        sqlite3_finalize(stmt);

        rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return -2; // SQL preparation error
        }

        sqlite3_bind_text(stmt, 1, trimmed_username, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, trimmed_password, -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (rc == SQLITE_DONE) {
            userHighestScore = 0; // New user starts with 0 score
            currentUserID = (int)sqlite3_last_insert_rowid(db);
            sqlite3_close(db);
            return 1; // New user registered successfully
        } else {
            sqlite3_close(db);
            return -2; // Database error
        }
    } else {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -2; // Database error
    }
}

// Update user's highest score in database
int updateHighestScore(const char* username, int newScore) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    const char* select_sql = "SELECT highest_score FROM users WHERE username = ?";
    const char* update_sql = "UPDATE users SET highest_score = ? WHERE username = ?";

    int rc = sqlite3_open("database/users.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return -1; // Database connection error
    }

    // Check current highest score
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1; // SQL preparation error
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        int currentHighest = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);

        // Only update if new score is higher
        if (newScore > currentHighest) {
            rc = sqlite3_prepare_v2(db, update_sql, -1, &stmt, NULL);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
                sqlite3_close(db);
                return -1; // SQL preparation error
            }

            sqlite3_bind_int(stmt, 1, newScore);
            sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);

            rc = sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            sqlite3_close(db);

            if (rc == SQLITE_DONE) {
                userHighestScore = newScore;
                return 0; // Successfully updated
            }
        } else {
            sqlite3_close(db);
            return 0; // No update needed
        }
    } else {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }

    return -1; // Error or user not found
}

int initializer() {

    float deltaTime = GetFrameTime() * speed;

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
                speed = 1;
            } else {
                DrawRectangle(menuX + 260, menuY + 440, 280, 70, LIGHTGRAY);
                DrawRectangleRec(rec4, (Color) {80, 80, 80, 130});
                DrawText("Mode < Hard >", menuX + 300, menuY + 460, 30, BLACK);
                speed = 2.5;
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
            char tempName[MAX_INPUT_CHARS + 3];
            strcpy(tempName, name);
            strcat(tempName, "_");
            DrawText(tempName, menuX + 265, menuY + 155, 30, BLACK);
            
            // Show password field as inactive
            DrawText("Enter username first...", menuX + 265, menuY + 225, 20, GRAY);
            
            int alpha = GetCharPressed();
            if ((alpha >= 32) && (alpha <= 126) && (letterCount < MAX_INPUT_CHARS))
            {
                name[letterCount] = (char)alpha;
                letterCount++;
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0) {
                    letterCount = 0;
                }
            }
            name[letterCount] = '\0'; // Null-terminate the string

            if (IsKeyPressed(KEY_ENTER) && strlen(name) >= 2) {
                block = true;
                authError = 0; // Clear any previous password errors
            }

        } else {
            // Show username (read-only now)
            DrawText(name, menuX + 265, menuY + 155, 30, BLACK);
            
            // Show password input with cursor
            char tempPass[MAX_INPUT_CHARS + 3];
            strcpy(tempPass, "");
            // Display asterisks for password
            for (int i = 0; i < passCount; i++) {
                strcat(tempPass, "*");
            }
            strcat(tempPass, "_");
            DrawText(tempPass, menuX + 265, menuY + 225, 30, BLACK);
            
            int alpha = GetCharPressed();
            if ((alpha >= 32) && (alpha <= 126) && (passCount < MAX_INPUT_CHARS))
            {
                pass[passCount] = (char)alpha;
                passCount++;
            }

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
                    int result = authenticateUser(name, pass);
                
                if (result == 0) {
                    // Successful login - existing user
                    currentMenuState = GAME;
                    authError = 0;
                } else if (result == 1) {
                    // Successful registration - new user
                    currentMenuState = GAME;
                    authError = 0;
                } else if (result == -1) {
                    // Wrong password - clear password field and show error
                    authError = 1;
                    memset(pass, 0, sizeof(pass));
                    passCount = 0;
                    // Stay in password input mode
                } else {
                    // Database error
                    authError = 2;
                    memset(pass, 0, sizeof(pass));
                    passCount = 0;
                }

                }
            }
        }

        // Display error messages and instructions
        if (authError == 1) {
            DrawText("Incorrect password! Please try again.", menuX + 260, menuY + 280, 20, RED);
        } else if (authError == 2) {
            DrawText("Database error! Please try again later.", menuX + 260, menuY + 280, 20, RED);
        } else if (!block && strlen(name) < 2) {
            DrawText("Username must be at least 2 characters", menuX + 260, menuY + 280, 20, ORANGE);
        } else if (block && strlen(pass) < 2) {
            DrawText("Password must be at least 2 characters", menuX + 260, menuY + 280, 20, ORANGE);
        } else if (block) {
            DrawText("New users will be registered automatically", menuX + 260, menuY + 280, 20, GREEN);
        }
        
        DrawText("Press Q to go back | Use ENTER to proceed", menuX + 260, menuY + 310, 18, DARKGRAY);
        DrawText("Use UP/DOWN to switch between fields", menuX + 260, menuY + 330, 18, DARKGRAY);

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
        char highScoreText[32];
        sprintf(highScoreText, "%d", userHighestScore);
        DrawText(highScoreText, menuX + 75, menuY + 100, 25, BLACK);
        DrawText("Score:", menuX + 90, menuY + 275, 21, BLACK);
        char Score[32];
        sprintf(Score, "%d", point);
        DrawText(Score, menuX + 115, menuY + 300, 25, BLACK);

        srand(time(NULL));
        gridOffsetX = menuX + 250;
        gridOffsetY = menuY + 50;
        DrawNextTetromino(menuX + 675 + 20, menuY + 50 + 20);
        UpdateTetris(deltaTime);

        if (gameOver) {
            // Show game over screen
            DrawText("Game Over!", menuX + 300, menuY + 250, 20, RED);
            DrawText("Press R to Restart", menuX + 300, menuY + 290, 20, LIGHTGRAY);

            if (IsKeyPressed(KEY_R)) {
                gameOver = false; // Reset game over flag
                updateHighestScore(name, point);
                startNewGame();   // Restart the game
            }
        }

        if (IsKeyPressed(KEY_Q)) {
            updateHighestScore(name, point);
            passCount = 0;
            letterCount = 0;
            startNewGame(); // Reset game state
            currentMenuState = MAIN_MENU;
        }
    }

    return 0;
}
