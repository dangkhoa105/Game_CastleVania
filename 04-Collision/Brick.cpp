#include "Brick.h"

void CBrick::Render()
{
	//animations["brick"]->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + widthbbox;
	b = y + heightbbox;
}