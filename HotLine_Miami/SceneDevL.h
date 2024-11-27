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

	float directionDegree;

	sf::Vector2f directionXY;
	float directionX = 0.f;
	float directionY = 0.f;	
	
public:
	SceneDevL();
	~SceneDevL() = default;

	void Init() override;
	void Enter() override;
	void Exit() override;

	/*void BloodEffect(sf::);*/

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

