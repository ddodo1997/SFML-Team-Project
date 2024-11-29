#pragma once
#include "DataTable.h"
#include "EnemyTable.h"
#include "DecorationTable.h"
struct DataWall
{
    std::string id = "";
    sf::Vector2f start = { 0.f, 0.f };
    sf::Vector2f end = { 0.f, 0.f };
    std::vector<std::string> textureIds;
};
struct DataEnemy
{
    std::string id = "";
    sf::Vector2f pos = { 0.f, 0.f };
    std::string weaponType = "";
    Enemy::Status state = Enemy::Status::Normal;
    std::vector<sf::Vector2f> waypoints;
};
class StageTable : public DataTable
{
private:
    sf::Vector2i tileSize;
    sf::Vector2i tileCount;

    std::string tileTextureId;
    std::vector<int> floorTiles;

    std::unordered_map<std::string, DataDecoration> decoTable;
    std::unordered_map<std::string, DataEnemy> enemyTable;
    std::unordered_map<std::string, DataWall> wallTable;
public:
    StageTable() : DataTable(DataTable::Types::Stages) { };
    ~StageTable() = default;

    bool Load() override;
    void Release() override;

    const sf::Vector2i& GetTileSize() const { return tileSize; }
    const sf::Vector2i& GetTileCount() const { return tileCount; }
    const std::string& GetTileTextureId() const { return tileTextureId; }
    const std::vector<int>& GetFloorTiles() const { return floorTiles; }
    const std::unordered_map<std::string, DataDecoration>& GetDecoTable() const { return decoTable; }
    const std::unordered_map<std::string, DataEnemy>& GetEnemyTable() const { return enemyTable; }
    const std::unordered_map<std::string, DataWall>& GetWallTable() const { return wallTable; }
};

