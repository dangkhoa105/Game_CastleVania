#include "Candle.h"

void CCandle::Render()
{
	if (isDestroy)
	{
		return;
	}
	if (state == CANDLE_STATE_BIG)
	{
		animations["big_candle"]->Render(x, y);
	}
	else if (state == CANDLE_STATE_SMALL)
	{
		animations["small_candle"]->Render(x, y);
	}
	
	//RenderBoundingBox();
}

void CCandle::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	switch (state)
	{
	case CANDLE_STATE_BIG:
		r = x + CANDLE_BBOX_WIDTH;
		b = y + CANDLE_BBOX_HEIGHT;
		break;
	case CANDLE_STATE_SMALL:
		r = x + SMALL_CANDLE_BBOX_WIDTH;
		b = y + SMALL_CANDLE_BBOX_HEIGHT;
		break;
	}
	
}

void CCandle::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
}

void CCandle::SetState(int state)
{
	CGameObject::SetState(state);
}
