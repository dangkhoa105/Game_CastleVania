#include "Stair.h"

void CStair::Render()
{
	RenderBoundingBox();
}

void CStair::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y + 12;
	r = x + STAIR_BBOX_WIDTH + 16;
	b = y + STAIR_BBOX_HEIGHT;
}
