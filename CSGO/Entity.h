#pragma once
#include "Struct.h"
#include "BaseOffset.h"
#include "Functions.h"
#include "GameData.h"
#include "Bones.h"

class CEntity	// ��Ϸʵ����
{
public:
	DWORD EntityAddress = 0x0;		// ʵ���ַ

	Vec3 Pos{ 0,0,0 };				// ʵ������
	Vec3 ScreenPos{ 0,0,0 };		// ��Ļ����
	RECINFO Rect;					// ��������
	int Health = 0;					// ����
	const float InitHealth = 100.0; // ��ʼ����
	float AngleX = 0.0f;			// ���X�Ƕ�ֵ
	float AngleY = 0.0f;			// ���Y�Ƕ�ֵ
	float Height = 0.0f;			// ʵ��߶�
	int Camp = -1;					// ��Ӫ
	CBone Bone;						// ����
	CBoneJoint Head;				// ͷ������

	// ʵ�����ݸ���
	bool UpDate();
	// ���·�������
	bool UpDataScreenPos();
	// �Ƿ񼤻�
	bool IsActive();
private:
	// ��������
	bool GetPos();
	// ����Ѫ��
	bool GetHealth();
	// ������Ӫ
	bool GetCamp();
	// ���½Ƕ�����
	bool GetAngles();
};

