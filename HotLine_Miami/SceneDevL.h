#pragma once
#include "Scene.h"

class Player;
class UiHudL;

class SceneDevL :
    public Scene
{
protected:
	Player* player;
	UiHudL* uiHud;

public:
	SceneDevL();
	~SceneDevL() = default;

	void Init() override;
	void Enter() override;
	void Exit() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

