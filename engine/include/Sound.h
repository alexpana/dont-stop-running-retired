#pragma once

#include <memory>
#include <string>

namespace engine {

    class SoundSample;

    class SoundSampleDeleter{
    public:
        void operator()(SoundSample*);
    };

    typedef std::unique_ptr<SoundSample, SoundSampleDeleter> SoundSamplePtr;

    class Sound {

    public:
        Sound();

        void initialize();

        void cleanup();

        SoundSamplePtr loadSample(const std::string filename);

        void playSampleOnce(SoundSample *sample);

        void playSampleRepeated(SoundSample *sample);

    private:
        struct Implementation;
        Implementation *impl;
    };
}