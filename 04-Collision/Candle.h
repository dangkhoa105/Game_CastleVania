#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CCandle : public CGameObject
{
	int id;
	bool die;
public:

	CCandle() : CGameObject()
	{
		ReadResourceFile* readResourceFile = ReadResourceFile::GetInstance();

		vector<string> animationsWhip = readResourceFile->GetAnimations("resources\\Castlevania\\castlevania_ani.xml");
		for each (string animation in animationsWhip)
		{
			AddAnimation(animation);

		}
	}
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);

	void SetState(int state);
	bool GetDie() { return this->die; }
};

