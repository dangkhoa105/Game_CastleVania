#include "Item.h"
#include "Brick.h"

void CItem::Render()
{
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
	case ID_IMONEYBAG:
		animations["item_money_bag"]->Render(x, y);
		break;
	case ID_IBOOMERANG:
		animations["item_boomerang"]->Render(x, y);
		break;
	case ID_ICROWN:
		animations["item_crown"]->Render(x, y);
		break;
	case ID_IDOUBLESHOT:
		animations["item_double_shot"]->Render(x, y);
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
	case ID_IMONEYBAG:
		r = x + IMONEYBAG_BBOX_WIDTH;
		b = y + IMONEYBAG_BBOX_HEIGHT;
		break;
	case ID_IBOOMERANG:
		r = x + IBOOMERANG_BBOX_WIDTH;
		b = y + IBOOMERANG_BBOX_HEIGHT;
		break;
	case ID_ICROWN:
		r = x + ICROWN_BBOX_WIDTH;
		b = y + ICROWN_BBOX_HEIGHT;
		break;
	case ID_IDOUBLESHOT:
		r = x + IDOUBLESHOT_BBOX_WIDTH;
		b = y + IDOUBLESHOT_BBOX_HEIGHT;
		break;
	default:
		break;
	}
}

void CItem::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	vy += GRAVITY_ITEM * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);// bắt đầu tính toán va chạm

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
