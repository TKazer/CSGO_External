#include "Entity.h"

bool CEntity::UpDate()
{
	if (this->EntityAddress == 0)
		return false;
	if (this->Bone.BoneCount == 0)
		this->Bone = CBone(this->EntityAddress);
	if (!this->IsActive())
		return false;
	if (!this->GetPos())		// 更新坐标
		return false;			
	if (!this->GetHealth())		// 更新血量
		return false;
	if (!this->GetCamp())		// 更新阵营
		return false;			
	if (!this->GetAngles())		// 更新角度
		return false;		
	if (!this->Bone.UpdateAllBoneData())
		return false;
	this->Head = this->Bone["head_0"];
	return true;
}

bool CEntity::UpDataScreenPos()
{
	if (!this->Head.InScreen())	
		return false;

	this->Height = Head.Pos.z - this->Pos.z + 5;
	if (!pGame->View->WorldToScreen(this->Pos, this->ScreenPos, this->Height))// 更新屏幕坐标
		return false;

	// 方框数据计算
	this->Rect.height = this->ScreenPos.y - this->ScreenPos.z;
	this->Rect.width = this->Rect.height * 0.53;
	this->Rect.point.x = this->ScreenPos.x - this->Rect.width / 2;
	this->Rect.point.y = this->ScreenPos.z;
	return true;
}

bool CEntity::IsActive()
{
	if (this->EntityAddress <= 0)
		return false;
	int TempNum;
	TempNum = Function::Read<int>(this->EntityAddress + Base::EntityData.Active);
	if (TempNum != 0)
		return false;
	return true;
}

bool CEntity::GetPos()
{
	if (this->EntityAddress <= 0)
		return false;
	this->Pos.x = Function::Read<float>(this->EntityAddress + Base::EntityData.Pos);
	this->Pos.y = Function::Read<float>(this->EntityAddress + Base::EntityData.Pos + 0x4);
	this->Pos.z = Function::Read<float>(this->EntityAddress + Base::EntityData.Pos + 0x8);
	return true;
}

bool CEntity::GetHealth()
{
	if (this->EntityAddress <= 0)
		return false;
	this->Health = Function::Read<int>(this->EntityAddress + Base::EntityData.Health);
	return this->Health > 0;
}

bool CEntity::GetCamp()
{
	if (this->EntityAddress <= 0)
		return false;
	this->Camp = Function::Read<int>(this->EntityAddress + Base::EntityData.Camp);
	return true;
}

bool CEntity::GetAngles()
{
	if (this->EntityAddress <= 0)
		return false;
	this->AngleX= Function::Read<float>(this->EntityAddress + Base::EntityData.AngleX);
	this->AngleY = Function::Read<float>(this->EntityAddress + Base::EntityData.AngleY);
	return true;
}
