#include "stdafx.h"
#include "HitBox.h"

HitBox::HitBox()
{
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Green);
	rect.setOutlineThickness(1.f);
}

void HitBox::UpdateTr(const sf::Transformable& tr, const sf::FloatRect& localBounds)
{
	rect.setOutlineColor(sf::Color::Green);
	rect.setSize({ localBounds.width, localBounds.height });
	rect.setOrigin(tr.getOrigin());
	rect.setPosition(tr.getPosition());
	rect.setScale(tr.getScale());
	rect.setRotation(tr.getRotation());
}

void HitBox::Draw(sf::RenderWindow& window)
{
	if (Variables::isDrawHitBox)
		window.draw(rect);
}
