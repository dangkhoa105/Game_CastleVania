#include "MoneyBagTrigger.h"

void CMoneyBagTrigger::Render()
{
	if (isDestroy)
	{
		return;
	}
	//RenderBoundingBox();
}

void CMoneyBagTrigger::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + MONEYBAG_BBOX_WIDTH;
	b = y + MONEYBAG_BBOX_HEIGHT;
}

void CMoneyBagTrigger::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
}
