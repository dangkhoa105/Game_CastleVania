#include "Skeleton.h"
#include "Simon.h"
#include "Brick.h"
#include "Game.h"
#include "PlayScene.h"
#include "debug.h"

void CSkeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CEnemy::Update(dt);
	auto pScene = CPlayScene::GetInstance();
	auto simon = pScene->GetSimon();
	coObjects->push_back(simon);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);// bắt đầu tính toán va chạm

	if (state == SKELETON_STATE_JUMPING)
		vy += SKELETON_GRAVITY * dt;

	if (throw_start != 0 && GetTickCount() - throw_start > 500)
		throw_start = 0;

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (dynamic_cast<CSimon*>(obj)) // if e->obj is Item Heart 
		{
			CSimon* f = dynamic_cast<CSimon*>(obj);
			float sl, st, sr, sb;
			float ml, mt, mr, mb;
			float al, at, ar, ab;
			f->GetBoundingBox(sl, st, sr, sb);
			simonPos.x = sl + (sr - sl) / 2;
			simonPos.y = st + (sb - st) / 2;
			this->GetBoundingBoxChangeState(ml, mt, mr, mb);
			this->GetBoundingBoxActive(al, at, ar, ab);

			if (CGame::AABB(ml, mt, mr, mb, sl, st, sr, sb) == true)
				this->SetState(SKELETON_STATE_JUMPING);

			if (CGame::AABB(al, at, ar, ab, sl, st, sr, sb) == true)
			{
				if (simonPos.x > x)
				{
					nx = 1;
					vx = -SKELETON_JUMPING_SPEED_X;
				}
				else if (simonPos.x < x)
				{
					nx = -1;
					vx = SKELETON_JUMPING_SPEED_X;
				}
				//vy = vy;
			}
		}
	}
	int random = rand() % (100 + 1 - 1) + 1;

	if (random == 1)
	{
		if (throw_start == 0 && this->state == SKELETON_STATE_JUMPING)
		{
			auto bone = new CBone();
			bone->nx = this->nx;
			bone->SetPosition(this->x, this->y);
			pScene->AddtoGrid(bone);
			throw_start = GetTickCount();
		}
	}

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		if (ny <= 0)
			y += min_ty * dy + ny * 0.4f;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is Item Heart 
			{
				if (ny == 0)
					return;

				if (ny != 0) vy = 0;

				if (onGround_start == 0)
				{
					onGround_start = GetTickCount();
				}
				if (onGround_start != 0 && GetTickCount() - onGround_start > 300)
				{
					if (simonPos.x > x)
						vx = SKELETON_JUMPING_SPEED_X * 2;
					else
						vx = -SKELETON_JUMPING_SPEED_X * 2;
					vy = -SKELETON_JUMPING_SPEED_X * 6 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / -SKELETON_JUMPING_SPEED_X * 2));
					onGround_start = 0;
				}
			}
			else
			{
				x += dx;
				if (e->ny != 0)
					y += dy;
			}
		}
	}

}

void CSkeleton::Render()
{
	if (this->state == SKELETON_STATE_JUMPING)
		animations["skeleton_ani_jumping"]->Render(-nx, x, y);

	//float l, t, r, b;
	//this->GetBoundingBoxActive(l, t, r, b);
	//RenderBoundingBox(RECT{ (long)l,(long)t,(long)r,(long)b });
}

void CSkeleton::SetState(int state)
{
	
	/*if (this->state == state)
		return;*/

	switch (state)
	{
	case SKELETON_STATE_IDLE:
		vx = vy = 0;
		isDestroy = false;
		isFinishReSpawn = false;
		StartRespawnTimeCounter();
		break;
	case SKELETON_STATE_JUMPING:
		if (nx > 0)
		{
			vx = -SKELETON_JUMPING_SPEED_X;
		}
		else
		{
			vx = SKELETON_JUMPING_SPEED_X;
		}
		vy = -SKELETON_JUMPING_SPEED_X * 2;
		reSpawnTimeStart = 0;
		isReSpawnWaiting = false;
		break;
	default:
		break;
	}
	CEnemy::SetState(state);
}

void CSkeleton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + bboxEnemyWidth;
	bottom = y + bboxEnemyHeight;
}

void CSkeleton::GetBoundingBoxActive(float& left, float& top, float& right, float& bottom)
{
	left = (x + bboxEnemyWidth / 2) - bboxEnemyActiveWidth;
	top = y;
	right = (x + bboxEnemyWidth / 2) + bboxEnemyActiveWidth;
	bottom = y + bboxEnemyActiveHeight;
}

void CSkeleton::GetBoundingBoxChangeState(float& left, float& top, float& right, float& bottom)
{
	if (state == SKELETON_STATE_IDLE)
	{
		left = (x + bboxEnemyWidth / 2) - bboxEnemyActiveWidth - bboxEnemyWidth;
		top = y;
		right = (x + bboxEnemyWidth / 2) + bboxEnemyActiveWidth + bboxEnemyWidth;
		bottom = y + bboxEnemyActiveHeight + bboxEnemyHeight;
	}
	else
		left = top = right = bottom = 0;
}
