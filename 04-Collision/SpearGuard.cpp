#include "SpearGuard.h"
#include <algorithm>
#include "debug.h"
#include "ReadResourceFile.h"
#include "Simon.h"
#include "Game.h"

void CSpearGuard::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SPEAR_GUARD_BBOX_WIDTH;
	bottom = y + SPEAR_GUARD_BBOX_HEIGTH;
}

void CSpearGuard::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	if (nx == 1) vx = SPEAR_GUARD_WALKING_SPEED;
	else if (nx == -1) vx = -SPEAR_GUARD_WALKING_SPEED;

	if (vx < 0 && x < 65) {
		x = 64; 
		vx = -vx;
		nx = 1;
	}

	if (vx > 0 && x > 284) {
		x = 284;
		vx = vx;
		nx = -1;
	}

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CSpearGuard::Render()
{
	string ani = "spear_guard_ani";

	animations[ani]->Render(-nx, x, y);
	//RenderBoundingBox();
}

//void CSpearGuard::SetState(int state)
//{
//	CGameObject::SetState(state);
//	switch (state)
//	{
//	case SPEAR_GUARD_STATE_WALKING:
//		if (nx == 1) vx = SPEAR_GUARD_WALKING_SPEED;
//		else if (nx == -1) vx = -SPEAR_GUARD_WALKING_SPEED;
//	}
//}