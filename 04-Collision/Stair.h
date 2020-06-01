#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CStair : public CGameObject
{
	int directionStair;
	bool isActive = false;
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
public:
	int GetDirectionStair() { return this->directionStair; }
	void SetDirectionStair(int directionStair) { this->directionStair = directionStair; }

	bool CheckActive() { 
		return this->isActive; }
	void SetActive(bool isActive) { 
		this->isActive = isActive; }
};

