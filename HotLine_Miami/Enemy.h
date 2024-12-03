#pragma once
#include "Weapon.h"

class Player;
class SceneGame;
class Wall2;
class Wall;
class Decoration;
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
		struct WayPoint
		{
			sf::Vector2f position;
			sf::CircleShape point;

			WayPoint(sf::Vector2f position) {
				this->position = position;
				point.setPosition(this->position);
				Utils::SetOrigin(point, Origins::MC);
			}
		};
		std::vector<WayPoint> wayPoints;
		sf::CircleShape originPoint;
		int wayPointCnt;
		int currentWayPoint = 0;
	};
	enum class Status
	{
		EditorMode = -1,
		Normal,
		Idle,
		Patrol,
		Aggro,
		SearchWeapon,
		Stun,
		GetUp,
		Die,
		Pounded
	};


protected:
	sf::Sprite body;
	sf::Sprite legs;
	sf::RectangleShape meleeHitBox;
	sf::ConvexShape viewAngle;
	sf::CircleShape weaponSearchRange;

	Player* player;
	std::vector<Wall*> walls;
	std::vector<Decoration*> decorations;

	SceneGame* sceneGame;

	Animator animatorBody;
	Animator animatorLegs;

	sf::Vector2f direction = { 0.f,0.f };
	sf::Vector2f prevPos;
	float speed = 30.f;

	Status currentStatus;

	Normal normal;
	Idle idle;
	Patrol patrol;

	Weapon::WeaponStatus weaponStatus;
	int hp = 1;

	float stunTimer = 0.f;
	float stunDelay = 2.f;

	float attackTimer = 0.f;

	bool isWalking = false;
	bool isAttacking = false;
public:
	Enemy(const std::string& name = "");
	~Enemy() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	sf::FloatRect GetLocalBounds()const { return legs.getLocalBounds(); }
	sf::FloatRect GetGlobalBounds()const { return legs.getGlobalBounds(); }

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
	void UpdatePounded(float dt);

	void FixedUpdate(float dt) override;

	void SetStatus(Status stat);
	void SetWeapon(Weapon::WeaponType type);
	

	void SetWayPoints(std::vector<sf::Vector2f> pos);
	void clearWayPoints();
	void Draw(sf::RenderWindow& window) override;

	Status GetStatus() const { return currentStatus; }
	sf::Vector2f GetDirection() const { return direction; }
	Weapon::WeaponType GetWeaponType() const { return weaponStatus.weaponType; }

	void PickupWeapon(Weapon* weapon);
	void OnHit(Weapon::WeaponStatus weaponStatus, sf::Vector2f direction, bool isThrow = false);
	void DropWeapon();
	void OnDie(sf::Vector2f direction);

	bool isDie() const{ return currentStatus == Status::Die ? true : false; }
	bool isStun() const { return currentStatus == Status::Stun ? true : false; }

	void Attack();
};
