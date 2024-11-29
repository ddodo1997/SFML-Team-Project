#pragma once
#include "GameObject.h"

class Weapon : public GameObject
{
public: 
	enum class WeaponType
	{
		None = -1,
		Bat,
		Knife,
		Machinegun,
		Shotgun,
	};

	struct WeaponStatus
	{
		Weapon::WeaponType	weaponType = Weapon::WeaponType::None;
		std::string			textureId = "";
		int					damage = 0;
		int					damageOnThrow = 0;
		bool				isRangedWeapon = false;
		int					maxBullet = 0;
		float				attackInterval = 0.f;
	};
	
protected:
	WeaponStatus	weaponStatus;
	
	sf::Sprite		weaponSprite;

	sf::Vector2f	direction;

	float			speedOnThrow = 250.f;
	float			onThrowTimer;

	float			speedOnDrop = 200.f;
	float			onDropTimer;
	
	// 개별 전달 필요
	int				remainingBullet = 0;
	bool			isPickupable = false;

public:
	Weapon(std::string name = "");
	~Weapon() = default;
	Weapon(const Weapon& other) = default;
	Weapon& operator=(const Weapon& other) = default;

	void Init() override;
	void Release() override;

	void Reset() override;

	void Update(float dt) override;
	void LateUpdate(float dt)  override {}
	void FixedUpdate(float dt) override {}
	void Draw(sf::RenderWindow& window) override;

	void SetPosition(sf::Vector2f pos);
	void SetRotation(float angle);

	void OnPickUp();
	void OnThrow(sf::Vector2f direction);
	void OnDrop(sf::Vector2f direction);

	void SetWeaponType(WeaponType type);
	WeaponType GetWeaponType();

	bool GetIsRanged();
	bool GetIsPickupable();

	void SetRemainingBullet(int bullet);
	int GetRemainingBullet();

	// Test Code
};

