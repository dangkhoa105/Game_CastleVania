#include "Candle.h"

void CCandle::Render()
{
	if (state == CANDLE_STATE_BIG)
	{
		animations["candle"]->Render(x, y);
	}
	else
	{	
		animations["effect"]->Render(x, y);	
		this->destroyed = animations["effect"]->GetOver();
	}
	
	//RenderBoundingBox();
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
	if (this->destroyed)
	{
		//destroyed = true;
		this->SetDie(true);
	}
}

void CCandle::SetState(int state)
{
	CGameObject::SetState(state);
	/*switch (state)
	{
	case CANDLE_STATE_BIG:
	case CANDLE_STATE_SMALL:	
	case CANDLE_STATE_DESTROYED:
		break;
	default:
		break;
	}*/
}
