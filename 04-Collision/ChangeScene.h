#pragma once
#include "GameObject.h"
#include "Define.h"

class CChangeScene : public CGameObject
{
	bool isChangeScene = false;
public:	
	int IdNextScene;
	CChangeScene() { }
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);

	void SetIsChangeScene(bool isChangeScene) { this->isChangeScene = isChangeScene; }
	bool GetIsChangeScene() { return this->isChangeScene; }

	void SetIdNextScene(int idScene) { this->IdNextScene = idScene; }
	int GetIdNextScene() { return IdNextScene; }
};

