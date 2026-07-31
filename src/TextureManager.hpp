#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

class TextureManager {
  public: 
    TextureManager();
    ~TextureManager();

    bool load(SDL_Renderer* renderer, const std::string& id, const std::string& path);
    SDL_Texture* get(const std::string& id) const;

  private:
    std::unordered_map<std::string, SDL_Texture*> textures;
};

#endif
