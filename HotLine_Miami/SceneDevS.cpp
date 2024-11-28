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

	Scene::Init();
}

void SceneDevS::Enter()
{
	Scene::Enter();
	tileMap->SetTexture(&TEXTURE_MGR.Get(STAGE_TABLE->GetTileTextureId()));
	tileMap->Initialize(STAGE_TABLE->GetTileSize(), STAGE_TABLE->GetTileCount(), STAGE_TABLE->GetFloorTiles());
	LoadDecorations();
	LoadWalls();
	LoadEnemies();
	worldView.setSize(windowSize * 0.5f);
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

		Wall* wall = new Wall("Wall");
		wall->Reset();
		wall->DrawWall(wallData);
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
		deco->Reset();
		deco->SetTexture(decoData.textureId);
		deco->SetPosition(decoData.pos);
		deco->SetRotation(decoData.rotation);

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
		enemy->Reset();
		enemy->SetStatus(enemyData.state);
		enemy->SetPosition({ enemyData.pos.x * 16.f + 8.f,  enemyData.pos.y * 16.f + 8.f });

		AddGo(enemy);
	}
}
