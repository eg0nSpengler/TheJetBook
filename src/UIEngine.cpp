#include "UIEngine.h"


UIEngine::UIEngine()
{	
	selectedTable = 0;
	currentItem = 0;

	specTableOne = { "Crew", "Length", "Wingspan", "Height", "Wing area", "Empty weight", "Gross weight", "Max takeoff weight", "Internal fuel capacity", "Engine(s)" };
	specTableTwo = { "Maximum speed", "Cruise speed", "Stall speed", "Combat range", "Ferry range", "Service ceiling", "g limits", "Roll rate", "Rate of climb", "T/W ratio" };
	tableOptions.push_back(specTableOne);
	tableOptions.push_back(specTableTwo);

	audioEngine = std::make_unique<AudioEngine>();
	jsonData = std::make_unique<JSONData>();
	deltaTime = std::make_unique<sf::Clock>();
	inputEvents = std::make_shared<sf::Event>();
	window = std::make_shared<sf::RenderWindow>(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), " ", sf::Style::Close);

	Init();
}

void UIEngine::Init()
{
	window->setVerticalSyncEnabled(true);

	ImGui::SFML::Init(*window);

	/*BEGIN IMGUI STYLE*/
	ImGuiStyle& style = ImGui::GetStyle();

	style.ChildRounding = CHILD_ROUNDING;
	style.FrameRounding = FRAME_ROUNDING;
	style.WindowTitleAlign = ImVec2(WINDOW_TITLE_ALIGN, style.WindowTitleAlign.y);

	style.DisplaySafeAreaPadding = ImVec2(style.DisplaySafeAreaPadding.x, WINDOW_SAFE_AREA_PADDING);

	/*END IMGUI STYLE*/
}

void UIEngine::Run()
{

#if defined _DEBUG
#else
	//style.WindowMenuButtonPosition = WINDOW_MENU_BUTTON;
#endif


	auto bgColor = sf::Color::Transparent;
	float color[3] = { 0.f, 0.f, 0.f };

	window->resetGLStates();

	while (window->isOpen())
	{

		while (window->pollEvent(*inputEvents))
		{
			ImGui::SFML::ProcessEvent(*inputEvents);

			if (inputEvents->type == sf::Event::Closed)
			{
				window->close();
			}
		}

		ImGui::SFML::Update(*window, deltaTime->restart());

		ImGui::SetNextWindowSize(ImVec2(PARENT_WINDOW_WIDTH, PARENT_WINDOW_HEIGHT), ImGuiCond_Once);
		//So we only show the demo window if we're in DEBUG config
#if defined _DEBUG
		ImGui::ShowDemoWindow();

#else
#endif

		ImGui::Begin(PARENT_WINDOW_TITLE, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
	
		/*BEGIN LEFT PANEL*/
		ImGui::BeginChild("LEFTPANE", ImVec2(LEFT_PANE_WINDOW_WIDTH, LEFT_PANE_WINDOW_HEIGHT), true, ImGuiWindowFlags_NoNavInputs);

		auto static isSelected = false;
		
		for (const auto& category : jsonData->json->items())
		{
				//getting each aircraft category
				auto tmp = category.key();

				//creating a tree node for each category
				if (ImGui::CollapsingHeader(tmp.c_str(), ImGuiTreeNodeFlags_None))
				{
					//getting each aircraft under each category
					for (const auto& ac : jsonData->json->at(tmp))
					{
						//getting the aircraft name
						auto acName = std::string(ac["name"]);

						//creating a selectable element for each aircraft
						if (ImGui::Selectable(acName.c_str(), isSelected))
						{
							audioEngine->PlayAudio(AudioEngine::SOUND_TYPE::SELECT_AIRCRAFT);
							jsonData->SetCurrentAircraft(acName.c_str());
						}
					}
				}	
		}

		

		/*END LEFT PANEL*/
		ImGui::EndChild(); ImGui::SameLine();

		auto defaultCursorPos = ImGui::GetCursorPos();

		/*BEGIN CENTER PANEL*/
		if (ImGui::BeginChild("CENTERPANE", ImVec2(CENTER_PANE_WINDOW_WIDTH, LEFT_PANE_WINDOW_HEIGHT / 2), false, ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
		{
			ImGui::Image(jsonData->GetAircraftImage());
		}

		/*END CENTER PANEL*/
		ImGui::EndChild(); ImGui::SameLine();
		auto newCursorPos = ImGui::GetCursorPos();

		ImGui::SetCursorPosX(defaultCursorPos.x);
		ImGui::SetCursorPosY(LEFT_PANE_WINDOW_HEIGHT / 2 + ImGui::GetFrameHeightWithSpacing() + 4);

		/*BEGIN BOTTOM PANEL*/
		if (ImGui::BeginChild("BOTTOMPANE", ImVec2(CENTER_PANE_WINDOW_WIDTH, LEFT_PANE_WINDOW_HEIGHT / 2), true, ImGuiWindowFlags_NoNavInputs))
		{

			for (auto i = CENTER_PANE_INDENT; i > 0; i--)
			{
				ImGui::Indent();
			}

			//setting up dropdown box
			if (ImGui::Combo(" ", &currentItem,"General characteristics\0Performance\0") || ImGui::IsItemActivated())
			{
				if (ImGui::IsItemActivated())
				{
					audioEngine->PlayAudio(AudioEngine::SOUND_TYPE::SELECT_DROPDOWN_ITEM);
				}

				switch (currentItem)
				{
				case 0:
					selectedTable = 0;
					jsonData->SetSpecSheet(JSONData::SPEC_TYPE::GEN);
					break;
				case 1:
					selectedTable = 1;
					jsonData->SetSpecSheet(JSONData::SPEC_TYPE::PERF);
					break;
				default:
					break;
				}

				jsonData->UpdateSpecSheet();
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
						ImGui::Text(tableOptions.at(selectedTable)[i].c_str());
						ImGui::TableNextColumn();
						ImGui::Text(jsonData->GetAircraftSpecData().at(i).c_str());
					}

				}
				ImGui::EndTable();
			}

		}

		/*END BOTTOM PANEL*/
		ImGui::EndChild();

		ImGui::SetCursorPos(newCursorPos);

		/*BEGIN RIGHT PANEL*/
		if (ImGui::BeginChild("RIGHTPANE", ImVec2(LEFT_PANE_WINDOW_WIDTH, LEFT_PANE_WINDOW_HEIGHT), true, ImGuiWindowFlags_NoNavInputs))
		{

			//Minus one since the last element is for avionics
			for (auto i = 0; i < jsonData->GetRightPanelOptions().size() - 1; i++)
			{
				if (ImGui::CollapsingHeader(jsonData->GetRightPanelOptions().at(i).c_str(), ImGuiTreeNodeFlags_Leaf))
				{ 
					ImGui::TextWrapped(jsonData->GetAircraftDetails().at(i).c_str());
				}
			}

			//back() provides a reference to the last element in the container
			//so no need to pass an index or iterate here
			if (ImGui::CollapsingHeader(jsonData->GetRightPanelOptions().back().c_str(), ImGuiTreeNodeFlags_Leaf))
			{
				for (const auto& avionic : jsonData->GetAircraftAvionics())
				{
					ImGui::TextWrapped(avionic.c_str());
				}
			}
			
		}

		/*END RIGHT PANEL*/
		ImGui::EndChild();

		ImGui::End();

		window->clear(bgColor);
		ImGui::SFML::Render(*window);

		window->display();
	}


	ImGui::SFML::Shutdown();

}