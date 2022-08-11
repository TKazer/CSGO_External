#pragma once
#include <Windows.h>
#include "Functions.h"

namespace Base 
{
	bool Update();// ƫ�Ƹ���
	DWORD EntityList;	// ʵ���б�
	DWORD LocalEntity;	// ����ʵ��
	DWORD ClientState;	// �ͻ�������
	DWORD CameraPos;	// �������
	DWORD Matrix;		// ����
	DWORD ViewAngles = 0x4D90;		// ������ת��
	struct EntityData_	// ʵ������
	{
		const DWORD Pos = 0x138;	// ���� 
		const DWORD Active = 0xED;	// ����
		const DWORD Camp = 0xF4;	// ��Ӫ
		const DWORD Health = 0x100;	// ����
		const DWORD AngleX = 0x130;	// ���X
		const DWORD AngleY = 0x12C; // ���Y
		const DWORD BoneMatrix = 0x26A8; // ��������
		const DWORD BoneName = 0x2950;	 // ��������
	}EntityData;
}