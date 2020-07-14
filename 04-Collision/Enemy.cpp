#include "Enemy.h"
#include "debug.h"

void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	CGameObject::Update(dt);
}

void CEnemy::SetState(int state)
{
	CGameObject::SetState(state);
}

void CEnemy::StartRespawnTimeCounter()
{
	isReSpawnWaiting = true;
	reSpawnTimeStart = GetTickCount();
}

bool CEnemy::IsActivate()
{
	if (isReSpawnWaiting == true && (GetTickCount() - reSpawnTimeStart >= reSpawnWaitingTime))
		return true;
	return false;
}

bool CEnemy::IsRespawn()
{	
	if (this->isDestroy)
	{
		if (!isSpawn)
		{
			isSpawn = true;
			spawnTime = GetTickCount();
		}
		if (isSpawn)
		{
			float remain = GetTickCount() - spawnTime;

			if (remain > 1)
			{
				isSpawn = false;
				spawnTime = 0;
				isDestroy = true;
				return true;
			}
		}
	}		
	else
	{
		return false;
	}
}
