#pragma once
/////////////////////////////////////////////////
// Include
#include <SFML\Graphics.hpp>
#include "settingsManager.h"


////////////////////////////////////////////////
// Const

const sf::IntRect kBoxImage = sf::IntRect(0, 0, 16, 16);
const sf::IntRect kPlatformImage = sf::IntRect(16, 0, 32, 8);
const sf::IntRect kBrokenPlatformImage = sf::IntRect(16, 8, 32, 8);
const sf::IntRect kPauseButtonImage = sf::IntRect(48, 0, 14, 14);

const int kPixlelsInRow = 400;
const int kPixlelsInColumn = 640;

const int kMaxObjectsCount = 256;

const float kPlayerSpeedX = 150.0f;
const float kAirResistance = 0.5f;
const float kGaravityAcceleration = 9800.0f;
const float kHeroJumpSpeed = 2000.0f;
const float kCameraSpeed = -300;

const float kCellSize = 64;