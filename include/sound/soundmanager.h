//Author: Simone Cannone  
#pragma once
#include <vector>
#include <string>
#include "AL/al.h"
#include "AL/alc.h"

enum ErrorCode {
  kErrorCode_OK = 0,
  kErrorCode_InvalidSourceName = -1,
};


enum Track;
class SoundSource;
class BranchTrack;
class SoundBuffer;
class SoundManager {
public:
    SoundManager();
    ~SoundManager();


    void addsrc(SoundSource& s);
    void addBuffer(SoundBuffer& s);

    void Layering();
    void BranchingImgui(Track& state, bool& playing, float& gain, SoundSource& src,
                        std::string name, std::string previous_name);
    void BranchingNode(BranchTrack* track, int size, const char* name);
    void CrossfadingImGui(int w, int h);
    int Crossfading(SoundSource& old_sound, SoundSource& new_sound);
    void updateCrossfading();
    void SoundsInfoImgui();

    void unbidAllSources();

private:



    std::vector<float> new_gain_;

    std::vector<SoundSource> src_manager_;
    std::vector<SoundBuffer> buffer_manager_;

    std::string crossfadingMessage_;
    bool bcrossfading;
    float crossfading_speed;
    int old_sound_id;
    int new_sound_id;

    std::vector<bool> active_sound;
    std::vector<bool> mute_sound;

};