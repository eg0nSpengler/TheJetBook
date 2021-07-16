#pragma once
#include <fstream>
#include <vector>

#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
class JSONData
{
public:
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

	JSONData();

	/// <summary>
	/// Returns the details associated with the current aircraft
	/// </summary>
	const std::vector<std::string>& GetAircraftDetails();
	/// <summary>
	/// Returns the avionics associated with the current aircraft
	/// </summary>
	const std::vector<std::string>& GetAircraftAvionics();
	/// <summary>
	/// Returns the individual spec data associated with the current aircraft
	/// </summary>
	const std::vector<std::string>& GetAircraftSpecData();
	/// <summary>
	/// Returns the right panel options
	/// </summary>
	/// <returns></returns>
	const std::vector<std::string>& GetRightPanelOptions();
	/// <summary>
	/// Returns the image associated with the current  aircraft
	/// </summary>
	const sf::Sprite& GetAircraftImage();
	/// <summary>
	/// Sets the current aircraft to be displayed
	/// </summary>
	/// <param name="selectedAircraft">The selected aircraft name</param>
	void SetCurrentAircraft(std::string selectedAircraft);
	/// <summary>
	/// Sets the new spec sheet type to be displayed
	/// </summary>
	/// <param name="newType">The type of spec sheet to switch to</param>
	void SetSpecSheet(SPEC_TYPE newType);
	/// <summary>
	/// Updates the spec sheet data
	/// </summary>
	void UpdateSpecSheet();
	/// <summary>
	/// Updates the current aircraft details
	/// </summary>
	void UpdateAircraftDetails();

public:
	std::unique_ptr<nlohmann::json> json;

private:
	void Init();
	void GetAircraftSpecs(const std::multimap<std::string, std::string>& map);

private:
	const std::string imgFilePath = "./Content/Images/";
	const std::string aircraftJSONFilePath = "Content/aircraft.json";

	std::string lastAircraft;
	std::string currentAircraft;
	SPEC_TYPE selectedSpecSheet;

private:
	/// <summary>
	/// Contains the images associated with each aircraft
	/// </summary>
	std::map<std::string, sf::Sprite> aircraftImages;
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

	std::vector<std::string> rightPanelOptions;
	std::vector<std::string> currentDetails;
	std::vector<std::string> currentAvionics;
	std::vector<std::string> currentSpecData;
	std::vector<std::string> aircraftCategories;
	std::vector<std::string> aircraftNames;
	std::vector<std::shared_ptr<sf::Texture>> imgTextures;
};

