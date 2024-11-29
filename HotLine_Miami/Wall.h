#pragma once
#include "StageTable.h"

class Wall : public GameObject
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
	sf::Transform transform;
public:
	Wall(const std::string& name = "");
	~Wall() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

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
