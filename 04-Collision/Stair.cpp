#include "Stair.h"

void CStair::Render()
{
	//RenderBoundingBox();
}

void CStair::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + STAIR_BBOX_WIDTH;
	b = y + STAIR_BBOX_HEIGHT;
}
