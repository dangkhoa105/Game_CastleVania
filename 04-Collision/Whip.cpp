#include "Whip.h"
#include "Candle.h"
#include "Effect.h"
#include "Heart.h"

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
		this->box = animations[ani]->GetOver();

		RenderBoundingBox();
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
	if (this->GetBox() == true)
	{
		for (UINT i = 0; i < colliable_objects->size(); i++)
		{
			LPGAMEOBJECT obj = colliable_objects->at(i);

			if (dynamic_cast<CCandle*>(obj))
			{
				CCandle* e = dynamic_cast<CCandle*> (obj);

				if (this->AABBx(obj) == true)
				{
					if (e->GetState() != CANDLE_STATE_DESTROYED)
					{
						e->SetState(CANDLE_STATE_DESTROYED);
						e->SetFall(true);
						e->effect->SetState(EFFECT);
						e->effect->SetPosition(e->x, e->y);
						for (UINT j = i + 1; j < colliable_objects->size(); j++) {
							LPGAMEOBJECT obj_1 = colliable_objects->at(j);
							if (dynamic_cast<Heart*>(obj_1)) {
								if (obj_1->GetId() == e->GetId()) {
									obj_1->SetFall(e->GetFall());
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	this->box = false;
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
