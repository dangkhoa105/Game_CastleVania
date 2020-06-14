#include "Bat.h"
#include "debug.h"

void CBat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + bboxEnemyWidth;
	bottom = y + bboxEnemyHeight;
	//RenderBoundingBox();
}

void CBat::GetBoundingBoxActive(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + bboxEnemyActiveWidth;
	bottom = y + bboxEnemyActiveHeight;
	//RenderBoundingBox();
}

void CBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BAT_STATE_IDLE)
		return;

	if (IsRespawn())
	{
		SetState(BAT_STATE_IDLE);
		return;
	}

	CEnemy::Update(dt);

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
	if (state == BAT_STATE_IDLE)
		animations["bat_ani_idle"]->Render(nx, x, y);
	if (state == BAT_STATE_FLYING)
		animations["bat_ani_flying"]->Render(nx, x, y);
}

void CBat::SetState(int state)
{
	CEnemy::SetState(state);
	switch (state)
	{
	case BAT_STATE_IDLE:
		vx = 0;
		vy = 0;
		isDestroy = false;
		isFinishReSpawn = false;
		this->bboxEnemyActiveWidth;
		this->bboxEnemyActiveHeight;
		StartRespawnTimeCounter();	
		break;
	case BAT_STATE_FLYING:
		if (nx == 1) vx = BAT_FLYING_SPEED_X;
		else if (nx == -1) vx = -BAT_FLYING_SPEED_X;
		reSpawnTimeStart = 0;
		isReSpawn = false;
		break;
	}
}
