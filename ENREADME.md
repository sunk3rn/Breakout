## [CZ verze](README.md)
# 🧱 Breakout 
Project for my Introduction to programing course at university, the game Breakout written in C with the SDL library.

## 📕 Rules
Using a paddle, the player must hit as many bricks as possible with a ball by bouncing it off walls and/or the paddle beneath them to hit the bricks and destroy them. If the player's paddle misses the ball's rebound, they lose a life. The player has three lives to clear the screen of bricks.

# 🕹️ Popis hry:

## 🏗️ Build Instructions

To compile the game, you need:
   - CMake
   - SDL2
   - SDL2-ttf

```bash
mkdir build
cd build
cmake ..
make && ./hra
```

## Before starting the game
-   The initial game field can be set in the starting-field.txt file. If this file is improperly formatted or does not exist, the default field configuration will be loaded.
-    The valid format is a 15x15 grid, and each block can have from 0 to 11 lives, with lives affecting its color. 0 = Empty space, an empty block.
-    The three best scores are saved in the score.txt file from highest to lowest. If the score is lower than the lowest, it won't be saved.

## 🎮 How to play

### 🎛️ Controls
-    In the menu, you can navigate using the arrow keys and confirm with the `Enter` key.
-    Once you start the game, press the spacebar to launch the ball. You can move the paddle either with the mouse or arrow keys (depending on the settings).


### ⚙️ Settings
- You can configure the controls using either the mouse or the keyboard and set the number of lives at the start of the game.

## Known issues:
-    The ball behaves chaotically when bouncing off the sides of the blocks.
-    Ball bounces are not random.