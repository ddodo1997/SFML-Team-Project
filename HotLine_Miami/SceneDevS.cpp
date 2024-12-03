#include "stdafx.h"
#include "SceneDevS.h"
#include "TileMap.h"
#include "TileMapEditor.h"
#include "StageTable.h"
#include "WallTable.h"
#include "Decoration.h"
#include "Enemy.h"
#include "Wall.h"
#include "Wall2.h"


SceneDevS::SceneDevS() : Scene(SceneIds::DevS)
{
}

void SceneDevS::Init()
{
	tileMap = AddGo(new TileMap("Tile Map"));
	tileMapEditor = AddGo(new TileMapEditor("Tile Map Editor"));
	// LoadDecorations();
	// LoadWalls();
	// LoadEnemies();

	Scene::Init();

}

void SceneDevS::Enter()
{
	Scene::Enter();
	tileMap->SetTexture(&TEXTURE_MGR.Get(STAGE_TABLE->GetTileTextureId()));
	tileMap->InitializeEmpty(STAGE_TABLE->GetTileSize(), { 40, 40 });
	//tileMap->Initialize(STAGE_TABLE->GetTileSize(), STAGE_TABLE->GetTileCount(), STAGE_TABLE->GetFloorTiles());
	// SetStatusEnemies();
	worldView.setSize(windowSize * zoomNoun);
	worldView.setCenter(0, 0);
	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);
	tileSize = STAGE_TABLE->GetTileSize();
	tileCount = tileMap->GetTileCount();
}

void SceneDevS::Exit()
{
	Scene::Exit();
}

void SceneDevS::Update(float dt)
{
	float cameraSpeed = 300.f * dt;
	sf::Vector2i mousePos = InputMgr::GetMousePosition();
	sf::Vector2f worldPos = ScreenToWorld(mousePos);

	direction.x = InputMgr::GetAxis(Axis::Horizontal);
	direction.y = InputMgr::GetAxis(Axis::Vertical);
	float mag = Utils::Magnitude(direction);
	if (mag > 1.f)
	{
		Utils::Normalize(direction);
	}
	worldView.move(cameraSpeed * direction);

	float wheelDelta = InputMgr::GetMouseWheelDelta();


	if (wheelDelta > 0)
	{
		zoomNoun -= 0.1f;
	}
	else if (wheelDelta < 0)
	{
		zoomNoun += 0.1f;
	}

	zoomNoun = Utils::Clamp(zoomNoun, 0.2f, 0.5f);

	worldView.setSize(windowSize * zoomNoun);

	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		tileMapEditor->SetMode(TileMapEditor::EditorMode::TileMode);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		tileMapEditor->SetMode(TileMapEditor::EditorMode::WallMode);
	}

	
	if (InputMgr::GetMouseButton(sf::Mouse::Left) && tileMapEditor->GetMode() == TileMapEditor::EditorMode::TileMode)
	{
		tileMap->PaintTile(worldPos, tileMapEditor->GetSelectedTileIndex());
	}

	if(InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		switch (tileMapEditor->GetMode())
		{
		case TileMapEditor::EditorMode::WallMode:
			if (worldPos.x <  0.f || worldPos.x > tileSize.x * tileCount.x || worldPos.y <  0.f || worldPos.y > tileSize.y * tileCount.y)
			{
				break;
			}
			CreateWall(worldPos);
			break;
		case TileMapEditor::EditorMode::DecorationMode:
			break;
		case TileMapEditor::EditorMode::EnemyMode:
			break;
		}
	}


	if (InputMgr::GetKeyDown(sf::Keyboard::Delete))
	{
		tileMap->InitializeEmpty(STAGE_TABLE->GetTileSize(), { 40, 40 });
		DeleteWalls();
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		SaveMap();
	}
	Scene::Update(dt);
}

void SceneDevS::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneDevS::CreateWall(const sf::Vector2f& pos)
{
	int xIndex = static_cast<int>(pos.x) / tileSize.x;
	int yIndex = static_cast<int>(pos.y) / tileSize.y;

	sf::Vector2f tileCenter = { (xIndex + 0.5f) * tileSize.x, (yIndex + 0.5f) * tileSize.y };
	sf::Vector2f delta = pos - tileCenter;

	sf::Vector2f wallPosition;
	Wall::Types wallType = WALL_TABLE->GetWall(tileMapEditor->GetSelectedWallTextureId()).type;

	sf::Vector2f wallStart, wallEnd;

	if (wallType == Wall::Types::Horizontal)
	{
		if (std::abs(delta.x) < std::abs(delta.y))
		{
			int yDirection = (delta.y > 0 ? 1 : 0);
			wallPosition = { static_cast<float>(xIndex * tileSize.x), static_cast<float>((yIndex + yDirection) * tileSize.y) };
			wallStart = { wallPosition.x - tileSize.x, wallPosition.y };
			wallEnd = { wallPosition.x + tileSize.x, wallPosition.y };
			if (wallStart.x < 0.f || wallEnd.x > tileSize.x * tileCount.x)
			{
				return;
			}
			for (const auto& createdWall : wallsHorizontal)
			{
				sf::Vector2f createdStart = createdWall->GetStartPosition();
				sf::Vector2f createdEnd = { createdStart.x + tileSize.x * 2, createdStart.y };

				if ((wallStart.x < createdEnd.x && wallEnd.x > createdStart.x) && wallStart.y == createdStart.y)
				{
					return;
				}
			}
		}
		else
		{
			return;
		}
	}
	else if (wallType == Wall::Types::Vertical)
	{
		if (std::abs(delta.y) < std::abs(delta.x))
		{
			int xDirection = (delta.x > 0 ? 1 : 0);
			wallPosition = { static_cast<float>((xIndex + xDirection) * tileSize.x), static_cast<float>(yIndex * tileSize.y) };
			wallStart = { wallPosition.x, wallPosition.y - tileSize.y };
			wallEnd = { wallPosition.x, wallPosition.y + tileSize.y };
			if (wallStart.y < 0.f || wallEnd.y > tileSize.y * tileCount.y)
			{
				return;
			}
			for (const auto& createdWall : wallsVertical)
			{
				sf::Vector2f createdStart = createdWall->GetStartPosition();
				sf::Vector2f createdEnd = { createdStart.x, createdStart.y + tileSize.y * 2 };

				if ((wallStart.y < createdEnd.y && wallEnd.y > createdStart.y) && wallStart.x == createdStart.x)
				{
					return;
				}
			}
		}
		else
		{
			return;
		}
	}

	// 벽 생성 및 추가
	Wall2* wall = new Wall2("Wall");
	wall->SetPosition(wallPosition);
	wall->SetStartPostion(wallStart);
	wall->SetEndPosition(wallEnd);
	wall->SetId(tileMapEditor->GetSelectedWallTextureId());
	wall->SetTexture(WALL_TABLE->GetWall(tileMapEditor->GetSelectedWallTextureId()).textureId);
	wall->SetOrigin(Origins::MC);

	if (wallType == Wall::Types::Horizontal)
	{
		wallsHorizontal.push_back(wall);
	}
	else
	{
		wallsVertical.push_back(wall);
	}

	AddGo(wall);
}

void SceneDevS::LoadWalls()
{
	const auto& wallTable = STAGE_TABLE->GetWallTable();

	for (const auto& WallPair : wallTable)
	{
		const DataWall& wallData = WallPair.second;

		Wall* wall = new Wall(wallData.id);
		//wall->Reset();
		wall->DrawWall(wallData);
		walls.push_back(wall);
		AddGo(wall);
	}
}

void SceneDevS::LoadDecorations()
{
	const auto& decoTable = STAGE_TABLE->GetDecoTable();
	for (const auto& decoPair : decoTable)
	{
		const DataDecoration& decoData = decoPair.second;

		Decoration* deco = new Decoration(decoData.id);
		//deco->Reset();
		deco->SetTexture(decoData.textureId);
		deco->SetRotation(decoData.rotation);
		deco->SetPosition(decoData.pos);
		decorations.push_back(deco);
		AddGo(deco);
	}
}

void SceneDevS::LoadEnemies()
{
	const auto& enemyTable = STAGE_TABLE->GetEnemyTable();
	for (const auto& enemyPair : enemyTable)
	{
		const DataEnemy& enemyData = enemyPair.second;

		Enemy* enemy = new Enemy(enemyData.id);
		// enemy->SetStatus(enemyData.state);
		enemy->SetPosition({ (enemyData.pos.x * STAGE_TABLE->GetTileSize().x) + 8.f,  (enemyData.pos.y * STAGE_TABLE->GetTileSize().y) + 8.f});
		enemy->SetWayPoints(enemyData.waypoints);

		enemies.push_back(enemy);
		AddGo(enemy);
	}
}

void SceneDevS::SetStatusEnemies()
{
	int i = 0;
	const auto& enemyTable = STAGE_TABLE->GetEnemyTable();
	for (const auto& enemyPair : enemyTable)
	{
		enemies[i++]->SetStatus(enemyPair.second.state);
	}
}

void SceneDevS::DeleteWalls()
{
	for (auto wall : wallsHorizontal)
	{
		RemoveGo(wall);
	}
	wallsHorizontal.clear();
	for (auto wall : wallsVertical)
	{
		RemoveGo(wall);
	}
	wallsVertical.clear();
}

void SceneDevS::SaveMap()
{
	json mapData;

	mapData["tileSizeX"] = tileMap->GetTileSize().x;
	mapData["tileSizeY"] = tileMap->GetTileSize().y;

	const sf::Vector2i& tileCount = tileMap->GetTileCount();
	const std::vector<int>& floorTiles = tileMap->GetFloorTiles();

	int minX = tileCount.x, minY = tileCount.y;
	int maxX = -1, maxY = -1;


	for (int y = 0; y < tileCount.y; y++)
	{
		for (int x = 0; x < tileCount.x; x++)
		{
			int tileIndex = y * tileCount.x + x;
			if (floorTiles[tileIndex] != -1)  // 유효한 타일이 있는 경우
			{
				if (x < minX)
				{
					minX = x;
				}
				if (y < minY)
				{
					minY = y;
				}
				if (x > maxX)
				{
					maxX = x;
				}
				if (y > maxY)
				{
					maxY = y;
				}
			}
		}
	}

	if (maxX == -1 || maxY == -1)
	{
		std::cout << "No tiles to save." << std::endl;
		return;
	}

	mapData["tileCountX"] = maxX - minX + 1;
	mapData["tileCountY"] = maxY - minY + 1;

	mapData["floor"]["textureId"] = STAGE_TABLE->GetTileTextureId();

	std::vector<int> resizedTiles((maxY - minY + 1) * (maxX - minX + 1), -1);

	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			int oldIndex = y * tileCount.x + x;
			int newIndex = (y - minY) * (maxX - minX + 1) + (x - minX);
			resizedTiles[newIndex] = floorTiles[oldIndex];
		}
	}

	mapData["floor"]["tiles"] = resizedTiles;

	SaveWall();
	mapData["wall"]["filePath"] = WALL_TABLE->GetFilePath();

	for (const auto& wall : mergedHorizontalWalls) {
		json wallData;
		wallData["id"] = wall.id;
		wallData["start"]["x"] = (int)wall.start.x / tileSize.x - minX;
		wallData["start"]["y"] = (int)wall.start.y / tileSize.y - minY;
		wallData["end"]["x"] = (int)wall.end.x / tileSize.x - minX;
		wallData["end"]["y"] = (int)wall.end.y / tileSize.y - minY;
		wallData["ids"] = wall.textureIds;

		mapData["wall"]["walls"].push_back(wallData);
	}

	for (const auto& wall : mergedVerticalWalls) {
		json wallData;
		wallData["id"] = wall.id;
		wallData["start"]["x"] = (int)wall.start.x / tileSize.x - minX;
		wallData["start"]["y"] = (int)wall.start.y / tileSize.y - minY;
		wallData["end"]["x"] = (int)wall.end.x / tileSize.x - minX;
		wallData["end"]["y"] = (int)wall.end.y / tileSize.y - minY;
		wallData["ids"] = wall.textureIds;

		mapData["wall"]["walls"].push_back(wallData);
	}

	std::ofstream outFile("tables/Test_Save.json");
	if (outFile.is_open())
	{
		outFile << mapData.dump(4);
		outFile.close();
		std::cout << "save success!!" << std::endl;
	}
	else
	{
		std::cerr << "save Failed.." << std::endl;
	}
}

void SceneDevS::SaveWall()
{
	std::vector<bool> processedHorizontal(wallsHorizontal.size(), false);
	std::vector<bool> processedVertical(wallsVertical.size(), false);

	for (int i = 0; i < wallsHorizontal.size(); i++)
	{
		if (processedHorizontal[i])
		{
			continue;
		}

		DataWall wallData;
		wallData.id = "Horizontal_" + std::to_string(i);
		
		wallData.start = wallsHorizontal[i]->GetStartPosition();
		wallData.end = wallsHorizontal[i]->GetEndPosition();
		wallData.textureIds.push_back(wallsHorizontal[i]->GetId());

		processedHorizontal[i] = true;

		for (int j = i + 1; j < wallsHorizontal.size(); j++)
		{
			if (processedHorizontal[j])
			{
				continue;
			}

			if (wallData.end == wallsHorizontal[j]->GetStartPosition())
			{
				wallData.end = wallsHorizontal[j]->GetEndPosition();
				wallData.textureIds.push_back(wallsHorizontal[j]->GetId());
				processedHorizontal[j] = true;
			}
			else if (wallData.start == wallsHorizontal[j]->GetEndPosition())
			{
				wallData.start = wallsHorizontal[j]->GetStartPosition();
				wallData.textureIds.insert(wallData.textureIds.begin(), wallsHorizontal[j]->GetId());
				processedHorizontal[j] = true;
			}
		}

		mergedHorizontalWalls.push_back(wallData);
	}

	for (int i = 0; i < wallsVertical.size(); i++)
	{
		if (processedVertical[i])
		{
			continue;
		}

		DataWall wallData;
		wallData.id = "Vertical_" + std::to_string(i);
		wallData.start = wallsVertical[i]->GetStartPosition();
		wallData.end = wallsVertical[i]->GetEndPosition();
		wallData.textureIds.push_back(wallsVertical[i]->GetId());

		processedVertical[i] = true;

		for (int j = i + 1; j < wallsVertical.size(); j++)
		{
			if (processedVertical[j])
			{
				continue;
			}

			if (wallData.end == wallsVertical[j]->GetStartPosition())
			{
				wallData.end = wallsVertical[j]->GetEndPosition();
				wallData.textureIds.push_back(wallsVertical[j]->GetId());
				processedVertical[j] = true;
			}
			else if (wallData.start == wallsVertical[j]->GetEndPosition())
			{
				wallData.start = wallsVertical[j]->GetStartPosition();
				wallData.textureIds.insert(wallData.textureIds.begin(), wallsVertical[j]->GetId());
				processedVertical[j] = true;
			}
		}

		mergedVerticalWalls.push_back(wallData);
	}
}

