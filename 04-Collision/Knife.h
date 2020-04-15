#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CKnife : public CGameObject
{
	bool isAttackKnife = false;
public:
	CKnife() : CGameObject()
	{
		AddAnimation("item_knife");
	}

	void ResetAttack() {
		animations["item_knife"]->ResetAnimation();
	}

	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);

	void SetAttackKnife(bool isAttackKnife) { this->isAttackKnife = isAttackKnife; }
	void SetState(int state);
};

