#pragma once

class Player;
class BodyGuard : public GameObject
{
public:
	struct Pattern
	{
		float speed = 50.f;
		float attackSpeed = 200.f;
		sf::Vector2f direction;

	};
protected:
	sf::Sprite body;

	Animator animator;

	Pattern pattern;

	Player* player;
public:
	BodyGuard(const std::string& name = "");
	~BodyGuard() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void SetPlayer(Player* player);
	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};
