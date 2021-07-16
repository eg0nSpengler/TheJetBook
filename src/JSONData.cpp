#include "JSONData.h"
#include "UIDefines.h"

#include <iostream>
JSONData::JSONData()
{
	json = std::make_unique<nlohmann::json>();
	selectedSpecSheet = SPEC_TYPE::GEN;
	rightPanelOptions = { "Role", "Nation", "Manufacturer", "First flight", "Service introduction", "Number built", "Operational status", "NATO designation/nickname", "Avionics" };

	Init();
}

void JSONData::Init()
{
	std::ifstream ifs(aircraftJSONFilePath);

	ifs >> *json.get();

	for (const auto& elem : json->items())
	{
		//adding each aircraft category
		aircraftCategories.push_back(elem.key());

		//getting each aircraft from each category
		for (const auto& item : *json->find(elem.key()))
		{
			auto tmpImg = std::string(item["img"]); //aircraft image
			auto name = std::string(item["name"]); //aircraft name
			auto tmpTexture = sf::Texture(); //texture for aircraft image

			//adding each aircraft name to a container
			aircraftNames.push_back(name);

			//in here were getting the images for each aircraft
			//and assigning them to a sf::Sprite
			if (tmpTexture.loadFromFile(imgFilePath + tmpImg))
			{
				//for those wondering why a separate container for textures is necessary
				//https://www.sfml-dev.org/tutorials/2.3/graphics-sprite.php#the-white-square-problem
				//if you didnt read that the short version is that when a sprite sets its texture
				//it doesn't get an actual copy of the texture, it just stores a pointer to the texture itself
				imgTextures.push_back(std::make_shared<sf::Texture>(tmpTexture));

				//here we're assigning each aircraft string with its own respective image
				aircraftImages.insert(std::pair<std::string, sf::Sprite>(name, sf::Sprite(*imgTextures.back())));
			}

			currentAircraft = name;
		}

		
	}

	//in here we're getting all the spec containers for each aircraft
	//and placing them in their respective maps
	for (const auto& elem : json->items())
	{

		//going through each category
		for (const auto& item : *json->find(elem.key()))
		{
			//getting each aircraft name
			auto name = item["name"];

			//getting each individual spec
			
			
			//getting the general specs for each aircraft
			for (const auto& genSpec : item["genspecs"])
			{
				
				//associating the aircraft with its listed specs
				aircraftGenData.insert(std::pair<std::string, std::string>(name, genSpec));
			}

			//getting the performance stats
			for (const auto& perfSpec : item["perf"])
			{

				aircraftPerfData.insert(std::pair<std::string, std::string>(name, perfSpec));
			}

			//getting the avionics
			for (const auto& aviSpec : item["avi"])
			{

				aircraftAvionicData.insert(std::pair<std::string, std::string>(name, aviSpec));
			}

			//getting details such as maker/nation
			for (const auto& detailSpec : item["details"])
			{

				aircraftDetailData.insert(std::pair<std::string, std::string>(name, detailSpec));
			}
		}

	}

	SetSpecSheet(SPEC_TYPE::GEN);
	UpdateAircraftDetails();
	UpdateSpecSheet();
}

void JSONData::GetAircraftSpecs(const std::multimap<std::string, std::string>& map)
{
	auto search = map.begin();

	if (search != map.end())
	{
		//getting the range of values for our aircraft key
		auto range = map.equal_range(currentAircraft);

		//for each value under this aircraft key
		for (auto i = range.first; i != range.second; ++i)
		{
			//add our values into this container
			//to be displayed on the spec sheet
			currentSpecData.push_back(i->second.c_str());
		}
	}
}

void JSONData::SetSpecSheet(SPEC_TYPE newSpecs)
{
	if (newSpecs == selectedSpecSheet)
	{
		return;
	}

	selectedSpecSheet = newSpecs;
}

const sf::Sprite& JSONData::GetAircraftImage()
{
	if (currentAircraft == lastAircraft)
	{
		//no need to loop through the map
		return aircraftImages.at(currentAircraft);
	}

	lastAircraft = currentAircraft;

	if (aircraftImages.find(currentAircraft) != aircraftImages.end())
	{
		return aircraftImages.at(currentAircraft);
	}

	return aircraftImages.at(0);
}

const std::vector<std::string>& JSONData::GetRightPanelOptions()
{
	return rightPanelOptions;
}

const std::vector<std::string>& JSONData::GetAircraftDetails()
{
	return currentDetails;
}

const std::vector<std::string>& JSONData::GetAircraftAvionics()
{
	return currentAvionics;
}

const std::vector<std::string>& JSONData::GetAircraftSpecData()
{
	return currentSpecData;
}

void JSONData::SetCurrentAircraft(std::string selectedAircraft)
{
	currentAircraft = selectedAircraft;
	UpdateAircraftDetails();
	UpdateSpecSheet();
}

void JSONData::UpdateSpecSheet()
{
	//we already have data in our container
	//clear it from the previous selection
	if (currentSpecData.size() > 0)
	{
		currentSpecData.clear();
	}

	switch (selectedSpecSheet)
	{
	case SPEC_TYPE::GEN:
		GetAircraftSpecs(aircraftGenData);
		break;
	case SPEC_TYPE::PERF:
		GetAircraftSpecs(aircraftPerfData);
		break;
	case SPEC_TYPE::AVI:
		GetAircraftSpecs(aircraftAvionicData);
		break;
	default:
		break;
	}
}

void JSONData::UpdateAircraftDetails()
{

	if (currentDetails.size() > 0 && currentAvionics.size() > 0)
	{
		currentDetails.clear();
		currentAvionics.clear();
	}

	auto range = aircraftDetailData.equal_range(currentAircraft);

	for (auto i = range.first; i != range.second; ++i)
	{
		currentDetails.push_back(i->second.c_str());
	}

	range = aircraftAvionicData.equal_range(currentAircraft);

	for (auto i = range.first; i != range.second; ++i)
	{
		currentAvionics.push_back(i->second.c_str());
	}
}
