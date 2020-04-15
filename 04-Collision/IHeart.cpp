#include "IHeart.h"
#include "Brick.h"

void CIHeart::Render()
{	
	animations["item_heart"]->Render(x, y);
}

void CIHeart::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + IHEART_BBOX_WIDTH;
	b = y + IHEART_BBOX_HEIGHT;
}

void CIHeart::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt, colliable_objects);

	vy += GRAVITY_ITEM * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	CalcPotentialCollisions(colliable_objects, coEvents);// bắt đầu tính toán va chạm

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else 
	{
		if (this->IsDestroy() == true) 
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
	
}

void CIHeart::SetState(int state)
{
	CGameObject::SetState(state);
}
