#pragma once

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

	sf::Sprite body;
	sf::Sprite leg;

	Animator animatorBody;
	Animator animatorLeg;

	sf::Vector2f direction;
	float speed = 400;

	bool isMoving = false;

	std::map<std::string, AnimationClip> aniClipMap;
	
public:
	Player(const std::string& name = "Player");
	~Player() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};
