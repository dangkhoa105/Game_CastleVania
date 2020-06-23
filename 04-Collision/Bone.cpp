#include "Bone.h"

void CBone::Render()
{
	animations["bone_ani"]->Render(-nx, x, y);
	//RenderBoundingBox();
}

void CBone::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BONE_BBOX_WIDTH;
	bottom = top + BONE_BBOX_HEIGHT;
}

void CBone::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);// bắt đầu tính toán va chạm

	if (nx == 1)
	{
		vx = BONE_SPEED_VX / 2;
	}
	else if (nx == -1)
	{
		vx = -BONE_SPEED_VX / 2;
	}

	vy += BONE_GRAVITY * dt;

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		x += dx;
		y += dy;
	}
}

void CBone::SetState(int state)
{
	CGameObject::SetState(state);
}
