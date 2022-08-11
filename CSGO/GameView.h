#pragma once
#include "OwnImGui.h"

class CView // ”Œœ∑ ”Õº¿‡
{
public:
	CView(const char* MenuName)
	{
		Gui = new OwnImGui(MenuName);
	}
	OwnImGui* Gui = nullptr;
	float Matrix[4][4]{};
	Vec3  CameraPos;
	bool WorldToScreen(const Vec3& Pos, Vec2& ToPos);
	bool WorldToScreen(const Vec3& Pos, Vec3& ToPos,float Height);
};
