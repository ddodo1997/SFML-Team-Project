#include "stdafx.h"
#include "SceneDevL.h"
#include <codecvt>
#include "Player.h"
#include "TextGo.h"
#include "UiHudL.h"
#include "Weapon.h"

SceneDevL::SceneDevL() : Scene(SceneIds::DevL)
{

}

void SceneDevL::Init()
{
	player = AddGo(new Player());
	uiHud = AddGo(new UiHudL());

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
}

void SceneDevL::Update(float dt)
{
	Scene::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::Numpad0))
	{
		player->OnHit(0, directionXY);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Numpad1))
	{
		player->OnHit(1, directionXY);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Numpad2))
	{
		player->OnHit(2, directionXY);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Numpad3))
	{
		player->OnHit(3, directionXY);
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
	float randPosX = Utils::RandomRange(100.f, 300.f);
	float randPosY = Utils::RandomRange(50.f, 150.f);
	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		SpawnWeapon(Weapon::WeaponType::Bat, 0, sf::Vector2f(randPosX, randPosY));
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		SpawnWeapon(Weapon::WeaponType::Knife, 0, sf::Vector2f(randPosX, randPosY));
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num3))
	{
		SpawnWeapon(Weapon::WeaponType::Machinegun, 24, sf::Vector2f(randPosX, randPosY));
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num4))
	{
		SpawnWeapon(Weapon::WeaponType::Shotgun, 2, sf::Vector2f(randPosX, randPosY));
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

void SceneDevL::Draw(sf::RenderWindow& window)
{


	Scene::Draw(window);
}

void SceneDevL::OnWeaponDrop(Weapon::WeaponType weaponType, int remainingBullet, sf::Vector2f dir, sf::Vector2f pos)
{
	Weapon* weapon = weaponPool.Take();
	weapons.push_back(weapon);
	
	weapon->SetWeaponType(weaponType);
	weapon->SetPosition(pos);
	weapon->SetActive(true);
	if (weapon->GetIsRanged())
	{
		weapon->SetRemainingBullet(remainingBullet);
	}
	else
	{
		weapon->SetRemainingBullet(0);
	}
	weapon->OnDrop(dir);
	AddGo(weapon);
}

void SceneDevL::PlayerTryPickUpWeapon()
{
	for (auto weapon : weapons)
	{
		if (player->GetHitBox().rect.getGlobalBounds().intersects(weapon->GetHitBox().rect.getGlobalBounds()))
		{
			PlayerPickUpWeapon(weapon->GetWeaponType(), weapon->GetRemainingBullet());
			weapon->SetActive(false);
			return;
		}
	}
}

void SceneDevL::PlayerPickUpWeapon(Weapon::WeaponType weaponType, int remainingBullet)
{
	player->WeaponPickUp(weaponType, remainingBullet);
}

void SceneDevL::SpawnWeapon(Weapon::WeaponType weaponType, int remainingBullet, sf::Vector2f pos)
{
	Weapon* weapon = weaponPool.Take();
	weapons.push_back(weapon);

	weapon->SetWeaponType(weaponType);
	weapon->SetPosition(pos);
	weapon->SetActive(true);
	if (weapon->GetIsRanged())
	{
		weapon->SetRemainingBullet(remainingBullet);
	}
	else
	{
		weapon->SetRemainingBullet(0);
	}
	AddGo(weapon);
}
