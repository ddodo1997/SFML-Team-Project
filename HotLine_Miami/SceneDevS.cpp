#include "stdafx.h"
#include "SceneDevS.h"
#include "TileMap.h"
#include "TileMapEditor.h"
#include "StageTable.h"
#include "Decoration.h"
#include "Enemy.h"
#include "Wall.h"


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
	tileMap->InitializeEmpty(STAGE_TABLE->GetTileSize(), {40, 40});
	//tileMap->Initialize(STAGE_TABLE->GetTileSize(), STAGE_TABLE->GetTileCount(), STAGE_TABLE->GetFloorTiles());
	// SetStatusEnemies();
	worldView.setSize(windowSize * zoomNoun);
	worldView.setCenter(0, 0);
	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);
}

void SceneDevS::Exit()
{
	Scene::Exit();
}

void SceneDevS::Update(float dt)
{
	float cameraSpeed = 300.f * dt;

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

	sf::Vector2i mousePos = InputMgr::GetMousePosition();
	if (InputMgr::GetMouseButton(sf::Mouse::Left))
	{
		sf::Vector2f worldPos = ScreenToWorld(mousePos);
		tileMap->PaintTile(worldPos, tileMapEditor->GetSelectedTileIndex());
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Delete))
	{
		tileMap->InitializeEmpty(STAGE_TABLE->GetTileSize(), { 40, 40 });
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

void SceneDevS::SaveMap() const
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

