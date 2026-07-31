#include "Enemy.hpp"

Enemy::Enemy(float x, float y)
      : Entity(x,y,30,20) {
}

void Enemy::update(float deltaTime) {
  //aqui não vai nada pois será atualizado no Game.cpp 
}

void Enemy::render(SDL_Renderer* renderer) {
  if (texture) {
    Entity::render(renderer); 
  } else {
    SDL_Rect rect = getBounds();
    SDL_SetRenderDrawColor(renderer, 255, 60, 60, 255); // vermelho
    SDL_RenderFillRect(renderer, &rect);
  }
}

void Enemy::moveBy(float dx, float dy) {
  x += dx;
  y += dy;
}
