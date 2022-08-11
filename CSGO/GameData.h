#pragma once
#include "GameView.h"

class CGame
{
public:
	struct Process_
	{
		DWORD Client_Module = 0;
		DWORD Engine_Module = 0;
		HANDLE hProcess = 0;
		DWORD Pid = 0;
	}Process;
	CView* View = nullptr;
}*pGame;
