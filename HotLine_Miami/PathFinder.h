#pragma once
struct Node
{
	sf::Vector2i position;
	float gCost;
	float hCost;
	float fCost() const { gCost + hCost; }
	Node* parent;
	bool isClosed;

	Node(sf::Vector2i pos = {0, 0}, float g = std::numeric_limits<float>::infinity(), float h = 0)
		: position(pos), gCost(g), hCost(h), parent(nullptr) {}
};

struct CompareNode {
	bool operator()(Node* a, Node* b) {
		return a->fCost() > b->fCost();
	}
};

class PathFinder
{
private:
	std::vector<int> map;
	sf::Vector2i mapSize;
	sf::Vector2f tileSize;
public:
	PathFinder();
	~PathFinder() = default;

	bool IsValid(sf::Vector2i pos);
	bool IsWalkable(sf::Vector2i pos, sf::Vector2i nextPos);
	std::vector<sf::Vector2i> GetNeighborNodes(sf::Vector2i pos);
	std::vector<sf::Vector2f> FindPath(sf::Vector2f startPos, sf::Vector2f targetPos);
	float GetHeuristic(sf::Vector2i start, sf::Vector2i end);
};

