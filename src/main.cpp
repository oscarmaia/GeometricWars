#include <iostream>
#include "Game.h"
#include "Vec2.h"

int main() {
  Vec2 a = Vec2(100, 200);
  Vec2 b = Vec2(400, 700);
  a.normalized(b);
  std::cout << a.normalized(b).y;
  Game game("./config/config.txt");
  game.run();
}
