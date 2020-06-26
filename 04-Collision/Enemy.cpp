#include "Enemy.h"

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
		if (isSpawn == false)
		{
			isSpawn = true;
			spawnTime = GetTickCount();
		}
		else
		{
			float remain = GetTickCount() - spawnTime;

			if (remain > (DWORD)450)
			{
				isSpawn = false;
				spawnTime = 0;
				return true;
			}
		}
	}	
	else
		return false;
}
