#pragma once
class Player;
class Weapon;
class Panther : public GameObject
{
public:
	enum class Status
	{
		None = -1,
		Pause,
		Aggro,
		Attack,
		HappyMeal,
		Stun,
		Die,
	};
	struct Pattern
	{
		float speed = 50.f;
		float attackSpeed = 200.f;

		float aggroTimer = 0.f;
		float aggroDelay = 0.f;

		float attackTimer = 0.f;
		float attackDelay = 5.f;

		float stunTimer = 0.f;
		float stunDelay = 2.f;

		Status currentStatus = Status::None;
	};
protected:
	sf::Sprite body;
	Animator animator;
	sf::RectangleShape meleeHitBox;

	sf::Vector2f direction;
	Weapon::WeaponStatus weaponStatus;

	Pattern pattern;

	Player* player;
public:
	Panther(const std::string& name = "");
	~Panther() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void SetPlayer(Player* player);

	void SetTimer() { pattern.aggroDelay = 3.f; }

	sf::Vector2f GetDirection() const { return direction; }

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Phase1(float dt);
	void OnPause(float dt);
	void OnAggro(float dt);
	void OnAttack(float dt);
	void OnStun(float dt);
	void OnDie(float dt);

	void SetStatus(Status stat);

	void Draw(sf::RenderWindow& window) override;

	bool IsHitAble() const { return pattern.currentStatus == Status::Attack; }
	bool IsDead() const { return pattern.currentStatus == Status::Die; }
	void Awaken();
};
