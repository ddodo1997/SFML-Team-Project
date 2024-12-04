#include "stdafx.h"
#include "WayPoint.h"

WayPoint::WayPoint(const std::string& name)
	: GameObject(name)
{
}

void WayPoint::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	waypointShape.setPosition(position);
}

void WayPoint::SetRotation(float angle)
{
	rotation = angle;
	waypointShape.setRotation(rotation);
}

void WayPoint::SetScale(const sf::Vector2f& s)
{
	scale = s;
	waypointShape.setScale(scale);
}

void WayPoint::SetColor(const sf::Color& color)
{
	waypointShape.setFillColor(color);
}

void WayPoint::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void WayPoint::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void WayPoint::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = -1;
}

void WayPoint::Release()
{
}

void WayPoint::Reset()
{
	radius = 6.f;
	waypointShape.setRadius(radius);
	waypointShape.setFillColor(sf::Color::Red);
	waypointShape.setOrigin(radius, radius);
	SetPosition({ 0.f, 0.f });
}

void WayPoint::Update(float dt)
{
}

void WayPoint::Draw(sf::RenderWindow& window)
{
	window.draw(waypointShape);
}
