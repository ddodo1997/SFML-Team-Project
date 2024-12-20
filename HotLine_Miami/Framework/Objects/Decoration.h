#pragma once
class SceneGame;
class Player;
class Enemy;
class Boss1;
class Decoration : public GameObject
{
protected:
	sf::Sprite body;
	std::string textureId;
	sf::Vector2f pos;

	SceneGame* sceneGame;
	Player* player;
	std::vector<Enemy*> enemies;
	Boss1* boss1;
public:
	Decoration(const std::string& name = "");
	~Decoration() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void SetTexture(const std::string& textureId);

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt)override;
	void Draw(sf::RenderWindow& window) override;

	sf::FloatRect GetLocalBounds() const { return body.getLocalBounds(); }
	sf::FloatRect GetGlobalBounds() const { return body.getGlobalBounds(); }
};
