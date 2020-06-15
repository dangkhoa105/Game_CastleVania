#include "Whip.h"
#include "Candle.h"
#include "Effect.h"
#include "SpearGuard.h"
#include "BreakWall.h"
#include"debug.h"
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
		//	this->box = animations[ani]->GetOver();

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
			bottom = top + 10;
		}
		else
		{
			left = x + 50;
			top = y + 15;
			right = left + 55;
			bottom = top + 10;
		}
	}
	else
	{
		if (nx == 1)
		{
			left = x + 140;
			top = y + 15;
			right = left + 85;
			bottom = top + 10;
		}
		else
		{
			left = x + 25;
			top = y + 15;
			right = left + 85;
			bottom = top + 10;
		}
	}
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	switch (this->level)
	{
	case  1:
		if (animations["whip_ani"]->GetCurrentFrame() < 2)
		{
			return;
		}
		break;
	case  2:
		if (animations["whip_ani_level_2"]->GetCurrentFrame() < 2)
		{
			return;
		}

		break;
	case  3:
		if (animations["whip_ani_level_3"]->GetCurrentFrame() < 8)
		{
			return;
		}
		break;
	default:
		break;
	}


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
		if (dynamic_cast<CEnemy*>(obj))
		{
			if (fight)
			{
				CEnemy* e = dynamic_cast<CEnemy*> (obj);

				if (this->AABB(obj) == true)
				{
					if (!e->isDestroy)
					{
						e->SetFall(true);

						e->TakeDamage(damage);
						if (e->hp == 0)
						{
							e->SetDestroy(true);
						}
						DebugOut(L"Take damage \n");
					}
				}
				fight = false;
			}
		}
		if (dynamic_cast<CBreakWall*>(obj))
		{
			CBreakWall* e = dynamic_cast<CBreakWall*> (obj);

			if (this->AABB(obj) == true)
			{
				/*if (e->GetState() != BREAKWALL_STATE_DESTROYED)
				{*/
					//e->SetState(BREAKWALL_STATE_DESTROYED);
					e->SetDestroy(true);
				//}
			}
		}
	}

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

void CWhip::SetLevel(int level)
{
	this->level = level;
}
