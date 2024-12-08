#include "stdafx.h"
#include "SceneGame.h"
#include "StageTable.h"
#include "Wall.h"
#include "Wall2.h"
#include "Decoration.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Player.h"
#include "TileMap.h"
#include "UiHud.h"
#include "Boss1.h"
#include "Boss2.h"
#include "BodyGuard.h"
#include "Cleaver.h"
#include "PathFinder.h"

SceneGame::SceneGame() : Scene(SceneIds::SceneGame)
{

}

void SceneGame::Init()
{
	player = AddGo(new Player("Player"));
	tileMap = AddGo(new TileMap("Tile Map"));
	uiHud = AddGo(new UiHud());
	cleaver = AddGo(new Cleaver("Cleaver"));
	pathFinder = new PathFinder();
	uiHud->SetPlayer(player);

	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();
}

void SceneGame::Enter()
{
	tileSize = static_cast<sf::Vector2f>(STAGE_TABLE->GetTileSize());
	Scene::Enter();
	// SetWalls_2(); // ���� �� ����� �� 
	LoadCurrentStage();

	worldView.setSize(windowSize * 0.2f);
	//worldView.setCenter(player->GetPosition());

	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);
	VIEW_MGR.Init();
	VIEW_MGR.SetCurrentSceneWorldView(&worldView);
	VIEW_MGR.SetCurrentSceneUiView(&uiView);
	VIEW_MGR.SetCurrentScenePlayer(player);

	SOUND_MGR.PlayBgm("sound/bgm/Paris2.ogg");
}

void SceneGame::Exit()
{
	SaveStage();
	RemoveAllObjPool();
	ClearStage();
	Scene::Exit();
}

void SceneGame::ClearInactivePoolObjects()
{
	auto weapon = weapons.begin();
	while (weapon != weapons.end())
	{
		if (!(*weapon)->IsActive())
		{
			weaponPool.Return(*weapon);
			RemoveGo(*weapon);
			weapon = weapons.erase(weapon);
		}
		else
		{
			weapon++;
		}
	}
	auto bullet = activeBullets.begin();
	while (bullet != activeBullets.end())
	{
		if (!(*bullet)->IsActive())
		{
			bulletPool.Return(*bullet);
			RemoveGo(*bullet);
			bullet = activeBullets.erase(bullet);
		}
		else
		{
			bullet++;
		}
	}
}

void SceneGame::Update(float dt)
{
	VIEW_MGR.Update(dt);
	if (uiHud != nullptr)
	{
		if (uiHud->IsPaused())
		{
			uiHud->Update(dt);
			return;
		}
	}

	IsClearStage();

	if (player->GetGlobalBounds().intersects(endPoint) && IsClearStage())
		LoadNextStage();

	Scene::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::R))
	{
		LoadCurrentStage();
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::T))
	{
		LoadNextStage();
	}
	if (InputMgr::GetKey(sf::Keyboard::Numpad7))
	{
		int newVol = SOUND_MGR.GetBgmVolume();
		newVol--;
		newVol = Utils::Clamp(newVol, 0, 100);
		SOUND_MGR.SetBgmVolume(newVol);
	}
	if (InputMgr::GetKey(sf::Keyboard::Numpad8))
	{
		int newVol = SOUND_MGR.GetBgmVolume();
		newVol++;
		newVol = Utils::Clamp(newVol, 0, 100);
		SOUND_MGR.SetBgmVolume(newVol);
	}
	if (InputMgr::GetKey(sf::Keyboard::Numpad4))
	{
		int newVol = SOUND_MGR.GetSfxVolume();
		newVol--;
		newVol = Utils::Clamp(newVol, 0, 100);
		SOUND_MGR.SetSfxVolume(newVol);
	}
	if (InputMgr::GetKey(sf::Keyboard::Numpad5))
	{
		int newVol = SOUND_MGR.GetSfxVolume();
		newVol++;
		newVol = Utils::Clamp(newVol, 0, 100);
		SOUND_MGR.SetSfxVolume(newVol);
	}

	float randPosX = Utils::RandomRange(20.f, 200.f);
	float randPosY = Utils::RandomRange(10.f, 80.f);
	if (InputMgr::GetKeyDown(sf::Keyboard::F1))
	{
		SpawnWeapon(Weapon::WeaponType::Bat, sf::Vector2f(randPosX, randPosY));
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F2))
	{
		SpawnWeapon(Weapon::WeaponType::Knife, sf::Vector2f(randPosX, randPosY));
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F3))
	{
		SpawnWeapon(Weapon::WeaponType::Machinegun, sf::Vector2f(randPosX, randPosY));
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F4))
	{
		SpawnWeapon(Weapon::WeaponType::Shotgun, sf::Vector2f(randPosX, randPosY));
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::F9))
	{
		Variables::isDrawHitBox = !Variables::isDrawHitBox;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::L))
	{
		std::vector<sf::Vector2f> path = pathFinder->FindPath(player->GetPosition(), enemies[3]->GetPosition());
		for (auto& pos : path)
		{
			std::cout << "(" << (int)pos.x / 16 << ", " << (int)pos.y / 16 << ")" << std::endl;
			player->SetPosition(pos);
		}
	}
	if (std::fabs(directionX) > 1000.f)
	{
		directionX = 0;
	}
	if (std::fabs(directionY) > 1000.f)
	{
		directionY = 0;
	}

	directionXY = { directionX, directionY };

	FRAMEWORK.GetWindow().setView(worldView);

	ClearInactivePoolObjects();
}


void SceneGame::LoadWalls()
{
	const auto& wallTable = STAGE_TABLE->GetCurrentStage().GetWallTable();

	for (const auto& WallPair : wallTable)
	{
		const DataWall& wallData = WallPair.second;

		Wall* wall = AddGo(new Wall(wallData.id));
		wall->Init();
		walls.push_back(wall);
	}
}

void SceneGame::LoadDecorations()
{
	const auto& decoTable = STAGE_TABLE->GetCurrentStage().GetDecoTable();
	for (const auto& decoPair : decoTable)
	{
		const DataDecoration& decoData = decoPair.second;

		Decoration* deco = AddGo(new Decoration(decoData.id));
		deco->Init();
		deco->SetTexture(decoData.textureId);
		deco->SetRotation(decoData.rotation);
		deco->SetPosition(decoData.pos);
		decorations.push_back(deco);
	}
}

void SceneGame::LoadEnemies()
{
	const auto& enemyTable = STAGE_TABLE->GetCurrentStage().GetEnemyTable();
	for (const auto& enemyPair : enemyTable)
	{
		const DataEnemy& enemyData = enemyPair.second;

		Enemy* enemy = AddGo(new Enemy(enemyData.id));
		enemy->Init();
		enemies.push_back(enemy);
	}
}

void SceneGame::SetWalls()
{
	int i = 0;
	const auto& wallTable = STAGE_TABLE->GetCurrentStage().GetWallTable();

	for (const auto& WallPair : wallTable)
	{
		const DataWall& wallData = WallPair.second;
		walls[i]->Reset();
		walls[i]->DrawWall(wallData);
		i++;
	}

}

void SceneGame::SetWalls_2()
{
	int i = 0;
	const auto& wallTable = STAGE_TABLE->GetCurrentStage().GetWallTable();

	for (const auto& WallPair : wallTable)
	{
		const DataWall& wallData = WallPair.second;

		sf::Vector2f start = wallData.start;
		sf::Vector2f end = wallData.end;
		
		if (start.x == end.x)
		{
			int yDirection = (start.y < end.y) ? 1 : -1;
			for (int y = start.y; y != end.y; y += yDirection * 2) 
			{
				Wall2* wall = AddGo(new Wall2("Wall_V"));
				wall->SetScale({ 1.f, 1.f }); 
				wall->SetPosition({ static_cast<float>(start.x * 16), static_cast<float>(y * 16)  + 16.f});
				wall->SetTexture(wallData.textureIds[i % wallData.textureIds.size()]);
				wall->SetOrigin(Origins::MC);
				walls_2.push_back(wall);
				i++;
			}
		}
		else if (start.y == end.y)
		{
			int xDirection = (start.x < end.x) ? 1 : -1;
			for (int x = start.x; x != end.x; x += xDirection * 2)
			{
				Wall2* wall = AddGo(new Wall2("Wall_H"));
				wall->SetScale({ 1.f, 1.f });
				wall->SetPosition({ static_cast<float>(x * 16) + 16.f, static_cast<float>(start.y * 16) });
				wall->SetTexture(wallData.textureIds[i % wallData.textureIds.size()]);
				wall->SetOrigin(Origins::MC);
				walls_2.push_back(wall);
				i++;
			}
		}
	}
}

void SceneGame::SetDecorations()
{
	int i = 0;
	const auto& decoTable = STAGE_TABLE->GetCurrentStage().GetDecoTable();
	for (const auto& decoPair : decoTable)
	{
		const DataDecoration& decoData = decoPair.second;
		decorations[i]->SetTexture(decoData.textureId);
		decorations[i]->SetRotation(decoData.rotation);
		decorations[i]->SetPosition(decoData.pos);
		i++;
	}
}

void SceneGame::SetEnemies()
{
	int i = 0;
	const auto& enemyTable = STAGE_TABLE->GetCurrentStage().GetEnemyTable();
	for (const auto& enemyPair : enemyTable)
	{
		const DataEnemy& enemyData = enemyPair.second;
		enemies[i]->Reset();
		enemies[i]->SetPosition({(enemyData.pos.x * STAGE_TABLE->GetTileSize().x) + 8.f,  (enemyData.pos.y * STAGE_TABLE->GetTileSize().y) + 8.f});
		enemies[i]->SetRotation(enemyData.rotation);
		enemies[i]->SetWeapon(enemyData.weaponType);
		enemies[i]->SetStatus(enemyData.state);
		enemies[i]->SetWayPoints(enemyData.waypoints);
		i++;
	}
}

void SceneGame::SetEndPoint()
{
	auto& vec = STAGE_TABLE->GetCurrentStage().GetEndPointArea();
	if (vec.empty())
		return;
	if (vec[0].x == vec[1].x)
	{
		endPoint = { vec[0].x * tileSize.x, vec[0].y * tileSize.y, 10.f ,(vec[1].y - vec[0].y + 1) * tileSize.y};
	}
	else
	{
		endPoint = { vec[0].x * tileSize.x, vec[0].y * tileSize.y,(vec[1].x - vec[0].x + 1) * tileSize.x , 10.f };
	}
}

BodyGuard* SceneGame::GetBodyGuard()
{
	if (boss2 == nullptr)
		return nullptr;
	return boss2->GetBodyGuard();
}
MafiaBoss* SceneGame::GetMafiaBoss()
{
	if (boss2 == nullptr)
		return nullptr;
	return boss2->GetMafiaBoss();
}

void SceneGame::LoadWeapons()
{
	const auto& weaponTable = STAGE_TABLE->GetCurrentStage().GetWeaponTable();
	for (const auto& weaponPair : weaponTable)
	{
		const DataWeapon& weaponData = weaponPair.second;

		auto weapon = SpawnWeapon(weaponData.weaponState.weaponType, { (weaponData.pos.x * STAGE_TABLE->GetTileSize().x) + 8.f,  (weaponData.pos.y * STAGE_TABLE->GetTileSize().y) + 8.f });
		weapon->SetRotation(weaponData.rotation);
	}
}

void SceneGame::OnWeaponDrop(Weapon::WeaponStatus weapon, sf::Vector2f pos)
{
	Weapon* newWeapon = weaponPool.Take();
	weapons.push_back(newWeapon);

	newWeapon->SetStatus(weapon);
	newWeapon->SetPosition(pos);
	newWeapon->SetActive(true);
	newWeapon->OnDrop();
	AddGo(newWeapon);
}

void SceneGame::OnWeaponThrow(Weapon::WeaponStatus weapon, sf::Vector2f dir, sf::Vector2f pos)
{
	Weapon* newWeapon = weaponPool.Take();
	weapons.push_back(newWeapon);

	newWeapon->SetStatus(weapon);
	newWeapon->SetPosition(pos);
	newWeapon->SetActive(true);
	newWeapon->OnThrow(dir);
	AddGo(newWeapon);
}

void SceneGame::OnWeaponThrow(Weapon::WeaponStatus weapon, sf::Vector2f dir, sf::Vector2f pos, BodyGuard* body)
{
	Weapon* newWeapon = weaponPool.Take();
	weapons.push_back(newWeapon);

	newWeapon->SetStatus(weapon);
	newWeapon->SetPosition(pos);
	newWeapon->SetActive(true);
	newWeapon->OnThrow(dir,body);
	AddGo(newWeapon);
}


Weapon* SceneGame::SpawnWeapon(Weapon::WeaponType weaponType, sf::Vector2f pos)
{
	Weapon* weapon = weaponPool.Take();
	weapons.push_back(weapon);

	weapon->SetStatus(WEAPON_TABLE->Get(weaponType));
	weapon->SetPosition(pos);
	weapon->SetActive(true);
	return AddGo(weapon);
}

Bullet* SceneGame::SpawnBullet()
{
	Bullet* bullet = bulletPool.Take();

	if(boss2 != nullptr)
		bullet->SetFountains(boss2->GetFountain1(), boss2->GetFountain2());
	activeBullets.push_back(bullet);
	return AddGo(bullet);
}

void SceneGame::ReturnBullet(Bullet* val)
{
	RemoveGo(val);
	bulletPool.Return(val);
	activeBullets.remove(val);
}

void SceneGame::ReturnWeapon(Weapon* val)
{
	RemoveGo(val);
	weaponPool.Return(val);
	weapons.remove(val);
}

void SceneGame::RemoveAllObjPool()
{
	for (auto bullet : activeBullets)
	{
		RemoveGo(bullet);
		bulletPool.Return(bullet);
	}
	activeBullets.clear();


	for (auto weapon : weapons)
	{
		RemoveGo(weapon);
		weaponPool.Return(weapon);
	}
	weapons.clear();
}


void SceneGame::Draw(sf::RenderWindow& window)
{
	VIEW_MGR.DrawBackground();
	Scene::Draw(window);
}

void SceneGame::LoadCurrentStage()
{
	ClearStage();
	tileMap->SetTexture(&TEXTURE_MGR.Get(STAGE_TABLE->GetCurrentStage().GetTileTextureId()));
	tileMap->Initialize(STAGE_TABLE->GetTileSize(), STAGE_TABLE->GetCurrentStage().GetTileCount(), STAGE_TABLE->GetCurrentStage().GetFloorTiles());
	player->SetPosition(STAGE_TABLE->GetCurrentStage().GetPlayerData().pos * tileSize.x);
	player->SetRotation(STAGE_TABLE->GetCurrentStage().GetPlayerData().rotation);
	VIEW_MGR.ResetPlayerDefaultPos(STAGE_TABLE->GetCurrentStage().GetPlayerData().pos * tileSize.x);

	LoadWalls(); // ��¥ �� ������ ����ϱ�
	LoadDecorations();
	LoadEnemies();
	LoadWeapons();


	if (STAGE_TABLE->GetCurrentStage().GetBoss1Data().pos != sf::Vector2f{ -1.f, -1.f })
	{
		boss = AddGo(new Boss1("Boss1"));
		boss->Reset();
		boss->SetPosition(STAGE_TABLE->GetCurrentStage().GetBoss1Data().pos * tileSize.x);
		boss->SetRotation(STAGE_TABLE->GetCurrentStage().GetBoss1Data().rotation);
	}
	if (STAGE_TABLE->GetCurrentStage().GetBoss2Position() != sf::Vector2f{ -1.f, -1.f })
	{
		boss2 = AddGo(new Boss2("Boss2"));
		boss2->Reset();
		boss2->SetPosition(STAGE_TABLE->GetCurrentStage().GetBoss2Position());
	}
	player->Reset();
	SetWalls();
	SetDecorations();
	SetEnemies();
	SetEndPoint();
}

void SceneGame::LoadNextStage()
{
	ClearStage();
	STAGE_TABLE->NextStage();
	tileMap->SetTexture(&TEXTURE_MGR.Get(STAGE_TABLE->GetCurrentStage().GetTileTextureId()));
	tileMap->Initialize(STAGE_TABLE->GetTileSize(), STAGE_TABLE->GetCurrentStage().GetTileCount(), STAGE_TABLE->GetCurrentStage().GetFloorTiles());
	player->SetPosition(STAGE_TABLE->GetCurrentStage().GetPlayerData().pos * tileSize.x);
	player->SetRotation(STAGE_TABLE->GetCurrentStage().GetPlayerData().rotation);
	VIEW_MGR.ResetPlayerDefaultPos(STAGE_TABLE->GetCurrentStage().GetPlayerData().pos * tileSize.x);


	LoadWalls(); // ��¥ �� ������ ����ϱ�
	LoadDecorations();
	LoadEnemies();
	LoadWeapons();

	if(STAGE_TABLE->GetCurrentStage().GetBoss1Data().pos != sf::Vector2f{-1.f, -1.f})
	{
		boss = AddGo(new Boss1("Boss1"));
		boss->Reset();
		boss->SetPosition(STAGE_TABLE->GetCurrentStage().GetBoss1Data().pos * tileSize.x);
		boss->SetRotation(STAGE_TABLE->GetCurrentStage().GetBoss1Data().rotation);
	}
	if (STAGE_TABLE->GetCurrentStage().GetBoss2Position() != sf::Vector2f{ -1.f, -1.f })
	{
		boss2 = AddGo(new Boss2("Boss2"));
		boss2->Reset();
		boss2->SetPosition(STAGE_TABLE->GetCurrentStage().GetBoss2Position());
	}
	player->Reset();
	SetWalls();
	SetDecorations();
	SetEnemies();
	SetEndPoint();
}

void SceneGame::ClearStage()
{
	RemoveAllObjPool();
	for (auto& wall : walls)
	{
		RemoveGo(wall);
	}
	for (auto& deco : decorations)
	{
		RemoveGo(deco);
	}
	for (auto& enemy : enemies)
	{
		RemoveGo(enemy);
	}

	walls.clear();
	decorations.clear();
	enemies.clear();

	if (boss != nullptr)
	{
		RemoveGo(boss);
		boss = nullptr;
	}
	if (boss2 != nullptr)
	{
		RemoveGo(boss2);
		boss2 = nullptr;
	}

	endPoint = { 0,0,0,0 };
}

bool SceneGame::IsClearStage()
{
	bool result = false;
	if (!enemies.empty())
	{
		for (auto enemy : enemies)
		{
			if (!enemy->isDie())
			{
				result = false;
				break;
			}
			result = true;
		}
	}

	if (boss != nullptr)
		result = boss->IsDead();

	if (boss2 != nullptr)
		result = boss2->IsClear();

	uiHud->SetIsCleared(result);

	return result;
}

void SceneGame::SaveStage()
{
	if(STAGE_TABLE->GetSavedStageIndex() <= STAGE_TABLE->GetCurrentStageIndex())
	{
		std::ifstream indexFile("tables/save_data.json");
		int saveIndex = 0;

		json data;
		indexFile >> data;
		indexFile.close();

		saveIndex = data["stage_count"];

		std::ofstream FileOut("tables/save_data.json");

		json outdata;
		STAGE_TABLE->SetSavedStageIndex(STAGE_TABLE->GetCurrentStageIndex());
		outdata["stage_count"] = saveIndex;
		outdata["save_stage"] = STAGE_TABLE->GetSavedStageIndex();
		FileOut << outdata.dump(4);
		FileOut.close();

		STAGE_TABLE->SetCurrentStageIndex(0);
	}
}
