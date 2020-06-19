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
	if (nx == 1)
	{
		vx = BONE_SPEED_VX;
	}
	else if (nx == -1)
	{
		vx = -BONE_SPEED_VX;
	}

	vy += BONE_GRAVITY * dt;

	x += dx;
	y += dy;
}

void CBone::SetState(int state)
{
	CGameObject::SetState(state);
}
