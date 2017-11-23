#pragma once
/////////////////////////////////////////////////
// Include
#include "guiObject.h"
#include <iostream>
#include <SFML\Graphics.hpp>

/////////////////////////////////////////////////
// Class GUIText
class GUIText : public GUIObject
{
public:
	GUIText();

	virtual void render();
	virtual void update();

	void setChangedValue(const int* changedValue) { m_intChangedValue = changedValue; };
	void setChangedValue(const double* changedValue) { m_doubleChangedValue = changedValue; };
	sf::Vector2f getTextPosition();

	void initialize(std::string string, Vec2 position, Vec2 halfSize, sf::Font* font, int characterSize, sf::Color color = sf::Color(45, 45, 45));

protected:
	std::string m_string;
	sf::Font* m_font;
	sf::Text m_text;

	Vec2 m_textOffset;

	int m_characterSize; 
	char s_start[32], s_end[32];

	const int* m_intChangedValue;
	const double* m_doubleChangedValue;
	size_t m_changedValuePosition;
};