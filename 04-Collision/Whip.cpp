#include "Whip.h"

void CWhip::Render()
{
	string ani;
	if (state == WHIP_STATE_FIGHT)
	{
		if (this->level == 1) 
			ani = "whip_ani";
		else if (this->level == 2) 
			ani = "whip_ani_level_2";
		else 
			ani = "whip_ani_level_3";
		animations[ani]->Render(nx, x, y);

		//	RenderBoundingBox();
	}
}

void CWhip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (this->level == 1 || this->level == 2)
	{
		if (nx == 1)
		{
			left = x + 140;
			top = y + 15;
			right = left + 55;
			bottom = top + 18;
		}
		else
		{
			left = x + 50;
			top = y + 15;
			right = left + 55;
			bottom = top + 18;
		}
	}
	else
	{
		if (nx == 1)
		{
			left = x + 140;
			top = y + 15;
			right = left + 85;
			bottom = top + 18;
		}
		else
		{
			left = x + 25;
			top = y + 15;
			right = left + 85;
			bottom = top + 18;
		}
	}
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
}

void CWhip::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case WHIP_STATE_FIGHT:
		break;
	}
}

int CWhip::GetState()
{
	return state;
}
