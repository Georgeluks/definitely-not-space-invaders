#ifndef ENEMY_GRID_HPP
#define ENEMY_GRID_HPP

#include <vector>
#include "Enemy.hpp"

class EnemyGrid {
  public:
    EnemyGrid(int screenWidth);
    ~EnemyGrid();

    void spawn(SDL_Texture* enemyTexture);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    std::vector<Enemy*>& getEnemies();
    bool isEmpty() const;
    Enemy* getRandomShooter() const;
    
  private:
    void clear();

    std::vector<Enemy*> enemies;

    float direction;
    float speed;
    int screenWidth;
};

#endif
