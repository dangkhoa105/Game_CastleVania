#include "Boss.h"
#include "Simon.h"
#include "Game.h"

void CBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->isDestroy)
	{
		return;
	}
	CEnemy::Update(dt);

	float acl, act, acr, acb;
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (dynamic_cast<CSimon*>(obj)) // if e->obj is Item Heart 
		{
			CSimon* f = dynamic_cast<CSimon*>(obj);
			float sl, st, sr, sb;
			float awl, awt, awr, awb;		
			f->GetBoundingBox(sl, st, sr, sb);
			this->GetBoundingBoxAwake(awl, awt, awr, awb);
			this->GetBoundingBoxActive(acl, act, acr, acb);
			if (CGame::AABB(awl, awt, awr, awb, sl, st, sr, sb) == true)
			{
				this->SetState(BOSS_STATE_FLYING);
				if (attack_time == 0)
				{
					attack_time = GetTickCount();
				}
			}
		}
	}

	//if (this->x )
}

void CBoss::Render()
{
	if (state == BOSS_STATE_IDLE)
		animations["boss_ani_idle"]->Render(-nx, x, y);
	if (state == BOSS_STATE_FLYING || state == BOSS_STATE_WAITING)
		animations["boss_ani_flying"]->Render(-nx, x, y);

	float l, t, r, b;
	this->GetBoundingBoxAwake(l, t, r, b);
	RenderBoundingBox(RECT{ (long)l,(long)t,(long)r,(long)b });
}

void CBoss::SetState(int state)
{
	CEnemy::SetState(state);
}

void CBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + bboxEnemyWidth;
	bottom = y + bboxEnemyHeight;
}

void CBoss::GetBoundingBoxActive(float& left, float& top, float& right, float& bottom)
{
	left = (x + bboxEnemyWidth / 2) - bboxEnemyActiveWidth;
	top = (y + bboxEnemyHeight / 2) - bboxEnemyActiveHeight;
	right = (x + bboxEnemyWidth / 2) + bboxEnemyActiveWidth;
	bottom = y + bboxEnemyActiveHeight;
}

void CBoss::GetBoundingBoxAwake(float& left, float& top, float& right, float& bottom)
{
	if (state != BOSS_STATE_IDLE)
		left = top = right = bottom = 0;
	else
	{
		left = (x + bboxEnemyWidth / 2) - bboxEnemyActiveWidth;
		top = (y + bboxEnemyHeight / 2) - bboxEnemyActiveHeight;
		right = (x + bboxEnemyWidth / 2) + bboxEnemyActiveWidth;
		bottom = y + bboxEnemyActiveHeight * 2;
	}
}
