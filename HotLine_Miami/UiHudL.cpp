#include "stdafx.h"
#include "UiHudL.h"

UiHudL::UiHudL(const std::string& name)
	: GameObject(name)
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 0;
}

void UiHudL::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiHudL::SetRotation(float angle)
{
	rotation = angle;
}

void UiHudL::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiHudL::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiHudL::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiHudL::Init()
{
	Reset();

}

void UiHudL::Release()
{
}

void UiHudL::Reset()
{
	mPosX.setFont(FONT_MGR.Get("fonts/DS-DIGI.ttf"));
	mPosX.setCharacterSize(30.f);
	mPosX.setFillColor(sf::Color::White);
	mPosX.setString(std::to_string(InputMgr::GetMousePosition().x));
	mPosX.setPosition({ 30.f,30.f });
	Utils::SetOrigin(mPosX, Origins::TL);

	mPosY.setFont(FONT_MGR.Get("fonts/DS-DIGI.ttf"));
	mPosY.setCharacterSize(30.f);
	mPosY.setFillColor(sf::Color::White);
	mPosY.setString(std::to_string(InputMgr::GetMousePosition().y));
	mPosY.setPosition({ 30.f,80.f });
	Utils::SetOrigin(mPosY, Origins::TL);
}

void UiHudL::Update(float dt)
{
	UpdateMPos(dt);
}

void UiHudL::UpdateMPos(float dt)
{
	mPosX.setString(std::to_string(InputMgr::GetMousePosition().x));
	Utils::SetOrigin(mPosX, Origins::TL);

	mPosY.setString(std::to_string(InputMgr::GetMousePosition().y));
	Utils::SetOrigin(mPosY, Origins::TL);
}

void UiHudL::Draw(sf::RenderWindow& window)
{
	window.draw(mPosX);
	window.draw(mPosY);
}
