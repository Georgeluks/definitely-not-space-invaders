#ifndef TEXT_RENDERER_HPP
#define TEXT_RENDERER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class TextRenderer {
  public:
    TextRenderer();
    ~TextRenderer();

    bool loadFont(const std::string& path, int size);
    void drawText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color);

  private:
    TTF_Font* font;
};

#endif
