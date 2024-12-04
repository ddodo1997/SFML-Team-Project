#pragma once
#include "Weapon.h"

class SceneGame;
class Weapon;
class Enemy;
class Wall;
class Decoration;

class Player : public GameObject
{
public:
	struct ClipInfo
	{
		std::string idle;
		std::string move;
	};

	enum class Mask
	{
		None,
		Chicken,
		Tiger,
		Rabbit,
		Wolf,
		Giraffe,
	};

protected:
	Weapon::WeaponStatus weaponStatus;
	Mask currentMask = Mask::Chicken;

	float attackTimer;

	SceneGame* sceneGame;
	std::vector<Wall*> walls;
	std::vector<Decoration*> decorations;

	Enemy* executingEnemy;

	std::vector<Enemy*> enemies;

	sf::Vector2f prevPos;

	sf::Sprite body;
	sf::Sprite leg;
	sf::Sprite mask;

	sf::RenderStates maskRenderState;

	Animator animatorBody;
	Animator animatorLeg;

	sf::Vector2f direction;
	sf::Vector2f look;

	float speed = 100;
	float onDieSpeed = 300;
	float onDieEffectAccumTime = 0.6f;

	bool isMoving = false;
	bool isAlive = true;

	sf::FloatRect attackHitBox;
	sf::RectangleShape attackHitBoxCheck;
	sf::CircleShape noiseCircle;

	bool isFlipped = false;
	bool isAttacking = false;

	bool isOnPound = false;
	bool isExecuting = false;
	bool isExecutionOnWall = false;
	float executionTimer = 0.f;
	int executionCount = 0;

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

	void SetScene(SceneGame* sceneDevL);

	bool IsDead() { return !isAlive; }

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void UpdateBodyAnimationMoving();
	void UpdateMask(float dt);
	void UpdateExecution(float dt);

	void UpdateExecutionDefualt(float dt);
	void UpdateExecutionBat(float dt);
	void UpdateExecutionKnife(float dt);
	void UpdateExecutionWall(float dt);

	void FixedUpdate(float dt) override;
	void UpdateOnDie(float dt);

	void Draw(sf::RenderWindow& window) override;

	void OnHit(Weapon::WeaponStatus weapon, sf::Vector2f hitDir);

	void OnHitByBat(sf::Vector2f hitDir);
	void OnHitByKnife(sf::Vector2f hitDir);
	void OnHitByMachinegun(sf::Vector2f hitDir);
	void OnHitByShotgun(sf::Vector2f hitDir);

	void SetDirection(sf::Vector2f dir) { direction = dir; }
	const sf::Vector2f& GetDirection() { return direction; }
	const sf::Vector2f& GetLook() { return look; }
	sf::Vector2f GetPrevPos() { return prevPos; }

	sf::FloatRect GetGlobalBounds() { return leg.getGlobalBounds(); }
	sf::FloatRect GetLocalBounds() { return leg.getLocalBounds(); }

	void TryPickUpWeapon();
	void WeaponPickUp(Weapon::WeaponStatus weapon);
	void SetWeaponStatus();
	void SetRemainingBullet(int remainingBullet) { weaponStatus.remainingBullet = remainingBullet; }
	int GetRemainingBullet();

	Weapon::WeaponStatus GetWeaponStatus() { return weaponStatus; }
	
	void ThrowWeapon(sf::Vector2f lookDir);
	void DropWeapon(sf::Vector2f hitDir = {0.f,0.f});

	void TryExecute();
	void Execute();

	void ExecuteAgainstWall();

	void ExecuteDefault();
	void ExecuteBat();
	void ExecuteKnife();
	void ExecuteMachinegun();
	void ExecuteShotgun();

	void Attack();

	void AttackDefault();
	void AttackBat();
	void AttackKnife();
	void AttackMachinegun();
	void AttackShotgun();
};
