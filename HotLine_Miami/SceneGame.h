#pragma once
#include "Scene.h"
class TileMap;
class TileMapEditor;
class Player;
class Enemy;
class Decoration;
class Wall2;
class Bullet;
class SceneGame :
    public Scene
{
protected:
	Player* player;
	TileMap* tileMap;

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
	std::vector<Wall2*> GetWalls() const { return walls; }

	void OnWeaponDrop(Weapon::WeaponStatus weapon, sf::Vector2f pos);
	void OnWeaponThrow(Weapon::WeaponStatus weapon, sf::Vector2f dir, sf::Vector2f pos);

	void PlayerTryPickUpWeapon();
	void PlayerPickUpWeapon(Weapon::WeaponStatus weapon);

	// Test Code
	void SpawnWeapon(Weapon::WeaponType weaponType, sf::Vector2f pos);


	Bullet* SpawnBullet();
	void ReturnBullet(Bullet* val);

	void RemoveAllObjPool();

	virtual void Draw(sf::RenderWindow& window);
};
