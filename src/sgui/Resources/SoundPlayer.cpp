#include <SFML/Audio/Listener.hpp>
#include "sgui/Resources/SoundPlayer.h"

namespace sgui
{
/////////////////////////////////////////////////
void SoundPlayer::setResource (SoundHolder& holder)
{
  m_soundBuffer = &holder;
}

/////////////////////////////////////////////////
void SoundPlayer::play (
  const std::string& soundIdentifier,
  const sf::Vector2f& position)
{
  // add sound to the list of the played one
  m_sounds.push_back (sf::Sound (m_soundBuffer->get (soundIdentifier)));
  // set sound position and attenuation
  auto& sound = m_sounds.back ();
  sound.setPosition ({position.x, 0.f, position.y});
  sound.setAttenuation (attenuation);
  sound.setMinDistance (minDistance);
  // play requested sound
  sound.play ();
}

/////////////////////////////////////////////////
void SoundPlayer::removeStoppedSounds ()
{
  m_sounds.remove_if ([] (const auto& sound) {
    return sound.getStatus () == sf::SoundSource::Status::Stopped;
  });
}

/////////////////////////////////////////////////
void SoundPlayer::setListenerPosition (const sf::Vector2f& position)
{
  sf::Listener::setPosition ({position.x, 0.f, position.y});
}

/////////////////////////////////////////////////
sf::Vector3f SoundPlayer::listenerPosition () const
{
  return sf::Listener::getPosition ();
}

/////////////////////////////////////////////////
float SoundPlayer::volume () const
{
  return sf::Listener::getGlobalVolume ();
}

/////////////////////////////////////////////////
void SoundPlayer::setVolume (const float volume)
{
  sf::Listener::setGlobalVolume (volume);
}

} // namespace sgui
