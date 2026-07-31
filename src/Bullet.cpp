#include "Bullet.hpp"

Bullet::Bullet(float x, float y, BulletOwner owner)
      : Entity(x, y, 4, 12), speed(500.0f), owner(owner) {
}

void Bullet::update(float deltaTime) {
  if (owner == BulletOwner::Player){
    y -= speed * deltaTime;
  } else {
    y += speed * deltaTime;
  }

  if (y + height < 0 || y > 600) {
    active = false;
  }
}

void Bullet::render(SDL_Renderer* renderer) {
  SDL_Rect rect = getBounds();

  if ( owner == BulletOwner::Player) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // amarelo
  } else {
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // magenta
  }
  SDL_RenderFillRect(renderer, &rect);
}

BulletOwner Bullet::getOwner() const {
  return owner;
}
