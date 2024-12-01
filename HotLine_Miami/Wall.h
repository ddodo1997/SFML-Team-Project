#pragma once
#include "StageTable.h"

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
	
	float length;
	DataWall data;
	HitBox hitBox;
public:
	Wall(const std::string& name = "");
	~Wall() = default;

	sf::FloatRect GetLocalBounds() const override;
	sf::FloatRect GetGlobalBounds() const override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetType(Types type);
	void SetTexture(const std::string& id);
	void DrawWall(const DataWall& dataWall);

	void UpdateTransform();
};
