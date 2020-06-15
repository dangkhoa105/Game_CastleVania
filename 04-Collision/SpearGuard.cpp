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

void CSpearGuard::GetBoundingBoxActive(float& left, float& top, float& right, float& bottom)
{
	left = (x+ SPEAR_GUARD_BBOX_WIDTH/2)-100;
	top = y;
	right = (x + SPEAR_GUARD_BBOX_WIDTH / 2)+100;
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

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CSpearGuard::Render()
{
	if (isDestroy)
	{
		return;
	}
	this->state;
	if (state == SPEAR_GUARD_STATE_WALKING)
		animations["spear_guard_ani"]->Render(-nx, x, y);

	float l, t, r, b;
	this->GetBoundingBoxActive(l, t, r, b);

	RenderBoundingBox(RECT{(long)l,(long)t,(long)r,(long)b});
}

void CSpearGuard::SetState(int state)
{
	CGameObject::SetState(state);
	switch (this->state)
	{

	default:
		break;
	}
}

void CSpearGuard::SetReturnPosition(int bX, int lX)
{
	this->beginPositionX = bX;
	this->lastPositionX = lX;
}
