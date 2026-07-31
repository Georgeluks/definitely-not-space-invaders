#include "EnemyGrid.hpp"
#include <cstdlib>

EnemyGrid::EnemyGrid(int screenWidth)
          : direction(1.0f), speed(60.0f), screenWidth(screenWidth){
}

EnemyGrid::~EnemyGrid() {
  clear();
}

void EnemyGrid::clear() {
  for (Enemy* enemy : enemies) {
    delete enemy;
  }
  enemies.clear();
}

void EnemyGrid::spawn(SDL_Texture* enemyTexture) {
  clear();
  direction = 1.0f;

  const int rows = 4;
  const int cols = 8;
  const int spacingX = 60;
  const int spacingY = 45;
  const int startX = 60;
  const int startY = 50;

  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      float x = startX + col * spacingX;
      float y = startY + row * spacingY;
      Enemy* enemy = new Enemy(x, y);
      enemy->setTexture(enemyTexture);
      enemies.push_back(enemy);
    }
  }
}

void EnemyGrid::update(float deltaTime) {
  bool shouldDescendAndReverse = false;

  for (Enemy* enemy : enemies) {
    SDL_Rect bounds = enemy->getBounds();
    float nextX = bounds.x + direction * speed * deltaTime;

    if (nextX < 0 || nextX + bounds.w > screenWidth) {
      shouldDescendAndReverse = true;
      break;
    }
  }

  for (Enemy* enemy : enemies) {
    if (shouldDescendAndReverse) {
      enemy->moveBy(0, 20.0f);
    } else {
      enemy->moveBy(direction * speed * deltaTime, 0);
    }
  }

  if (shouldDescendAndReverse) {
    direction *= -1.0f;
  }

  // Remove enemies inativos (destruídos por colisão) 
  for (auto it = enemies.begin(); it != enemies.end(); ) {
    if (!(*it)->isActive()) {
      delete *it;
      it = enemies.erase(it);
    } else {
      ++it;
    }
  }
}

void EnemyGrid::render(SDL_Renderer* renderer) {
  for (Enemy* enemy : enemies) {
    enemy->render(renderer);
  }
}

std::vector<Enemy*>& EnemyGrid::getEnemies() {
  return enemies;
}

bool EnemyGrid::isEmpty() const {
  return enemies.empty();
}

Enemy* EnemyGrid::getRandomShooter() const {
  if (enemies.empty()) {
    return nullptr;
  }
  int index = rand() % enemies.size();
  return enemies[index];
}
