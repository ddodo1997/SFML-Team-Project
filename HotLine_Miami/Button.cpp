#include "stdafx.h"
#include "Button.h"

Button::Button(const std::string& name)
	: GameObject(name)
{
}

void Button::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	background.setPosition(position);
	text.setPosition(position);
}

void Button::SetRotation(float angle)
{
	rotation = angle;
	background.setRotation(rotation);
	text.setRotation(rotation);
}

void Button::SetScale(const sf::Vector2f& s)
{
	scale = s;
	background.setScale(scale);
	text.setScale(scale);
}

void Button::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(background, originPreset);
		origin = Utils::SetOrigin(text, originPreset);
	}
}

void Button::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	background.setOrigin(origin);
	text.setOrigin(origin);
}

void Button::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 1;
}

void Button::Release()
{
}

void Button::Reset()
{
	float fontSize = 40.f;
	sf::Font& font = FONT_MGR.Get("fonts/DS-DIGI.ttf");

	text.setFont(font);
	text.setCharacterSize(fontSize);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1.f);

	background.setFillColor(sf::Color(150, 150, 150, 150));
	background.setSize({200.f, 80.f});
	background.setOutlineColor(sf::Color::Black);
	background.setOutlineThickness(1.f);
	
	isClicked = false;
	Utils::SetOrigin(text, Origins::MC);
	Utils::SetOrigin(background, Origins::MC);
	SetScale({ 1.f, 1.f });
	SetPosition({ 0.f, 0.f });
}

void Button::Update(float dt)
{
}

void Button::Draw(sf::RenderWindow& window)
{
	window.draw(background);
	window.draw(text);
}

void Button::OnClick(bool clicked)
{
	isClicked = clicked;
	if (isClicked)
	{
		background.setFillColor(sf::Color::Red);
	}
	else
	{
		background.setFillColor(sf::Color(150, 150, 150, 150));
	}
}
