#include "stdafx.h"
#include "Boss2.h"
#include "SceneGame.h"
#include "Player.h"
Boss2::Boss2(const std::string& name)
	: GameObject(name)
{
}

void Boss2::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	panther1.SetPosition({ position.x + -32.f, position.y + -160.f });
	panther2.SetPosition({ position.x + 32.f, position.y + -160.f });
	bodyGuard.SetPosition({ position.x + 64.f, position.y + -250.f });
	mafiaBoss.SetPosition({ position.x, position.y + -230.f });
	fountain1.SetPosition({ position.x + -160.f , position.y });
	fountain2.SetPosition({ position.x + 160.f, position.y });
	awakeBound = { position.x + 0.f,position.y + 0.f, 640.f, 1120.f };
}

void Boss2::SetRotation(float angle)
{
	rotation = angle;
}

void Boss2::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void Boss2::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void Boss2::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void Boss2::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
}

void Boss2::Release()
{
}

void Boss2::Reset()
{
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = sceneGame->GetPlayer();
	 panther1.Reset();
	 panther2.Reset();
	bodyGuard.Reset();
	mafiaBoss.Reset();
	fountain1.Reset();
	fountain2.Reset();
}

void Boss2::SetPlayer()
{
	 panther1.SetPlayer(player);
	 panther2.SetPlayer(player);
	bodyGuard.SetPlayer(player);
	mafiaBoss.SetPlayer(player);
	//fountain1.SetPlayer(player);
	//fountain2.SetPlayer(player);
}

void Boss2::Update(float dt)
{
	panther1.Update(dt);
	panther2.Update(dt);
	bodyGuard.Update(dt);
	mafiaBoss.Update(dt);
	fountain1.Update(dt);
	fountain2.Update(dt);
	switch (currentPhase)
	{
	case Phase::None:
		break;
	case Phase::Panther:
		Phase1(dt);
		break;
	case Phase::BodyGuard:
		Phase2(dt);
		break;
	case Phase::MafiaBoss:
		Phase3(dt);
		break;
	case Phase::Clear:
		break;
	}
}

void Boss2::Phase1(float dt)
{
}

void Boss2::Phase2(float dt)
{
}

void Boss2::Phase3(float dt)
{
}

void Boss2::ChangePhase(Phase phase)
{
	auto prevPhase = currentPhase;
	currentPhase = phase;

	switch (currentPhase)
	{
	case Phase::None:
		break;
	case Phase::Panther:
		break;
	case Phase::BodyGuard:
		break;
	case Phase::MafiaBoss:
		break;
	case Phase::Clear:
		break;
	}
}

void Boss2::Draw(sf::RenderWindow& window)
{
	panther1.Draw(window);
	panther2.Draw(window);
	bodyGuard.Draw(window);
	mafiaBoss.Draw(window);
	fountain1.Draw(window);
	fountain2.Draw(window);
}
