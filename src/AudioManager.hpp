#ifndef AUDIO_MANAGER_HPP
#define AUDIO_MANAGER_HPP

#include <SDL2/SDL_mixer.h>
#include <string>
#include <unordered_map>

class AudioManager {
  public:
    AudioManager();
    ~AudioManager();

    bool loadSound(const std::string& id, const std::string& path);
    bool loadMusic(const std::string& id, const std::string& path);

    void playSound(const std::string& id);
    void playMusic(const std::string& id, int loops = -1);
    void stopMusic();

  private:
    std::unordered_map<std::string, Mix_Chunk*> sounds;
    std::unordered_map<std::string, Mix_Music*> musicTracks;
};

#endif
