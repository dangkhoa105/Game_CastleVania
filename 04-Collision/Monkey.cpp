#include "Monkey.h"
#include "Game.h"

void CMonkey::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CEnemy::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);// bắt đầu tính toán va chạm

	if (this->GetJumpStartTime() && GetTickCount() - this->GetJumpStartTime() > 1000)
	{
		this->SetState(MONKEY_STATE_JUMPING);
		this->ResetJumpStartTime();
	}

	if (this->isJumping)
		vy += MONKEY_GRAVITY * dt;

	if (this->GetJumpStartTime())
		return;

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
				this->SetState(MONKEY_STATE_START);
			}
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
		//x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//if (ny <= 0)
		//	y += min_ty * dy + ny * 0.4f;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];		
			if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is Item Heart 
			{
				if (e->nx != 0)
				{
					y += dy;
				}
				float minvy = -0.9;
				float maxvy = -0.4;
				float minvx = 0.2;
				float maxvx = 0.5;
				vx = 0;
				vy = 0;
				if (onGround_start == 0)
				{
					onGround_start = GetTickCount();					
				}
				if (onGround_start != 0 && GetTickCount() - onGround_start > 300)
				{
					int random = rand()%(2 + 1 - 1) + 1;
					vx = minvx + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxvx - minvx)));
					vy = minvy + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxvy - minvy)));
					vx = random == 1 ? vx : -vx;
					if (vx > 0)
						this->nx = 1;
					else if(vx < 0)
						this->nx = -1;
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

void CMonkey::Render()
{
	if (state == MONKEY_STATE_IDLE)
		animations["monkey_ani_idle"]->Render(-nx, x, y);
	else if (state == MONKEY_STATE_START)
		animations["monkey_ani_jumping"]->Render(-nx, x, y);
	else if (state == MONKEY_STATE_JUMPING)
		animations["monkey_ani_jumping"]->Render(-nx, x, y);

	float l, t, r, b;
	this->GetBoundingBoxActive(l, t, r, b);
	RenderBoundingBox(RECT{ (long)l,(long)t,(long)r,(long)b });
}

void CMonkey::SetState(int state)
{
	if (this->state == state)
		return;

	switch (state)
	{
	case MONKEY_STATE_IDLE:
		vx = vy = 0;
		this->isJumping = false;
		break;
	case MONKEY_STATE_START:
		this->jump_start = GetTickCount();
		this->isJumping = false;
		vx = vy = 0;
		break;
	case MONKEY_STATE_JUMPING:
		//vx = MONKEY_JUMPING_SPEED_X * 2;	
		this->isJumping = true;
		break;
	default:
		break;
	}
	CEnemy::SetState(state);
}

void CMonkey::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isDestroy)
		return;

	left = x;
	top = y;
	right = x + bboxEnemyWidth;
	bottom = y + bboxEnemyHeight;
}

void CMonkey::GetBoundingBoxActive(float& left, float& top, float& right, float& bottom)
{
	if (this->state == MONKEY_STATE_JUMPING)
		left = top = right = bottom = 0;
	else
	{
		left = x;
		top = y;
		right = x + bboxEnemyActiveWidth;
		bottom = y + bboxEnemyActiveHeight;
	}
}