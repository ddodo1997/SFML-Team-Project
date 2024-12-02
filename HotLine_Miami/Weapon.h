#pragma once
#include "GameObject.h"

class Enemy;
class SceneGame;

class Weapon : public GameObject
{
public: 
	static int		indexCounter;

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
		int					remainingBullet = 0;
		float				attackInterval = 0.f;
		float				hitBoxWidth = 0.f;
		float				hitBoxHeight = 0.f;
		float				noiseRadius = 0.f;
	};
	
protected:
	WeaponStatus	weaponStatus;	
	sf::Sprite		weaponSprite;
	sf::Vector2f	direction;

	
	float			speedOnThrow = 200.f;
	float			onThrowTimer;

	float			speedOnDrop = 200.f;
	float			onDropTimer;
	
	// 개별 전달 필요
	bool			isPickupable = false;

	SceneGame*		scenePointer;

	long long				indexNo;

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
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetPosition(sf::Vector2f pos);
	void SetRotation(float angle);
	void SetOrigin(Origins origin) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;
	void SetScale(const sf::Vector2f& scale) override;

	const WeaponStatus& GetStatus() { return weaponStatus; }
	void SetStatus(const WeaponStatus& status);

	void OnPickUp();
	void OnThrow(sf::Vector2f direction);
	void OnDrop(sf::Vector2f direction = {0.f,0.f});
	void OnHitWall();

	// 구조체 Get, Set 함수
	void SetWeaponType(WeaponType type);
	WeaponType GetWeaponType();


	bool GetIsRanged();
	bool GetIsPickupable();
	// Test Code

	sf::FloatRect GetLocalBounds()const { return weaponSprite.getLocalBounds(); }
	sf::FloatRect GetGlobalBounds()const { return weaponSprite.getGlobalBounds(); }

};

