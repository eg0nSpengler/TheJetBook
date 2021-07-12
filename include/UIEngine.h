#pragma once
#include "UIDefines.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <imgui-sfml/imgui-SFML.h>
#include <imgui/imgui.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Audio.hpp>

#include <nlohmann/json.hpp>

class UIEngine 
{
	enum class SPEC_TYPE
	{
		/// <summary>
		/// General specs
		/// </summary>
		GEN,
		/// <summary>
		/// Performance specs
		/// </summary>
		PERF,
		/// <summary>
		/// Avionics
		/// </summary>
		AVI
	};

public:
	UIEngine();
	void Init();
	void Run();
	void SetupAudio();
	void GetAircraftImagesFromJSON();
	void GetAircraftInfoFromJSON();
	void ShowAircraftSpecs(SPEC_TYPE type);
	void GetAircraftSpecs(const std::multimap<std::string, std::string>& map);
	void GetAircraftDetails();
	const sf::Sprite& ShowAircraftImage();

private:
	int selectedTable;
	int currentItem;
	int numRows;
	SPEC_TYPE selectedSpecSheet;

private:
	std::string imgFilePath;
	std::string currentAircraft;
	/// <summary>
	/// Contains general spec data for aircraft
	/// </summary>
	std::multimap<std::string, std::string> aircraftGenData;
	/// <summary>
	/// Contains performance data for aircraft
	/// </summary>
	std::multimap<std::string, std::string> aircraftPerfData;
	/// <summary>
	/// Contains avionic data for aircraft
	/// </summary>
	std::multimap<std::string, std::string> aircraftAvionicData;
	/// <summary>
	/// Contains details(nation/maker/etc) for aircraft
	/// </summary>
	std::multimap<std::string, std::string> aircraftDetailData;
	std::map<std::string, sf::Sprite> aircraftImages;
	std::vector<std::shared_ptr<sf::Texture>> imgTextures;
	std::vector<std::vector<const char*>> tableOptions;
	/// <summary>
	/// These all contain const char* since if we set these to contain std::strings instead
	/// I'd end up having to do a string conversion for each element before applying them to ImGui objects...
	/// </summary>
	std::vector<const char*> rightPanelOptions;
	std::vector<const char*> currentData;
	std::vector<const char*> currentDetails;
	std::vector<const char*> currentAvionics;
	std::vector<const char*> tableOne;
	std::vector<const char*> tableTwo;

private:
	std::shared_ptr<sf::Texture> noSelectionTexture;
	std::shared_ptr<sf::Sprite> noSelectionImg;

private:
	std::unique_ptr<sf::Sound> SND_MANAGER;
	std::shared_ptr<sf::SoundBuffer> SND_MOUSEOVER;
	std::shared_ptr<sf::SoundBuffer> SND_EXPANDCATEGORY;
	std::shared_ptr<sf::SoundBuffer> SND_SELECTAC;
};

