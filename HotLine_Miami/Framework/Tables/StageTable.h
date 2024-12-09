#pragma once
#include "DataTable.h"
#include "EnemyTable.h"
#include "DecorationTable.h"
struct DataWall
{
    std::string id = "";
    sf::Vector2f start = { 0.f, 0.f };
    sf::Vector2f end = { 0.f, 0.f };
    // Wall::Types type = Wall::Types::Horizontal;
    std::vector<std::string> textureIds;

};
struct DataEnemy
{
    std::string id = "";
    sf::Vector2f pos = { 0.f, 0.f };
    float rotation = 0.f;
    Weapon::WeaponType weaponType = Weapon::WeaponType::None;
    Enemy::Status state = Enemy::Status::Normal;
    std::vector<sf::Vector2f> waypoints;
};
struct DataWeapon
{
    std::string id = "";
    sf::Vector2f pos = { 0.f, 0.f };
    float rotation = 0.f;
    Weapon::WeaponStatus weaponState = Weapon::WeaponStatus();
};
struct DataPlayer
{
    sf::Vector2f pos = { 0.f, 0.f };
    float rotation = 0.f;
};
struct DataBoss1
{
    sf::Vector2f pos = { 0.f, 0.f };
    float rotation = 0.f;
};
struct DataStage
{
    sf::Vector2i tileSize = { 16, 16 };
    sf::Vector2i tileCount = { 0, 0 };

    std::string tileTextureId = "";
    std::vector<int> floorTiles;

    std::unordered_map<std::string, DataDecoration> decoTable;
    std::unordered_map<std::string, DataEnemy> enemyTable;
    std::unordered_map<std::string, DataWall> wallTable;
    std::unordered_map<std::string, DataWeapon> weaponTable;

    DataPlayer playerData;
    DataBoss1 boss1Data;
    std::vector<sf::Vector2f> endPoints;
    sf::Vector2f boss2Position;

    const sf::Vector2i& GetTileSize() const { return tileSize; }
    const sf::Vector2i& GetTileCount() const { return tileCount; }
    const std::string& GetTileTextureId() const { return tileTextureId; }
    const std::vector<int>& GetFloorTiles() const { return floorTiles; }
    const std::unordered_map<std::string, DataDecoration>& GetDecoTable() const { return decoTable; }
    const std::unordered_map<std::string, DataEnemy>& GetEnemyTable() const { return enemyTable; }
    const std::unordered_map<std::string, DataWall>& GetWallTable() const { return wallTable; }
    const std::unordered_map<std::string, DataWeapon>& GetWeaponTable() const { return weaponTable; }
    const DataPlayer& GetPlayerData() const { return playerData; }
    const DataBoss1& GetBoss1Data() const { return boss1Data; }
    const sf::Vector2f& GetBoss2Position() const { return boss2Position; }
    const std::vector<sf::Vector2f>& GetEndPointArea() const { return endPoints; }
};
class StageTable : public DataTable
{
private:
    sf::Vector2i tileSize;
    std::vector<DataStage> stageDatas;
    int currentStageIndex;
    int savedStageIndex;
public:
    StageTable() : DataTable(DataTable::Types::Stages) { };
    ~StageTable() = default;

    bool Load() override;
    void Release() override;

    void ParseDecorations(const json& decorations, DataStage& stage);
    void ParseWalls(const json& walls, DataStage& stage);
    void ParseEnemies(const json& enemies, DataStage& stage);
    void ParseWeapons(const json& weapons, DataStage& stage);

    const sf::Vector2i GetTileSize() const { return tileSize; }
    const DataStage& GetCurrentStage() const;
    void SetSavedStageIndex(int idx);
    void SetCurrentStageIndex(int idx);
    const int GetSavedStageIndex() const { return savedStageIndex; }
    const int GetCurrentStageIndex() const { return currentStageIndex; }
    const int GetStageCount() const { return stageDatas.size(); }
    
    void LoadStage();
    void NextStage();
};

