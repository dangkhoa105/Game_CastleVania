#include "Ghost.h"
#include "Simon.h"
#include "PlayScene.h"
#include "Game.h"

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
	auto simon = CPlayScene::GetInstance()->GetSimon();
	coObjects->push_back(simon);

	D3DXVECTOR2 simonPos = { 0, 0 };
	D3DXVECTOR2 ghostPos = { 0, 0 };
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
			if (CGame::GetInstance()->AABB(l, t, r, b, lg, tg, rg, bg))
				this->SetState(GHOST_STATE_FLYING);
		}
	}

	float l, t, r, b;
	this->GetBoundingBox(l, t, r, b);
	ghostPos.x = l + (r - l) / 2;
	ghostPos.y = t + (b - t) / 2;

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
	/*if (isDestroy)
	{
		return;
	}*/
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
		isReSpawnWaiting = false;
		break;
	}
	CEnemy::SetState(state);
}

void CGhost::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	/*if (isDestroy)
		return;*/

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
	left = x - bboxEnemyActiveWidth;
	top = y;
	right = x - bboxEnemyActiveWidth - bboxEnemyWidth;
	bottom = y + bboxEnemyActiveHeight;
}
