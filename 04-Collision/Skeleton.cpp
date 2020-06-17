#include "Skeleton.h"

void CSkeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CEnemy::Update(dt);
}

void CSkeleton::Render()
{
	animations["skeleton_ani_jumping"]->Render(-nx, x, y);
}

void CSkeleton::SetState(int state)
{
	CEnemy::SetState(state);
}

void CSkeleton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isDestroy)
		return;

	left = x;
	top = y;
	right = x + bboxEnemyWidth;
	bottom = y + bboxEnemyHeight;
}

void CSkeleton::GetBoundingBoxActive(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + bboxEnemyActiveWidth;
	bottom = y + bboxEnemyActiveHeight;
}