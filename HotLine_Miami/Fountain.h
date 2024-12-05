#pragma once
class Player;
class Panther;
class BodyGuard;
class Fountain : public GameObject
{
protected:
	sf::Sprite body;

	Animator animator;

	Player* player;
	Panther* panther1;
	Panther* panther2;

	BodyGuard* bodyguard;
public:
	Fountain(const std::string& name = "");
	~Fountain() = default;

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
