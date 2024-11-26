#pragma once
#include "Scene.h"
#include "json.hpp"
using json = nlohmann::json;
class TileMap;
class TileMapEditor;
class SceneDevS : public Scene
{
protected:
	TileMap* tileMap;
	TileMapEditor* tileMapEditor;
public:
	SceneDevS();
	~SceneDevS() = default;

	void Init() override;
	void Enter() override;
	void Exit() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

