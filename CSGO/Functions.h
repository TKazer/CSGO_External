#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <atlconv.h>
#include <vector>
#include "GameData.h"

namespace Function {
	// ��ȡ����ID
	DWORD GetProcessID(const char* ProcessName);
	// ��ȡ����ģ����
	HMODULE GetProcessModuleHandle(DWORD Pid, const char* ModuleName);
	// �ڴ�����
	std::vector<DWORD> SearchMemory(HANDLE hProcess, char* Pattern, DWORD StartAddress, DWORD EndAddress);
	// �ֽ�����ת����
	int BytesToInt(byte* Bytes);
	// ����ϵ��ת
	Vec2 RevolveCoordinatesSystem(float RevolveAngle, Vec2 OriginPos, Vec2 DestPos);
	int COLOR_RGBA(const float* Color)
	{
		return ImColor(Color[0], Color[1], Color[2], Color[3]);
	}

	template <typename Type>
	Type Read(DWORD Address)
	{
		Type Temp;
		if (!ReadProcessMemory(pGame->Process.hProcess, reinterpret_cast<LPCVOID>(Address), &Temp, sizeof(Type), 0))
			return Type{};
		return Temp;
	}

	template <typename Type>
	bool Read(DWORD Address,Type* Buffer,int Size)
	{
		return ReadProcessMemory(pGame->Process.hProcess, reinterpret_cast<LPCVOID>(Address), Buffer, Size, 0);
	}

	template <typename Type>
	bool Write(DWORD Address, const Type& Value)
	{
		return WriteProcessMemory(pGame->Process.hProcess, reinterpret_cast<LPVOID>(Address),&Value, sizeof(Type), 0);
	}

	auto ReadMemory(DWORD ptr, void* buff, size_t size)->bool
	{
		return ReadProcessMemory(pGame->Process.hProcess, (void*)ptr, buff, size, NULL);
	}
}