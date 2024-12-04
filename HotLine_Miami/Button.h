#pragma once

class Button : public GameObject
{
protected:
	sf::RectangleShape background;
	sf::Text text;
	bool isClicked;
public:
	Button(const std::string& name = "");
	~Button() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	sf::FloatRect GetGlobalBounds() const override { return background.getGlobalBounds(); }

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	bool IsClicked() const { return isClicked; }

	void SetString(const std::string& text)
	{ 
		this->text.setString(text);
		Utils::SetOrigin(this->text, Origins::BC);
	}
	void OnClick(bool clicked);
};
