#include <algorithm>
#include "debug.h"
#include "ReadResourceFile.h"
#include "Simon.h"
#include "Game.h"

#include "Goomba.h"
#include "Brick.h"
#include "Candle.h"

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is Brick 
			{
				//isJumping = false;
				isGround = true;
				x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
				if (ny <= 0)
					y += min_ty * dy + ny * 0.4f;

				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
			}
			if (dynamic_cast<CCandle*>(e->obj)) // if e->obj is Candle 
			{
				x += dx;
				if (this->GetY() <= 320 - 64 - 32)
				{
					y += dy;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CSimon::Render()
{
	string ani;
	if (state == SIMON_STATE_IDLE) {
		if (nx > 0) ani = "simon_ani_idle";
		else ani = "simon_ani_idle";
	}
	else if (state == SIMON_STATE_WALKING_RIGHT)
		ani = "simon_ani_walking";
	else if (state == SIMON_STATE_WALKING_LEFT)
		ani = "simon_ani_walking";
	else if (state == SIMON_STATE_JUMP)
		ani = "simon_ani_jumping";
	else if (state == SIMON_STATE_SIT)
		ani = "simon_ani_sitting";
	else if (state == SIMON_STATE_JUMP_ATTACK)
		ani = "simon_ani_attacking";
	else if (state == SIMON_STATE_SIT_ATTACK)
		ani = "simon_ani_sit_attacking";
	else if (state == SIMON_STATE_STAND_ATTACK)
		ani = "simon_ani_attacking";
	
	
	int alpha = 255;
	if (untouchable) alpha = 128;
	animations[ani]->Render(nx, x, y, alpha);
	this->isComplete = animations[ani]->GetOver();
	
	// Whip
	this->whip->SetNx(nx);

	if (state == SIMON_STATE_SIT_ATTACK)
	{
		this->whip->SetPosition(this->x - 90, this->y + 15);
	}
	else if (state == SIMON_STATE_JUMP_ATTACK)
	{
		this->whip->SetPosition(this->x - 89, this->y + 4);
	}
	else
	{
		this->whip->SetPosition(this->x - 90, this->y + 3);
	}

	this->whip->Render();

	RenderBoundingBox();
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_IDLE:
		this->whip->SetState(WHIP_STATE_IDLE);
		vx = 0;
		break;
	case SIMON_STATE_WALKING_RIGHT:
		this->whip->SetState(WHIP_STATE_IDLE);
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		this->whip->SetState(WHIP_STATE_IDLE);
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		this->whip->SetState(WHIP_STATE_IDLE);
		isGround = false;
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	case SIMON_STATE_SIT:
		this->whip->SetState(WHIP_STATE_IDLE);
		vx = 0;
		vy = 0;
		break;
	case SIMON_STATE_SIT_ATTACK:
	case SIMON_STATE_STAND_ATTACK:
		this->whip->SetState(WHIP_STATE_FIGHT);
		break;
	default:
		break;
	}
}

void CSimon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (this->nx > 0) {
		left = this->x + 15;
		top = this->y;
		right = x + SIMON_BBOX_WIDTH + 17;
		bottom = y + SIMON_BBOX_HEIGHT;
	}
	else
	{
		left = this->x + 13;
		top = this->y;
		right = x + SIMON_BBOX_WIDTH + 15;
		bottom = y + SIMON_BBOX_HEIGHT;
	}
	
}

bool CSimon::IsHitting()
{
	//return this->isComplete;
	return state == SIMON_STATE_STAND_ATTACK || state == SIMON_STATE_SIT_ATTACK;
}

bool CSimon::IsComplete()
{
	return this->isComplete;
}

