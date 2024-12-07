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
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = sceneGame->GetPlayer();
	panther1.Reset();
	panther2.Reset();
	panther2.SetTimer();
	bodyGuard.Reset();
	mafiaBoss.Reset();
	fountain1.Reset();
	fountain2.Reset();
	ChangePhase(Phase::None);
	SetPlayer();
}

void Boss2::SetPlayer()
{
	panther1.SetPlayer(player);
	panther2.SetPlayer(player);
	bodyGuard.SetPlayer(player);
	bodyGuard.SetMafiaBoss(&mafiaBoss);
	mafiaBoss.SetPlayer(player);
	fountain1.SetPlayer(player);
	fountain1.SetBodyGuard(&bodyGuard);
	fountain1.SetPanthers(&panther1, &panther2);
	fountain2.SetPlayer(player);
	fountain2.SetBodyGuard(&bodyGuard);
	fountain2.SetPanthers(&panther1, &panther2);
}

void Boss2::Update(float dt)
{
	if (player == nullptr)
	{
		return;
	}

	if (player->IsDead())
	{
		sortingOrder = 3;
	}
	if (awakeBound.intersects(player->GetCollisionBox().getGlobalBounds()) && currentPhase == Phase::None)
		ChangePhase(Phase::Panther);
	
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
	if (panther1.IsDead() && panther2.IsDead())
		ChangePhase(Phase::BodyGuard);
	panther1.Phase1(dt);
	panther2.Phase1(dt);
}

void Boss2::Phase2(float dt)
{
	if (bodyGuard.IsDead())
		ChangePhase(Phase::MafiaBoss);
	bodyGuard.Phase2(dt);
}

void Boss2::Phase3(float dt)
{
	mafiaBoss.Phase3(dt);
}

void Boss2::ChangePhase(Phase phase)
{
	auto prevPhase = currentPhase;
	currentPhase = phase;

	switch (currentPhase)
	{
	case Phase::None:
		panther1.SetStatus(Panther::Status::None);
		bodyGuard.SetStatus(BodyGuard::Status::None);
		break;
	case Phase::Panther:
		panther1.Awaken();
		panther2.Awaken();
		break;
	case Phase::BodyGuard:
		bodyGuard.Awaken();
		break;
	case Phase::MafiaBoss:
		mafiaBoss.Awaken();
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
