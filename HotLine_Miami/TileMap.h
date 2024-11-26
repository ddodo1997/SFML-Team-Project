#pragma once

class TileMap : public GameObject
{
protected:
	sf::VertexArray va;
	std::string tileMapTexId = "graphics/Map/Tiles/tlFloorTiles.png";
	sf::Texture* texture = nullptr;

	sf::Vector2i cellCount;
	sf::Vector2f cellSize = { 16.f, 16.f };
	sf::Transform transform;
public:
	TileMap(const std::string& name = "");
	~TileMap() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	sf::FloatRect GetLocalBounds() const override;
	sf::FloatRect GetGlobalBounds() const override;
	sf::Vector2f GetCellSize() const;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void Set(const sf::Vector2i& count);
	void LoadMapJson(const std::string& filename);
	void UpdateTransform();
};
