#pragma once
#include "Scene.h"
#include "json.hpp"
#include "DataTableMgr.h"
#include "StageTable.h"
#include "SceneGame.h"
using json = nlohmann::json;
class TileMap;
class TileMapEditor;
class WayPoint;
class Player;
class Enemy;
class Decoration;
class Wall;
class Wall2;
class SceneDevS : public SceneGame
{
protected:
	int minX, minY;

	Player* player = nullptr;
	Boss1* boss1 = nullptr;
	Boss2* boss2 = nullptr;
	TileMap* tileMap;
	TileMapEditor* tileMapEditor;
	Enemy* patrolEnemy;
	std::vector<Enemy*> enemies;
	std::vector<Decoration*> decorations;

	std::vector<Wall*> walls;

	std::vector<Wall2*> wallsVertical;
	std::vector<Wall2*> wallsHorizontal;

	std::vector<DataWall> mergedHorizontalWalls;
	std::vector<DataWall> mergedVerticalWalls;

	std::vector<std::pair<Enemy::Status, Enemy*>> createdEnemies;
	std::vector<WayPoint*> waypoints;

	std::vector<Weapon*> createdWeapons;

	std::vector<WayPoint*> endPoints;

	sf::Vector2f direction;
	float zoomNoun = 0.5f;
	bool isWayPointMode;
	sf::Color waypointColor;
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
	void StartWayPointMode(Enemy* enemy);
	void AddWayPoints(const sf::Vector2f& pos);

	void CreateWeapon(const sf::Vector2f& pos);
	void CreatePlayer(const sf::Vector2f& pos);
	void CreateBoss1(const sf::Vector2f& pos);
	void CreateBoss2(const sf::Vector2f& pos);
	void CreateEndPoint(const sf::Vector2f& pos);

	void LoadWalls();
	void LoadDecorations();
	void LoadEnemies();

	void SetStatusEnemies();
	Player* GetPlayer() { return player; }

	std::vector<Enemy*> GetEnemies() const { return enemies; }
	std::vector<Decoration*> GetDecorations() const { return decorations; }
	std::vector<Wall*> GetWalls() const { return walls; }

	void DeleteWall(const sf::Vector2f& pos);
	void DeleteEnemy(const sf::Vector2f& pos);
	void DeleteWaypoint(const sf::Vector2f& pos);
	void DeleteWeapon(const sf::Vector2f& pos);
	void DeleteEndPoint(const sf::Vector2f& pos);

	void DeleteWalls();
	void DeleteEnemies();
	void DeleteWaypoints();
	void DeleteWeapons();
	void DeletePlayer();
	void DeleteBoss1();
	void DeleteBoss2();
	void DeleteEndPoints();

	void SaveMap();
	void SaveWall(json& mapData);
	void SaveEnemies(json& mapData);
	void SaveWeapons(json& mapData);
	void SavePlayer(json& mapData);
	void SaveBoss(json& mapData);
	void SaveBoss2(json& mapData);
	void SaveEndPoint(json& mapData);

	std::string EnemyStatusToString(const Enemy::Status& state);
	std::string EnemyWeaponToString(const Weapon::WeaponType& type);
};

