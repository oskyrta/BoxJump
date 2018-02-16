#pragma once
/////////////////////////////////////////////////
// Include
#include "uiObject.h"
#include <SFML\Graphics.hpp>

/////////////////////////////////////////////////
// Class UIText
class UIText : public UIObject
{
public:
	UIText();
	~UIText() {};

	void render();
	void update();

	void setChangedValue(const int* changedValue) { m_intChangedValue = changedValue; };
	void setChangedValue(const double* changedValue) { m_doubleChangedValue = changedValue; };
	void setString(std::string string) { m_text.setString(string); };
	sf::Vector2f getTextPosition();

	void initialize(PropertyTree* tree);

protected:
	void initializeText();

protected:
	std::string m_string;
	sf::Font* m_font;
	sf::Text m_text;
	sf::Color m_color;
	Vec2 m_align;

	Vec2 m_textOffset;
	Vec2 m_offset;
	Vec2 m_textSize;

	int m_characterSize; 
	char s_start[32], s_end[32];

	const int* m_intChangedValue;
	const double* m_doubleChangedValue;
	size_t m_changedValuePosition;
};