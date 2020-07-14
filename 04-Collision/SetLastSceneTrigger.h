#pragma once
#include "GameObject.h"
#include "Define.h"

class CSetLastSceneTrigger : public CGameObject
{
public:
	CSetLastSceneTrigger() {};
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
};

