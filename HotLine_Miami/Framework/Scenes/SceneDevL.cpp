#include "stdafx.h"
#include "SceneDevL.h"
#include <codecvt>
#include "Player.h"
#include "TextGo.h"
#include "UiHudL.h"
#include "TileMap.h"
#include "Decoration.h"
#include "Wall.h"

SceneDevL::SceneDevL() : Scene(SceneIds::DevL)
{

}

void SceneDevL::Init()
{
	FRAMEWORK.GetWindow().setFramerateLimit(120);
	//tileMap->SetTexture(&TEXTURE_MGR.Get(STAGE_TABLE->GetTileTextureId()));
	//tileMap->InitializeEmpty(STAGE_TABLE->GetTileSize(), { 40, 40 });
	player = AddGo(new Player());
	uiHud = AddGo(new UiHudL());
	enemies.push_back(AddGo(new Enemy()));

	Scene::Init();
}

void SceneDevL::Enter()
{	
	worldView.setSize({ windowSize.x * 0.2f,windowSize.y * 0.2f });
	worldView.setCenter(0.f, 0.f);
	VIEW_MGR.SetWorldViewScale({ 0.2f,0.2f });

	uiView.setSize(windowSize);
	uiView.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);
	VIEW_MGR.SetUiViewSize(uiView.getSize());

	Scene::Enter();
}

void SceneDevL::Exit()
{
	RemovePoolObjects();

	Scene::Exit();
}

void SceneDevL::RemovePoolObjects()
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

void SceneDevL::ClearInactivePoolObjects()
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

void SceneDevL::Update(float dt)
{
	Scene::Update(dt);

	Weapon::WeaponType tempIndex = Weapon::WeaponType::Bat;
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

	uiHud->UpdateWeaponStatus(player->GetWeaponStatus(), player->GetRemainingBullet());

	worldView.setCenter(player->GetPosition());
	FRAMEWORK.GetWindow().setView(worldView);

	ClearInactivePoolObjects();
}

void SceneDevL::Draw(sf::RenderWindow& window)
{

	Scene::Draw(window);
}

void SceneDevL::OnWeaponDrop(Weapon::WeaponStatus weapon, sf::Vector2f pos)
{
	Weapon* newWeapon = weaponPool.Take();
	weapons.push_back(newWeapon);
	
	newWeapon->SetStatus(weapon);
	newWeapon->SetPosition(pos);
	newWeapon->SetActive(true);
	newWeapon->OnDrop();
	AddGo(newWeapon);
}

void SceneDevL::OnWeaponThrow(Weapon::WeaponStatus weapon, sf::Vector2f dir, sf::Vector2f pos)
{
	Weapon* newWeapon = weaponPool.Take();
	weapons.push_back(newWeapon);

	newWeapon->SetStatus(weapon);
	newWeapon->SetPosition(pos);
	newWeapon->SetActive(true);
	newWeapon->OnThrow(dir);
	AddGo(newWeapon);
}

void SceneDevL::PlayerTryPickUpWeapon()
{
	for (auto weapon : weapons)
	{
		if (player->GetHitBox().rect.getGlobalBounds().intersects(weapon->GetHitBox().rect.getGlobalBounds()))
		{
			PlayerPickUpWeapon(weapon->GetStatus());
			weapon->SetActive(false);
			return;
		}
	}
}

void SceneDevL::PlayerPickUpWeapon(Weapon::WeaponStatus weapon)
{
	player->WeaponPickUp(weapon);
}

void SceneDevL::SpawnWeapon(Weapon::WeaponType weaponType, sf::Vector2f pos)
{
	Weapon* weapon = weaponPool.Take();
	weapons.push_back(weapon);

	weapon->SetStatus(WEAPON_TABLE->Get(weaponType));
	weapon->SetPosition(pos);
	weapon->SetActive(true);
	AddGo(weapon);
}

Bullet* SceneDevL::SpawnBullet()
{
	Bullet* bullet = bulletPool.Take();
	activeBullets.push_back(bullet);
	return AddGo(bullet);
}

void SceneDevL::ReturnBullet(Bullet* val)
{
	RemoveGo(val);
	bulletPool.Return(val);
	activeBullets.remove(val);
}
