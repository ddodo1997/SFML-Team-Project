#pragma once

class WayPoint : public GameObject
{
protected:
	sf::CircleShape waypointShape;
	float radius;
public:
	WayPoint(const std::string& name = "");
	~WayPoint() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;
	void SetColor(const sf::Color& color);
	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};
