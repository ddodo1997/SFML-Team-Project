#pragma once
#include "Scene.h"
#include "PalmTreeEffect.h"

class UiMenu;
class PalmTreeEffect;

class SceneMenu :
	public Scene
{
protected:
	UiMenu* uiMenu;

	std::list<PalmTreeEffect*> palmtrees;
	ObjectPool<PalmTreeEffect> palmtreePool;

	float palmtreeTimer = 0.f;

public:
	SceneMenu();
	~SceneMenu() = default;
	virtual void Init();
	virtual void Release();
	virtual void Enter();
	virtual void Exit();
	virtual void Update(float dt);

	void CreatePalmTree(bool isLeft);

	virtual void Draw(sf::RenderWindow& window);
};

