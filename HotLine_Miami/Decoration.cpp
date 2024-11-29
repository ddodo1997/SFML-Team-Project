#include "stdafx.h"
#include "Decoration.h"

Decoration::Decoration(const std::string& name)
	: GameObject(name)
{
}

void Decoration::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Decoration::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Decoration::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Decoration::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Decoration::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Decoration::SetTexture(const std::string& textureId)
{
	this->textureId = textureId;
	body.setTexture(TEXTURE_MGR.Get(this->textureId));
}

void Decoration::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 1;
}

void Decoration::Release()
{
}

void Decoration::Reset()
{
	SetOrigin(Origins::MC);
	SetScale({ 1.f, 1.f });
}

void Decoration::Update(float dt)
{
}

void Decoration::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}
