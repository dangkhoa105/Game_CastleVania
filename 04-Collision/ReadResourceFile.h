#pragma once
#include <d3dx9.h>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

#include "define.h"

using namespace std;

using namespace rapidxml;

class ReadResourceFile
{
	static ReadResourceFile* __instance;
public:
	static ReadResourceFile* GetInstance();
	
	// load file hinh xml
	void LoadSpritesSheetFile(const char* filePath, LPDIRECT3DTEXTURE9 tex);
	void LoadAnimationsSheetFile(const char* filePath);

	// load hinh cua cac nhan vat
	void LoadTextures();
	vector<string> GetAnimations(const char* filePath);

	void LoadAllResource();
};

