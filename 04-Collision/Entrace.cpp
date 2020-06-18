#include "Entrace.h"

void CEntrace::Render()
{
	//RenderBoundingBox();
}

void CEntrace::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + ENTRACE_BBOX_WIDTH;
	b = y + ENTRACE_BBOX_HEIGHT;
}

void CEntrace::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
}
