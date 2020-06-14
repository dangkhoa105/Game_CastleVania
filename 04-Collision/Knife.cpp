#include "Knife.h"
#include "Candle.h"
#include "SpearGuard.h"

void CKnife::Render()
{
	if (isDestroy)
	{
		return;
	}
	if (state == KNIFE_STATE_FIGHT)
	{
		animations["item_knife"]->Render(-nx, x, y);
	}

	//RenderBoundingBox();
}

void CKnife::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + KNIFE_BBOX_WIDTH;
	bottom = top + KNIFE_BBOX_HEIGHT;
}

void CKnife::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{

	CGameObject::Update(dt);

	if (nx > 0)
	{
		vx = KNIFE_VX;
	}
	else
	{
		vx = -KNIFE_VX;
	}

	x += dx;
	y += dy;
	for (UINT i = 0; i < colliable_objects->size(); i++)
	{
		LPGAMEOBJECT obj = colliable_objects->at(i);

		if (dynamic_cast<CCandle*>(obj))
		{
			CCandle* e = dynamic_cast<CCandle*> (obj);

			if (this->AABB(obj) == true)
			{
				if (e->GetState() != CANDLE_STATE_DESTROYED)
				{
					e->SetState(CANDLE_STATE_DESTROYED);
					e->SetDestroy(true);
					e->SetFall(true);
				}
			}
		}
		if (dynamic_cast<CSpearGuard*>(obj))
		{
			CSpearGuard* e = dynamic_cast<CSpearGuard*> (obj);

			if (this->AABB(obj) == true)
			{
				if (e->GetState() != SPEAR_GUARD_STATE_DIE)
				{
					e->SetState(SPEAR_GUARD_STATE_DIE);
					e->SetDestroy(true);
					e->SetFall(true);
					/*e->effect->SetState(EFFECT);
					e->effect->SetPosition(e->x, e->y);*/
				}
			}
		}
	}

}

void CKnife::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KNIFE_STATE_FIGHT:
		/*vx = nx > 0 ? this->vx = KNIFE_VX : this->vx = -KNIFE_VX;*/
		break;
	}
}
