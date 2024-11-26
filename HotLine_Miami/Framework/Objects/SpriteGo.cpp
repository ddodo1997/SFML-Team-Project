#include "stdafx.h"
#include "SpriteGo.h"

SpriteGo::SpriteGo(const std::string& texId, const std::string& name)
	: GameObject(name), textureId(texId)
{
}

void SpriteGo::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(sprite, originPreset);
	}
}

void SpriteGo::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	sprite.setOrigin(origin);
}

void SpriteGo::Reset()
{
	sprite.setTexture(TEXTURE_MGR.Get(textureId));
	SetOrigin(originPreset);
}

void SpriteGo::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	sprite.setPosition(pos);
}

void SpriteGo::SetRotation(float angle)
{
	rotation = angle;
	sprite.setRotation(angle);
}

void SpriteGo::SetScale(const sf::Vector2f& s)
{
	scale = s;
	sprite.setScale(scale);
}

void SpriteGo::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void SpriteGo::Init()
{
}

void SpriteGo::Release()
{
}

void SpriteGo::Update(float dt)
{
}
