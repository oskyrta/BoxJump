#pragma once
////////////////////////////////////////////////
// Include
#include <string>

////////////////////////////////////////////////
// Forward declaration
class Vec2;
enum HeroSkin;

/////////////////////////////////
// Functions

bool IsKeyDown(int virtualKeyCode);
int GetRandomInt(int min, int max);
float GetRandomFloat(float min, float max);
float getSign(float num);

Vec2 GetPositionByTag(std::string tag);

template<class T> void swap(T &A, T &B);

std::string getSpriteNameBySkin(HeroSkin skin);