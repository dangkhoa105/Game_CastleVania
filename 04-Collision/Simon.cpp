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

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
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
					if (this->isOnStair) {
						x += dx;
						y += dy;
					}
					else if (this->state == SIMON_STATE_HURT) 
					{
						this->state = SIMON_STATE_IDLE;
						this->isGround = true;
					}
					else
					{
						this->isGround = true;
						if (ny != 0) vy = 0;
					}

					if (this->state == SIMON_STATE_SIT_ATTACK || this->state == SIMON_STATE_STAND_ATTACK)
						vx = 0;

					isGround = true;

					if (this->state != SIMON_STATE_AUTO_WALKING)
						if (nx != 0) vx = 0;

					if (ny != 0) vy = 0;
				}
				else if (e->ny > 0 && this->vy < 0) {
					y += dy;
					if (nx != 0) vx = 0;
				}
				else if (e->nx != 0) {
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
					case ID_SMALL_IHEART:
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
					case ID_IBOOMERANG:
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
				else if (dynamic_cast<CSpearGuard*>(e->obj))
				{
					CSpearGuard* spearGuard = dynamic_cast<CSpearGuard*>(e->obj);
					if (untouchable_start == 0) {

						if (!this->isOnStair)
						{
							this->SetState(SIMON_STATE_HURT);
							x += dx;
							y += dy;
						}
						if (untouchable != 1) {
							StartUntouchable();
							break; // không xét tiếp va chạm khi defect
						}
					}
					else {
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
				else if (dynamic_cast<CBat*>(e->obj))
				{									
					if (untouchable_start == 0) {

						if (!this->isOnStair)
						{
							this->SetState(SIMON_STATE_HURT);
							x += dx;
							y += dy;
						}
						if (untouchable != 1) {
							StartUntouchable();
							break; // không xét tiếp va chạm khi defect
						}
					}
					else {
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
				case ID_SMALL_IHEART:
					break;
				case ID_IKNIFE:
					subWeapon = SUBWEAPON::KNIFE;
					break;
				case ID_IWHIP:
					this->SetState(SIMON_STATE_ITEM);
					this->level += 1;
					this->whip->SetLevel(level);
					break;
				case ID_IBOOMERANG:
					this->SetState(SIMON_STATE_ITEM);
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
				if (!this->isColliWithStair)
				{
					if (this->isOnStair)
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
			if (this->AABB(obj) == true)
			{
				if (untouchable_start == 0) {

					if (!this->isOnStair)
					{
						this->SetState(SIMON_STATE_HURT);
						x += dx;
						y += dy;
					}
					if (untouchable != 1) {
						StartUntouchable();
						break; // không xét tiếp va chạm khi defect
					}
				}
				else if (obj->nx == 0)
					y += dy;
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

	int alpha = 255;
	if (untouchable) alpha = 128;
	animations[ani]->Render(nx, x, y, alpha);

	if (this->attack_start != 0 && !this->isKnife)
		this->whip->Render();

	
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
		if (this->isAutoWalk) {
			vx = SIMON_WALKING_STAIR_SPEED;
		}
		else 
			vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		this->whip->SetState(WHIP_STATE_IDLE);
		this->knife->SetState(KNIFE_STATE_IDLE);
		if (this->isAutoWalk) {
			vx = -SIMON_WALKING_STAIR_SPEED;
		}
		else
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
	case SIMON_STATE_STAIR_UP_IDLE:
		this->isFirstStepOnStair = true; 
		this->isStartOnStair = false; // cho phép nhấn tiếp
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
		vy = -SIMON_WALKING_STAIR_SPEED;
		vx = SIMON_WALKING_STAIR_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_STAIR_DOWN_RIGHT:
		vy = SIMON_WALKING_STAIR_SPEED;
		vx = SIMON_WALKING_STAIR_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_STAIR_DOWN_IDLE:
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
		vy = -SIMON_WALKING_STAIR_SPEED;
		vx = -SIMON_WALKING_STAIR_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_STAIR_DOWN_LEFT:		
		vy = SIMON_WALKING_STAIR_SPEED;
		vx = -SIMON_WALKING_STAIR_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_ITEM:
		this->ResetAttack();
		this->update_start = GetTickCount();
		this->whip->SetState(WHIP_STATE_IDLE);
		this->knife->SetState(KNIFE_STATE_IDLE);
		vx = 0;
		isOnStair = false;
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
	case SIMON_STATE_HURT:
		this->ResetAttack();
		this->update_start = GetTickCount();
		this->vy = -SIMON_HURT_SPEED_Y;
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

void CSimon::HandleFirstStepOnStair()
{
	if (this->onStairDirection == STAIRDIRECTION::DEFAULT)
		return;

	this->vx = 0;
	this->vy = 0;

	if (this->state == SIMON_STATE_WALKING_LEFT) {
		int a = 2;
	}

	// up right
	if (this->onStairDirection == STAIRDIRECTION::UPRIGHT) {
		if (stairPos.x - this->x > 20) {
			this->isAutoWalk = true;
			SetState(SIMON_STATE_WALKING_RIGHT);
			return;
		}
		else if (stairPos.x - this->x < 20 - 5) {
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
		if (stairPos.x - this->x < 36) {
			this->isAutoWalk = true;
			SetState(SIMON_STATE_WALKING_LEFT);
			return;
		}
		else if (stairPos.x - this->x > 36 + 5) {
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
			this->SetState(SIMON_STATE_STAIR_UP_RIGHT);
		}
	}
	else if (this->onStairDirection == STAIRDIRECTION::DOWNRIGHT) {
		if (stairPos.x - this->x > 50) {
			this->isAutoWalk = true;
			SetState(SIMON_STATE_WALKING_RIGHT);
			return;
		}
		else if (stairPos.x - this->x < 50 + 5) {
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
		if (stairPos.x - this->x < 18) {
			this->isAutoWalk = true;
			SetState(SIMON_STATE_WALKING_LEFT);
			return;
		}
		else if (stairPos.x - this->x > 18 + 5) {
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

void CSimon::HandlePerStepOnStair()
{
	if (this->state == SIMON_STATE_STAIR_UP_RIGHT)
	{
		if (this->x - lastStepOnStairPos.x > 16) {
			if (lastStepOnStairPos.y - this->y > 16) {
				this->x = lastStepOnStairPos.x + 16;
				this->y = lastStepOnStairPos.y - 16;
				this->SetState(SIMON_STATE_STAIR_UP_IDLE);
				this->isOnStair = true;
				//DebugOut(L" x=%f y=%f \n", this->x, this->y);
			}
		}
	}
	else if (this->state == SIMON_STATE_STAIR_UP_LEFT)
	{
		if (this->x - lastStepOnStairPos.x < 16) {
			if (lastStepOnStairPos.y - this->y > 16) {
				this->x = lastStepOnStairPos.x - 16;
				this->y = lastStepOnStairPos.y - 16;
				this->SetState(SIMON_STATE_STAIR_UP_IDLE);
				this->isOnStair = true;
				
			}
		}
	}
	if (this->state == SIMON_STATE_STAIR_DOWN_RIGHT)
	{
		if (this->x - lastStepOnStairPos.x > 16) {
			if (lastStepOnStairPos.y - this->y < 16) {
				this->x = lastStepOnStairPos.x + 16;
				this->y = lastStepOnStairPos.y + 16;
				this->SetState(SIMON_STATE_STAIR_DOWN_IDLE);
				this->isOnStair = true;
				//DebugOut(L" x=%f y=%f \n", this->x, this->y);
			}
		}
	}
	else if (this->state == SIMON_STATE_STAIR_DOWN_LEFT)
	{
		
		if (lastStepOnStairPos.x -this->x > 16) {

			if (lastStepOnStairPos.y - this->y < 16) {
				this->x = lastStepOnStairPos.x - 16;
				this->y = lastStepOnStairPos.y + 16;
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

void CSimon::CheckCollisionWithActiveEnemy(vector<LPGAMEOBJECT>* listObjects)
{
	float simon_l, simon_t, simon_r, simon_b;

	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);

	for (UINT i = 0; i < listObjects->size(); i++)
	{
		CEnemy* enemy = dynamic_cast<CEnemy*>(listObjects->at(i));
		if (enemy == NULL)
			continue;

		// Không cần xét vùng active nữa khi nó đang active / destroyed
		if (enemy->GetState() == 0)
			continue;

		float enemy_l, enemy_t, enemy_r, enemy_b;
		enemy->GetBoundingBoxActive(enemy_l, enemy_t, enemy_r, enemy_b);

		if (CGame::AABB(simon_l, simon_t, simon_r, simon_b, enemy_l, enemy_t, enemy_r, enemy_b) == true)
		{
			D3DXVECTOR2 enemyEntryPostion = enemy->GetEntryPosition();
			int tempNx = x < enemy->x ? -1 : 1;
			if (dynamic_cast<CBat*>(enemy))
			{
				CBat* bat = dynamic_cast<CBat*>(enemy);

				if (bat->GetState() == BAT_STATE_IDLE && bat->IsActivate() == true)
					bat->SetState(BAT_STATE_FLYING);
				bat->SetNx(tempNx);
			}
		}
	}
}


