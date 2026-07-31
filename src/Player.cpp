#include "Player.hpp"

Player::Player(float x, float y)
      : Entity(x, y, 40, 20), speed(300.0f), direction(0), 
      screenWidth(801), lives(3) {
}

void Player::update(float deltaTime) {
  x += direction * speed *deltaTime;

  // Trava o player dentro da tela 
  if (x < 0) {
    x = 0;
  }
  if (x + width > screenWidth){
    x = screenWidth - width;
  }
}

void Player::render(SDL_Renderer* renderer) {
  if (texture) {
    Entity::render(renderer); 
  } else {
    SDL_Rect rect = getBounds();
    SDL_SetRenderDrawColor(renderer, 0, 255, 100, 255); //verde
    SDL_RenderFillRect(renderer, &rect);
  }
}

void Player::moveLeft() {
  direction = -1;
}

void Player::moveRight() {
  direction = 1;
}

void Player::stop() {
  direction = 0;
}

void Player::takeDamage() {
  lives--;
  if (lives <= 0) {
    lives = 0;
    active = false;
  }
}

int Player::getLives() const {
  return lives;
}

bool Player::isDead() const {
  return !active;
}
