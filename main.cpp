#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include <imgui-sfml/imgui-SFML.h>
#include <imgui/imgui.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <nlohmann/json.hpp>

void init();
void GetAircraftImages();
void ShowAircraftImage(const char* str);
void ShowAircraftDetails(int selected);

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define PARENT_WINDOW_WIDTH 800
#define PARENT_WINDOW_HEIGHT 600
#define PARENT_WINDOW_TITLE "The JetBook"
#define LEFT_PANE_WINDOW_WIDTH 200
#define LEFT_PANE_WINDOW_HEIGHT 600
#define CENTER_PANE_WINDOW_WIDTH 400
#define CENTER_PANE_WINDOW_HEIGHT 300
#define RIGHT_PANE_WINDOW_WIDTH 400
#define RIGHT_PANE_WINDOW_HEIGHT 300
#define IDLE_COLOR sf::Color::White
#define HOVER_COLOR sf::Color::Cyan


static const std::string imgFilePath = "./Content/Images/";
static std::vector<sf::Texture> aircraftImages;

int main()
{
	//GetAircraftImages();
	init();
	return 0;
}

void init()
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
					std::string tmp = item["name"]; //aircraft name
					std::string tmpImg = item["img"]; //aircraft image

					//string conversion again, so that we can apply it as the Selectable labels
					auto tmpC = tmp.c_str();
					auto tmpImgC = tmpImg.c_str();

					//creating a selectable element for each aircraft
					if (ImGui::Selectable(tmpC, isSelected))
					{
						//passing in the img name to access the corresponding aircraft image
						ShowAircraftImage(tmpImgC);
					}
				}
			}
			
		}

		/*for (const auto& item : j["Attack"])
		{
			//Get the aircraft name
			std::string tmp = item["name"];
			//Convert the string to a char array
			auto tmpC = tmp.c_str();
		}*/

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
			static int currentItem = 0;
			static int numRows = 10;
			static std::vector<const char*> currentOptions;
			static std::vector<const char*> tableOptions = { "Crew", "Length", "Wingspan", "Height", "Wing area", "Empty weight", "Gross weight", "Max takeoff weight", "Fuel capacity", "Engine(s)" };

			for (auto i = 3; i > 0; i--)
			{
				ImGui::Indent();
			}
			
			//setting up dropdown box
			if (ImGui::Combo(" ", &currentItem, "General characteristics\0Performance\0Avionics"))
			{
				switch (currentItem)
				{
				case 0:
					break;
				case 1:
					break;
				case 2:
					break;
				default:
					break;
				}
			}

			//displaying the table for the corresponding dropdown option 
			if (ImGui::BeginTable("CurrentTable", 1))
			{
				for (auto i = 0; i < tableOptions.capacity(); i++)
				{
					ImGui::TableNextRow();

					for (auto iy = 0; iy < 1; iy++)
					{
						ImGui::TableSetColumnIndex(iy);
						ImGui::Text(tableOptions[i]);
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

void GetAircraftImages()
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

void ShowAircraftImage(const char* str)
{
	
}

void ShowAircraftDetails(int selected)
{

}