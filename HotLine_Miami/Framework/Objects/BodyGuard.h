#pragma once

class Player;
class Weapon;
class MafiaBoss;
class Fountain;
class BodyGuard : public GameObject
{
public:
	enum class Status
	{
		None = -1,
		Phase,
		Throw,
		Attack,
		Stun,
		Pounded1,
		Crawl,
		Pounded2,
		Die
	};
	struct Pattern
	{
		float attackSpeed = 300.f;

		float poundingTimer = 0.f;
		float poundingDelay = 4.f;

		float crawlTimer = 0.f;
		float crawlDelay = 0.9f;

		float crawlingTimer = 0.f;
		float crawlingDelay = 1.f;

		bool isCrawling = false;
		float crawlingSpeed = 20.f;

		Status currentStatus = Status::None;
	};
protected:
	sf::Sprite body;
	sf::Sprite legs;
	Animator animatorBody;
	Animator animatorLegs;
	bool isWalking = false;

	sf::RectangleShape meleeHitBox;

	sf::Vector2f direction;
	Weapon::WeaponStatus weaponStatus;

	Pattern pattern;

	Player* player;
	MafiaBoss* mafiaBoss;
	Fountain* fountain1;
	Fountain* fountain2;
public:
	BodyGuard(const std::string& name = "BodyGuard");
	~BodyGuard() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	sf::Vector2f& GetDirection() { return direction; }
	
	void SetPlayer(Player* player);
	void SetMafiaBoss(MafiaBoss* boss);
	
	void Init() override;
	void Release() override;
	void Reset() override;
	void ResetPatterns();
	
	void Update(float dt) override;
	void Phase2(float dt);
	void OnAttack(float dt);
	void OnStun(float dt);
	void OnCrawl(float dt);
	void OnDie(float dt);

	void SetStatus(Status stat);

	void Draw(sf::RenderWindow& window) override;

	Status GetCurrentStauts() const { return pattern.currentStatus; }

	void Awaken();
	void Attack();
	void Throw();
	void OnHit();
	void OnDie();
	bool IsDead() const { return pattern.currentStatus == Status::Die; }

};
