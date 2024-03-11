//Author: Simone Cannone

/**
 * @file soundbuffer.h
 * @brief Header file for the SoundBuffer class.
 *
 * @defgroup SoundBuffer SoundBuffer
 * @brief This file defines the SoundBuffer class, which manage the sound on the engine.
 */
#pragma once
#include "AL/al.h"
#include "AL/alc.h"
#include <string>
#include <optional>

 /**
  * @brief Struct that holds the RIFF data of the Wave file.
  * The RIFF data is the meta data information that holds,
  * the ID, size and format of the wave file.
  */
struct RIFF_Header {
  char chunkID[4]; ///< Identifier of the RIFF chunk.
  long chunkSize;  ///< Size of the chunk, not including chunkSize or chunkID.
  char format[4]; ///< Format of the audio data.
};

/**
 * @brief Struct to hold fmt subchunk data for WAVE files.
 */
struct WAVE_Format {
  char subChunkID[4]; ///< Identifier of the subchunk.
  long subChunkSize; ///< Size of the subchunk.
  short audioFormat; ///< Audio format.
  short numChannels; ///< Number of channels.
  long sampleRate; ///< Sample rate.
  long byteRate; ///< Byte rate.
  short blockAlign; ///< Block alignment.
  short bitsPerSample; ///< Bits per sample.
};

/**
 * @brief Struct to hold the data of the wave file.
 */
struct WAVE_Data {
  char subChunkID[4]; ///< Identifier of the data subchunk.
  long subChunk2Size; ///< Size of the data block.
};

/**
 * @brief Class representing a sound buffer.
 */
class SoundBuffer {

public:

  /**
   * @brief Factory function to create a sound buffer from a file.
   * @param filename Path to the wave file.
   * @return Optional containing the created sound buffer if successful, empty optional otherwise.
   */
  static std::optional<SoundBuffer> MakeBuffer(const std::string filename);

  /**
   * @brief Move constructor.
   */
  SoundBuffer(SoundBuffer&& b);

  /**
   * @brief Copy constructor.
   */
  SoundBuffer(const SoundBuffer& b) = default;

  /**
   * @brief Getter for the buffer ID.
   * @return Buffer ID.
   */
  ALuint getBuffer();

  /**
   * @brief Getter for the frequency.
   * @return Frequency.
   */
  int Fequency();

  /**
   * @brief Getter for the size.
   * @return Size.
   */
  int Size();

  /**
   * @brief Getter for the format.
   * @return Format.
   */
  int Format();

  /**
   * @brief Destructor.
   */
  ~SoundBuffer();

  /**
   * @brief Indicates if the buffer is active.
   */
  bool active_;

private:
  /**
   * @brief Default constructor.
   */
  SoundBuffer();

  ALuint buffer_; ///< Buffer ID.
  ALsizei size_; ///< Size.
  ALsizei frequency_; ///< Frequency.
  ALenum format_; ///< Format.
  std::string name_; ///< File name.

};
