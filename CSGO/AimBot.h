#pragma once
#include <Windows.h>
#include "BaseOffset.h"
#include "GameData.h"

namespace AimControl
{
	static int HotKey = VK_LMENU;	// Ãé×¼ÈÈ¼ü
	static float AimRange = 100;	// Ãé×¼·¶Î§

	void Aim_ByAngle(const Vec3& LocalPos, const Vec3& AimPos)
	{
		DWORD ClientState = Function::Read<DWORD>(pGame->Process.Engine_Module + Base::ClientState);
		DWORD AngleAddress;
		AngleAddress = ClientState + Base::ViewAngles;

		float Yaw, Pitch;
		float Distance;
		Vec3 OppPos;

		OppPos.x = AimPos.x - LocalPos.x;
		OppPos.y = AimPos.y - LocalPos.y;
		OppPos.z = AimPos.z - LocalPos.z;

		Distance = sqrt(pow(OppPos.x, 2) + pow(OppPos.y, 2));

		Yaw = atan2f(OppPos.y, OppPos.x) * 180 / M_PI;
		Pitch = -atan(OppPos.z / Distance) * 180 / M_PI;

		Function::Write<Vec2>(AngleAddress, { Pitch,Yaw });
	}
}