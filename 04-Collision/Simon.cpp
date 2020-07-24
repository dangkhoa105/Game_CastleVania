#include <algorithm>
#include "debug.h"
#include "ReadResourceFile.h" 
#include "Simon.h"
#include "Game.h"

#include "Goomba.h"
#include "Brick.h"
#include "Candle.h"
#include "MoneyBagTrigger.h"
#include "Item.h"
#include "ChangeScene.h"
#include "BreakWallTrigger.h"

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == SIMON_STATE_DIE)
		return;

	// Calculate dx, dy 
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);// bắt đầu tính toán va chạm
	//DebugOut(L" vy=%f", vy);

	if (this->isStartOnStair) {
		if (!this->isFirstStepOnStair)
			this->HandleFirstStepOnStair();
		else {
			if (this->onStairDirection == STAIRDIRECTION::UPRIGHT)
				this->SetState(SIMON_STATE_STAIR_UP_RIGHT);
			else if (this->onStairDirection == STAIRDIRECTION::UPLEFT)
				this->SetState(SIMON_STATE_STAIR_UP_LEFT);
			else if (this->onStairDirection == STAIRDIRECTION::DOWNRIGHT)
				this->SetState(SIMON_STATE_STAIR_DOWN_RIGHT);
			else if (this->onStairDirection == STAIRDIRECTION::DOWNLEFT)
				this->SetState(SIMON_STATE_STAIR_DOWN_LEFT);
		}
	}
	this->HandlePerStepOnStair();

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// Simple fall down
	if (!this->isStartOnStair && !this->isOnStair
		&& state != SIMON_STATE_STAIR_UP_IDLE
		&& state != SIMON_STATE_STAIR_DOWN_IDLE
		&& state != SIMON_STATE_ATTACK_STAIR_UP
		&& state != SIMON_STATE_ATTACK_STAIR_DOWN)
	{
		vy += SIMON_GRAVITY * dt;
	}


	bool colSweptAABBNY = false;
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

			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CBreakWall*>(e->obj)) // if e->obj is Brick 
			{
				if (e->ny < 0)
				{
					colBrickSweptAABB = false;
					if (hp == 0)
					{
						this->SetState(SIMON_STATE_DIE);
						this->isOnStair = false;
						break;
					}
					if (this->state == SIMON_STATE_JUMP)
					{
						this->SetState(SIMON_STATE_IDLE);
					}

					if (this->isOnStair) {
						x += dx;
						y += dy;
					}
					else if (this->state == SIMON_STATE_HURT)
					{
						this->state = SIMON_STATE_IDLE;
					}
					else
					{
						if (ny != 0) vy = 0;
					}

					if (this->state == SIMON_STATE_SIT_ATTACK || this->state == SIMON_STATE_STAND_ATTACK)
						vx = 0;

					if (this->state != SIMON_STATE_AUTO_WALKING)
						if (nx != 0) vx = 0;

					if (ny != 0) vy = 0;
				}
				else if (e->ny > 0 && this->vy < 0) {
					colBrickSweptAABB = true;
					y += dy;
					if (nx != 0) vx = 0;
				}
				if (e->nx != 0) {
					if (this->isStartOnStair || this->isOnStair)
					{
						x += dx;
						y += dy;
					}
					else {

						//if (nx != 0) vx = 0;
						if (ny != 0) vy = 0;
					}
				}

				if (this->state == SIMON_STATE_AUTO_WALKING)
					break;
			}
			else if (dynamic_cast<CChangeScene*>(e->obj)) // if e->obj is Change Scene 
			{
				/*if (this->GetState() != SIMON_STATE_AUTO_WALKING)
				{
					x += dx;
					if (e->ny != 0)
					{
						y += dy;
					}
				}*/
				/*else if (this->GetState() == SIMON_STATE_AUTO_WALKING || this->GetState() == SIMON_STATE_AUTO_WALKING_STAIR)
				{*/
				CChangeScene* changeScene = dynamic_cast<CChangeScene*>(e->obj);
				//changeScene->SetIsChangeScene(true);
				idChangeScene = changeScene->GetIdNextScene();
				//changeScene->SetDestroy(true);
			//}
			}
			else
			{
				if (dynamic_cast<CCandle*>(e->obj)) // if e->obj is Candle 
				{
					if (e->nx != 0)
						x += dx;
					if (e->ny != 0 && isGround == false)
					{
						y += dy;
					}
				}
				else if (dynamic_cast<CBridge*>(e->obj)) // if e->obj is Candle 
				{
					CBridge* bridge = dynamic_cast<CBridge*>(e->obj);
					if (state == SIMON_STATE_JUMP)
					{
						SetState(SIMON_STATE_IDLE);
					}
				}
				else if (dynamic_cast<CItem*>(e->obj)) // if e->obj is Item Heart 
				{
					CItem* item = dynamic_cast<CItem*>(e->obj);
					switch (item->idItem)
					{
					case ID_IHEART:
						this->heart += 5;
						break;
					case ID_SMALL_IHEART:
						this->heart += 1;
						break;
					case ID_IKNIFE:
						subWeapons = SUBWEAPON::KNIFE;
						break;
					case ID_IWHIP:
						this->SetState(SIMON_STATE_ITEM);
						this->level += 1;
						this->whip->SetLevel(level);
						break;
					case ID_IMONEYBAG_SPECIAL:
						score += 1000;
						break;
					case ID_IMONEYBAG_400:
						score += 400;
						break;
					case ID_IMONEYBAG_700:
						score += 700;
						break;
					case ID_IMONEYBAG_1000:
						score += 1000;
						break;
					case ID_IBOOMERANG:
						subWeapons = SUBWEAPON::BOOMERANG;
						break;
					case ID_IAXE:
						subWeapons = SUBWEAPON::AXE;
						break;
					case ID_ICLOCK:
						subWeapons = SUBWEAPON::CLOCK;
						break;
					case ID_IGAS:
						subWeapons = SUBWEAPON::GAS;
						break;
					case ID_ICROWN:
						score += 2000;
						break;
					case ID_IDOUBLESHOT:
						break;
					case ID_IKFC:
						this->hp += 4;
						break;
					case ID_IPOTION:
						if (untouchable_start == 0)
						{
							if (untouchable != 1) {
								StartUntouchable();
								break; // không xét tiếp va chạm khi defect
							}
						}
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
				else if (dynamic_cast<CBreakWallTrigger*>(e->obj)) // if e->obj is Money Bag Trigger
				{
					x += dx;
					if (e->ny != 0)
					{
						y += dy;
					}

					for (UINT i = 0; i < coObjects->size(); i++)
					{
						LPGAMEOBJECT obj = coObjects->at(i);

						if (dynamic_cast<CBreakWallTrigger*>(obj))
						{
							CBreakWallTrigger* e = dynamic_cast<CBreakWallTrigger*> (obj);

							if (this->AABB(obj) == true)
							{
								e->SetDestroy(true);
							}
						}
					}
				}
				else if (dynamic_cast<CSetLastSceneTrigger*>(e->obj)) // if e->obj is Money Bag Trigger
				{
					x += dx;
					if (e->ny != 0)
					{
						y += dy;
					}

					for (UINT i = 0; i < coObjects->size(); i++)
					{
						LPGAMEOBJECT obj = coObjects->at(i);

						if (dynamic_cast<CSetLastSceneTrigger*>(obj))
						{
							CSetLastSceneTrigger* e = dynamic_cast<CSetLastSceneTrigger*> (obj);

							if (this->AABB(obj) == true)
							{
								e->SetDestroy(true);
								this->isColliWithTrigger = true;
							}
						}
					}
				}
				else if (dynamic_cast<CEnemy*>(e->obj))
				{
					auto f = dynamic_cast<CEnemy*>(e->obj);
					CBat* bat = dynamic_cast<CBat*>(e->obj);
					CCrow* crow = dynamic_cast<CCrow*>(e->obj);
					if (untouchable_start == 0 && f->state != 0 && !f->isDestroy)
					{
						if (!this->isOnStair)
						{
							if (dynamic_cast<CBat*>(e->obj))
								bat->isDestroy = true;
							if (dynamic_cast<CCrow*>(e->obj))
								crow->isDestroy = true;
							this->SetState(SIMON_STATE_HURT);
							x += dx;
							y += dy;
						}
						else 
							this->SetHp();

						if (untouchable != 1) {
							StartUntouchable();
							break; // không xét tiếp va chạm khi defect
						}
					}
					else
					{
						if (e->nx != 0)
						{
							x += dx;
						}
						if (e->ny != 0)
						{
							y += dy;
						}
					}
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	bool colGround = false;

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
					this->heart += 5;
					break;
				case ID_SMALL_IHEART:
					this->heart += 1;
					break;
				case ID_IKNIFE:
					subWeapons = SUBWEAPON::KNIFE;
					break;
				case ID_IWHIP:
					this->SetState(SIMON_STATE_ITEM);
					this->level += 1;
					this->whip->SetLevel(level);
					break;
				case ID_IMONEYBAG_SPECIAL:
					score += 1000;
					break;
				case ID_IMONEYBAG_400:
					score += 400;
					break;
				case ID_IMONEYBAG_700:
					score += 700;
					break;
				case ID_IMONEYBAG_1000:
					score += 1000;
					break;
				case ID_IBOOMERANG:
					subWeapons = SUBWEAPON::BOOMERANG;
					break;
				case ID_IAXE:
					subWeapons = SUBWEAPON::AXE;
					break;
				case ID_ICLOCK:
					subWeapons = SUBWEAPON::CLOCK;
					break;
				case ID_IGAS:
					subWeapons = SUBWEAPON::GAS;
					break;
				case ID_ICROWN:
					score += 2000;
					break;
				case ID_IDOUBLESHOT:
					this->isDoubleShot = true;
					break;
				case ID_IKFC:
					this->hp += 4;
					break;
				case ID_IPOTION:
					if (untouchable_start == 0)
					{
						if (untouchable != 1) {
							StartUntouchable();
							break; // không xét tiếp va chạm khi defect
						}
					}
					break;
				default:
					break;
				}
				item->SetDestroy(true);
			}
		}
		if (dynamic_cast<CStair*>(obj)) // if e->obj is Item Heart 
		{
			CStair* f = dynamic_cast<CStair*>(obj);
			if (this->AABB(obj) == true)
			{
				if (state == SIMON_STATE_STAIR_UP_LEFT && f->GetDirectionStair() == (int)STAIRDIRECTION::UPLEFTRIGHT)
				{
					float sl, st, sr, sb;
					f->GetBoundingBox(sl, st, sr, sb);
					float l, t, r, b;
					this->GetBoundingBox(l, t, r, b);

					if (b < sb)
					{
						SetState(SIMON_STATE_IDLE);
						this->isOnStair = false;
						this->isStartOnStair = false;
						this->isFirstStepOnStair = false;
						this->onStairDirection = static_cast<STAIRDIRECTION>(f->GetDirectionStair());
						return;
					}
				}

				if ((state == SIMON_STATE_STAIR_DOWN_LEFT && f->GetDirectionStair() == (int)STAIRDIRECTION::UPLEFTRIGHT))
				{
					SetState(SIMON_STATE_IDLE);
					this->isOnStair = false;
					this->isStartOnStair = false;
					this->isFirstStepOnStair = false;
					return;
				}

				if (!this->isColliWithStair)
				{
					if (this->isOnStair && f->GetDirectionStair() != (int)STAIRDIRECTION::UPLEFTRIGHT)
					{
						SetState(SIMON_STATE_IDLE);
						this->isOnStair = false;
						this->isStartOnStair = false;
						this->isFirstStepOnStair = false;
						return;
					}
					this->isColliWithStair = true;
					this->onStairDirection = static_cast<STAIRDIRECTION>(f->GetDirectionStair());
					this->stairPos = { f->x,f->y };
					f->SetActive(true);
					return;
				}
			}
			else if (f->CheckActive())
			{
				f->SetActive(false);
				this->isColliWithStair = false;
				if (!this->isOnStair)
					this->onStairDirection = STAIRDIRECTION::DEFAULT; //reset
			}
		}
		if (dynamic_cast<CSpearGuard*>(obj)) // if e->obj is Item Heart 
		{
			CSpearGuard* f = dynamic_cast<CSpearGuard*>(obj);
			float sl, st, sr, sb;
			float ml, mt, mr, mb;
			this->GetBoundingBox(sl, st, sr, sb);
			f->GetBoundingBoxActive(ml, mt, mr, mb);
			if (CGame::AABB(ml, mt, mr, mb, sl, st, sr, sb) == true)
			{
				if (untouchable != 1)
				{
					if (f->x < this->x)
					{
						f->nx = 1;
						f->vx = SPEAR_GUARD_WALKING_SPEED * 2;
					}
					else
					{
						f->vx = -SPEAR_GUARD_WALKING_SPEED * 2;
						f->nx = -1;
					}
				}
			}
			if (this->AABB(obj) == true) // if e->obj is Item Heart 
			{
				if (untouchable_start == 0 && !f->isDestroy) {

					if (!this->isOnStair)
					{						
						this->SetState(SIMON_STATE_HURT);
					}
					else
						this->SetHp();

					if (untouchable != 1) {
						StartUntouchable();
						break; // không xét tiếp va chạm khi defect
					}
					
				}
			}
		}
		if (dynamic_cast<CBat*>(obj)) // if e->obj is Item Heart 
		{
			CBat* f = dynamic_cast<CBat*>(obj);
			float sl, st, sr, sb;
			float ml, mt, mr, mb;
			this->GetBoundingBox(sl, st, sr, sb);
			f->GetBoundingBoxActive(ml, mt, mr, mb);
			if (CGame::AABB(ml, mt, mr, mb, sl, st, sr, sb) == true)
			{
				if (f->GetState() == BAT_STATE_IDLE && f->IsActivate() == true)
					f->SetState(BAT_STATE_FLYING);
			}
		}
		if (dynamic_cast<CGhost*>(obj)) // if e->obj is Item Heart 
		{
			CGhost* f = dynamic_cast<CGhost*>(obj);
			if (this->AABB(obj) == true) // if e->obj is Item Heart 
			{
				if (untouchable_start == 0 && f->state != GHOST_STATE_IDLE && !f->isDestroy)
				{
					if (!this->isOnStair)
					{
						this->SetState(SIMON_STATE_HURT);
					}
					else
						this->SetHp();

					if (untouchable != 1) {
						StartUntouchable();
						break; // không xét tiếp va chạm khi defect
					}
				}
			}
		}
		if (dynamic_cast<CMonkey*>(obj)) // if e->obj is Item Heart 
		{
			CMonkey* f = dynamic_cast<CMonkey*>(obj);
			if (this->AABB(obj) == true) // if e->obj is Item Heart 
			{
				if (untouchable_start == 0 && f->state != MONKEY_STATE_IDLE && !f->isDestroy) {

					if (!this->isOnStair)
					{
						this->SetState(SIMON_STATE_HURT);
					}
					else
						this->SetHp();

					if (untouchable != 1) {
						StartUntouchable();
						break; // không xét tiếp va chạm khi defect
					}
				}
			}
		}
		if (dynamic_cast<CBone*>(obj)) // if e->obj is Item Heart 
		{
			CBone* f = dynamic_cast<CBone*>(obj);
			if (this->AABB(obj) == true) // if e->obj is Item Heart 
			{
				if (untouchable_start == 0) {

					if (!this->isOnStair)
					{
						this->SetState(SIMON_STATE_HURT);
					}
					else
						this->SetHp();

					if (untouchable != 1) {
						StartUntouchable();
						break; // không xét tiếp va chạm khi defect
					}
				}
			}
		}
		if (dynamic_cast<CBoss*>(obj)) // if e->obj is Item Heart 
		{
			CBoss* f = dynamic_cast<CBoss*>(obj);
			if (this->AABB(obj) == true) // if e->obj is Item Heart 
			{
				if (untouchable_start == 0) {

					if (!this->isOnStair)
					{
						this->SetState(SIMON_STATE_HURT);
					}
					else
						this->SetHp();

					if (untouchable != 1) {
						StartUntouchable();
						break; // không xét tiếp va chạm khi defect
					}
				}
			}
		}
		if (dynamic_cast<CBrick*>(obj)) {
			CBrick* e = dynamic_cast<CBrick*>(obj);

			float l, t, r, b;
			float ml, mt, mr, mb;
			this->GetBoundingBox(l, t, r, b);
			e->GetBoundingBox(ml, mt, mr, mb);
			mt = mt - 2;

			if (CGame::AABB(l, t, r, b, ml, mt, mr, mb))
			{
				colGround = true;
			}
		}
		if (dynamic_cast<CBreakWall*>(obj)) {
			CBreakWall* e = dynamic_cast<CBreakWall*>(obj);

			float l, t, r, b;
			float ml, mt, mr, mb;
			this->GetBoundingBox(l, t, r, b);
			e->GetBoundingBox(ml, mt, mr, mb);
			mt = mt - 2;

			if (CGame::AABB(l, t, r, b, ml, mt, mr, mb))
			{
				colGround = true;
			}
		}
		if (dynamic_cast<CBridge*>(obj)) {
			float l, t, r, b;
			float ml, mt, mr, mb;
			this->GetBoundingBox(l, t, r, b);
			obj->GetBoundingBox(ml, mt, mr, mb);
			mt = mt - 2;

			if (CGame::AABB(l, t, r, b, ml, mt, mr, mb))
			{
				colGround = true;
				x += obj->dx * 1;
			}
		}
	}

	if (colGround)
	{
		if (!colBrickSweptAABB)
		{
			this->isGround = true;
		}
	}
	else
	{
		this->isGround = false;
	}

	if (this->attack_start != 0)
	{
		if (!this->isSubWeapon)
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
	//RenderBoundingBox();s
	string ani;
	if (state == SIMON_STATE_IDLE) {
		if (nx > 0) ani = "simon_ani_idle";
		else ani = "simon_ani_idle";
	}
	else if (state == SIMON_STATE_AUTO_WALKING)
		ani = "simon_ani_walking";
	else if (state == SIMON_STATE_WALKING_RIGHT || state == SIMON_STATE_WALKING_LEFT)
		ani = "simon_ani_walking";
	else if (state == SIMON_STATE_STAIR_UP_IDLE)
		ani = "simon_ani_stair_up_idle";
	else if (state == SIMON_STATE_STAIR_UP_RIGHT || state == SIMON_STATE_STAIR_UP_LEFT)
		ani = "simon_ani_stair_up";
	else if (state == SIMON_STATE_STAIR_DOWN_IDLE)
		ani = "simon_ani_stair_down_idle";
	else if (state == SIMON_STATE_STAIR_DOWN_RIGHT || state == SIMON_STATE_STAIR_DOWN_LEFT)
		ani = "simon_ani_stair_down";
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
	else if (state == SIMON_STATE_HURT)
		ani = "simon_ani_hurt";
	else if (state == SIMON_STATE_ATTACK_STAIR_UP)
		ani = "simon_ani_attack_stair_up";
	else if (state == SIMON_STATE_ATTACK_STAIR_DOWN)
		ani = "simon_ani_attack_stair_down";
	else if (state == SIMON_STATE_DIE)
		ani = "simon_ani_die";

	int alpha = 255;
	if (untouchable) alpha = 128;
	animations[ani]->Render(nx, x, y, alpha);

	if (this->attack_start != 0 && !this->isSubWeapon)
		this->whip->Render();
}

void CSimon::SetState(int state)
{
	switch (state)
	{
	case SIMON_STATE_IDLE:
		this->whip->SetState(WHIP_STATE_IDLE);
		this->subWeapon->SetIsFight(false);
		vx = 0;
		break;
	case SIMON_STATE_AUTO_WALKING:
		this->whip->SetState(WHIP_STATE_IDLE);
		this->subWeapon->SetIsFight(false);
		nx = 1;
		vx = SIMON_WALKING_SPEED / 2;
		break;
	case SIMON_STATE_WALKING_RIGHT:
		this->whip->SetState(WHIP_STATE_IDLE);
		this->subWeapon->SetIsFight(false);
		if (this->isAutoWalk) {
			vx = SIMON_WALKING_STAIR_SPEED;
		}
		else
			vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		this->whip->SetState(WHIP_STATE_IDLE);
		this->subWeapon->SetIsFight(false);
		if (this->isAutoWalk) {
			vx = -SIMON_WALKING_STAIR_SPEED;
		}
		else
			vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		this->whip->SetState(WHIP_STATE_IDLE);
		this->subWeapon->SetIsFight(false);

		vy = -SIMON_JUMP_SPEED_Y;
		break;
	case SIMON_STATE_SIT:
		this->whip->SetState(WHIP_STATE_IDLE);
		this->subWeapon->SetIsFight(false);
		vx = 0;
		vy = 0;
		break;
	case SIMON_STATE_STAIR_UP_IDLE:
		this->isFirstStepOnStair = true;
		this->isStartOnStair = false; // cho phép nhấn tiếp
		isOnStair = true;
		//tránh trường hợp khi simon attack y thay đổi làm floor
		// làm tròn xuống 1px
		if (nx == 1)
		{
			this->onStairDirection = STAIRDIRECTION::UPRIGHT;
		}
		else if (nx == -1)
		{
			this->onStairDirection = STAIRDIRECTION::UPLEFT;
		}
		if (this->state != SIMON_STATE_ATTACK_STAIR_DOWN && this->state != SIMON_STATE_ATTACK_STAIR_UP)
			this->lastStepOnStairPos = { floor(this->x),floor(this->y) };
		vx = 0;
		vy = 0;
		break;
	case SIMON_STATE_STAIR_UP_RIGHT:
		isOnStair = true;
		vy = -SIMON_WALKING_STAIR_SPEED;
		vx = SIMON_WALKING_STAIR_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_STAIR_DOWN_RIGHT:
		isOnStair = true;
		vy = SIMON_WALKING_STAIR_SPEED;
		vx = SIMON_WALKING_STAIR_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_STAIR_DOWN_IDLE:
		isOnStair = true;
		this->isFirstStepOnStair = true;
		this->isStartOnStair = false; // cho phép nhấn tiếp
		//tránh trường hợp khi simon attack y thay đổi làm floor
		// làm tròn xuống 1px
		if (nx == 1)
		{
			this->onStairDirection = STAIRDIRECTION::DOWNRIGHT;
		}
		else if (nx == -1)
		{
			this->onStairDirection = STAIRDIRECTION::DOWNLEFT;
		}
		if (this->state != SIMON_STATE_ATTACK_STAIR_DOWN && this->state != SIMON_STATE_ATTACK_STAIR_UP)
			this->lastStepOnStairPos = { floor(this->x),floor(this->y) };
		vx = 0;
		vy = 0;
		break;
	case SIMON_STATE_STAIR_UP_LEFT:
		isOnStair = true;
		vy = -SIMON_WALKING_STAIR_SPEED;
		vx = -SIMON_WALKING_STAIR_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_STAIR_DOWN_LEFT:
		isOnStair = true;
		vy = SIMON_WALKING_STAIR_SPEED;
		vx = -SIMON_WALKING_STAIR_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_ITEM:
		this->ResetAttack();
		this->update_start = GetTickCount();
		this->whip->SetState(WHIP_STATE_IDLE);
		this->subWeapon->SetIsFight(false);
		vx = 0;
		isOnStair = false;
		break;
	case SIMON_STATE_SIT_ATTACK:
		vx = 0;
		if (isSubWeapon == true)
		{
			this->whip->ResetAttack();
			this->whip->SetState(WHIP_STATE_FIGHT);
			this->subWeapon->SetIsFight(true);
		}
		else
		{
			this->whip->ResetAttack();
			this->whip->SetState(WHIP_STATE_FIGHT);
			this->subWeapon->SetIsFight(false);
		}
		this->attack_start = GetTickCount();
		break;
	case SIMON_STATE_STAND_ATTACK:
		vx = this->state == SIMON_STATE_IDLE || this->state == SIMON_STATE_WALKING_LEFT
			|| this->state == SIMON_STATE_WALKING_RIGHT ? 0 : vx;
		if (isSubWeapon == true)
		{
			this->whip->ResetAttack();
			this->whip->SetState(WHIP_STATE_FIGHT);
			this->subWeapon->SetIsFight(true);
		}
		else
		{
			this->whip->ResetAttack();
			this->whip->SetState(WHIP_STATE_FIGHT);
			this->subWeapon->SetIsFight(false);
		}
		this->attack_start = GetTickCount();
		break;
	case SIMON_STATE_HURT:
		this->ResetAttack();
		this->attack_start = GetTickCount();
		this->vy = -SIMON_HURT_SPEED_Y;
		this->SetHp();
		if (this->nx == 1)
		{
			this->vx = -SIMON_HURT_SPEED_X;
		}
		else
		{
			this->vx = SIMON_HURT_SPEED_X;
		}
		break;
	case SIMON_STATE_ATTACK_STAIR_UP:
	case SIMON_STATE_ATTACK_STAIR_DOWN:
		this->isOnStair = true;
		this->isFirstStepOnStair = true;
		this->vx = 0;
		this->vy = 0;
		if (isSubWeapon == true)
		{
			this->whip->ResetAttack();
			this->whip->SetState(WHIP_STATE_FIGHT);
			this->subWeapon->SetIsFight(true);
		}
		else
		{
			this->whip->ResetAttack();
			this->whip->SetState(WHIP_STATE_FIGHT);
			this->subWeapon->SetIsFight(false);
		}
		this->attack_start = GetTickCount();
		break;
	case SIMON_STATE_DIE:
		vx = vy = 0;
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

void CSimon::HandleFirstStepOnStair()
{
	if (this->onStairDirection == STAIRDIRECTION::DEFAULT)
		return;

	this->vx = 0;
	this->vy = 0;

	if (this->state == SIMON_STATE_WALKING_LEFT) {
		int a = 2;
	}

	if (isUpStair)
	{// up right
		if (this->onStairDirection == STAIRDIRECTION::UPRIGHT || this->onStairDirection == STAIRDIRECTION::UPLEFTRIGHT) {
			if (stairPos.x - this->x > SIMON_DISTANCE_UPRIGHT) {
				this->isAutoWalk = true;
				SetState(SIMON_STATE_WALKING_RIGHT);
				return;
			}
			else if (stairPos.x - this->x < SIMON_DISTANCE_UPRIGHT - 5) {
				this->isAutoWalk = true;
				SetState(SIMON_STATE_WALKING_LEFT);
				return;
			}
			else {
				if (state == SIMON_STATE_WALKING_LEFT) {
					if (nx == -1) nx = 1;
					else if (nx == 1) nx = -1;
				}
				this->isAutoWalk = false;
				this->isOnStair = true;
				this->isFirstStepOnStair = true;
				this->lastStepOnStairPos = { floor(this->x),floor(this->y) };
				//DebugOut(L"Step x=%f y=%f \n", this->LastStepOnStairPos.x, this->LastStepOnStairPos.y);
				this->SetState(SIMON_STATE_STAIR_UP_RIGHT);
			}
		}
		else if (this->onStairDirection == STAIRDIRECTION::UPLEFT) {
			if (stairPos.x - this->x < SIMON_DISTANCE_UPLEFT) {
				this->isAutoWalk = true;
				SetState(SIMON_STATE_WALKING_LEFT);
				return;
			}
			else if (stairPos.x - this->x > SIMON_DISTANCE_UPLEFT + 5) {
				this->isAutoWalk = true;
				SetState(SIMON_STATE_WALKING_RIGHT);
				return;
			}
			else {
				if (state == SIMON_STATE_WALKING_RIGHT) {
					if (nx == -1) nx = 1;
					else if (nx == 1) nx = -1;
				}
				this->isAutoWalk = false;
				this->isOnStair = true;
				this->isFirstStepOnStair = true;
				this->lastStepOnStairPos = { floor(this->x),floor(this->y) };
				//DebugOut(L"Step x=%f y=%f \n", this->LastStepOnStairPos.x, this->LastStepOnStairPos.y);
				this->SetState(SIMON_STATE_STAIR_UP_LEFT);
			}
		}
	}
	else
	{
		if (this->onStairDirection == STAIRDIRECTION::UPLEFTRIGHT) {
			if (stairPos.x - this->x > SIMON_DISTANCE_UPLEFTRIGHT) {
				this->isAutoWalk = true;
				SetState(SIMON_STATE_WALKING_RIGHT);
				return;
			}
			else if (stairPos.x - this->x < SIMON_DISTANCE_UPLEFTRIGHT - 5) {
				this->isAutoWalk = true;
				SetState(SIMON_STATE_WALKING_LEFT);
				return;
			}
			else {
				if (state == SIMON_STATE_WALKING_LEFT) {
					if (nx == -1) nx = 1;
					else if (nx == 1) nx = -1;
				}
				this->isAutoWalk = false;
				this->isOnStair = true;
				this->isFirstStepOnStair = true;
				this->lastStepOnStairPos = { floor(this->x),floor(this->y) };
				//DebugOut(L"Step x=%f y=%f \n", this->LastStepOnStairPos.x, this->LastStepOnStairPos.y);
				this->SetState(SIMON_STATE_STAIR_DOWN_RIGHT);
			}
		}
		else if (this->onStairDirection == STAIRDIRECTION::DOWNRIGHT) {
			if (stairPos.x - this->x > SIMON_DISTANCE_DOWNRIGHT) {
				this->isAutoWalk = true;
				SetState(SIMON_STATE_WALKING_RIGHT);
				return;
			}
			else if (stairPos.x - this->x < SIMON_DISTANCE_DOWNRIGHT - 5) {
				this->isAutoWalk = true;
				SetState(SIMON_STATE_WALKING_LEFT);
				return;
			}
			else {
				if (state == SIMON_STATE_WALKING_LEFT) {
					if (nx == -1) nx = 1;
					else if (nx == 1) nx = -1;
				}
				this->isAutoWalk = false;
				this->isOnStair = true;
				this->isFirstStepOnStair = true;
				this->lastStepOnStairPos = { floor(this->x),floor(this->y) };
				//DebugOut(L"Step x=%f y=%f \n", this->LastStepOnStairPos.x, this->LastStepOnStairPos.y);
				this->SetState(SIMON_STATE_STAIR_DOWN_RIGHT);
			}
		}
		else if (this->onStairDirection == STAIRDIRECTION::DOWNLEFT) {
			if (stairPos.x - this->x < SIMON_DISTANCE_DOWNLEFT) {
				this->isAutoWalk = true;
				SetState(SIMON_STATE_WALKING_LEFT);
				return;
			}
			else if (stairPos.x - this->x > SIMON_DISTANCE_DOWNLEFT + 5) {
				this->isAutoWalk = true;
				SetState(SIMON_STATE_WALKING_RIGHT);
				return;
			}
			else {
				if (state == SIMON_STATE_WALKING_RIGHT) {
					if (nx == -1) nx = 1;
					else if (nx == 1) nx = -1;
				}
				this->isAutoWalk = false;
				this->isOnStair = true;
				this->isFirstStepOnStair = true;
				this->lastStepOnStairPos = { floor(this->x),floor(this->y) };
				//DebugOut(L"Step x=%f y=%f \n", this->LastStepOnStairPos.x, this->LastStepOnStairPos.y);
				this->SetState(SIMON_STATE_STAIR_DOWN_LEFT);
			}
		}
	}
}

void CSimon::HandlePerStepOnStair()
{
	if (this->state == SIMON_STATE_STAIR_UP_RIGHT)
	{
		if (this->x - lastStepOnStairPos.x > SIMON_DISTANCE_STEP) {
			if (lastStepOnStairPos.y - this->y > SIMON_DISTANCE_STEP) {
				this->x = lastStepOnStairPos.x + SIMON_DISTANCE_STEP;
				this->y = lastStepOnStairPos.y - SIMON_DISTANCE_STEP;
				this->SetState(SIMON_STATE_STAIR_UP_IDLE);
				this->isOnStair = true;
				//DebugOut(L" x=%f y=%f \n", this->x, this->y);
			}
		}
	}
	else if (this->state == SIMON_STATE_STAIR_UP_LEFT)
	{
		if (this->x - lastStepOnStairPos.x < SIMON_DISTANCE_STEP) {
			if (lastStepOnStairPos.y - this->y > SIMON_DISTANCE_STEP) {
				this->x = lastStepOnStairPos.x - SIMON_DISTANCE_STEP;
				this->y = lastStepOnStairPos.y - SIMON_DISTANCE_STEP;
				this->SetState(SIMON_STATE_STAIR_UP_IDLE);
				this->isOnStair = true;

			}
		}
	}
	if (this->state == SIMON_STATE_STAIR_DOWN_RIGHT)
	{
		if (this->x - lastStepOnStairPos.x > SIMON_DISTANCE_STEP) {
			if (lastStepOnStairPos.y - this->y < SIMON_DISTANCE_STEP) {
				this->x = lastStepOnStairPos.x + SIMON_DISTANCE_STEP;
				this->y = lastStepOnStairPos.y + SIMON_DISTANCE_STEP;
				this->SetState(SIMON_STATE_STAIR_DOWN_IDLE);
				this->isOnStair = true;
				//DebugOut(L" x=%f y=%f \n", this->x, this->y);
			}
		}
	}
	else if (this->state == SIMON_STATE_STAIR_DOWN_LEFT)
	{
		if (lastStepOnStairPos.x - this->x > SIMON_DISTANCE_STEP) {
			if (lastStepOnStairPos.y - this->y < SIMON_DISTANCE_STEP) {
				this->x = lastStepOnStairPos.x - SIMON_DISTANCE_STEP;
				this->y = lastStepOnStairPos.y + SIMON_DISTANCE_STEP;
				this->SetState(SIMON_STATE_STAIR_DOWN_IDLE);
				this->isOnStair = true;
			}
		}
	}
}

bool CSimon::AutoWalk(float step)
{
	if (this->x > step) {
		this->SetState(SIMON_STATE_IDLE);
		return true;
	}
	return false;
}



