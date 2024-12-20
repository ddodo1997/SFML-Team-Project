#pragma once
#include "Weapon.h"
class SceneGame;
class Player;
class Cleaver;
class Boss1 : public GameObject
{
public:
	enum class Patterns
	{
		None = -1,
		Pattern1,
		Pattern2,
		PickUpCleaver,
		Stun,
		Crawl,
		Pounded,
		Die,
	};

	struct Pattern
	{
		int patternCnt = 0;

		float attackTimer = 0.f;
		float attackDelay = 0.5f;

		float patternTimer = 0.f;
		float patternDelay = 4.f;

		float prepareAttackTimer = 0.f;
		float prepareAttackDelay = 0.3f;

		float attackingTimer = 0.f;
		float attackingDelay = 0.6f;

		float pryTimer = 0.f;
		float pryDelay = 2.2f;

		float stunTimer = 0.f;
		float stunDelay = 1.5f;

		float crawlTimer = 0.f;
		float crawlDelay = 0.9f;

		float crawlingTimer = 0.f;
		float crawlingDelay = 1.f;
		bool isCrawling = false;

		sf::Vector2f targetDirection;
		float speed = 200.f;
		float crawlingSpeed = 20.f;

		bool isAttacking = false;
		bool isOnAttack = false;
	};

protected:
	sf::Sprite body;
	sf::Sprite legs;
	sf::RectangleShape meleeHitBox;

	Animator animatorBody;
	Animator animatorLegs;

	Weapon::WeaponStatus weaponStatus;

	sf::Vector2f direction;
	float speed = 120.f;

	Pattern pattern;

	Patterns currentPattern;

	SceneGame* sceneGame;
	Player* player;
	Cleaver* cleaver;
	bool isPhase2 = false;
	bool isWalking;
	bool isAlive = true;
public:
	Boss1(const std::string& name = "");
	~Boss1() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;
	sf::FloatRect GetGlobalBounds() const override { return legs.getGlobalBounds(); }
	const sf::Vector2f& GetDirection() const { return direction; }

	void Init() override;
	void Release() override;
	void Reset() override;
	void SetPatterns();
	void SetWeaponStatus();
	Weapon::WeaponStatus GetWeaponStatus() { return weaponStatus; }

	void Update(float dt) override;
	void Pattern1(float dt);
	void Pattern2(float dt);
	void OnPry(float dt);
	void OnStun(float dt);
	void OnCrawl(float dt);

	void ChangePattern(Patterns pattern);

	void Draw(sf::RenderWindow& window) override;

	void Attack();
	void OnHit(sf::Vector2f dir);
	void OnDie();
	bool IsCanAttack() const{ return currentPattern == Patterns::None; }
	bool IsExcutable() const { return currentPattern == Patterns::Crawl; }
	bool IsDead() const { return !isAlive; }
};
