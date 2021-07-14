#pragma once
#include "UIDefines.h"
#include "AudioEngine.h"
#include "JSONData.h"


#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <imgui-sfml/imgui-SFML.h>
#include <imgui/imgui.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

class UIEngine 
{

public:
	UIEngine();
	void Run();
private:
	int selectedTable;
	int currentItem;
private:
	std::vector<std::vector<std::string>> tableOptions;
	std::vector<std::string> specTableOne;
	std::vector<std::string> specTableTwo;
private:
	std::unique_ptr<AudioEngine> audioEngine;
	std::unique_ptr<JSONData> jsonData;
};

