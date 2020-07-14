#include "SetLastSceneTrigger.h"

void CSetLastSceneTrigger::Render()
{
	if (this->isDestroy)
		return;
	//RenderBoundingBox();
}

void CSetLastSceneTrigger::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + SETLASTSCENETRIGGER_BBOX_WIDTH;
	b = y + SETLASTSCENETRIGGER_BBOX_HEIGHT;
}

void CSetLastSceneTrigger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}
