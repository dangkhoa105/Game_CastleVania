#include "ReadResourceFile.h"
#include <rapidxml\rapidxml_utils.hpp>
#include "Sprites.h"
#include "Textures.h"
#include "Animations.h"

ReadResourceFile* ReadResourceFile::__instance = NULL;

ReadResourceFile* ReadResourceFile::GetInstance()
{
	if (__instance == NULL) __instance = new ReadResourceFile();
	return __instance;
}

void ReadResourceFile::LoadSpritesSheetFile(const char* filePath, LPDIRECT3DTEXTURE9 tex)
{
	rapidxml::file<> xmlFile(filePath);
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* rootNode = doc.first_node("TextureAtlas");
	for (xml_node<>* spriteNode = rootNode->first_node(); spriteNode; spriteNode = spriteNode->next_sibling()) {

		string idSprite;
		int left;
		int top;
		int right;
		int bottom;

		idSprite = spriteNode->first_attribute("n")->value();
		left = atoi(spriteNode->first_attribute("x")->value());
		top = atoi(spriteNode->first_attribute("y")->value());
		right = atoi(spriteNode->first_attribute("w")->value());
		bottom = atoi(spriteNode->first_attribute("h")->value());

		CSprites* sprites = CSprites::GetInstance();
		sprites->Add(idSprite, left, top, right, bottom, tex);
	}
}

void ReadResourceFile::LoadAnimationsSheetFile(const char* filePath)
{
	rapidxml::file<> xmlFile(filePath);
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* rootNode = doc.first_node("animations");

	int i = 0;
	for (xml_node<>* animationNode = rootNode->first_node(); animationNode; animationNode = animationNode->next_sibling()) {
		LPANIMATION ani;

		int defaultTime = atoi(animationNode->first_attribute("defaultTime")->value());
		ani = new CAnimation(defaultTime);

		xml_node<>* frameNode = rootNode->first_node("frame");
		for (xml_node<>* frameNode = animationNode->first_node("frame"); frameNode; frameNode = frameNode->next_sibling())
		{
			string spriteId = frameNode->first_attribute("spriteID")->value();
			int time = atoi(frameNode->first_attribute("time")->value());
			ani->Add(spriteId, time);
		}

		string aniId = string(animationNode->first_attribute("ID")->value());
		CAnimations* animations = CAnimations::GetInstance();
		animations->Add(aniId, ani);
	}
}

void ReadResourceFile::LoadTextures()
{
	CTextures* textures = CTextures::GetInstance();

	textures->Add(ID_TEX_SIMON, L"resources\\Castlevania.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BRICK, L"resources\\Castlevania.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WHIP, L"resources\\Castlevania.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_CANDLE, L"resources\\Castlevania.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_EFFECT, L"resources\\Castlevania.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_IHEART, L"resources\\Castlevania.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_IWHIP, L"resources\\Castlevania.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_IKNIFE, L"resources\\Castlevania.png", D3DCOLOR_XRGB(255, 0, 255));

	textures->Add(ID_TEX_BBOX, L"resources\\bbox.png", D3DCOLOR_XRGB(0, 0, 0));
}

vector<string> ReadResourceFile::GetAnimations(const char* filePath)
{
	vector<string> animationsList;

	rapidxml::file<> xmlFile(filePath);
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* rootNode = doc.first_node("animations");
	int i = 0;
	for (xml_node<>* animationNode = rootNode->first_node(); animationNode; animationNode = animationNode->next_sibling()) {

		string aniId = string(animationNode->first_attribute("ID")->value());
		animationsList.push_back(aniId);
	}

	return animationsList;
}

void ReadResourceFile::LoadAllResource()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();
	ReadResourceFile* ReadResourceFile = ReadResourceFile::GetInstance();

	ReadResourceFile->LoadTextures();
	auto texSimon = textures->Get(ID_TEX_SIMON);
	auto texBrick = textures->Get(ID_TEX_BRICK);
	auto texWhip = textures->Get(ID_TEX_WHIP);
	auto texCandle = textures->Get(ID_TEX_CANDLE);
	auto texEffect = textures->Get(ID_TEX_EFFECT);
	auto texIHeart = textures->Get(ID_TEX_IHEART);
	auto texIWhip = textures->Get(ID_TEX_IWHIP);
	auto texIKnife = textures->Get(ID_TEX_IKNIFE);

	ReadResourceFile->LoadSpritesSheetFile("resources\\Castlevania\\castlevania.xml", texSimon);
	ReadResourceFile->LoadSpritesSheetFile("resources\\Castlevania\\castlevania.xml", texBrick);
	ReadResourceFile->LoadSpritesSheetFile("resources\\Castlevania\\castlevania.xml", texWhip);
	ReadResourceFile->LoadSpritesSheetFile("resources\\Castlevania\\castlevania.xml", texCandle);
	ReadResourceFile->LoadSpritesSheetFile("resources\\Castlevania\\castlevania.xml", texEffect);
	ReadResourceFile->LoadSpritesSheetFile("resources\\Castlevania\\castlevania.xml", texIHeart);
	ReadResourceFile->LoadSpritesSheetFile("resources\\Castlevania\\castlevania.xml", texIWhip);
	ReadResourceFile->LoadSpritesSheetFile("resources\\Castlevania\\castlevania.xml", texIKnife);
	
	ReadResourceFile->LoadAnimationsSheetFile("resources\\Castlevania\\castlevania_ani.xml");
}
