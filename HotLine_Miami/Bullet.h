#pragma once
#include "Weapon.h"

class Player;
class Enemy;
class Wall2;

class Bullet : public GameObject
{
public:
	enum class Owner
	{
		Player,
		Enemies
	};
protected:
	sf::Sprite body;

	float speed = 500.f;
	sf::Vector2f direction;

	Animator animator;

	Owner currentOwner;

	std::vector<Wall2*> walls;

	Weapon::WeaponStatus weaponStatus;
public:
	Bullet(const std::string& name = "");
	~Bullet() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;

	void FixedUpdate(float dt)override;
	void FixedUpdatePlayer(float dt);
	void FixedUpdateEnemies(float dt);

	void Draw(sf::RenderWindow& window) override;

	void Fire(sf::Vector2f direction, Player* owner, Weapon::WeaponStatus weaponStatus);
	void Fire(sf::Vector2f direction, Enemy* owner, Weapon::WeaponStatus weaponStatus);
};