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

int main()
{
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
					std::string tmp = item["name"];
					auto tmpC = tmp.c_str();

					//creating a selectable element for each aircraft
					if (ImGui::Selectable(tmpC, isSelected))
					{

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

		auto defaultCursorPos = ImGui::GetCursorPos();
		
		/*BEGIN CENTER PANEL*/
		ImGui::BeginChild("CENTERPANE", ImVec2(CENTER_PANE_WINDOW_WIDTH, LEFT_PANE_WINDOW_HEIGHT / 2), true, ImGuiWindowFlags_NoNavInputs);

		/*BEGIN BOTTOM PANEL*/
		//ImGui::BeginChild("BOTTOMPANE", ImVec2(CENTER_PANE_WINDOW_WIDTH, CENTER_PANE_WINDOW_HEIGHT), true, ImGuiWindowFlags_NoNavInputs);
		/*END BOTTOM PANEL*/
		//ImGui::EndChild();

		/*END CENTER PANEL*/
		ImGui::EndChild(); ImGui::SameLine();

		/*BEGIN RIGHT PANEL*/
		ImGui::BeginChild("RIGHTPANE", ImVec2(LEFT_PANE_WINDOW_WIDTH, LEFT_PANE_WINDOW_HEIGHT), true, ImGuiWindowFlags_NoNavInputs);
		/*END RIGHT PANEL*/
		ImGui::EndChild(); ImGui::SameLine();
		
		ImGui::End();

		window.clear(bgColor);
		ImGui::SFML::Render(window);
		window.display();
	}

	
	ImGui::SFML::Shutdown();

}
