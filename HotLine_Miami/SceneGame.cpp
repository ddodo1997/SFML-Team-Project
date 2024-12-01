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
SceneGame::SceneGame() : Scene(SceneIds::SceneGame)
{
}
void SceneGame::Init()
{
	player = AddGo(new Player("Player"));
	tileMap = AddGo(new TileMap("Tile Map"));
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
	SetEnemies();
	SetWalls();
	SetDecorations();

	tileMap->SetTexture(&TEXTURE_MGR.Get(STAGE_TABLE->GetTileTextureId()));
	tileMap->Initialize(STAGE_TABLE->GetTileSize(), STAGE_TABLE->GetTileCount(), STAGE_TABLE->GetFloorTiles());

	worldView.setSize(windowSize * 0.2f);
	worldView.setCenter(player->GetPosition());

	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);
}

void SceneGame::Exit()
{
	RemoveAllObjPool();
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	worldView.setCenter(player->GetPosition());
	if (InputMgr::GetKeyDown(sf::Keyboard::R))
		SCENE_MGR.ChangeScene(SceneIds::SceneGame);
	Scene::Update(dt);

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



}


void SceneGame::LoadWalls()
{
	const auto& wallTable = STAGE_TABLE->GetWallTable();

	for (const auto& WallPair : wallTable)
	{
		const DataWall& wallData = WallPair.second;

		Wall* wall = AddGo(new Wall(wallData.id));
		walls.push_back(wall);
		wall->DrawWall(wallData);
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