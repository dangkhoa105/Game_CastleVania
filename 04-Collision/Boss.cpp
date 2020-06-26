#include "Boss.h"
#include "Simon.h"
#include "Game.h"
#include "PlayScene.h"
void CBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CEnemy::Update(dt);

	auto simon = CPlayScene::GetInstance()->GetSimon();
	coObjects->push_back(simon);


	float sl, st, sr, sb;
	float acl, act, acr, acb;

	float awl, awt, awr, awb;
	simon->GetBoundingBox(sl, st, sr, sb);
	this->GetBoundingBoxAwake(awl, awt, awr, awb);
	this->GetBoundingBoxActive(acl, act, acr, acb);
	simonPos.x = sl + (sr - sl) / 2;
	simonPos.y = st + (sb - st) / 2;
	if (CGame::AABB(awl, awt, awr, awb, sl, st, sr, sb) == true)
	{
		this->SetState(BOSS_STATE_FLYING);
	}

	D3DXVECTOR2 cam = CGame::GetInstance()->GetCamPos();

	if ((this->x < cam.x || this->x > cam.x + SCREEN_WIDTH - 100 || this->y < cam.y || this->y > cam.y + SCREEN_HEIGHT - 200) && state == BOSS_STATE_FLYING)
	{
		if (flyBack_start == 0)
			flyBack_start = GetTickCount();

		attack_start = 0;

		if (x < cam.x)
			vx = BOSS_FLYING_SPEED_X;
		else
			vx = -BOSS_FLYING_SPEED_X;

		float targetX = cam.x + rand() % (SCREEN_WIDTH);
		float targetY = cam.y + rand() % (SCREEN_HEIGHT);

		flyBack_time = abs(targetX - x) / BOSS_FLYING_SPEED_X;
		this->vy = (targetY - y) / BOSS_FLYING_BACK_TIME;
	}

	if (waiting_start != 0 && GetTickCount() - waiting_start > this->waiting_time)
	{
		this->waiting_start = 0;
	}

	if (state == BOSS_STATE_FLYING && attack_start == 0 && flyBack_start == 0 && waiting_start == 0)
	{
		flyRandom = false;
		if (CGame::AABB(acl, act, acr, acb, sl, st, sr, sb) == true)
		{
			float time_x = abs(x - simonPos.x) / (BAT_FLYING_SPEED_X);
			if (x > simonPos.x)
			{
				vx = -(BOSS_FLYING_SPEED_X);
			}
			else
			{
				vx = (BOSS_FLYING_SPEED_X);
			}

			vy = abs(y - simonPos.y) / time_x;
			attack_start = GetTickCount();
		}
		else
		{
			attack_start = GetTickCount();
		}
	}

	if (this->attack_start != 0 && GetTickCount() - this->attack_start > BOSS_ATTACK_TIME)
	{
		this->attack_start = 0;
		if (this->flyBack_start == 0)
		{
			this->flyBack_start = GetTickCount();
		}

		if (CGame::AABB(acl, act, acr, acb, sl, st, sr, sb) == false)
		{
			float targetX = cam.x + rand() % (SCREEN_WIDTH / 2);
			float targetY = cam.y + rand() % (SCREEN_HEIGHT / 2);

			if (x < cam.x)
				vx = BOSS_FLYING_SPEED_X;
			else
				vx = -BOSS_FLYING_SPEED_X;

			this->vy = (targetY - y) / BOSS_FLYING_BACK_TIME;
		}
	}

	if (state == BOSS_STATE_FLYING && this->flyBack_start != 0 && GetTickCount() - this->flyBack_start > this->flyBack_time)
	{
		if (!flyRandom)
		{
			this->flyBack_start = GetTickCount();
			flyRandom = true;
			if (this->x < cam.x + SCREEN_WIDTH)
			{
				this->vx = -BOSS_FLYING_SPEED_X;
			}
			else
			{
				this->vx = BOSS_FLYING_SPEED_X;
			}

			float targetX = cam.x + rand() % (SCREEN_WIDTH / 2);
			float targetY = cam.y + rand() % (SCREEN_HEIGHT / 3);

			this->vy = (targetY - y) / BOSS_FLYING_BACK_TIME;
		}
		this->flyBack_start = 0;
		waiting_start = GetTickCount();
		this->waiting_time = BOSS_WAITING_TIME_SHORT;
		this->vx = 0;
		this->vy = 0;

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

void CBoss::Render()
{
	if (state == BOSS_STATE_IDLE)
		animations["boss_ani_idle"]->Render(-nx, x, y);
	if (state == BOSS_STATE_FLYING || state == BOSS_STATE_WAITING)
		animations["boss_ani_flying"]->Render(-nx, x, y);

	float l, t, r, b;
	this->GetBoundingBoxActive(l, t, r, b);
	RenderBoundingBox(RECT{ (long)l,(long)t,(long)r,(long)b });
}

void CBoss::SetState(int state)
{
	isDestroy = false;
	CEnemy::SetState(state);
	switch (state)
	{
	case BOSS_STATE_FLYING:
		break;
	default:
		break;
	}
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
	bottom = y + bboxEnemyActiveHeight * 2;
}

void CBoss::GetBoundingBoxAwake(float& left, float& top, float& right, float& bottom)
{
	if (state != BOSS_STATE_IDLE)
		left = top = right = bottom = 0;
	else
	{
		left = (x + bboxEnemyWidth / 2) - bboxEnemyActiveWidth - 32;
		top = (y + bboxEnemyHeight / 2) - bboxEnemyActiveHeight;
		right = (x + bboxEnemyWidth / 2) + bboxEnemyActiveWidth;
		bottom = y + bboxEnemyActiveHeight * 2;
	}
}
