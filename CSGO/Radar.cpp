#include "Radar.h"

void Base_Radar::SetRange(const float& Range)
{
	this->RenderRange = Range;
}

void Base_Radar::SetCrossColor(const ImColor& Color)
{
	this->CrossColor = Color;
}

void Base_Radar::SetPos(const Vec2& Pos)
{
	this->Pos = Pos;
}

void Base_Radar::SetSize(const float& Size)
{
	this->Width = Size;
}

float Base_Radar::GetSize()
{
	return this->Width;
}

Vec2 Base_Radar::GetPos()
{
	return this->Pos;
}

void Base_Radar::SetProportion(const float& Proportion)
{
	this->Proportion = Proportion;
}

void Base_Radar::AddPoint(const Vec3& LocalPos, const float& LocalYaw, const Vec3& Pos, ImColor Color, int Type, float Yaw)
{
	Vec2 PointPos;
	float Distance;
	float Angle;

	this->LocalYaw = LocalYaw;

	Distance = sqrt(pow(LocalPos.x - Pos.x, 2) + pow(LocalPos.y - Pos.y, 2));

	Angle = atan2(Pos.y - LocalPos.y, Pos.x - LocalPos.x) * 180 / M_PI;
	Angle = (this->LocalYaw - Angle) * M_PI / 180;

	Distance = Distance / this->Proportion * this->RenderRange * 2;

	PointPos.x = this->Pos.x + Distance * sin(Angle);
	PointPos.y = this->Pos.y - Distance * cos(Angle);

	Distance = sqrt(pow(this->Pos.x - PointPos.x, 2) + pow(this->Pos.y - PointPos.y, 2));
	if (Distance > this->RenderRange)
		return;

	std::tuple<Vec2, ImColor, int, float> Data(PointPos, Color, Type, Yaw);
	this->Points.push_back(Data);
}

void DrawTriangle(Vec2 Center, ImColor Color, float Width, float Height, float Yaw)
{
	Vec2 a, b, c;
	Vec2 Re_a, Re_b, Re_c;
	a = { Center.x - Width / 2,Center.y };
	b = { Center.x + Width / 2,Center.y };
	c = { Center.x,Center.y - Height };
	a = Function::RevolveCoordinatesSystem(-Yaw, Center, a);
	b = Function::RevolveCoordinatesSystem(-Yaw, Center, b);
	c = Function::RevolveCoordinatesSystem(-Yaw, Center, c);
	ImGui::GetForegroundDrawList()->AddTriangleFilled(
		ImVec2(a.x, a.y),
		ImVec2(b.x, b.y),
		ImVec2(c.x, c.y),
		Color);
}

void Base_Radar::Render()
{
	if (Width <= 0)
		return;

	// Cross
	std::pair<Vec2, Vec2> Line1;
	std::pair<Vec2, Vec2> Line2;

	Line1.first = Vec2(this->Pos.x - this->Width / 2, this->Pos.y);
	Line1.second = Vec2(this->Pos.x + this->Width / 2, this->Pos.y);
	Line2.first = Vec2(this->Pos.x, this->Pos.y - this->Width / 2);
	Line2.second = Vec2(this->Pos.x, this->Pos.y + this->Width / 2);

	if (this->Opened)
	{
		pGame->View->Gui->Line(Line1.first, Line1.second, this->CrossColor, 1);
		pGame->View->Gui->Line(Line2.first, Line2.second, this->CrossColor, 1);
		for (auto PointSingle : this->Points)
		{
			Vec2	PointPos = std::get<0>(PointSingle);
			ImColor PointColor = std::get<1>(PointSingle);
			int		PointType = std::get<2>(PointSingle);
			float	PointYaw = std::get<3>(PointSingle);
			if (PointType == 0)
			{
				// 圆形样式
				pGame->View->Gui->CircleFilled(PointPos, this->PointRadius, PointColor);
				pGame->View->Gui->Circle(PointPos, this->PointRadius, ImColor(0, 0, 0), 1);
			}	
			else if (PointType==1)
			{
				// 箭头样式
				Vec2 a, b, c;
				Vec2 Re_a, Re_b, Re_c;
				Vec2 Re_Point;
				float Angle = (this->LocalYaw - PointYaw) + 180;
				Re_Point = Function::RevolveCoordinatesSystem(Angle, this->Pos, PointPos);

				Re_a = Vec2(Re_Point.x, Re_Point.y + this->ArrowSize);
				Re_b = Vec2(Re_Point.x - this->ArrowSize / 1.5, Re_Point.y - this->ArrowSize / 2);
				Re_c = Vec2(Re_Point.x + this->ArrowSize / 1.5, Re_Point.y - this->ArrowSize / 2);

				a = Function::RevolveCoordinatesSystem(-Angle, this->Pos, Re_a);
				b = Function::RevolveCoordinatesSystem(-Angle, this->Pos, Re_b);
				c = Function::RevolveCoordinatesSystem(-Angle, this->Pos, Re_c);

				ImGui::GetForegroundDrawList()->AddQuadFilled(
					ImVec2(a.x, a.y),
					ImVec2(b.x, b.y),
					ImVec2(PointPos.x, PointPos.y),
					ImVec2(c.x, c.y),
					PointColor
				);
				ImGui::GetForegroundDrawList()->AddQuad(
					ImVec2(a.x, a.y),
					ImVec2(b.x, b.y),
					ImVec2(PointPos.x, PointPos.y),
					ImVec2(c.x, c.y),
					ImColor(0, 0, 0, 150),
					0.1
				);
			}
			else
			{
				// 圆弧箭头
				Vec2 TrianglePoint;
				float Angle = (this->LocalYaw - PointYaw) - 90;
				pGame->View->Gui->Arc(PointPos, this->ArcArrowSize, ImColor(220, 220, 220, 170), -Angle, 0.4, 0.05);
				pGame->View->Gui->Arc(PointPos, this->ArcArrowSize, ImColor(220, 220, 220, 200), -Angle, 0.25, 1.5);
				pGame->View->Gui->CircleFilled(PointPos, 0.85 * this->ArcArrowSize, PointColor, 30);
				pGame->View->Gui->Circle(PointPos, 0.95 * this->ArcArrowSize, ImColor(0, 0, 0, 150), 0.1);
				TrianglePoint.x = PointPos.x + this->ArcArrowSize * cos(-Angle * M_PI / 180);
				TrianglePoint.y = PointPos.y - this->ArcArrowSize * sin(-Angle * M_PI / 180);
				DrawTriangle(TrianglePoint, ImColor(255, 255, 255, 220), 0.7 * this->ArcArrowSize, 0.5 * this->ArcArrowSize, Angle + 90);
			}
		}
	}
	if (this->Points.size() > 0)
		this->Points.clear();
}