#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SDL2/SDL.h>

class Entity {
  public:
    Entity(float x, float y, int width, int height);
    virtual ~Entity();

    virtual void update(float deltaTime);
    virtual void render(SDL_Renderer* renderer);

    SDL_Rect getBounds() const;
    bool isActive() const;
    void destroy();

    void setTexture(SDL_Texture* texture);

  protected:
    float x, y;
    int width, height;
    bool active;

    SDL_Texture* texture;
};

#endif
