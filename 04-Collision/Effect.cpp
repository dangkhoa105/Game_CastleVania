#include "Effect.h"

void CEffect::Render()
{
	if (state == EFFECT)
		animations["effect"]->Render(x, y);
}

void CEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}
