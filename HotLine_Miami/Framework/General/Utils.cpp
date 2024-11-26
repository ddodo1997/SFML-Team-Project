#define _USE_MATH_DEFINES

#include "stdafx.h"
#include "Utils.h"
#include <cmath>

std::mt19937 Utils::generator;
const float Utils::PI = acosf(-1.f);

void Utils::Init()
{
    std::random_device rd;
    generator.seed(rd());
}

int Utils::RandomRange(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(generator);
}

float Utils::RandomRange(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(generator);
}

float Utils::RandomValue()
{
    return RandomRange(0.f, 1.f);
}

sf::Vector2f Utils::RandomOnUnitCircle()
{
    float angle = RandomRange(0.f, 2.f * PI);
    return sf::Vector2f(std::cosf(angle), std::sinf(angle));
}

sf::Vector2f Utils::RandomOnUnitCircle(float angleLimit)
{
    float angle = RandomRange(PI / 4, 3 * PI / 4); 

    return sf::Vector2f(std::cosf(angle), std::sinf(angle));
}

sf::Vector2f Utils::RandomInUnitCircle()
{
    return RandomOnUnitCircle() * RandomValue();
}

sf::Color Utils::RandomColor(bool opaque)
{
    sf::Uint8 r = RandomRange(0, 255);
    sf::Uint8 g = RandomRange(0, 255);
    sf::Uint8 b = RandomRange(0, 255);
    sf::Uint8 a = opaque ?  255 : RandomRange(0, 255);
    return sf::Color(r, g, b, a);
}

sf::Vector2f Utils::SetOrigin(sf::Transformable& obj, Origins preset, const sf::FloatRect rect)
{
    sf::Vector2f newOrigin(rect.width, rect.height);
    newOrigin.x *= ((int)preset % 3) * 0.5f;
    newOrigin.y *= ((int)preset / 3) * 0.5f;
    obj.setOrigin(newOrigin);
    return newOrigin;
}

sf::Vector2f Utils::SetOrigin(sf::Shape& obj, Origins preset)
{
    return SetOrigin(obj, preset, obj.getLocalBounds());
}

sf::Vector2f Utils::SetOrigin(sf::Text& obj, Origins preset)
{
    return SetOrigin(obj, preset, obj.getLocalBounds());
}

sf::Vector2f Utils::SetOrigin(sf::Sprite& obj, Origins preset)
{
    return SetOrigin(obj, preset, obj.getLocalBounds());
}

float Utils::Clamp(float value, float min, float max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

float Utils::Clamp01(float value)
{
    return Clamp(value, 0.f, 1.f);
}

float Utils::Lerp(float min, float max, float t, bool clamp)
{
    if (clamp)
        t = Clamp01(t);
    return min + (max - min) * t;
}

sf::Vector2f Utils::Lerp(const sf::Vector2f& min, const sf::Vector2f& max, float t, bool clamp)
{
    if (clamp)
        t = Clamp01(t);
    return min + (max - min) * t;
}

sf::Color Utils::Lerp(const sf::Color& min, const sf::Color& max, float t, bool clamp)
{
    if (clamp)
        t = Clamp01(t);

    return sf::Color(
        (sf::Uint8)Lerp(min.r, max.r, t, clamp),
        (sf::Uint8)Lerp(min.g, max.g, t, clamp),
        (sf::Uint8)Lerp(min.b, max.b, t, clamp),
        (sf::Uint8)Lerp(min.a, max.a, t, clamp)
    );
}

float Utils::SqrMagnitude(const sf::Vector2f& vec)
{
    return vec.x * vec.x + vec.y * vec.y;
}

float Utils::Magnitude(const sf::Vector2f& vec)
{
    return std::sqrtf(SqrMagnitude(vec));
}

void Utils::Normailize(sf::Vector2f& vec)
{
    float mag = Magnitude(vec);
    if (mag != 0)
        vec /= mag;
}

sf::Vector2f Utils::GetNormal(const sf::Vector2f& vec)
{
    float mag = Magnitude(vec);
    if (mag == 0)
        return vec;
    return vec / mag;
}

float Utils::Distance(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
    return Magnitude(p2 - p1);
}

float Utils::RadianToDegree(float radian)
{
    return radian * (180.f / PI);
}

float Utils::DegreeToRadian(float degree)
{
    return degree * (PI / 180.f);
}

float Utils::AngleRadian(const sf::Vector2f& vec)
{
    return std::atan2(vec.y, vec.x);
}

float Utils::Angle(const sf::Vector2f& vec)
{
    return RadianToDegree(AngleRadian(vec));
}

float Utils::Dot(const sf::Vector2f& a, const sf::Vector2f& b)
{
    return a.x * b.x + a.y * b.y;
}

bool Utils::CheckCollision(const sf::RectangleShape& shapeA, const sf::RectangleShape& shapeB)
{
    auto pointsA = GetShapePoints(shapeA);
    auto pointsB = GetShapePoints(shapeB);
    return PolygonsIntersect(pointsA, shapeA.getTransform(), pointsB, shapeB.getTransform());
}

bool Utils::CheckCollision(const HitBox& boxA, const HitBox& boxB)
{
    return CheckCollision(boxA.rect, boxB.rect);
}

bool Utils::PointInTransformBounds(const sf::Transformable& transformable, const sf::FloatRect& localBounds, const sf::Vector2f& point)
{
    sf::Transform inverse = transformable.getInverseTransform();
    sf::Vector2f localPoint = inverse.transformPoint(point);
    return localBounds.contains(localPoint);
}

std::vector<sf::Vector2f> Utils::GetShapePoints(const sf::RectangleShape& shape)
{
    sf::FloatRect localBounds = shape.getLocalBounds();
    return GetRectanglePointsFromBounds(localBounds);
}

std::vector<sf::Vector2f> Utils::GetRectanglePointsFromBounds(const sf::FloatRect& localBounds)
{
    std::vector<sf::Vector2f> points(4);
    points[0] = sf::Vector2f(localBounds.left, localBounds.top);
    points[1] = sf::Vector2f(localBounds.left + localBounds.width, localBounds.top);
    points[2] = sf::Vector2f(localBounds.left + localBounds.width, localBounds.top + localBounds.height);
    points[3] = sf::Vector2f(localBounds.left, localBounds.top + localBounds.height);
    return points;
}

bool Utils::PolygonsIntersect(const std::vector<sf::Vector2f>& polygonA, const sf::Transform& transformA, const std::vector<sf::Vector2f>& polygonB, const sf::Transform& transformB)
{
    std::vector<sf::Vector2f> axes;
    int countA = polygonA.size();
    for (int i = 0; i < countA; ++i)
    {
        sf::Vector2f p1 = transformA.transformPoint(polygonA[i]);
        sf::Vector2f p2 = transformA.transformPoint(polygonA[(i + 1) % countA]);
        sf::Vector2f edge = p2 - p1;
        sf::Vector2f normal(-edge.y, edge.x);
        axes.push_back(Utils::GetNormal(normal));
    }

    int countB = polygonB.size();
    for (int i = 0; i < countB; ++i)
    {
        sf::Vector2f p1 = transformB.transformPoint(polygonB[i]);
        sf::Vector2f p2 = transformB.transformPoint(polygonB[(i + 1) % countB]);
        sf::Vector2f edge = p2 - p1;
        sf::Vector2f normal(-edge.y, edge.x);
        axes.push_back(Utils::GetNormal(normal));
    }

    for (const auto& axis : axes)
    {
        float minA = std::numeric_limits<float>::max();
        float maxA = std::numeric_limits<float>::lowest();
        for (const auto& point : polygonA)
        {
            sf::Vector2f transformedPoint = transformA.transformPoint(point);
            float projection = Dot(axis, transformedPoint);
            minA = std::min(minA, projection);
            maxA = std::max(maxA, projection);
        }

        float minB = std::numeric_limits<float>::max();
        float maxB = std::numeric_limits<float>::lowest();
        for (const auto& point : polygonB)
        {
            sf::Vector2f transformedPoint = transformB.transformPoint(point);
            float projection = Dot(axis, transformedPoint);
            minB = std::min(minB, projection);
            maxB = std::max(maxB, projection);
        }

        if (maxA < minB || maxB < minA)
        {
            return false;
        }
    }
    return true;
}

std::vector<sf::Vector2f> Utils::Get6Points(const sf::FloatRect& bounds)
{
    std::vector<sf::Vector2f> result;

    sf::Vector2f topLeft = sf::Vector2f(bounds.left, bounds.top);
    sf::Vector2f topRight = sf::Vector2f(bounds.left + bounds.width, bounds.top);
    sf::Vector2f bottomLeft = sf::Vector2f(bounds.left, bounds.top + bounds.height);
    sf::Vector2f bottomRight = sf::Vector2f(bounds.left + bounds.width, bounds.top + bounds.height);

    float thirdWidth = bounds.width * 0.3333f;
    float halfHeight = bounds.height * 0.5f;

    result.push_back(sf::Vector2f(bounds.left + thirdWidth, bounds.top)); // 왼쪽 3분점 위
    result.push_back(sf::Vector2f(bounds.left + thirdWidth * 2.f, bounds.top)); // 오른쪽 3분점 위
    result.push_back(sf::Vector2f(bounds.left, bounds.top + halfHeight)); // 왼쪽 세로 중점
    result.push_back(sf::Vector2f(bounds.left + bounds.width, bounds.top + halfHeight)); // 오른쪽 세로 중점
    result.push_back(sf::Vector2f(bounds.left + thirdWidth, bounds.top + bounds.height)); // 왼쪽 3분점 아래
    result.push_back(sf::Vector2f(bounds.left + thirdWidth * 2.f, bounds.top + bounds.height)); // 오른쪽 3분점 아래

    //	0 1
    // 2   3
    //	4 5

    return result;
}

std::vector<sf::Vector2f> Utils::GetUpperPoints(const sf::FloatRect& bounds)
{
    std::vector<sf::Vector2f> result;

    sf::Vector2f topLeft = sf::Vector2f(bounds.left, bounds.top);
    sf::Vector2f topRight = sf::Vector2f(bounds.left + bounds.width, bounds.top);

    for (int i = 1; i < 10; i++)
    {
        result.push_back(Utils::Lerp(topLeft, topRight, 0.1 * i));
    }

    return result;
}

sf::Vector2f Utils::FindClosesPoint(const sf::FloatRect& srcBounds, const std::vector<sf::Vector2f>& targetBounds)
{
    sf::Vector2f ballCenter = srcBounds.getPosition() + sf::Vector2f(srcBounds.width / 2.f, srcBounds.height / 2.f);

    float minDistance = std::numeric_limits<float>::max();
    sf::Vector2f closestPoint;

    for (const auto& middle : targetBounds) {
        float dist = Utils::Distance(ballCenter, middle);
        if (dist < minDistance) {
            minDistance = dist;
            closestPoint = middle;
        }
    }

    return closestPoint;
}

sf::Vector2f Utils::GetCenter(const sf::FloatRect& rect) 
{
    sf::Vector2f topLeft = sf::Vector2f(rect.left, rect.top);
    sf::Vector2f bottomRight = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);

    sf::Vector2f center = (topLeft + bottomRight) * 0.5f;

    return center;
}

float Utils::GetRelativePosition(const sf::Vector2f& v1, const sf::Vector2f& v2, const sf::Vector2f& v3)
{
    sf::Vector2f direction = v2 - v1;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    sf::Vector2f toV3 = v3 - v1;
    float projection = (toV3.x * direction.x + toV3.y * direction.y) / length;
    if (projection < 0 || projection > length) {
        return -1.0f;
    }

    return 2.0f * projection / length - 1.0f;
}
