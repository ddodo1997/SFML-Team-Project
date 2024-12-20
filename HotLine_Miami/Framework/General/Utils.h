#pragma once
class GameObject;
class Utils
{
private:
	static std::mt19937 generator;

public:
	static const float PI;

	static void Init();

	static int RandomRange(int min, int max);
	static float RandomRange(float min, float max);
	static float RandomValue();	// 0.0f ~ 1.0f
	static sf::Vector2f RandomOnUnitCircle();
	static sf::Vector2f RandomOnUnitCircle(float angleLimit);
	static sf::Vector2f RandomInUnitCircle();
	static sf::Color RandomColor(bool opaque = true);

	// Origin
	static sf::Vector2f SetOrigin(sf::Transformable &obj, Origins preset, const sf::FloatRect bound);
	static sf::Vector2f SetOrigin(sf::Shape& obj, Origins preset);
	static sf::Vector2f SetOrigin(sf::Text& obj, Origins preset);
	static sf::Vector2f SetOrigin(sf::Sprite& obj, Origins preset);
	static sf::Vector2f SetOrigin5SQ(sf::Transformable& obj, Origin5SQ preset, const sf::FloatRect bound);
	static sf::Vector2f SetOrigin5SQ(sf::Shape& obj, Origin5SQ preset);
	static sf::Vector2f SetOrigin5SQ(sf::Text& obj, Origin5SQ preset);
	static sf::Vector2f SetOrigin5SQ(sf::Sprite& obj, Origin5SQ preset);

	// Math
	static float Clamp(float value, float min, float max);
	static float Clamp01(float value);

	static float Lerp(float min, float max, float t, bool clamp = true);
	static sf::Vector2f Lerp(const sf::Vector2f& min, const sf::Vector2f& max, float t, bool clamp = true);
	static sf::Color Lerp(const sf::Color& min, const sf::Color& max, float t, bool clamp = true);

	static float SqrMagnitude(const sf::Vector2f& vec);
	static float Magnitude(const sf::Vector2f& vec);

	static void Normalize(sf::Vector2f& vec);
	static sf::Vector2f GetNormal(const sf::Vector2f& vec);
	static float Distance(const sf::Vector2f& p1, const sf::Vector2f& p2);

	static float RadianToDegree(float radian);
	static float DegreeToRadian(float degree);

	static float AngleRadian(const sf::Vector2f& vec);
	static float Angle(const sf::Vector2f& vec);
	static sf::Vector2f DegreeToNormal(float degree);
	static sf::Vector2f RadianToNormal(float degree);

	static sf::Vector2f AngleSpread(sf::Vector2f dir, float degree = 0);

	static float Dot(const sf::Vector2f& a, const sf::Vector2f& b);

	//사각형 충돌
	static bool CheckCollision(const sf::RectangleShape& shapeA, const sf::RectangleShape& shapeB);
	static bool CheckCollision(const HitBox& boxA, const HitBox& boxB);
	static std::vector<sf::Vector2f> GetShapePoints(const sf::RectangleShape& shape);
	
	static bool PointInTransformBounds(const sf::Transformable& transformable, const sf::FloatRect& localBounds, const sf::Vector2f& point);
	static std::vector<sf::Vector2f> GetRectanglePointsFromBounds(const sf::FloatRect& localBounds);
	static bool PolygonsIntersect(const std::vector<sf::Vector2f>& polygonA, const sf::Transform& transformA, const std::vector<sf::Vector2f>& polygonB, const sf::Transform& transformB);

	static std::vector<sf::Vector2f> GetCenterPoints(const sf::FloatRect& bounds);
	static std::vector<sf::Vector2f> Get6Points(const sf::FloatRect& bounds);
	static std::vector<sf::Vector2f> GetUpperPoints(const sf::FloatRect& bounds);

	static sf::Vector2f FindClosesPoint(const sf::FloatRect& srcBounds, const std::vector<sf::Vector2f>& targetPoints);
	static sf::Vector2f FindClosesPoint(const sf::Vector2f& srcBounds, const std::vector<sf::Vector2f>& targetBounds);
	static sf::Vector2f GetCenter(const sf::FloatRect& rect);
	static float GetRelativePosition(const sf::Vector2f& v1, const sf::Vector2f& v2, const sf::Vector2f& v3);

	static sf::Vector2f GetTransratedPoint(const sf::Vector2f& origin, const sf::Vector2f& direction, float maxDistance);
	static bool RayCast(const sf::Vector2f& origin, const sf::Vector2f& direction, float maxDistance, GameObject* target);

	// 이미지 내 Pixel 감지
	static sf::Vector2i FindPixelByColor(const sf::Sprite& sprite, bool fromCenter = false);
};
