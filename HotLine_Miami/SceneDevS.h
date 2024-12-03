#pragma once
#include "Scene.h"
#include "json.hpp"
#include "DataTableMgr.h"
#include "StageTable.h"
#include "SceneGame.h"
using json = nlohmann::json;
class TileMap;
class TileMapEditor;

class Player;
class Enemy;
class Decoration;
class Wall;
class Wall2;
class SceneDevS : public SceneGame
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

	std::vector<DataWall> mergedHorizontalWalls;
	std::vector<DataWall> mergedVerticalWalls;

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
	void CreateEnemy(const sf::Vector2f& pos);

	void LoadWalls();
	void LoadDecorations();
	void LoadEnemies();

	void SetStatusEnemies();
	Player* GetPlayer() { return player; }

	std::vector<Enemy*> GetEnemies() const { return enemies; }
	std::vector<Decoration*> GetDecorations() const { return decorations; }
	std::vector<Wall*> GetWalls() const { return walls; }

	void DeleteWalls();
	void SaveMap();
	void SaveWall();
};

