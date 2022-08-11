#include "Functions.h"

Vec2 Function::RevolveCoordinatesSystem(float RevolveAngle, Vec2 OriginPos, Vec2 DestPos)
{
	Vec2 ResultPos;
	if (RevolveAngle == 0)
		return DestPos;
	ResultPos.x = OriginPos.x + (DestPos.x - OriginPos.x) * cos(RevolveAngle * M_PI / 180) + (DestPos.y - OriginPos.y) * sin(RevolveAngle * M_PI / 180);
	ResultPos.y = OriginPos.y - (DestPos.x - OriginPos.x) * sin(RevolveAngle * M_PI / 180) + (DestPos.y - OriginPos.y) * cos(RevolveAngle * M_PI / 180);
	return ResultPos;
}

DWORD Function::GetProcessID(const char* ProcessName)
{
	PROCESSENTRY32 ProcessInfoPE;
	ProcessInfoPE.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(15, 0);
	Process32First(hSnapshot, &ProcessInfoPE);
	USES_CONVERSION;
	do{
		if (strcmp(W2A(ProcessInfoPE.szExeFile), ProcessName) == 0)
		{
			CloseHandle(hSnapshot);
			return ProcessInfoPE.th32ProcessID;
		}
	} while (Process32Next(hSnapshot, &ProcessInfoPE));
	CloseHandle(hSnapshot);
	return 0;
}

HMODULE Function::GetProcessModuleHandle(DWORD Pid, const char* ModuleName)
{
	MODULEENTRY32 ModuleInfoPE;
	ModuleInfoPE.dwSize = sizeof(MODULEENTRY32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, Pid);
	Module32First(hSnapshot, &ModuleInfoPE);
	USES_CONVERSION;
	do {
		if (strcmp(W2A(ModuleInfoPE.szModule), ModuleName) == 0)
		{
			CloseHandle(hSnapshot);
			return ModuleInfoPE.hModule;
		}
	} while (Module32Next(hSnapshot, &ModuleInfoPE));
	CloseHandle(hSnapshot);
	return 0;
}

int Function::BytesToInt(byte* Bytes)
{
	int Result = Bytes[0] & 0xFF;
	Result |= ((Bytes[1] << 8) & 0xFF00);
	Result |= ((Bytes[2] << 16) & 0xFF0000);
	Result |= ((Bytes[3] << 24) & 0xFF000000);
	return Result;
}