#include "Bridge.h"
#include "Brick.h"

void CBridge::Render()
{
	animations["bridge"]->Render(nx, x, y);
}

void CBridge::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRIDGE_BBOX_WIDTH;
	b = y + BRIDGE_BBOX_HEIGHT;
}

void CBridge::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(colliable_objects, coEvents);

	

	//if (vx < 0 && x < this->beginPositionX) {
	//	x = beginPositionX;
	//	vx = -vx;
	//	nx = 1;
	//}

	//if (vx > 0 && x > lastPositionX) {
	//	x = lastPositionX;
	//	vx = vx;
	//	nx = -1;
	//}

	if (coEvents.size() == 0)
	{
		x += dx;
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
				this->vx = -vx;
			}
			else
			{
				if (nx != 0)
				{
					x += dx;
					y += dy;
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

