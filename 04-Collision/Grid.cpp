#include "Grid.h"
#include"debug.h"
#include"Game.h"
#include"Item.h"
#include"Effect.h"
#include"Enemy.h"
#include "Candle.h"
#include "PlayScene.h"


void Grid::Add(LPGAMEOBJECT object, bool isAlwayUpdateObject)
{

	CellIndex cell = object->GetCellIndex();
	//tính vị trí của object
	int cellX = cell.x;
	int cellY = cell.y;
	if (cellX >= gridCol || cellX < 0 || cellY < 0 || cellY > gridRow - 1)
		return;

	//đánh dấu object thuộc cell nào
	// để có thể truy vấn nhanh vị trị của object trong grid


	if (!isAlwayUpdateObject)
		this->grid[cellY][cellX].push_back(object);
	else
		alwaysUpdateList.push_back(object);
}

void Grid::AddWhenUpdate(LPGAMEOBJECT object)
{
	float x_, y_;
	object->GetPosition(x_, y_);
	//tính vị trí của object
	int cellX = (int)(x_ / this->cellSize);
	int cellY = (int)(y_ / this->cellSize);
	if (cellX >= gridCol || cellX < 0 || cellY < 0 || cellY > gridRow - 1)
		return;

	//đánh dấu object thuộc cell nào
	// để có thể truy vấn nhanh vị trị của object trong grid

	object->SetCellIndex(cellX, cellY);
	this->grid[cellY][cellX].push_back(object);

}

void Grid::Update(LPGAMEOBJECT object)
{
	float cx_, cy_;

	D3DXVECTOR2 cam = CGame::GetInstance()->GetCamPos();

	cx_ = cam.x;
	cy_ = cam.y;

	float x, y;
	object->GetPosition(x, y);

	float l, t, r, b;
	object->GetBoundingBox(l, t, r, b);

	if (CGame::GetInstance()->AABB(l, t, r, b, cx_, cy_, cx_ + SCREEN_WIDTH, cy_ + SCREEN_HEIGHT))
		object->SetActive();
	else
	{
		if (object->CheckActive())
		{
			if (dynamic_cast<CItem*>(object) || dynamic_cast<CEnemy*>(object))
			{
				object->isDestroy = true;
			}
			if (dynamic_cast<CSubWeapon*>(object))
			{
				object->isDestroy = true;
				CPlayScene::GetInstance()->SetCountSW();
			}
		}
	}

	CellIndex oldCell = object->GetCellIndex();

	int cellX = (int)(x / this->cellSize);
	int cellY = (int)(y / this->cellSize);

	if (object->IsDestroy())
	{
		for (vector<LPGAMEOBJECT>::iterator it = grid[oldCell.y][oldCell.x].begin(); it != grid[oldCell.y][oldCell.x].end(); )
		{
			if ((*it) == object && !dynamic_cast<CEnemy*>(object) || dynamic_cast<CBoss*>(object))
			{
				it = grid[oldCell.y][oldCell.x].erase(it);
			}
			else ++it;
		}
		return;
	}

	if (oldCell.x == cellX && oldCell.y == cellY) return;
	if (oldCell.x != -1 && oldCell.y != -1)
	{				// loại bỏ cell cũ
		for (vector<LPGAMEOBJECT>::iterator it = grid[oldCell.y][oldCell.x].begin(); it != grid[oldCell.y][oldCell.x].end(); )
		{
			if ((*it) == object)
			{
				it = grid[oldCell.y][oldCell.x].erase(it);
			}
			else ++it;
		}
	}


	AddWhenUpdate(object);
}

void Grid::GetListobjectFromGrid(vector<LPGAMEOBJECT>& listobjects)
{
	vector<LPGAMEOBJECT> enemiesobject;
	vector<LPGAMEOBJECT> itemobject;
	vector<LPGAMEOBJECT> effectobject;
	vector<LPGAMEOBJECT> subweaponobject;
	enemiesobject.clear();
	itemobject.clear();
	effectobject.clear();
	subweaponobject.clear();

	float camx, camy;
	D3DXVECTOR2 cam = CGame::GetInstance()->GetCamPos();
	camx = cam.x;
	camy = cam.y;
	int startCol = floor(camx / this->cellSize);
	startCol = startCol > 0 ? startCol + -1 : 0;
	int endCol = floor((camx + SCREEN_WIDTH) / this->cellSize);
	endCol = endCol > gridCol ? gridCol : endCol + 1;
	for (int i = 0; i < this->gridRow; i++) {
		for (int j = startCol; j < endCol; j++) {
			for (size_t k = 0; k < this->grid[i][j].size(); k++)
			{
				LPGAMEOBJECT obj = this->grid[i][j].at(k);
				if (1 == 1)
				{
					if (dynamic_cast<CEffect*>(obj))
						effectobject.push_back(obj);
					else if (dynamic_cast<CItem*>(obj))
						itemobject.push_back(obj);
					else if (dynamic_cast<CEnemy*>(obj))
						enemiesobject.push_back(obj);
					else if (dynamic_cast<CSubWeapon*>(obj))
						subweaponobject.push_back(obj);
					else
						listobjects.push_back(obj);
				}
			}
		}
	}

	listobjects.insert(listobjects.end(), alwaysUpdateList.begin(), alwaysUpdateList.end());
	listobjects.insert(listobjects.end(), effectobject.begin(), effectobject.end());
	listobjects.insert(listobjects.end(), itemobject.begin(), itemobject.end());
	listobjects.insert(listobjects.end(), enemiesobject.begin(), enemiesobject.end());
	listobjects.insert(listobjects.end(), subweaponobject.begin(), subweaponobject.end());
}

void Grid::LoadGridObject(string filePath)
{
	//load objects item

	rapidxml::file<> xmlFile(filePath.c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* rootNode = doc.first_node("grid");
	int objCollectId = 0;
	const int grid_col = std::atoi(rootNode->first_attribute("numCol")->value());
	const int grid_row = std::atoi(rootNode->first_attribute("numRow")->value());
	const int cell_size = std::atoi(rootNode->first_attribute("cellSize")->value());
	this->cellSize = cell_size;
	this->gridCol = grid_col + 1;
	this->gridRow = grid_row + 1;

	// clear grid
	this->grid.resize(gridRow);

	for (int i = 0; i < gridRow; i++)
		grid[i].resize(gridCol);

	for (size_t i = 0; i < gridRow; i++)
	{
		for (size_t j = 0; j < gridCol; j++)
		{
			this->grid[i][j].clear();
		}
	}


	auto pScene = CPlayScene::GetInstance();

	for (xml_node<>* oChild = rootNode->first_node(); oChild; oChild = oChild->next_sibling()) //cú pháp lập
	{
		const std::string nodeName = oChild->name();

		if (nodeName == "bricks")
		{
			for (xml_node<>* grand = oChild->first_node(); grand; grand = grand->next_sibling()) //cú pháp lập
			{
				CBrick* brick = new CBrick();
				const int x = std::atoi(grand->first_attribute("x")->value());
				const int y = std::atoi(grand->first_attribute("y")->value());
				const int col = std::atoi(grand->first_attribute("col")->value());
				const int row = std::atoi(grand->first_attribute("row")->value());
				const int widthbbox = std::atoi(grand->first_attribute("widthbbox")->value());
				const int heightbbox = std::atoi(grand->first_attribute("heightbbox")->value());
				brick->SetPosition(x, y);
				brick->SetCellIndex(col, row);
				brick->widthbbox = widthbbox;
				brick->heightbbox = heightbbox;
				Add(brick, true);
			}
		}
		else if (nodeName == "candles")
		{
			for (xml_node<>* grand = oChild->first_node(); grand; grand = grand->next_sibling()) //cú pháp lập
			{
				CCandle* candle = new CCandle();
				const int x = std::atoi(grand->first_attribute("x")->value());
				const int y = std::atoi(grand->first_attribute("y")->value());
				const int col = std::atoi(grand->first_attribute("col")->value());
				const int row = std::atoi(grand->first_attribute("row")->value());
				const int itemID = std::atoi(grand->first_attribute("itemId")->value());
				const int state = std::atoi(grand->first_attribute("state")->value());
				candle->SetItem(itemID);
				candle->SetPosition(x, y);
				candle->SetState(state);
				candle->SetCellIndex(col, row);
				Add(candle);
			}
		}
		else if (nodeName == "entrace")
		{
			CEntrace* entrace = new CEntrace();
			const int x = std::atoi(oChild->first_attribute("x")->value());
			const int y = std::atoi(oChild->first_attribute("y")->value());
			const int col = std::atoi(oChild->first_attribute("col")->value());
			const int row = std::atoi(oChild->first_attribute("row")->value());
			entrace->SetPosition(x, y);
			entrace->SetCellIndex(col, row);
			Add(entrace);
		}
		else if (nodeName == "changeScene")
		{
			CChangeScene* changeScene = new CChangeScene();
			const int x = std::atoi(oChild->first_attribute("x")->value());
			const int y = std::atoi(oChild->first_attribute("y")->value());
			const int col = std::atoi(oChild->first_attribute("col")->value());
			const int row = std::atoi(oChild->first_attribute("row")->value());
			const int idChangeScene = std::atoi(oChild->first_attribute("idChangeScene")->value());
			changeScene->SetPosition(x, y);
			changeScene->SetIdNextScene(idChangeScene);
			changeScene->SetCellIndex(col, row);
			Add(changeScene);
		}
		else if (nodeName == "moneyBagTrigger")
		{
			CMoneyBagTrigger* moneyBagTrigger = new CMoneyBagTrigger();
			const int x = std::atoi(oChild->first_attribute("x")->value());
			const int y = std::atoi(oChild->first_attribute("y")->value());
			const int col = std::atoi(oChild->first_attribute("col")->value());
			const int row = std::atoi(oChild->first_attribute("row")->value());
			moneyBagTrigger->SetPosition(x, y);
			moneyBagTrigger->SetCellIndex(col, row);
			Add(moneyBagTrigger);
		}
		else if (nodeName == "breakWallTrigger")
		{
			CBreakWallTrigger* breakWallTrigger = new CBreakWallTrigger();
			const int x = std::atoi(oChild->first_attribute("x")->value());
			const int y = std::atoi(oChild->first_attribute("y")->value());
			const int col = std::atoi(oChild->first_attribute("col")->value());
			const int row = std::atoi(oChild->first_attribute("row")->value());
			breakWallTrigger->SetPosition(x, y);
			breakWallTrigger->SetCellIndex(col, row);
			Add(breakWallTrigger);
		}
		else if (nodeName == "wall")
		{
			pScene->wall = new CWall();
			const int x = std::atoi(oChild->first_attribute("x")->value());
			const int y = std::atoi(oChild->first_attribute("y")->value());
			const int col = std::atoi(oChild->first_attribute("col")->value());
			const int row = std::atoi(oChild->first_attribute("row")->value());
			pScene->wall->SetPosition(x, y);
			pScene->wall->SetCellIndex(col, row);
			Add(pScene->wall);
		}
		else if (nodeName == "stairs")
		{
			for (xml_node<>* grand = oChild->first_node(); grand; grand = grand->next_sibling()) //cú pháp lập
			{
				CStair* stair = new CStair();
				const int x = std::atoi(grand->first_attribute("x")->value());
				const int y = std::atoi(grand->first_attribute("y")->value());
				const int col = std::atoi(grand->first_attribute("col")->value());
				const int row = std::atoi(grand->first_attribute("row")->value());
				const int stairDirection = std::atoi(grand->first_attribute("stairDirection")->value());
				stair->SetPosition(x, y);
				stair->SetCellIndex(col, row);
				stair->SetDirectionStair(stairDirection);
				Add(stair);
			}
		}
		else if (nodeName == "bridge")
		{
			CBridge* bridge = new CBridge();
			const int x = std::atoi(oChild->first_attribute("x")->value());
			const int y = std::atoi(oChild->first_attribute("y")->value());
			const int col = std::atoi(oChild->first_attribute("col")->value());
			const int row = std::atoi(oChild->first_attribute("row")->value());
			const int beginPositionX = std::atoi(oChild->first_attribute("beginPositionX")->value());
			const int lastPositionX = std::atoi(oChild->first_attribute("lastPositionX")->value());
			bridge->SetPosition(x, y);
			bridge->SetCellIndex(col, row);
			bridge->SetReturnPosition(beginPositionX, lastPositionX);
			Add(bridge);
		}
		else if (nodeName == "breakWalls")
		{
			for (xml_node<>* grand = oChild->first_node(); grand; grand = grand->next_sibling()) //cú pháp lập
			{
				CBreakWall* breakWall = new CBreakWall();
				const int x = std::atoi(grand->first_attribute("x")->value());
				const int y = std::atoi(grand->first_attribute("y")->value());
				const int col = std::atoi(grand->first_attribute("col")->value());
				const int row = std::atoi(grand->first_attribute("row")->value());
				breakWall->SetPosition(x, y);
				breakWall->SetCellIndex(col, row);
				Add(breakWall);
			}
		}
		else if (nodeName == "setLastSceneTrigger")
		{
			CSetLastSceneTrigger* setLastSceneTrigger = new CSetLastSceneTrigger();
			const int x = std::atoi(oChild->first_attribute("x")->value());
			const int y = std::atoi(oChild->first_attribute("y")->value());
			const int col = std::atoi(oChild->first_attribute("col")->value());
			const int row = std::atoi(oChild->first_attribute("row")->value());
			setLastSceneTrigger->SetPosition(x, y);
			setLastSceneTrigger->SetCellIndex(col, row);
			Add(setLastSceneTrigger);
		}
		else if (nodeName == "enemies")
		{
			for (xml_node<>* grand = oChild->first_node(); grand; grand = grand->next_sibling()) //cú pháp lập
			{
				const std::string nodeNameGrand = grand->name();
				if (nodeNameGrand == "spearGuards")
				{
					for (xml_node<>* oGrand = grand->first_node(); oGrand; oGrand = oGrand->next_sibling()) //cú pháp lập
					{
						CSpearGuard* spearGuard = new CSpearGuard();
						const int x = std::atoi(oGrand->first_attribute("x")->value());
						const int y = std::atoi(oGrand->first_attribute("y")->value());
						const int col = std::atoi(oGrand->first_attribute("col")->value());
						const int row = std::atoi(oGrand->first_attribute("row")->value());
						const int beginPositionX = std::atoi(oGrand->first_attribute("beginPositionX")->value());
						const int lastPositionX = std::atoi(oGrand->first_attribute("lastPositionX")->value());
						const int bboxEnemyWidth = std::atoi(oGrand->first_attribute("bboxEnemyWidth")->value());
						const int bboxEnemyHeight = std::atoi(oGrand->first_attribute("bboxEnemyHeight")->value());
						const int bboxEnemyActiveWidth = std::atoi(oGrand->first_attribute("bboxEnemyActiveWidth")->value());
						const int bboxEnemyActiveHeight = std::atoi(oGrand->first_attribute("bboxEnemyActiveHeight")->value());
						spearGuard->SetPosition(x, y);
						spearGuard->SetCellIndex(col, row);
						spearGuard->SetReturnPosition(beginPositionX, lastPositionX);
						spearGuard->SetBboxEnemy(bboxEnemyWidth, bboxEnemyHeight);
						spearGuard->SetBboxEnemyActive(bboxEnemyActiveWidth, bboxEnemyActiveHeight);
						spearGuard->SetEntryPosition(x, y);
						Add(spearGuard);
					}
				}
				else if (nodeNameGrand == "bat")
				{
					CBat* bat = new CBat();
					const int x = std::atoi(grand->first_attribute("x")->value());
					const int y = std::atoi(grand->first_attribute("y")->value());
					const int col = std::atoi(grand->first_attribute("col")->value());
					const int row = std::atoi(grand->first_attribute("row")->value());
					const int bboxEnemyWidth = std::atoi(grand->first_attribute("bboxEnemyWidth")->value());
					const int bboxEnemyHeight = std::atoi(grand->first_attribute("bboxEnemyHeight")->value());
					const int bboxEnemyActiveWidth = std::atoi(grand->first_attribute("bboxEnemyActiveWidth")->value());
					const int bboxEnemyActiveHeight = std::atoi(grand->first_attribute("bboxEnemyActiveHeight")->value());
					bat->SetPosition(x, y);
					bat->SetCellIndex(col, row);
					bat->SetBboxEnemy(bboxEnemyWidth, bboxEnemyHeight);
					bat->SetBboxEnemyActive(bboxEnemyActiveWidth, bboxEnemyActiveHeight);
					bat->SetEntryPosition(x, y);
					Add(bat);
				}
				else if (nodeNameGrand == "ghost")
				{
					CGhost* ghost = new CGhost();
					const int x = std::atoi(grand->first_attribute("x")->value());
					const int y = std::atoi(grand->first_attribute("y")->value());
					const int col = std::atoi(grand->first_attribute("col")->value());
					const int row = std::atoi(grand->first_attribute("row")->value());
					const int beginPositionX = std::atoi(grand->first_attribute("beginPositionX")->value());
					const int lastPositionX = std::atoi(grand->first_attribute("lastPositionX")->value());
					const int bboxEnemyWidth = std::atoi(grand->first_attribute("bboxEnemyWidth")->value());
					const int bboxEnemyHeight = std::atoi(grand->first_attribute("bboxEnemyHeight")->value());
					const int bboxEnemyActiveWidth = std::atoi(grand->first_attribute("bboxEnemyActiveWidth")->value());
					const int bboxEnemyActiveHeight = std::atoi(grand->first_attribute("bboxEnemyActiveHeight")->value());
					ghost->SetPosition(x, y);
					ghost->SetCellIndex(col, row);
					ghost->SetReturnPosition(beginPositionX, lastPositionX);
					ghost->SetBboxEnemy(bboxEnemyWidth, bboxEnemyHeight);
					ghost->SetBboxEnemyActive(bboxEnemyActiveWidth, bboxEnemyActiveHeight);
					ghost->SetEntryPosition(x, y);
					Add(ghost);
				}
				else if (nodeNameGrand == "monkeys")
				{
					for (xml_node<>* oGrand = grand->first_node(); oGrand; oGrand = oGrand->next_sibling()) //cú pháp lập
					{
						CMonkey* monkey = new CMonkey();
						const int x = std::atoi(oGrand->first_attribute("x")->value());
						const int y = std::atoi(oGrand->first_attribute("y")->value());
						const int col = std::atoi(oGrand->first_attribute("col")->value());
						const int row = std::atoi(oGrand->first_attribute("row")->value());
						const int beginPositionX = std::atoi(oGrand->first_attribute("beginPositionX")->value());
						const int lastPositionX = std::atoi(oGrand->first_attribute("lastPositionX")->value());
						const int bboxEnemyWidth = std::atoi(oGrand->first_attribute("bboxEnemyWidth")->value());
						const int bboxEnemyHeight = std::atoi(oGrand->first_attribute("bboxEnemyHeight")->value());
						const int bboxEnemyActiveWidth = std::atoi(oGrand->first_attribute("bboxEnemyActiveWidth")->value());
						const int bboxEnemyActiveHeight = std::atoi(oGrand->first_attribute("bboxEnemyActiveHeight")->value());
						monkey->SetPosition(x, y);
						monkey->SetCellIndex(col, row);
						monkey->SetReturnPosition(beginPositionX, lastPositionX);
						monkey->SetBboxEnemy(bboxEnemyWidth, bboxEnemyHeight);
						monkey->SetBboxEnemyActive(bboxEnemyActiveWidth, bboxEnemyActiveHeight);
						monkey->SetEntryPosition(x, y);
						Add(monkey);
					}
				}
				else if (nodeNameGrand == "skeletons")
				{
					for (xml_node<>* oGrand = grand->first_node(); oGrand; oGrand = oGrand->next_sibling()) //cú pháp lập
					{
						CSkeleton* skeleton = new CSkeleton();
						const int x = std::atoi(oGrand->first_attribute("x")->value());
						const int y = std::atoi(oGrand->first_attribute("y")->value());
						const int col = std::atoi(oGrand->first_attribute("col")->value());
						const int row = std::atoi(oGrand->first_attribute("row")->value());
						const int beginPositionX = std::atoi(oGrand->first_attribute("beginPositionX")->value());
						const int lastPositionX = std::atoi(oGrand->first_attribute("lastPositionX")->value());
						const int bboxEnemyWidth = std::atoi(oGrand->first_attribute("bboxEnemyWidth")->value());
						const int bboxEnemyHeight = std::atoi(oGrand->first_attribute("bboxEnemyHeight")->value());
						const int bboxEnemyActiveWidth = std::atoi(oGrand->first_attribute("bboxEnemyActiveWidth")->value());
						const int bboxEnemyActiveHeight = std::atoi(oGrand->first_attribute("bboxEnemyActiveHeight")->value());
						skeleton->SetPosition(x, y);
						skeleton->SetCellIndex(col, row);
						skeleton->SetReturnPosition(beginPositionX, lastPositionX);
						skeleton->SetBboxEnemy(bboxEnemyWidth, bboxEnemyHeight);
						skeleton->SetBboxEnemyActive(bboxEnemyActiveWidth, bboxEnemyActiveHeight);
						skeleton->SetEntryPosition(x, y);
						Add(skeleton);
					}
				}
				else if (nodeNameGrand == "crows")
				{
					for (xml_node<>* oGrand = grand->first_node(); oGrand; oGrand = oGrand->next_sibling()) //cú pháp lập
					{
						CCrow* crow = new CCrow();
						const int x = std::atoi(oGrand->first_attribute("x")->value());
						const int y = std::atoi(oGrand->first_attribute("y")->value());
						const int col = std::atoi(oGrand->first_attribute("col")->value());
						const int row = std::atoi(oGrand->first_attribute("row")->value());
						const int bboxEnemyWidth = std::atoi(oGrand->first_attribute("bboxEnemyWidth")->value());
						const int bboxEnemyHeight = std::atoi(oGrand->first_attribute("bboxEnemyHeight")->value());
						const int bboxEnemyActiveWidth = std::atoi(oGrand->first_attribute("bboxEnemyActiveWidth")->value());
						const int bboxEnemyActiveHeight = std::atoi(oGrand->first_attribute("bboxEnemyActiveHeight")->value());
						crow->SetPosition(x, y);
						crow->SetCellIndex(col, row);
						crow->SetBboxEnemy(bboxEnemyWidth, bboxEnemyHeight);
						crow->SetBboxEnemyActive(bboxEnemyActiveWidth, bboxEnemyActiveHeight);
						crow->SetEntryPosition(x, y);
						Add(crow);
					}
				}
				else if (nodeNameGrand == "zombies")
				{
					for (xml_node<>* oGrand = grand->first_node(); oGrand; oGrand = oGrand->next_sibling()) //cú pháp lập
					{
						CZombie* zombie = new CZombie();
						const int x = std::atoi(oGrand->first_attribute("x")->value());
						const int y = std::atoi(oGrand->first_attribute("y")->value());
						const int col = std::atoi(oGrand->first_attribute("col")->value());
						const int row = std::atoi(oGrand->first_attribute("row")->value());
						const int nx = std::atoi(oGrand->first_attribute("nx")->value());
						const int bboxEnemyWidth = std::atoi(oGrand->first_attribute("bboxEnemyWidth")->value());
						const int bboxEnemyHeight = std::atoi(oGrand->first_attribute("bboxEnemyHeight")->value());
						zombie->SetPosition(x, y);
						zombie->SetCellIndex(col, row);
						zombie->SetBboxEnemy(bboxEnemyWidth, bboxEnemyHeight);
						zombie->SetNx(nx);
						zombie->SetEntryPosition(x, y);
						Add(zombie);
					}
				}
				else if (nodeNameGrand == "boss")
				{
					CBoss* boss = new CBoss();
					const int x = std::atoi(grand->first_attribute("x")->value());
					const int y = std::atoi(grand->first_attribute("y")->value());
					const int col = std::atoi(grand->first_attribute("col")->value());
					const int row = std::atoi(grand->first_attribute("row")->value());
					const int bboxEnemyWidth = std::atoi(grand->first_attribute("bboxEnemyWidth")->value());
					const int bboxEnemyHeight = std::atoi(grand->first_attribute("bboxEnemyHeight")->value());
					const int bboxEnemyActiveWidth = std::atoi(grand->first_attribute("bboxEnemyActiveWidth")->value());
					const int bboxEnemyActiveHeight = std::atoi(grand->first_attribute("bboxEnemyActiveHeight")->value());
					boss->SetPosition(x, y);
					boss->SetCellIndex(col, row);
					boss->SetBboxEnemy(bboxEnemyWidth, bboxEnemyHeight);
					boss->SetBboxEnemyActive(bboxEnemyActiveWidth, bboxEnemyActiveHeight);
					boss->SetEntryPosition(x, y);
					Add(boss);
				}
			}
		}
	}




}
