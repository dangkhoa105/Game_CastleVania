#include "Monkey.h"

void CMonkey::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CEnemy::Update(dt);
}

void CMonkey::Render()
{
	if (state == MONKEY_STATE_IDLE)
		animations["monkey_ani_idle"]->Render(-nx, x, y);
	else if (state == MONKEY_STATE_START)
		animations["monkey_ani_jumping"]->Render(-nx, x, y);
	else if (state == MONKEY_STATE_JUMPING)
		animations["monkey_ani_jumping"]->Render(-nx, x, y);

	float l, t, r, b;
	this->GetBoundingBoxActive(l, t, r, b);
	RenderBoundingBox(RECT{ (long)l,(long)t,(long)r,(long)b });
}

void CMonkey::SetState(int state)
{
	CEnemy::SetState(state);
	switch (state)
	{
	case MONKEY_STATE_IDLE:
		vx = vy = 0;
		break;
	case MONKEY_STATE_START:
		vx = vy = 0;
		break;
	case MONKEY_STATE_JUMPING:
		break;
	default:
		break;
	}
}

void CMonkey::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isDestroy)
		return;

	left = x;
	top = y;
	right = x + bboxEnemyWidth;
	bottom = y + bboxEnemyHeight;
}

void CMonkey::GetBoundingBoxActive(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + bboxEnemyActiveWidth;
	bottom = y + bboxEnemyActiveHeight;
}