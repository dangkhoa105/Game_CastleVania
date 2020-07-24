#include "Ghost.h"
#include "Simon.h"
#include "PlayScene.h"
#include "Game.h"

void CGhost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (freezeEnemy)
	{
		return;
	}
	if (state == GHOST_STATE_IDLE)
	{
		vx = 0;
		vy = 0;
	}

	CEnemy::Update(dt);
	auto simon = CPlayScene::GetInstance()->GetSimon();
	coObjects->push_back(simon);

	

	if (IsRespawn())
	{		
		if (count_start == 0)
			count_start = GetTickCount();
		if (count_start != 0 && GetTickCount() - count_start > reSpawnWaitingTime)
		{
			SetState(GHOST_STATE_IDLE);
			count_start = 0;
		}
		return;
	}

	D3DXVECTOR2 simonPos = { 0, 0 };
	D3DXVECTOR2 ghostPos = { 0, 0 };

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	

	for (size_t i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CSimon*>(coObjects->at(i)))
		{
			auto simon = dynamic_cast<CSimon*>(coObjects->at(i));
			float l, t, r, b;
			float lg, tg, rg, bg;
			simon->GetBoundingBox(l, t, r, b);
			simonPos.x = l + (r - l) / 2;
			simonPos.y = t + (b - t) / 2;
			int simonNx = this->nx;
			this->GetBoundingBoxActive(lg, tg, rg, bg);

			if (CGame::GetInstance()->AABB(lg, tg, rg, bg, l, t, r, b))
				this->SetState(GHOST_STATE_FLYING);
		}
	}

	float l, t, r, b;
	this->GetBoundingBox(l, t, r, b);
	ghostPos.x = l + (r - l) / 2;
	ghostPos.y = t + (b - t) / 2;

	if (!isDestroy)
	{
		coEvents.clear();
		CalcPotentialCollisions(coObjects, coEvents);

		if (simonPos.x > ghostPos.x + 32)
		{
			nx = 1;
			vx = GHOST_FLYING_SPEED_X;
		}
		else if (simonPos.x < ghostPos.x - 32)
		{
			nx = -1;
			vx = -GHOST_FLYING_SPEED_X;
		}

		x += dx;

		y = GHOST_DROP * sin(x * BAT_FLYING_SPEED_Y) + this->GetEntryPositionY();

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}	
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
	switch (state)
	{
	case GHOST_STATE_IDLE:	
		vx = vy = 0;
		x = initPositionX;
		y = initPositionY;
		isDestroy = false;
		isFinishReSpawn = false;
		StartRespawnTimeCounter();
		break;
	case GHOST_STATE_FLYING:	
		reSpawnTimeStart = 0;
		isReSpawnWaiting = false;
		break;
	default:
		break;
	}
	CEnemy::SetState(state);
}

void CGhost::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + bboxEnemyWidth;
	bottom = y + bboxEnemyHeight;
}

void CGhost::GetBoundingBoxActive(float& left, float& top, float& right, float& bottom)
{
	left = x - bboxEnemyActiveWidth;
	top = y;
	right = x - bboxEnemyActiveWidth - bboxEnemyWidth;
	bottom = y + bboxEnemyActiveHeight;
}
