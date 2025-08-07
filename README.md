# TetrisGame

#### Video Demo:  <URL HERE>

#### Description:
This project is a modern take on **Tetris**, one of the most iconic and influential puzzle games of all time. Originally developed by Alexey Pajitnov in 1984, Tetris has become a timeless classic known for its simple yet highly engaging gameplay. It has influenced generations of games and is celebrated for its perfect balance of strategy, reflexes, and spatial awareness.

Our version is implemented entirely in **C**, using the **raylib** graphics library for rendering and user interaction, and **SQLite3** for managing persistent user data. It retains the original charm of Tetris while introducing several modern enhancements such as animations, sound control, user accounts, and difficulty modes.

Whether you're a developer looking for a raylib + SQLite3 sample project, or just a fan of classic games, this Tetris clone demonstrates how a game can be built from scratch with basic yet powerful tools.

 ### Key Features:

- 🎮 **Gameplay Mechanics**
- Classic Tetris block system with rotation and line clearing.
- Two difficulty levels:
    - **Easy** – slower drop speed for relaxed play.
    - **Hard** – increased speed for a more intense challenge.
- Real-time score tracking.
- Next block preview for strategic planning.
- Game Over detection and graceful restart options.

- 👤 **User Authentication**
- Login system using usernames and passwords.
- User data is saved using **SQLite3**, including high scores.
- Each session is personalized — no need to start from scratch!

- 🖼️ **Visuals & Animation**
- Custom icons used throughout the UI for a polished look.
- "Star shower" animation adds a dynamic visual effect during gameplay.

- 🔊 **Audio**
- Sound effects enhance the user experience.
- Audio can be toggled on/off from the settings menu.

---

### How to Build & Run

1. **Dependencies**:
- [raylib](https://www.raylib.com/) – a simple and easy-to-use library to enjoy game programming.
- `sqlite3` – lightweight, embedded SQL database engine.

2. **Build Command** (example for Unix-like systems):

```bash

gcc main.c game.c menu.c -o tetris-game -lraylib -lsqlite3 -lm -ldl -lpthread
```

3. **Run the Game**:
```bash

./tetris-game
```

### Why Tetris?

Tetris is more than just a game — it's a study in minimalist game design. It has been used in cognitive research, stress relief, and even therapeutic settings. The simplicity of its rules contrasted with the depth of its challenge makes it a perfect candidate for programming projects, allowing developers to explore areas such as:

- Game loops and rendering

- Collision detection

- State management

- User input handling

- Persistent data storage

This project serves as a fun yet educational experience in building a fully functional game from scratch using C and open-source libraries.
