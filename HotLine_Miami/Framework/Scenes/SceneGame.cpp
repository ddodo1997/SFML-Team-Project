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
#include "UiHudL.h"
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
	uiHud = AddGo(new UiHudL());

	cleaver = AddGo(new Cleaver("Cleaver"));
	pathFinder = new PathFinder();
	uiHud->SetPlayer(player);

	LoadWalls(); // ��¥ �� ������ ����ϱ�
	LoadDecorations();
	LoadEnemies();

	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();
}

void SceneGame::Enter()
{
	tileSize = static_cast<sf::Vector2f>(STAGE_TABLE->GetTileSize());
	SetWalls(); // ��¥ �� ����� ��
	Scene::Enter();
	// SetWalls_2(); // ���� �� ����� �� 
	SetEnemies();
	SetDecorations();
	SetWeapons();
	player->SetPosition(STAGE_TABLE->GetPlayerData().pos * tileSize.x);
	player->SetRotation(STAGE_TABLE->GetPlayerData().rotation);
	if (sf::Vector2f({ -1.f,-1.f }) != STAGE_TABLE->GetBoss1Data().pos)
	{
		boss = AddGo(new Boss1("Boss1"));
		boss->Reset();
		boss->SetPosition(STAGE_TABLE->GetBoss1Data().pos * tileSize.x);
		boss->SetRotation(STAGE_TABLE->GetBoss1Data().rotation);
	}	
	if (sf::Vector2f({ -1.f,-1.f }) != STAGE_TABLE->GetBoss2Position())
	{
		boss2 = AddGo(new Boss2("Boss2"));
		boss2->Reset();
		boss2->SetPosition({ -1000.f, -1000.f, });
	}
	tileMap->SetTexture(&TEXTURE_MGR.Get(STAGE_TABLE->GetTileTextureId()));
	tileMap->Initialize(STAGE_TABLE->GetTileSize(), STAGE_TABLE->GetTileCount(), STAGE_TABLE->GetFloorTiles());

	worldView.setSize(windowSize * 0.2f);
	//worldView.setCenter(player->GetPosition());

	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);
	VIEW_MGR.Init();
	VIEW_MGR.SetCurrentSceneWorldView(&worldView);
	VIEW_MGR.SetCurrentSceneUiView(&uiView);
	VIEW_MGR.SetCurrentScenePlayer(player);

	SOUND_MGR.PlayBgm("sound/bgm/Paris2.ogg");
	SOUND_MGR.SetBgmVolume(0);
	SOUND_MGR.SetSfxVolume(20);
}

void SceneGame::Exit()
{
	RemoveAllObjPool();
	Scene::Exit();
	uiHud = nullptr;
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

	Scene::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::R))
	{
		SCENE_MGR.ChangeScene(SceneIds::SceneGame);
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

	if(uiHud != nullptr)
		uiHud->UpdateWeaponStatus(player->GetWeaponStatus(), player->GetRemainingBullet());

	//worldView.setCenter(player->GetPosition());
	FRAMEWORK.GetWindow().setView(worldView);

	ClearInactivePoolObjects();
}


void SceneGame::LoadWalls()
{
	const auto& wallTable = STAGE_TABLE->GetWallTable();

	for (const auto& WallPair : wallTable)
	{
		const DataWall& wallData = WallPair.second;

		Wall* wall = AddGo(new Wall(wallData.id));
		walls.push_back(wall);
	}
}

void SceneGame::LoadDecorations()
{
	const auto& decoTable = STAGE_TABLE->GetDecoTable();
	for (const auto& decoPair : decoTable)
	{
		const DataDecoration& decoData = decoPair.second;

		Decoration* deco = AddGo(new Decoration(decoData.id));
		decorations.push_back(deco);
		deco->SetTexture(decoData.textureId);
		deco->SetRotation(decoData.rotation);
		deco->SetPosition(decoData.pos);
	}
}

void SceneGame::LoadEnemies()
{
	const auto& enemyTable = STAGE_TABLE->GetEnemyTable();
	for (const auto& enemyPair : enemyTable)
	{
		const DataEnemy& enemyData = enemyPair.second;

		Enemy* enemy = AddGo(new Enemy(enemyData.id));
		enemies.push_back(enemy);
	}
}

void SceneGame::SetWalls()
{
	int i = 0;
	const auto& wallTable = STAGE_TABLE->GetWallTable();

	for (const auto& WallPair : wallTable)
	{
		const DataWall& wallData = WallPair.second;
		walls[i++]->DrawWall(wallData);
	}
}

void SceneGame::SetWalls_2()
{
	int i = 0;
	const auto& wallTable = STAGE_TABLE->GetWallTable();

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
	const auto& decoTable = STAGE_TABLE->GetDecoTable();
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
	const auto& enemyTable = STAGE_TABLE->GetEnemyTable();
	for (const auto& enemyPair : enemyTable)
	{
		const DataEnemy& enemyData = enemyPair.second;
		enemies[i]->SetPosition({(enemyData.pos.x * STAGE_TABLE->GetTileSize().x) + 8.f,  (enemyData.pos.y * STAGE_TABLE->GetTileSize().y) + 8.f});
		enemies[i]->SetRotation(enemyData.rotation);
		enemies[i]->SetWeapon(enemyData.weaponType);
		enemies[i]->SetStatus(enemyData.state);
		enemies[i]->SetWayPoints(enemyData.waypoints);
		i++;
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

void SceneGame::SetWeapons()
{
	const auto& weaponTable = STAGE_TABLE->GetWeaponTable();
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

	if(boss != nullptr)
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