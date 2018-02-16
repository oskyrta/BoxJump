#pragma once
///////////////////////////////////
//Include
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\info_parser.hpp>
#include <string>
#include <list>
#include <SFML\Graphics.hpp>

///////////////////////////////////
// Enum
enum SettingsFile
{
	SettingsFile_WindowSettings,
	SettingsFile_ObjectsSettings,
	SettingsFile_InterfaceSettings,
	SettingsFile_Statistic,
	SettingsFile_Sprite,
	SettingsFile_Binds
};

///////////////////////////////////
// Sturct
struct SpriteContainer
{
	std::string name;
	sf::IntRect rect;
	sf::Texture *texturePtr;

	bool operator==(const std::string& a);
	bool operator==(const SpriteContainer& a);
};

///////////////////////////////////
// Class DataManager
class DataManager
{
public:
	DataManager() {};
	~DataManager();

	static DataManager* instance()
	{
		static DataManager *instance_ = new DataManager();
		return instance_;
	};

	void loadData();
	SpriteContainer* getSpriteContainer(std::string name);
	void writeData(SettingsFile fileName);

	boost::property_tree::ptree* getBinds()				{ return &m_binds; };
	boost::property_tree::ptree* getInterfaceSettings() { return &m_interface; };
	boost::property_tree::ptree* getWindowSettings()	{ return &m_windowSettings; };
	boost::property_tree::ptree* getObjectsSettgins()	{ return &m_objectsSettings; };
	boost::property_tree::ptree* getStatistic()			{ return &m_statistic; };
	boost::property_tree::ptree* getSpriteParameters()	{ return &m_spriteParameters; };

	sf::Font* getFont();

private:
	void loadSettings();
	void loadSprites();
	void devideSprites();
	SpriteContainer* findSprite(std::string name);

private:
	std::string m_rep; 
	boost::property_tree::ptree m_binds;
	boost::property_tree::ptree m_interface;
	boost::property_tree::ptree m_windowSettings;
	boost::property_tree::ptree m_objectsSettings;
	boost::property_tree::ptree m_statistic;
	boost::property_tree::ptree m_spriteParameters;
	
	sf::Font* m_font;
	sf::Texture m_atlas;

	std::list<SpriteContainer> m_sprites;
};