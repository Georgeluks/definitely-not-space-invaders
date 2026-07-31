#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"

class Player : public Entity {
  public:
    Player(float x, float y);

    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

    void moveLeft();
    void moveRight();
    void stop();
    
    void takeDamage();
    int getLives() const;
    bool isDead() const;

  private:
    float speed; //pixels por segundo
    int direction; //-1 = esquerda, 0 parado, 1 = direita
    int screenWidth;
    
    int lives;
};

#endif
