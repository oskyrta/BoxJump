#pragma once
/////////////////////////////////////////////////
// Include
#include "guiObject.h"
#include <iostream>
#include <SFML\Graphics.hpp>

/////////////////////////////////////////////////
// Class GUIText
class GUIText : public virtual GUIObject
{
public:
	GUIText(Camera* camera, std::string tag, Vec2 halfSize) 
		: GUIObject(camera, tag, halfSize) 
	{
		m_font = 0;
		m_intChangedValue = 0;
		m_doubleChangedValue = 0;
		m_characterSize = 8;

		m_changedValuePosition = 0;

		m_text = sf::Text();
	};

	virtual void render();

	void setChangedValue(const int* changedValue) { m_intChangedValue = changedValue; };
	void setChangedValue(const double* changedValue) { m_doubleChangedValue = changedValue; };
	void setString(std::string string) { m_text.setString(string);  };
	sf::Vector2f getTextPosition();

	void initialize(std::string string, sf::Font* font, int characterSize, sf::Color color = sf::Color(45, 45, 45));

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