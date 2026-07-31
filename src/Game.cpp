#include "Game.hpp"
#include <iostream>
#include <cstdlib> // rand()
#include <ctime>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <SDL2/SDL_image.h>

Game::Game() : window(nullptr), renderer(nullptr), running(false), lastFrameTime(0), 
              deltaTime(0.0f), player(nullptr), spacePressedLastFrame(false), score(0),
              enemyGrid(nullptr), screenWidth(800), state(GameState::Menu),
              enemyShootTimer(0.0f), enemyShootInterval(1.2f), titleText(nullptr), uiText(nullptr),
              textureManager(nullptr), audioManager(nullptr) {
}

Game::~Game() {
  clean();
}

bool Game::init(const char* title, int width, int height) {
  srand(static_cast<unsigned int>(time(nullptr)));

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
    return false;
  }

  if (TTF_Init() != 0) {
    std::cerr << "Erro ao inicializar TTF: " << TTF_GetError() << std::endl;
    return false;
  }

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    std::cerr << "Erro ao inicializar IMG: " << IMG_GetError() << std::endl;
    return false;
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0){
    std::cerr << "Erro ao inicializar Mixer: " << Mix_GetError() << std::endl;
    return false;
  }

  window = SDL_CreateWindow(
      title,
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      width, height,
      SDL_WINDOW_SHOWN
  );
  if (!window) {
    std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    std::cerr << "Erro ao criar o renderer" << SDL_GetError() << std::endl;
    return false;
  }

  textureManager = new TextureManager();
  textureManager->load(renderer, "player", "assets/sprites/player.png");
  textureManager->load(renderer, "red", "assets/sprites/red.png");

  titleText = new TextRenderer();
  if (!titleText->loadFont("assets/fonts/DejaVuSans-Bold.ttf", 48)) {
    return false;
  }

  uiText = new TextRenderer();
  if (!uiText->loadFont("assets/fonts/DejaVuSans-Bold.ttf", 24)) {
    return false;
  }

  audioManager = new AudioManager();
  audioManager->loadSound("shoot", "assets/sounds/bullet.mp3");
  audioManager->loadSound("explosion", "assets/sounds/enemy_death.mp3");
  audioManager->loadSound("hit", "assets/sounds/hit.mp3");
  audioManager->loadSound("game_over", "assets/sounds/game_over.mp3");
  audioManager->loadMusic("theme", "assets/sounds/bg_music.mp3");


  running = true;
  lastFrameTime = SDL_GetTicks64();
  return true;
}

void Game::run() {
  while (running) {
    // Calcula Delta Time 
    Uint64 currentFrameTime = SDL_GetTicks64();
    deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f; //ms -> segundos
    lastFrameTime= currentFrameTime;

    handleEvents();
    update();
    render();
    
    // Limita a ~60 FPS
    SDL_Delay(16);
  }
}

void Game::handleEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      running = false;
    }
  }
}

void Game::update() {
  switch (state) {
    case GameState::Menu:
      updateMenu();
      break;
    case GameState::Playing:
      updatePlaying();
      break;
    case GameState::GameOver:
      updateGameOver();
      break;
  }
}


//criar render()
void Game::render() {
  switch (state) {
    case GameState::Menu:
      renderMenu();
      break;
    case GameState::Playing:
      renderPlaying();
      break;
    case GameState::GameOver:
      renderGameOver();
      break;
  }
}

//criar clean()
void Game::clean() {
  delete player;
  player = nullptr;
  
  for (Bullet* bullet : bullets) {
    delete bullet;
  }
  bullets.clear();
  
  delete enemyGrid;
  enemyGrid = nullptr;

  delete titleText;
  titleText = nullptr;

  delete uiText;
  uiText = nullptr;

  delete textureManager;
  textureManager = nullptr;

  delete audioManager;
  audioManager = nullptr;

  if (renderer) {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
  }

  if (window) {
    SDL_DestroyWindow(window);
    window = nullptr;
  }
  
  TTF_Quit();
  IMG_Quit();
  Mix_CloseAudio();
  SDL_Quit();
}

void Game::spawnBullet() {
  SDL_Rect playerBounds = player->getBounds();
  float bulletX = playerBounds.x + (playerBounds.w / 2.0f) - 2.0f; 
  float bulletY = playerBounds.y;

  bullets.push_back(new Bullet(bulletX, bulletY, BulletOwner::Player));

  audioManager->playSound("shoot");
}

void Game::checkCollisions() {
  GameState previousState = state;

  // --- Bullet vs Enemy --- 
  for (Bullet* bullet : bullets) {
    if (!bullet->isActive()) continue;

    SDL_Rect bulletBounds = bullet->getBounds();
    
    if (bullet->getOwner() == BulletOwner::Player) {
      //bullet do player checa contra enemies 
      for (Enemy* enemy : enemyGrid->getEnemies()) {
        if (!enemy->isActive()) continue;

        SDL_Rect enemyBounds = enemy->getBounds();

        if (SDL_HasIntersection(&bulletBounds, &enemyBounds)) {
          bullet->destroy();
          enemy->destroy();
          score += 10;
          audioManager->playSound("explosion");

          break;
        }
      }
    } else {
      // bullet do enemy: checa contra o player 
      SDL_Rect playerBounds = player->getBounds();
      if (SDL_HasIntersection(&bulletBounds, &playerBounds)){
        bullet->destroy();
        player->takeDamage();
        audioManager->playSound("hit");

        if (player->isDead()) {
          state = GameState::GameOver;
        }
      }
    }
  }

  // --- Enemy vs Player -- 
  SDL_Rect playerBounds = player->getBounds();
  for (Enemy* enemy : enemyGrid->getEnemies()) {
    if (!enemy->isActive()) continue;

    SDL_Rect enemyBounds = enemy->getBounds();
    if (SDL_HasIntersection(&playerBounds, &enemyBounds)) {
      enemy->destroy();
      player->takeDamage();
      audioManager->playSound("hit");
      
      if (player->isDead()) {
        state = GameState::GameOver;
      }
    }
  }

  if (previousState != GameState::GameOver && state == GameState::GameOver) {
    audioManager->stopMusic();
    audioManager->playSound("game_over");
  }
}

void Game::resetGame() {
  delete player;
  for (Bullet* bullet : bullets) {
    delete bullet;
  }

  player = new Player(380.0f, 550.0f);
  player->setTexture(textureManager->get("player"));
  
  if (!enemyGrid) {
    enemyGrid = new EnemyGrid(screenWidth);
  }
  enemyGrid->spawn(textureManager->get("red"));
  
  score = 0;
  enemyShootTimer = 0.0f;

  audioManager->playMusic("theme");
}

void Game::updateMenu() {
  const Uint8* keystate = SDL_GetKeyboardState(nullptr);
  bool spacePressedNow = keystate[SDL_SCANCODE_SPACE];

  if (spacePressedNow && !spacePressedLastFrame) {
    resetGame();
    state = GameState::Playing;
  }
  spacePressedLastFrame = spacePressedNow;
}

void Game::updatePlaying() {
  const Uint8* keystate = SDL_GetKeyboardState(nullptr);

  if (keystate[SDL_SCANCODE_LEFT]) {
    player->moveLeft();
  } else if (keystate[SDL_SCANCODE_RIGHT]) {
    player->moveRight();
  } else {
    player->stop();
  }

  bool spacePressedNow = keystate[SDL_SCANCODE_SPACE];
  if (spacePressedNow && !spacePressedLastFrame) {
    spawnBullet();
  }
  spacePressedLastFrame = spacePressedNow;

  player->update(deltaTime);

  for (Bullet* bullet : bullets) {
    bullet->update(deltaTime);
  }
  for (auto it = bullets.begin(); it != bullets.end(); ) {
    if (!(*it)->isActive()) {
      delete *it;
      it = bullets.erase(it);
    } else {
      ++it;
    }
  }

  enemyGrid->update(deltaTime);
  updateEnemyShooting(deltaTime);
  checkCollisions();

}

void Game::updateGameOver() {
  const Uint8* keystate = SDL_GetKeyboardState(nullptr);
  bool spacePressedNow = keystate[SDL_SCANCODE_SPACE];

  if (spacePressedNow && !spacePressedLastFrame) {
    state = GameState::Menu;
  }

  spacePressedLastFrame = spacePressedNow;
}

void Game::renderMenu() {
  SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255); // cinza escuro
  SDL_RenderClear(renderer);

  SDL_Color white{ 255, 255, 255, 255 };
  titleText->drawText(renderer, "DEFINITELY NOT SPACE INVADERS", 180, 220, white);
  uiText->drawText(renderer, "PRESS SPACE TO START", 250, 320, white);

  SDL_RenderPresent(renderer);
}

void Game::renderPlaying() {
  SDL_SetRenderDrawColor(renderer, 10, 10, 40, 255);
  SDL_RenderClear(renderer);

  player->render(renderer);
  for (Bullet* bullet : bullets) {
    bullet->render(renderer);
  }
  
  enemyGrid->render(renderer);

  SDL_Color white{ 255, 255, 255, 255 };
  std::string scoreText = "Score: " + std::to_string(score);
  std::string livesText = "Lives: " + std::to_string(player->getLives());

  uiText->drawText(renderer, scoreText, 10, 10, white);
  uiText->drawText(renderer, livesText, 10, 40, white);

  SDL_RenderPresent(renderer);
}

void Game::renderGameOver() {
  SDL_SetRenderDrawColor(renderer, 80, 10, 10, 255);
  SDL_RenderClear(renderer);

  SDL_Color red{ 255, 0, 0, 1};
  SDL_Color white{ 255, 255, 255, 255};
  titleText->drawText(renderer, "GAME OVER", 255, 220, red);

  std::string finalScore = "Final Score: " + std::to_string(score);
  uiText->drawText(renderer, finalScore, 280, 300, white);
  uiText->drawText(renderer, "PRESS SPACE FOR MENU", 240, 340, white);

  SDL_RenderPresent(renderer);
}

void Game::spawnEnemyBullet() {
  Enemy* shooter = enemyGrid->getRandomShooter();
  if (!shooter) {
    return;
  }

  SDL_Rect bounds = shooter->getBounds();
  float bulletX = bounds.x + (bounds.w / 2.0f) - 2.0f;
  float bulletY = bounds.y + bounds.h;

  bullets.push_back(new Bullet(bulletX, bulletY, BulletOwner::Enemy));
}

void Game::updateEnemyShooting(float deltaTime) {
  enemyShootTimer += deltaTime;

  if (enemyShootTimer >= enemyShootInterval) {
    spawnEnemyBullet();
    enemyShootTimer = 0.0f;
  }
}
