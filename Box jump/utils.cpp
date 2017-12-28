////////////////////////////////////////////
// Include
#include "utils.h"
#include "hero.h"
#include "vec2.h"
#include "settingsManager.h"
#include <Windows.h>

////////////////////////////////////////////
// Extern declaration
extern SettingsManager settingsManager;

////////////////////////////////////////////
// Funtions
bool IsKeyDown(int virtualKeyCode)
{
	short keyState = GetAsyncKeyState(virtualKeyCode);
	return ((keyState & 0x8000) > 0);
}

int GetRandomInt(int min, int max)
{
	return ( min + rand() % (max - min + 1) );
}

float GetRandomFloat(float min, float max)
{
	return ( min + (max - min) * (rand()%101 / 100.0) );
}

float getSign(float num)
{
	return (num < 0) ? -1 : 1;
}

Vec2 GetPositionByTag(std::string tag)
{
	return Vec2(settingsManager.p_interfaceSettings->get<int>(tag + ".x", 0), settingsManager.p_interfaceSettings->get<int>(tag + ".y", 0));
}

template<class T>
void swap(T &A, T &B)
{
	T tmp = A;
	A = B;
	B = tmp;
}

std::string getSpriteNameBySkin(HeroSkin skin)
{
	switch (skin)
	{
	case HeroSkin_Chest:	return "ChestSprite";
	case HeroSkin_Pallet:	return "PalletSprite";
	case HeroSkin_Present:	return "PresentSprite";
	default:				return "ChestSprite";
	}
}