#include "Crow.h"
#include "Simon.h"
#include "Game.h"
#include "PlayScene.h"
#include "debug.h"

void CCrow::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CEnemy::Update(dt);
	auto pScene = CPlayScene::GetInstance();

	D3DXVECTOR2 simonPos = { 0, 0 };
	int unTouchable = 0;
	for (size_t i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CSimon*>(coObjects->at(i)))
		{
			auto simon = dynamic_cast<CSimon*>(coObjects->at(i));
			float l, t, r, b;
			simon->GetBoundingBox(l, t, r, b);
			simonPos.x = l + (r - l) / 2;
			simonPos.y = t + (b - t) / 2;
			unTouchable = simon->GetUntouchable();
		}
	}
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (dynamic_cast<CSimon*>(obj)) // if e->obj is Item Heart 
		{
			CSimon* f = dynamic_cast<CSimon*>(obj);
			float sl, st, sr, sb;
			float ml, mt, mr, mb;
			f->GetBoundingBox(sl, st, sr, sb);
			this->GetBoundingBoxActive(ml, mt, mr, mb);
			if (CGame::AABB(ml, mt, mr, mb, sl, st, sr, sb) == true)
			{				
				this->SetState(CROW_STATE_FLYING);				
			}
		}
	}

	DebugOut(L"simonX=%f, simonY=%f, growX=%f, growY=%f \n", simonPos.x, simonPos.y, this->x, this->y);

	if (flyingWaiting_start != 0 && GetTickCount() - flyingWaiting_start > 300)
	{
		this->SetState(CROW_STATE_WAITING);
	}

	if (flying_start != 0 && GetTickCount() - flying_start > 300)
	{
		this->SetState(CROW_STATE_FLYING);
	}

	if (unTouchable != 1)
	{
		if (this->y - simonPos.y < 1 && this->GetState() == CROW_STATE_FLYING)
		{
			nx = 1;
			vx = -CROW_FLYING_SPEED_X;
			vy = CROW_FLYING_SPEED_Y;
			flying_start = GetTickCount();

		}
		else if (this->y - simonPos.y > 6 && this->GetState() == CROW_STATE_FLYING)
		{
			nx = -1;
			vx = CROW_FLYING_SPEED_X;
			vy = -CROW_FLYING_SPEED_Y;
			flying_start = GetTickCount();
		}
		else if (this->y - simonPos.y >= 1 && this->y - simonPos.y <= 6 && this->GetState() == CROW_STATE_FLYING)
		{
			if (this->x < simonPos.x)
			{
				nx = -1;
				vx = CROW_FLYING_SPEED_X;
				vy = 0;
			}
			else if (this->x > simonPos.x)
			{
				nx = 1;
				vx = -CROW_FLYING_SPEED_X;
				vy = 0;
			}
		}
	}
	else {
		if (flying_start == 0)
		{
			flying_start = GetTickCount();
		}

		if (flying_start != 0 && GetTickCount() - flying_start > 500)
		{
			int random = rand() % (2 + 1 - 1) + 1;
			nx = random == 1 ? 1 : -1;
			if (nx == 1 && this->GetState() == CROW_STATE_FLYING)
			{
				vx = -0.05;
				vy = CROW_FLYING_SPEED_X;
				flying_start = 0;
			}
			else if (nx == -1 && this->GetState() == CROW_STATE_FLYING)
			{
				vx = 0.05;
				vy = -CROW_FLYING_SPEED_X;
				flying_start = 0;
			}
		}
	}

	x += dx;
	y += dy;

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CCrow::Render()
{
	if (state == CROW_STATE_IDLE)
		animations["crow_ani_idle"]->Render(nx, x, y);
	if (state == CROW_STATE_WAITING)
		animations["crow_ani_idle"]->Render(nx, x, y);
	if (state == CROW_STATE_FLYING)
		animations["crow_ani_flying"]->Render(nx, x, y);

	//float l, t, r, b;
	//this->GetBoundingBoxActive(l, t, r, b);
	//RenderBoundingBox(RECT{ (long)l,(long)t,(long)r,(long)b });
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
	case CROW_STATE_WAITING:
		vx = 0;
		vy = 0;		
		break;
	case CROW_STATE_FLYING:
		reSpawnTimeStart = 0;
		isReSpawn = false;
		flyingWaiting_start = GetTickCount();
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
	if (this->state == CROW_STATE_FLYING || this->state == CROW_STATE_WAITING)
		left = top = right = bottom = 0;
	else
	{
		left = (x + bboxEnemyWidth / 2) - bboxEnemyActiveWidth;
		top = (y + bboxEnemyHeight / 2) - bboxEnemyActiveHeight;
		right = (x + bboxEnemyWidth / 2) + bboxEnemyActiveWidth;
		bottom = y + bboxEnemyActiveHeight;
	}
}
