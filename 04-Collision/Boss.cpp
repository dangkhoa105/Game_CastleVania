#include "Boss.h"

void CBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CEnemy::Update(dt);
}

void CBoss::Render()
{
	if (state == BOSS_STATE_IDLE)
		animations["boss_ani_idle"]->Render(-nx, x, y);
	if (state == BOSS_STATE_FLYING)
		animations["boss_ani_flying"]->Render(-nx, x, y);
}

void CBoss::SetState(int state)
{
	CEnemy::SetState(state);
}

void CBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + bboxEnemyWidth;
	bottom = y + bboxEnemyHeight;
}

void CBoss::GetBoundingBoxActive(float& left, float& top, float& right, float& bottom)
{
	left = (x + bboxEnemyWidth / 2) - bboxEnemyActiveWidth;
	top = (y + bboxEnemyHeight / 2) - bboxEnemyActiveHeight;
	right = (x + bboxEnemyWidth / 2) + bboxEnemyActiveWidth;
	bottom = y + bboxEnemyActiveHeight;
}
