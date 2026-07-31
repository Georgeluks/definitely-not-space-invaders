#ifndef ENEMY_HPP
#define ENEMY_HPP 

#include "Entity.hpp"

class Enemy : public Entity {
  public:
    Enemy(float x, float y);

    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;
    
    void moveBy(float dx, float dy);

  private:
    // Enemy nao precisa de logica propria de movimento
    // pois quem decide o quanto ou quando mover é o game (via moveBy)
    // visto que o movimento é coordenado entre todos os inimigos do grid

};

#endif
