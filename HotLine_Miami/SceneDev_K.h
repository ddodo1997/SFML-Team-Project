#pragma once
#include "Scene.h"
class Enemy;
class SceneDev_K :
    public Scene
{
protected:
	Enemy* enemy;
public:
	SceneDev_K();
	virtual ~SceneDev_K() = default;

	virtual void Init();
	virtual void Release();

	virtual void Enter();
	virtual void Exit();

	virtual void Update(float dt);

	virtual void Draw(sf::RenderWindow& window);
};

