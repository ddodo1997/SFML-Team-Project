#pragma once
#include "Scene.h"
#include "json.hpp"
#include "DataTableMgr.h"
using json = nlohmann::json;
class TileMap;
class TileMapEditor;
class Player;
class Enemy;
class Decoration;
class Wall;
class Wall2;
class SceneDevS : public Scene
{
protected:
	Player* player;
	TileMap* tileMap;
	TileMapEditor* tileMapEditor;

	std::vector<Enemy*> enemies;
	std::vector<Decoration*> decorations;
	std::vector<Wall*> walls;
	std::vector<Wall2*> wallsVertical;
	std::vector<Wall2*> wallsHorizontal;
	sf::Vector2f direction;

	float zoomNoun = 0.5f;
	sf::Vector2i tileSize;
	sf::Vector2i tileCount;
public:
	SceneDevS();
	~SceneDevS() = default;

	void Init() override;
	void Enter() override;
	void Exit() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void CreateWall(const sf::Vector2f& pos);

	void LoadWalls();
	void LoadDecorations();
	void LoadEnemies();

	void SetStatusEnemies();
	Player* GetPlayer() { return player; }

	std::vector<Enemy*> GetEnemies() const { return enemies; }
	std::vector<Decoration*> GetDecorations() const { return decorations; }
	std::vector<Wall*> GetWalls() const { return walls; }

	void SaveMap() const;
};

