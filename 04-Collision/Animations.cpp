#include "Animations.h"
#include "Game.h"
#include "debug.h"
#include "Define.h"
#include "Sprites.h"

void CAnimation::Add(string spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

void CAnimation::Render(float x, float y, int alpha)
{

	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			
			lastFrameTime = now;
			
			if (currentFrame == frames.size())
			{
				if (!isLoop) currentFrame--;
				else currentFrame = 0;
			}
		}

	}

	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}

void CAnimation::Render(int nx, float x, float y, int alpha)
{

	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
		
			lastFrameTime = now;
			
			if (currentFrame == frames.size())
			{
				if (!isLoop) currentFrame--;
				else currentFrame = 0;
			}
				
		}

	}

	frames[currentFrame]->GetSprite()->Draw(nx, x, y, alpha);
}

CAnimations* CAnimations::__instance = NULL;

CAnimations* CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(string id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION CAnimations::Get(string id)
{
	return animations[id];
}