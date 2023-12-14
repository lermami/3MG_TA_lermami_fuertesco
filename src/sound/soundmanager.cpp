#include "soundmanager.h"

#include "soundbuffer.h"
#include "soundsource.h"

#include "imgui.h"

SoundManager::SoundManager() {
  // Open device and create context

  bcrossfading = false;
  old_sound_id = 0;
  new_sound_id = 0;
  crossfading_speed = 1.0f;
}

SoundManager::~SoundManager() {
  for (auto& a : src_manager_) {
    a.destructible_ = true;
  }

  for (auto& a : buffer_manager_) {
    a.active_ = false;
  }
}

void SoundManager::addsrc(SoundSource& s) {
  src_manager_.push_back(std::move(s));
  new_gain_.push_back(s.Gain());
  active_sound.push_back(false);
  mute_sound.push_back(false);
  // s.destructible_ = true;
}

void SoundManager::addBuffer(SoundBuffer& s) {
  buffer_manager_.push_back(std::move(s));
  // s.active_ = false;
}

void SoundManager::Layering() {
  ImGui::Begin("Layering");

  if (ImGui::Button("Play All")) {
    for (int i = 0; i < src_manager_.size(); i++) {
      src_manager_.at(i).Play();
    }
  }
  ImGui::SameLine();

  if (ImGui::Button("Stop All")) {
    for (int i = 0; i < src_manager_.size(); i++) {
      src_manager_.at(i).Stop();
    }
  }
  ImGui::SameLine();

  if (ImGui::Button("Active All")) {
    for (int i = 0; i < src_manager_.size(); i++) {
      src_manager_.at(i).setGain(1.0f);
      new_gain_[i] = src_manager_.at(i).Gain();
    }
  }
  ImGui::SameLine();

  if (ImGui::Button("Mute All")) {
    for (int i = 0; i < src_manager_.size(); i++) {
      src_manager_.at(i).setGain(0.0f);
      new_gain_[i] = src_manager_.at(i).Gain();
    }
  }

  for (int i = 0; i < src_manager_.size(); i++) {
    ImGui::PushID(i);
    new_gain_[i] = src_manager_.at(i).Gain();
    bool open = ImGui::CollapsingHeader(src_manager_.at(i).Name().c_str(),
                                        ImGuiTreeNodeFlags_None);
    if (open) {
      if (ImGui::Button("Play")) {
        src_manager_.at(i).Play();
      }
      ImGui::SameLine();

      if (ImGui::Button("Stop")) {
        src_manager_.at(i).Stop();
      }
      ImGui::SameLine();

      if (ImGui::Button("Active")) {
        active_sound[i] = true;
        mute_sound[i] = false;
      }
      ImGui::SameLine();

      if (active_sound[i] && src_manager_.at(i).Gain() < 0.99f) {
        src_manager_.at(i).updateGain(0.01f);
        new_gain_[i] = src_manager_.at(i).Gain();
      } else {
        active_sound[i] = false;
      }

      if (ImGui::Button("Mute")) {
        mute_sound[i] = true;
        active_sound[i] = false;

      }

      if (mute_sound[i] && src_manager_.at(i).Gain() > 0.0f) {
        src_manager_.at(i).updateGain(-0.01f);
        new_gain_[i] = src_manager_.at(i).Gain();

      } else {
        mute_sound[i] = false;
      }

      ImGui::SliderFloat("Gain", &new_gain_[i], 0.0f, 1.0f, "%.3f");
      if (new_gain_[i] != src_manager_.at(i).Gain()) {
        src_manager_.at(i).setGain(new_gain_[i]);
      }

      float aux_p = src_manager_.at(i).Pitch();
      ImGui::SliderFloat("Pitch", &aux_p, 0.0f, 2.0f, "%.3f");
      src_manager_.at(i).setPitch(aux_p);

      float* pos_imgui = src_manager_.at(i).getPos();
      ImGui::DragFloat3("Position", pos_imgui, 0.01f, -10.0f, 10.0f, "%.2f");
      src_manager_.at(i).setPos(pos_imgui);

      // float* aux_vel = src_manager_.at(i).getVelocity();
      // ImGui::DragFloat3("Velocity", aux_vel);
      // src_manager_.at(i).setVelocity(aux_vel);

      bool loop = src_manager_.at(i).Loop();
      if (ImGui::Checkbox("Loop", &loop)) {
        src_manager_.at(i).setLoop(loop);
      }
    }
    ImGui::PopID();
  }

  ImGui::End();
}

int SoundManager::Crossfading(SoundSource& old_sound, SoundSource& new_sound) {
  bool check1 = false, check2 = false;
  int id = 0;

  for (auto& sound : src_manager_) {
    if (sound.Name() == old_sound.Name()) {
      check1 = true;
      old_sound_id = id;
    }
    if (sound.Name() == new_sound.Name()) {
      check2 = true;
      new_sound_id = id;
    }

    id++;
  }

  if (check1 && check2) {
    bcrossfading = true;
    return ErrorCode::kErrorCode_OK;
  }

  return ErrorCode::kErrorCode_InvalidSourceName;
}

void SoundManager::updateCrossfading() {
  if (bcrossfading) {
    SoundSource& oldS = src_manager_[old_sound_id];
    SoundSource& newS = src_manager_[new_sound_id];
    oldS.updateGain(-0.005f * crossfading_speed);

    if (newS.Gain() < 1.0f) newS.updateGain(0.005f * crossfading_speed);

    if (oldS.Gain() < 0.01f) {
      bcrossfading = false;
      oldS.setGain(0.0f);
      newS.setGain(1.0f);
    }
  }
}

void SoundManager::SoundsInfoImgui() {
  ImGui::Begin("Sounds information");
  // Play all sounds
  if (ImGui::Button("Play All")) {
    for (auto& sound : src_manager_) {
      sound.Play();
      sound.setGain(1.0f);
    }
  }
  ImGui::SameLine();

  // Mute all sounds
  if (ImGui::Button("Mute All")) {
    for (auto& sound : src_manager_) {
      sound.setGain(0.0f);
    }
  }

  // Sounds information
  for (auto& sound : src_manager_) {
    if (ImGui::CollapsingHeader(sound.Name().c_str(),
                                ImGuiTreeNodeFlags_DefaultOpen)) {
      std::string label = sound.Name() + std::string(" Mute");

      bool muted = sound.Gain() == 0;
      if (ImGui::Checkbox(label.c_str(), &muted)) {
        muted ? sound.setGain(0.0f) : sound.setGain(1.0f);
      }

      label = sound.Name() + std::string(" Gain");
      float aux_gain = sound.Gain();
      ImGui::DragFloat(label.c_str(), &aux_gain, 0.1f, 0.0f, 1.0f);
      sound.setGain(aux_gain);

      label = sound.Name() + std::string(" Pitch");
      float aux_pitch = sound.Pitch();
      ImGui::DragFloat(label.c_str(), &aux_pitch, 0.1f, 0.0f, 1.0f);
      sound.setPitch(aux_pitch);

      label = sound.Name() + std::string(" Position");
      float* aux_pos = sound.getPos();
      ImGui::DragFloat3(label.c_str(), aux_pos);
      sound.setPos(aux_pos);

      label = sound.Name() + std::string(" Velocity");
      float* aux_vel = sound.getVelocity();
      ImGui::DragFloat3(label.c_str(), aux_vel);
      sound.setVelocity(aux_vel);
    }
  }
  ImGui::End();
}

void SoundManager::unbidAllSources() {
  for (auto& a : src_manager_) {
    a.unbindSound();
  }
}

void SoundManager::CrossfadingImGui(int w, int h) {
  ImGui::Begin("Crossfading");

  // Select first sound to crossfade
  static int selected_item_1 = 0;
  if (ImGui::BeginCombo("Sound 1",
                        src_manager_[selected_item_1].Name().c_str())) {
    for (size_t i = 0; i < src_manager_.size(); i++) {
      const bool isSelected = (selected_item_1 == i);
      if (ImGui::Selectable(src_manager_[i].Name().c_str(), isSelected))
        selected_item_1 = i;

      if (isSelected) ImGui::SetItemDefaultFocus();
    }

    ImGui::EndCombo();
  }

  // Select second sound to crossfade
  static int selected_item_2 = 1;
  if (ImGui::BeginCombo("Sound 2",
                        src_manager_[selected_item_2].Name().c_str())) {
    for (size_t i = 0; i < src_manager_.size(); i++) {
      const bool isSelected = (selected_item_2 == i);
      if (ImGui::Selectable(src_manager_[i].Name().c_str(), isSelected))
        selected_item_2 = i;

      if (isSelected) ImGui::SetItemDefaultFocus();
    }

    ImGui::EndCombo();
  }

  ImGui::DragFloat("Speed", &crossfading_speed, 0.1f, 1.0f, 10.0f);

  // Execute crossfade
  if (ImGui::Button("Start crossfading")) {
    auto& sound1 = src_manager_[selected_item_1];
    auto& sound2 = src_manager_[selected_item_2];

    if (!sound1.isPlaying() && !sound2.isPlaying()) {
      crossfadingMessage_ = "Sounds are not playing.";
    } else if (!sound1.isPlaying()) {
      crossfadingMessage_ =
          sound1.Name().c_str() + std::string(" is not playing.");
    } else if (!sound2.isPlaying()) {
      crossfadingMessage_ =
          sound2.Name().c_str() + std::string(" is not playing.");
    }

    if (selected_item_1 != selected_item_2) {
      if (sound1.Gain() > sound2.Gain()) {
        old_sound_id = selected_item_1;
        new_sound_id = selected_item_2;
        bcrossfading = true;
        crossfadingMessage_.erase();
      } else if (sound1.Gain() < sound2.Gain()) {
        old_sound_id = selected_item_2;
        new_sound_id = selected_item_1;
        bcrossfading = true;
        crossfadingMessage_.erase();
      } else {
        crossfadingMessage_ =
            "You can't crossfade two sounds with the same gain.";
      }
    } else {
      crossfadingMessage_ = "You selected the same sound.";
    }
  }

  if (crossfadingMessage_.size() != 0) {
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Error: %s",
                       crossfadingMessage_.c_str());
  }

  ImGui::End();
}