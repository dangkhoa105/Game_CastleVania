#include "Bat.h"

void CBat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BAT_BBOX_WIDTH;
	bottom = y + BAT_BBOX_HEIGTH;
}

void CBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	if (nx == 1) vx = BAT_FLYING_SPEED_X;
	else if (nx == -1) vx = -BAT_FLYING_SPEED_X;

	if (coEvents.size() == 0)
	{
		x += dx;
		y = BAT_DROP * sin(x * BAT_FLYING_SPEED_Y) + drop;
	}
	else
	{
		x += dx;
		y = BAT_DROP * sin(x * BAT_FLYING_SPEED_Y) + drop;
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBat::Render()
{
	animations["bat_ani_flying"]->Render(nx, x, y);
}
