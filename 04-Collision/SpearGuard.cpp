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
	right = x + bboxEnemyWidth;
	bottom = y + bboxEnemyHeight;
}

void CSpearGuard::GetBoundingBoxActive(float& left, float& top, float& right, float& bottom)
{
	left = (x + bboxEnemyWidth / 2) - bboxEnemyActiveWidth;
	top = y;
	right = (x + bboxEnemyWidth / 2) + bboxEnemyActiveWidth;
	bottom = y + bboxEnemyHeight;
}

void CSpearGuard::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CEnemy::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	if (nx == 1) vx = SPEAR_GUARD_WALKING_SPEED;
	else if (nx == -1) vx = -SPEAR_GUARD_WALKING_SPEED;
	
	float l, t, r, b;
	this->GetBoundingBoxActive(l, t, r, b);

	if (vx < 0 && x < this->beginPositionX) {
		x = beginPositionX;
		vx = -vx;
		nx = 1;
	}

	if (vx > 0 && x > lastPositionX) {
		x = lastPositionX;
		vx = vx;
		nx = -1;
	}

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		x += dx;
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CSpearGuard::Render()
{
	if (isDestroy)
	{
		return;
	}
	if (state == SPEAR_GUARD_STATE_WALKING)
		animations["spear_guard_ani"]->Render(-nx, x, y);

	/*float l, t, r, b;
	this->GetBoundingBoxActive(l, t, r, b);*/

	//RenderBoundingBox(RECT{(long)l,(long)t,(long)r,(long)b});
	RenderBoundingBox();
}

void CSpearGuard::SetState(int state)
{
	CEnemy::SetState(state);
}
