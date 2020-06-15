#pragma once
#include "GameObject.h"
#include "Define.h"
#include "ReadResourceFile.h"

class CEnemy : public CGameObject
{	
public:
	int hp;
	bool isSpawn;
	int spawnTime;
	bool isReSpawn;
	int reSpawnTime;
	DWORD reSpawnTimeStart;
	bool isFinishReSpawn;

	int bboxEnemyWidth;
	int bboxEnemyHeight;

	int bboxEnemyActiveWidth;
	int bboxEnemyActiveHeight;

	void TakeDamage(int damage) 
	{ 
		hp = hp - damage;
		if (hp<0)
		{
			hp = 0;
		}
	}

	D3DXVECTOR2 initPosition; //vị trí ban đầu

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void GetBoundingBoxActive(float& left, float& top, float& right, float& bottom) = 0;

	void StartRespawnTimeCounter();
	bool IsActivate();
	bool IsRespawn();
	void SetEntryPosition(float x, float y) { initPosition.x = x; initPosition.y = y; }
	D3DXVECTOR2 GetEntryPosition() { return initPosition; }	
	
	void SetBboxEnemy(int bW, int bH) { this->bboxEnemyWidth = bW; this->bboxEnemyHeight = bH; }
	void SetBboxEnemyActive(int bW, int bH) { this->bboxEnemyActiveWidth = bW; this->bboxEnemyActiveHeight = bH; }
};

