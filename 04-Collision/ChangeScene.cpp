#include "ChangeScene.h"

void CChangeScene::Render()
{
	//RenderBoundingBox();
}

void CChangeScene::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + CHANGESCENE_BBOX_WIDTH;
	b = y + CHANGESCENE_BBOX_HEIGHT;
}

void CChangeScene::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
}
