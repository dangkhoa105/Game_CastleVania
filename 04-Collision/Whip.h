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

public:

	CWhip() : CGameObject()
	{
		ReadResourceFile* readResourceFile = ReadResourceFile::GetInstance();

		vector<string> animationsWhip = readResourceFile->GetAnimations("resources\\Castlevania\\castlevania_ani.xml");
		for each (string animation in animationsWhip)
		{
			AddAnimation(animation);

		}
	}

	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	//void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void SetState(int state);
	int GetState();
};

