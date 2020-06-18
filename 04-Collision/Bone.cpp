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
	right = left + 32;
	bottom = top + 32;
}

void CBone::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
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
}

void CBone::SetState(int state)
{
	CGameObject::SetState(state);
}
