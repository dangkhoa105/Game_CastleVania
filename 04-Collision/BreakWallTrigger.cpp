#include "BreakWallTrigger.h"

void CBreakWallTrigger::Render()
{
	if (isDestroy)
	{
		return;
	}
	RenderBoundingBox();
}

void CBreakWallTrigger::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BREAKWALLTRIGGER_BBOX_WIDTH;
	b = y + BREAKWALLTRIGGER_BBOX_HEIGHT;
}

void CBreakWallTrigger::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
}
