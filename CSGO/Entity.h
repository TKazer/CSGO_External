#pragma once
#include "Struct.h"
#include "BaseOffset.h"
#include "Functions.h"
#include "GameData.h"
#include "Bones.h"

class CEntity	// 游戏实体类
{
public:
	DWORD EntityAddress = 0x0;		// 实体地址

	Vec3 Pos{ 0,0,0 };				// 实体坐标
	Vec3 ScreenPos{ 0,0,0 };		// 屏幕坐标
	RECINFO Rect;					// 方框数据
	int Health = 0;					// 生命
	const float InitHealth = 100.0; // 初始生命
	float AngleX = 0.0f;			// 鼠标X角度值
	float AngleY = 0.0f;			// 鼠标Y角度值
	float Height = 0.0f;			// 实体高度
	int Camp = -1;					// 阵营
	CBone Bone;						// 骨骼
	CBoneJoint Head;				// 头部骨骼

	// 实体数据更新
	bool UpDate();
	// 更新方框数据
	bool UpDataScreenPos();
	// 是否激活
	bool IsActive();
private:
	// 更新坐标
	bool GetPos();
	// 更新血量
	bool GetHealth();
	// 更新阵营
	bool GetCamp();
	// 更新角度数据
	bool GetAngles();
};

