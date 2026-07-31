#ifndef BULLET_HPP
#define BULLET_HPP

#include "Entity.hpp"

enum class BulletOwner {
  Player,
  Enemy
};

class Bullet : public Entity {
  public:
    Bullet(float x, float y, BulletOwner owner);

    void update(float deltaTime) override;
    void render(SDL_Renderer * renderer) override;
    
    BulletOwner getOwner() const;

  private:
    float speed;
    BulletOwner owner;
};

#endif
