#pragma once
#include <Windows.h>
#include "Functions.h"

namespace Base 
{
	bool Update();// 偏移更新
	DWORD EntityList;	// 实体列表
	DWORD LocalEntity;	// 本地实体
	DWORD ClientState;	// 客户端数据
	DWORD CameraPos;	// 相机坐标
	DWORD Matrix;		// 矩阵
	DWORD ViewAngles = 0x4D90;		// 本地旋转角
	struct EntityData_	// 实体数据
	{
		const DWORD Pos = 0x138;	// 坐标 
		const DWORD Active = 0xED;	// 激活
		const DWORD Camp = 0xF4;	// 阵营
		const DWORD Health = 0x100;	// 生命
		const DWORD AngleX = 0x130;	// 鼠标X
		const DWORD AngleY = 0x12C; // 鼠标Y
		const DWORD BoneMatrix = 0x26A8; // 骨骼矩阵
		const DWORD BoneName = 0x2950;	 // 骨骼名字
	}EntityData;
}