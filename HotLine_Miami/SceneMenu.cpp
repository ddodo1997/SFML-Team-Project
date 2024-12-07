#include "stdafx.h"
#include "SceneMenu.h"
#include "UiMenu.h"

SceneMenu::SceneMenu() : Scene(SceneIds::SceneMenu)
{

}

void SceneMenu::Init()
{
	uiMenu = AddGo(new UiMenu());

	Scene::Init();
}

void SceneMenu::Release()
{


	Scene::Release();
}

void SceneMenu::Enter()
{
	worldView.setSize(windowSize);
	//worldView.setCenter(player->GetPosition());

	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);
	VIEW_MGR.Init();
	VIEW_MGR.SetCurrentSceneWorldView(&worldView);
	VIEW_MGR.SetCurrentSceneUiView(&uiView);

	SOUND_MGR.PlayBgm("sound/bgm/Paris2.ogg");
	SOUND_MGR.SetBgmVolume(0);
	SOUND_MGR.SetSfxVolume(20);

	Scene::Enter();
}

void SceneMenu::Exit()
{


	Scene::Exit();
}

void SceneMenu::Update(float dt)
{
	VIEW_MGR.Update(dt);

	Scene::Update(dt);
}

void SceneMenu::Draw(sf::RenderWindow& window)
{
	VIEW_MGR.DrawBackground();

	Scene::Draw(window);
}
