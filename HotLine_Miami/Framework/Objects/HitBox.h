#pragma once

class HitBox
{
private:
	struct Points
	{
		float top;
		float left;
		float bottom;
		float right;
		sf::Vector2f center;
	};

public:
	HitBox();

	Points points;

	sf::RectangleShape rect;

	void UpdateTr(const sf::Transformable& tr, const sf::FloatRect& localBound);
	void Draw(sf::RenderWindow& window);
};