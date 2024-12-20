#pragma once
#include "Enemy.h"

class Button;
class Player;
class Boss1;
class Boss2;
class TileMapEditor : public GameObject
{
public:
	enum class EditorMode
	{
		TileMode,
		WallMode,
		DecorationMode,
		EnemyMode,
		WeaponMode,
		PlayerAndBossMode
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
	Enemy::Status selectedEnemyStatus;
	Button* normalButton;
	Button* idleButton;
	Button* patrolButton;

	Player* player;
	Player* selectedPlayer = nullptr;
	Boss1* boss_1;
	Boss1* selectedBoss_1 = nullptr;
	MafiaBoss* boss_2;
	Boss2* selectedBoss_2 = nullptr;
	std::string selectedPlayerOrBoss;

	std::vector<Weapon*> weaponsUI;
	int seletedWeaponIndex = -1;
	Weapon selectedWeapon;

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

	sf::FloatRect GetGlobalBounds() const { return background.getGlobalBounds(); }

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void UpdateTileMode(float dt);
	void UpdateWallMode(float dt);
	void UpdateDecoMode(float dt);
	void UpdateEnemyMode(float dt);
	void UpdateWeaponMode(float dt);
	void UpdatePlayerAndBossMode(float dt);
	void UpdateSelectedSpritePosition();
	void UpdateEnemyModeButtons(sf::Vector2f worldPos);
	void Draw(sf::RenderWindow& window) override;

	int GetSelectTileIndex(const sf::Vector2f& position);
	bool IsInTileSelectArea(const sf::Vector2f& position) const;
	void SetSelectTileIndex(int index);
	int GetSelectedTileIndex() const { return selectedTileIndex; }

	void SetSelectWall(std::pair<std::string, sf::Sprite> wall);
	std::string GetSelectedWallTextureId() const { return selectedWallTextureId; }

	Enemy GetSelectedEnemy() const { return selectedEnemy; }
	Enemy::Status GetSelectedEnemyStatus() const { return selectedEnemyStatus; }

	Weapon GetSelectedWeapon() const { return selectedWeapon; }

	std::string GetSelectedPlayerOrBoss() const { return selectedPlayerOrBoss; }
	Player* GetPlayer() const { return selectedPlayer; }
	Boss1* GetBoss1() const { return selectedBoss_1; }

	void SetMode(EditorMode mode);
	EditorMode GetMode() const { return currentMode; }
};
