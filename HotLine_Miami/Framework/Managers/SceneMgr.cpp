#include "stdafx.h"
#include "SceneMgr.h"

void SceneMgr::Init()
{
	for (auto scene : scenes)
	{
		scene->Init();
	}

	currentScene = startScene;
	scenes[(int)currentScene]->Enter();
}

void SceneMgr::Release()
{
	for (auto scene : scenes)
	{
		if (currentScene == scene->id)
		{
			scene->Exit();
		}
		scene->Release();
		delete scene;
	}
	scenes.clear();
}

void SceneMgr::ChangeScene(SceneIds id)
{
	nextScene = id;
}

void SceneMgr::Update(float dt)
{
	scenes[(int)currentScene]->Update(dt);
}

void SceneMgr::LateUpdate(float dt)
{
	scenes[(int)currentScene]->LateUpdate(dt);
}

void SceneMgr::FixedUpdate(float dt)
{
	scenes[(int)currentScene]->FixedUpdate(dt);
}

void SceneMgr::OnPreDraw()
{
	scenes[(int)currentScene]->OnPreDraw();
}

void SceneMgr::Draw(sf::RenderWindow& window)
{
	OnPreDraw();
	scenes[(int)currentScene]->Draw(window);
	OnPostDraw();
}

void SceneMgr::OnPostDraw()
{
	scenes[(int)currentScene]->OnPostDraw();
	if (nextScene != SceneIds::None)
	{
		scenes[(int)currentScene]->Exit();
		currentScene = nextScene;
		nextScene = SceneIds::None;
		scenes[(int)currentScene]->Enter();
	}
}
