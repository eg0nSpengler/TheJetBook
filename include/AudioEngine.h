#pragma once

#include <SFML/Audio.hpp>

class AudioEngine
{
public:
	enum class SOUND_TYPE
	{
		MOUSE_OVER,
		SELECT_AIRCRAFT,
		SELECT_DROPDOWN_ITEM
	};

public:
	AudioEngine();
	/// <summary>
	/// Plays audio
	/// </summary>
	/// <param name="sfxType">The type of sound effect to play</param>
	void PlayAudio(SOUND_TYPE sfxType);

private:
	void Init();
private:
	std::unique_ptr<sf::Sound> AudioManager;
	std::shared_ptr<sf::SoundBuffer> SFX_MOUSE_OVER;
	std::shared_ptr<sf::SoundBuffer> SFX_SELECT_AIRCRAFT;
	std::shared_ptr<sf::SoundBuffer> SFX_SELECT_DROPDOWN_OPTION;
};

