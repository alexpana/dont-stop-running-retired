#include "Sound.h"

#include <iostream>

#include <SDL_mixer.h>

namespace engine {
    struct Sound::Implementation {
    };

    struct SoundSample {
    public:
        SoundSample(Mix_Chunk *mixChunk) : mixChunk(mixChunk) {}

        ~SoundSample() {
            Mix_FreeChunk(mixChunk);
        }

        Mix_Chunk *mixChunk;
    };


    Sound::Sound() {
    }

    void Sound::initialize() {
        Mix_Init(0);

        int error = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

        Mix_Volume(-1, 64);

        if (error == -1) {
            std::cout << "Could not initialize SDL_mixer. Reason: " << Mix_GetError() << std::endl;
        }
    }

    void Sound::cleanup() {
        Mix_CloseAudio();
        Mix_Quit();
    }

    SoundSamplePtr Sound::loadSample(const std::string filename) {
        Mix_Chunk *chunk = Mix_LoadWAV(filename.c_str());
        Mix_VolumeChunk(chunk, 64);

        return SoundSamplePtr(new SoundSample{chunk});
    }

    void Sound::playSampleOnce(SoundSample *sample) {
        Mix_PlayChannel(-1, sample->mixChunk, 0);
    }

    void Sound::playSampleRepeated(SoundSample *sample) {
        Mix_PlayChannel(-1, sample->mixChunk, -1);
    }

    void SoundSampleDeleter::operator()(SoundSample *sample) {
        delete sample;
    }
}
