#include <iostream>
#include <Windows.h>
#include <string>
#include <iomanip>
#include <mutex>
#include "BaseOffset.h"
#include "Entity.h"
#include "Render.hpp"
#include "MenuConfig.h"
#include "AimBot.h"
#include "Radar.h"

// Author: Liv , email: 1319923129@qq.com

void Menu()
{
	if (ImGui::CollapsingHeader("ESP"))
	{
		ImGui::Checkbox("Box", &MenuConfig::ShowBox);
		ImGui::Checkbox("Health", &MenuConfig::ShowHealth);
		ImGui::Checkbox("LosLine", &MenuConfig::ShowLosLine);
		ImGui::Checkbox("Cube", &MenuConfig::ShowCube);
		ImGui::SliderInt("Cube Sides", &MenuConfig::CubeSides, 4, 360);
		ImGui::Checkbox("MagicCircle", &MenuConfig::ShowMagicCircle);
		ImGui::Checkbox("Circle", &MenuConfig::ShowCircle);
		ImGui::Checkbox("Bone", &MenuConfig::Bone);
		ImGui::Checkbox("Radar", &MenuConfig::Radar);
		ImGui::Combo("RadarType", &MenuConfig::RadarType, " Circle \0 Arrow \0 Circle&Arc \0");
	}
	if (ImGui::CollapsingHeader("Aim"))
	{
		ImGui::Checkbox("AimBot", &MenuConfig::AimBot);
		ImGui::Combo("AimType", &MenuConfig::AimType, " Memory \0 API \0");
		ImGui::Combo("HotKey", &MenuConfig::AimHotKey, " LALT \0 CAPITAL \0");
		{
			switch (MenuConfig::AimHotKey)
			{
			case 0:
				AimControl::HotKey = VK_LMENU;
				break;
			case 1:
				AimControl::HotKey = VK_CAPITAL;
				break;
			}
		}
		ImGui::SliderFloat("AimRange", &AimControl::AimRange, 50, 200);
	}
}

CEntity* LocalEntity = new CEntity; // 本地实体

void UpdateData()
{
	while (true)
	{
		// 本地玩家
		LocalEntity->EntityAddress = Function::Read<DWORD>(pGame->Process.Client_Module + Base::LocalEntity);
		LocalEntity->UpDate();

		// 矩阵&相机坐标
		ReadProcessMemory(pGame->Process.hProcess, (LPCVOID)(pGame->Process.Client_Module + Base::Matrix), &pGame->View->Matrix, 64, 0);
		pGame->View->CameraPos = Function::Read<Vec3>(pGame->Process.Client_Module + Base::CameraPos);
	}
}

void DrawMain()
{
	Menu();

	// 自瞄数据
	float DistanceToSight = 0;
	float MaxAimDistance = 100000;
	Vec3  AimPos{ 0,0,0 };

	// 雷达
	Base_Radar Radar;
	Radar.SetPos({ (float)pGame->View->Gui->Window.Width / 2,(float)pGame->View->Gui->Window.Height / 2 });
	Radar.SetProportion(2680);
	Radar.SetRange(150);
	Radar.SetCrossColor(ImColor(0, 0, 0, 255));
	Radar.Opened = MenuConfig::Radar;

	// 瞄准范围绘制
	pGame->View->Gui->Circle(
		{ (float)pGame->View->Gui->Window.Width / 2,(float)pGame->View->Gui->Window.Height / 2 },
		AimControl::AimRange, ImColor(255, 255, 255, 255), 1);

	for (int i = 0; i < 20; i++)
	{
		DWORD Address = Function::Read<DWORD>(pGame->Process.Client_Module + Base::EntityList + 16 * i);
		if (Address == 0)
			continue;
		CEntity Entity;
		Entity.EntityAddress = Address;

		if (!Entity.UpDate())
			continue;
		// 过滤本人
		if (LocalEntity->EntityAddress == Entity.EntityAddress)
			continue;
		// 过滤队友
		//if (LocalEntity->Camp == Entity.Camp)
			//continue;

		// 添加到雷达
		Radar.AddPoint(LocalEntity->Pos, LocalEntity->AngleX, Entity.Pos, ImColor(237, 85, 106, 200), MenuConfig::RadarType, Entity.AngleX);
		// 更新屏幕坐标数据
		if (!Entity.UpDataScreenPos())
			continue;

		// 功能开关
		if (MenuConfig::ShowBox)
			Render.ShowRect(&Entity, 1.5, IM_COL32(255, 255, 0, 255));
		if (MenuConfig::ShowHealth)
			pGame->View->Gui->Text(std::to_string(Entity.Health).c_str(), { Entity.Rect.point.x,Entity.Rect.point.y - 20 }, IM_COL32(255, 255, 0, 255));
		if (MenuConfig::ShowLosLine)
			Render.ShowLosLine(&Entity, 50, IM_COL32(255, 0, 0, 255));
		if (MenuConfig::ShowCube)
			Render.ShowCube(&Entity, MenuConfig::CubeSides, 30, 1.5, IM_COL32(0, 255, 0, 255));
		if (MenuConfig::ShowMagicCircle)
			Render.ShowMagicCircle(&Entity, 30, 1.5);
		if (MenuConfig::ShowCircle)
			Render.CirCle3D_RainBow(&Entity, 30, 1);
		if(MenuConfig::Bone)
			Render.DrawBone(&Entity, ImColor(255, 255, 255, 255));

		// 自瞄最近选择
		if (MenuConfig::AimBot)
		{
			DistanceToSight = sqrt(pow(Entity.Head.ScreenPos.x - pGame->View->Gui->Window.Width / 2, 2) +
				pow(Entity.Head.ScreenPos.y - pGame->View->Gui->Window.Height / 2, 2));
			if (DistanceToSight < AimControl::AimRange)
			{
				if (DistanceToSight < MaxAimDistance)
				{
					MaxAimDistance = DistanceToSight;
					AimPos = Entity.Head.Pos;
				}
			}
		}
	}

	// 雷达渲染
	Radar.Render();

	if (MenuConfig::AimBot && GetAsyncKeyState(AimControl::HotKey))
	{
		if (AimPos != 0)
		{
			if (MenuConfig::AimType == 0)
				AimControl::Aim_ByAngle(pGame->View->CameraPos, AimPos);
			else if (MenuConfig::AimType == 1)
				;//...
		}
	}
}

int main()
{
	// 游戏信息指针初始
	pGame = new CGame;
	pGame->View = new CView("Menu");
	pGame->Process.Pid = Function::GetProcessID("csgo.exe");

	if (pGame->Process.Pid <= 0)
	{
		std::cout << "Get \"csgo.exe\" process-id failed." << std::endl;
		system("pause");
		return 0;
	}

	pGame->Process.hProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_CREATE_THREAD, TRUE, pGame->Process.Pid);
	pGame->Process.Client_Module = reinterpret_cast<DWORD>(Function::GetProcessModuleHandle(pGame->Process.Pid, "client.dll"));
	pGame->Process.Engine_Module = reinterpret_cast<DWORD>(Function::GetProcessModuleHandle(pGame->Process.Pid, "engine.dll"));

	// 更新偏移
	if (!Base::Update())
	{
		std::cout << "Updating Offset failed." << std::endl;
		system("pause");
		return 0;
	}
	Base::CameraPos = Base::Matrix - 0x284;

	std::cout << "Pid:" << pGame->Process.Pid << std::endl;
	std::cout << "Client:" << pGame->Process.Client_Module << std::endl;
	std::cout << "Engine:" << pGame->Process.Engine_Module << std::endl;

	std::cout << "Offset:" << std::endl;
	std::cout << "--EntityList:" << std::setiosflags(std::ios::uppercase) << std::hex << Base::EntityList << std::endl;
	std::cout << "--LocalEntity:" << std::setiosflags(std::ios::uppercase) << std::hex << Base::LocalEntity << std::endl;
	std::cout << "--Matrix:" << std::setiosflags(std::ios::uppercase) << std::hex << Base::Matrix << std::endl;
	std::cout << "--ClientState:" << std::setiosflags(std::ios::uppercase) << std::hex << Base::ClientState << std::endl;

	// 启动获取数据线程
	std::thread Thread_UpdateData(UpdateData);

	std::cout << "Runing..." << std::endl;
	pGame->View->Gui->Start("", "Counter-Strike: Global Offensive - Direct3D 9", DrawMain);
	delete pGame;
	return 0;
}
