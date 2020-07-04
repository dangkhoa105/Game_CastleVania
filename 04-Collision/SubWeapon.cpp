#include "SubWeapon.h"
#include "Candle.h"
#include "SpearGuard.h"
#include "debug.h"
#include "Simon.h"
#include "Bat.h"
#include "Brick.h"
#include "PlayScene.h"

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

	auto simon = CPlayScene::GetInstance()->GetSimon();
	coObjects->push_back(simon);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);// bắt đầu tính toán va chạm

	switch (this->state)
	{
	case STATE_BOOMERANG:
		if (nx > 0) vx -= (SUBWEAPON_VX / 50);
		else vx += (SUBWEAPON_VX / 50);
		break;
	case STATE_AXE:
		if (nx > 0) vx = SUBWEAPON_VX * 3 / 5;
		else vx = -SUBWEAPON_VX * 3 / 5;
		vy += SUBWEAPON_GRAVITY * dt;
		break;
	case STATE_GAS:
		if (!isBurning)
		{
			if (nx > 0) vx = SUBWEAPON_VX * 2 / 5;
			else vx = -SUBWEAPON_VX * 2 / 5;
			vy += SUBWEAPON_GRAVITY * dt;
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
			else if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is Simon 
			{
				if (state == STATE_GAS)
				{
					isBurning = true;					
					vx = 0;
					vy = 0;

					if (burning_start != 0 && GetTickCount() - burning_start >= 2000)
					{
						this->SetDestroy(true);
						burning_start = 0;
					}
				}
				if (state == STATE_BOOMERANG)
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

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (dynamic_cast<CBrick*>(obj))
		{
			if (this->AABB(obj) == true)
			{
				if (state == STATE_GAS)
				{
					isBurning = true;
					vx = 0;
					vy = 0;
					if (freeze_start != 0)
					{
						vx = 0;
						vy = 0;
					}
					if (GetTickCount() - freeze_start >= 2000)
					{
						this->SetDestroy(true);
						CPlayScene::GetInstance()->SetCountSW();
						freeze_start = 0;
					}
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
			CEnemy* e = dynamic_cast<CEnemy*> (obj);
			float vxs = e->vx;
			float vys = e->vy;
			if (state == STATE_CLOCK)
			{
				if (freeze_start != 0)
				{
					obj->vx = 0;
					obj->vy = 0;
				}
				if (GetTickCount() - freeze_start >= 2000)
				{
					obj->vx = vxs;
					obj->vy = vys;
					freeze_start = 0;
				}
			}
			fight = true;
			if (fight)
			{				
				CEnemy* e = dynamic_cast<CEnemy*> (obj);			

				if (this->AABB(obj) == true)
				{
					if (!e->isDestroy)
					{
						e->TakeDamage(damage);
						if (e->hp == 0)
						{
							e->SetDestroy(true);
						}
						DebugOut(L"Take damage \n");
						fight = false;
					}
				}

			}
		}
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
