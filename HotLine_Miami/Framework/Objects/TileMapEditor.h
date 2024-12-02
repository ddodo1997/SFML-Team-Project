#pragma once
#include "Enemy.h"
class TileMapEditor : public GameObject
{
public:
	enum class EditorMode
	{
		TileMode,
		WallMode,
		DecorationMode,
		EnemyMode,
	};
protected:
	sf::Texture* texture = nullptr;
	sf::VertexArray tileSelector;
	sf::Sprite selectedTileSprite;
	std::string tileMapTexId = "graphics/Map/Tiles/tlFloorTiles.png";
	int selectedTileIndex = -1;

	std::unordered_map<std::string, sf::Sprite> wallSprites;
	sf::Sprite selectedWallSprite;
	std::string selectedWallTextureId = "";

	std::vector<Enemy*> enemiesUI;
	int selectedEnemyIndex = -1;
	Enemy selectedEnemy;

	EditorMode currentMode;
	sf::RectangleShape background;
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
	void UpdateTileMode(float dt);
	void UpdateWallMode(float dt);
	void UpdateDecoMode(float dt);
	void UpdateEnemyMode(float dt);
	void UpdateSelectedSpritePosition();
	void Draw(sf::RenderWindow& window) override;

	int GetSelectTileIndex(const sf::Vector2f& position);
	bool IsInTileSelectArea(const sf::Vector2f& position) const;
	void SetSelectTileIndex(int index);
	int GetSelectedTileIndex() const { return selectedTileIndex; }

	void SetSelectWall(std::pair<std::string, sf::Sprite> wall);
	std::string GetSelectedWallTextureId() const { return selectedWallTextureId; }

	Enemy GetSelectedEnemy() const { return selectedEnemy; }

	void SetMode(EditorMode mode);
	EditorMode GetMode() const { return currentMode; }
};
