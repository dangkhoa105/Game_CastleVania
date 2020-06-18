#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CWhip : public CGameObject
{
	int level = 1;
	
	DWORD untouchable_start;
	DWORD timeDelete;
	int untouchable;
	int damage = 1;

public:
	bool fight;
	CWhip() : CGameObject()
	{
		AddAnimation("whip_ani", false);
		AddAnimation("whip_ani_level_2", false);
		AddAnimation("whip_ani_level_3", false);
	}

	void ResetAttack() {
		animations["whip_ani"]->ResetAnimation();
		animations["whip_ani_level_2"]->ResetAnimation();
		animations["whip_ani_level_3"]->ResetAnimation();
	}

	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	//void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);

	void SetState(int state);
	int GetState();

	void SetLevel(int level);
};

