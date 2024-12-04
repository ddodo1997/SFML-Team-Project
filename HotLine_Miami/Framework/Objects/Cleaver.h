#pragma once
class Player;
class SceneGame;
class Boss1;
class Cleaver : public GameObject
{
protected:
	sf::Sprite body;
	SceneGame* sceneGame;
	Player* player;
	Boss1* boss;

	sf::Vector2f direction;
	float speed;

	bool isStuck = false;
public:
	Cleaver(const std::string& name = "");
	~Cleaver() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	virtual sf::FloatRect GetLocalBounds()const { return body.getLocalBounds(); }
	virtual sf::FloatRect GetGlobalBounds()const { return body.getGlobalBounds(); }

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void Throw(sf::Vector2f direction, sf::Vector2f pos);
	void OnPickUp();
	void OnStuckWall();
	bool isStuckWall() const { return isStuck; }
};

