﻿#include "SubWeapon.h"
#include "Candle.h"
#include "SpearGuard.h"
#include "debug.h"
#include "Simon.h"
#include "Bat.h"
#include "Brick.h"
#include "PlayScene.h"
#include"Game.h"
void CSubWeapon::Render()
{
	if (state == STATE_CLOCK)
		return;

	string ani;
	switch (this->state)
	{
	case STATE_KNIFE:
		ani = "item_knife";
		break;
	case STATE_BOOMERANG:
		ani = "boomerang_ani";
		break;
	case STATE_AXE:
		ani = "axe_ani";
		break;
	case STATE_GAS:
		if (!isBurning)
			ani = "gas_ani_idle";
		else
			ani = "gas_ani_burning";
		break;
	}
	animations[ani]->Render(-nx, x, y);

	//RenderBoundingBox();
}

void CSubWeapon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	switch (this->state)
	{
	case STATE_KNIFE:
		right = x + SUBWEAPON_BBOX_WIDTH;
		bottom = y + SUBWEAPON_BBOX_HEIGHT / 2;
		break;
	case STATE_BOOMERANG:
		right = x + SUBWEAPON_BBOX_WIDTH;
		bottom = y + SUBWEAPON_BBOX_HEIGHT;
	case STATE_AXE:
		right = x + SUBWEAPON_BBOX_WIDTH;
		bottom = y + SUBWEAPON_BBOX_HEIGHT;
		break;
	case STATE_GAS:
		if (!isBurning)
		{
			right = x + SUBWEAPON_BBOX_WIDTH / 2;
			bottom = y + SUBWEAPON_BBOX_HEIGHT / 2;
		}
		else
		{
			right = x + SUBWEAPON_BBOX_WIDTH;
			bottom = y + SUBWEAPON_BBOX_HEIGHT;
		}
		break;
	}
}

void CSubWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	auto cGame = CGame::GetInstance();

	auto simon = CPlayScene::GetInstance()->GetSimon();

	D3DXVECTOR2 cam = cGame->GetCamPos();
	coObjects->push_back(simon);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);// bắt đầu tính toán va chạm

	switch (this->state)
	{
	case STATE_BOOMERANG:
		if (nx > 0)
		{
			vx -= (SUBWEAPON_VX / 50);
			if (vx < 0) reserve_right = true;
		}
		else
		{
			vx += (SUBWEAPON_VX / 50);
			if (vx > 0) reserve_left = true;
		}
		if (this->x < cam.x)
		{
			reserve_left = true;
			if (!reserve_right)
			{
				vx = !reset ? 0 : vx;
				reset = true;
			}
		}
		else
		{
			reset = nx < 0 ? false : reset;
		}
		if (this->x + 32 > cam.x + SCREEN_WIDTH)
		{
			reserve_right = true;
			if (!reserve_left)
			{
				vx = !reset ? 0 : vx;
				reset = true;
			}
		}
		else
		{
			reset = nx > 0 ? false : reset;
		}
		break;
	case STATE_AXE:
		if (nx > 0) vx = SUBWEAPON_VX * 3 / 5;
		else vx = -SUBWEAPON_VX * 3 / 5;
		vy += SUBWEAPON_GRAVITY * dt;
		break;
	case STATE_GAS:
		if (!isBurning)
		{
			if (nx > 0) vx = SUBWEAPON_VX * 3.5f / 5;
			else vx = -SUBWEAPON_VX * 3.5f / 5;
			vy += SUBWEAPON_GRAVITY * 2.5f * dt;
		}
		break;
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

			if (dynamic_cast<CSimon*>(e->obj)) // if e->obj is Simon 
			{
				CSimon* f = dynamic_cast<CSimon*>(e->obj);
				if (state == STATE_BOOMERANG)
				{
					this->SetDestroy(true);
					CPlayScene::GetInstance()->SetCountSW();
				}
				if (state == STATE_GAS)
				{
					f->x += f->dx;
					if (e->ny != 0)
						f->y += f->dy;
				}
			}
			else if (dynamic_cast<CEnemy*>(e->obj)) // if e->obj is Simon 
			{
				CEnemy* enemy = dynamic_cast<CEnemy*>(e->obj);
				float vxs = enemy->vx;
				float vys = enemy->vy;

				if (dynamic_cast<CBoss*>(e->obj))
				{
					if (simon->subWeapon->fight)
					{
						simon->subWeapon->isColliWithBoss = true;
						if (!enemy->isDestroy && enemy->state != 0)
						{
							enemy->TakeDamage(damage);
							if (enemy->hp == 0)
							{
								enemy->SetDestroy(true);
							}
							DebugOut(L"Take damage \n");
							simon->subWeapon->fight = false;
						}
					}
				}
				else
				{
					if (simon->subWeapon->fight)
					{
						if (!enemy->isDestroy && enemy->state != 0)
						{
							enemy->TakeDamage(damage);
							if (enemy->hp == 0)
							{
								enemy->SetDestroy(true);
							}
							DebugOut(L"Take damage \n");
							simon->subWeapon->fight = false;
						}
					}
				}


				if (nx != 0)
				{
					x += dx;
				}
				if (ny != 0)
				{
					y += dy;
				}
			}
			else if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is Simon 
			{
				if (state == STATE_GAS)
				{
					isBurning = true;
					vx = 0;
					vy = 0;

					if (burning_start == 0)
						burning_start = GetTickCount();

					if (burning_start != 0 && GetTickCount() - burning_start > FREEZE_TIME)
					{
						isBurning = false;
						this->SetDestroy(true);
						burning_start = 0;
					}
				}
				else
				{
					x += dx;
					y += dy;
				}
			}
			else
			{
				x += dx;
				y += dy;
			}
		}
	}

	D3DXVECTOR2 simonPos = { 0, 0 };

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (dynamic_cast<CSimon*>(obj))
		{
			auto simon = dynamic_cast<CSimon*>(obj);
			float l, t, r, b;
			simon->GetBoundingBox(l, t, r, b);
			if (state==STATE_CLOCK)
			{
				x = l;
				y = t;
			}
			simonPos.x = l + (r - l) / 2;
			simonPos.y = t + (b - t) / 2;
		}
		if (dynamic_cast<CBrick*>(obj))
		{
			if (this->AABB(obj) == true)
			{
				if (state == STATE_GAS)
				{
					isBurning = true;
					vx = 0;
					vy = 0;

					if (burning_start == 0)
						burning_start = GetTickCount();				
				}
			}
		}
		if (dynamic_cast<CCandle*>(obj))
		{
			CCandle* e = dynamic_cast<CCandle*>(obj);

			if (this->AABB(obj) == true)
			{
				if (state == STATE_KNIFE)
				{
					this->SetDestroy(true);
					CPlayScene::GetInstance()->SetCountSW();
				}
				if (e->GetState() != CANDLE_STATE_DESTROYED)
				{
					e->SetState(CANDLE_STATE_DESTROYED);
					e->SetDestroy(true);
					e->SetFall(true);
				}
			}
		}
		if (dynamic_cast<CEnemy*>(obj))
		{
			CEnemy* e = dynamic_cast<CEnemy*>(obj);
			float vxs = e->vx;
			float vys = e->vy;

			if (state == STATE_CLOCK)
			{
				if (freeze_start != 0)
				{
				
					e->freezeEnemy = true;
				}
				if (GetTickCount() - freeze_start >= FREEZE_TIME)
				{					
					e->freezeEnemy = false;					
					freeze_start = 0;
					this->isDestroy = true;
				}
			}
		}
	}

	if (burning_start != 0 && GetTickCount() - burning_start > FREEZE_TIME)
	{
		isBurning = false;
		this->SetDestroy(true);
		CPlayScene::GetInstance()->SetCountSW();
		burning_start = 0;
	}
}

void CSubWeapon::SetState(int state)
{
	switch (state)
	{
	case STATE_KNIFE:
		if (nx > 0)
			vx = SUBWEAPON_VX * 3 / 5;
		else
			vx = -SUBWEAPON_VX * 3 / 5;
		vy = 0;
		break;
	case STATE_BOOMERANG:
		vx = nx * SUBWEAPON_VX;
		vy = 0;
		break;
	case STATE_AXE:
		vx = SUBWEAPON_VX;
		vy = -SUBWEAPON_VY;
		break;
	case STATE_CLOCK:
		freeze_start = GetTickCount();
		break;
	case STATE_GAS:
		if (!isBurning)
		{
			vx = SUBWEAPON_VX;
			vy = -SUBWEAPON_VY;
		}
		else
			burning_start = GetTickCount();
		break;
	}
	CGameObject::SetState(state);
}
