#pragma once
/////////////////////////////////////////////////
// Include
#include <SFML\Graphics.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\info_parser.hpp>
#include <iostream>

/////////////////////////////////////////////////
// Enum 
enum SettingsFile
{
	SettingsFile_WindowSettings,
	SettingsFile_ObjectsSettings,
	SettingsFile_InterfaceSettings,
	SettingsFile_Statistic,
	SettingsFile_Sprite
};

/////////////////////////////////////////////////
// Class SettingsManager
class SettingsManager
{
public:
	~SettingsManager();

	void initialize();
	void writeData(SettingsFile fileName);

	boost::property_tree::ptree* p_windowSettings;
	boost::property_tree::ptree* p_objectsSettings;
	boost::property_tree::ptree* p_interfaceSettings;
	boost::property_tree::ptree* p_statistic;
	boost::property_tree::ptree* p_spriteParameters;

private:
	boost::property_tree::ptree m_windowSettings;
	boost::property_tree::ptree m_objectsSettings;
	boost::property_tree::ptree m_interfaceSettings;
	boost::property_tree::ptree m_statistic;
	boost::property_tree::ptree m_spriteParameters;

	std::string m_rep;
};