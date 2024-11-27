#pragma once

class SceneDevL;

class Player : public GameObject
{
public:
	struct ClipInfo
	{
		std::string idle;
		std::string move;
	};

protected:
	// Weapon* weapon;
	SceneDevL* sceneDevL;

	sf::Sprite body;
	sf::Sprite leg;

	Animator animatorBody;
	Animator animatorLeg;

	sf::Vector2f direction;
	float speed = 100;
	float onDieSpeed = 300;
	float onDieEffectAccumTime = 0.6f;

	bool isMoving = false;
	bool isAlive = true;

	std::map<std::string, AnimationClip> aniClipMap;

	std::string dieSpriteDirectory = "graphics/player/Die/";
	
public:
	Player(const std::string& name = "Player");
	~Player() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void SetScene(SceneDevL* sceneDevL);

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void UpdateOnDie(float dt);

	void Draw(sf::RenderWindow& window) override;

	void OnHit(int weaponType, sf::Vector2f hitDir);

	void OnHitByBat(sf::Vector2f hitDir);
	void OnHitByKnife(sf::Vector2f hitDir);
	void OnHitByMachinegun(sf::Vector2f hitDir);
	void OnHitByShotgun(sf::Vector2f hitDir);

	// for test only
	void OnHit(int weaponType, float hitDirRad);
	void OnHitByBat(float hitDirRad);
	void OnHitByKnife(float hitDirRad);
	void OnHitByMachinegun(float hitDirRad);
	void OnHitByShotgun(float hitDirRad);
};
