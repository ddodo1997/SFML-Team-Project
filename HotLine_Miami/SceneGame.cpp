#include "stdafx.h"
#include "SceneGame.h"
#include "StageTable.h"
#include "Wall.h"
#include "Decoration.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Player.h"
#include "TileMap.h"
#include "UiHudL.h"

SceneGame::SceneGame() : Scene(SceneIds::SceneGame)
{
}
void SceneGame::Init()
{
	player = AddGo(new Player("Player"));
	tileMap = AddGo(new TileMap("Tile Map"));
	uiHud = AddGo(new UiHudL());
	LoadDecorations();
	LoadWalls();
	LoadEnemies();

	Scene::Init();
}

void SceneGame::Release()
{
}

void SceneGame::Enter()
{
	Scene::Enter();
	SetStatusEnemies();

	tileMap->SetTexture(&TEXTURE_MGR.Get(STAGE_TABLE->GetTileTextureId()));
	tileMap->Initialize(STAGE_TABLE->GetTileSize(), STAGE_TABLE->GetTileCount(), STAGE_TABLE->GetFloorTiles());

	worldView.setSize(windowSize * 0.2f);
	worldView.setCenter(0, 0);

	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::RemovePoolObjects()
{
	for (auto weapon : weapons)
	{
		RemoveGo(weapon);
		weaponPool.Return(weapon);
	}
	weapons.clear();

	for (auto bullet : activeBullets)
	{
		RemoveGo(bullet);
		bulletPool.Return(bullet);
	}
	activeBullets.clear();

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
		player->Reset();
	}
	if (InputMgr::GetKey(sf::Keyboard::Numpad7))
	{
		directionX -= 1.f;
	}
	if (InputMgr::GetKey(sf::Keyboard::Numpad8))
	{
		directionX += 1.f;
	}
	if (InputMgr::GetKey(sf::Keyboard::Numpad4))
	{
		directionY -= 1.f;
	}
	if (InputMgr::GetKey(sf::Keyboard::Numpad5))
	{
		directionY += 1.f;
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

	worldView.setCenter(player->GetPosition());
	FRAMEWORK.GetWindow().setView(worldView);

	ClearInactivePoolObjects();
}


void SceneGame::LoadWalls()
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

void SceneGame::LoadDecorations()
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

void SceneGame::LoadEnemies()
{
	const auto& enemyTable = STAGE_TABLE->GetEnemyTable();
	for (const auto& enemyPair : enemyTable)
	{
		const DataEnemy& enemyData = enemyPair.second;

		Enemy* enemy = new Enemy(enemyData.id);
		// enemy->SetStatus(enemyData.state);
		enemy->SetPosition({ (enemyData.pos.x * STAGE_TABLE->GetTileSize().x) + 8.f,  (enemyData.pos.y * STAGE_TABLE->GetTileSize().y) + 8.f });
		enemy->SetWayPoints(enemyData.waypoints);

		enemies.push_back(enemy);
		AddGo(enemy);
	}
}

void SceneGame::SetStatusEnemies()
{
	int i = 0;
	const auto& enemyTable = STAGE_TABLE->GetEnemyTable();
	for (const auto& enemyPair : enemyTable)
	{
		enemies[i++]->SetStatus(enemyPair.second.state);
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

void SceneGame::PlayerTryPickUpWeapon()
{
	for (auto weapon : weapons)
	{
		if (player->GetHitBox().rect.getGlobalBounds().intersects(weapon->GetHitBox().rect.getGlobalBounds()))
		{
			PlayerPickUpWeapon(weapon->GetStatus());
			weapon->SetActive(false);

			weaponPool.Return(weapon);
			RemoveGo(weapon);
			return;
		}
	}
}

void SceneGame::PlayerPickUpWeapon(Weapon::WeaponStatus weapon)
{
	player->WeaponPickUp(weapon);
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
void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}