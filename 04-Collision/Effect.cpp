#include "Effect.h"

void CEffect::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	if (isDestroy)
		return;

	if (life_start == 0)
		life_start = GetTickCount();

	if (GetTickCount() - life_start > EFFECT_TIME)
	{
		this->SetDestroy(true);
		life_start = 0;
	}

	switch (state)
	{
	case EFFECT_STATE_BREAKWALL:
		vy += BREAKWALL_BREAKING_GRAVITY * dt;
		x += dx;
		y += dy;
		break;
	case EFFECT_STATE_BREAKWALL_DESTROYED:
		this->SetDestroy(false);
		break;
	default:
		break;
	}
}

void CEffect::Render()
{
	if (state == EFFECT_STATE_CANDLE)
		animations["effect"]->Render(x, y);
	else if (state == EFFECT_STATE_ENEMIES)
		animations["effect_enemies_die"]->Render(x, y);
	else if (state == EFFECT_STATE_BREAKWALL)
		animations["breakWall_breaking"]->Render(x, y);
	else if (state == EFFECT_STATE_BREAKWALL_DESTROYED)
		animations["breakWall_destroyed"]->Render(x, y);
	else if (state == EFFECT_STATE_MONEYBAG_SPECIAL)
		animations["effect_moneybag_special"]->Render(x, y);
	else if (state == EFFECT_STATE_MONEYBAG_400)
		animations["effect_moneybag_400"]->Render(x, y);
	else if (state == EFFECT_STATE_MONEYBAG_700)
		animations["effect_moneybag_700"]->Render(x, y);
	else if (state == EFFECT_STATE_MONEYBAG_1000)
		animations["effect_moneybag_1000"]->Render(x, y);
	else if (state == EFFECT_STATE_2000)
		animations["effect_2000"]->Render(x, y);

	//RenderBoundingBox();
}

void CEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}
