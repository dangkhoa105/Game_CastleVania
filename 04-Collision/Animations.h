#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>
#include "Sprites.h"

using namespace std;
/*
	Sprite animation
*/
class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef CAnimationFrame* LPANIMATION_FRAME;

class CAnimation
{
	DWORD lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;

	bool isComplete = false;

	bool isLoop;// ani có lặp lại không 
public:
	CAnimation(int defaultTime) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void ResetAnimation() { this->currentFrame = -1; }

	int GetCurrentFrame() { return this->currentFrame; }
	void SetLoop(bool flag=true){this->isLoop = flag; };
	void Add(string spriteId, DWORD time = 0);
	void Render(float x, float y, int alpha = 255);
	void Render(int nx, float x, float y, int alpha = 255);
};

typedef CAnimation* LPANIMATION;

class CAnimations
{
	static CAnimations* __instance;

	unordered_map<string, LPANIMATION> animations;

public:
	void Add(string id, LPANIMATION ani);
	LPANIMATION Get(string id);

	static CAnimations* GetInstance();
};