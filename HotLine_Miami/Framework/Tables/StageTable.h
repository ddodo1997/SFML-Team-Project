#pragma once
#include "DataTable.h"
struct DataDecoration
{
    std::string id;
    std::string type;
    sf::Vector2f pos;
    sf::Vector2f size;
    int rotation;
};
class StageTable : public DataTable
{
private:
    sf::Vector2i tileSize;
    sf::Vector2i tileCount;

    std::string tileTextureId;
    std::string decoTextureId;

    std::vector<int> floorTiles;
    std::vector<std::string> wallIds;
    std::vector<std::string> enemyIds;
    std::unordered_map<std::string, DataDecoration> decoTable;
public:
    StageTable() : DataTable(DataTable::Types::Stages) { };
    ~StageTable() = default;

    bool Load() override;
    void Release() override;

    const sf::Vector2i& GetTileSize() const { return tileSize; }
    const sf::Vector2i& GetTileCount() const { return tileCount; }
    const std::string& GetTileTextureId() const { return tileTextureId; }
    const std::vector<int>& GetFloorTiles() const { return floorTiles; }
    const std::vector<std::string>& GetWallIds() const { return wallIds; }
    const std::vector<std::string>& GetEnemyIds() const { return enemyIds; }
    const std::unordered_map<std::string, DataDecoration>& GetDecoTable() const { return decoTable; }
};

