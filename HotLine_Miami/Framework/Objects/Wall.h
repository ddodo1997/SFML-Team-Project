#pragma once
#include "StageTable.h"
class SceneGame;
class Player;
class Enemy;
class Weapon;

class Boss1;
class Cleaver;

class Wall : public GameObject, public sf::Transformable
{
public:
	enum class Types
	{
		Horizontal,
		Vertical,
	};
protected:
	sf::VertexArray va;
	std::vector<const sf::Texture*> textures;
	SceneGame* sceneGame;
	Player* player;
	Boss1* boss1;
	Cleaver* cleaver;
	std::vector<Enemy*> enemies;
	std::list<Weapon*> weapons;
	
	float length;
	DataWall data;
	HitBox hitBox;


public:
	Wall(const std::string& name = "");
	~Wall() = default;

	void SetOrigin(Origins preset) override;

	sf::FloatRect GetLocalBounds() const override;
	sf::FloatRect GetGlobalBounds() const override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt)override;
	void Draw(sf::RenderWindow& window) override;

	void SetType(Types type);
	void SetTexture(const std::string& id);
	void DrawWall(const DataWall& dataWall);

	void UpdateTransform();
};
