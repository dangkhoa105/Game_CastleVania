#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CItem : public CGameObject
{
public:
	int idItem;
	CItem()
	{
		AddAnimation("item_whip");
		AddAnimation("item_money_bag");
		AddAnimation("item_heart");
		AddAnimation("item_small_heart");
		AddAnimation("item_knife");
		AddAnimation("item_boomerang");
		AddAnimation("item_crown");
		AddAnimation("item_double_shot");
	}
	int GetId() { return idItem; }
	void SetId(int id) { this->idItem = id; }

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	void SetState(int state);
};

