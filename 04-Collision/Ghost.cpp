//#include "Ghost.h"
//
//void CGhost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
//{
//	if (state == GHOST_STATE_IDLE)
//		return;
//
//	if (IsRespawn())
//	{
//		SetState(GHOST_STATE_IDLE);
//		return;
//	}
//
//	CEnemy::Update(dt);
//
//	vector<LPCOLLISIONEVENT> coEvents;
//	vector<LPCOLLISIONEVENT> coEventsResult;
//
//	coEvents.clear();
//	CalcPotentialCollisions(coObjects, coEvents);
//	//
//	// TO-DO: make sure Goomba can interact with the world and to each of them too!
//	// 
//
//	if (nx == 1) vx = GHOST_FLYING_SPEED_X;
//	else if (nx == -1) vx = -GHOST_FLYING_SPEED_X;
//
//	if (coEvents.size() == 0)
//	{
//		x += dx;
//		y = (BAT_DROP / 2) * sin(x * BAT_FLYING_SPEED_Y) + drop;
//	}
//	else
//	{
//		x += dx;
//		y = (BAT_DROP / 2) * sin(x * BAT_FLYING_SPEED_Y) + drop;
//	}
//
//	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
//}
//
//void CGhost::Render()
//{
//}
//
//void CGhost::SetState(int state)
//{
//}
//
//void CGhost::GetBoundingBox(float& left, float& top, float& right, float& bottom)
//{
//}
//
//void CGhost::GetBoundingBoxActive(float& left, float& top, float& right, float& bottom)
//{
//}
//
//void CGhost::SetReturnPosition(int bX, int lX)
//{
//}
