#pragma once

class Enemy : public GameObject
{
public:
	struct Normal
	{
		float moveTimer = 0.f;
		float moveDelay = 3.f;

		bool isMoving = false;
		float movingTimer = 0.f;
		float movingDelay;
		int movingCnt;
	};
	struct Idle
	{
		float lookAwayTimer = 0.f;
		float lookAwayDelay;
	};
	struct Patrol
	{

	};
	enum class Status
	{
		Normal,
		Idle,
		Patrol,
		Aggro,
		SearchWeapon,
		Stun,
		GetUp,
		Die
	};
protected:
	sf::Sprite body;

	Animator animator;

	sf::Vector2f direction = { 0.f,0.f };

	float speed = 30.f;

	Status currentStatus;

	Normal normal;
	Idle idle;
	Patrol patrol;

	int hp = 1;

	float stunTimer = 0.f;
	float stunDelay = 2.f;
public:
	Enemy(const std::string& name = "");
	~Enemy() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void SetPatterns();

	void Update(float dt) override;
	void UpdateNormal(float dt);
	void UpdateIdle(float dt); 
	void UpdatePatrol(float dt);
	void UpdateAggro(float dt);
	void UpdateSearchWeapon(float dt);
	void UpdateStun(float dt);
	void UpdateGetUp(float dt);
	void UpdateDie(float dt);

	void SetStatus(Status stat);

	void Draw(sf::RenderWindow& window) override;

	void OnHit(int damage, sf::Vector2f direction);
	void OnDie(sf::Vector2f direction);
};
