#include "AudioManager.hpp"
#include <iostream>

AudioManager::AudioManager() {
}

AudioManager::~AudioManager(){
  for (auto& pair : sounds) {
    Mix_FreeChunk(pair.second);
  }
  sounds.clear();

  for (auto& pair : musicTracks) {
    Mix_FreeMusic(pair.second);
  }
  musicTracks.clear();
}

bool AudioManager::loadSound(const std::string& id, const std::string& path) {
  Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
  if (!chunk) {
    std::cerr << "Erro ao Carregar som " << path << ": " << Mix_GetError() << std::endl;
    return false;
  }
  sounds[id] = chunk;
  return true;
}

bool AudioManager::loadMusic(const std::string& id, const std::string& path) {
  Mix_Music* music = Mix_LoadMUS(path.c_str());
  if (!music) {
    std::cerr << "Erro ao carregar musica " << path << ": " << Mix_GetError() << std::endl;
    return false;
  }
  musicTracks[id] = music;
  return true;
}

void AudioManager::playSound(const std::string& id) {
  auto it = sounds.find(id);
  if (it == sounds.end()) {
    return;
  }
  Mix_PlayChannel(-1, it->second, 0);
}

void AudioManager::playMusic(const std::string& id, int loops) {
  auto it = musicTracks.find(id);
  if (it == musicTracks.end()) {
    return;
  }
  Mix_PlayMusic(it->second, loops);
}

void AudioManager::stopMusic() {
  Mix_HaltMusic();
}
