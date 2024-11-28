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
		directionX -= 0.001;
	}
	if (InputMgr::GetKey(sf::Keyboard::Numpad8))
	{
		directionX += 0.001;
	}
	if (InputMgr::GetKey(sf::Keyboard::Numpad4))
	{
		directionY -= 0.001;
	}
	if (InputMgr::GetKey(sf::Keyboard::Numpad5))
	{
		directionY += 0.001;
	}

	if (std::fabs(directionX) > 1)
	{
		directionX = 0;
	}
	if (std::fabs(directionY) > 1)
	{
		directionY = 0;
	}

	directionXY = { directionX, directionY };

	uiHud->UpdateHitDir(directionXY);

	ClearInactivePoolObjects();
}

void SceneDevL::Draw(sf::RenderWindow& window)
{


	Scene::Draw(window);
}

void SceneDevL::OnWeaponDrop(Weapon::WeaponType weaponType, int remainingBullet, sf::Vector2f dir)
{
	Weapon* weapon = weaponPool.Take();
	weapons.push_back(weapon);

	weapon->SetWeaponType(weaponType);
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
