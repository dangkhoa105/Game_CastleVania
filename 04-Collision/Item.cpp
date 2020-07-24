#include "Item.h"
#include "Brick.h"

void CItem::Render()
{
	if (state!= ID_IMONEYBAG_SPECIAL && state!= ID_ICROWN)
	{
		if (wait_time != 0 && GetTickCount() - wait_time < 500)
		{
			return;
		}
	}
	
	switch (idItem)
	{
	case ID_IHEART:
		animations["item_heart"]->Render(x, y);
		break;
	case ID_SMALL_IHEART:
		animations["item_small_heart"]->Render(x, y);
		break;
	case ID_IKNIFE:
		animations["item_knife"]->Render(x, y);
		break;
	case ID_IWHIP:
		animations["item_whip"]->Render(x, y);
		break;
	case ID_IMONEYBAG_SPECIAL:
		animations["item_money_bag_special"]->Render(x, y);
		break;
	case ID_IMONEYBAG_400:
		animations["item_money_bag_400"]->Render(x, y);
		break;
	case ID_IMONEYBAG_700:
		animations["item_money_bag_700"]->Render(x, y);
		break;
	case ID_IMONEYBAG_1000:
		animations["item_money_bag_1000"]->Render(x, y);
		break;
	case ID_IBOOMERANG:
		animations["item_boomerang"]->Render(x, y);
		break;
	case ID_IAXE:
		animations["item_axe"]->Render(x, y);
		break;
	case ID_ICLOCK:
		animations["clock_ani"]->Render(x, y);
		break;
	case ID_IGAS:
		animations["item_gas"]->Render(x, y);
		break;
	case ID_ICROWN:
		animations["item_crown"]->Render(x, y);
		break;
	case ID_IDOUBLESHOT:
		animations["item_double_shot"]->Render(x, y);
		break;
	case ID_IKFC:
		animations["item_kfc"]->Render(x, y);
		break;
	case ID_IPOTION:
		animations["item_potion"]->Render(x, y);
		break;
	default:
		break;
	}

}

void CItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	switch (idItem)
	{
	case ID_IHEART:
		r = x + IHEART_BBOX_WIDTH;
		b = y + IHEART_BBOX_HEIGHT;
		break;
	case ID_SMALL_IHEART:
		r = x + ISMALLHEART_BBOX_WIDTH;
		b = y + ISMALLHEART_BBOX_HEIGHT;
		break;
	case ID_IKNIFE:
		r = x + IKNIFE_BBOX_WIDTH;
		b = y + IKNIFE_BBOX_HEIGHT;
		break;
	case ID_IWHIP:
		r = x + IWHIP_BBOX_WIDTH;
		b = y + IWHIP_BBOX_HEIGHT;
		break;
	case ID_IMONEYBAG_SPECIAL:
		r = x + IMONEYBAG_BBOX_WIDTH;
		b = y + IMONEYBAG_BBOX_HEIGHT;
		break;
	case ID_IMONEYBAG_400:
		r = x + IMONEYBAG_BBOX_WIDTH;
		b = y + IMONEYBAG_BBOX_HEIGHT;
		break;
	case ID_IMONEYBAG_700:
		r = x + IMONEYBAG_BBOX_WIDTH;
		b = y + IMONEYBAG_BBOX_HEIGHT;
		break;
	case ID_IMONEYBAG_1000:
		r = x + IMONEYBAG_BBOX_WIDTH;
		b = y + IMONEYBAG_BBOX_HEIGHT;
		break;
	case ID_IBOOMERANG:
		r = x + IBOOMERANG_BBOX_WIDTH;
		b = y + IBOOMERANG_BBOX_HEIGHT;
		break;
	case ID_IAXE:
		r = x + IAXE_BBOX_WIDTH;
		b = y + IAXE_BBOX_HEIGHT;
		break;
	case ID_ICLOCK:
		r = x + ICLOCK_BBOX_WIDTH;
		b = y + ICLOCK_BBOX_HEIGHT;
		break;
	case ID_IGAS:
		r = x + IGAS_BBOX_WIDTH;
		b = y + IGAS_BBOX_HEIGHT;
		break;
	case ID_ICROWN:
		r = x + ICROWN_BBOX_WIDTH;
		b = y + ICROWN_BBOX_HEIGHT;
		break;
	case ID_IDOUBLESHOT:
		r = x + IDOUBLESHOT_BBOX_WIDTH;
		b = y + IDOUBLESHOT_BBOX_HEIGHT;
		break;
	case ID_IKFC:
		r = x + IKFC_BBOX_WIDTH;
		b = y + IKFC_BBOX_HEIGHT;
		break;
	case ID_IPOTION:
		r = x + IPOTION_BBOX_WIDTH;
		b = y + IPOTION_BBOX_HEIGHT;
		break;
	default:
		break;
	}
}

void CItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state != ID_IMONEYBAG_SPECIAL && state != ID_ICROWN)
	{
		if (wait_time != 0 && GetTickCount() - wait_time < 500)
		{
			return;
		}
	}
	CGameObject::Update(dt);

	vy += GRAVITY_ITEM * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);// bắt đầu tính toán va chạm

	if (this->idItem == ID_SMALL_IHEART)
	{
		x = 10 * cos(y * 0.07) + this->positionInitX;
	}

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is Brick 
			{
				vy = 0;
			}
		}

	}
}

void CItem::SetState(int state)
{
	CGameObject::SetState(state);
}
