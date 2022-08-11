#pragma once
#include "BaseOffset.h"

/*
	Pattern   : 特征
	Off	      : 地址偏移
	ResultOff : 结果地址偏移
*/
DWORD GetAddress(char* Pattern,DWORD Module, DWORD Off, DWORD ResultOff)
{
	DWORD Result;
	std::vector<DWORD> SearchResult;
	
	SearchResult = Function::SearchMemory(pGame->Process.hProcess, Pattern, (DWORD)Module, (DWORD)Module + 0xfffffff);
	if (SearchResult.size() <= 0)
		return 0;

	Result = Function::Read<DWORD>(SearchResult.at(0) + Off);
	return Result - Module + ResultOff;
}

bool Base::Update()
{
	if ((Base::EntityList = GetAddress((char*)"BB ?? ?? ?? ?? 83 FF 01 0F 8C ?? ?? ?? ?? 3B F8",pGame->Process.Client_Module, 1, 0)) <= 0)
		return false;
	if ((Base::ClientState = GetAddress((char*)"A1 ?? ?? ?? ?? 33 D2 6A 00 6A 00 33 C9 89 B0", pGame->Process.Engine_Module, 1, 0)) <= 0)
		return false;
	if ((Base::Matrix = GetAddress((char*)"0F 10 05 ?? ?? ?? ?? 8D 85 ?? ?? ?? ?? B9", pGame->Process.Client_Module, 3, 176)) <= 0)
		return false;
	if ((Base::LocalEntity = GetAddress((char*)"C2 04 00 8D 04 92 42 56", pGame->Process.Client_Module, 11, 4)) <= 0)
		return false;

	return true;
}