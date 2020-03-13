#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Animations.h"
#include "Textures.h"
#include "Define.h"

#include "Simon.h"
#include "Brick.h"
#include "Goomba.h"
#include "Whip.h"
#include "Candle.h"
#include "TileMap.h"

CGame* game;

CSimon* simon;
CGoomba* goomba;
CTileMap* tilemap;

vector<LPGAMEOBJECT> objects;

class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander* keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

 	if (KeyCode == DIK_SPACE) {
		if (simon->isGround == false || simon->IsHitting() == true) return;
		simon->SetState(SIMON_STATE_JUMP);
	}
	else if (KeyCode == DIK_Z) {
		if (simon->IsHitting() == true) return;
		if (simon->GetState() == SIMON_STATE_SIT) simon->SetState(SIMON_STATE_SIT_ATTACK);
		else if (simon->GetState() == SIMON_STATE_IDLE || simon->GetState() == SIMON_STATE_JUMP) simon->SetState(SIMON_STATE_STAND_ATTACK);
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE* states)
{
	// disable control key when Simon die 
	//if (simon->isJumping == true) return;
	if ((simon->GetState() == SIMON_STATE_STAND_ATTACK || simon->GetState() == SIMON_STATE_SIT_ATTACK) && !simon->IsComplete()) return;
	if ((simon->GetState() == SIMON_STATE_JUMP || simon->GetState() == SIMON_STATE_IDLE) && simon->isGround == false) return;
	if (simon->GetState() == SIMON_STATE_DIE) return;

	if (game->IsKeyDown(DIK_RIGHT)) {
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		simon->SetState(SIMON_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_DOWN)) {
		simon->SetState(SIMON_STATE_SIT);
	}
	else {
		simon->SetState(SIMON_STATE_IDLE);
	}
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and simon object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	CTextures* textures = CTextures::GetInstance();
	textures->Add(400, L"resources\\Map\\map1.png", D3DCOLOR_XRGB(255, 0, 255));
	auto texMap = textures->Get(400);
	tilemap = new CTileMap(L"resources\\Map\\map1.txt", 1536, 320, 32, 32);
	tilemap->SetTileMap(texMap);
	tilemap->LoadResources();

	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	ReadResourceFile* ReadResourceFile = ReadResourceFile::GetInstance();

	ReadResourceFile->LoadAllResource();

	LPANIMATION ani;
	
	// candle
	for (int i = 0; i < 5; i++)
	{
		CCandle* candle = new CCandle();
		candle->SetPosition(160 + i * 270, 320 - 64 - 32);
		objects.push_back(candle);
	}

	// simon
	simon = new CSimon();			
	simon->SetPosition(100, 200);
	objects.push_back(simon);

	// brick
	ani = new CAnimation(100);		
	ani->Add("brick");
	animations->Add("brick", ani);

	for (int i = 0; i < 100; i++)
	{
		CBrick* brick = new CBrick();
		brick->AddAnimation("brick");
		brick->SetPosition(0 + i * 16.0f, 320 - 32);
		objects.push_back(brick);
	}
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	// We know that Simon is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}


	// Update camera to follow simon
	if (simon->x > SCREEN_WIDTH / 2) {
		CGame::GetInstance()->SetCamPos(simon->x - SCREEN_WIDTH /*cx*/, 0.0f /*cy*/);
	}

	float cx, cy;
	simon->GetPosition(cx, cy);

	if (cx > SCREEN_WIDTH / 2) {
		CGame::GetInstance()->SetCamPos(cx - SCREEN_WIDTH / 2, 0.0f /*cy*/);
	}
}

/*
	Render a frame
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		tilemap->Draw(game->GetCamPos());
		for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();

		//simon->Render();
		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);


	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}