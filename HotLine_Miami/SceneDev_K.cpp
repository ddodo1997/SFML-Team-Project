#include "stdafx.h"
#include "SceneDev_K.h"
#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"
SceneDev_K::SceneDev_K() :Scene(SceneIds::Dev_K)
{
}

void SceneDev_K::Init()
{
	enemy = AddGo(new Enemy("enemy"));
	player = AddGo(new Player("player"));

	Scene::Init();
}

void SceneDev_K::Release()
{
	Scene::Release();
}

void SceneDev_K::Enter()
{
	Scene::Enter();
	worldView.setSize({ windowSize.x * 0.2f,windowSize.y * 0.2f  });
	worldView.setCenter(0.f, 0.f);

	uiView.setSize(windowSize);
	uiView.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);
}

void SceneDev_K::Exit()
{
	RemoveAllObjPool();
	Scene::Exit();
}

void SceneDev_K::Update(float dt)
{
	Scene::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::Num4))
	{
		Variables::isDrawHitBox = !Variables::isDrawHitBox;
	}
}

void SceneDev_K::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

Bullet* SceneDev_K::SpawnBullet()
{
	Bullet* bullet = bulletPool.Take();
	activeBullets.push_back(bullet);
	return AddGo(bullet);
}

void SceneDev_K::ReturnBullet(Bullet* val)
{
	RemoveGo(val);
	bulletPool.Return(val);
	activeBullets.remove(val);
}

void SceneDev_K::RemoveAllObjPool()
{
	for (auto bullet : activeBullets)
	{
		RemoveGo(bullet);
		bulletPool.Return(bullet);
	}
	activeBullets.clear();
}
