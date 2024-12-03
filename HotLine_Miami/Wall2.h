#pragma once

class Wall2 : public GameObject
{
protected:
	sf::Sprite body;
	std::string id;
	std::string textureId;
	sf::Vector2f startPosition;
	sf::Vector2f endPosition;
public:
	Wall2(const std::string& name = "");
	~Wall2() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetStartPostion(const sf::Vector2f& pos);
	sf::Vector2f GetStartPosition() const { return startPosition; };
	void SetEndPosition(const sf::Vector2f& pos);
	sf::Vector2f GetEndPosition() const { return endPosition; };
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void SetId(const std::string& id);
	std::string GetId() const { return id; }
	void SetTexture(const std::string& textureId);

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	sf::FloatRect GetLocalBounds() const { return body.getLocalBounds(); }
	sf::FloatRect GetGlobalBounds() const { return body.getGlobalBounds(); }
};
