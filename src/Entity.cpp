#include "Entity.hpp"

Entity::Entity(float x, float y, int width, int height)
      : x(x), y(y), width(width), height(height), active(true), texture(nullptr) {
}

Entity::~Entity() {
}

void Entity::update(float deltaTime){
  // comportamento padrão vazio; classes filhas sobrescrevem isso
}

void Entity::render(SDL_Renderer* renderer) {
  SDL_Rect rect = getBounds();

  if (texture) {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
  } else {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
  }
}

SDL_Rect Entity::getBounds() const {
  return SDL_Rect{ static_cast<int>(x), static_cast<int>(y), width, height};
}

bool Entity::isActive() const {
  return active;
}

void Entity::destroy() {
  active = false;
}

void Entity::setTexture(SDL_Texture* texture) {
  this->texture = texture;
}
