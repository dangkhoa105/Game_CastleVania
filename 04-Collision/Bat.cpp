#include "Bat.h"
#include "debug.h"
#include"Simon.h"
#include "PlayScene.h"
void CBat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + bboxEnemyWidth;
	bottom = y + bboxEnemyHeight;
}

void CBat::GetBoundingBoxActive(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + bboxEnemyActiveWidth;
	bottom = y + bboxEnemyActiveHeight;
}

void CBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CEnemy::Update(dt);

	//if (state == BAT_STATE_IDLE)
	//	return;

	if (IsRespawn())
	{
		if (count_start == 0)
			count_start = GetTickCount();
		if (count_start != 0 && GetTickCount() - count_start > reSpawnWaitingTime)
		{
			attack_start = 0;
			SetState(BAT_STATE_IDLE);		
			count_start = 0;
		}
		return;
	}

	auto simon = CPlayScene::GetInstance()->GetSimon();
	coObjects->push_back(simon);

	
	for (size_t i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CSimon*>(coObjects->at(i)))
		{
			auto simon= dynamic_cast<CSimon*>(coObjects->at(i));
			float l, t, r, b;
			simon->GetBoundingBox(l, t, r, b);
			simonPos.x = l + (r - l) / 2;
			simonPos.y = t + (b - t) / 2;
		}
	}

	//attack_start = 0;
	
	if (state == BAT_STATE_FLYING && attack_start == 0)
	{
		 time_x = abs(x + bboxEnemyWidth - simonPos.x) / (BAT_FLYING_SPEED_X / 2);
 		 if (x > simonPos.x)
		 {
			 vx = -(BAT_FLYING_SPEED_X / 2);
		 }
		 else
		 {
			 vx = (BAT_FLYING_SPEED_X / 2);
		 }

		 vy = abs(y + bboxEnemyHeight - simonPos.y) / time_x;
		 attack_start = GetTickCount();
	}

	if (state == BAT_STATE_FLYING && attack_start != 0 && GetTickCount() - attack_start > time_x)
	{
		this->vx = this->vx;
		this->vy = 0;
		dy = this->vy * dt;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;
	
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBat::Render()
{
	if (state == BAT_STATE_IDLE)
		animations["bat_ani_idle"]->Render(nx, x, y);
	if (state == BAT_STATE_FLYING)
		animations["bat_ani_flying"]->Render(-nx, x, y);

	//float l, t, r, b;
	//this->GetBoundingBoxActive(l, t, r, b);
	//RenderBoundingBox(RECT{ (long)l,(long)t,(long)r,(long)b });
}

void CBat::SetState(int state)
{
	switch (state)
	{
	case BAT_STATE_IDLE:		
		vx = vy = 0;
		x = initPositionX;
		y = initPositionY;
		isDestroy = false;
		isFinishReSpawn = false;
		StartRespawnTimeCounter();	
		break;
	case BAT_STATE_FLYING:
		reSpawnTimeStart = 0;
		isReSpawnWaiting = false;
		break;
	}
	CEnemy::SetState(state);
}
