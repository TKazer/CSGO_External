#pragma once
#include "BaseOffset.h"
#include "Functions.h"
#include <list>
#include <map>

class CBoneJoint
{
public:
	Vec3 Pos;
	Vec2 ScreenPos{ -1, -1 };
	int ID;
	std::string Name;
	// ��ȡ�����ڵ���Ļ����
	bool GetBoneJointScreenPos();
	// ��ȡ�����ڵ�����
	bool GetBoneJointPos(const DWORD& BoneMatrixAddress);
	// ��ȡ�����ڵ�����
	bool GetBoneJointName(const DWORD& BoneNameAddress);
	// �ж��Ƿ�����Ļ��
	bool InScreen();
	// ...
};

class CBone
{
public:
	CBone(){};
	CBone(DWORD EntityAddress_);
	// �������б�
	std::vector<CBoneJoint> BoneList;
	// ������ӳ��
	std::map<std::string, CBoneJoint> BoneMap;
	// �������й�������
	bool UpdateAllBoneData();
	// ���Ҷ�Ӧ�������ؽ�����
	CBoneJoint operator[](std::string BoneName);
	// �����ؽ�����
	int BoneCount = 0;
private:
	// ������ַ
	DWORD EntityAddress = 0;
	DWORD BoneMatrixAddress = 0;
	DWORD BoneNameAddress = 0;
	// ��ȡ��������
	int  GetBoneCount();
};

namespace BoneJointName
{
	// ����
	std::list<std::string> Trunk = { "head_0","neck_0","spine_2", "pelvis" };
	// ���
	std::list<std::string> LeftArm = { "neck_0",  "arm_upper_L", "arm_lower_L", "hand_L" };
	// �ұ�
	std::list<std::string> RightArm = { "neck_0", "arm_upper_R","arm_lower_R", "hand_R" };
	// ����	
	std::list<std::string> LeftLeg = { "pelvis", "leg_upper_L" , "leg_lower_L", "ankle_L" };
	// ����
	std::list<std::string> RightLeg = { "pelvis", "leg_upper_R" , "leg_lower_R", "ankle_R" };
	// ���б�
	std::vector<std::list<std::string>> List = { Trunk, LeftArm, RightArm, LeftLeg, RightLeg };
}