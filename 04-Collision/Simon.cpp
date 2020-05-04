﻿#include <algorithm>
#include "debug.h"
#include "ReadResourceFile.h"
#include "Simon.h"
#include "Game.h"

#include "Goomba.h"
#include "Brick.h"
#include "Candle.h"
#include "IHeart.h"
#include "IMoneyBag.h"
#include "MoneyBagTrigger.h"
#include "Item.h"
#include "ChangeScene.h"

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
		CalcPotentialCollisions(coObjects, coEvents);// bắt đầu tính toán va chạm

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
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		if (ny <= 0)
			y += min_ty * dy + ny * 0.4f;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is Brick 
			{
				if (e->ny != 0)
				{
					if (this->state == SIMON_STATE_SIT_ATTACK || this->state == SIMON_STATE_STAND_ATTACK)
						vx = 0;

					isGround = true;

					if (this->state != SIMON_STATE_AUTO_WALKING)
						if (nx != 0) vx = 0;

					if (ny != 0) vy = 0;
				}

				if (this->state == SIMON_STATE_AUTO_WALKING)
					break;
			}
			else if (dynamic_cast<CChangeScene*>(e->obj)) // if e->obj is Change Scene 
			{
				if (this->GetState() != SIMON_STATE_AUTO_WALKING)
				{
					x += dx;
					if (e->ny != 0)
					{
						y += dy;
					}
				}
				else
				{
					CChangeScene* changeScene = dynamic_cast<CChangeScene*>(e->obj);
					//changeScene->SetIsChangeScene(true);
					idChangeScene = changeScene->GetIdNextScene();
					changeScene->SetDestroy(true);
				}
			}
			else
			{
				if (dynamic_cast<CCandle*>(e->obj)) // if e->obj is Candle 
				{
					x += dx;
					if (e->ny != 0)
					{
						y += dy;
					}
				}
				else if (dynamic_cast<CItem*>(e->obj)) // if e->obj is Item Heart 
				{
					CItem* item = dynamic_cast<CItem*>(e->obj);
					switch (item->idItem)
					{
					case ID_IHEART:
						break;
					case ID_IKNIFE:
						subWeapon = SUBWEAPON::KNIFE;
						break;
					case ID_IWHIP:
						this->SetState(SIMON_STATE_ITEM);
						this->level += 1;
						this->whip->SetLevel(level);
						break;
					case ID_IMONEYBAG:
						this->SetState(SIMON_STATE_ITEM);
						break;
					default:
						break;
					}
					item->SetDestroy(true);
				}
				else if (dynamic_cast<CEntrace*>(e->obj)) // if e->obj is Entrace
				{
					CEntrace* entrace = dynamic_cast<CEntrace*>(e->obj);
					entrace->SetDestroy(true);
					this->SetState(SIMON_STATE_AUTO_WALKING);
				}
				else if (dynamic_cast<CMoneyBagTrigger*>(e->obj)) // if e->obj is Money Bag Trigger
				{
					x += dx;
					if (e->ny != 0)
					{
						y += dy;
					}

					for (UINT i = 0; i < coObjects->size(); i++)
					{
						LPGAMEOBJECT obj = coObjects->at(i);

						if (dynamic_cast<CMoneyBagTrigger*>(obj))
						{
							CMoneyBagTrigger* e = dynamic_cast<CMoneyBagTrigger*> (obj);

							if (this->AABB(obj) == true)
							{
								if (e->GetState() != MONEYBAG_STATE_TOUCH)
								{
									e->SetState(MONEYBAG_STATE_TOUCH);
									e->SetDestroy(true);
								}
							}
						}
					}
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	// Xử lý va chạm AABB, cho 2 object trùng nhau
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (dynamic_cast<CItem*>(obj)) // if e->obj is Item Heart 
		{
			CItem* e = dynamic_cast<CItem*>(obj);
			if (this->AABB(obj) == true)
			{
				CItem* item = dynamic_cast<CItem*>(obj);
				switch (item->idItem)
				{
				case ID_IHEART:
					break;
				case ID_IKNIFE:
					subWeapon = SUBWEAPON::KNIFE;
					break;
				case ID_IWHIP:
					this->SetState(SIMON_STATE_ITEM);
					this->level += 1;
					this->whip->SetLevel(level);
					break;
				default:
					break;
				}
				item->SetDestroy(true);
			}
		}
	}

	if (this->attack_start != 0)
	{
		if (!this->isKnife)
		{
			if (this->state == SIMON_STATE_SIT_ATTACK)
			{
				this->whip->SetPosition(this->x - 90, this->y + 15);
			}
			else
			{
				this->whip->SetPosition(this->x - 90, this->y + 3);
			}
			this->whip->SetNx(this->nx);
			this->whip->Update(dt, coObjects);
		}
	}
}

void CSimon::Render()
{
	string ani;
	if (state == SIMON_STATE_IDLE) {
		if (nx > 0) ani = "simon_ani_idle";
		else ani = "simon_ani_idle";
	}
	else if (state == SIMON_STATE_AUTO_WALKING)
		ani = "simon_ani_walking";
	else if (state == SIMON_STATE_WALKING_RIGHT)
		ani = "simon_ani_walking";
	else if (state == SIMON_STATE_WALKING_LEFT)
		ani = "simon_ani_walking";
	else if (state == SIMON_STATE_JUMP)
		ani = "simon_ani_jumping";
	else if (state == SIMON_STATE_SIT)
		ani = "simon_ani_sitting";
	else if (state == SIMON_STATE_ITEM)
		ani = "simon_ani_item";
	else if (state == SIMON_STATE_SIT_ATTACK)
		ani = "simon_ani_sit_attacking";
	else if (state == SIMON_STATE_STAND_ATTACK)
		ani = "simon_ani_attacking";

	int alpha = 255;
	if (untouchable) alpha = 128;
	animations[ani]->Render(nx, x, y, alpha);

	if (this->attack_start != 0 && !this->isKnife)
		this->whip->Render();

	//RenderBoundingBox();
}

void CSimon::SetState(int state)
{
	switch (state)
	{
	case SIMON_STATE_IDLE:
		this->whip->SetState(WHIP_STATE_IDLE);
		this->knife->SetState(KNIFE_STATE_IDLE);
		vx = 0;
		break;
	case SIMON_STATE_AUTO_WALKING:
		this->whip->SetState(WHIP_STATE_IDLE);
		this->knife->SetState(KNIFE_STATE_IDLE);
		nx = 1;
		vx = SIMON_WALKING_SPEED / 2;
		break;
	case SIMON_STATE_WALKING_RIGHT:
		this->whip->SetState(WHIP_STATE_IDLE);
		this->knife->SetState(KNIFE_STATE_IDLE);
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		this->whip->SetState(WHIP_STATE_IDLE);
		this->knife->SetState(KNIFE_STATE_IDLE);
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		this->whip->SetState(WHIP_STATE_IDLE);
		this->knife->SetState(KNIFE_STATE_IDLE);
		isGround = false;
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	case SIMON_STATE_SIT:
		this->whip->SetState(WHIP_STATE_IDLE);
		this->knife->SetState(KNIFE_STATE_IDLE);
		vx = 0;
		vy = 0;
		break;
	case SIMON_STATE_ITEM:
		this->ResetAttack();
		this->update_start = GetTickCount();
		this->whip->SetState(WHIP_STATE_IDLE);
		this->knife->SetState(KNIFE_STATE_IDLE);
		vx = 0;
		break;
	case SIMON_STATE_SIT_ATTACK:
		vx = 0;
		if (isKnife == true)
		{
			this->whip->ResetAttack();
			this->whip->SetState(WHIP_STATE_FIGHT);
			this->knife->SetState(KNIFE_STATE_FIGHT);
		}
		else
		{
			this->whip->ResetAttack();
			this->whip->SetState(WHIP_STATE_FIGHT);
			this->knife->SetState(KNIFE_STATE_IDLE);
		}
		this->attack_start = GetTickCount();
		break;
	case SIMON_STATE_STAND_ATTACK:
		vx = this->state == SIMON_STATE_IDLE || this->state == SIMON_STATE_WALKING_LEFT
			|| this->state == SIMON_STATE_WALKING_RIGHT ? 0 : vx;
		if (isKnife == true)
		{
			this->whip->ResetAttack();
			this->whip->SetState(WHIP_STATE_FIGHT);
			this->knife->SetState(KNIFE_STATE_FIGHT);
		}
		else
		{
			this->whip->ResetAttack();
			this->whip->SetState(WHIP_STATE_FIGHT);
			this->knife->SetState(KNIFE_STATE_IDLE);
		}
		this->attack_start = GetTickCount();
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
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
	return state == SIMON_STATE_STAND_ATTACK || state == SIMON_STATE_SIT_ATTACK;
}

//bool CSimon::IsComplete()
//{
//	return this->isComplete;
//}

