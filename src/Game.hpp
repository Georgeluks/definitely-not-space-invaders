#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL.h>
#include <vector>
#include "Player.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "EnemyGrid.hpp"
#include "TextRenderer.hpp"
#include "TextureManager.hpp"
#include "AudioManager.hpp"

enum class GameState {
  Menu,
  Playing,
  GameOver 
};


class Game {
  public:
    Game();
    ~Game();

    bool init(const char* title, int width, int height);
    void run();

  private:
    void handleEvents();
    void update();
    void render();
    void clean();

    void spawnBullet();
    void spawnEnemyBullet();
    void updateEnemyShooting(float deltaTime);
    void checkCollisions();
 
    void resetGame();

    void updateMenu();
    void updatePlaying();
    void updateGameOver();

    void renderMenu();
    void renderPlaying();
    void renderGameOver();

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    Uint64 lastFrameTime;
    float deltaTime;

    Player* player;
    std::vector<Bullet*> bullets;
    EnemyGrid* enemyGrid;

    bool spacePressedLastFrame;

    int screenWidth;
    float enemyShootTimer;
    float enemyShootInterval;
    int score;
    GameState state;

    TextRenderer* titleText;
    TextRenderer* uiText;

    TextureManager* textureManager;

    AudioManager* audioManager;
};

#endif
