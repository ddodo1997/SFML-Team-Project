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
#include "Cleaver.h"
SceneGame::SceneGame() : Scene(SceneIds::SceneGame)
{

}

void SceneGame::Init()
{
	player = AddGo(new Player("Player"));
	tileMap = AddGo(new TileMap("Tile Map"));
	uiHud = AddGo(new UiHudL());
	boss = AddGo(new Boss1("Boss1"));
	cleaver = AddGo(new Cleaver("Cleaver"));

	LoadWalls(); // 통짜 벽 쓸때만 사용하기
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
	SetWalls(); // 통짜 벽 사용할 때
	Scene::Enter();

	// SetWalls_2(); // 개별 벽 사용할 때 
	SetEnemies();
	SetDecorations();

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
	SOUND_MGR.SetBgmVolume(50);
	SOUND_MGR.SetSfxVolume(50);
}

void SceneGame::Exit()
{
	RemoveAllObjPool();
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
	Scene::Update(dt);
	VIEW_MGR.Update(dt);

	Weapon::WeaponType tempIndex = Weapon::WeaponType::Bat;
	if (InputMgr::GetKeyDown(sf::Keyboard::Numpad0))
	{
		player->OnHit(WEAPON_TABLE->Get(tempIndex), directionXY);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Numpad1))
	{
		tempIndex = Weapon::WeaponType::Knife;
		player->OnHit(WEAPON_TABLE->Get(tempIndex), directionXY);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Numpad2))
	{
		tempIndex = Weapon::WeaponType::Machinegun;
		player->OnHit(WEAPON_TABLE->Get(tempIndex), directionXY);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Numpad3))
	{
		tempIndex = Weapon::WeaponType::Shotgun;
		player->OnHit(WEAPON_TABLE->Get(tempIndex), directionXY);
	}
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

	if (std::fabs(directionX) > 1000.f)
	{
		directionX = 0;
	}
	if (std::fabs(directionY) > 1000.f)
	{
		directionY = 0;
	}

	directionXY = { directionX, directionY };

	uiHud->UpdateHitDir(directionXY);
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


void SceneGame::SpawnWeapon(Weapon::WeaponType weaponType, sf::Vector2f pos)
{
	Weapon* weapon = weaponPool.Take();
	weapons.push_back(weapon);

	weapon->SetStatus(WEAPON_TABLE->Get(weaponType));
	weapon->SetPosition(pos);
	weapon->SetActive(true);
	AddGo(weapon);
}

Bullet* SceneGame::SpawnBullet()
{
	Bullet* bullet = bulletPool.Take();
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
	Scene::Draw(window);
}