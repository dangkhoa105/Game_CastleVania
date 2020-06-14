#include "Effect.h"

void CEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (GetTickCount() - life_start > EFFECT_TIME)
	{
		this->SetDestroy(true);
	}
}

void CEffect::Render()
{
	if (state == EFFECT_STATE_CANDLE)
		animations["effect"]->Render(x, y);
	else if (state == EFFECT_STATE_ENEMIES)
		animations["effect_enemies_die"]->Render(x, y);
}

void CEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}
