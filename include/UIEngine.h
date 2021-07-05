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
		/// Avionic systems
		/// </summary>
		AVI
	};

public:
	UIEngine();
	void Init();
	void Run();
	void GetAircraftImagesFromJSON();
	void GetAircraftInfoFromJSON();
	void ShowAircraftSpecs(SPEC_TYPE type);
	void GetAircraftSpecs(const std::multimap<std::string, std::string>& map);
	const sf::Sprite& ShowAircraftImage();

private:
	int selectedTable;
	int currentItem;
	int numRows;

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
	/// Contains avionic system(s) data for aircraft
	/// </summary>
	std::multimap<std::string, std::string> aircraftAviData;
	std::map<std::string, sf::Sprite> aircraftImages;
	std::vector<std::shared_ptr<sf::Texture>> imgTextures;
	std::vector<std::vector<const char*>> tableOptions;
	std::vector<const char*> currentData;
	std::vector<const char*> tableOne;
	std::vector<const char*> tableTwo;

private:
	SPEC_TYPE selectedSpecSheet;
};

