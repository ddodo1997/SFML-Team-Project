#pragma once
#include "Scene.h"
class Enemy;
class Player;
class Bullet;
class SceneDev_K :
    public Scene
{
protected:
	Enemy* enemy;
	Player* player;
	ObjectPool<Bullet> bulletPool;
	std::list<Bullet*> activeBullets;
public:
	SceneDev_K();
	virtual ~SceneDev_K() = default;

	virtual void Init();
	virtual void Release();

	virtual void Enter();
	virtual void Exit();

	virtual void Update(float dt);

	virtual void Draw(sf::RenderWindow& window);

	Player* GetPlayer() { return player; }
	Enemy* GetEnemy() { return enemy; }

	Bullet* SpawnBullet();
	void ReturnBullet(Bullet* val);

	void RemoveAllObjPool();
};

