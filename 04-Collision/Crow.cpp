#include "Crow.h"
#include "Simon.h"

void CCrow::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == CROW_STATE_IDLE)
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
			int simonNx = this->nx;
		}
	}

	if (this->y - simonPos.y > 0)
	{
		 
	}

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
}

void CCrow::Render()
{
	if (state == CROW_STATE_IDLE)
		animations["crow_ani_idle"]->Render(nx, x, y);
	if (state == CROW_STATE_FLYING)
		animations["crow_ani_flying"]->Render(nx, x, y);
}

void CCrow::SetState(int state)
{
	CEnemy::SetState(state);
	switch (state)
	{
	case CROW_STATE_IDLE:
		vx = 0;
		vy = 0;
		isDestroy = false;
		isFinishReSpawn = false;
		this->bboxEnemyActiveWidth;
		this->bboxEnemyActiveHeight;
		StartRespawnTimeCounter();
		break;
	case CROW_STATE_FLYING:
		reSpawnTimeStart = 0;
		isReSpawn = false;
		break;
	default:
		break;
	}
}

void CCrow::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + bboxEnemyWidth;
	bottom = y + bboxEnemyHeight;
}

void CCrow::GetBoundingBoxActive(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + bboxEnemyActiveWidth;
	bottom = y + bboxEnemyActiveHeight;
}
