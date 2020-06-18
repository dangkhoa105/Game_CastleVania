#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"
#include "Effect.h"

class CCandle : public CGameObject
{
	int id;
	bool destroyed;
	int itemDef = -1;
public:
	
	CCandle() : CGameObject()
	{
		AddAnimation("big_candle");	
		AddAnimation("small_candle");
	}

	int GetItem() { return this->itemDef; }
	void SetItem(int itemId) { this->itemDef = itemId; };

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);

	void SetState(int state);
	//bool IsDestroy();
};

