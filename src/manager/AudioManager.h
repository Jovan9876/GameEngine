//
// Created by jerry on 2025-11-29.
//

#ifndef GAMEENGINE_AUDIOMANAGER_H
#define GAMEENGINE_AUDIOMANAGER_H

#include "SDL3_mixer/SDL_mixer.h"
#include <string>
#include <unordered_map>

class AudioManager {
    MIX_Mixer *mixer = nullptr;;
    MIX_Track *musicTrack = nullptr;
    static MIX_Track *sfxTrack;
    static std::unordered_map<std::string, MIX_Audio*> audio;

public:
    AudioManager();
    void loadAudio(const std::string& name, const char* path) const;

    void playMusic(const std::string& name) const;
    void stopMusic() const;

    static void playSfx(const std::string& name);
};

#endif //GAMEENGINE_AUDIOMANAGER_H