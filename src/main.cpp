#include "Game.hpp"

int main() {
  Game game;

  if (!game.init("Space Invaders", 800, 600)) {
    return 1;
  }

  game.run();

  return 0;
}
