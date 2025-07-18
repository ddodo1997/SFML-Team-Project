#include "stdafx.h"
#include "SceneEditor.h"
#include "TileMap.h"
#include "TileMapEditor.h"
#include "StageTable.h"
#include "WallTable.h"
#include "Decoration.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Wall.h"
#include "Wall2.h"
#include "Player.h"
#include "Boss1.h"
#include "Boss2.h"
#include "WayPoint.h"


SceneEditor::SceneEditor() : SceneGame()
{
	id = SceneIds::SceneEditor;
}

void SceneEditor::Init()
{
	VIEW_MGR.Init();
	tileMap = AddGo(new TileMap("Tile Map"));
	tileMapEditor = AddGo(new TileMapEditor("Tile Map Editor"));
	// LoadDecorations();
	// LoadWalls();
	// LoadEnemies();

	Scene::Init();
}

void SceneEditor::Enter()
{
	Scene::Enter();
	FRAMEWORK.GetWindow().setMouseCursorVisible(true);
	tileMap->SetTexture(&TEXTURE_MGR.Get(STAGE_TABLE->GetCurrentStage().GetTileTextureId()));
	tileMap->InitializeEmpty(STAGE_TABLE->GetTileSize(), { 40, 40 });
	//tileMap->Initialize(STAGE_TABLE->GetTileSize(), STAGE_TABLE->GetTileCount(), STAGE_TABLE->GetFloorTiles());
	// SetStatusEnemies();
	worldView.setSize(windowSize * zoomNoun);
	worldView.setCenter(0, 0);
	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);
	VIEW_MGR.SetCurrentSceneWorldView(&worldView);
	VIEW_MGR.SetCurrentSceneUiView(&uiView);
	tileSize = STAGE_TABLE->GetTileSize();
	tileCount = tileMap->GetTileCount();
	isWayPointMode = false;
}

void SceneEditor::Exit()
{
	Scene::Exit();
}

void SceneEditor::Update(float dt)
{
	Scene::Update(dt);
	VIEW_MGR.Update(dt);
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

	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneIds::SceneMenu);
	}

	if(!isWayPointMode)
	{
		if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
		{
			tileMapEditor->SetMode(TileMapEditor::EditorMode::TileMode);
		}
		if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
		{
			tileMapEditor->SetMode(TileMapEditor::EditorMode::WallMode);
		}
		if (InputMgr::GetKeyDown(sf::Keyboard::Num3))
		{
			tileMapEditor->SetMode(TileMapEditor::EditorMode::EnemyMode);
		}
		if (InputMgr::GetKeyDown(sf::Keyboard::Num4))
		{
			tileMapEditor->SetMode(TileMapEditor::EditorMode::WeaponMode);
		}
		if (InputMgr::GetKeyDown(sf::Keyboard::Num5))
		{
			tileMapEditor->SetMode(TileMapEditor::EditorMode::PlayerAndBossMode);
		}

		if (InputMgr::GetMouseButton(sf::Mouse::Left))
		{
			if (tileMapEditor->GetGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
			{
				return;
			}

			switch (tileMapEditor->GetMode())
			{
			case TileMapEditor::EditorMode::TileMode:
				tileMap->PaintTile(worldPos, tileMapEditor->GetSelectedTileIndex());
				break;
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
				if (worldPos.x <  0.f || worldPos.x > tileSize.x * tileCount.x || worldPos.y <  0.f || worldPos.y > tileSize.y * tileCount.y)
				{
					break;
				}
				CreateEnemy(worldPos);
				break;
			case TileMapEditor::EditorMode::WeaponMode:
				if (worldPos.x <  0.f || worldPos.x > tileSize.x * tileCount.x || worldPos.y <  0.f || worldPos.y > tileSize.y * tileCount.y)
				{
					break;
				}
				CreateWeapon(worldPos);
				break;
			case TileMapEditor::EditorMode::PlayerAndBossMode:
				if (worldPos.x <  0.f || worldPos.x > tileSize.x * tileCount.x || worldPos.y <  0.f || worldPos.y > tileSize.y * tileCount.y)
				{
					break;
				}
				if (tileMapEditor->GetSelectedPlayerOrBoss() == "Player")
				{
					CreatePlayer(worldPos);
				}
				else if (tileMapEditor->GetSelectedPlayerOrBoss() == "Boss1")
				{
					CreateBoss1(worldPos);
				}
				else if (tileMapEditor->GetSelectedPlayerOrBoss() == "Boss2")
				{
					CreateBoss2(worldPos);
				}
				break;
			}
		}

		if (InputMgr::GetMouseButtonDown(sf::Mouse::Right) && tileMapEditor->GetMode() == TileMapEditor::EditorMode::TileMode)
		{
			CreateEndPoint(worldPos);
		}

		if (InputMgr::GetMouseButton(sf::Mouse::Right))
		{
			if (tileMapEditor->GetGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
			{
				return;
			}

			switch (tileMapEditor->GetMode())
			{
			case TileMapEditor::EditorMode::TileMode:
				break;
			case TileMapEditor::EditorMode::WallMode:
				if (worldPos.x <  0.f || worldPos.x > tileSize.x * tileCount.x || worldPos.y <  0.f || worldPos.y > tileSize.y * tileCount.y)
				{
					break;
				}
				DeleteWall(worldPos);
				break;
			case TileMapEditor::EditorMode::DecorationMode:
				break;
			case TileMapEditor::EditorMode::EnemyMode:
				if (worldPos.x <  0.f || worldPos.x > tileSize.x * tileCount.x || worldPos.y <  0.f || worldPos.y > tileSize.y * tileCount.y)
				{
					break;
				}
				DeleteEnemy(worldPos);
				break;
			case TileMapEditor::EditorMode::WeaponMode:
				if (worldPos.x <  0.f || worldPos.x > tileSize.x * tileCount.x || worldPos.y <  0.f || worldPos.y > tileSize.y * tileCount.y)
				{
					break;
				}
				DeleteWeapon(worldPos);
				break;
			case TileMapEditor::EditorMode::PlayerAndBossMode:
				if (worldPos.x <  0.f || worldPos.x > tileSize.x * tileCount.x || worldPos.y <  0.f || worldPos.y > tileSize.y * tileCount.y)
				{
					break;
				}
				if (tileMapEditor->GetSelectedPlayerOrBoss() == "Player")
				{
					DeletePlayer();
				}
				else if (tileMapEditor->GetSelectedPlayerOrBoss() == "Boss1")
				{
					DeleteBoss1();
				}
				else if (tileMapEditor->GetSelectedPlayerOrBoss() == "Boss2")
				{
					DeleteBoss2();
				}
				break;
			}
		}

		if (InputMgr::GetKeyDown(sf::Keyboard::F9))
		{
			Variables::isDrawHitBox = !Variables::isDrawHitBox;
		}

		if (InputMgr::GetKeyDown(sf::Keyboard::Delete))
		{
			tileMap->InitializeEmpty(STAGE_TABLE->GetTileSize(), { 40, 40 });
			DeleteWalls();
			DeleteEnemies();
			DeleteWaypoints();
			DeleteWeapons();
			DeletePlayer();
			DeleteBoss1();
			DeleteBoss2();
			DeleteEndPoints();

			minX = 0;
			minY = 0;
			zoomNoun = 0.5f;
			// SCENE_MGR.ChangeScene(SceneIds::DevS);
		}
		if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
		{
			SaveMap();
		}


	}
	else
	{
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
		{
			AddWayPoints(worldPos);
		}
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Right))
		{
			DeleteWaypoint(worldPos);
		}
		if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
		{
			patrolEnemy = nullptr;
			isWayPointMode = false;
		}
	}
}

void SceneEditor::Draw(sf::RenderWindow& window)
{
	VIEW_MGR.DrawBackground();
	Scene::Draw(window);
}

void SceneEditor::CreateWall(const sf::Vector2f& pos)
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

	// �� ���� �� �߰�
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

void SceneEditor::CreateEnemy(const sf::Vector2f& pos)
{
	int xIndex = static_cast<int>(pos.x) / tileSize.x;
	int yIndex = static_cast<int>(pos.y) / tileSize.y;

	sf::Vector2f enemyPosition = { xIndex * tileSize.x + tileSize.x * 0.5f, yIndex * tileSize.y + tileSize.y * 0.5f };
	Enemy selectedEnemy = tileMapEditor->GetSelectedEnemy();

	for (auto& enemy : createdEnemies)
	{
		if (enemyPosition == enemy.second->GetPosition())
		{
			enemy.first = tileMapEditor->GetSelectedEnemyStatus();
			enemy.second->SetRotation(selectedEnemy.GetRotation());
			enemy.second->SetWeapon(selectedEnemy.GetWeaponType());
			enemy.second->SetStatus(Enemy::Status::EditorMode);
			if (enemy.first == Enemy::Status::Patrol)
			{
				StartWayPointMode(enemy.second);
			}
			return;
		}
	}

	Enemy* enemy = new Enemy("Enemy");
	enemy->Reset();
	enemy->SetPosition(enemyPosition);
	enemy->SetRotation(selectedEnemy.GetRotation());
	enemy->SetOrigin(Origins::MC);
	enemy->SetWeapon(selectedEnemy.GetWeaponType());
	enemy->SetStatus(Enemy::Status::EditorMode);

	createdEnemies.push_back(std::make_pair(tileMapEditor->GetSelectedEnemyStatus(), enemy));
	AddGo(enemy);

	if (enemy->GetStatus() == Enemy::Status::Patrol)
	{
		StartWayPointMode(enemy);
	}
}

void SceneEditor::StartWayPointMode(Enemy* enemy)
{
	isWayPointMode = true;
	waypoints.clear();
	waypointColor = Utils::RandomColor();
	enemy->AddWayPoint(enemy->GetPosition());
	WayPoint* waypoint = new WayPoint("WayPoint");
	waypoint->Reset();
	waypoint->SetPosition(enemy->GetPosition());
	waypoint->SetColor(waypointColor);
	AddGo(waypoint);
	waypoints.push_back(waypoint);
	patrolEnemy = enemy;
}

void SceneEditor::AddWayPoints(const sf::Vector2f& pos)
{
	int xIndex = static_cast<int>(pos.x) / tileSize.x;
	int yIndex = static_cast<int>(pos.y) / tileSize.y;
	sf::Vector2f pointPosition = { (xIndex * tileSize.x) + tileSize.x * 0.5f, (yIndex * tileSize.y) + tileSize.y * 0.5f };

	for (const auto& waypoint : waypoints)
	{
		if (pointPosition == waypoint->GetPosition())
		{
			return;
		}
	}

	if (patrolEnemy)
	{
		WayPoint* waypoint = new WayPoint("WayPoint");
		waypoint->Reset();
		waypoint->SetPosition(pointPosition);
		waypoint->SetColor(waypointColor);
		patrolEnemy->AddWayPoint(pointPosition);
		AddGo(waypoint);
		waypoints.push_back(waypoint);
	}
}

void SceneEditor::CreateWeapon(const sf::Vector2f& pos)
{
	int xIndex = static_cast<int>(pos.x) / tileSize.x;
	int yIndex = static_cast<int>(pos.y) / tileSize.y;

	sf::Vector2f weaponPosition = { xIndex * tileSize.x + tileSize.x * 0.5f, yIndex * tileSize.y + tileSize.y * 0.5f };
	Weapon selectedWeapon = tileMapEditor->GetSelectedWeapon();

	for (auto& weapon : createdWeapons)
	{
		if (weaponPosition == weapon->GetPosition())
		{
			weapon->SetRotation(selectedWeapon.GetRotation());
			weapon->SetWeaponType(selectedWeapon.GetWeaponType());
			return;
		}
	}

	Weapon* weapon = new Weapon("Weapon");
	weapon->Reset();
	weapon->SetPosition(weaponPosition);
	weapon->SetRotation(selectedWeapon.GetRotation());
	weapon->SetOrigin(Origins::MC);
	weapon->SetWeaponType(selectedWeapon.GetWeaponType());

	createdWeapons.push_back(weapon);
	AddGo(weapon);
}

void SceneEditor::CreatePlayer(const sf::Vector2f& pos)
{
	int xIndex = static_cast<int>(pos.x) / tileSize.x;
	int yIndex = static_cast<int>(pos.y) / tileSize.y;

	sf::Vector2f playerPosition = { xIndex * tileSize.x + tileSize.x * 0.5f, yIndex * tileSize.y + tileSize.y * 0.5f };
	if (player != nullptr)
	{
		player->SetPosition(playerPosition);
		player->SetRotation(tileMapEditor->GetPlayer()->GetRotation());
		return;
	}

	player = new Player("Player");
	player->Reset();
	player->UpdateBodyAnimationMoving();
	player->SetControlable(false);
	player->SetPosition(playerPosition);
	player->SetRotation(tileMapEditor->GetPlayer()->GetRotation());

	AddGo(player);
}

void SceneEditor::CreateBoss1(const sf::Vector2f& pos)
{
	int xIndex = static_cast<int>(pos.x) / tileSize.x;
	int yIndex = static_cast<int>(pos.y) / tileSize.y;

	sf::Vector2f boss1Position = { xIndex * tileSize.x + tileSize.x * 0.5f, yIndex * tileSize.y + tileSize.y * 0.5f };
	if (boss1 != nullptr)
	{
		boss1->SetPosition(boss1Position);
		boss1->SetRotation(tileMapEditor->GetBoss1()->GetRotation());
		return;
	}

	boss1 = new Boss1("Boss1");
	boss1->Reset();
	boss1->SetPosition(boss1Position);
	boss1->SetRotation(tileMapEditor->GetBoss1()->GetRotation());

	AddGo(boss1);
}

void SceneEditor::CreateBoss2(const sf::Vector2f& pos)
{
	if (boss2 != nullptr)
	{
		boss2->SetPosition(pos);
		return;
	}

	boss2 = new Boss2("Boss2");
	boss2->Reset();
	boss2->SetPosition(pos);

	AddGo(boss2);
}

void SceneEditor::CreateEndPoint(const sf::Vector2f& pos)
{
	int xIndex = static_cast<int>(pos.x) / tileSize.x;
	int yIndex = static_cast<int>(pos.y) / tileSize.y;

	if (xIndex < 0 || xIndex >= tileCount.x || yIndex < 0 || yIndex >= tileCount.y)
	{
		return;
	}

	sf::Vector2f pointPosition = { (xIndex * tileSize.x) + tileSize.x * 0.5f, (yIndex * tileSize.y) + tileSize.y * 0.5f };

	for (int i = 0; i < endPoints.size(); i++)
	{
		if (endPoints[i]->GetGlobalBounds().contains(pos))
		{
			RemoveGo(endPoints[i]);
			endPoints.erase(endPoints.begin() + i);
			return;
		}
	}

	WayPoint* endPoint = new WayPoint("WayPoint");
	endPoint->Reset();
	endPoint->SetPosition(pointPosition);
	AddGo(endPoint);
	endPoints.push_back(endPoint);
}

void SceneEditor::LoadWalls()
{
	const auto& wallTable = STAGE_TABLE->GetCurrentStage().GetWallTable();

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

void SceneEditor::LoadDecorations()
{
	const auto& decoTable = STAGE_TABLE->GetCurrentStage().GetDecoTable();
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

void SceneEditor::LoadEnemies()
{
	const auto& enemyTable = STAGE_TABLE->GetCurrentStage().GetEnemyTable();
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

void SceneEditor::SetStatusEnemies()
{
	int i = 0;
	const auto& enemyTable = STAGE_TABLE->GetCurrentStage().GetEnemyTable();
	for (const auto& enemyPair : enemyTable)
	{
		enemies[i++]->SetStatus(enemyPair.second.state);
	}
}

void SceneEditor::DeleteWall(const sf::Vector2f& pos)
{
	for (int i = 0; i < wallsHorizontal.size(); i++)
	{
		if (wallsHorizontal[i]->GetGlobalBounds().contains(pos))
		{
			RemoveGo(wallsHorizontal[i]);
			wallsHorizontal.erase(wallsHorizontal.begin() + i);
		}
	}
	for (int i = 0; i < wallsVertical.size(); i++)
	{
		if (wallsVertical[i]->GetGlobalBounds().contains(pos))
		{
			RemoveGo(wallsVertical[i]);
			wallsVertical.erase(wallsVertical.begin() + i);
		}
	}
}

void SceneEditor::DeleteEnemy(const sf::Vector2f& pos)
{
	for (int i = 0; i < createdEnemies.size(); i++)
	{
		if (createdEnemies[i].second->GetGlobalBounds().contains(pos))
		{
			RemoveGo(createdEnemies[i].second);
			createdEnemies.erase(createdEnemies.begin() + i);
		}
	}
}

void SceneEditor::DeleteWaypoint(const sf::Vector2f& pos)
{
	for (int i = 0; i < waypoints.size(); i++)
	{
		if (waypoints[i]->GetGlobalBounds().contains(pos))
		{
			RemoveGo(waypoints[i]);
			waypoints.erase(waypoints.begin() + i);
		}
	}
}

void SceneEditor::DeleteWeapon(const sf::Vector2f& pos)
{
	for (int i = 0; i < createdWeapons.size(); i++)
	{
		if (createdWeapons[i]->GetGlobalBounds().contains(pos))
		{
			RemoveGo(createdWeapons[i]);
			createdWeapons.erase(createdWeapons.begin() + i);
		}
	}
}

void SceneEditor::DeleteEndPoint(const sf::Vector2f& pos)
{
	for (int i = 0; i < endPoints.size(); i++)
	{
		if (endPoints[i]->GetGlobalBounds().contains(pos))
		{
			RemoveGo(endPoints[i]);
			endPoints.erase(endPoints.begin() + i);
		}
	}
}

void SceneEditor::DeleteWalls()
{
	for (auto& wall : wallsHorizontal)
	{
		RemoveGo(wall);
	}
	wallsHorizontal.clear();
	for (auto& wall : wallsVertical)
	{
		RemoveGo(wall);
	}
	wallsVertical.clear();
}

void SceneEditor::DeleteEnemies()
{
	for (auto& enemy : createdEnemies)
	{
		RemoveGo(enemy.second);
	}
	createdEnemies.clear();
}

void SceneEditor::DeleteWaypoints()
{
	for (auto& waypoint : waypoints)
	{
		RemoveGo(waypoint);
	}
	waypoints.clear();
}

void SceneEditor::DeleteWeapons()
{
	for (auto& weapon : createdWeapons)
	{
		RemoveGo(weapon);
	}
	weapons.clear();
}

void SceneEditor::DeletePlayer()
{
	RemoveGo(player);
	player = nullptr;
}

void SceneEditor::DeleteBoss1()
{
	RemoveGo(boss1);
	boss1 = nullptr;
}

void SceneEditor::DeleteBoss2()
{
	RemoveGo(boss2);
	boss2 = nullptr;
}

void SceneEditor::DeleteEndPoints()
{
	for (auto& endPoint : endPoints)
	{
		RemoveGo(endPoint);
	}
	endPoints.clear();
}

void SceneEditor::SaveMap()
{
	json mapData;

	mapData["tileSizeX"] = tileMap->GetTileSize().x;
	mapData["tileSizeY"] = tileMap->GetTileSize().y;

	const sf::Vector2i& tileCount = tileMap->GetTileCount();
	const std::vector<int>& floorTiles = tileMap->GetFloorTiles();

	minX = tileCount.x; 
	minY = tileCount.y;
	int maxX = -1, maxY = -1;


	for (int y = 0; y < tileCount.y; y++)
	{
		for (int x = 0; x < tileCount.x; x++)
		{
			int tileIndex = y * tileCount.x + x;
			if (floorTiles[tileIndex] != -1)  // ��ȿ�� Ÿ���� �ִ� ���
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

	mapData["floor"]["textureId"] = STAGE_TABLE->GetCurrentStage().GetTileTextureId();

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
	mapData["decorations"]["decos"] = json::array();

	SaveWall(mapData);
	SaveEnemies(mapData);
	SaveWeapons(mapData);
	SavePlayer(mapData);
	SaveBoss(mapData);
	SaveBoss2(mapData);
	SaveEndPoint(mapData);

	std::ifstream indexFile("tables/save_data.json");
	int saveIndex = 0;

	json data;
	indexFile >> data;
	indexFile.close();

	saveIndex = data["stage_count"];

	std::string fileName = "tables/stage_" + std::to_string(saveIndex) + ".json";

	std::ofstream outFile(fileName);
	if (outFile.is_open())
	{
		outFile << mapData.dump(4);
		outFile.close();

		std::ofstream indexFileOut("tables/save_data.json");

		json data;
		data["stage_count"] = saveIndex + 1;
		data["save_stage"] = STAGE_TABLE->GetSavedStageIndex();
        indexFileOut << data.dump(4);
        indexFileOut.close();

		std::cout << "save success!!" << std::endl;
	}
	else
	{
		std::cerr << "save Failed.." << std::endl;
	}

	tileMap->InitializeEmpty(STAGE_TABLE->GetTileSize(), { 40, 40 });
	DeleteWalls();
	DeleteEnemies();
	DeleteWaypoints();
	DeleteWeapons();
	DeletePlayer();
	DeleteBoss1();
	DeleteBoss2();
	DeleteEndPoints();

	minX = 0;
	minY = 0;
	zoomNoun = 0.5f;
}

void SceneEditor::SaveWall(json& mapData)
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
}

void SceneEditor::SaveEnemies(json& mapData)
{
	mapData["enemy"]["enemies"] = json::array();
	int i = 1;
	for (const auto& enemyPair : createdEnemies)
	{
		const auto& enemy = enemyPair.second;

		json enemyData;
		enemyData["id"] = "enemy_" + std::to_string(i++);
		enemyData["state"] = "enemy_" + EnemyStatusToString(enemyPair.first) + EnemyWeaponToString(enemy->GetWeaponType());
		enemyData["x"] = static_cast<int>(enemy->GetPosition().x / tileSize.x) - minX;
		enemyData["y"] = static_cast<int>(enemy->GetPosition().y / tileSize.y) - minY;
		enemyData["rotation"] = static_cast<int>(enemy->GetRotation());

		enemyData["waypoint"] = json::array();
		if (enemyPair.first == Enemy::Status::Patrol) {
			for (const auto& waypoint : enemy->GetWayPoints()) {
				json waypointData;
				waypointData["x"] = static_cast<int>(waypoint.position.x / tileSize.x) - minX;
				waypointData["y"] = static_cast<int>(waypoint.position.y / tileSize.y) - minY;
				enemyData["waypoint"].push_back(waypointData);
			}
		}

		mapData["enemy"]["enemies"].push_back(enemyData);
	}
}

void SceneEditor::SaveWeapons(json& mapData)
{
	mapData["weapon"]["weapons"] = json::array();
	int i = 1;

	for (const auto& weapon : createdWeapons)
	{
		json weaponData;
		weaponData["id"] = "weapon_" + std::to_string(i++);
		weaponData["type"] = weapon->GetWeaponType();
		weaponData["x"] = static_cast<int>(weapon->GetPosition().x / tileSize.x) - minX;
		weaponData["y"] = static_cast<int>(weapon->GetPosition().y / tileSize.y) - minY;
		weaponData["rotation"] = static_cast<int>(weapon->GetRotation());


		mapData["weapon"]["weapons"].push_back(weaponData);
	}
}

void SceneEditor::SavePlayer(json& mapData)
{
	if(player != nullptr)
	{
		mapData["player"]["x"] = static_cast<int>(player->GetPosition().x / tileSize.x) - minX;
		mapData["player"]["y"] = static_cast<int>(player->GetPosition().y / tileSize.y) - minY;
		mapData["player"]["rotation"] = player->GetRotation();
	}
}

void SceneEditor::SaveBoss(json& mapData)
{
	if(boss1 != nullptr)
	{
		mapData["boss1"]["x"] = static_cast<int>(boss1->GetPosition().x / tileSize.x) - minX;
		mapData["boss1"]["y"] = static_cast<int>(boss1->GetPosition().y / tileSize.y) - minY;
		mapData["boss1"]["rotation"] = boss1->GetRotation();
	}
}

void SceneEditor::SaveBoss2(json& mapData)
{
	if(boss2 != nullptr)
	{
		mapData["boss2"]["x"] = boss2->GetPosition().x - minX * tileSize.x;
		mapData["boss2"]["y"] = boss2->GetPosition().y - minY * tileSize.y;
	}
}

void SceneEditor::SaveEndPoint(json& mapData)
{
	mapData["EndPosition"] = json::array();
	for (const auto& endPoint : endPoints)
	{
		json endPos;
		endPos["x"] = static_cast<int>(endPoint->GetPosition().x / tileSize.x) - minX;
		endPos["y"] = static_cast<int>(endPoint->GetPosition().y / tileSize.y) - minX;

		mapData["EndPosition"].push_back(endPos);
	}
}

std::string SceneEditor::EnemyStatusToString(const Enemy::Status& state)
{
	if (state == Enemy::Status::Normal)
	{
		return "N";
	}
	else if (state == Enemy::Status::Idle)
	{
		return "I";
	}
	else if (state == Enemy::Status::Patrol)
	{
		return "P";
	}
}

std::string SceneEditor::EnemyWeaponToString(const Weapon::WeaponType& type)
{
	if (type == Weapon::WeaponType::Bat)
	{
		return "B";
	}
	else if (type == Weapon::WeaponType::Knife)
	{
		return "K";
	}
	else if (type == Weapon::WeaponType::Machinegun)
	{
		return "M";
	}
	else if (type == Weapon::WeaponType::Shotgun)
	{
		return "S";
	}
}

