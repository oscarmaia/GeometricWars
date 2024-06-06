#include <iostream>
#include "Game.h"
#include "Vec2.h"

int main() {
  Game game("./config/config.txt");
  game.run();
}
