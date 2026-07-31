#include "TextRenderer.hpp"
#include <iostream>

TextRenderer::TextRenderer() : font(nullptr) {
}

TextRenderer::~TextRenderer() {
  if (font) {
    TTF_CloseFont(font);
    font = nullptr;
  }
}

bool TextRenderer::loadFont(const std::string& path, int size) {
  font = TTF_OpenFont(path.c_str(), size);
  if (!font) {
    std::cerr << "Erro ao carregar fonte: " << TTF_GetError() << std::endl;
    return false;
  }
  return true;
}

void TextRenderer::drawText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color) {
  if (!font) {
    return;
  }

  SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
  if (!surface) {
    std::cerr << "Erro ao renderizar text: " << TTF_GetError() << std::endl;
    return;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  if (!texture) {
    std::cerr << "Erro ao criar texture: " << SDL_GetError() << std::endl;
    return;
  }

  int textWidth, textHeight;
  SDL_QueryTexture(texture, nullptr, nullptr, &textWidth, &textHeight);

  SDL_Rect destRect{ x, y, textWidth, textHeight};
  SDL_RenderCopy(renderer, texture, nullptr, &destRect);

  SDL_DestroyTexture(texture);
}
