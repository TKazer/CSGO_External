#include "Entity.h"
#include <math.h>

class CRender
{
public:
	// 绘制方框
	void ShowRect(const CEntity* Entity, const float ThickNess, int Color);
	// 绘制朝向
	void ShowLosLine(const CEntity* Entity, const float Length, int Color);
	// 3D圆
	void CirCle3D(const CEntity* Entity, const float Radius, int Color, const float ThickNess);
	// 绘制任意边形3D体
	void ShowCube(const CEntity* Entity, const int Sides,const float Range, const float ThickNess, int Color);
	// 魔法阵
	void ShowMagicCircle(const CEntity* Entity, const float Range, const float ThickNess);
	// 跑马灯颜色转换
	void LightChange(float* Color);
	// 渐变圈
	void CirCle3D_RainBow(const CEntity* Entity, const float Radius, const float ThickNess);
	// 骨骼绘制
	void DrawBone(CEntity* Entity, int Color);
}Render;

void CRender::DrawBone(CEntity* Entity, int Color)
{
	CBoneJoint Previous, Current;
	for (auto i : BoneJointName::List)
	{
		Previous.Pos = Vec3(0, 0, 0);
		for (auto Name : i)
		{
			Current = Entity->Bone[Name];
			if (Previous.Pos == 0)
			{
				Previous = Current;
				//pGame->View->Gui->Text(Current.Name.c_str(), Current.ScreenPos, ImColor(255, 255, 0, 255));
				continue;
			}
			if (Previous.InScreen() && Current.InScreen())
			{
				pGame->View->Gui->Line(Previous.ScreenPos, Current.ScreenPos, Color, 1);
				//pGame->View->Gui->Text(Current.Name.c_str(), Current.ScreenPos, ImColor(255, 255, 0, 255));
			}
			Previous = Current;
		}
	}
}

void CRender::CirCle3D(const CEntity* Entity, const float Radius, int Color, const float ThickNess)
{
	const int COUNT = 35;
	Vec3 Points[COUNT];
	Vec2 Previous, Current;
	float Angle = 0;
	for (int i = 0; i < COUNT+1; i++)
	{
		Current = Vec3(0, 0, 0);
		Angle = 180 * (1 + 2 * i) / COUNT;
		Points[i].x = Entity->Pos.x + cos(Angle * M_PI / 180) * Radius;
		Points[i].y = Entity->Pos.y + sin(Angle * M_PI / 180) * Radius;
		Points[i].z = Entity->Pos.z;
		if (!pGame->View->WorldToScreen(Points[i], Current))
		{
			Points[i] = Vec3(0, 0, 0);
			Current = Vec2(0, 0);
			Previous = Vec2(0, 0);
			continue;
		}
		if (i)
		{
			if (Previous != 0)
			{
				pGame->View->Gui->Line(Previous, Current, Color, ThickNess);
			}
		}
		Previous = Current;
	}
}

void CRender::ShowRect(const CEntity* Entity, const float ThickNess, int Color)
{
	pGame->View->Gui->Rectangle(Entity->Rect.point, Entity->Rect.width, Entity->Rect.height, Color, ThickNess);
}

void CRender::CirCle3D_RainBow(const CEntity* Entity, const float Radius, const float ThickNess)
{
	float flPoint = M_PI * 2.0f / 97;
	float R = 255.0f; float G = 0.0f; float B = 0.0f;
	int Counts = 0;

	for (float flAngle = 0; flAngle < (M_PI * 2.0f); flAngle += flPoint)
	{
		G += (Counts == 0 ? 15 : 0);
		if (Counts == 0 && G >= 255) Counts++;
		R -= (Counts == 1 ? 15 : 0);
		if (Counts == 1 && R <= 0)   Counts++;
		B += (Counts == 2 ? 15 : 0);
		if (Counts == 2 && B >= 255) Counts++;
		G -= (Counts == 3 ? 15 : 0);
		if (Counts == 3 && G <= 0)   Counts++;
		R += (Counts == 4 ? 15 : 0);
		if (Counts == 4 && R >= 255) Counts++;
		B -= (Counts == 5 ? 15 : 0);
		if (Counts == 5 && B <= 0)   Counts++;

		Vec3 Start(Radius * cosf(flAngle) + Entity->Pos.x, Radius * sinf(flAngle) + Entity->Pos.y, Entity->Pos.z);
		Vec3 End(Radius * cosf(flAngle + flPoint) + Entity->Pos.x, Radius * sinf(flAngle + flPoint) + Entity->Pos.y, Entity->Pos.z);
		Vec2 StartSc;
		Vec2 EndSc;

		if (pGame->View->WorldToScreen(Start, StartSc) && pGame->View->WorldToScreen(End, EndSc))
		{
			pGame->View->Gui->Line(StartSc, EndSc, IM_COL32(R, G, B, 255), ThickNess);//下层圆彩色
		}
	}

}

void CRender::ShowLosLine(const CEntity* Entity, const float Length, int Color)
{
	Vec2 StartPoint, EndPoint;
	Vec3 Temp;
	StartPoint = Entity->Head.ScreenPos;
	float LineLength = cos(Entity->AngleY * M_PI / 180) * Length;

	Temp.x = Entity->Head.Pos.x + cos(Entity->AngleX * M_PI / 180) * LineLength;
	Temp.y = Entity->Head.Pos.y + sin(Entity->AngleX * M_PI / 180) * LineLength;
	Temp.z = Entity->Head.Pos.z - sin(Entity->AngleY * M_PI / 180) * Length;
	pGame->View->WorldToScreen(Temp, EndPoint);
	if (EndPoint == 0)
		return;
	pGame->View->Gui->Line(StartPoint, EndPoint, Color, 1.5);
}

void CRender::ShowCube(const CEntity* Entity, const int Sides, const float Range, const float ThickNess, int Color)
{
	Vec3* Top = new Vec3[Sides + 1];
	Vec3* Bottom = new Vec3[Sides + 1];
	Vec2 TopPoint, BottomPoint;
	for (int i = 0; i < Sides + 1; i++)
	{
		float Rad =180 * (1.0f + 2.0f * i) / Sides;
		Vec2 TempTop, TempBottom;

		Top[i].x = Bottom[i].x = Entity->Pos.x + cos((Entity->AngleX + Rad) * M_PI / 180) * Range;
		Top[i].y = Bottom[i].y = Entity->Pos.y + sin((Entity->AngleX + Rad) * M_PI / 180) * Range;
		Bottom[i].z = Entity->Pos.z;
		Top[i].z = Entity->Head.Pos.z + 11;
		if (!pGame->View->WorldToScreen(Top[i], TempTop))
			break;
		if (!pGame->View->WorldToScreen(Bottom[i], TempBottom))
			break;
		if (TempTop == 0 || TempBottom == 0)
			break;
		pGame->View->Gui->Line(TempTop, TempBottom, Color, ThickNess);
		if (i)
		{
			pGame->View->Gui->Line(TopPoint, TempTop, Color, ThickNess);
			pGame->View->Gui->Line(BottomPoint, TempBottom, Color, ThickNess);
		}
		BottomPoint = TempBottom;
		TopPoint = TempTop;
	}
}

void CRender::ShowMagicCircle(const CEntity* Entity, const float Range, const float ThickNess)
{
	Vec3 Bottom[9];
	Vec2 BottomPoint;

	static float MagicCircleColor[4]{ 0,0,0,1 };
	static float MagicCircleAngle = 0;
	static DWORD MagicCircleTime = 0;

	this->LightChange(MagicCircleColor);
	for (int i = 0; i < 9; i++)
	{
		// 限速
		if (GetTickCount() - MagicCircleTime >= 8)
		{
			MagicCircleAngle++;
			MagicCircleTime = GetTickCount();
		}

		if (MagicCircleAngle == 360)
			MagicCircleAngle = 0;

		float Rad = (float)360/9*3.375*i;
		Vec2 TempBottom;

		Bottom[i].x = Entity->Pos.x + cos((MagicCircleAngle + Rad) * M_PI / 180) * Range;
		Bottom[i].y = Entity->Pos.y + sin((MagicCircleAngle + Rad) * M_PI / 180) * Range;
		Bottom[i].z = Entity->Pos.z;
		if (!pGame->View->WorldToScreen(Bottom[i], TempBottom))
			break;
		if (TempBottom == 0)
			break;
		if (i)
			pGame->View->Gui->Line(BottomPoint, TempBottom, Function::COLOR_RGBA(MagicCircleColor), ThickNess);
		BottomPoint = TempBottom;
	}
}

void CRender::LightChange(float* Color)
{
	float speed = 10;
	Color[0] = sin(speed / 1500 * clock()) * 0.5 + 0.5;
	Color[1] = sin(speed / 1500 * clock() + 2 * M_PI / 3) * 0.5 + 0.5;
	Color[2] = sin(speed / 1500 * clock() + 4 * M_PI / 3) * 0.5 + 0.5;
}