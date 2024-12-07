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
	palmtreeTimer = 0.f;

	worldView.setSize(windowSize);
	worldView.setCenter(windowSize * 0.5f);
	//worldView.setCenter(player->GetPosition());

	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);
	VIEW_MGR.Init();
	VIEW_MGR.SetCurrentSceneWorldView(&worldView);
	VIEW_MGR.SetCurrentSceneUiView(&uiView);

	SOUND_MGR.PlayBgm("sound/bgm/HorseSteppin.ogg");
	SOUND_MGR.SetBgmVolume(10);
	SOUND_MGR.SetSfxVolume(20);
	Scene::Enter();
}

void SceneMenu::Exit()
{
	for (auto bullet : palmtrees)
	{
		RemoveGo(bullet);
		palmtreePool.Return(bullet);
	}
	palmtrees.clear();

	Scene::Exit();
}

void SceneMenu::Update(float dt)
{
	palmtreeTimer += dt;

	VIEW_MGR.Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::Q))
	{
		CreatePalmTree(true);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::E))
	{
		CreatePalmTree(false);
	}

	if (palmtreeTimer > 0.8f)
	{
		palmtreeTimer -= 0.8f;
		CreatePalmTree(true);
		CreatePalmTree(false);
	}



	Scene::Update(dt);
}

void SceneMenu::CreatePalmTree(bool isLeft)
{
	PalmTreeEffect* palmTree = palmtreePool.Take();
	palmTree->OnCreation(isLeft);
	palmtrees.push_back(palmTree);
	AddGo(palmTree);
}

void SceneMenu::Draw(sf::RenderWindow& window)
{
	VIEW_MGR.DrawBackground();

	Scene::Draw(window);
}
