#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
	AudioManager = std::make_unique<sf::Sound>();

	SFX_MOUSE_OVER = std::make_shared<sf::SoundBuffer>();
	SFX_SELECT_AIRCRAFT = std::make_shared<sf::SoundBuffer>();
	SFX_SELECT_DROPDOWN_OPTION = std::make_shared<sf::SoundBuffer>();

	Init();
}

void AudioEngine::Init()
{
	SFX_MOUSE_OVER->loadFromFile("./Content/SFX/aaa_on.ogg");
	SFX_SELECT_DROPDOWN_OPTION->loadFromFile("./Content/SFX/arrows.ogg");
	SFX_SELECT_AIRCRAFT->loadFromFile("./Content/SFX/file.ogg");
}


void AudioEngine::PlayAudio(SOUND_TYPE sfxType)
{
	
	switch (sfxType)
	{
	case SOUND_TYPE::MOUSE_OVER:
		AudioManager->setBuffer(*SFX_MOUSE_OVER);
		AudioManager->play();
		break;
	case SOUND_TYPE::SELECT_AIRCRAFT:
		AudioManager->setBuffer(*SFX_SELECT_AIRCRAFT);
		AudioManager->play();
		break;
	case SOUND_TYPE::SELECT_DROPDOWN_ITEM:
		AudioManager->setBuffer(*SFX_SELECT_DROPDOWN_OPTION);
		AudioManager->play();
		break;
	default:
		break;
	}
}
