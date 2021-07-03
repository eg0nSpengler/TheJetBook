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
		GEN,
		PERF,
		ARM
	};

public:
	UIEngine();
	void Init();
	void Run();
	void GetAircraftImagesFromJSON();
	void GetAircraftInfoFromJSON();
	void ShowAircraftImage(std::string str);
	void ShowAircraftSpecs(SPEC_TYPE type);
	void GetAircraftSpecs(const std::multimap<std::string, std::string>& map);

private:
	int selectedTable;
	int currentItem;
	int numRows;

private:
	std::string imgFilePath;
	std::string currentAircraft;
	std::multimap<std::string, std::string> aircraftGenData;
	std::multimap<std::string, std::string> aircraftPerfData;
	std::vector<sf::Texture> aircraftImages;
	std::vector<const char*> currentData;
	std::vector<const char*> tableOne;
	std::vector<const char*> tableTwo;
	std::vector<std::vector<const char*>> tableOptions;

private:
	SPEC_TYPE selectedSpecSheet;
};

