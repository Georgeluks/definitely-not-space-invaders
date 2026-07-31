#include "TextureManager.hpp"
#include <SDL2/SDL_image.h>
#include <iostream>

TextureManager::TextureManager() { 
}

TextureManager::~TextureManager() {
  for (auto& pair : textures) {
    SDL_DestroyTexture(pair.second);
  }
  textures.clear();
}

bool TextureManager::load(SDL_Renderer* renderer, const std::string& id, const std::string& path) {
  SDL_Surface* surface = IMG_Load(path.c_str());
  if (!surface) {
    std::cerr << "Erro ao carregar imagem " << path << ": " << IMG_GetError() << std::endl;
    return false;
  }

  SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  if (!newTexture) {
    std::cerr << "Erro ao criar textura de " << path << ": " << SDL_GetError() << std::endl;
    return false;
  }

  textures[id] = newTexture;
  return true;
}

SDL_Texture* TextureManager::get(const std::string& id) const {
  auto it = textures.find(id);
  if (it == textures.end()) {
    return nullptr;
  }

  return it->second;
}
