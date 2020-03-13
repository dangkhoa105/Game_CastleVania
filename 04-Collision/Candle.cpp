#include "Candle.h"

void CCandle::Render()
{
	animations["candle"]->Render(x, y);
	RenderBoundingBox();
}

void CCandle::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + CANDLE_BBOX_WIDTH;
	b = y + CANDLE_BBOX_HEIGHT;
}

void CCandle::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
}

void CCandle::SetState(int state)
{
	CGameObject::SetState(state);
}
