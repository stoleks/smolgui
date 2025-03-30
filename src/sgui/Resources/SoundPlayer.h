#pragma once

#include <list>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "sgui/Resources/ResourcesHolder.h"

namespace sgui
{
/**
 * @brief Allow to play spatialized sounds
 */
class SoundPlayer
{
public:
  /**
   * SoundPlayer is not copyable
   */
  SoundPlayer () = default;
  SoundPlayer (const SoundPlayer&) = delete;
  SoundPlayer& operator= (const SoundPlayer&) = delete;
  /**
   * @brief set sounds files
   */
  void setResource (SoundHolder& holder);
  /**
   * @brief play a sound at a given position
   */
  void play (
         const std::string& sound,
         const sf::Vector2f& position = {});
  /**
   * @brief remove all stopped sounds
   */
  void removeStoppedSounds ();
  /**
   * @brief get listener position
   */
  sf::Vector3f listenerPosition () const;
  /**
   * @brief set listener position
   */
  void setListenerPosition (const sf::Vector2f& position);
  /**
   * @brief get sound volume
   */
  float volume () const;
  /**
   * @brief set sound volume
   */
  void setVolume (const float volume);
public:
  float attenuation = 0.f;
  float minDistance = 100.f;
private:
  SoundHolder* m_soundBuffer;
  std::list <sf::Sound> m_sounds;
};

} // namespace sgui
