#pragma once

class PalmTreeEffect : public GameObject
{
protected:
	sf::Sprite treeSprite;
	sf::Vector2f speed = { 0.f,0.f };
	sf::Vector2f acceleration = {0.f , 0.f};

	std::string textureId;

	int spriteType;
	bool isLeft;

	float timer;

	sf::Vector2f bscb = { 100, 255 };

public:
	PalmTreeEffect(const std::string& name = "");
	~PalmTreeEffect() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void OnCreation(bool isLeft = true);

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};
