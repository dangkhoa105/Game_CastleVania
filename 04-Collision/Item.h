#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CItem : public CGameObject
{
public:
	int idItem;
	int positionInitX;
	CItem()
	{
		AddAnimation("item_whip");
		AddAnimation("item_money_bag_400");
		AddAnimation("item_money_bag_700");
		AddAnimation("item_money_bag_1000");
		AddAnimation("item_heart");
		AddAnimation("item_small_heart");
		AddAnimation("item_knife");
		AddAnimation("item_boomerang");
		AddAnimation("item_axe");
		AddAnimation("clock_ani");
		AddAnimation("item_gas");
		AddAnimation("item_crown");
		AddAnimation("item_double_shot");
		AddAnimation("item_kfc");
		AddAnimation("item_potion");
	}
	int GetId() { return idItem; }
	void SetId(int id) { this->idItem = id; }

	void SetPositionInit(int x) { this->positionInitX = x; }

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void SetState(int state);
};

