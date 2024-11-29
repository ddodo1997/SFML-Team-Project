#pragma once
#include "Scene.h"
#include "json.hpp"
#include "DataTableMgr.h"
using json = nlohmann::json;
class TileMap;
class TileMapEditor;
class Player;
class Enemy;
class SceneDevS : public Scene
{
protected:
	Player* player;
	TileMap* tileMap;
	TileMapEditor* tileMapEditor;

	std::vector<Enemy*> enemies;
public:
	SceneDevS();
	~SceneDevS() = default;

	void Init() override;
	void Enter() override;
	void Exit() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void LoadWalls();
	void LoadDecorations();
	void LoadEnemies();

	void SetStatusEnemies();
	Player* GetPlayer() { return player; }

};

