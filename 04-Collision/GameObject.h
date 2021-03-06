﻿#pragma once
#ifndef _PROJECTILOBJECT_H_
#define _PROJECTILOBJECT_H_
#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"

using namespace std;

class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;
struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	CCollisionEvent(float t, float nx, float ny, LPGAMEOBJECT obj = NULL) { this->t = t; this->nx = nx; this->ny = ny; this->obj = obj; }

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};
struct CellIndex {
	int x;
	int y;
	CellIndex(int x, int y) :x(x), y(y) {};
};



class CGameObject
{
public:

	float x; 
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;	 
	int ny;

	int state;

	int id;
	bool die;
	bool fall;

	
	bool box;

	DWORD dt; 

	unordered_map<string, LPANIMATION> animations;

	bool isDestroy = false; // cờ hiệu để biết object bị hủy trong quá trình update; B
	bool isActive = false;
	CellIndex cellIndex = { -1,-1 };


	void SetActive() {
		isActive = true;
	}
	bool CheckActive() {
		return isActive;
	}

	void SetCellIndex(int cellX, int cellY) {
		CellIndex cellIndex = { cellX,cellY };
		this->cellIndex = cellIndex;
	}

	CellIndex GetCellIndex() {
		return cellIndex;
	}
	void SetDestroy(bool isDestroy) { this->isDestroy = isDestroy; }
	bool IsDestroy() { return this->isDestroy; }
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	void SetDie(bool die) { this->die = die; }
	bool GetDie() { return this->die; }

	void SetFall(bool fall) { this->fall = fall; }
	bool GetFall() { return this->fall; }

	void SetId(int a) { this->id = a; }
	int GetId() { return this->id; }

	void SetNx(int nx) { this->nx = nx; }
	int GetNx() { return this->nx; }

	void SetNy(int ny) { this->ny = ny; }
	int GetNy() { return this->ny; }

	bool GetBox() { return this->box; }

	int GetState() { return this->state; }
	void SetState(int state) { this->state = state; }

	void RenderBoundingBox();
	void RenderBoundingBox(RECT rect);
	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny);

	void AddAnimation(string aniId,bool isLoop=true);

	CGameObject();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;

	bool AABB(LPGAMEOBJECT coO);

	~CGameObject();
};

#endif