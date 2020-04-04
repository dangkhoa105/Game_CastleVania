#include "Heart.h"

void Heart::Render()
{
	if (fall) {
		animations["item_heart"]->Render(x, y);
	}
}

void Heart::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + HEART_BBOX_WIDTH;
	b = y + HEART_BBOX_HEIGHT;
}

void Heart::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt, colliable_objects);
	if (fall) {
		y += 0.1 * dt;
		if (y > 320 - 32 - HEART_BBOX_WIDTH) {
			vy = 0;
			y = 320 - 32 - HEART_BBOX_HEIGHT;
		}
	}
}

void Heart::SetState(int state)
{
	CGameObject::SetState(state);
}
