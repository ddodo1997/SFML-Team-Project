#pragma once

class Player;
class MafiaBoss : public GameObject
{
public:
	enum class Status
	{
		None = -1,
		Awaken,
		DoubleShot,
		DoubleReload,
		Hit,
		SoloShot,
		SoloReload,
		Suicide,
		Die
	};


	struct Pattern
	{
		bool isShanks = false;

		int doubleMaxAmmo = 40;
		int soloMaxAmmo = 20;
		int currentAmmo;

		float getGunsTimer = 0.f;
		float getGunsDelay = 2.f;

		float shotTimer = 0.f;

		float doubleReloadingTimer = 0.f;
		float doubleReloadingDelay = 4.f;

		float soloReloadingTimer = 0.f;
		float soloReloadingDelay = 2.f;

		float firstHitTimer = 0.f;
		float firstHitDelay = 1.5f;

		sf::Vector2f targetDirection;
		Status currentStatus = Status::None;
	};

protected:
	sf::Sprite body;
	sf::Sprite legs;

	Animator animatorBody;

	Weapon::WeaponStatus weaponStatus;

	Pattern pattern;
	Player* player;
public:
	MafiaBoss(const std::string& name = "MafiaBoss");
	~MafiaBoss() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;
	void SetPlayer(Player* player);

	void Init() override;
	void Release() override;
	void Reset() override;
	void ResetPatterns();
	void Update(float dt) override;

	void Phase3(float dt);
	void Awakning(float dt);
	void DoubleShot(float dt);
	void DoubleReload(float dt);
	void LostArms(float dt);
	void SoloShot(float dt);
	void SoloReload(float dt);
	void Suiciding(float dt);


	void Draw(sf::RenderWindow& window) override;

	void SetStatus(Status stat);

	void Awaken();
	void OnHit();

	bool IsDead() const { return pattern.currentStatus == Status::Die; }
	bool IsHitAble() const;
};
