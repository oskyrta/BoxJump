#pragma once

/////////////////////////////////
// Functions

bool IsKeyDown(int virtualKeyCode);
int GetRandomInt(int min, int max);
float GetRandomFloat(float min, float max);
float getSign(float num);

template<class T> void swap(T A, T B);