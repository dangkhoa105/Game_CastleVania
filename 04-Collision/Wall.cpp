#include "Wall.h"

void CWall::Render()
{
	animations["wall"]->Render(x, y);
}

void CWall::GetBoundingBox(float& l, float& t, float& r, float& b)
{

}
