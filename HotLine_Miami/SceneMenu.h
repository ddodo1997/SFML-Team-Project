#pragma once
#include "Scene.h"

class UiMenu;

class SceneMenu :
	public Scene
{
protected:
	UiMenu* uiMenu;

public:
	SceneMenu();
	~SceneMenu() = default;
	virtual void Init();
	virtual void Release();
	virtual void Enter();
	virtual void Exit();
	virtual void Update(float dt);

	virtual void Draw(sf::RenderWindow& window);
};

