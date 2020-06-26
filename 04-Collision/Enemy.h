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
	DWORD reSpawnTimeStart;
	bool isReSpawnWaiting;
	int reSpawnWaitingTime;
	bool isFinishReSpawn;

	int bboxEnemyWidth;
	int bboxEnemyHeight;

	int bboxEnemyActiveWidth;
	int bboxEnemyActiveHeight;

	int beginPositionX;
	int lastPositionX;

	CEnemy() 
	{
		isReSpawnWaiting = false;
		reSpawnWaitingTime = 0;
		spawnTime = 0;;	
		reSpawnTimeStart = 0;
		isFinishReSpawn = false;
		isDestroy = true;
	}

	void TakeDamage(int damage) 
	{ 
		hp = hp - damage;
		if (hp<0)
		{
			hp = 0;
		}
	}

	float initPositionX; //vị trí ban đầu
	float initPositionY;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void GetBoundingBoxActive(float& left, float& top, float& right, float& bottom) = 0;

	void StartRespawnTimeCounter();
	bool IsActivate();
	bool IsRespawn();

	void SetEntryPosition(float x, float y) 
	{ 
		this->initPositionX = x;
		this->initPositionY = y;
	}	
	
	float GetEntryPositionY() { return initPositionY; }
	
	void SetBboxEnemy(int bW, int bH) { this->bboxEnemyWidth = bW; this->bboxEnemyHeight = bH; }
	void SetBboxEnemyActive(int bW, int bH) { this->bboxEnemyActiveWidth = bW; this->bboxEnemyActiveHeight = bH; }

	void SetReturnPosition(int bX, int lX) 
	{
		this->beginPositionX = bX;
		this->lastPositionX = lX;
	}
};

