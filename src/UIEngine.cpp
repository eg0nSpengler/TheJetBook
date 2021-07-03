#include "UIEngine.h"


UIEngine::UIEngine()
{	
	imgFilePath = "./Content/Images/";
	selectedSpecSheet = SPEC_TYPE::GEN;
	selectedTable = 0;
	currentItem = 0;
	numRows = 10;
	tableOne = { "Crew", "Length", "Wingspan", "Height", "Wing area", "Empty weight", "Gross weight", "Max takeoff weight", "Internal fuel capacity", "Engine(s)" };
	tableTwo = { "Maximum speed", "Cruise speed", "Stall speed", "Combat range", "Ferry range", "Service ceiling", "g limits", "Roll rate", "Rate of climb", "T/W ratio" };

	Init();
}

void UIEngine::Init()
{
	//GetAircraftImages();
	GetAircraftInfoFromJSON();
	ShowAircraftSpecs(selectedSpecSheet);
}

void UIEngine::Run()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), " ", sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	ImGui::SFML::Init(window);

	sf::Color bgColor = sf::Color::Cyan;
	float color[3] = { 0.f, 0.f, 0.f };

	window.resetGLStates();

	sf::Clock deltaTime;

	std::ifstream ifs("Content/aircraft.json");

	nlohmann::json j;
	ifs >> j;

	auto acStr = " ";

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaTime.restart());

		ImGui::SetNextWindowSize(ImVec2(PARENT_WINDOW_WIDTH, PARENT_WINDOW_HEIGHT), ImGuiCond_Once);
		ImGui::Begin(PARENT_WINDOW_TITLE, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
		ImGui::ShowDemoWindow();

		/*BEGIN LEFT PANEL*/
		ImGui::BeginChild("LEFTPANE", ImVec2(LEFT_PANE_WINDOW_WIDTH, LEFT_PANE_WINDOW_HEIGHT), true, ImGuiWindowFlags_NoNavInputs);

		static bool isSelected = false;
		//getting each aircraft category
		for (const auto& elem : j.items())
		{
			//string conversion
			std::string tmp = elem.key();
			auto tmpC = tmp.c_str();

			//creating a tree node for each category
			if (ImGui::CollapsingHeader(tmpC, ImGuiTreeNodeFlags_None))
			{
				//getting each aircraft from each category
				//and placing it under their respective category
				for (const auto& item : j[tmp])
				{
					std::string tmpName = item["name"]; //aircraft name
					std::string tmpImg = item["img"]; //aircraft image
					currentAircraft = tmpName;

					//string conversion again, so that we can apply it as the Selectable labels
					auto tmpC = tmpName.c_str();

					//creating a selectable element for each aircraft
					if (ImGui::Selectable(tmpC, isSelected))
					{
						//passing in the img name to access the corresponding aircraft image
						ShowAircraftImage(tmpImg);
						ShowAircraftSpecs(selectedSpecSheet);
					}
				}
			}

		}

		/*END LEFT PANEL*/
		ImGui::EndChild(); ImGui::SameLine();

		static ImVec2 defaultCursorPos = ImGui::GetCursorPos();

		/*BEGIN CENTER PANEL*/
		if (ImGui::BeginChild("CENTERPANE", ImVec2(CENTER_PANE_WINDOW_WIDTH, LEFT_PANE_WINDOW_HEIGHT / 2), true, ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
		{

		}

		/*END CENTER PANEL*/
		ImGui::EndChild(); ImGui::SameLine();
		auto newCursorPos = ImGui::GetCursorPos();

		ImGui::SetCursorPosX(defaultCursorPos.x);
		ImGui::SetCursorPosY(LEFT_PANE_WINDOW_HEIGHT / 2 + ImGui::GetFrameHeightWithSpacing() + 4);

		/*BEGIN BOTTOM PANEL*/
		if (ImGui::BeginChild("BOTTOMPANE", ImVec2(CENTER_PANE_WINDOW_WIDTH, LEFT_PANE_WINDOW_HEIGHT / 2), true, ImGuiWindowFlags_NoNavInputs))
		{

			if (tableOptions.capacity() <= 0)
			{
				tableOptions.emplace_back(tableOne);
				tableOptions.emplace_back(tableTwo);
			}

			for (auto i = CENTER_PANE_INDENT; i > 0; i--)
			{
				ImGui::Indent();
			}

			//setting up dropdown box
			if (ImGui::Combo(" ", &currentItem, "General characteristics\0Performance\0Armament"))
			{
				switch (currentItem)
				{
				case 0:
					selectedTable = 0;
					selectedSpecSheet = SPEC_TYPE::GEN;
					break;
				case 1:
					selectedTable = 1;
					selectedSpecSheet = SPEC_TYPE::PERF;
					break;
				case 2:
					selectedTable = 2;
					selectedSpecSheet = SPEC_TYPE::ARM;
					break;
				default:
					break;
				}

				ShowAircraftSpecs(selectedSpecSheet);
			}

			//displaying the table for the corresponding dropdown option 
			if (ImGui::BeginTable("CurrentTable", NUM_COLUMNS))
			{
				for (auto i = 0; i < tableOptions.at(selectedTable).capacity(); i++)
				{
					ImGui::TableNextRow();

					for (auto iy = 0; iy < 1; iy++)
					{
						ImGui::TableSetColumnIndex(iy);
						ImGui::Text(tableOptions.at(selectedTable)[i]);
						ImGui::TableNextColumn();
						ImGui::Text(currentData.at(i));
					}

				}
				ImGui::EndTable();
			}

		}

		/*END BOTTOM PANEL*/
		ImGui::EndChild();

		ImGui::SetCursorPos(newCursorPos);
		/*BEGIN RIGHT PANEL*/
		ImGui::BeginChild("RIGHTPANE", ImVec2(LEFT_PANE_WINDOW_WIDTH, LEFT_PANE_WINDOW_HEIGHT), true, ImGuiWindowFlags_NoNavInputs);
		/*END RIGHT PANEL*/
		ImGui::EndChild();

		ImGui::End();

		window.clear(bgColor);
		ImGui::SFML::Render(window);
		window.display();
	}


	ImGui::SFML::Shutdown();

}

void UIEngine::GetAircraftImagesFromJSON()
{
	std::ifstream ifs("Content/aircraft.json");

	nlohmann::json j;
	ifs >> j;
	sf::Texture tmpTexture;
	tmpTexture.create(CENTER_PANE_WINDOW_WIDTH, CENTER_PANE_WINDOW_HEIGHT);

	
	for (const auto& elem : j.items())
	{
		//string conversion
		std::string tmp = elem.key();

			//getting each aircraft from each category
			//and placing it under their respective category
			for (const auto& item : j[tmp])
			{
				std::string tmpImg = item["img"]; //aircraft image

				if (tmpTexture.loadFromFile(imgFilePath + tmpImg))
				{
					aircraftImages.emplace_back(sf::Texture(tmpTexture));
				}
			}
		
	}
}

void UIEngine::GetAircraftInfoFromJSON()
{
	std::ifstream ifs("Content/aircraft.json");

	nlohmann::json j;
	ifs >> j;

	for (const auto& elem : j.items())
	{
		//string conversion
		std::string tmp = elem.key();

		//going through each category
		for (const auto& item : j[tmp])
		{
			//getting each aircraft name
			std::string nameTmp = item["name"];

			currentAircraft = nameTmp;

			//getting the general specs for each aircraft
			for (const auto& genSpec : item["genspecs"])
			{
				//getting each individual spec
				std::string genSpecTmp = genSpec;

				//associating the aircraft with its listed specs
				aircraftGenData.insert(std::pair<std::string, std::string>(nameTmp, genSpecTmp));
			}

			for (const auto& perfSpec : item["perf"])
			{
				std::string perfSpecTmp = perfSpec;

				aircraftPerfData.insert(std::pair<std::string, std::string>(nameTmp, perfSpecTmp));
			}
		}

	}

	/*for (std::pair<std::string, std::string> elem : aircraftData)
	{
		std::cout << elem.first << " :: " << elem.second << '\n';
	}*/
}

void UIEngine::ShowAircraftImage(std::string str)
{

}

void UIEngine::ShowAircraftSpecs(SPEC_TYPE type)
{
	//we already have data in our container
	//clear it from the previous selection
	if (currentData.size() > 0)
	{
		currentData.clear();
	}

	switch (type)
	{
	case SPEC_TYPE::GEN:
		GetAircraftSpecs(aircraftGenData);
		break;
	case SPEC_TYPE::PERF:
		GetAircraftSpecs(aircraftPerfData);
		break;
	case SPEC_TYPE::ARM:
		break;
	default:
		break;
	}

}

void UIEngine::GetAircraftSpecs(const std::multimap<std::string, std::string>& map)
{
	auto search = map.begin();

	if (search != map.end())
	{
		//getting the range of values for our aircraft key
		auto range = map.equal_range(currentAircraft);

		//for each value under this aircraft key
		for (auto i = range.first; i != range.second; ++i)
		{
			//string conversion
			auto tmp = i->second.c_str();

			//add our values into this container
			//to be displayed on the spec sheet
			currentData.push_back(tmp);
		}
	}
}
