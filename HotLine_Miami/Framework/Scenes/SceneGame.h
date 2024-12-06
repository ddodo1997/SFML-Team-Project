#pragma once
#include "Scene.h"

class TileMap;
class TileMapEditor;
class Player;
class Enemy;
class Decoration;
class Wall;
class Wall2;
class Bullet;
class UiHudL;
class Boss1;
class Boss2;
class Cleaver;
class PathFinder;

class SceneGame :
    public Scene
{
protected:
	Player* player;
	TileMap* tileMap;
	UiHudL* uiHud;
	Boss1* boss;
	Boss2* boss2;
	Cleaver* cleaver;
	PathFinder* pathFinder;

	sf::Vector2f directionXY;
	float directionX = 0.f;
	float directionY = 0.f;

	std::vector<Enemy*> enemies;
	std::vector<Decoration*> decorations;
	std::vector<Wall*> walls;
	std::vector<Wall2*> walls_2;

	std::list<Weapon*> weapons;
	ObjectPool<Weapon> weaponPool;

	ObjectPool<Bullet> bulletPool;
	std::list<Bullet*> activeBullets;
public:
	SceneGame();
	~SceneGame() = default;
	virtual void Init();
	virtual void Release();

	virtual void Enter();
	virtual void Exit();
	void ClearInactivePoolObjects();

	virtual void Update(float dt);

	void LoadWalls();
	void LoadDecorations();
	void LoadEnemies();
	void LoadWeapons();

	void SetWalls();
	void SetWalls_2();
	void SetDecorations();
	void SetEnemies();
	void SetWeapons();
	Player* GetPlayer() { return player; }
	Boss1* GetBoss1() { return boss; }
	Cleaver* GetCleaver() { return cleaver; }

	std::vector<Enemy*> GetEnemies() const { return enemies; }
	std::vector<Decoration*> GetDecorations() const { return decorations; }
	std::vector<Wall*> GetWalls() const { return walls; }
	std::vector<Wall2*> GetWalls_2() const { return walls_2; }

	const std::list<Weapon*>& GetActiveWeapons() const { return weapons; }

	void OnWeaponDrop(Weapon::WeaponStatus weapon, sf::Vector2f pos);
	void OnWeaponThrow(Weapon::WeaponStatus weapon, sf::Vector2f dir, sf::Vector2f pos);


	// Test Code
	Weapon* SpawnWeapon(Weapon::WeaponType weaponType, sf::Vector2f pos);

	Bullet* SpawnBullet();
	void ReturnBullet(Bullet* val);

	Weapon* SpawnWeapon();
	void ReturnWeapon(Weapon* val);

	void RemoveAllObjPool();

	virtual void Draw(sf::RenderWindow& window);
};

