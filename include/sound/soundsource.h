//Author: Simone Cannone

/**
 * @file soundsource.h
 * @brief Header file for the SoundSource class.
 *
 * @defgroup SoundSource SoundSource
 * @brief This file defines the SoundSource class.
 */
#pragma once
#include "AL/al.h"
#include "AL/alc.h"
#include <string>

class SoundBuffer;


/**
 * @brief Class representing a sound source.
 */
class SoundSource {

public:
  /**
   * @brief Default constructor.
   */
  SoundSource();

  /**
   * @brief Constructor that initializes the sound source with certain parameters.
   * @param name Name of the sound source.
   * @param pos Initial position of the sound source.
   * @param speed Initial speed of the sound source.
   * @param gain Volume of the sound source (default 1.0f).
   * @param pitch Pitch of the sound source (default 1.0f).
   */
  SoundSource(std::string name, ALfloat pos[3], ALfloat speed[3], float gain = 1.0f, float pitch = 1.0f);

  /**
   * @brief Destructor of the class.
   */
  ~SoundSource();

  /**
   * @brief Copy constructor.
   */
  SoundSource(const SoundSource& other);

  /**
   * @brief Copy constructor.
   */
  SoundSource(SoundSource& other);

  /**
   * @brief Move constructor.
   */
  SoundSource(SoundSource&& other);

  /**
   * @brief Assignment operator.
   */
  SoundSource& operator=(const SoundSource& o);

  /**
   * @brief Sets whether the sound source is looped or not.
   * @param state Loop state (true for looped playback, false otherwise).
   */
  void setLoop(bool state);

  /**
   * @brief Sets the position of the sound source.
   * @param pos Position of the sound source.
   */
  void setPos(ALfloat pos[3]);

  /**
   * @brief Gets the current position of the sound source.
   * @return Pointer to the position of the sound source.
   */
  ALfloat* getPos();

  /**
   * @brief Sets the velocity of the sound source.
   * @param speed Velocity of the sound source.
   */
  void setVelocity(ALfloat speed[3]);

  /**
   * @brief Gets the current velocity of the sound source.
   * @return Pointer to the velocity of the sound source.
   */
  ALfloat* getVelocity();

  /**
   * @brief Updates the position of the sound source.
   * @param pos New position of the sound source.
   */
  void updatePos(ALfloat pos[3]);

  /**
   * @brief Updates the gain of the sound source.
   * @param gain New gain of the sound source.
   */
  void updateGain(float gain);

  /**
   * @brief Updates the pitch of the sound source.
   * @param pitch New pitch of the sound source.
   */
  void updatePitch(float pitch);

  /**
   * @brief Sets the gain of the sound source.
   * @param gain Gain of the sound source.
   */
  void setGain(float gain);

  /**
   * @brief Sets the pitch of the sound source.
   * @param pitch Pitch of the sound source.
   */
  void setPitch(float pitch);

  /**
   * @brief Gets the gain of the sound source.
   * @return Gain of the sound source.
   */
  float Gain();

  /**
   * @brief Gets the pitch of the sound source.
   * @return Pitch of the sound source.
   */
  float Pitch();

  /**
   * @brief Checks if the sound source is set to loop.
   * @return True if the sound source is set to loop, false otherwise.
   */
  bool Loop();

  /**
   * @brief Adds a sound buffer to the sound source.
   * @param b Pointer to the sound buffer to add.
   * @return True if added successfully, false otherwise.
   */
  bool addSound(SoundBuffer* b);

  /**
   * @brief Unbinds the current sound buffer from the sound source.
   */
  void unbindSound();

  /**
   * @brief Checks if the sound source is currently playing.
   * @return True if the sound source is playing, false otherwise.
   */
  bool isPlaying();

  /**
   * @brief Plays the sound source.
   */
  void Play();

  /**
   * @brief Stops playback of the sound source.
   */
  void Stop();

  /**
   * @brief Gets the name of the sound source.
   * @return Name of the sound source.
   */
  std::string Name();

  /**
   * @brief Indicates whether the sound source is destructible.
   */
  bool destructible_;

private:
  std::string name_; ///< Name of the sound source.
  ALfloat pos_[3]; ///< Position of the sound source.
  ALfloat speed_[3]; ///< Speed of the sound source.
  float gain_; ///< Volume of the sound source.
  float pitch_; ///< Pitch of the sound source.
  bool loop_; ///< Loop indicator.

  ALuint src_; ///< Sound source identifier.
  ALuint buffer_; ///< Sound buffer identifier.

};
