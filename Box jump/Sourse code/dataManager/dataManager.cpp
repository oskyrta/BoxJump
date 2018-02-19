#define _CRT_SECURE_NO_WARNINGS

////////////////////////////////////
// Include
#include "dataManager\dataManager.h"
#include <Windows.h>
#include <filesystem>
#include <string>
#include <strstream>
#include <list>
#include <iostream>
#include <boost\foreach.hpp>

namespace fs = std::experimental::filesystem;

////////////////////////////////////
// Functions
bool compareTex(SpriteContainer& i, SpriteContainer& j) { return i.texturePtr->getSize().y > j.texturePtr->getSize().y; };

////////////////////////////////////
// Class SpriteContainer
bool SpriteContainer::operator==(const std::string& a)
{
	if (a == this->name) return true;
	else return false;
}

bool SpriteContainer::operator==(const SpriteContainer& a)
{
	if (a.name == this->name && a.rect == this->rect && a.texturePtr == this->texturePtr) return true;
	else return false;
}

////////////////////////////////////
// Class DataManager
DataManager::~DataManager() 
{
	writeData(SettingsFile_Binds);
	//writeData(SettingsFile_InterfaceSettings);
	writeData(SettingsFile_Statistic);
	writeData(SettingsFile_ObjectsSettings);
	writeData(SettingsFile_WindowSettings);
}

sf::Font* DataManager::getFont() { return m_font; }

void DataManager::writeData(SettingsFile fileName)
{
	switch (fileName)
	{
	case SettingsFile_WindowSettings:
		boost::property_tree::info_parser::write_info(m_rep + "windowSettings.info", m_windowSettings);
		break;

	case SettingsFile_ObjectsSettings:
		boost::property_tree::info_parser::write_info(m_rep + "objectsParameters.info", m_objectsSettings);
		break;

	case SettingsFile_InterfaceSettings:
		boost::property_tree::info_parser::write_info(m_rep + "interface.info", m_interface);
		break;

	case SettingsFile_Statistic:
		boost::property_tree::info_parser::write_info(m_rep + "statistic.info", m_statistic);
		break;

	case SettingsFile_Sprite:
		boost::property_tree::info_parser::write_info(m_rep + "spriteParameters.info", m_spriteParameters);
		break;

	case SettingsFile_Binds:
		boost::property_tree::info_parser::write_info(m_rep + "binds.info", m_binds);
		break;
	}
}

SpriteContainer* DataManager::getSpriteContainer(std::string name)
{
	for (auto & s : m_sprites)
		if (s == name) return &s;

	return 0;
}

void DataManager::loadSettings()
{
	std::ifstream in(m_rep + "binds.info");
	boost::property_tree::info_parser::read_info(in, m_binds);
	in.close();

	in.open(m_rep + "interface.info");
	boost::property_tree::info_parser::read_info(in, m_interface);
	in.close();

	in.open(m_rep + "windowSettings.info");
	boost::property_tree::info_parser::read_info(in, m_windowSettings);
	in.close();

	in.open(m_rep + "objectsParameters.info");
	boost::property_tree::info_parser::read_info(in, m_objectsSettings);
	in.close();

	in.open(m_rep + "statistic.info");
	boost::property_tree::info_parser::read_info(in, m_statistic);
	in.close();

	in.open(m_rep + "spriteParameters.info");
	boost::property_tree::info_parser::read_info(in, m_spriteParameters);
	in.close();
}

SpriteContainer* DataManager::findSprite(std::string name) {
	for (auto & t : m_sprites) {
		if (t == name) {
			return &t;
		}
	}

	return 0;
}

void DataManager::loadSprites(){
	int spritesCount = 0;

	std::strstream paths;
	std::string pathToSprites = "Resources\\Sprites\\", path;

	for (auto & p : fs::directory_iterator(pathToSprites))
	{
		// Get path to sprite
		paths << p;
		paths >> path;
		paths.clear();

		SpriteContainer container; sf::Texture* tex = new sf::Texture();
		// Write temp texture to container
		container.texturePtr = new sf::Texture();
		container.texturePtr->loadFromFile(path);
		// Write sprite name from path
		container.name = path.substr(pathToSprites.length(), path.length() - pathToSprites.length() - 4);

		//std::cout << container.name << std::endl;

		spritesCount++;
		m_sprites.push_back(container);
	}
	m_sprites.sort(compareTex);

	int currentLevel = 0, nextLevel = 0, levelFill, maxSize;
	levelFill = maxSize = 256;

	for (auto & t : m_sprites)
	{
		if (levelFill + t.texturePtr->getSize().x > maxSize)
		{
			currentLevel = nextLevel;
			nextLevel += t.texturePtr->getSize().y;
			levelFill = 0;
		}

		sf::IntRect rect;
		rect.left = levelFill;
		rect.top = currentLevel;
		rect.width = t.texturePtr->getSize().x;
		rect.height = t.texturePtr->getSize().y;
		t.rect = rect;

		levelFill += rect.width;
	}
	sf::Image atlas;
	atlas.create(maxSize, nextLevel);

	for (auto & t : m_sprites)
	{
		atlas.copy(t.texturePtr->copyToImage(), t.rect.left, t.rect.top);
		t.texturePtr = &m_atlas;
	}
	m_atlas.loadFromImage(atlas);
}

void DataManager::devideSprites() {
	BOOST_FOREACH(auto &v, m_spriteParameters){
		const SpriteContainer* sprite = findSprite(v.first);

		if (sprite) {
			boost::property_tree::ptree* sprites = m_spriteParameters.get_child_optional(v.first).get_ptr();

			BOOST_FOREACH(auto &s, *sprites) {
				SpriteContainer newSprite;
				newSprite.name = s.first;
				newSprite.rect.height = s.second.get<int>("height");
				newSprite.rect.width = s.second.get<int>("width");
				newSprite.rect.top = sprite->rect.top + s.second.get<int>("y");
				newSprite.rect.left = sprite->rect.left + s.second.get<int>("x");
				newSprite.texturePtr = sprite->texturePtr;

				m_sprites.push_back(newSprite);
			}
			m_sprites.remove(*sprite);
		}
	}
}

void DataManager::loadData()
{
	//Get path to settings folder in AppData\Roaming
	m_rep = getenv("appdata");
	m_rep += "\\Box Jump\\Settings\\";

	m_font = new sf::Font();
	m_font->loadFromFile("Resources\\stan0753.ttf");

	loadSettings();
	loadSprites();
	devideSprites();
}