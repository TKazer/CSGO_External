#include "Bones.h"

CBone::CBone(DWORD EntityAddress_)
{
	// 初始骨骼地址数据
	this->EntityAddress = EntityAddress_;
	this->BoneMatrixAddress = Function::Read<DWORD>(this->EntityAddress + Base::EntityData.BoneMatrix);
	DWORD Temp = Function::Read<DWORD>(EntityAddress + Base::EntityData.BoneName);
	this->BoneNameAddress = Function::Read<DWORD>(Temp);
	this->BoneCount = this->GetBoneCount();

	if (this->BoneCount > 100 || this->BoneCount < 0)
	{
		this->BoneCount = 0;
	}
	this->BoneList.resize(this->BoneCount);

	// 初始化骨骼列表ID
	for (int i = 0; i < this->BoneCount; i++)
		this->BoneList[i].ID = i;
}

CBoneJoint CBone::operator[](std::string BoneName)
{
	return this->BoneMap[BoneName];
}

bool CBone::UpdateAllBoneData()
{
	if (this->EntityAddress == 0 || this->BoneMatrixAddress == 0 || this->BoneNameAddress == 0)
		return false;
	if (this->BoneCount <= 0)
		return false;

	for (int i = 0; i < this->BoneCount; i++)
	{
		if (!this->BoneList[i].GetBoneJointName(this->BoneNameAddress))
			return false;
		if (!this->BoneList[i].GetBoneJointPos(this->BoneMatrixAddress))
			return false;
		this->BoneList[i].GetBoneJointScreenPos();
		this->BoneMap.insert({ this->BoneList[i].Name, this->BoneList[i] });
	}

	return true;
}

int CBone::GetBoneCount()
{
	DWORD Count = Function::Read<DWORD>(this->BoneNameAddress + 0x9C) - 1;
	return Count;
}

bool CBoneJoint::InScreen()
{
	return this->ScreenPos != -1;
}

bool CBoneJoint::GetBoneJointScreenPos()
{
	return pGame->View->WorldToScreen(this->Pos, this->ScreenPos);
}

bool CBoneJoint::GetBoneJointPos(const DWORD& BoneMatrixAddress)
{
	DWORD Address;
	Address = BoneMatrixAddress;
	if (Address <= 0)
		return false;
	Address += 0xC + 0x30 * this->ID;
	this->Pos.x = Function::Read<float>(Address);
	this->Pos.y = Function::Read<float>(Address + 0x10);
	this->Pos.z = Function::Read<float>(Address + 0x20);
	return true;
}

bool CBoneJoint::GetBoneJointName(const DWORD& BoneNameAddress)
{
	DWORD eax = BoneNameAddress;
	DWORD edx = Function::Read<DWORD>(eax + 0xA0);
	eax += edx + this->ID * 0xD8;
	edx = Function::Read<DWORD>(eax);
	char BoneName[32]{};
	if (Function::ReadMemory(eax + edx, &BoneName, 32))
		this->Name = BoneName;
	else
		return false;
	return true;
}