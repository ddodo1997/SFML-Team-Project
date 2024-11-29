#pragma once

class TileMap : public GameObject
{
protected:
	sf::VertexArray va;
	sf::VertexArray gridLines;
	std::string tileMapTexId;
	sf::Texture* texture = nullptr;
	std::vector<int> floorTiles;

	sf::Vector2i tileSize;
	sf::Vector2i tileCount;
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
	sf::Vector2i GetCellSize() const;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void Initialize(const sf::Vector2i& tileSize, const sf::Vector2i& tileCount, const std::vector<int>& floorTiles);
	void InitializeEmpty(const sf::Vector2i& tileSize, const sf::Vector2i& tileCount);
	void UpdateTransform();
	void SetTexture(sf::Texture* texture) { this->texture = texture; }

	void PaintTile(const sf::Vector2f& mousePos, const int index);
	std::vector<int> GetFloorTiles() const { return floorTiles; }
};
