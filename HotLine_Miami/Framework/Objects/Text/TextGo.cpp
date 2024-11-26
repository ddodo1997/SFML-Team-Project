#include "stdafx.h"
#include "TextGo.h"

TextGo::TextGo(const std::string& fontId, const std::string& name)
	: GameObject(name), fontId(fontId)
{
}

void TextGo::SetOrigin(Origins preset)
{
	originPreset = preset;
	origin = Utils::SetOrigin(text, originPreset);
}

void TextGo::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	text.setOrigin(origin);
}

void TextGo::Reset()
{
	text.setFont(FONT_MGR.Get(fontId));
	SetString(stringId);
}

void TextGo::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	text.setPosition(pos);
}

void TextGo::SetRotation(float angle)
{
	rotation = angle;
	text.setRotation(angle);
}

void TextGo::SetScale(const sf::Vector2f& s)
{
	scale = s;
	text.setScale(scale);
}

void TextGo::Draw(sf::RenderWindow& window)
{
	window.draw(text);
}

void TextGo::OnLocallize(Languages lang)
{
	Variables::currentLang = lang;
	text.setString(STRING_TABLE->Get(stringId));
	SetOrigin(originPreset);
}

void TextGo::Set(int size, const sf::Color &color)
{
	text.setCharacterSize(size);
	text.setFillColor(color);
}

void TextGo::SetString(const std::string& id)
{
	//stringId = id;
	//text.setString(STRING_TABLE->Get(stringId));
	text.setString(id);
	SetOrigin(originPreset);
}

void TextGo::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 0;
}

void TextGo::Release()
{
}

void TextGo::Update(float dt)
{
}
