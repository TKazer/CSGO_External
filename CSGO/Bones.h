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
	// 获取骨骼节点屏幕坐标
	bool GetBoneJointScreenPos();
	// 获取骨骼节点坐标
	bool GetBoneJointPos(const DWORD& BoneMatrixAddress);
	// 获取骨骼节点名字
	bool GetBoneJointName(const DWORD& BoneNameAddress);
	// 判断是否在屏幕内
	bool InScreen();
	// ...
};

class CBone
{
public:
	CBone(){};
	CBone(DWORD EntityAddress_);
	// 骨骼点列表
	std::vector<CBoneJoint> BoneList;
	// 骨骼点映射
	std::map<std::string, CBoneJoint> BoneMap;
	// 更新所有骨骼数据
	bool UpdateAllBoneData();
	// 查找对应骨骼名关节数据
	CBoneJoint operator[](std::string BoneName);
	// 骨骼关节数量
	int BoneCount = 0;
private:
	// 骨骼地址
	DWORD EntityAddress = 0;
	DWORD BoneMatrixAddress = 0;
	DWORD BoneNameAddress = 0;
	// 获取骨骼数量
	int  GetBoneCount();
};

namespace BoneJointName
{
	// 脊骨
	std::list<std::string> Trunk = { "head_0","neck_0","spine_2", "pelvis" };
	// 左臂
	std::list<std::string> LeftArm = { "neck_0",  "arm_upper_L", "arm_lower_L", "hand_L" };
	// 右臂
	std::list<std::string> RightArm = { "neck_0", "arm_upper_R","arm_lower_R", "hand_R" };
	// 左腿	
	std::list<std::string> LeftLeg = { "pelvis", "leg_upper_L" , "leg_lower_L", "ankle_L" };
	// 右腿
	std::list<std::string> RightLeg = { "pelvis", "leg_upper_R" , "leg_lower_R", "ankle_R" };
	// 总列表
	std::vector<std::list<std::string>> List = { Trunk, LeftArm, RightArm, LeftLeg, RightLeg };
}