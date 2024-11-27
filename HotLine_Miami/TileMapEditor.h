#pragma once

class TileMapEditor : public GameObject
{
protected:
	sf::Texture* texture = nullptr;
	sf::VertexArray tileSelector;
	sf::Sprite selectedTileSprite;
	std::string tileMapTexId = "graphics/Map/Tiles/tlFloorTiles.png";

	int selectedTileIndex = -1;
public:
	TileMapEditor(const std::string& name = "");
	~TileMapEditor() = default;

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

	int GetSelectTileIndex(const sf::Vector2f& position);
	bool IsInTileSelectArea(const sf::Vector2f& position) const;
	void SetSelectTileIndex(int index);

};
