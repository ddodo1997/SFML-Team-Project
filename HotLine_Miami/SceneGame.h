#pragma once
#include "Scene.h"

class TileMap;
class TileMapEditor;
class Player;
class Enemy;
class Decoration;
class Wall2;
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
	std::vector<Wall2*> walls;

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

	void SetWalls();
	void SetDecorations();
	void SetEnemies();
	Player* GetPlayer() { return player; }

	std::vector<Enemy*> GetEnemies() const { return enemies; }
	std::vector<Decoration*> GetDecorations() const { return decorations; }
<<<<<<< HEAD
	std::vector<Wall2*> GetWalls() const { return walls; }

	const std::list<Weapon*>& GetActiveWeapons() const { return weapons; }
=======
	std::vector<Wall*> GetWalls() const { return walls; }
	std::list<Weapon*> GetWeapons() const { return weapons; }
>>>>>>> 79b2384d25d2bf1a95275259d55393d57ec1df79

	void OnWeaponDrop(Weapon::WeaponStatus weapon, sf::Vector2f pos);
	void OnWeaponThrow(Weapon::WeaponStatus weapon, sf::Vector2f dir, sf::Vector2f pos);


	// Test Code
	void SpawnWeapon(Weapon::WeaponType weaponType, sf::Vector2f pos);

	Bullet* SpawnBullet();
	void ReturnBullet(Bullet* val);

	Weapon* SpawnWeapon();
	void ReturnWeapon(Weapon* val);

	void RemoveAllObjPool();

	virtual void Draw(sf::RenderWindow& window);
};

