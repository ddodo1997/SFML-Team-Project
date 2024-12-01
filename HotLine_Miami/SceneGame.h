#pragma once
#include "Scene.h"

class TileMap;
class TileMapEditor;
class Player;
class Enemy;
class Decoration;
class Wall;
class Bullet;
class UiHudL;

class SceneGame :
    public Scene
{
protected:
	Player* player;
	TileMap* tileMap;
	UiHudL* uiHud;

	sf::Vector2f directionXY;
	float directionX = 0.f;
	float directionY = 0.f;

	std::vector<Enemy*> enemies;
	std::vector<Decoration*> decorations;
	std::vector<Wall*> walls;

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
	void RemovePoolObjects();
	void ClearInactivePoolObjects();

	virtual void Update(float dt);

	void LoadWalls();
	void LoadDecorations();
	void LoadEnemies();

	void SetStatusEnemies();
	Player* GetPlayer() { return player; }

	std::vector<Enemy*> GetEnemies() const { return enemies; }
	std::vector<Decoration*> GetDecorations() const { return decorations; }
	std::vector<Wall*> GetWalls() const { return walls; }

	void OnWeaponDrop(Weapon::WeaponStatus weapon, sf::Vector2f pos);
	void OnWeaponThrow(Weapon::WeaponStatus weapon, sf::Vector2f dir, sf::Vector2f pos);

	void PlayerTryPickUpWeapon();
	void PlayerPickUpWeapon(Weapon::WeaponStatus weapon);

	// Test Code
	void SpawnWeapon(Weapon::WeaponType weaponType, sf::Vector2f pos);


	Bullet* SpawnBullet();
	void ReturnBullet(Bullet* val);

	virtual void Draw(sf::RenderWindow& window);
};

