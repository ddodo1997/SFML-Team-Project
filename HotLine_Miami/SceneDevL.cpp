#include "stdafx.h"
#include "SceneDevL.h"
#include <codecvt>
#include "Player.h"
#include "TextGo.h"
#include "UiHudL.h"

SceneDevL::SceneDevL() : Scene(SceneIds::DevL)
{

}

void SceneDevL::Init()
{
	player = AddGo(new Player());
	uiHud = AddGo(new UiHudL());	

	Scene::Init();
}

void SceneDevL::Enter()
{

	Scene::Enter();
}

void SceneDevL::Exit()
{

	Scene::Exit();
}

void SceneDevL::Update(float dt)
{

	Scene::Update(dt);
}

void SceneDevL::Draw(sf::RenderWindow& window)
{

	Scene::Draw(window);
}
