#include "stdafx.h"
#include "ViewMgr.h"


void ViewMgr::Init()
{
	Reset();
}

void ViewMgr::Reset()
{
	auto wSize = FRAMEWORK.GetWindowSizeF();
	worldView.setSize(wSize);
	uiView.setSize(wSize);

	worldView.setCenter(0.f, 0.f);
	uiView.setCenter(wSize / 2.f);
}

void ViewMgr::Update(float dt)
{
	
}

sf::Vector2f ViewMgr::ScreenToWorld(sf::Vector2i screenPos)
{
	return FRAMEWORK.GetWindow().mapPixelToCoords(screenPos, worldView);
}


sf::Vector2i ViewMgr::WorldToScreen(sf::Vector2f worldPos)
{
	return FRAMEWORK.GetWindow().mapCoordsToPixel(worldPos, worldView);
}

sf::Vector2f ViewMgr::ScreenToUi(sf::Vector2i screenPos)
{
	return FRAMEWORK.GetWindow().mapPixelToCoords(screenPos, uiView);
}

sf::Vector2i ViewMgr::UiToScreen(sf::Vector2f worldPos)
{
	return FRAMEWORK.GetWindow().mapCoordsToPixel(worldPos, uiView);
}

void ViewMgr::SetWorldViewSize(sf::Vector2f size)
{
	worldView.setSize(size);
}

sf::Vector2f ViewMgr::GetWorldViewSize()
{
	return worldView.getSize();
}

void ViewMgr::SetWorldViewScale(sf::Vector2f scale)
{
	worldViewScale = scale;
	worldView.setSize({ defaultFHDSize.x * worldViewScale.x, defaultFHDSize.y * worldViewScale.y });
}

void ViewMgr::SetUiViewSize(sf::Vector2f size)
{
	uiView.setSize(size);
}

sf::Vector2f ViewMgr::GetUiViewSize()
{
	return uiView.getSize();
}

