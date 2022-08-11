#include "GameView.h"

bool CView::WorldToScreen(const Vec3& Pos, Vec2& ToPos)
{
	float View, SightX = this->Gui->Window.Width / 2, SightY = this->Gui->Window.Height / 2;
	if (&Pos == 0)
		return false;
	View = Matrix[3][0] * Pos.x + Matrix[3][1] * Pos.y + Matrix[3][2] * Pos.z + Matrix[3][3];
	if (View <= 0.01)
		return false;
	View = 1 / View;
	ToPos.x = SightX + (Matrix[0][0] * Pos.x + Matrix[0][1] * Pos.y + Matrix[0][2] * Pos.z + Matrix[0][3]) * View * SightX;
	ToPos.y = SightY - (Matrix[1][0] * Pos.x + Matrix[1][1] * Pos.y + Matrix[1][2] * Pos.z + Matrix[1][3]) * View * SightY;
	return true;
}

bool CView::WorldToScreen(const Vec3& Pos, Vec3& ToPos, float Height)
{
	float View, SightX = this->Gui->Window.Width / 2, SightY = this->Gui->Window.Height / 2;
	if (&Pos == 0)
		return false;
	View = Matrix[3][0] * Pos.x + Matrix[3][1] * Pos.y + Matrix[3][2] * Pos.z + Matrix[3][3];
	if (View <= 0.01)
		return false;
	View = 1 / View;
	ToPos.x = SightX + (Matrix[0][0] * Pos.x + Matrix[0][1] * Pos.y + Matrix[0][2] * Pos.z + Matrix[0][3]) * View * SightX;
	ToPos.y = SightY - (Matrix[1][0] * Pos.x + Matrix[1][1] * Pos.y + Matrix[1][2] * Pos.z + Matrix[1][3]) * View * SightY;
	ToPos.z = SightY - (Matrix[1][0] * Pos.x + Matrix[1][1] * Pos.y + Matrix[1][2] * (Pos.z + Height) + Matrix[1][3]) * View * SightY;
	return true;
}