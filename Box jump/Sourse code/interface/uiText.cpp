#define _SCL_SECURE_NO_WARNINGS

////////////////////////////////////////////////
// Include
#include "uiText.h"
#include "render/camera.h"
#include "dataManager\dataManager.h"

////////////////////////////////////////////////
// Class UIText

UIText::UIText() {
	m_font = 0;
	m_intChangedValue = 0;
	m_doubleChangedValue = 0;
	m_characterSize = 8;
	m_align = Vec2();

	m_changedValuePosition = 0;

	m_text = sf::Text();
}

float getAlignCoefByString(std::string align) {
	if (align == "Left"   ||  align == "Bottom")	return 0;
	if (align == "Center" ||  align == "Middle")	return -0.5f;
	if (align == "Right"  ||  align == "Top")		return -1;

	return 0;
}

void UIText::initialize(PropertyTree* tree) {
	UIObject::initialize(tree);
	
	m_string = m_tree->get<std::string>("String", "");
	m_characterSize = m_tree->get<int>("CharacterSize");

	m_color.r = m_tree->get<int>("Color.r");
	m_color.g = m_tree->get<int>("Color.g");
	m_color.b = m_tree->get<int>("Color.b");

	initializeText();

	m_size = m_textSize;
}

void UIText::initializeText() {
	// Initialize text variable
	m_text.setFont(*m_dataManager->getFont());
	m_text.setCharacterSize(m_characterSize);
	m_text.setString(m_string);
	m_text.setFillColor(m_color);

	m_align.x = getAlignCoefByString(m_tree->get<std::string>("VAlign", ""));
	m_align.y = getAlignCoefByString(m_tree->get<std::string>("HAlign", ""));

	m_textSize.x = m_text.getLocalBounds().width;
	m_textSize.y = m_text.getLocalBounds().height;

	int i = 0;
	while(i < m_string.size()) {
		if (m_string[i] == '^') {
			m_changedValuePosition = i;
			break;
		}
		i++;
	}

	m_string.copy(s_start, m_changedValuePosition, 0);
	if(m_changedValuePosition > 0)
		m_string.copy(s_end, m_string.size() - (m_changedValuePosition + 1), m_changedValuePosition+1);
}

void UIText::render() {
	if (m_intChangedValue) 
		m_text.setString(s_start + std::to_string(*m_intChangedValue) + s_end);
	if (m_doubleChangedValue)
		m_text.setString(s_start + std::to_string(*m_doubleChangedValue) + s_end);

	m_text.setPosition(getTextPosition());

	m_camera->getRenderWindow()->draw(m_text);
	UIObject::render();
}

void UIText::update() {
	UIObject::update();
}

sf::Vector2f UIText::getTextPosition() { 
	return (m_camera->getCenterPosition() + m_textOffset + m_offset + m_pos + m_textSize*m_align).getInt().getSFVector();
};
