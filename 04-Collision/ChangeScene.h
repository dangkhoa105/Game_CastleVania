#pragma once
#include "GameObject.h"
#include "Define.h"

class CChangeScene : public CGameObject
{
	int IdScene;

public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);

	int GetIdScene() { return IdScene; }
};

