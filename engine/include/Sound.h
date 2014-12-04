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

        // Volume interval is (0, 1)
        void setVolume(double volume);

        void setSampleVolume(SoundSample* sample, double volume);

    private:
        struct Implementation;
        Implementation *impl;
    };
}