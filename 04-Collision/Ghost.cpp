#include "Ghost.h"
#include "Simon.h"

void CGhost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == GHOST_STATE_IDLE)
	{
		vx = 0;
		vy = 0;
	}

	if (IsRespawn())
	{
		//SetState(GHOST_STATE_IDLE);
		return;
	}

	CEnemy::Update(dt);

	D3DXVECTOR2 simonPos = { 0, 0 };
	for (size_t i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CSimon*>(coObjects->at(i)))
		{
			auto simon = dynamic_cast<CSimon*>(coObjects->at(i));
			float l, t, r, b;
			simon->GetBoundingBox(l, t, r, b);
			simonPos.x = l + (r - l) / 2;
			simonPos.y = t + (b - t) / 2;
		}
	}
	simonPos;
	if (this->x - int(simonPos.x) == 128)
		this->SetState(GHOST_STATE_FLYING);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	if (nx == 1) 
		vx = -GHOST_FLYING_SPEED_X;
	else if (nx == -1) 
		vx = GHOST_FLYING_SPEED_X;

	float l, t, r, b;
	this->GetBoundingBoxActive(l, t, r, b);

	if (vx > 0 && x < this->beginPositionX) {
		x = this->beginPositionX;
		vx = vx;
		nx = -nx;
	}

	if (vx < 0 && x > this->lastPositionX) {
		x = this->lastPositionX;
		vx = vx;
		nx = -nx;
	}

	x -= dx;
	
	y = 15 * sin(x * BAT_FLYING_SPEED_Y) + drop;

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGhost::Render()
{
	if (state == GHOST_STATE_FLYING)
		animations["ghost_ani_flying"]->Render(-nx, x, y);

	//float l, t, r, b;
	//this->GetBoundingBoxActive(l, t, r, b);
	//RenderBoundingBox(RECT{ (long)l,(long)t,(long)r,(long)b });
	//RenderBoundingBox();
}

void CGhost::SetState(int state)
{
	if (isDestroy)
	{
		return;
	}
	switch (state)
	{
	case GHOST_STATE_IDLE:
		vx = 0;
		vy = 0;
		isDestroy = false;
		isFinishReSpawn = false;
		this->bboxEnemyActiveWidth;
		this->bboxEnemyActiveHeight;
		StartRespawnTimeCounter();
		break;
	case GHOST_STATE_FLYING:
		reSpawnTimeStart = 0;
		isReSpawn = false;
		break;
	}
	CEnemy::SetState(state);
}

void CGhost::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isDestroy)
		return;

	if (this->state == GHOST_STATE_IDLE)
		left = top = right = bottom = 0;
	else
	{
		left = x;
		top = y;
		right = x + bboxEnemyWidth;
		bottom = y + bboxEnemyHeight;
	}
}

void CGhost::GetBoundingBoxActive(float& left, float& top, float& right, float& bottom)
{
	left = (x + bboxEnemyWidth / 2) - bboxEnemyActiveWidth;
	top = y;
	right = (x + bboxEnemyWidth / 2) + bboxEnemyActiveWidth;
	bottom = y + bboxEnemyHeight;
}
