#include "BreakWall.h"

void CBreakWall::Render()
{
	if (state == BREAKWALL_STATE_IDLE)
		animations["breakWall_idle"]->Render(x, y);
	/*else if (state == BREAKWALL_STATE_DESTROYED)
		animations["breakWall_destroyed"]->Render(x, y);*/
	//RenderBoundingBox();
}

void CBreakWall::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	switch (state)
	{
	case BREAKWALL_STATE_IDLE:
		l = x;
		t = y;
		r = x + BREAKWALL_BBOX_WIDTH;
		b = y + BREAKWALL_BBOX_HEIGHT;
		break;
	/*case BREAKWALL_STATE_DESTROYED:
		l = t = r = b = 0;
		break;*/
	default:
		break;
	}
	
}

void CBreakWall::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	/*if (isDestroy)
		SetState(BREAKWALL_STATE_DESTROYED);*/
}
