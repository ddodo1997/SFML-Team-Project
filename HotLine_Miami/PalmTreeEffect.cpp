#include "stdafx.h"
#include "PalmTreeEffect.h"

PalmTreeEffect::PalmTreeEffect(const std::string& name)
	: GameObject(name)
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = -1;
}

void PalmTreeEffect::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	treeSprite.setPosition(position);
}

void PalmTreeEffect::SetRotation(float angle)
{
	rotation = angle;
	// ¾È¾¸
}

void PalmTreeEffect::SetScale(const sf::Vector2f& s)
{
	scale = s;
	treeSprite.setScale(scale);
}

void PalmTreeEffect::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(treeSprite, originPreset);
	}
}

void PalmTreeEffect::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
}

void PalmTreeEffect::OnCreation(bool isLeft)
{
	this->isLeft = isLeft;
	timer = 0.f;
	SetActive(true);
	
	auto halfViewSize = VIEW_MGR.GetUiViewSize()*0.5f;

	if (isLeft)
	{
		spriteType = Utils::RandomRange(4, 7);
		SetPosition({ halfViewSize.x - 50.f, halfViewSize.y + 180.f });
		speed.x = -10.f;
		acceleration.x = -200.f;
	}
	else
	{
		spriteType = Utils::RandomRange(0, 3);
		SetPosition({ halfViewSize.x + 50.f, halfViewSize.y + 180.f });
		speed.x = 10.f;
		acceleration.x = 200.f;
	}
	textureId = "graphics/ETC/MainMenuScene/sprPalmTrees_" + std::to_string(spriteType) + ".png";
	treeSprite.setTexture(TEXTURE_MGR.Get(textureId), true);
	SetScale({ 0.2f, 0.2f });

	if (isLeft)
	{
		Utils::SetOrigin5SQ(treeSprite, Origin5SQ::o33);
	}
	else
	{
		Utils::SetOrigin5SQ(treeSprite, Origin5SQ::o13);
	}
}

void PalmTreeEffect::Init()
{

}

void PalmTreeEffect::Release()
{

}

void PalmTreeEffect::Reset()
{

}

void PalmTreeEffect::Update(float dt)
{
	timer += dt;
	SetPosition(position + speed * dt);
	speed += acceleration * dt;

	sf::Vector2f timerPair = { timer ,timer};
	SetScale(scale + timerPair * dt);

	if (timer > 4.f)
		SetActive(false);
}

void PalmTreeEffect::Draw(sf::RenderWindow& window)
{
	window.draw(treeSprite);
}
