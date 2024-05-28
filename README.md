## GeometricWars - Game Overview

The game involves various entities such as players and enemies, each with their own unique properties and behaviors. The game's configuration is defined in a configuration file, with each line specifying different aspects of the game such as window size, font format, player, bullet specification, and enemy specifications.

## Game Entities

### Player

The player is represented by a shape which is defined in the config file. The player spawns in the center of the screen and moves in the direction specified by the WASD keys. The player is confined to move only within the bounds of the window. The player will shoot a bullet toward the mouse pointer when the left mouse button is clicked. The speed, size, and lifespan of the bullets are read from the config file.

### Enemies

Enemies spawn in a random location on the screen every x frame where X is defined in the configuration file. Enemies must not overlap the sides of the screen at the time of spawn. Enemy shapes have a random number of vertices, between a given minimum and maximum number, which is specified in the config file. Enemy shape radius will also be specified in the configuration file. Enemies will be a given random color upon spawning. Enemies will be given a random speed upon spawning, between a minimum and maximum number, which is specified in the config file. When an enemy reaches the edge of the window, it should bounce off in the opposite direction at the same speed.

## Special Effects

Any special effects which do not alter gameplay can be added for up to 5% bonus marks on the assignment. Note that assignments cannot go above 100% total marks, but the 5% bonus can overwrite any marks lost in other areas of the assignment.

## Game Controls

- "P" key pauses the game.
- "ESC" key should close the game

## Configuration File

The configuration file will have one line each specifying the window size, font format, player, bullet specification, and enemy specifications.

## Tree Architecture

- Components.h
- Entity.cpp
- Entity.h
- EntityManager.cpp
- EntityManager.h
- Game.cpp
- Game.h
- main.cpp
- Vec2.cpp
- Vec2.h



# Configuration File

The configuration file will have one line each specifying the window size, font format, player, bullet specification, and enemy specifications.

```
Window 1280 720 60 0
Font fonts/tech.ttf 24 255 255 255
Player 32 32 5 5 5 5 255 00 4 8
Enemy 32 32 3 3 255 255 255 2 3 8 90 60
Bullet 10 10 20 255 255 255 255 255 255 2 20 90
```

Window W H FL FS
- W(int width) H(int height) FL(int frame) FS(full screen mode or not 0/1)

Font F S R G B
- F(string filename) S(int size) RGB(int colors)

Player SR CR S FR FG FB OR OG OB OT V
- SR(int shape radius) CR(int collision radius) S(float speed) Frgb(int fill color) Orgb(int outline color) OT(int outline tickness) V(int shape vertices)

Enemy SR CR SMIN SMAX OR OG OB OT VMIN VMAX L SI
- SR(int shape radius) CR(int collision radius) SMIN-SMAX(float range speed) O-RGB(int outline color) OT(int outline tickness) VMIN-VMAX(int range vertices) L (int Small entt life span) SI(int Spawn interval)

Bullet SR CR S FR FG FB OR OG OB OT V L
- SR(int shape radius) CR(int collision radius) S(float speed) Frgb(int fill color) Orgb(int outline color) OT(int outline tickness) V(int shape vertices) L(int Life span)