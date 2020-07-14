#include "Zombie.h"
#include "Simon.h"
#include "Game.h"

void CZombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CEnemy::Update(dt);
	auto simon = CPlayScene::GetInstance()->GetSimon();
	coObjects->push_back(simon);

	if (IsRespawn())
	{
		SetState(ZOMBIE_STATE_IDLE);
		if (simon->x < this->x)
			nx = -1;
		else
			nx = 1;
		return;
	}

	D3DXVECTOR2 cam = CGame::GetInstance()->GetCamPos();

	//if ((this->x < cam.x - bboxEnemyWidth || this->x > cam.x + SCREEN_WIDTH || this->y < cam.y || this->y > cam.y + SCREEN_HEIGHT) && state == ZOMBIE_STATE_WALKING)
	//{
	//	isDestroy = true;
	//}

	if (this->GetState() == ZOMBIE_STATE_IDLE && this->IsActivate() == true)
		this->SetState(ZOMBIE_STATE_WALKING);

	if (state == ZOMBIE_STATE_WALKING)
		vy += MONKEY_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		//vy += MONKEY_GRAVITY * dt;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		x += min_tx * dx + nx * 0.1f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.1f;

		if (nx != 0 && ny == 0)
		{
			this->nx *= -1;
			this->vx = -vx;
		}
		else if (ny == -1)
		{
			vy = 0;
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CZombie::Render()
{
	if (state == ZOMBIE_STATE_WALKING)
		animations["zombie_ani_walking"]->Render(-nx, x, y);

	float l, t, r, b;
	this->GetBoundingBoxActive(l, t, r, b);
	RenderBoundingBox(RECT{ (long)l,(long)t,(long)r,(long)b });
	RenderBoundingBox();
}

void CZombie::SetState(int state)
{
	switch (state)
	{
	case ZOMBIE_STATE_IDLE:
		vx = vy = 0;
		x = initPositionX;
		y = initPositionY;
		isDestroy = false;
		isFinishReSpawn = false;
		StartRespawnTimeCounter();
		break;
	case ZOMBIE_STATE_WALKING:
		if (nx > 0)
			vx = ZOMBIE_WALKING_SPEED_X;
		else
			vx = -ZOMBIE_WALKING_SPEED_X;
		vy = 0;
		reSpawnTimeStart = 0;
		isDestroy = false;
		isReSpawnWaiting = false;
		break;
	default:
		break;
	}
	CEnemy::SetState(state);
}

void CZombie::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + bboxEnemyWidth;
	bottom = y + bboxEnemyHeight;
}

void CZombie::GetBoundingBoxActive(float& left, float& top, float& right, float& bottom)
{
	left = (x + bboxEnemyWidth / 2) - bboxEnemyActiveWidth;
	top = y;
	right = (x + bboxEnemyWidth / 2) + bboxEnemyActiveWidth;
	bottom = y + bboxEnemyActiveHeight;
}
