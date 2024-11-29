#pragma once
#include "Scene.h"
#include "Weapon.h"

class Player;
class UiHudL;
class Weapon;

class SceneDevL :public Scene
{
protected:
	Player* player;
	UiHudL* uiHud;

	float directionDegree;

	sf::Vector2f directionXY;
	float directionX = 0.f;
	float directionY = 0.f;

	std::list<Weapon*> weapons;
	ObjectPool<Weapon> weaponPool;

public:
	SceneDevL();
	~SceneDevL() = default;


	void Init() override;
	void Enter() override;
	void Exit() override;
	void RemovePoolObjects();
	void ClearInactivePoolObjects();

	/*void BloodEffect(sf::);*/

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void OnWeaponDrop(Weapon::WeaponType weaponType, int remainingBullet, sf::Vector2f dir, sf::Vector2f pos);
	void OnWeaponThrow(Weapon::WeaponType weaponType, int remainingBullet, sf::Vector2f dir, sf::Vector2f pos);
	
	
	void PlayerTryPickUpWeapon();
	void PlayerPickUpWeapon(Weapon::WeaponType weaponType, int remainingBullet);
	// Test Code
	void SpawnWeapon(Weapon::WeaponType weaponType, int remainingBullet, sf::Vector2f pos);
};

