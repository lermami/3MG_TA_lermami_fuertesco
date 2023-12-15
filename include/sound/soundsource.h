//Author: Simone Cannone  
#pragma once
#include "AL/al.h"
#include "AL/alc.h"
#include <string>

class SoundBuffer;

class SoundSource {

public:
    SoundSource();
    SoundSource(std::string name,ALfloat pos[3], ALfloat speed[3],float gain = 1.0f, float pitch = 1.0f);
    ~SoundSource();
    SoundSource(const SoundSource& other);
    SoundSource(SoundSource& other);
    SoundSource(SoundSource&& other);
    SoundSource& operator=(const SoundSource& o);  

    void setLoop(bool state); 
    void setPos(ALfloat pos[3]);
    ALfloat* getPos();
    void setVelocity(ALfloat speed[3]);
    ALfloat* getVelocity();
    void updatePos(ALfloat pos[3]);
    void updateGain(float gain);
    void updatePitch(float pitch);
    void setGain(float gain);
    void setPitch(float pitch);
    float Gain();
    float Pitch();
    bool Loop();

    bool addSound(SoundBuffer* b);
    void unbindSound();
    bool isPlaying();
    void Play();
    void Stop();
    std::string Name();
    

    bool destructible_;
private:
    std::string name_;
    ALfloat pos_[3];
    ALfloat speed_[3];
    float gain_;
    float pitch_;
    bool loop_;

    ALuint src_;
    ALuint buffer_;
    

};
